#include<iostream>
#include<vector>
#include<cmath>
#include<chrono>
#include<random>
#include "lodepng.h"
#include "kmeans.h"

using std::cout;
using std::endl;
using std::uniform_int_distribution;

kmeans::kmeans() { }

// Load image from file and create the kmeans object
kmeans::kmeans(const char* filename, int clusterNum):clusterNum(clusterNum) {
    unsigned error = lodepng::decode(imageData, width, height, filename);
    if(error) cout << "decoder error " << error << ": " << lodepng_error_text(error) << endl;
    newImageData = imageData;
    centroidData = vector<double>(clusterNum * 4, 0);
    clusterAssignments = vector<int>(width * height, 0);
    this->initializeCentroids();
}

kmeans::~kmeans() { }

// Initialize centroids with K-means++
// Not suitable for generic algorithms because it is a bog standard for loop
void kmeans::initializeCentroids() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    vector<long long> distances(width*height, 1);
    vector<int> centroidIndex(clusterNum, 0);
    long long total = width*height;

    // Iterate over number of clusters
    for (int i = 0; i < clusterNum; ++i) {
        // First centroid is chosen with uniform probability
        uniform_int_distribution<long long> distribution(1,total);
        long long randomNum = distribution(generator);
        long long temp = 0;
        int newCentroid = 0;

        // Choose centroid according to random number
        for (size_t j = 0; j < width*height; ++j) {
            temp += distances[j];
            if (randomNum >= temp) {
                newCentroid = j;
            }
            else {
                break;
            }
        }
        centroidIndex[i] = newCentroid;

        // Generate probability distribution for the next centroid based on squared two-norm
        for (size_t j = 0; j < width*height; ++j) {
            int dist = calculateSquaredDistanceOther(j, centroidIndex[0]);
            for (int k = 1; k < i + 1; ++k) {
                if (calculateSquaredDistanceOther(j, centroidIndex[k]) < dist) {
                    dist = calculateSquaredDistanceOther(j, centroidIndex[k]);
                }
            }
            distances[j] = dist;
        }
        total = 0;
        for (auto x : distances) {
            total += x;
        }
    }

    // Write in centroid data
    for (int i = 0; i < clusterNum; ++i) {
        for (int j = 0; j < 4; ++j) {
            centroidData[4*i + j] = (double)imageData[4*centroidIndex[i]+j];
        }
    }
}

// Use clusterAssignments and centroid data to assign colors to pixels
void kmeans::generateImage() {
    for (size_t i = 0; i < width*height; ++i) {
        for (int j = 0; j < 4; ++j) {
            newImageData[4*i + j] = (unsigned char)centroidData[4*clusterAssignments[i]+j];
        }
    }
}

// Write the processed image to disk
void kmeans::writePNG(const char* filename) {
    unsigned error = lodepng::encode(filename, newImageData, width, height);
    if(error) cout << "encoder error " << error << ": "<< lodepng_error_text(error) << endl;
}

const unsigned char* kmeans::getString() {
    return reinterpret_cast<const unsigned char*>(newImageData.data());
}

int kmeans::getWidth() {
    return width;
}

int kmeans::getHeight() {
    return height;
}

int kmeans::getIteration() {
    return iterNum;
}

bool kmeans::isConverged() {
    return converged;
}

// Calculates the squared two-norm of the distance between two color values
// Returns double when dealing with centroids, which may not hold int values
double kmeans::calculateSquaredDistance(int imageNum, int centroidNum) {
    double sum = 0;
    for (int i = 0; i < 4; ++i) {
        double temp = (double)imageData[4*imageNum+i]-centroidData[4*centroidNum+i];
        temp *= temp;
        sum += temp;
    }
    return sum;
}

// Calculates the squared two-norm of the distance between two color values
// Returns int for tasks that guarentee int returns
int kmeans::calculateSquaredDistanceOther(int imageNum, int centroidNum) {
    int sum = 0;
    for (int i = 0; i < 4; ++i) {
        int temp = imageData[4*imageNum+i]-imageData[4*centroidNum+i];
        temp *= temp;
        sum += temp;
    }
    return sum;
}

// Assigns pixels to a centroid based on two-norm distance
int kmeans::classifyPoint(int a) {
    int cluster = 0;
    double minDistance = calculateSquaredDistance(a, 0);
    for (int i = 1; i < clusterNum; ++i) {
        double temp = calculateSquaredDistance(a, i);
        if (temp < minDistance) {
            cluster = i;
            minDistance = temp;
        }
    }
    return cluster;
}

// Generates centroids based on average location of points assigned to its class
void kmeans::calculateCentroids() {
    vector<int> tally(clusterNum, 0);
    centroidData = vector<double>(clusterNum*4, 0);
    for (size_t i = 0; i < width*height; ++i) {
        tally[clusterAssignments[i]] += 1;
        for (int j = 0; j < 4; ++j) {
            centroidData[4*clusterAssignments[i]+j] += imageData[4*i+j];
        }
    }
    for (int i = 0; i < clusterNum; ++i) {
        for (int j = 0; j < 4; ++j) {
            centroidData[4*i+j] /= tally[i];
        }
    }
}

// Used to run the above classifyPoint in parallel
void kmeans::classifyPoints() {
    vector<int> compare = clusterAssignments;
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, width*height, 512), kmeans::pointClassifier(*this));
    if (clusterAssignments == compare) {
        converged = true;
    }
}

// Aggregates functions to generate the next iteration
int kmeans::nextIteration() {
    ++iterNum;
    this->classifyPoints();
    this->calculateCentroids();
    this->generateImage();
    return iterNum;
}

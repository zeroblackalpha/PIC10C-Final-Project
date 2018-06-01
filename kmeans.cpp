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

kmeans::kmeans(const char* filename, int clusterNum):clusterNum(clusterNum) {
    unsigned error = lodepng::decode(imageData, width, height, filename);
    if(error) cout << "decoder error " << error << ": " << lodepng_error_text(error) << endl;
    newImageData = imageData;
    centroidData = vector<double>(clusterNum * 4, 0);
    clusterAssignments = vector<int>(width * height, 0);
}

kmeans::~kmeans() { }

void kmeans::initializeCentroids() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    vector<long long> distances(width*height, 1);
    vector<int> centroidIndex(clusterNum, 0);
    long long total = width*height;
    for (int i = 0; i < clusterNum; ++i) {
        uniform_int_distribution<long long> distribution(1,total);
        long long randomNum = distribution(generator);
        long long temp = 0;
        int newCentroid = 0;
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
    for (int i = 0; i < clusterNum; ++i) {
        for (int j = 0; j < 4; ++j) {
            centroidData[4*i + j] = (double)imageData[4*centroidIndex[i]+j];
        }
    }
}

void kmeans::generateImage() {
    for (size_t i = 0; i < width*height; ++i) {
        for (int j = 0; j < 4; ++j) {
            newImageData[4*i + j] = (unsigned char)centroidData[4*clusterAssignments[i]+j];
        }
    }
}

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

double kmeans::calculateSquaredDistance(int imageNum, int centroidNum) {
    double sum = 0;
    for (int i = 0; i < 4; ++i) {
        double temp = (double)imageData[4*imageNum+i]-centroidData[4*centroidNum+i];
        temp *= temp;
        sum += temp;
    }
    return sum;
}

int kmeans::calculateSquaredDistanceOther(int imageNum, int centroidNum) {
    int sum = 0;
    for (int i = 0; i < 4; ++i) {
        int temp = imageData[4*imageNum+i]-imageData[4*centroidNum+i];
        temp *= temp;
        sum += temp;
    }
    return sum;
}

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

void kmeans::classifyPoints() {
    vector<int> compare = clusterAssignments;
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, width*height, 512), pointClassifier(*this));
    if (clusterAssignments == compare) {
        converged = true;
    }
}

int kmeans::nextIteration() {
    ++iterNum;
    this->classifyPoints();
    this->calculateCentroids();
    this->generateImage();
    return iterNum;
}

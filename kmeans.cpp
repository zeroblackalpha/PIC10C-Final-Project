#include<iostream>
#include<vector>
#include<numeric>
#include<cmath>
#include<chrono>
#include<random>
#include "lodepng.h"
#include "kmeans.h"

using std::cout;
using std::endl;
using std::inner_product;
using std::pow;
using std::plus;
using std::uniform_int_distribution;

kmeans::kmeans(const char* filename, int clusterNum):clusterNum(clusterNum) {
    unsigned error = lodepng::decode(imageData, width, height, filename);
    if(error) cout << "decoder error " << error << ": " << lodepng_error_text(error) << endl;
    newImageData = imageData;
    centroidData = vector<unsigned char>(clusterNum * 4, 0);
    clusterAssignments = vector<int>(width * height, 0);
    this->initializeCentroids();
}

void kmeans::initializeCentroids() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    uniform_int_distribution<int> distribution(0,clusterNum - 1);
    for (int i = 0; i < width*height; ++i){
        clusterAssignments[i] = distribution(generator);
    }
    this->calculateCentroids();
}

void kmeans::generateImage() {
    for (int i = 0; i < width*height; ++i) {
        for (int j = 0; j < 4; ++j) {
            newImageData[4*i + j] = (unsigned char)centroidData[4*clusterAssignments[i]+j];
        }
    }
}

void kmeans::writePNG(const char* filename) {
    unsigned error = lodepng::encode(filename, newImageData, width, height);
    if(error) cout << "encoder error " << error << ": "<< lodepng_error_text(error) << endl;
}

bool kmeans::isConverged() {
    return converged; //PLZ fix
}

int kmeans::calculateSquaredDistance(int imageNum, int centroidNum) {
    int sum = 0;
    for (int i = 0; i < 4; ++i) {
        int temp = (int)newImageData[4*imageNum+i]-(int)centroidData[4*centroidNum+i];
        temp *= temp;
        sum += temp;
    }
}

int kmeans::classifyPoint(int a) {
    int cluster = 0;
    int minDistance = calculateSquaredDistance(a*4, 0);
    for (int i = 1; i < clusterNum; ++i) {
        int temp = calculateSquaredDistance(a*4, i*4);
        if (temp < minDistance) {
            cluster = i;
            minDistance = temp;
            std::cout << cluster;
        }
    }
    return cluster;
}

void kmeans::calculateCentroids() {
    vector<int> tally(clusterNum, 0);
    vector<int> centroidPoints(clusterNum*4, 0);
    for (int i = 0; i < width*height; ++i) {
        tally[clusterAssignments[i]] += 1;
        for (int j = 0; j < 4; ++j) {
            centroidPoints[4*clusterAssignments[i]+j] += newImageData[4*i+j];
        }
    }
    for (int i = 0; i < clusterNum; ++i) {
        for (int j = 0; j < 4; ++j) {
            centroidData[4*i+j] = centroidPoints[4*i+j] / tally[i];
        }
    }
}

void kmeans::nextIteration() {
    std::cout << "step 2\n";
    for (int i = 0; i < width*height; ++i) {
        clusterAssignments[i] = classifyPoint(i);
    }
    this->calculateCentroids();
    std::cout << "step 2\n";
    this->generateImage();
    std::cout << "step 2\n";
}
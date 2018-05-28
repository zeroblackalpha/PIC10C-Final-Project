#ifndef KMEANS_H
#define KMEANS_H

#include <vector>

using std::vector;

class kmeans {
public:
    kmeans() = delete;
    kmeans(const char* filename, int clusterNum);
    void writePNG(const char* filename);
    void nextIteration();
    bool isConverged();

private:
    double calculateSquaredDistance(int imageNum, int centroidNum);
    int calculateSquaredDistanceOther(int imageNum, int centroidNum);
    int classifyPoint(int a);
    void classifyPoints();
    void initializeCentroids();
    void calculateCentroids();
    void generateImage();

    vector<unsigned char> imageData;
    vector<unsigned char> newImageData;    
    vector<double> centroidData;
    vector<int> clusterAssignments;
    int clusterNum;
    int iterNum = 0;
    unsigned width;
    unsigned height;
    bool converged = false;
};

#endif // KMEANS_H
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
    int calculateSquaredDistance(int imageNum, int centroidNum);
    int classifyPoint(int a);
    void initializeCentroids();
    void calculateCentroids();
    void generateImage();

    vector<unsigned char> imageData;
    vector<unsigned char> newImageData;    
    vector<unsigned char> centroidData;
    vector<int> clusterAssignments;
    int clusterNum;
    unsigned width;
    unsigned height;
    bool converged = false;
};

#endif // KMEANS_H
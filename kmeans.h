#ifndef KMEANS_H
#define KMEANS_H

#include <vector>

using std::vector;

class kmeans {
public:
    kmeans();
    kmeans(const char* filename, int clusterNum);
    ~kmeans();
    const unsigned char* getString();
    int getWidth();
    int getHeight();
    void writePNG(const char* filename);
    int nextIteration();
    bool isConverged();
    void initializeCentroids();

private:
    double calculateSquaredDistance(int imageNum, int centroidNum);
    int calculateSquaredDistanceOther(int imageNum, int centroidNum);
    int classifyPoint(int a);
    void classifyPoints();
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

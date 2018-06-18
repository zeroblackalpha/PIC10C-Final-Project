#ifndef KMEANS_H
#define KMEANS_H

#include <vector>
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"

using std::vector;

class kmeans {

friend class pointClassifier;
public:
// Used in parallelization
class pointClassifier {
    public:
        void operator() ( const tbb::blocked_range<std::size_t>& r ) const {
            for (std::size_t i = r.begin(); i != r.end(); ++i) {
                picture.clusterAssignments[i] = picture.classifyPoint(i);
            }
        }

        pointClassifier(kmeans &p) : picture(p) { }

    private:
        kmeans& picture;
    };

public:
    kmeans();
    kmeans(const char* filename, int clusterNum);
    ~kmeans();
    const unsigned char* getString();
    int getIteration();
    int getWidth();
    int getHeight();
    void writePNG(const char* filename);
    int nextIteration();
    bool isConverged();

private:
    void initializeCentroids();
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

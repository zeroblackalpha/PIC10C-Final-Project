#include<iostream>
#include "kmeans.h"

int main() {
    const char* filename = "test.png";
    const char* filename1 = "test1.png";
    kmeans image = kmeans(filename, 10);
    while (!image.isConverged()) {
        image.nextIteration();
    }
    image.writePNG(filename1);
    system("pause");
}

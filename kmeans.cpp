#include<iostream>
#include<vector>
#include "lodepng.h"

void decodeOneStep(const char* filename) {
    std::vector<unsigned char> image;
    unsigned width, height;
    //Decodes the data into the vector "image", 4 bytes per pixel, ordered RGBARGBA
    unsigned error = lodepng::decode(image, width, height, filename);
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
    //Encodes the image
    unsigned error = lodepng::encode(filename, image, width, height);
    if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
    //NOTE: this function will overwrite the file without warning!
}

int main() {
    const char* filename = "test.png";
    decodeOneStep(filename);
    

    //generate some image
    unsigned width = 512, height = 512;
    std::vector<unsigned char> image;
    image.resize(width * height * 4);
    for(unsigned y = 0; y < height; y++)
    for(unsigned x = 0; x < width; x++)
    {
        image[4 * width * y + 4 * x + 0] = 255 * !(x & y);
        image[4 * width * y + 4 * x + 1] = x ^ y;
        image[4 * width * y + 4 * x + 2] = x | y;
        image[4 * width * y + 4 * x + 3] = 255;
    }

  encodeOneStep(filename, image, width, height);
}
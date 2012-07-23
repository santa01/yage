#include <cstdlib>

#include "Image.h"

Image::Image() {
    this->imageSize = 0;
    this->imageData = NULL;
}

Image::~Image() {
    if (this->imageData) {
        delete[] this->imageData;
    }
}

#include <memory.h>
#include <fstream>
#include <string>

#include "TGAImage.h"
#include "Logger.h"

TGAImage::TGAImage() {
    this->format = GL_BGRA;
    memset(&this->header, 0, sizeof(this->header));
}

TGAImage::TGAImage(const TGAImage& orig) {
    this->format = orig.format;
    this->header = orig.header;
    
    if (orig.imageData != NULL) {
        this->imageData = new char[orig.imageSize];
        memcpy(&this->imageData, &orig.imageData, orig.imageSize);
    }
}

TGAImage::~TGAImage() {
}

TGAImage& TGAImage::operator =(const TGAImage& orig) {
    this->format = orig.format;
    this->header = orig.header;
    
    if (this->imageData != NULL) {
        delete[] this->imageData;
    }
    
    this->imageData = new char[orig.imageSize];
    memcpy(&this->imageData, &orig.imageData, orig.imageSize);
    
    return *this;
}

bool TGAImage::load(const std::string& name) {
    std::fstream file(name.c_str(), std::ios::binary | std::ios::in);
    if (!file.good()) {
        Logger::getInstance().log(Logger::LOG_ERROR, "Cannot open file %s", name.c_str());
        return false;
    }

    TGAFooter footer;
    file.seekg(-sizeof(footer), std::ios::end);
    file.read((char*)&footer, sizeof(footer));
    if (memcmp((char*)footer.signature, "TRUEVISION-XFILE", sizeof(footer.signature))) {
        Logger::getInstance().log(Logger::LOG_ERROR, "%s is not valid TGA file", name.c_str());
        return false;
    }

    file.seekg(0, std::ios::beg);
    file.read((char*)&this->header, sizeof(this->header));
    if (this->header.imageType != TGA_IMAGE_UNCOMPRESSED_TRUECOLOR) {    // TODO
        return false;
    }

    file.seekg(0, std::ios::end);
    int fileLength = file.tellg();
    int dataOffset = sizeof(TGAHeader) +
                     this->header.idLength +
                     this->header.colorMapSpec.colorMapLength *
                     this->header.colorMapSpec.colorMapEntrySize;
    int dataLength = (footer.developerDirOffset ? footer.developerDirOffset :
                     (footer.extentionAreaOffset ? footer.extentionAreaOffset :
                     fileLength - sizeof(footer))) - dataOffset;
    
    if (this->imageData != NULL) {
        delete[] this->imageData;
    }
    
    this->imageData = new char[dataLength];
    file.seekg(dataOffset, std::ios::beg);
    file.read(this->imageData, dataLength);
    
    return true;
}

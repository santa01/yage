#include <memory.h>
#include <fstream>
#include <string>

#include "TGAImage.h"
#include "Logger.h"

TGAImage::TGAImage() {
    memset(&this->header, 0, sizeof(this->header));
}

TGAImage::TGAImage(const TGAImage& orig) {
    this->format = orig.format;
    this->header = orig.header;
    
    if (orig.imageData != NULL) {
        this->imageDataSize = orig.imageDataSize;
        this->imageData = new char[this->imageDataSize];
        memcpy(&this->imageData, &orig.imageData, this->imageDataSize);
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
    
    this->imageDataSize = orig.imageDataSize;
    this->imageData = new char[this->imageDataSize];
    memcpy(&this->imageData, &orig.imageData, this->imageDataSize);
    
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
    if (this->header.imageType != TGA_IMAGE_UNCOMPRESSED_TRUECOLOR) {  // TODO: Implement RLE decoding
        return false;
    }
    
    switch (this->header.imageSpec.depth) {
        case 32:
            this->format = GL_BGRA;
            break;
            
        case 24:
            this->format = GL_BGR;
            break;
            
        default:
            return false;  // TODO: handle other formats
    }

    file.seekg(0, std::ios::end);
    int fileLength = file.tellg();
    int dataOffset = sizeof(TGAHeader) +
                     this->header.idLength +
                     this->header.colorMapSpec.colorMapLength *
                     this->header.colorMapSpec.colorMapEntrySize;
    this->imageDataSize = (footer.developerDirOffset ? footer.developerDirOffset :
                          (footer.extentionAreaOffset ? footer.extentionAreaOffset :
                          fileLength - sizeof(footer))) - dataOffset;
    
    if (this->imageData != NULL) {
        delete[] this->imageData;
    }
    
    this->imageData = new char[this->imageDataSize];
    file.seekg(dataOffset, std::ios::beg);
    file.read(this->imageData, this->imageDataSize);
    file.close();
    
    return true;
}

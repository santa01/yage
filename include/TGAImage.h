/* Reduced functionality, consider use of TGA_IMAGE_UNCOMPRESSED_TRUECOLOR image types! */

#ifndef TGAIMAGE_H
#define	TGAIMAGE_H

#include <global.h>
#include <stdint.h>
#include <string>

#include "Image.h"

enum {
    TGA_COLORMAP_NOT_INCLUDED,
    TGA_COLORMAP_INCLUDED
};

enum {
    TGA_IMAGE_NODATA,
    TGA_IMAGE_UNCOMPRESSED_COLORMAPPED,
    TGA_IMAGE_UNCOMPRESSED_TRUECOLOR,
    TGA_IMAGE_UNCOMPRESSED_BLACKWHITE,
    TGA_RLE_COLORMAPPED = 9,
    TGA_RLE_TRUECOLOR,
    TGA_RLE_BLACKWHITE
};

#pragma pack(push, 1)

typedef struct {
    uint16_t firstEntryIndex;
    uint16_t colorMapLength;
    uint8_t colorMapEntrySize;
} ColorMapSpec;

typedef struct {
    uint8_t attrBitPerPixel: 4;
    uint8_t transferOrder: 2;
    uint8_t compat: 2;
} ImageDescr;

typedef struct {
    uint16_t xOrigin;
    uint16_t yOrigin;
    uint16_t width;
    uint16_t height;
    uint8_t depth;
    ImageDescr imageDescr;
} ImageSpec;

typedef struct {
    uint8_t idLength;
    uint8_t colorMapType;
    uint8_t imageType;
    ColorMapSpec colorMapSpec;
    ImageSpec imageSpec;
} TGAHeader;

typedef struct {
    uint32_t extentionAreaOffset;
    uint32_t developerDirOffset;
    uint8_t signature[16];          // Should be "TRUEVISION-XFILE"
    uint8_t reservedChar;           // Should be '.'
    uint8_t terminator;             // Should be '\0'
} TGAFooter;

#pragma pack(pop)

class TGAImage: public Image {
    public:
        TGAImage();
        TGAImage(const TGAImage& orig);
        ~TGAImage();
        
        TGAImage& operator =(const TGAImage& orig);
        
        int getWidth() const  {
            return this->header.imageSpec.width;
        }
        
        int getHeight() const {
            return this->header.imageSpec.height;
        }
        
        bool load(const std::string& name);

    private:
        TGAHeader header;
};

#endif	/* TGAIMAGE_H */

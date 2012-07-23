#ifndef IMAGE_H
#define	IMAGE_H

#include <cstdlib>

class Image {
    public:
        Image();
        virtual ~Image();
        
        bool isNull() const {
            return (this->imageData == NULL);
        }
        
        virtual int getWidth() const = 0;
        virtual int getHeight() const = 0;
        
        int getFormat() const {
            return this->format;
        }
        
        const char* data() const {
            return this->imageData;
        }

    protected:
        int format;
        int imageSize;
        char* imageData;
};

#endif	/* IMAGE_H */

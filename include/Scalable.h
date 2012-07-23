#ifndef SCALABLE_H
#define	SCALABLE_H

class Scalable {
    public:
        virtual ~Scalable();

        virtual void scaleX(float factor) = 0;
        virtual void scaleY(float factor) = 0;
        virtual void scaleZ(float factor) = 0;
        
        virtual float getXFactor() const = 0;
        virtual float getYFactor() const = 0;
        virtual float getZFactor() const = 0;
        
        virtual void scale(float factor) = 0;
};

#endif	/* SCALABLE_H */

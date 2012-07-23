#ifndef QUATERNION_H
#define	QUATERNION_H

#include "Vec3.h"
#include "Mat4.h"

class Quaternion {
    public:
        static const int X = 0;
        static const int Y = 1;
        static const int Z = 2;
        static const int W = 3;
        
        Quaternion();
        Quaternion(float x, float y, float z, float w);
        Quaternion(const Vec3& axis, float angle);
        
        Quaternion operator *(const Quaternion& quaternmion) const;
        
        void normalize();
        float length() const;
        
        float get(int index) const;
        void set(int index, float value);
        
        Mat4 extractMat4() const;

    private:
        float vector[4];
};

#endif	/* QUATERNION_H */

#ifndef VEC4_H
#define	VEC4_H

#include "Vec3.h"

class Vec4 {
    public:
        static const int X = 0;
        static const int Y = 1;
        static const int Z = 2;
        static const int W = 3;

        Vec4();
        Vec4(float x, float y, float z, float w);

        Vec4 operator -(const Vec4& vec) const;
        Vec4 operator +(const Vec4& vec) const;
        Vec4 operator *(float scalar) const;
        Vec4& operator -=(const Vec4& vec);
        Vec4& operator +=(const Vec4& vec);
        Vec4& operator *=(float scalar);
        bool operator ==(const Vec4& vector) const;
        bool operator !=(const Vec4& vector) const;
        Vec4 operator -() const;
        
        float dot(const Vec4& vec) const;

        float get(int index) const;
        void set(int index, float value);
        
        const float* data() const  {
            return (float*)&this->vector;
        }
        
        Vec3 extractVec3() const;

    private:
        float vector[4];
};

#endif	/* VEC4_H */

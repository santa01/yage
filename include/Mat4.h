#ifndef MAT4_H
#define	MAT4_H

#include "Vec4.h"
#include "Mat3.h"

class Mat4 {
    public:
        Mat4();

        Mat4 operator *(const Mat4& mat) const;
        Vec4 operator *(const Vec4& vec) const;
        Mat4 operator *(float scalar) const;
        Mat4 operator +(const Mat4& matrix) const;
        Mat4 operator -(const Mat4& matrix) const;
        bool operator ==(const Mat4& matrix) const;
        bool operator !=(const Mat4& matrix) const;

        void transpose();

        float get(int row, int column) const;
        void set(int row, int column, float value);
        
        const float* data() const  {
            return (float*)&this->matrix;
        }
        
        Mat3 extractMat3() const;

    private:
        float matrix[4][4];
};

#endif	/* MAT4_H */

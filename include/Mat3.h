#ifndef MAT3_H
#define	MAT3_H

#include "Vec3.h"

class Mat3 {
    public:
        Mat3();

        Mat3 operator *(const Mat3& matrix) const;
        Vec3 operator *(const Vec3& vector) const;
        Mat3 operator *(float scalar) const;
        Mat3 operator +(const Mat3& matrix) const;
        Mat3 operator -(const Mat3& matrix) const;
        bool operator ==(const Mat3& matrix) const;
        bool operator !=(const Mat3& matrix) const;

        void transpose();
        
        float get(int row, int column) const;
        void set(int row, int column, float value);
        
        const float* data() const  {
            return (float*)&this->matrix;
        }

    private:
        float matrix[3][3];
};

#endif	/* MAT3_H */

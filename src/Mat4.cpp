#include <cmath>
#include <algorithm>

#include "Mat4.h"
#include "Vec4.h"

Mat4::Mat4() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->matrix[i][j] = 0.0f;
        }
    }

    this->matrix[0][0] = 1.0f;
    this->matrix[1][1] = 1.0f;
    this->matrix[2][2] = 1.0f;
    this->matrix[3][3] = 1.0f;
}

Mat4 Mat4::operator *(const Mat4& matrix) const {
    Mat4 result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.set(i, j, this->matrix[i][0] * matrix.get(0, j) +
                             this->matrix[i][1] * matrix.get(1, j) +
                             this->matrix[i][2] * matrix.get(2, j) +
                             this->matrix[i][3] * matrix.get(3, j));
        }
    }
    
    return result;
}

Vec4 Mat4::operator *(const Vec4& vector) const {
    Vec4 result;

    for (int i = 0; i < 4; i++) {
        result.set(i, this->matrix[i][0] * vector.get(Vec4::X) +
                      this->matrix[i][1] * vector.get(Vec4::Y) +
                      this->matrix[i][2] * vector.get(Vec4::Z) +
                      this->matrix[i][3] * vector.get(Vec4::W));
    }
    
    return result;
}

Mat4 Mat4::operator *(float scalar) const {
    Mat4 result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.set(i, j, this->matrix[i][j] * scalar);
        }
    }
    
    return result;
}

Mat4 Mat4::operator +(const Mat4& matrix) const {
    Mat4 result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.set(i, j, this->matrix[i][j] + matrix.get(i, j));
        }
    }
    
    return result;
}

Mat4 Mat4::operator -(const Mat4& matrix) const {
    Mat4 result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.set(i, j, this->matrix[i][j] - matrix.get(i, j));
        }
    }
    
    return result;    
}

bool Mat4::operator ==(const Mat4& matrix) const {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (this->matrix[i][j] != matrix.get(i, j)) {
                return false;
            }
        }
    }
    
    return true;
}

bool Mat4::operator !=(const Mat4& matrix) const {
    return !(*this == matrix);
}

void Mat4::transpose() {
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 4; j++) {
            std::swap(this->matrix[j][i], this->matrix[i][j]);
        }
    }
}

float Mat4::get(int row, int column) const {
    if (column < 0 || column > 3 || row < 0 || row > 3)
        return NAN;
    return this->matrix[row][column];
}

void Mat4::set(int row, int column, float value) {
    if (column < 0 || column > 3 || row < 0 || row > 3)
        return;
    this->matrix[row][column] = value;
}

Mat3 Mat4::extractMat3() const {
    Mat3 result;
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.set(i, j, this->matrix[i][j]);
        }
    }
    
    return result;      
}

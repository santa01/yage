#define _ISOC99_SOURCE
#include <cmath>

#include "Vec4.h"
#include "Vec3.h"

Vec4::Vec4() {
    this->vector[Vec4::X] = 0.0f;
    this->vector[Vec4::Y] = 0.0f;
    this->vector[Vec4::Z] = 0.0f;
    this->vector[Vec4::W] = 1.0f;
}

Vec4::Vec4(float x, float y, float z, float w) {
    this->vector[Vec4::X] = x;
    this->vector[Vec4::Y] = y;
    this->vector[Vec4::Z] = z;
    this->vector[Vec4::W] = w;
}

Vec4 Vec4::operator -(const Vec4& vector) const {
    Vec4 me(*this);
    return me -= vector;
}

Vec4 Vec4::operator +(const Vec4& vector) const {
    Vec4 me(*this);
    return me += vector;
}

Vec4 Vec4::operator *(float scalar) const {
    Vec4 me(*this);
    return me *= scalar;
}

Vec4& Vec4::operator -=(const Vec4& vector) {
    this->vector[Vec4::X] -= vector.get(Vec4::X);
    this->vector[Vec4::Y] -= vector.get(Vec4::Y);
    this->vector[Vec4::Z] -= vector.get(Vec4::Z);
    this->vector[Vec4::W] -= vector.get(Vec4::W);
    return *this;
}

Vec4& Vec4::operator +=(const Vec4& vector) {
    this->vector[Vec4::X] += vector.get(Vec4::X);
    this->vector[Vec4::Y] += vector.get(Vec4::Y);
    this->vector[Vec4::Z] += vector.get(Vec4::Z);
    this->vector[Vec4::W] += vector.get(Vec4::W);
    return *this;
}

Vec4& Vec4::operator *=(float scalar) {
    this->vector[Vec4::X] *= scalar;
    this->vector[Vec4::Y] *= scalar;
    this->vector[Vec4::Z] *= scalar;
    this->vector[Vec4::W] *= scalar;
    return *this;
}

bool Vec4::operator ==(const Vec4& vector) const {
    return (this->vector[Vec4::X] == vector.get(Vec4::X)) &&
           (this->vector[Vec4::Y] == vector.get(Vec4::Y)) &&
           (this->vector[Vec4::Z] == vector.get(Vec4::Z)) &&
           (this->vector[Vec4::W] == vector.get(Vec4::W));
}

bool Vec4::operator !=(const Vec4& vector) const {
    return !(*this == vector);
}

float Vec4::dot(const Vec4& vector) const {
    return this->vector[Vec4::X] * vector.get(Vec4::X) +
           this->vector[Vec4::Y] * vector.get(Vec4::Y) +
           this->vector[Vec4::Z] * vector.get(Vec4::Z) +
           this->vector[Vec4::W] * vector.get(Vec4::W);
}

float Vec4::get(int index) const {
    switch (index) {
        case Vec4::X:
        case Vec4::Y:
        case Vec4::Z:
        case Vec4::W:
            return this->vector[index];

        default:
            return NAN;            
    }
}

void Vec4::set(int index, float value) {
    switch (index) {
        case Vec4::X:
        case Vec4::Y:
        case Vec4::Z:
        case Vec4::W:
            this->vector[index] = value;
            break;           
    }
    
}

Vec3 Vec4::extractVec3() const {
    return Vec3(this->vector[Vec4::X],
                this->vector[Vec4::Y],
                this->vector[Vec4::Z]);
}

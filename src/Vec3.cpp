#define _ISOC99_SOURCE
#include <cmath>

#include "Vec3.h"

Vec3::Vec3() {
    for (int i = 0; i < 3; i++)
        this->vector[i] = 0.0f;
}

Vec3::Vec3(float x, float y, float z) {
    this->vector[Vec3::X] = x;
    this->vector[Vec3::Y] = y;
    this->vector[Vec3::Z] = z;
}

Vec3 Vec3::operator -(const Vec3& vector) const {
    Vec3 me(*this);
    return me -= vector;
}

Vec3 Vec3::operator +(const Vec3& vector) const {
    Vec3 me(*this);
    return me += vector;
}

Vec3 Vec3::operator *(float scalar) const {
    Vec3 me(*this);
    return me *= scalar;
}

Vec3& Vec3::operator -=(const Vec3& vector) {
    this->vector[Vec3::X] -= vector.get(Vec3::X);
    this->vector[Vec3::Y] -= vector.get(Vec3::Y);
    this->vector[Vec3::Z] -= vector.get(Vec3::Z);
    return *this;
}

Vec3& Vec3::operator +=(const Vec3& vector) {
    this->vector[Vec3::X] += vector.get(Vec3::X);
    this->vector[Vec3::Y] += vector.get(Vec3::Y);
    this->vector[Vec3::Z] += vector.get(Vec3::Z);
    return *this;
}

Vec3& Vec3::operator *=(float scalar) {
    this->vector[Vec3::X] *= scalar;
    this->vector[Vec3::Y] *= scalar;
    this->vector[Vec3::Z] *= scalar;
    return *this;
}

bool Vec3::operator ==(const Vec3& vector) const {
    return (this->vector[Vec3::X] == vector.get(Vec3::X)) &&
           (this->vector[Vec3::Y] == vector.get(Vec3::Y)) &&
           (this->vector[Vec3::Z] == vector.get(Vec3::Z));
}

bool Vec3::operator !=(const Vec3& vector) const {
    return !(*this == vector);
}

float Vec3::dot(const Vec3& vector) const {
    return this->vector[Vec3::X] * vector.get(Vec3::X) +
           this->vector[Vec3::Y] * vector.get(Vec3::Y) +
           this->vector[Vec3::Z] * vector.get(Vec3::Z);
}

Vec3 Vec3::cross(const Vec3& vector) const {
    return Vec3(this->vector[Vec3::Y] * vector.get(Vec3::Z) -
                        this->vector[Vec3::Z] * vector.get(Vec3::Y),
                this->vector[Vec3::Z] * vector.get(Vec3::X) -
                        this->vector[Vec3::X] * vector.get(Vec3::Z),
                this->vector[Vec3::X] * vector.get(Vec3::Y) -
                        this->vector[Vec3::Y] * vector.get(Vec3::X));
}

void Vec3::normalize() {
    float length = this->length();
    this->vector[Vec3::X] /= length;
    this->vector[Vec3::Y] /= length;
    this->vector[Vec3::Z] /= length;
}

float Vec3::length() const {
    return sqrtf(this->squareLength());
}

float Vec3::squareLength() const {
    return this->vector[Vec3::X] * this->vector[Vec3::X] +
           this->vector[Vec3::Y] * this->vector[Vec3::Y] +
           this->vector[Vec3::Z] * this->vector[Vec3::Z];
}

float Vec3::get(int index) const {
    switch (index) {
        case Vec3::X:
        case Vec3::Y:
        case Vec3::Z:
            return this->vector[index];

        default:
            return NAN;            
    }
}

void Vec3::set(int index, float value) {
    switch (index) {
        case Vec3::X:
        case Vec3::Y:
        case Vec3::Z:
            this->vector[index] = value;
            break;          
    }
}

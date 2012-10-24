#include <cmath>

#include "Quaternion.h"
#include "Mat4.h"
#include "Vec3.h"

Quaternion::Quaternion() {
    this->vector[Quaternion::X] = 0.0f;
    this->vector[Quaternion::Y] = 0.0f;
    this->vector[Quaternion::Z] = 0.0f;
    this->vector[Quaternion::W] = 1.0f;
}

Quaternion::Quaternion(float x, float y, float z, float w) {
    this->vector[Quaternion::X] = x;
    this->vector[Quaternion::Y] = y;
    this->vector[Quaternion::Z] = z;
    this->vector[Quaternion::W] = w;
}

Quaternion::Quaternion(const Vec3& axis, float angle) {
    float sinAngle = sinf(angle / 2);
    
    this->vector[Quaternion::X] = axis.get(Vec3::X) * sinAngle;
    this->vector[Quaternion::Y] = axis.get(Vec3::Y) * sinAngle;
    this->vector[Quaternion::Z] = axis.get(Vec3::Z) * sinAngle;
    this->vector[Quaternion::W] = cosf(angle / 2);
}

Quaternion Quaternion::operator *(const Quaternion& quaternion) const {
    Quaternion result;

    result.set(Quaternion::W, this->vector[Quaternion::W] * quaternion.get(Quaternion::W) -
                              this->vector[Quaternion::X] * quaternion.get(Quaternion::X) -
                              this->vector[Quaternion::Y] * quaternion.get(Quaternion::Y) -
                              this->vector[Quaternion::Z] * quaternion.get(Quaternion::Z));
    
    result.set(Quaternion::X, this->vector[Quaternion::W] * quaternion.get(Quaternion::X) +
                              this->vector[Quaternion::X] * quaternion.get(Quaternion::W) +
                              this->vector[Quaternion::Y] * quaternion.get(Quaternion::Z) -
                              this->vector[Quaternion::Z] * quaternion.get(Quaternion::Y));
    
    result.set(Quaternion::Y, this->vector[Quaternion::W] * quaternion.get(Quaternion::Y) +
                              this->vector[Quaternion::Y] * quaternion.get(Quaternion::W) +
                              this->vector[Quaternion::Z] * quaternion.get(Quaternion::X) -
                              this->vector[Quaternion::X] * quaternion.get(Quaternion::Z));
    
    result.set(Quaternion::Z, this->vector[Quaternion::W] * quaternion.get(Quaternion::Z) +
                              this->vector[Quaternion::Z] * quaternion.get(Quaternion::W) +
                              this->vector[Quaternion::X] * quaternion.get(Quaternion::Y) -
                              this->vector[Quaternion::Y] * quaternion.get(Quaternion::X));

    result.normalize();
    return result;
}

Quaternion& Quaternion::normalize() {
    float length = this->length();
    this->vector[Quaternion::X] /= length;
    this->vector[Quaternion::Y] /= length;
    this->vector[Quaternion::Z] /= length;
    this->vector[Quaternion::W] /= length;
    return *this;
}

float Quaternion::length() const {
    return sqrtf(vector[Quaternion::X] * vector[Quaternion::X] +
                 vector[Quaternion::Y] * vector[Quaternion::Y] +
                 vector[Quaternion::Z] * vector[Quaternion::Z] +
                 vector[Quaternion::W] * vector[Quaternion::W]);
}

float Quaternion::get(int index) const {
    switch (index) {
        case Quaternion::X:
        case Quaternion::Y:
        case Quaternion::Z:
        case Quaternion::W:
            return this->vector[index];

        default:
            return NAN;            
    }
}

void Quaternion::set(int index, float value) {
    switch (index) {
        case Quaternion::X:
        case Quaternion::Y:
        case Quaternion::Z:
        case Quaternion::W:
            this->vector[index] = value;
            break;           
    }
}

Mat4 Quaternion::extractMat4() const {
    Mat4 result;
    
    result.set(0, 0, 1 - 2 * this->vector[Quaternion::Y] * this->vector[Quaternion::Y] -
                         2 * this->vector[Quaternion::Z] * this->vector[Quaternion::Z]);
    result.set(0, 1, 2 * this->vector[Quaternion::X] * this->vector[Quaternion::Y] -
                     2 * this->vector[Quaternion::Z] * this->vector[Quaternion::W]);
    result.set(0, 2, 2 * this->vector[Quaternion::X] * this->vector[Quaternion::Y] +
                     2 * this->vector[Quaternion::Y] * this->vector[Quaternion::W]);
    
    result.set(1, 0, 2 * this->vector[Quaternion::X] * this->vector[Quaternion::Y] +
                     2 * this->vector[Quaternion::Z] * this->vector[Quaternion::W]);
    result.set(1, 1, 1 - 2 * this->vector[Quaternion::X] * this->vector[Quaternion::X] -
                         2 * this->vector[Quaternion::Z] * this->vector[Quaternion::Z]);
    result.set(1, 2, 2 * this->vector[Quaternion::Y] * this->vector[Quaternion::Z] -
                     2 * this->vector[Quaternion::X] * this->vector[Quaternion::W]);
    
    result.set(2, 0, 2 * this->vector[Quaternion::X] * this->vector[Quaternion::Z] -
                     2 * this->vector[Quaternion::Y] * this->vector[Quaternion::W]);
    result.set(2, 1, 2 * this->vector[Quaternion::Y] * this->vector[Quaternion::Z] +
                     2 * this->vector[Quaternion::X] * this->vector[Quaternion::W]);
    result.set(2, 2, 1 - 2 * this->vector[Quaternion::X] * this->vector[Quaternion::X] -
                         2 * this->vector[Quaternion::Y] * this->vector[Quaternion::Y]);
    
    return result;
}

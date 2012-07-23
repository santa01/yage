#include "Camera.h"
#include "Mat3.h"
#include "Mat4.h"
#include "Vec3.h"
#include "Quaternion.h"

Camera::Camera():
        up(0.0f, 1.0f, 0.0f),
        target(0.0f, 0.0f, 1.0f) {
}

Camera::Camera(float x, float y, float z):
        up(0.0f, 1.0f, 0.0f),
        target(0.0f, 0.0f, 1.0f) {
    this->setPosition(x, y, z); 
}

Camera::Camera(const Vec3& position):
        up(0.0f, 1.0f, 0.0f),
        target(0.0f, 0.0f, 1.0f) {
    this->setPosition(position);
}

void Camera::setPosition(const Vec3& position) {
    this->translationMatrix.set(0, 3, -position.get(Vec3::X));
    this->translationMatrix.set(1, 3, -position.get(Vec3::Y));
    this->translationMatrix.set(2, 3, -position.get(Vec3::Z));
}

Vec3 Camera::getPosition() const {
    return Vec3(-this->translationMatrix.get(0, 3),
                -this->translationMatrix.get(1, 3),
                -this->translationMatrix.get(2, 3));
}

float Camera::getXAngle() const {
    // TODO
}

float Camera::getYAngle() const {
    // TODO
}

float Camera::getZAngle() const {
    // TODO
}

void Camera::rotate(const Vec3& vector, float angle) {
    Quaternion q(vector, angle * 3.1415f / 180.0f);
    q.normalize();
    
    Mat3 rotationMatrix = q.extractMat4().extractMat3();
    this->up = rotationMatrix * this->up;
    this->target = rotationMatrix * this->target;
    
    Vec3 right = this->up.cross(this->target);
    right.normalize();

    this->rotationMatrix.set(0, 0, right.get(Vec3::X));
    this->rotationMatrix.set(0, 1, right.get(Vec3::Y));
    this->rotationMatrix.set(0, 2, right.get(Vec3::Z));
    
    this->rotationMatrix.set(1, 0, this->up.get(Vec3::X));
    this->rotationMatrix.set(1, 1, this->up.get(Vec3::Y));
    this->rotationMatrix.set(1, 2, this->up.get(Vec3::Z));
    
    this->rotationMatrix.set(2, 0, this->target.get(Vec3::X));
    this->rotationMatrix.set(2, 1, this->target.get(Vec3::Y));
    this->rotationMatrix.set(2, 2, this->target.get(Vec3::Z));
}

Vec3 Camera::getRightVector() const {
    Vec3 right = this->up.cross(this->target);
    right.normalize();
    return right;
}

void Camera::lookAt(float x, float y, float z) {
    this->lookAt(Vec3(x, y, z));
}

void Camera::lookAt(const Vec3& vector) {
    Vec3 currentTarget = this->target;
    currentTarget.normalize();
    
    Vec3 newTarget = vector;
    newTarget.normalize();
    
    Vec3 rotationAxis = currentTarget.cross(vector);
    float angle = currentTarget.dot(vector);
    
    this->rotate(rotationAxis, angle);
}

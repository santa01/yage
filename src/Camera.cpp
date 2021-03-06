#include <cmath>

#include "Camera.h"
#include "Mat3.h"
#include "Mat4.h"
#include "Vec3.h"
#include "Quaternion.h"
#include "PerspectiveProjection.h"
#include "SkyBox.h"

Camera::Camera() {
    this->initialize();
}

Camera::Camera(float x, float y, float z) {
    this->setPosition(x, y, z);
    this->initialize();
}

Camera::Camera(const Vec3& position) {
    this->setPosition(position);
    this->initialize();
}

Camera::~Camera() {
    if (this->projection) {
        delete this->projection;
    }
}

void Camera::setPosition(const Vec3& position) {
    this->translationMatrix.set(0, 3, -position.get(Vec3::X));
    this->translationMatrix.set(1, 3, -position.get(Vec3::Y));
    this->translationMatrix.set(2, 3, -position.get(Vec3::Z));

    if (this->skyBox != NULL) {
        this->skyBox->setPosition(position);
    }
}

Vec3 Camera::getPosition() const {
    return Vec3(-this->translationMatrix.get(0, 3),
                -this->translationMatrix.get(1, 3),
                -this->translationMatrix.get(2, 3));
}

float Camera::getXAngle() const {
    // TODO: implement
    return 0.0f;
}

float Camera::getYAngle() const {
    // TODO: implement
    return 0.0f;
}

float Camera::getZAngle() const {
    // TODO: implement
    return 0.0f;
}

void Camera::rotate(const Vec3& vector, float angle) {
    if (vector == Vec3::ZERO) {
        return;
    }
    
    Vec3 axis(vector);
    Quaternion q(axis.normalize(), angle * M_PI / 180.0f);
    q.normalize();
    
    Mat3 rotationMatrix = q.extractMat4().extractMat3();
    Vec3 up(this->rotationMatrix.get(1, 0),
            this->rotationMatrix.get(1, 1),
            this->rotationMatrix.get(1, 2));
    Vec3 target(this->rotationMatrix.get(2, 0),
                this->rotationMatrix.get(2, 1),
                this->rotationMatrix.get(2, 2));
    up = rotationMatrix * up;
    target = rotationMatrix * target;
    
    Vec3 right = target.cross(up);
    right.normalize();

    this->updateRotationMatrix(right, up, target);
}

Vec3 Camera::getUp() const {
    return Vec3(this->rotationMatrix.get(1, 0),
                this->rotationMatrix.get(1, 1),
                this->rotationMatrix.get(1, 2));
}

Vec3 Camera::getTarget() const {
    return -Vec3(this->rotationMatrix.get(2, 0),
                 this->rotationMatrix.get(2, 1),
                 this->rotationMatrix.get(2, 2));
}

Vec3 Camera::getRight() const {
    Vec3 up(this->rotationMatrix.get(1, 0),
            this->rotationMatrix.get(1, 1),
            this->rotationMatrix.get(1, 2));
    Vec3 target(this->rotationMatrix.get(2, 0),
                this->rotationMatrix.get(2, 1),
                this->rotationMatrix.get(2, 2));
    Vec3 right = target.cross(up);
    return right.normalize();
}

void Camera::lookAt(float x, float y, float z) {
    this->lookAt(Vec3(x, y, z));
}

void Camera::lookAt(const Vec3& vector) {
    if (vector == Vec3::ZERO) {
        return;
    }
    
    Vec3 target(-vector);
    target.normalize();

    Vec3 right = this->getRight();
    if (target != Vec3::UNIT_Y && target != -Vec3::UNIT_Y) {
        right = target.cross(Vec3::UNIT_Y);
        right.normalize();  
    }
    
    Vec3 up = right.cross(target);
    up.normalize();
    
    this->updateRotationMatrix(right, up, target);
}

void Camera::updateRotationMatrix(const Vec3& right, const Vec3& up, const Vec3& target) {
    this->rotationMatrix.set(0, 0, right.get(Vec3::X));
    this->rotationMatrix.set(0, 1, right.get(Vec3::Y));
    this->rotationMatrix.set(0, 2, right.get(Vec3::Z));
    
    this->rotationMatrix.set(1, 0, up.get(Vec3::X));
    this->rotationMatrix.set(1, 1, up.get(Vec3::Y));
    this->rotationMatrix.set(1, 2, up.get(Vec3::Z));
    
    this->rotationMatrix.set(2, 0, target.get(Vec3::X));
    this->rotationMatrix.set(2, 1, target.get(Vec3::Y));
    this->rotationMatrix.set(2, 2, target.get(Vec3::Z));
}

void Camera::initialize() {
    this->lookAt(Vec3::UNIT_Z);
    this->projection = new PerspectiveProjection();
    this->skyBox = NULL;
}

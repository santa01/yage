#include "OrthographicProjection.h"

OrthographicProjection::OrthographicProjection() {
    this->aspectRatio = 1.3333f;
    this->nearPlane = -100.0f;
    this->farPlane = 100.0f;
    
    this->updatePerspectiveMatrix();
}

void OrthographicProjection::setNearPlane(float nearPlane) {
    this->nearPlane = nearPlane;
    this->updatePerspectiveMatrix();
}

void OrthographicProjection::setFarPlane(float farPlane) {
    this->farPlane = farPlane;
    this->updatePerspectiveMatrix();
}

void OrthographicProjection::setAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
    this->updatePerspectiveMatrix();
}

void OrthographicProjection::updatePerspectiveMatrix() {
    this->projection.set(0, 0, 10.0f / ((this->farPlane - this->nearPlane) *
                               this->aspectRatio));
    this->projection.set(1, 1, 10.0f / (this->farPlane - this->nearPlane));
    this->projection.set(2, 2, -2.0f / (this->farPlane - this->nearPlane));
    this->projection.set(2, 3, (-this->farPlane - this->nearPlane) /
                               (this->farPlane - this->nearPlane));
}

#include <cmath>

#include "PerspectiveProjection.h"

PerspectiveProjection::PerspectiveProjection() {
    this->aspectRatio = 1.3333f;
    this->nearPlane = 0.1f;
    this->farPlane = 1000.0f;
    this->fov = 90.0f;

    this->projection.set(3, 2, -1.0f);
    this->projection.set(3, 3, 0.0f);
    
    this->updateFieldOfView();
    this->updateClipDistances();
}

void PerspectiveProjection::setNearPlane(float nearPlane) {
    this->nearPlane = nearPlane;
    this->updateClipDistances();
}

void PerspectiveProjection::setFarPlane(float farPlane) {
    this->farPlane = farPlane;
    this->updateClipDistances();
}

void PerspectiveProjection::setFov(float fov) {
    this->fov = fov;
    this->updateFieldOfView();
}

void PerspectiveProjection::setAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
    this->updateFieldOfView();
}

void PerspectiveProjection::updateClipDistances() {
    this->projection.set(2, 2, (-this->farPlane - this->nearPlane) /
                               (this->farPlane - this->nearPlane));
    this->projection.set(2, 3, (-2.0f * this->farPlane * this->nearPlane) /
                               (this->farPlane - this->nearPlane));
}

void PerspectiveProjection::updateFieldOfView() {
    this->projection.set(0, 0, 1.0f / (tanf(this->fov * M_PI / 180.0f / 2.0f) *
                               this->aspectRatio));
    this->projection.set(1, 1, 1.0f / (tanf(this->fov * M_PI / 180.0f / 2.0f)));
}

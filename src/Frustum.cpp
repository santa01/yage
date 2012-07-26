#define _ISOC99_SOURCE
#include <cmath>

#include "Frustum.h"

Frustum::Frustum() {
    this->aspectRatio = 1.3333f;
    this->nearPlane = 0.1f;
    this->farPlane = 1000.0f;
    this->fov = 90.0f;

    this->perspective.set(3, 2, 1.0f);
    this->perspective.set(3, 3, 0.0f);
    
    this->updateFieldOfView();
    this->updateClipDistances();
}

Frustum::~Frustum() {
}

void Frustum::setNearPlane(float nearPlane) {
    this->nearPlane = nearPlane;
    this->updateClipDistances();
}

void Frustum::setFarPlane(float farPlane) {
    this->farPlane = farPlane;
    this->updateClipDistances();
}

void Frustum::setFov(float fov) {
    this->fov = fov;
    this->updateFieldOfView();
}

void Frustum::setAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
    this->updateFieldOfView();
}

void Frustum::updateClipDistances() {
    this->perspective.set(2, 2, (- this->farPlane - this->nearPlane) /
                                (this->nearPlane - this->farPlane));
    this->perspective.set(2, 3, (2 * this->farPlane * this->nearPlane) /
                                (this->nearPlane - this->farPlane));
}

void Frustum::updateFieldOfView() {
    this->perspective.set(0, 0, 1.0f / (tanf(this->fov * M_PI / 180.0f / 2.0f) *
                                this->aspectRatio));
    this->perspective.set(1, 1, 1.0f / (tanf(this->fov * M_PI / 180.0f / 2.0f)));
}

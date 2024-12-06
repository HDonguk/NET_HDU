#include "Camera.h"

Camera::Camera(float width, float height) : width(width), height(height), offsetX(0), offsetY(0), boundWidth(0), boundHeight(0) {
}

Camera::~Camera() {
}

void Camera::Update(float playerX, float playerY) {
    offsetX = playerX - width / 2;
    offsetY = playerY - height / 2;

    // 경계를 벗어나지 않도록 위치 제한
    if (offsetX < 0) offsetX = 0;
    if (offsetY < 0) offsetY = 0;
    if (offsetX > boundWidth - width) offsetX = boundWidth - width;
    if (offsetY > boundHeight - height) offsetY = boundHeight - height;
}

void Camera::SetBounds(float width, float height) {
    boundWidth = width;
    boundHeight = height;
}

float Camera::GetOffsetX() const {
    return offsetX;
}

float Camera::GetOffsetY() const {
    return offsetY;
}

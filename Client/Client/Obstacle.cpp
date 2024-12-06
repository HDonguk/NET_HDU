#include "Obstacle.h"

Obstacle::Obstacle(float x, float y, const std::wstring& imagePath) : x(x), y(y) {
    obstacleImage.Load(imagePath.c_str());
}

Obstacle::~Obstacle() {
}

void Obstacle::Draw(HDC hdc, float offsetX, float offsetY) {
    obstacleImage.Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
}

float Obstacle::GetX() const {
    return x;
}

float Obstacle::GetY() const {
    return y;
}

float Obstacle::GetWidth() const {
    return static_cast<float>(obstacleImage.GetWidth());
}

float Obstacle::GetHeight() const {
    return static_cast<float>(obstacleImage.GetHeight());
}

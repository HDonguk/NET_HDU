#pragma once

#include <atlimage.h>
#include <string>

class Obstacle {
public:
    Obstacle(float x, float y, const std::wstring& imagePath);
    ~Obstacle();

    void Draw(HDC hdc, float offsetX, float offsetY);
    float GetX() const;
    float GetY() const;
    float GetWidth() const;
    float GetHeight() const;

private:
    float x, y;
    CImage obstacleImage;
};

#pragma once

class Camera {
public:
    Camera(float width, float height);
    ~Camera();

    void Update(float playerX, float playerY);
    float GetOffsetX() const;
    float GetOffsetY() const;

    void SetBounds(float width, float height); // 경계를 설정하는 메서드 추가

private:
    float width, height;
    float offsetX, offsetY;
    float boundWidth, boundHeight; // 경계를 위한 변수
};

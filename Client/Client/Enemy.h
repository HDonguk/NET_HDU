#pragma once
#include <atlimage.h>
#include <vector>
#include "Obstacle.h"

class Enemy {
public:
    Enemy(float x, float y, float speed, int health, float animationSpeed = 5.0f, float eWidth = 50.0f, float eHeight = 50.0f);
    virtual ~Enemy();

    void Update(float frameTime, float playerX, float playerY, const std::vector<Obstacle*>& obstacles);
    void Draw(HDC hdc, float offsetX, float offsetY);

    void TakeDamage(int damage);
    bool IsDead() const;

    float GetX() const;
    float GetY() const;
    float GetWidth() const;
    float GetHeight() const;

    virtual void UpdateBoss(float frameTime, float playerX, float playerY, const std::vector<Obstacle*>& obstacles);

    // void SetBounds(float width, float height);

protected:
    virtual void LoadImages(); // override를 허용하기 위한 virtual

    void UpdateDeathEffect(float frameTime);
    void DrawDeathEffect(HDC hdc, float offsetX, float offsetY);

    float x, y;
    float eWidth, eHeight;
    float speed;
    int health;
    float boundWidth, boundHeight;

    int currentFrame;
    float frameTimeAccumulator;
    float animationSpeed;

    std::vector<CImage> idleImages;

    bool CheckCollision(float newX, float newY, const std::vector<Obstacle*>& obstacles) const;

    bool isDying;
    float deathEffectDuration;
    float deathEffectStart;
    std::vector<CImage> deathEffectImages;
};

// BrainMonster
class BrainMonster : public Enemy {
public:
    BrainMonster(float x, float y, float speed, int health = 50, float eWidth = 27.0f, float eHeight = 36.0f);
protected:
    void LoadImages() override;
};

// EyeMonster
class EyeMonster : public Enemy {
public:
    EyeMonster(float x, float y, float speed, int health = 50, float eWidth = 37.0f, float eHeight = 29.0f);
protected:
    void LoadImages() override; 
};

// BigBoomer
class BigBoomer : public Enemy {
public:
    BigBoomer(float x, float y, float speed, int health = 500, float eWidth = 45.0f, float eHeight = 51.0f);
protected:
    void LoadImages() override; 
};

// Lamprey
class Lamprey : public Enemy {
public:
    Lamprey(float x, float y, float speed, int health = 1000, float eWidth = 50.0f, float eHeight = 50.0f);
protected:
    void LoadImages() override;
};

// Yog
class Yog : public Enemy {
public:
    Yog(float x, float y, float speed, int health = 2500, float eWidth = 64.0f, float eHeight = 54.0f);
protected:
    void LoadImages() override; 
};

class WingedMonster : public Enemy {
public:
    WingedMonster(float x, float y, float speed, int health = 10000, float eWidth = 50.0f, float eHeight = 50.0f);

    void UpdateBoss(float frameTime, float playerX, float playerY, const std::vector<Obstacle*>& obstacles) override;

protected:
    void LoadImages() override;

private:
    bool isDashing;
    float dashCooldown;
    float dashSpeed;
    float dashTimer;
    float dashDirectionX;
    float dashDirectionY;
};


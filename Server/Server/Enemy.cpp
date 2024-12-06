#include "Enemy.h"
#include "GameFrameWork.h"
#include <cmath>

#define EnemyWidth 50.0f
#define EnemyHeight 50.0f

Enemy::Enemy(float x, float y, float speed, int health, float animationSpeed, float eWidth, float eHeight)
    : x(x), y(y), speed(speed), health(health), boundWidth(0), boundHeight(0), currentFrame(0), frameTimeAccumulator(0.0f), animationSpeed(animationSpeed), eWidth(eWidth), eHeight(eHeight),
    isDying(false), deathEffectDuration(0.5f), deathEffectStart(0.0f) {
    LoadImages();

    // Load death effect images
    deathEffectImages.resize(4);
    deathEffectImages[0].Load(L"./resources/effect/DeathFX_0.png");
    deathEffectImages[1].Load(L"./resources/effect/DeathFX_1.png");
    deathEffectImages[2].Load(L"./resources/effect/DeathFX_2.png");
    deathEffectImages[3].Load(L"./resources/effect/DeathFX_3.png");
}

Enemy::~Enemy() {
}

void Enemy::LoadImages() {
    idleImages.resize(5);
}


void Enemy::Update(float frameTime, float playerX, float playerY, const std::vector<Obstacle*>& obstacles) {

    if (isDying) {
        UpdateDeathEffect(frameTime);
        return;
    }

    float dx = playerX - x;
    float dy = playerY - y;
    float distance = sqrt(dx * dx + dy * dy);

    float directionX = dx / distance;
    float directionY = dy / distance;

    float newX = x + directionX * speed * frameTime;
    float newY = y + directionY * speed * frameTime;

    if (!CheckCollision(newX, newY, obstacles)) {
        x = newX;
        y = newY;
    }

    frameTimeAccumulator += frameTime;
    if (frameTimeAccumulator >= animationSpeed) {
        currentFrame = (currentFrame + 1) % idleImages.size();
        frameTimeAccumulator = 0.0f;
    }
}

void Enemy::UpdateDeathEffect(float frameTime) {

    deathEffectStart += frameTime;
    if (deathEffectStart >= deathEffectDuration) {
        //PlaySound(L"./resources/sounds/enemyDeath.wav", NULL, SND_FILENAME | SND_ASYNC);// Big Noise
        isDying = false;
        deathEffectStart = 0.0f;
    }
}

bool Enemy::CheckCollision(float newX, float newY, const std::vector<Obstacle*>& obstacles) const {
    for (const auto& obstacle : obstacles) {
        float ox = obstacle->GetX();
        float oy = obstacle->GetY();
        float ow = obstacle->GetWidth();
        float oh = obstacle->GetHeight();

        if (newX < ox + ow &&
            newX + eWidth > ox &&
            newY < oy + oh &&
            newY + eHeight > oy) {
            return true;
        }
    }
    return false;
}

void Enemy::Draw(HDC hdc, float offsetX, float offsetY) {
    if (isDying) {
        DrawDeathEffect(hdc, offsetX, offsetY);
        return;
    }

    if (!idleImages[currentFrame].IsNull()) {
        idleImages[currentFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
    }
}

void Enemy::DrawDeathEffect(HDC hdc, float offsetX, float offsetY) {
    int frame = static_cast<int>((deathEffectStart / deathEffectDuration) * deathEffectImages.size());
    if (frame >= 0 && frame < deathEffectImages.size()) {
        deathEffectImages[frame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
    }
}

void Enemy::TakeDamage(int damage) {
    health -= damage;
    if (health <= 0 && !isDying) {
        isDying = true;
        health = 0;
        deathEffectStart = 0.0f;
    }
}

bool Enemy::IsDead() const {
    return health <= 0 && !isDying;
}

float Enemy::GetX() const {
    return x;
}

float Enemy::GetY() const {
    return y;
}

float Enemy::GetWidth() const {
    return eWidth;
}

float Enemy::GetHeight() const {
    return eHeight;
}

// BrainMonster
BrainMonster::BrainMonster(float x, float y, float speed, int health, float eWidth, float eHeight)
    : Enemy(x, y, 20.0f, health, 0.2f, eWidth, eHeight) {
    LoadImages();
}

void BrainMonster::LoadImages() {
    idleImages.resize(4);
    idleImages[0].Load(L"./resources/enemy/BrainMonster_0.png");
    idleImages[1].Load(L"./resources/enemy/BrainMonster_1.png");
    idleImages[2].Load(L"./resources/enemy/BrainMonster_2.png");
    idleImages[3].Load(L"./resources/enemy/BrainMonster_3.png");
}

// EyeMonster
EyeMonster::EyeMonster(float x, float y, float speed, int health, float eWidth, float eHeight)
    : Enemy(x, y, 40.0f, health, 0.2f, eWidth, eHeight) {
    LoadImages();
}

void EyeMonster::LoadImages() {
    idleImages.resize(3);
    idleImages[0].Load(L"./resources/enemy/EyeMonster_0.png");
    idleImages[1].Load(L"./resources/enemy/EyeMonster_1.png");
    idleImages[2].Load(L"./resources/enemy/EyeMonster_2.png");
}

// BigBoomer
BigBoomer::BigBoomer(float x, float y, float speed, int health, float eWidth, float eHeight)
    : Enemy(x, y, 30.0f, health, 0.2f, eWidth, eHeight) {
    LoadImages();
}

void BigBoomer::LoadImages() {
    idleImages.resize(4);
    idleImages[0].Load(L"./resources/enemy/BigBoomer_0.png");
    idleImages[1].Load(L"./resources/enemy/BigBoomer_1.png");
    idleImages[2].Load(L"./resources/enemy/BigBoomer_2.png");
    idleImages[3].Load(L"./resources/enemy/BigBoomer_3.png");
}

// Lamprey
Lamprey::Lamprey(float x, float y, float speed, int health, float eWidth, float eHeight)
    : Enemy(x, y, 40.0f, health, 0.2f, eWidth, eHeight) {
    LoadImages();
}

void Lamprey::LoadImages() {
    idleImages.resize(5);
    idleImages[0].Load(L"./resources/enemy/T_Lamprey_0.png");
    idleImages[1].Load(L"./resources/enemy/T_Lamprey_1.png");
    idleImages[2].Load(L"./resources/enemy/T_Lamprey_2.png");
    idleImages[3].Load(L"./resources/enemy/T_Lamprey_3.png");
    idleImages[4].Load(L"./resources/enemy/T_Lamprey_4.png");
}

// Yog
Yog::Yog(float x, float y, float speed, int health, float eWidth, float eHeight)
    : Enemy(x, y, 50.0f, health, 0.2f, eWidth, eHeight) {
    LoadImages();
}

void Yog::LoadImages() {
    idleImages.resize(4);
    idleImages[0].Load(L"./resources/enemy/T_Yog_0.png");
    idleImages[1].Load(L"./resources/enemy/T_Yog_1.png");
    idleImages[2].Load(L"./resources/enemy/T_Yog_2.png");
    idleImages[3].Load(L"./resources/enemy/T_Yog_3.png");
}

// WingedMonster
WingedMonster::WingedMonster(float x, float y, float speed, int health, float eWidth, float eHeight)
    : Enemy(x, y, 60.0f, health, 0.2f, eWidth, eHeight), isDashing(false), dashCooldown(1.5f), dashSpeed(600.0f), dashTimer(0.0f) {
    LoadImages();
}



void WingedMonster::LoadImages() {
    idleImages.resize(5);
    idleImages[0].Load(L"./resources/enemy/WingedMonster_0.png");
    idleImages[1].Load(L"./resources/enemy/WingedMonster_1.png");
    idleImages[2].Load(L"./resources/enemy/WingedMonster_2.png");
    idleImages[3].Load(L"./resources/enemy/WingedMonster_3.png");
    idleImages[4].Load(L"./resources/enemy/WingedMonster_4.png");
}

void Enemy::UpdateBoss(float frameTime, float playerX, float playerY, const std::vector<Obstacle*>& obstacles) {

}

void WingedMonster::UpdateBoss(float frameTime, float playerX, float playerY, const std::vector<Obstacle*>& obstacles) {
    dashTimer += frameTime;

    if (isDying) {
        UpdateDeathEffect(frameTime);
        return;
    }

    if (isDashing) {
        float newX = x + dashDirectionX * dashSpeed * frameTime;
        float newY = y + dashDirectionY * dashSpeed * frameTime;

        if (!CheckCollision(newX, newY, obstacles)) {
            x = newX;
            y = newY;
        }


        if (dashTimer >= 0.2f) {
            isDashing = false;
            dashTimer = 0.0f;
        }
    }
    else {

        float dx = playerX - x;
        float dy = playerY - y;
        float distance = sqrt(dx * dx + dy * dy);

        float directionX = dx / distance;
        float directionY = dy / distance;

        float newX = x + directionX * speed * frameTime;
        float newY = y + directionY * speed * frameTime;

        if (!CheckCollision(newX, newY, obstacles)) {
            x = newX;
            y = newY;
        }

        // dash 1.5 seconds
        if (dashTimer >= dashCooldown) {
            isDashing = true;
            PlaySound(L"./resources/sounds/dash.wav", NULL, SND_FILENAME | SND_ASYNC);
            dashDirectionX = directionX;
            dashDirectionY = directionY;
            dashTimer = 0.0f;
        }
    }

    frameTimeAccumulator += frameTime;
    if (frameTimeAccumulator >= animationSpeed) {
        currentFrame = (currentFrame + 1) % idleImages.size();
        frameTimeAccumulator = 0.0f;
    }
}

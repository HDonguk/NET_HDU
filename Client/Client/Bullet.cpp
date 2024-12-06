#include "Bullet.h"
#include <cmath>

Bullet::Bullet(float x, float y, float targetX, float targetY, int damage, float speed)
    : x(x), y(y), speed(speed), damage(damage), isHit(false), hitEffectDuration(0.25f), hitEffectTime(0.0f) {
    float dx = targetX - x;
    float dy = targetY - y;
    float distance = sqrt(dx * dx + dy * dy);
    directionX = dx / distance;
    directionY = dy / distance;
    bulletImage.Load(L"./resources/gun/SmallCircle.png");

    // Load hit effect images
    hitEffectImages.resize(2);
    hitEffectImages[0].Load(L"./resources/effect/HitImpactFX_0.png");
    hitEffectImages[1].Load(L"./resources/effect/HitImpactFX_1.png");
}

Bullet::~Bullet() {
    for (auto& image : hitEffectImages) {
        image.Destroy();
    }
}

void Bullet::Update(float frameTime) {
    if (isHit) {
        UpdateHitEffect(frameTime);
    }
    else {
        x += directionX * speed * frameTime;
        y += directionY * speed * frameTime;
    }
}

void Bullet::UpdateHitEffect(float frameTime) {
    hitEffectTime += frameTime;
}

void Bullet::Draw(HDC hdc, float offsetX, float offsetY) {
    if (isHit) {
        DrawHitEffect(hdc, offsetX, offsetY);
    }
    else {
        bulletImage.Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
    }
}

void Bullet::DrawHitEffect(HDC hdc, float offsetX, float offsetY) {
    int frame = static_cast<int>((hitEffectTime / hitEffectDuration) * hitEffectImages.size());
    if (frame >= hitEffectImages.size()) {
        frame = hitEffectImages.size() - 1; // 마지막 프레임을 유지
    }
    if (frame >= 0 && frame < hitEffectImages.size()) {
        hitEffectImages[frame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
    }
}

bool Bullet::IsOutOfBounds(float width, float height) const {
    return x < 0 || y < 0 || x > width || y > height;
}

bool Bullet::CheckCollision(float enemyX, float enemyY, float enemyWidth, float enemyHeight) const {
    return x > enemyX && x < enemyX + enemyWidth &&
        y > enemyY && y < enemyY + enemyHeight;
}

int Bullet::GetDamage() const {
    return damage;
}

bool Bullet::isEffectFinished() const {
    return hitEffectTime >= hitEffectDuration;
}

// RevolverBullet
RevolverBullet::RevolverBullet(float x, float y, float targetX, float targetY)
    : Bullet(x, y, targetX, targetY, 50, 1500.0f) {}

// HeadshotGunBullet
HeadshotGunBullet::HeadshotGunBullet(float x, float y, float targetX, float targetY)
    : Bullet(x, y, targetX, targetY, 100, 1500.0f) {}

// ClusterGunBullet
ClusterGunBullet::ClusterGunBullet(float x, float y, float targetX, float targetY)
    : Bullet(x, y, targetX, targetY, 75, 1500.0f) {}

// DualShotgunBullet
DualShotgunBullet::DualShotgunBullet(float x, float y, float targetX, float targetY, float spreadAngle)
    : Bullet(x, y, targetX, targetY, 100, 1500.0f) {
    // targetX와 targetY에 대한 방향 벡터를 계산
    float dx = targetX - x;
    float dy = targetY - y;
    float distance = sqrt(dx * dx + dy * dy);

    // 기본 방향 단위 벡터 계산
    float directionX = dx / distance;
    float directionY = dy / distance;

    // spreadAngle을 적용하여 방향을 조정
    float newDirectionX = directionX * cos(spreadAngle) - directionY * sin(spreadAngle);
    float newDirectionY = directionX * sin(spreadAngle) + directionY * cos(spreadAngle);

    // 새로운 방향에 따른 targetX, targetY 계산
    float newTargetX = x + newDirectionX * 100;
    float newTargetY = y + newDirectionY * 100;

    // Bullet의 방향 업데이트
    this->directionX = newDirectionX;
    this->directionY = newDirectionY;
}
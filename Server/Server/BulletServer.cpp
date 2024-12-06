#include "Bullet.h"
#include "Packet.h"

Bullet::Bullet(float x, float y, float targetX, float targetY, int damage, float speed) //서버 클라 분리
    : x(x), y(y), speed(speed), damage(damage), isHit(false), hitEffectDuration(0.25f), hitEffectTime(0.0f) {
    float dx = targetX - x;
    float dy = targetY - y;
    float distance = sqrt(dx * dx + dy * dy);
    directionX = dx / distance;
    directionY = dy / distance;

}
Bullet::~Bullet() { // 클라 서버
    for (auto& image : hitEffectImages) {
        image.Destroy();
    }
}

void Bullet::Update(float frameTime) { // 
    if (isHit) {
        UpdateHitEffect(frameTime); // 
    }
    else {
        x += directionX * speed * frameTime;
        y += directionY * speed * frameTime;
    }
}
void Bullet::UpdateHitEffect(float frameTime) {//서버
    hitEffectTime += frameTime;
}
//bool BulletServer::CheckCollision(float enemyX, float enemyY, float enemyWidth, float enemyHeight) const {
//    return Bullet::CheckCollision(enemyX, enemyY, enemyWidth, enemyHeight);
//}

bool Bullet::IsOutOfBounds(float width, float height) const {//서버
    return x < 0 || y < 0 || x > width || y > height;
}
bool Bullet::CheckCollision(float enemyX, float enemyY, float enemyWidth, float enemyHeight) const {//서버
    return x > enemyX && x < enemyX + enemyWidth &&
        y > enemyY && y < enemyY + enemyHeight;
}

int Bullet::GetDamage() const {//서버
    return damage;
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


/*
s_bulletPacket Bullet::GenerateStatePacket() const {
    s_bulletPacket packet;
    packet.s_bulletPosX = x;        // bullet.h  private 접근 위해서 getter
    packet.s_bulletPosY = getY();       
    packet.speed = getSpeed();          
    packet.directionX = getTargetX() - getX(); 
    packet.directionY = getTargetY() - getY(); 
    packet.damage = getDamage();     
    //packet.isActive = isActive;
    return packet;
}
*/

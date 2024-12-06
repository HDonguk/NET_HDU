#include "Bullet.h"

ClientBullet::ClientBullet(float x, float y, float targetX, float targetY, int damage, float speed)
    : Bullet(x, y, targetX, targetY, damage, speed) {
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
//update 서버

void ClientBullet::ReceiveStateFromServer(const s_bulletPacket& packet) { // 패킷 받기

    x = packet.s_bulletPosX;
    y = packet.s_bulletPosY;
    speed = packet.speed;
    damage = packet.damage;
    //isActive = packet.isActive;

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
bool Bullet::isEffectFinished() const {
    return hitEffectTime >= hitEffectDuration;
}




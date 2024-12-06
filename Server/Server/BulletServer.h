#pragma once

#include "Bullet.h"
#include "Packet.h"

class BulletServer : public Bullet {
public:
    BulletServer(float x, float y, float targetX, float targetY, int damage, float speed);

    // 상태 업데이트
    void Update(float frameTime);

    // 충돌 처리
    bool CheckCollision(float enemyX, float enemyY, float enemyWidth, float enemyHeight) const;

    // 총알 비활성화 여부
   // bool IsInactive() const;
    bool IsOutOfBounds(float width, float height) const;
    // 상태 패킷 생성
    s_bulletPacket GenerateStatePacket() const;

private:
    bool isActive; // 총알 활성화 여부
};

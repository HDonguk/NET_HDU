#pragma once

#include "Player.h"
#include <string>
#include "Packet.h"

class PlayerServer : public Player {
public:
    PlayerServer(int id, const std::string& name, float x, float y, float speed, int maxHealth);

    // 입력 처리
    void ProcessInput(const c_inputPacket& input);

    // 상태 패킷 생성
    s_playerPacket GenerateStatePacket() const;

private:
    int id;                // 플레이어 ID
    std::string name;      // 플레이어 이름
};

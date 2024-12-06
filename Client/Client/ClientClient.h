#pragma once

#include <vector>
#include <winsock2.h>
#include "ClientPlayer.h"
#include "ClientBullet.h"
#include "Packet.h"

class ClientClient {
public:
    ClientClient(SOCKET serverSocket);
    void updateFromServer(); // 서버에서 상태 업데이트
    void renderGameObjects(); // 게임 객체 렌더링 (예제)

private:
    SOCKET serverSocket;                    // 서버 소켓
    ClientPlayer player;                    // 플레이어
    std::vector<ClientBullet> bullets;      // 총알 리스트

    void updatePlayerFromServer(const s_playerPacket& packet);
    void updateBulletsFromServer(const s_bulletPacket& packet);
};

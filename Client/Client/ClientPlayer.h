#pragma once

#include "Player.h"
#include "Packet.h" // 패킷 구조 포함
#include <winsock2.h>

class ClientPlayer : public Player {
public:
    ClientPlayer(SOCKET serverSocket, float x, float y, float speed, int maxHealth);

    // 서버로 입력 전송
    void SendInputToServer(const std::string& input);

    // 서버로부터 상태 수신
    void ReceiveStateFromServer();

private:
    SOCKET serverSocket; // 서버와 연결된 소켓
};

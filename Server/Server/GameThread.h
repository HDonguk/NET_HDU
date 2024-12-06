#pragma once
#include <vector>
#include <queue>
#include <winsock2.h> // 올바른 위치로 이동
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Event.h"
#include "Packet.h"
#include "Utility.h"
#include "Obstacle.h" 
#include <chrono>
#include"stdafx.h"

class GameThread {
public:
    GameThread(); // 기본 생성자 추가
    GameThread(SOCKET serverSocket);
    ~GameThread();

    void run();  // 게임 루프를 실행
    void stop(); // 게임 루프를 종료

private:
    void collectEvents();  // 클라이언트 이벤트 수집
    void sortEventsByFrame(); // 이벤트를 프레임 기준으로 정렬
    void processEvent(const Event& event); // 수집된 이벤트 처리
    void updateGameObjects();  // 게임 객체 업데이트
    void checkCollisions(); // 충돌 감지 및 처리
    void sendUpdatedStateToClients(); // 클라이언트로 상태 전송
    void waitUntilNextFrame(const std::chrono::time_point<std::chrono::steady_clock>& frameStartTime); // 프레임 간 동기화

    bool gameRunning; // 게임 루프 실행 여부
    std::queue<Event> eventQueue; // 이벤트 큐

    // 게임 객체 목록
    std::vector<Player> players;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    std::vector<Obstacle*> obstacles; 

    // 서버 통신 관련 소켓
    SOCKET serverSocket; // 수정된 부분
};

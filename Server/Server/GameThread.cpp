//#include "Common.h"
#include "GameThread.h"
#include <iostream>      // 콘솔 입출력
#include <chrono>        // 프레임 간 시간 측정
#include <thread>        // sleep_for 사용
#include "stdafx.h"
#include  "list"
#include "Client.h"
extern list<c_inputPacket> sharedInputList; // 전역으로 한번 써보기
extern list<Client> waitClientList; // 클라이언트 리스트 전역 변수 정의
extern CRITICAL_SECTION cs;         // Critical Section 전역 변수 정의
// 30FPS 기준
#define FRAME_TIME 0.033f

GameThread::GameThread() {
    // 기본 생성자 정의 (필요에 따라 초기화 작업 추가)
    serverSocket = INVALID_SOCKET;
    std::cout << "기본 GameThread 생성자 호출" << std::endl;
}

GameThread::GameThread(SOCKET serverSocket)
    : serverSocket(serverSocket), gameRunning(false) {}

GameThread::~GameThread() {
    stop();
}


void GameThread::run() {
    gameRunning = true;
    cout << "gameThread run()" << endl;
    while (gameRunning) {
        auto frameStartTime = std::chrono::steady_clock::now();

    //1. updateEnemys
    //2. updateBullet
    //3. collisionAction
    //4. updatePlayerStatus
        cout << "updateGameObjects()" << endl;
        // 3. 게임 객체 업데이트
        updateGameObjects();

        // 4. 충돌 감지
        //checkCollisions();

        // 5. 클라이언트 상태 동기화
        sendUpdatedStateToClients();

        // 6. 프레임 간 동기화
        waitUntilNextFrame(frameStartTime);
    }
}

void GameThread::stop() {
    gameRunning = false;
}

void GameThread::collectEvents() {
    // 클라이언트로부터 이벤트 수집
    // 각 클라이언트에서 받은 패킷을 이벤트 큐에 추가
}

void GameThread::sortEventsByFrame() {
    // 이벤트 큐를 타임스탬프 또는 프레임 기준으로 정렬
}

/*
void GameThread::processEvent(const Event& event) {
    switch (event.type) {
    case EventType::PlayerMove:
        for (auto& player : players) {
            if (player.GetID() == event.playerID) {
                player.ProcessInput(event.inputData); // 입력 처리
            }
        }
        break;

    case EventType::FireBullet:
        bullets.emplace_back(event.bulletData.startX, event.bulletData.startY,
            event.bulletData.targetX, event.bulletData.targetY,
            event.bulletData.damage, event.bulletData.speed);
        break;

        // 추가 이벤트 처리 가능
    }
}
*/
void GameThread::updateGameObjects() {
    EnterCriticalSection(&cs);
    if (!sharedInputList.empty()) {
        // 공유 입력 리스트에서 입력 패킷 가져오기
        c_inputPacket inputPacket = sharedInputList.front();
        sharedInputList.pop_front();  // 가져온 입력 패킷 제거
        LeaveCriticalSection(&cs);

        // 각 플레이어 오브젝트 업데이트
        for (auto& player : players) {
            // 플레이어 입력을 반영한 업데이트
            player.Update(FRAME_TIME, inputPacket, obstacles);

            // 디버깅 로그
            std::cout << "게임 로직 처리: moveLeft=" << inputPacket.moveLeft
                << ", moveRight=" << inputPacket.moveRight
                << ", moveUp=" << inputPacket.moveUp
                << ", moveDown=" << inputPacket.moveDown << std::endl;
        }
    }
    else {
        std::cerr << "Warning: sharedInputList is empty!" << std::endl;
        LeaveCriticalSection(&cs);
    }


    for (auto& enemy : enemies) {
     //   enemy.Update(FRAME_TIME);
    }

    for (auto& bullet : bullets) {
    //   bullet.Update(FRAME_TIME);
    }
}
/*
void GameThread::checkCollisions() {
    for (auto& bullet : bullets) {
        for (auto& enemy : enemies) {
            if (bullet.CheckCollision(enemy.GetX(), enemy.GetY(), enemy.GetWidth(), enemy.GetHeight())) {
                enemy.TakeDamage(bullet.getDamage());
                bullet.SetInactive();
            }
        }
    }
}
*/
void GameThread::sendUpdatedStateToClients() {
    for (const auto& player : players) {
        s_playerPacket playerPacket = player.GenerateStatePacket();
        send(serverSocket, reinterpret_cast<char*>(&playerPacket), sizeof(playerPacket), 0);
    }

    /*
    for (const auto& bullet : bullets) {
        if (!bullet.IsInactive()) {
            s_bulletPacket bulletPacket = bullet.GenerateStatePacket();
            send(serverSocket, reinterpret_cast<char*>(&bulletPacket), sizeof(bulletPacket), 0);
        }
    }

    for (const auto& enemy : enemies) {
        s_enemyPacket enemyPacket = enemy.GenerateStatePacket();
        send(serverSocket, reinterpret_cast<char*>(&enemyPacket), sizeof(enemyPacket), 0);
    }
    */
}


void GameThread::waitUntilNextFrame(const std::chrono::time_point<std::chrono::steady_clock>& frameStartTime) {
    auto frameEndTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = frameEndTime - frameStartTime;

    if (elapsed.count() < FRAME_TIME) {
        std::this_thread::sleep_for(std::chrono::duration<float>(FRAME_TIME - elapsed.count()));
    }
}
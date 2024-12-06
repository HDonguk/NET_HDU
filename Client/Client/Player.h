#pragma once

#include <atlimage.h>
#include <vector>
#include "Obstacle.h"
#include "Enemy.h"      
#include "Camera.h"
#include <string>
#include "Packet.h" // 패킷 구조 포함
#include <winsock2.h>

class GameFramework;

class Player {
public:
    Player(float x, float y, float speed, float animationSpeed, GameFramework* gameFramework);
    ~Player();

    //ClientPlayer(SOCKET serverSocket, float x, float y, float speed, int maxHealth);
    void Update(SOCKET s);
    // 서버로 입력 전송
    void sendInputToServer(SOCKET s);

    // 서버로부터 상태 수신
    void ReceiveStateFromServer(SOCKET s);

    void Update(float frameTime, const std::vector<Obstacle*>& obstacles);
    void Move(float dx, float dy, const std::vector<Obstacle*>& obstacles);

    float GetX() const;
    float GetY() const;

    void SetBounds(float width, float height);
    void DrawBoundingBox(HDC hdc, float offsetX, float offsetY) const;

    void LoadImages();
    void Draw(HDC hdc, float offsetX, float offsetY);

    void SetDirectionLeft(bool isLeft);
    bool IsDirectionLeft() const;

    void AddExperience(int amount);
    void DrawExperienceBar(HDC hdc, RECT clientRect);

    void LevelUp();
    bool CheckCollision(float newX, float newY, const std::vector<Obstacle*>& obstacles) const;

    void ApplyUpgrade(const std::wstring& upgrade);

    float x, y;
    float speed;
    float animationSpeed;
    int currentFrame;
    float frameTimeAccumulator;
    bool moveLeft, moveRight, moveUp, moveDown;
    bool isMoving;
    bool directionLeft;

    float boundWidth, boundHeight;

    int level;
    int experience;
    int experienceToNextLevel;

    CImage idleImages[5];
    CImage runImages[4];
    CImage r_idleImages[5];
    CImage r_runImages[4];

    std::vector<CImage> levelUpEffectImages;
    float levelUpEffectTime;
    const float levelUpEffectDuration = 1.5f;

    // New health-related attributes
    int health;
    int maxHealth;
    float invincibilityTime;
    float currentInvincibilityTime;

    // Heart animation frames
    std::vector<CImage> heartImages;
    int heartAnimationFrame;
    float heartAnimationSpeed;
    float heartAnimationAccumulator;

    // New methods
    void TakeDamage(int amount);
    void DrawHealth(HDC hdc, float offsetX, float offsetY);
    void DrawInvincibilityIndicator(HDC hdc, float offsetX, float offsetY);
    bool IsInvincible() const;
    void UpdateInvincibility(float frameTime);

    //void sendInputToServer();

    GameFramework* gameFramework;
    SOCKET serverSocket; // 서버와 연결된 소켓
};

#pragma once

#include <atlimage.h>
#include <vector>
#include "Obstacle.h"
#include "Enemy.h"      
#include "Camera.h"
#include <string>
#include "Packet.h"
class GameFramework;

class Player {
public:
    Player(float x, float y, float speed, float animationSpeed, GameFramework* gameFramework);
    ~Player();

    void Update(float frameTime, const c_inputPacket& input, const std::vector<Obstacle*>& obstacles);
    void Move(float dx, float dy, const std::vector<Obstacle*>& obstacles);

    float GetX() const;
    float GetY() const;

    void SetBounds(float width, float height);
  //  void DrawBoundingBox(HDC hdc, float offsetX, float offsetY) const;

   // void LoadImages();
    void Draw(HDC hdc, float offsetX, float offsetY);

    void SetDirectionLeft(bool isLeft);
    bool IsDirectionLeft() const;

    void AddExperience(int amount);
   // void DrawExperienceBar(HDC hdc, RECT clientRect);

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
   // void DrawHealth(HDC hdc, float offsetX, float offsetY);
   // void DrawInvincibilityIndicator(HDC hdc, float offsetX, float offsetY);
    bool IsInvincible() const;
    void UpdateInvincibility(float frameTime);

    GameFramework* gameFramework;

    // 입력 처리
    void ProcessInput(const c_inputPacket& input);

    // 상태 패킷 생성
    s_playerPacket GenerateStatePacket() const;
    int id;                // 플레이어 ID
    std::string name;      // 플레이어 이름
};

#include "Player.h"
#include "GameFrameWork.h"

#include <cmath>
#include <iostream>

#define PlayerWidth 20.0f
#define PlayerHeight 25.0f

extern HFONT hFont;

Player::Player(float x, float y, float speed, float animationSpeed, GameFramework* gameFramework)
    : x(x), y(y), speed(speed), animationSpeed(animationSpeed), currentFrame(0), frameTimeAccumulator(0.0f),
    moveLeft(false), moveRight(false), moveUp(false), moveDown(false), isMoving(false),
    boundWidth(0), boundHeight(0), directionLeft(false),
    level(1), experience(0), experienceToNextLevel(100), levelUpEffectTime(0.0f),
    health(4), maxHealth(4), invincibilityTime(2.0f), currentInvincibilityTime(0.0f),
    heartAnimationFrame(0), heartAnimationSpeed(0.2f), heartAnimationAccumulator(0.0f),
    gameFramework(gameFramework) {
    LoadImages();
}

Player::~Player() {
}

void Player::Update(float frameTime, const std::vector<Obstacle*>& obstacles) {

    frameTimeAccumulator += frameTime;
    levelUpEffectTime -= frameTime;
    UpdateInvincibility(frameTime);

    if (frameTimeAccumulator >= animationSpeed) {
        if (isMoving) {
            currentFrame = (currentFrame + 1) % 4; // Run 애니메이션이 4 프레임이므로
        }
        else {
            currentFrame = (currentFrame + 1) % 5; // Idle 애니메이션이 5 프레임이므로
        }
        frameTimeAccumulator = 0.0f;
    }

    isMoving = false;
    if (moveLeft) { Move(-speed, 0, obstacles); isMoving = true; }
    if (moveRight) { Move(speed, 0, obstacles); isMoving = true; }
    if (moveUp) { Move(0, -speed, obstacles); isMoving = true; }
    if (moveDown) { Move(0, speed, obstacles); isMoving = true; }
}

void Player::Move(float dx, float dy, const std::vector<Obstacle*>& obstacles) {

    float newX = x + dx;
    float newY = y + dy;

    if (!CheckCollision(newX, newY, obstacles)) {
        x = newX;
        y = newY;
    }

    // 경계를 벗어나지 않도록 위치 제한
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > boundWidth - PlayerWidth) x = boundWidth - PlayerWidth * 2;
    if (y > boundHeight - PlayerHeight) y = boundHeight - PlayerHeight * 2;
}

bool Player::CheckCollision(float newX, float newY, const std::vector<Obstacle*>& obstacles) const {
    for (const auto& obstacle : obstacles) {
        float ox = obstacle->GetX();
        float oy = obstacle->GetY();
        float ow = obstacle->GetWidth();
        float oh = obstacle->GetHeight();

        if (newX < ox + ow &&
            newX + PlayerWidth > ox &&
            newY < oy + oh &&
            newY + PlayerHeight > oy) {
            return true;
        }
    }
    return false;
}

void Player::SetBounds(float width, float height) {
    boundWidth = width;
    boundHeight = height;
}

float Player::GetX() const {
    return x;
}

float Player::GetY() const {
    return y;
}

void Player::LoadImages() {
    idleImages[0].Load(L"./resources/player/Idle_0.png");
    idleImages[1].Load(L"./resources/player/Idle_1.png");
    idleImages[2].Load(L"./resources/player/Idle_2.png");
    idleImages[3].Load(L"./resources/player/Idle_3.png");
    idleImages[4].Load(L"./resources/player/Idle_4.png");

    runImages[0].Load(L"./resources/player/Run_0.png");
    runImages[1].Load(L"./resources/player/Run_1.png");
    runImages[2].Load(L"./resources/player/Run_2.png");
    runImages[3].Load(L"./resources/player/Run_3.png");

    r_idleImages[0].Load(L"./resources/player/rIdle_0.png");
    r_idleImages[1].Load(L"./resources/player/rIdle_1.png");
    r_idleImages[2].Load(L"./resources/player/rIdle_2.png");
    r_idleImages[3].Load(L"./resources/player/rIdle_3.png");
    r_idleImages[4].Load(L"./resources/player/rIdle_4.png");

    r_runImages[0].Load(L"./resources/player/rRun_0.png");
    r_runImages[1].Load(L"./resources/player/rRun_1.png");
    r_runImages[2].Load(L"./resources/player/rRun_2.png");
    r_runImages[3].Load(L"./resources/player/rRun_3.png");

    levelUpEffectImages.resize(9);
    levelUpEffectImages[0].Load(L"./resources/effect/T_LevelUpFX_0.png");
    levelUpEffectImages[1].Load(L"./resources/effect/T_LevelUpFX_1.png");
    levelUpEffectImages[2].Load(L"./resources/effect/T_LevelUpFX_2.png");
    levelUpEffectImages[3].Load(L"./resources/effect/T_LevelUpFX_3.png");
    levelUpEffectImages[4].Load(L"./resources/effect/T_LevelUpFX_4.png");
    levelUpEffectImages[5].Load(L"./resources/effect/T_LevelUpFX_5.png");
    levelUpEffectImages[6].Load(L"./resources/effect/T_LevelUpFX_6.png");
    levelUpEffectImages[7].Load(L"./resources/effect/T_LevelUpFX_7.png");
    levelUpEffectImages[8].Load(L"./resources/effect/T_LevelUpFX_8.png");

    heartImages.resize(4);
    heartImages[0].Load(L"./resources/ui/HeartAnimation_0.png");
    heartImages[1].Load(L"./resources/ui/HeartAnimation_1.png");
    heartImages[2].Load(L"./resources/ui/HeartAnimation_2.png");
    heartImages[3].Load(L"./resources/ui/HeartAnimation_3.png");
}

void Player::DrawHealth(HDC hdc, float offsetX, float offsetY) {
    heartAnimationAccumulator += heartAnimationSpeed;
    if (heartAnimationAccumulator >= 1.0f) {
        heartAnimationFrame = (heartAnimationFrame + 1) % 3; // Loop through 0, 1, 2
        heartAnimationAccumulator = 0.0f;
    }

    for (int i = 0; i < maxHealth; ++i) {
        int drawX = 20 + i * 40; // Adjust the spacing as needed
        int drawY = 40;
        if (i < health) {
            heartImages[heartAnimationFrame].Draw(hdc, drawX, drawY);
        }
        else {
            heartImages[3].Draw(hdc, drawX, drawY);
        }
    }
}

void Player::DrawExperienceBar(HDC hdc, RECT clientRect) {
    // 회색 배경 (전체 너비)
    RECT backgroundRect;
    backgroundRect.left = 0;
    backgroundRect.top = 0;
    backgroundRect.right = clientRect.right;
    backgroundRect.bottom = 25;

    HBRUSH grayBrush = CreateSolidBrush(RGB(128, 128, 128));
    FillRect(hdc, &backgroundRect, grayBrush);
    DeleteObject(grayBrush);

    // 연한 초록색 경험치 바 (현재 경험치에 비례한 너비)
    float percent = static_cast<float>(experience) / experienceToNextLevel;
    RECT experienceRect = backgroundRect;
    experienceRect.right = static_cast<LONG>(percent * clientRect.right);

    HBRUSH greenBrush = CreateSolidBrush(RGB(144, 238, 144));
    FillRect(hdc, &experienceRect, greenBrush);
    DeleteObject(greenBrush);

    HFONT hFont25 = CreateFont(
        -24,                      // Height of the font
        0,                        // Width of the font
        0,                        // Escapement angle
        0,                        // Orientation angle
        FW_NORMAL,                // Font weight
        FALSE,                    // Italic attribute
        FALSE,                    // Underline attribute
        FALSE,                    // Strikeout attribute
        ANSI_CHARSET,             // Character set identifier
        OUT_TT_PRECIS,            // Output precision
        CLIP_DEFAULT_PRECIS,      // Clipping precision
        ANTIALIASED_QUALITY,      // Output quality
        DEFAULT_PITCH | FF_DONTCARE,  // Pitch and family
        L"ChevyRay - Lantern"     // Font name
    );

    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont25);

    wchar_t levelText[20];
    swprintf_s(levelText, L"LEVEL %d", level);

    RECT textRect;
    textRect.left = 0;
    textRect.top = 0;
    textRect.right = clientRect.right;
    textRect.bottom = 30;
    DrawText(hdc, levelText, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // 원래 폰트로 복원하고 새로 생성한 폰트 삭제
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont25);
}

void Player::DrawInvincibilityIndicator(HDC hdc, float offsetX, float offsetY) {
    if (IsInvincible()) {
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(173, 216, 230)); // Light blue color
        HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH); // Transparent brush
        HGDIOBJ hOldPen = SelectObject(hdc, hPen);
        HGDIOBJ hOldBrush = SelectObject(hdc, hBrush);

        Ellipse(hdc, static_cast<int>(x + 10.0f - offsetX - 20), static_cast<int>(y + 12.5f - offsetY - 20), static_cast<int>(x + 10.0f - offsetX + 20), static_cast<int>(y + 12.5f - offsetY + 20));

        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hPen);
    }
}

void Player::DrawBoundingBox(HDC hdc, float offsetX, float offsetY) const {
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    MoveToEx(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY), NULL);
    LineTo(hdc, static_cast<int>(x - offsetX + PlayerWidth), static_cast<int>(y - offsetY));

    LineTo(hdc, static_cast<int>(x - offsetX + PlayerWidth), static_cast<int>(y - offsetY + PlayerHeight));

    LineTo(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY + PlayerHeight));

    LineTo(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

void Player::Draw(HDC hdc, float offsetX, float offsetY) {
    if (isMoving) {
        if (directionLeft) {
            if (!r_runImages[currentFrame].IsNull()) {
                r_runImages[currentFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
            }
        }
        else {
            if (!runImages[currentFrame].IsNull()) {
                runImages[currentFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
            }
        }
    }
    else {
        if (directionLeft) {
            if (!r_idleImages[currentFrame].IsNull()) {
                r_idleImages[currentFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
            }
        }
        else {
            if (!idleImages[currentFrame].IsNull()) {
                idleImages[currentFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
            }
        }
    }

    // 레벨업 이펙트 그리기
    if (levelUpEffectTime > 0) {
        int frame = static_cast<int>((levelUpEffectDuration - levelUpEffectTime) / levelUpEffectDuration * levelUpEffectImages.size());
        if (frame >= 0 && frame < levelUpEffectImages.size()) {
            if (frame <= 5) {
                // 번개 이펙트 위치
                int drawX = static_cast<int>(x + 10.0f - offsetX - levelUpEffectImages[frame].GetWidth() / 2);
                int drawY = static_cast<int>(y + 12.5f - offsetY - levelUpEffectImages[frame].GetHeight());
                levelUpEffectImages[frame].Draw(hdc, drawX, drawY);
            }
            else {
                // 날개 이펙트 위치
                int drawX = static_cast<int>(x + 10.0f - offsetX - levelUpEffectImages[frame].GetWidth() / 2);
                int drawY = static_cast<int>(y + 12.5f - offsetY - levelUpEffectImages[frame].GetHeight() / 2);
                levelUpEffectImages[frame].Draw(hdc, drawX, drawY);
            }
        }
    }
}

void Player::SetDirectionLeft(bool isLeft) {
    directionLeft = isLeft;
}

bool Player::IsDirectionLeft() const {
    return directionLeft;
}

void Player::AddExperience(int amount) {
    experience += amount;
    PlaySound(L"./resources/sounds/Obtain_Points.wav", NULL, SND_FILENAME | SND_ASYNC);
    while (experience >= experienceToNextLevel) {
        experience -= experienceToNextLevel;
        LevelUp();
    }
}

void Player::LevelUp() {
    PlaySound(L"./resources/sounds/LevelUp.wav", NULL, SND_FILENAME | SND_ASYNC);
    level++;
    // 경험치 요구량 = *1.5f
    experienceToNextLevel = static_cast<int>(experienceToNextLevel * 1.5f);
    levelUpEffectTime = levelUpEffectDuration;

    // 레벨업 할 시 기능추가
    if (gameFramework) {
        gameFramework->LevelUpUpgrade();
    }
}

bool Player::IsInvincible() const {
    return currentInvincibilityTime > 0;
}

void Player::UpdateInvincibility(float frameTime) {
    if (currentInvincibilityTime > 0) {
        currentInvincibilityTime -= frameTime;
        if (currentInvincibilityTime < 0) currentInvincibilityTime = 0;
    }
}

void Player::TakeDamage(int amount) {
    if (!IsInvincible()) {
        PlaySound(L"./resources/sounds/hurt.wav", NULL, SND_FILENAME | SND_ASYNC);
        health -= amount;
        if (health < 0) health = 0;
        currentInvincibilityTime = invincibilityTime;
    }
}

void Player::ApplyUpgrade(const std::wstring& upgrade) {
    if (upgrade == L"MaxHp +1") {
        maxHealth += 1;
        health += 1;
    }
    else if (upgrade == L"Add Speed") {
        speed += 0.5f;
    }
    // Other upgrade cases...
}
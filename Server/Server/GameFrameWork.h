#pragma once

#include <atlimage.h>
#include "Player.h"
#include "Camera.h"
#include "Gun.h"
#include "Enemy.h"
#include <vector>
#include "Obstacle.h"
#include "Bullet.h"
#include "Item.h"
#include <string>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

enum UpgradeOptions {
    MaxHp = 0,
    MaxAmmo,
    AddSpeed,
    UpgradeGun,
    UpgradeOptionsCount // This helps to know the number of options
};

class GameFramework {
public:
    GameFramework();
    ~GameFramework();

    void Create(HWND hWnd);
    void Update(float frameTime);
    void Draw(HDC hdc);
    void OnKeyBoardProcessing(UINT iMessage, WPARAM wParam, LPARAM lParam);
    void OnMouseProcessing(UINT iMessage, WPARAM wParam, LPARAM lParam);

    void Clear();

    void SpawnItem(float x, float y);

    // 정지 모드
    void TogglePause();
    void PauseKeyDown(WPARAM wParam);
    void OnKeyUp(WPARAM wParam);
    void PauseMenuSelect();

    // 레벨업 모드
    void LevelUpUpgrade();
    void ShowUpgradePanel();
    void HideUpgradePanel();
    void DrawUpgradePanel(HDC hdc);
    void HandleUpgradeInput();

    // 메인 메뉴
    void DrawMainMenu(HDC hdc);
    void ToggleMainMenu();
    void HandleMenuInput(WPARAM wParam);
    void ResetGame();

    void PlayGameSound(LPCWSTR soundFile);
    void PlayMainMenuMusic();
    void StopMainMenuMusic();
    void PlaySoundEffect(const std::wstring& soundFile);
    void PlayBackgroundMusic();
    void StopBackgroundMusic();
    //void TestBackgroundMusic(); 

    void StartCreateObstacles();

private:
    void InitializeDoubleBuffering(HDC hdc);
    void CleanupDoubleBuffering();

    void StartCreateEnemies();
    void CreateObstacles(int numObstacles);

    void FireBullet(float x, float y, float targetX, float targetY);

    void SpawnBrainMonster();
    void SpawnEyeMonster();
    void SpawnBigBoomer();
    void SpawnLamprey();
    void SpawnYog();
    void SpawnWingedMonster();

    bool f9Pressed = false;

    void SpawnWingedMonsterNearPlayer();
    void ClearEnemies();
    void HandleCheatKeys();

    void DrawBulletUI(HDC hdc);
    void DrawReloadingUI(HDC hdc);
    void DrawGameTime(HDC hdc);

    HWND m_hWnd;
    HDC m_hdcBackBuffer;
    HBITMAP m_hBitmap;
    HBITMAP m_hOldBitmap;

    CImage mapImage;
    Player* player;
    Camera* camera;

    Gun* currentGun;
    Revolver revolver;
    HeadshotGun headshotGun;
    ClusterGun clusterGun;
    DualShotgun dualShotgun;

    CImage cursorImage;
    CImage clickImage;
    bool showClickImage;
    float clickImageTimer;
    POINT cursorPos;

    float enemySpawnTimer;
    float bigBoomerSpawnTimer;
    float lampreySpawnTimer;
    float yogSpawnTimer;

    const float enemySpawnInterval = 10.0f;
    const float bigBoomerSpawnInterval = 30.0f;
    const float lampreySpawnInterval = 45.0f;
    const float yogSpawnInterval = 60.0f;

    std::vector<Enemy*> enemies;
    std::vector<Obstacle*> obstacles;
    std::vector<Bullet*> bullets;
    std::vector<Item*> items;

    float frameTime;  // 프레임 타임을 저장
    int gameTimeSeconds;

    CImage bulletUI;
    CImage bulletUsedUI;

    void HandleDebugKeys();

    bool f1Pressed = false;
    bool f2Pressed = false;
    bool f3Pressed = false;
    bool f4Pressed = false;

    // 정지 모드
    bool isPaused;
    CImage pauseUIImage;
    RECT resumeRect, mainMenuRect, quitRect;
    bool isResumeHovered, isMainMenuHovered, isQuitHovered;
    void DrawPauseMenu(HDC hdc);
    int selectedMenuIndex;

    // 업그레이드 모드
    bool isShowingUpgradePanel;
    int selectedUpgradePanel;
    UpgradeOptions upgradeOptions[2];
    CImage selectedPanelImage;
    CImage unselectedPanelImage;

    bool isUpgradePanelLoaded = false;

    std::wstring GetUpgradeOptionText(UpgradeOptions option);

    // 메인메뉴
    bool isMainMenu;
    int menuAnimationFrame;
    float menuAnimationAccumulator;
    int selectedMenuItem;
    CImage menuImages[3];

    bool isBackgroundMusicPlaying;
    bool isMainMenuMusicPlaying;


};

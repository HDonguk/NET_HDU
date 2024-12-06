// Gun.h
#pragma once
#include <atlimage.h> // CImage 사용을 위한 헤더
#include <windows.h>  // For POINT

class Gun {
public:
    Gun(int maxAmmo);
    virtual ~Gun();

    virtual void LoadImages() = 0;
    void Draw(HDC hdc, float playerX, float playerY, float cursorX, float cursorY, bool directionLeft);
    void Reload();
    bool FireBullet();
    bool IsReloading() const;
    void UpdateReload(float frameTime);

    int maxAmmo;
    int currentAmmo;
    bool reloading;
    float reloadTime;
    float reloadTimer;

    CImage gunImage;
    CImage r_gunImage;

    constexpr double DEG2RAD(double deg) const {
        return deg * (3.14159265358979323846 / 180.0);
    }

    CImage reloadImages[3]; // 장전 이미지를 저장할 배열
    int reloadFrame;
};

class Revolver : public Gun {
public:
    Revolver();
    virtual void LoadImages() override;
};

class HeadshotGun : public Gun {
public:
    HeadshotGun();
    virtual void LoadImages() override;
};

class ClusterGun : public Gun {
public:
    ClusterGun();
    virtual void LoadImages() override;
};

class DualShotgun : public Gun {
public:
    DualShotgun();
    virtual void LoadImages() override;
};

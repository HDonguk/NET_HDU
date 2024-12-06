#pragma once
#include <atlimage.h> 
#include <windows.h>

class Item {
public:
    Item(float x, float y);
    virtual ~Item();

    void Update(float frameTime);
    void Draw(HDC hdc, float offsetX, float offsetY);

    float GetX() const;
    float GetY() const;

    bool IsCollected() const;
    void SetCollected();

private:
    void LoadImages();

    float x, y;
    float animationTime;
    float switchTime;
    int currentFrame;

    CImage images[2];
    bool collected;
};

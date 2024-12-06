#include "Item.h"

Item::Item(float x, float y)
    : x(x), y(y), animationTime(0.0f), switchTime(0.25f), currentFrame(0), collected(false) {
    LoadImages();
}

Item::~Item() {
}

void Item::LoadImages() {
    images[0].Load(L"./resources/item/T_ChargeUp_0.png");
    images[1].Load(L"./resources/item/T_ChargeUp_1.png");
}

void Item::Update(float frameTime) {
    animationTime += frameTime;
    if (animationTime >= switchTime) {
        animationTime = 0.0f;
        currentFrame = (currentFrame + 1) % 2;
    }
}

void Item::Draw(HDC hdc, float offsetX, float offsetY) {
    if (!collected && !images[currentFrame].IsNull()) {
        images[currentFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
    }
}

float Item::GetX() const {
    return x;
}

float Item::GetY() const {
    return y;
}

bool Item::IsCollected() const {
    return collected;
}

void Item::SetCollected() {
    collected = true;
}

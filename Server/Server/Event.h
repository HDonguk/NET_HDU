// Event.h
#pragma once

enum class EventType {
    PlayerMove,
    FireBullet,
    CollectItem,
    EnemySpawn,
    Collision
};

struct Event {
    EventType type;
    int entityId;
    float posX;
    float posY;
    int damage;
};
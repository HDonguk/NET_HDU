// Packet.h
#pragma once
#include <iostream>
#include <string>
using namespace std;

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

extern SOCKET sock;
extern char buf[BUFSIZE + 1];
extern size_t retval;

// UI 패킷용
#define GAMESTART 20040
#define VICTORY 20041
#define DEFEAT 20042

class Client    // 합치면서 수정 예정
{
public:
	char name[20];
	int ID;

	Client()
	{
		*name = NULL;
		ID = -1;
	}
};

struct PlayerStatusPacket {
	unsigned short playerId;
	unsigned short health;
	float posX;
	float posY;
};


struct s_initPacket
{
	float s_obstacleX;
	float s_obstacleY;
	float s_playerInitX;
	float s_playerInitY;
	unsigned short s_playerMaxHealth;
	unsigned short s_initialAmmoCount;
};

struct s_enemyPacket
{
	unsigned short s_enemyID;
	unsigned short s_enemyType;
	float s_enemyPosX;
	float s_enemyPosY;
	unsigned short s_enemyHealth;
};

struct s_itemPacket
{
	unsigned short s_itemID;
	float s_itemPosX;
	float s_itemPosY;
	unsigned short s_itemType;
};

struct s_bulletPacket {
	float s_bulletPosX;        // 총알 위치 X
	float s_bulletPosY;        // 총알 위치 Y
	float speed;               // 총알 속도
	float directionX;          // 총알 방향 X
	float directionY;          // 총알 방향 Y
	int damage;                // 총알 데미지
	bool isActive;             // 총알 활성 상태
};

struct s_obstaclePacket
{
	float s_obstaclePosX;
	float s_obstaclePosY;
};

struct s_UIPacket
{
	unsigned short s_UIType;

	s_UIPacket() {}
	s_UIPacket(int _type)
	{
		s_UIType = _type;
	}
};

struct s_playerPacket
{
	string s_playerName;
	unsigned short s_playerID;
	float s_playerPosX;
	float s_playerPosY;
	float s_playerSpeed;
	unsigned short s_playerHealth;
	unsigned short s_playerLevel;
	int s_playerEXP;
	bool s_isPlayerDead;
};

struct c_playerPacket
{
	string c_playerName;
	unsigned short c_playerID;
	float c_playerPosX;
	float c_playerPosY;
};

struct c_bulletPacket
{
	float c_playerX;
	float c_playerY;
	unsigned short c_bulletID;
	float c_targetX;
	float c_targetY;
	unsigned short bulletDamage;
};

struct c_inputPacket
{
	unsigned short moveLeft;
	unsigned short moveRight;
	unsigned short moveUp;
	unsigned short moveDown;
	string c_key;
	float c_mouseX;
	float c_mouseY;
	unsigned short c_playerID;
};
// Packet.h
#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// UI ÆÐÅ¶¿ë
#define GAMESTART 20040
#define VICTORY 20041
#define DEFEAT 20042



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
/*
struct s_bulletPacket
{
	unsigned short s_bulletID;
	float s_bulletPosX;
	float s_bulletPosY;
	bool bulletIsActive;
};*/
struct s_bulletPacket {
	float s_bulletPosX;        // ÃÑ¾Ë À§Ä¡ X
	float s_bulletPosY;        // ÃÑ¾Ë À§Ä¡ Y
	float speed;               // ÃÑ¾Ë ¼Óµµ
	float directionX;          // ÃÑ¾Ë ¹æÇâ X
	float directionY;          // ÃÑ¾Ë ¹æÇâ Y
	int damage;                // ÃÑ¾Ë µ¥¹ÌÁö
	bool isActive;             // ÃÑ¾Ë È°¼º »óÅÂ
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
	string c_key;
	unsigned short moveLeft;
	unsigned short moveRight;
	unsigned short moveUp;
	unsigned short moveDown;
	float c_mouseX;
	float c_mouseY;
	unsigned short c_playerID;
};
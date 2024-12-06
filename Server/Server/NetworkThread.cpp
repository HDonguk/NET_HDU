#include "Common.h"
#include "packet.h"
#include <random>
#include <vector>
#include <queue>
using namespace std;

class Client
{
public:
	char* name;
	int ID;

	Client()
	{
		name = NULL;
		ID = -1;
	}
};

queue<Client> waitClientList;
HANDLE hMatchingEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
HANDLE hGameStartEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
bool gameOver = false;

// 랜덤 ID부여
int makeRandomID();
// 게임 데이터 받기
void receiveGameData(SOCKET s);
// 게임 데이터 전송
void sendGameData(SOCKET s);
// 게임 결과 전송
void sendResult(SOCKET s, int result);


DWORD WINAPI networkThread(LPVOID arg)
{
	SOCKET clientSock = (SOCKET)arg;
	int retval;
	Client client;

	// 닉네임 받기
	retval = recv(clientSock, client.name, 20, 0);
	if (retval == SOCKET_ERROR) err_display("receive - clientName");

	// ID 할당
	retval = send(clientSock, (char*)makeRandomID(), sizeof(client.ID), 0);
	if (retval == SOCKET_ERROR) err_display("send - clientID");

	while (true)
	{
		// 매칭 돌릴 때 까지 대기
		WaitForSingleObject(hMatchingEvent, INFINITE);

		// 큐에 입장
		waitClientList.push(client);

		// 게임 잡힐 때 까지 대기
		WaitForSingleObject(hGameStartEvent, INFINITE);

		// 게임 시작 신호 전송
		s_UIPacket start(GAMESTART);

		retval = send(clientSock, (char*)&start, sizeof(start), 0);
		if (retval == SOCKET_ERROR) err_display("send - s_UIPacket(start)");

		// initPacket 전송
		s_initPacket init;

		retval = send(clientSock, (char*)&init, sizeof(init), 0);
		if (retval == SOCKET_ERROR) err_display("send - initPacket");

		// 게임 루프
		while (!gameOver)
		{
			// 게임 데이터 받기
			receiveGameData(clientSock);

			// 게임 데이터 전송
			sendGameData(clientSock);
		}
		// 게임 결과 전송
		sendResult(clientSock, 0);
	}
}

int makeRandomID()
{
	return rand() / 1000;
}

void receiveGameData(SOCKET s)
{
	int retval;

	// c_playerPacket 받기
	c_playerPacket playerPacket;
	retval = recv(s, (char*)&playerPacket, sizeof(playerPacket), 0);
	if (retval == SOCKET_ERROR) err_display("receive - c_playetPacket");

	// c_bulletPacket 받기
	c_bulletPacket bulletPacket;
	retval = recv(s, (char*)&bulletPacket, sizeof(bulletPacket), 0);
	if (retval == SOCKET_ERROR) err_display("receive - c_bulletPacket");

	// c_inputPacket 받기
	c_inputPacket inputPacket;
	retval = recv(s, (char*)&inputPacket, sizeof(inputPacket), 0);
	if (retval == SOCKET_ERROR) err_display("receive - c_inputPacket");
}

void sendGameData(SOCKET s)
{
	int retval;

	s_enemyPacket enemyPacket;
	retval = recv(s, (char*)&enemyPacket, sizeof(enemyPacket), 0);
	if (retval == SOCKET_ERROR) err_display("send - enemyPacket");

	s_itemPacket itemPacket;
	retval = recv(s, (char*)&itemPacket, sizeof(itemPacket), 0);
	if (retval == SOCKET_ERROR) err_display("send - itemPacket");

	s_bulletPacket bulletPacket;
	retval = recv(s, (char*)&bulletPacket, sizeof(bulletPacket), 0);
	if (retval == SOCKET_ERROR) err_display("send - bulletPacket");

	s_obstaclePacket obstaclePacket;
	retval = recv(s, (char*)&obstaclePacket, sizeof(obstaclePacket), 0);
	if (retval == SOCKET_ERROR) err_display("send - obstaclePacket");

	s_UIPacket UIPacket;
	retval = recv(s, (char*)&UIPacket, sizeof(UIPacket), 0);
	if (retval == SOCKET_ERROR) err_display("send - UIPacket");

	s_playerPacket playerPacket;
	retval = recv(s, (char*)&playerPacket, sizeof(playerPacket), 0);
	if (retval == SOCKET_ERROR) err_display("send - playerPacket");
}

void sendResult(SOCKET s, int result)
{
	int retval;

	s_UIPacket UIPacket(result);
	retval = recv(s, (char*)&UIPacket, sizeof(UIPacket), 0);
	if (retval == SOCKET_ERROR) err_display("send - UIPacket");
}
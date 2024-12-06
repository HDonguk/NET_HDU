#include <iostream>
#include <string>

#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Bullet.h"
#include "Obstacle.h"
#include "Common.h"
#include "Packet.h"
#include <random>
#include <vector>
#include <list>
#include"Client.h"
#include "GameThread.h"
#define SERVERPORT 9000
#define BUFSIZE    512

using namespace std;

list<c_inputPacket> sharedInputList;
list<Client> waitClientList; // 전역(gameThread 에서 이용하기 위해) 수정필요
 CRITICAL_SECTION cs;                // 전역으로 Critical Section 선언 //Client 클래스 헤더파일로 분리 
 int nextID = 1; // 전역 변수 nextID 정의

//list<Client> waitClientList;
HANDLE hGameStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
bool gameOver = false;
//CRITICAL_SECTION cs;


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
	retval = recv(clientSock, client.name, sizeof(client.name), 0);
	if (retval == SOCKET_ERROR) {
		err_display("receive - clientName");
	}
	else {
		cout << "recv - name success - " << client.name << endl;
	}

	// ID 할당
	retval = send(clientSock, (char*)&client.ID, sizeof(client.ID), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send - clientID");
	}
	else {
		cout << "send - ID success - " << client.ID << endl;
	}
	while (true)
	{
		// 매칭 신호 수신
		unsigned short start;
		retval = recv(clientSock, (char*)&start, sizeof(start), MSG_WAITALL);
		if (retval == SOCKET_ERROR) err_display("receive - c_playetPacket");
		if (start != GAMESTART) {
			err_display("receive - start");
		}
		else {
			cout << client.ID << " - Matching start" << endl;
		}

		// 리스트에 입장
		EnterCriticalSection(&cs);		// 크리티컬섹션으로 다른스레드와 동기화
		waitClientList.push_back(client);
		LeaveCriticalSection(&cs);
		cout << "enter List" << endl;
		cout << waitClientList.size()<< "개의 클라이언트 대기중" << endl;


		// 게임 잡힐 때 까지 대기
		WaitForSingleObject(hGameStartEvent, INFINITE);
		waitClientList.remove(client);
		cout << "매칭 완료" << endl;

		// 게임 시작 신호 전송
		s_UIPacket gameStart(GAMESTART);
		retval = send(clientSock, (char*)&gameStart, sizeof(gameStart), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send - s_UIPacket(gameStart)");
		}
		else{
			cout << "Game start " << endl;
		}

		// initPacket 전송
		s_initPacket init;
		retval = send(clientSock, (char*)&init, sizeof(init), 0);
		if (retval == SOCKET_ERROR) { 
			err_display("send - initPacket"); 
		}
		else {
			cout << "send - initPacket success - " << client.ID << endl;
		}
		// 게임 루프
		
		while (!gameOver)
		{
			cout << "Game In " << endl;
			// 클라이언트로부터 입력 수신
			c_inputPacket inputPacket = {};
			int retval = recv(clientSock, (char*)&inputPacket, sizeof(inputPacket), 0);
			if (retval == SOCKET_ERROR) {
				std::cerr << "Failed to receive input from client. Error: " << WSAGetLastError() << std::endl;
				break;
			}
			else {
				std::cout << "Received input from client: moveLeft=" << inputPacket.moveLeft
					<< ", moveRight=" << inputPacket.moveRight
					<< ", moveUp=" << inputPacket.moveUp
					<< ", moveDown=" << inputPacket.moveDown << std::endl;
			}

			

			// 받은 입력을 공유 리스트에 추가
			EnterCriticalSection(&cs);
			sharedInputList.push_back(inputPacket);
			std::cout << "Input received: moveLeft=" << inputPacket.moveLeft
				<< ", moveRight=" << inputPacket.moveRight
				<< ", moveUp=" << inputPacket.moveUp
				<< ", moveDown=" << inputPacket.moveDown << std::endl;
			LeaveCriticalSection(&cs);

			// 게임 데이터 받기
			receiveGameData(clientSock);

			// 게임 데이터 전송
			sendGameData(clientSock);
		}
		// 게임 결과 전송
		sendResult(clientSock, 0);
		
	}
	closesocket(clientSock);
	nextID--;
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
	if (retval == SOCKET_ERROR) {
		err_display("receive - c_inputPacket");
	}
	else {
		// 단일 유저로 가정하고 waitClientList의 첫 번째 클라이언트를 사용 수정해야할 부분
		EnterCriticalSection(&cs);
		if (!waitClientList.empty()) {
			waitClientList.front().inputPacket = inputPacket; // 첫 번째 클라이언트의 입력 패킷 저장
		}
		LeaveCriticalSection(&cs);
	}
	
}

void sendGameData(SOCKET s)
{
	int retval;

	s_enemyPacket enemyPacket;
	retval = send(s, (char*)&enemyPacket, sizeof(enemyPacket), 0);
	if (retval == SOCKET_ERROR) {
		int errorCode = WSAGetLastError();
		std::cerr << "Send error: " << errorCode << std::endl;
	}
	else {
		std::cout << "Send success: " << retval << " bytes" << std::endl;
	}

	s_itemPacket itemPacket;
	retval = send(s, (char*)&itemPacket, sizeof(itemPacket), 0);
	if (retval == SOCKET_ERROR) err_display("send - itemPacket");

	s_bulletPacket bulletPacket;
	retval = send(s, (char*)&bulletPacket, sizeof(bulletPacket), 0);
	if (retval == SOCKET_ERROR) err_display("send - bulletPacket");

	s_obstaclePacket obstaclePacket;
	retval = send(s, (char*)&obstaclePacket, sizeof(obstaclePacket), 0);
	if (retval == SOCKET_ERROR) err_display("send - obstaclePacket");

	s_UIPacket UIPacket;
	retval = send(s, (char*)&UIPacket, sizeof(UIPacket), 0);
	if (retval == SOCKET_ERROR) err_display("send - UIPacket");

	s_playerPacket playerPacket;
	retval = send(s, (char*)&playerPacket, sizeof(playerPacket), 0);
	if (retval == SOCKET_ERROR) { // 이것만 일단 조정
		int errorCode = WSAGetLastError();
		std::cerr << "Send error: " << errorCode << std::endl;
	}
	else {
		std::cout << "Send success: " << retval << " bytes" << std::endl;
	}
}

void sendResult(SOCKET s, int result)
{
	int retval;

	s_UIPacket UIPacket(result);
	retval = send(s, (char*)&UIPacket, sizeof(UIPacket), 0);
	if (retval == SOCKET_ERROR) err_display("send - UIPacket");
}


DWORD WINAPI gameThread(LPVOID arg) {
	std::cout << "GameThread 생성" << std::endl;

	// 매칭 로직
	bool Matching = true;
	while (Matching) {
		EnterCriticalSection(&cs);
		if (waitClientList.size() == 1) {
			std::cout << "GameThread - Matching finish" << std::endl;
			Matching = false;
		}
		LeaveCriticalSection(&cs);
	}

	// 매칭 완료 후 게임 시작 이벤트 설정
	SetEvent(hGameStartEvent);
	std::cout << "gameThread start" << std::endl;

	// GameThread 객체 생성 및 게임 실행
	GameThread game;  // GameThread 객체 생성
	std::cout << "gameThread run in" << std::endl;
	game.run();       // 게임 실행

	return 0;
}

int main(int argc, char* argv[])
{
	InitializeCriticalSection(&cs);
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	printf("[알림] 윈속 초기화 성공\n");

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		err_quit("socket()");

	// bind
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		err_quit("bind()");

	// listen
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		err_quit("listen()");

	// 데이터 통신 변수
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread_g, hThread;
	// 게임스레드 생성
	CreateThread(NULL, 0, gameThread, NULL, 0, NULL); //4q

	while (1) {

		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			addr, ntohs(clientaddr.sin_port));

		// 네트워크스레드 생성
		hThread = CreateThread(NULL, 0, networkThread, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }

	}

	// 소켓 닫기
	closesocket(listen_sock);
	DeleteCriticalSection(&cs);

	// 윈속 종료
	WSACleanup();
	return 0;
}

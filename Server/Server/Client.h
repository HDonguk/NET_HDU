#pragma once
extern int nextID; // 전역 변수 nextID 선언
class Client
{
public:
	char name[20];
	int ID;
	SOCKET clientSock; // 클라이언트 소켓 추가
	c_inputPacket inputPacket; // 입력 패킷을 저장하는 필드 추가

	Client()
	{
		*name = NULL;
		ID = nextID++;	//생성자에서 ID 부여하게 수정
	}

	bool operator==(const Client& other) const {
		return name == other.name;
	}
};
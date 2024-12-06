#pragma once

#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <memory.h>
#include <tchar.h>
#include <atlimage.h> // CImage 사용을 위한 헤더
#include <vector>
#include <string>

// 필요한 헤더 파일 포함
#include "Player.h"            // 플레이어 클래스
#include "Enemy.h"             // 적 클래스
#include "Bullet.h"            // 총알 클래스
#include "Event.h"             // 이벤트 정의
#include "GameFramework.h"     // 게임 전체 관리 클래스
#include "Packet.h"
#include "Player.h"
#include "Utility.h"
#include "GameThread.h"

//gamethread 이용 헤더
#include "PlayerServer.h"
#include "BulletServer.h"


using namespace std;
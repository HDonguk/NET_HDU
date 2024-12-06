#pragma once

#include "Common.h"	//winsock 이 window.h 보다 먼저 포함되어야함
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <memory.h>
#include <tchar.h>
#include <atlimage.h> // CImage 사용을 위한 헤더
#include <vector>

// 필요한 헤더 파일 포함
#include "GameFramework.h"
#include "Client.h"
#include "Player.h"
#include "Camera.h"
#include "Enemy.h"
#include "Obstacle.h"
#include "Packet.h"
#include "ErrDisplay.h"

using namespace std;
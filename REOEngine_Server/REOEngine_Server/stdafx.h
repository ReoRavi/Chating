// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"


// 서버 라이브러리 링크
#pragma comment(lib, "ws2_32")

// 서버 헤더파일
#include <winsock2.h>

#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <process.h>
#include <conio.h>

// STL
#include <vector>
#include <list>
#include <algorithm>

// 하트비트의 최대 크기
#define MAX_HEARTBEAT		3

// Lobby
#define MAX_ROOMCOUNT	24	

// Room
#define MAX_ROOMUSERS	20

// 최대 유저 수
#define MAX_USER					300

// 유저의 상태
enum eUserState
{
	eUserState_NULL,
	eUserState_Lobby,
	eUserState_Room
};

#pragma comment(lib, "ReoLib.lib")

#include "../../ReoLib/ReoLib/Headers.h"
#include "../../ReoLib/ReoLib/MyHeaders.h"

#include "User.h"
#include "UserManager.h"

#include "Room.h"
#include "RoomManager.h"

#include "Lobby.h"

#include "ChatServer.h"

#include "PT_WritePacket.h"
#include "PT_ReadPacket.h"



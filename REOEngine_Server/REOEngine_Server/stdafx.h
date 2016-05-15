// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"


// ���� ���̺귯�� ��ũ
#pragma comment(lib, "ws2_32")

// ���� �������
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

// ��Ʈ��Ʈ�� �ִ� ũ��
#define MAX_HEARTBEAT		3

// Lobby
#define MAX_ROOMCOUNT	24	

// Room
#define MAX_ROOMUSERS	20

// �ִ� ���� ��
#define MAX_USER					300

// ������ ����
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



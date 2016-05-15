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
#include <conio.h>
#include <process.h>

// STL
#include <vector>
#include <string>
#include <algorithm>

enum eSceneType
{
	eScene_MainMenu,
	eScene_Login,
	eScene_Info,
	eScene_Lobby,
	eScene_CreateRoom,
	eScene_Room
};

// public
#define	MAX_XPOS			118
#define	MAX_YPOS			28

#define	UI_XPOS				MAX_XPOS / 2
#define	UI_YPOS				MAX_YPOS / 2

#define	KEY_UP					72
#define	KEY_DOWN			80
#define	KEY_LEFT				75
#define	KEY_RIGHT			77

#define	KEY_ENTER			13
#define	KEY_ESCAPE		27

// MainMenu Scene
#define	TITLE_XPOS			MAX_XPOS / 2	
#define	TITLE_YPOS			MAX_YPOS / 5

#define	MAIN_UICOUNT	3

#define	CURSOR_GAP		4

// ConnectToServer Scene
#define	CONNECT_UICOUNT		2

// Lobby Scene
#define	ROOMINDEX_XPOS		6
#define	HOSTNAME_XPOS			22
#define	ROOMSTATE_XPOS		60
#define	CURSOR_XPOS				64

#define	STATEUI_YPOS				2
#define	CURSOR_YPOS				3

#define	MAX_LOBBYUICOUNT	4
#define	MAX_ROOMCOUNT			CURSOR_YPOS + 20

// CreateRoom Scene
#define	BOX_YPOS			4
#define	BOX_SIZE				10

// Room Scene
#define	INPUT_YPOS		24
#define	MAX_CHATING		23

#define	NOTICE_YPOS		15

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#pragma comment(lib, "ReoLib.lib")


#include "../../ReoLib/ReoLib/Headers.h"
#include "../../ReoLib/ReoLib/MyHeaders.h"

// Packet
#include "PT_WritePacket.h"
#include "PT_ReadPacket.h"

#include "Client.h"

// UI
#include "UI.h"

#include "MainMenuUI.h"
#include "LoginUI.h"
#include "LobbyUI.h"
#include "CreateRoomUI.h"
#include "RoomUI.h"

// Scene
#include "Scene.h"
#include "Scene_MainMenu.h"
#include "Scene_Login.h"
#include "Scene_Lobby.h"
#include "Scene_CreateRoom.h"
#include "Scene_Room.h"

#include "SceneManager.h"

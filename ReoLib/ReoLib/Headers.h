#pragma once

// 서버 라이브러리 링크
#pragma comment(lib, "ws2_32")
#pragma comment(lib,"mswsock.lib")

// 서버 헤더파일
#include <winsock2.h>
#include <mswsock.h>

#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <process.h>
#include <conio.h>

// STL
#include <vector>

// 버퍼의 최대 크기
#define		MAX_BUFFER_LENGTH		256

// 서버 포트
#define		DEFAULT_PORT					1237
// 루프백  IP
#define		LOOPBACK_IP						"127.0.0.1"

// 쓰레드를 종료시킬 때 쓰이는 상수
#define		THREAD_QUIT						1

// XOREncrypt의 키의 갯수
#define		XOR_KEYCOUNT					29

// IOCP에서 어떤 입출력이 진행됬는지 알려줄 열거형
enum IO_TYPE
{
	IO_ACCPET,
	IO_READ,
	IO_WRITE
};

// OVERLAPPED 구조체.
typedef struct OVERLAPPED_DATA
{
	OVERLAPPED		overlapped;
	IO_TYPE				ioType;
	VOID*						object;
}	PER_IOCONTEXT, *PPER_IOCONTEXT;

// ePacket_씬이름_메세지
enum ePACKET_PROTOCOL
{
	ePacket_NULL,
	ePacket_Login_UserInfo,
	ePacket_Lobby_InfoUpdate,
	ePacket_Lobby_JoinRoom,
	ePacket_CreateRoom_Request,
	ePacket_Room_Message,
	ePacket_Room_Exit,
	ePacket_HeartBeat
};

#include "Log.h"

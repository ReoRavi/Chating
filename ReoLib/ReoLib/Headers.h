#pragma once

// ���� ���̺귯�� ��ũ
#pragma comment(lib, "ws2_32")
#pragma comment(lib,"mswsock.lib")

// ���� �������
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

// ������ �ִ� ũ��
#define		MAX_BUFFER_LENGTH		256

// ���� ��Ʈ
#define		DEFAULT_PORT					1237
// ������  IP
#define		LOOPBACK_IP						"127.0.0.1"

// �����带 �����ų �� ���̴� ���
#define		THREAD_QUIT						1

// XOREncrypt�� Ű�� ����
#define		XOR_KEYCOUNT					29

// IOCP���� � ������� ��������� �˷��� ������
enum IO_TYPE
{
	IO_ACCPET,
	IO_READ,
	IO_WRITE
};

// OVERLAPPED ����ü.
typedef struct OVERLAPPED_DATA
{
	OVERLAPPED		overlapped;
	IO_TYPE				ioType;
	VOID*						object;
}	PER_IOCONTEXT, *PPER_IOCONTEXT;

// ePacket_���̸�_�޼���
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

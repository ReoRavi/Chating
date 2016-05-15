#pragma once

class CLobby;

//=================================================
// CRoom
// Info : ��, �κ���� �������� �����Ѵ�.
class CRoom
{
public:
	CRoom();
	virtual ~CRoom();

private :
	// �� ���� �������� ������ List
	std::list<CUser*> users;

	// ���� ����
	CUser*		host;
	// �� �ȿ� �ִ� �������� ����
	INT				userCount;

	// ���� ��ȣ
	INT				roomIndex;
	// ���� �̸�
	std::string	roomName;

	// ���� Ȱ��ȭ��°�.
	BOOL			bActive;

public :
	// �ʱ�ȭ
	BOOL		Initialize(INT index);
	// �޸� ����
	BOOL		Release(VOID);

	// host
	CUser*	GetRoomHost(VOID);
	VOID		SetRoomHost(CUser* host);

	// userCount
	INT			GetRoomUserCount(VOID);
	VOID		SetRoomUserCount(INT count);

	// roomIndex
	INT			GetRoomIndex(VOID);
	VOID		SetRoomIndex(INT index);

	// roomName
	std::string	GetRoomName(VOID);
	VOID			SetRoomName(std::string roomName);

	// bActive
	BOOL		GetRoomActive(VOID);
	VOID		SetRoomActive(BOOL active);

	// �濡 �����Ѵ�.
	BOOL		ConnectToRoom(CUser* connectUser);
	// �濡�� ������.
	BOOL		DisconnectToRoom(CUser* disconnectUser);

	// ���� �ݴ´�.
	BOOL		CloseRoom(CLobby* lobby);

	// ���� �����Ѵ�.
	BOOL		ResetRoom(VOID);

	// �� ���� ��� �����鿡�� ��Ŷ�� �����Ѵ�.
	BOOL		SendToAll(DWORD length, ePACKET_PROTOCOL protocol, BYTE* buffer);
};


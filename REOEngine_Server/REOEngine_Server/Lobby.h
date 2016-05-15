#pragma once

//=================================================
// CLobby
// Info : ��, �κ���� �������� �����Ѵ�.
class CLobby
{
public:
	CLobby();
	virtual ~CLobby();

private :
	// ���� �����ϴ� �Ŵ��� ��ü.
	CRoomManager*	roomManager;
	// �κ���� �������� �����ϴ� List.
	std::list<CUser*>	users;
	// �κ���� �������� ī��Ʈ.
	INT							userCount;

public:
	// �ʱ�ȭ.
	BOOL	Initialize(VOID);
	// �޸� ����.
	BOOL	Release(VOID);

	// roomManager
	CRoomManager	*	GetRoomManager(VOID);
	VOID						SetRoomManager(CRoomManager* roomManager);

	// userCount 
	INT			GetUserCount(VOID);
	VOID		SetUserCount(INT count);

	// ������ �κ� �����Ѵ�.
	BOOL		ConnectToLobby(CUser* connectedUser);
	// ������ �κ񿡼� ���� �����Ѵ�.
	BOOL		DisconnectToLobby(CUser* disconnectedUser);

	// ���� �����.
	BOOL		CreateRoom(CUser* host, std::string roomName);

	// �κ���� ��� �������� ��Ŷ�� ������.
	BOOL		SendToAll(DWORD length, ePACKET_PROTOCOL protocol, BYTE* buffer);
};


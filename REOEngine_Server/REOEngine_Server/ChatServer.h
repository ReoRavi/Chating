#pragma once

//=================================================
// CChatServer
// Info : ���� ���� ��ü, IOCP�� ����Ѵ�.
class CChatServer : public CIocp
{
public:
	CChatServer();
	virtual ~CChatServer();

public :
	// ���� ���� ������Ʈ.
	CNetworkObject*	listenObject;
	// �κ�.
	CLobby*					lobby;
	// ���� ��ü�� �����ϴ� �Ŵ���.
	CUserManager		userManager;

	// KeepAlive ������ �ڵ�
	HANDLE					keepAliveHandle;
	// KeepAlive�� ������ �̺�Ʈ
	HANDLE					keepAliveDestroyEvent;

public :
	// �ʱ�ȭ
	BOOL		Initialize(VOID);
	// �޸� ����
	BOOL		Release(VOID);

	// ���� ����
	VOID		Loop(VOID);

	// ������ ���¸� ������.
	VOID		ShowDataTable(VOID);

public :
	// KeepAlive ������
	VOID	KeepAliveThread(VOID);

	// IOCP �����Լ��� ���
	BOOL	IOConnected(VOID* object);
	BOOL	IODisconnected(VOID* object);
	VOID	IOReceived(VOID* object, DWORD receivedByte);

public :
	// ��Ŷ ó�� �Լ��� ����

	// �α��� ��û.
	VOID PT_PROC_LOGIN_USERINFO(CUser* signaledObject, BYTE* buffer);
	// ���� �����.
	VOID PT_PROC_CREATEROOM_REQUEST(CUser* signaledObject, BYTE* buffer);
	// �κ� ���� ������Ʈ.
	VOID PT_PROC_LOBBY_INFOUPDATE(CUser* signaledObject, BYTE* buffer);
	// �濡 ������.
	VOID PT_PROC_LOBBY_JOINROOM(CUser* signaledObject, BYTE* buffer);
	// �濡 �޼����� ���޵�
	VOID PT_PROC_ROOM_MESSAGE(CUser* signaledObject, BYTE* buffer);
	// �濡�� ����
	VOID PT_PROC_ROOM_EXIT(CUser* signaledObject, BYTE* buffer);
	// Ŭ���̾�Ʈ ��Ʈ��Ʈ üũ
	VOID PT_PROC_HEARTBEAT_CHECK(CUser* signaledObject, BYTE* buffer);
};


#pragma once

//===================================================
// CClient
// Info - ���α׷��� ��Ʈ��ũ ó���� ����ϴ� ��ü,
// Singleton�� ��ӹ޾� ���α׷������� �ϳ��� �����ϴ� �ν��Ͻ��� ����.
class CClient :	public CClientSection,
							public ISingleton<CClient>	
{
public:
	CClient();
	virtual ~CClient();

public :
	// ���� ����
	BOOL		bConnected;

	// HeartBeat �������� �ڵ�
	HANDLE					heartbeatHandle;
	// HeartBeat �̺�Ʈ
	HANDLE					heartbeatEvent;

public :
	// �ʱ�ȭ
	BOOL		Initialize(VOID);
	// �޸� ����
	BOOL		Release(VOID);

	// ��Ʈ��Ʈ�� üũ�� ������
	VOID	HeartBeatThread(VOID);

public:
	// CClientSection�� �����Լ�
	// ������ �Ϸ�Ǿ��� �� ȣ��ȴ�.
	VOID	OnIOConnect(VOID);
	// ������ ������ �� ȣ��ȴ�.
	VOID	OnIODisconnect(VOID);

	// CClientSection�� �����Լ�, 
	// CClientSection���� ��Ŷ�� �ް� �� ��Ŷ�� ó���� ���۵��� ���⼭ �����Ѵ�.
	VOID	PacketProc(ePACKET_PROTOCOL protocol, BYTE* buffer);

	// �κ� ������ ������Ʈ ��.
	VOID	PT_PROC_LOBBY_INFOUPDATE(BYTE* buffer);
	// �� ���� ��û�� ���.
	VOID	PT_PROC_LOBBY_JOINROOM(BYTE* buffer);
	// �� ����� ��û�� ���.
	VOID	PT_PROC_CREATEROOM_REQUEST(BYTE* buffer);
	// �濡�� �޼����� ���޵�.
	VOID	PT_PROC_ROOM_MESSAGE(BYTE* buffer);		
	// �濡�� ������ ��û�� ���.
	VOID	PT_PROC_ROOM_EXIT(BYTE* buffer);
	// ��Ʈ��Ʈ üũ.
	VOID	PT_PROC_HEARTBEAT_CHECK(BYTE* buffer);
};

// �̱������� ��𼭵� ���� ������ �ν��Ͻ� ����
#define	g_pNetwork	CClient::GetInstance()

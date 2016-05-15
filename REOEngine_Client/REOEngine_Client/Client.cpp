#include "stdafx.h"
#include "Client.h"

//=================================================
// HeartBeatThreadCallBack
// Info : ��Ʈ��Ʈ �����带 �������� �ݹ��Լ�.
// parameter - CClient ��ü�� ���޵ȴ�.
unsigned WINAPI HeartBeatThreadCallBack(LPVOID parameter)
{
	CClient* server = reinterpret_cast<CClient*>(parameter);

	server->HeartBeatThread();

	return 0;
}

CClient::CClient()
{
}


CClient::~CClient()
{
}

//=================================================
// Initialize
// Info : �ʱ�ȭ
BOOL CClient::Initialize(VOID)
{
	// CClientSection �ʱ�ȭ 
	if (!CClientSection::InitializeTCP())
	{
		CLog::WriteLogWithDate("CClient : CClient - CClientSection::InitializeTCP() Error!\n");
		
		return FALSE;
	}

	bConnected = FALSE;

	return TRUE;
}

//=================================================
// Release
// Info : �޸� ����
BOOL CClient::Release(VOID)
{
	if (!CClientSection::Release())
	{
		CLog::WriteLogWithDate("CClient : Release - CClientSection::Release() Error!\n");

		return FALSE;
	}

	return TRUE;
}

//=================================================
// HeartBeatThread
// Info : ��Ʈ��Ʈ üũ�� ���� ������. 
// 3�е��� �̺�Ʈ�� �߻����� ������ ������ ������ ����ٰ� �Ǵ��ϰ� �����Ѵ�.
VOID CClient::HeartBeatThread(VOID)
{
	while (TRUE)
	{
		// 3�е��� ������ ���� �̺�Ʈ�� �߻����� ������ ��ȯ�Ѵ�.
		DWORD result = WaitForSingleObject(heartbeatEvent, 180000);

		// �ð��� �� �Ǿ �̺�Ʈ�� �߻����� �ʾҴ�.
		if (result == WAIT_TIMEOUT)
		{
			std::string error = "������ ������ �߻��߽��ϴ�. Enter�� ������ �����մϴ�.";
			COORD pos = { MAX_XPOS / 2 - error.length() / 2, 5 };

			system("cls");

			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
			printf("%s", error.c_str());
			getch();

			g_pSceneManager->run = FALSE;

		}
	}
}

//=================================================
// OnIOConnect
// Info : ���� ��û�� �������� �� ȣ��ȴ�.
VOID CClient::OnIOConnect(VOID)
{
	bConnected = TRUE;

	// ���� ��û�� �����ϸ� ��Ʈ��Ʈ �����带 �����Ų��.
	heartbeatHandle = (HANDLE)_beginthreadex(NULL, 0, HeartBeatThreadCallBack, this, 0, NULL);

	heartbeatEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (!heartbeatEvent)
	{
		CLog::WriteLogWithDate("CClient : OnIOConnect - CreateEvent() Error!\n");
	}
}

//=================================================
// OnIODisconnect
// Info : �������� ���� ��û ������� �߻����� �� ȣ��ȴ�.
VOID CClient::OnIODisconnect(VOID)
{
	// ������ Ŭ���̾�Ʈ���� �������� ���Ḧ �ϸ� �� �Լ��� ������� ������,
	// �������� ������ ���� �� �̻� ������� �Ұ��� ��쿡�� ���������� ����ó���� �Ǿ� �� �Լ��� �����Ѵ�.
	std::string error = "������ ������ �߻��߽��ϴ�. Enter�� ������ �����մϴ�.";
	COORD pos = { MAX_XPOS / 2 - error.length() / 2, 5 };

	system("cls");

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	printf("%s", error.c_str());
	getch();

	g_pSceneManager->run = FALSE;

}

//=================================================
// PacketProc
// Info : ���� ��Ŷ�� ���� ó���� ���۵��� ������.
// protocol - ��Ŷ�� ��������
// buffer - ��Ŷ�� ����
VOID CClient::PacketProc(ePACKET_PROTOCOL protocol, BYTE* buffer)
{
	// �������ݿ� ���� ó���Ѵ�.
	switch (protocol)
	{
	case ePACKET_PROTOCOL::ePacket_NULL:
		CLog::WriteLogWithDate("Error Packet send..\n");

		break;

	case ePACKET_PROTOCOL::ePacket_Lobby_InfoUpdate:
		PT_PROC_LOBBY_INFOUPDATE(buffer);

		break;

	case ePACKET_PROTOCOL::ePacket_Lobby_JoinRoom:
		PT_PROC_LOBBY_JOINROOM(buffer);

		break;

	case ePACKET_PROTOCOL::ePacket_CreateRoom_Request:
		PT_PROC_CREATEROOM_REQUEST(buffer);

		break;

	case ePACKET_PROTOCOL::ePacket_Room_Message:
		PT_PROC_ROOM_MESSAGE(buffer);

		break;

	case ePACKET_PROTOCOL::ePacket_Room_Exit:
		PT_PROC_ROOM_EXIT(buffer);

		break;

	case ePACKET_PROTOCOL::ePacket_HeartBeat:
		PT_PROC_HEARTBEAT_CHECK(buffer);
		break;

	default:
		break;
	}
}

//=================================================
// PT_PROC_LOBBY_INFOUPDATE
// Info : �κ� ������Ʈ ��Ŷ�� ó���Ѵ�.
// buffer - ��Ŷ�� ����
VOID CClient::PT_PROC_LOBBY_INFOUPDATE(BYTE* buffer)
{
	CScene_Lobby* lobby = reinterpret_cast<CScene_Lobby*>(g_pSceneManager->GetCurrentScene());

	INT roomCount = 0;
	INT roomIndex[MAX_ROOMCOUNT] = { 0, };
	std::string roomHost[MAX_ROOMCOUNT];
	std::string roomName[MAX_ROOMCOUNT];

	// ���� ����, ���� �ε���, ���� ȣ��Ʈ �̸�, ���� �̸��� �޴´�.
	PT_READ_LOBBY_INFOUPDATE(buffer, roomCount, roomIndex, roomHost, roomName);

	// ���� ���� �κ����� �˻��Ѵ�. (���������� �κ�� ǥ�õ����� �� ����� ȭ���� ���� �ִ�.)
	if (g_pSceneManager->GetCurrentSceneType() == eSceneType::eScene_Lobby)
	{
		// ���� ������Ʈ�� ���� �ٽ� �׸���.
		lobby->DrawAllSection();
		lobby->DrawCursor(lobby->cursorIndex);
		lobby->DrawRoomInfo(roomCount, roomIndex, roomHost, roomName);
	}
}

//=================================================
// PT_PROC_LOBBY_JOINROOM
// Info : �� ���� ��Ŷ�� ó���Ѵ�.
// buffer - ��Ŷ�� ����
VOID CClient::PT_PROC_LOBBY_JOINROOM(BYTE* buffer)
{
	CScene_Lobby* lobby = reinterpret_cast<CScene_Lobby*>(g_pSceneManager->GetCurrentScene());

	// ���� ���� ����
	BOOL bSuccess;

	PT_READ_LOBBY_JOINROOM(buffer, bSuccess);
	
	// ������ �����ߴٸ� ������ ���� �ٲ۴�.
	if (bSuccess)
		g_pSceneManager->ChangeScene(eSceneType::eScene_Room);
}

//=================================================
// PT_PROC_CREATEROOM_REQUEST
// Info : �� ���� ��û�� ó���Ѵ�. 
// buffer - ��Ŷ�� ����
VOID CClient::PT_PROC_CREATEROOM_REQUEST(BYTE* buffer)
{
	CScene_CreateRoom* createRoom = reinterpret_cast<CScene_CreateRoom*>(g_pSceneManager->GetCurrentScene());
	
	// ���� ���� ����
	BOOL	bSuccess;

	PT_READ_CREATEROOM_REQUEST(buffer, bSuccess);

	// ���� ���δ� �Լ� �ȿ��� ó���Ѵ�.
	createRoom->RequestResult(bSuccess);
}

//=================================================
// PT_PROC_ROOM_MESSAGE
// Info : �濡 �޼����� ���ŵǾ���.
// buffer - ��Ŷ�� ����
VOID CClient::PT_PROC_ROOM_MESSAGE(BYTE* buffer)
{
	CScene_Room*		room = reinterpret_cast<CScene_Room*>(g_pSceneManager->GetCurrentScene());
	std::string				message;

	PT_READ_ROOM_MESSAGE(buffer, message);

	// �޼����� �濡 ����Ѵ�.
	room->ReceiveChating(message);
}

//=================================================
// PT_PROC_ROOM_EXIT
// Info : �濡�� ���� ��û�� ó���Ѵ�.
// buffer - ��Ŷ�� ����
VOID CClient::PT_PROC_ROOM_EXIT(BYTE* buffer)
{
	printf("Enter�� �����ø� �κ�� �̵��մϴ�.");
	getchar();
	g_pSceneManager->ChangeScene(eSceneType::eScene_Lobby);
}

//=================================================
// PT_PROC_HEARTBEAT_CHECK
// Info : ��Ʈ��Ʈ ó���� �Ѵ�.
// buffer - ��Ŷ�� ����
VOID CClient::PT_PROC_HEARTBEAT_CHECK(BYTE* buffer)
{
	// ��Ʈ��Ʈ ��Ŷ�� �޾ұ� ������, �������� ����ִٰ� ������ ���ش�.
	WritePacket(0, ePACKET_PROTOCOL::ePacket_HeartBeat, NULL);

	// �̺�Ʈ�� �߻����� ��Ʈ��Ʈ�� �ʱ�ȭ�Ѵ�.
	SetEvent(heartbeatEvent);
}


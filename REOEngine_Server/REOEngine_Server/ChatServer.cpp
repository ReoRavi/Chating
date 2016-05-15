#include "stdafx.h"
#include "ChatServer.h"

//=================================================
// KeepThreadCallBack
// Info : KeepAlive �����带 �������� �ݹ�.
// parameter - CChatServer ��ü
unsigned WINAPI KeepThreadCallBack(LPVOID parameter)
{
	CChatServer* server = reinterpret_cast<CChatServer*>(parameter);

	server->KeepAliveThread();

	return 0;
}

CChatServer::CChatServer()
{
}


CChatServer::~CChatServer()
{
}

//=================================================
// Initialize
// Info : �ʱ�ȭ
BOOL CChatServer::Initialize(VOID)
{
	// IOCP �ʱ�ȭ
	if (!CIocp::Initialize())
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - CIocp::Initialize() Error!\n");

		return FALSE;
	}

	listenObject = new CNetworkObject();

	// ���� ��ü �ʱ�ȭ
	if (!listenObject->Initialize())
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - listenObject->Initialize() Error!\n");

		return FALSE;
	}

	// TCP ������ �����
	if (!listenObject->TCPSocket())
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - listenObject->TCPSocket() Error!\n");

		return FALSE;
	}

	// ���Ͽ� �ּ� �Ҵ�
	if (!listenObject->Bind(DEFAULT_PORT))
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - listenObject->Bind() Error!\n");

		return FALSE;
	}

	// ������ �����·� ��ȯ
	if (!listenObject->Listen(SOMAXCONN))
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - listenObject->Listen() Error!\n");

		return FALSE;
	}

	// ������ IOCP�� ����Ѵ�.
	if (!CIocp::RegisterSocketToIocp(listenObject->GetSocket(), reinterpret_cast<ULONG_PTR>(listenObject)))
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - CIocp::RegisterSocketToIocp Error!\n");

		return FALSE;
	}

	// KeepAlive ������ ����.
	keepAliveHandle = (HANDLE)_beginthreadex(NULL, 0, KeepThreadCallBack, this, 0, NULL);

	// KeepAlive �����带 �����ų �̺�Ʈ ����
	keepAliveDestroyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (!keepAliveDestroyEvent)
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - CreateEvent Error!\n");

		return FALSE;
	}

	lobby = new CLobby();

	// �κ� �ʱ�ȭ
	if (!lobby->Initialize())
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - lobby->Initialize() Error!\n");

		return FALSE;
	}

	// ���� �Ŵ��� �ʱ�ȭ
	if (!userManager.Initialize(listenObject->GetSocket()))
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - userManager.Initialize() Error!\n");

		return FALSE;
	}

	return TRUE;
}

//=================================================
// Release
// Info : �޸� ����
BOOL CChatServer::Release(VOID)
{
	if (keepAliveHandle)
	{
		SetEvent(keepAliveDestroyEvent);
	
		CloseHandle(keepAliveHandle);
	}

	if (listenObject->Release())
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - listenObject->Release()) Error!\n");

		return FALSE;
	}

	if (!userManager.Release())
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - userManager.Release() Error!\n");
		
		return FALSE;
	}

	if (CIocp::Release())
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - CIocp::Release() Error!\n");

		return FALSE;
	}

	return TRUE;
}

//=================================================
// Loop
// Info : ����
VOID CChatServer::Loop(VOID)
{
	while (TRUE)
	{
		// ������ ��Ȳ�� ������. �ǽð����� ������Ʈ
		ShowDataTable();
		system("cls");
	}
}

//=================================================
// ShowDataTable
// Info : ������ ��Ȳ�� ������. ����� ������ �����ߴ���, �κ�, ���� ����
VOID CChatServer::ShowDataTable(VOID)
{
	COORD pos = { 0, 0 };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

	// �� ���� �� 
	printf("Total User Count : %d\n", userManager.userCount);
	// �κ� �ִ� ���� ��
	printf("Lobby User Count : %d\n", lobby->GetUserCount());
	// ���� ��
	printf("Room Count : %d\n", lobby->GetRoomManager()->GetActiveRoomCount());

	CRoomManager* roomManager = lobby->GetRoomManager();

	INT count = 0;
	
	// Ȱ��ȭ�� ���� ������ �� ���� ������ �����
	for (int index = 0; index < MAX_ROOMCOUNT; index++)
	{
		if (roomManager->GetRoom(index)->GetRoomActive())
		{
			CRoom* room = roomManager->GetRoom(index);

			printf("%s Room(%d) User Count : %d\n", room->GetRoomName().c_str(), room->GetRoomIndex(), room->GetRoomUserCount());
		
			count++;
		}

		if (count >= lobby->GetRoomManager()->GetActiveRoomCount())
			break;
	}
}

//=================================================
// KeepAliveThread
// Info : KeepAlive ������, Ŭ���̾�Ʈ�� ����ִ����� �׽�Ʈ�Ѵ�.
VOID CChatServer::KeepAliveThread(VOID)
{
	while (TRUE)
	{
		// 1�е��� ������ ���� �̺�Ʈ�� �߻����� ������ ��ȯ�Ѵ�.
		DWORD result = WaitForSingleObject(keepAliveDestroyEvent, 10000);

		// Destoy �̺�Ʈ�� �߻��Ͽ���.
		if (result == WAIT_OBJECT_0)
			return;

		// ��Ʈ��Ʈ üũ
		userManager.CheckAllHeartBeat(listenObject->GetSocket());

		// ��Ʈ��Ʈ üũ ��Ŷ�� ������.
		userManager.SendToAll(0, ePACKET_PROTOCOL::ePacket_HeartBeat, NULL);
	}
}

//=================================================
// IOConnected
// Info : IOCP�� �����Լ�, �����û�� ���� �� ����ȴ�.
// object - ������� �Ͼ CUser ��ü
BOOL	CChatServer::IOConnected(VOID* object)
{
	CUser* signaledObject = reinterpret_cast<CUser*>(object);

	// ������ �Ϸ�� ������ IOCP�� ����Ѵ�.
	if (!CIocp::RegisterSocketToIocp(signaledObject->GetSocket(), reinterpret_cast<ULONG_PTR>(signaledObject)))
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - CIocp::RegisterSocketToIocp() Error!\n");

		return FALSE;
	}

	// ������ �ޱ����� �ʱ�ȭ�Ѵ�.
	if (!signaledObject->SetReadFromIOCP())
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - signaledObject->SetReadFromIOCP() Error!\n");

		return FALSE;
	}

	// ���� ó��
	signaledObject->SetConnected(TRUE);

	// �κ� ����
	lobby->ConnectToLobby(signaledObject);

	userManager.userCount++;

	return TRUE;
}

//=================================================
// IODisconnected
// Info : IOCP�� �����Լ�, ���� ���ᰡ �Ͼ�� �� ����ȴ�.
// object - ������� �Ͼ CUser ��ü
BOOL	CChatServer::IODisconnected(VOID* object)
{
	CUser* signaledObject = reinterpret_cast<CUser*>(object);

	// ������ �κ� �־��ٸ�
	if (signaledObject->GetUserState() == eUserState::eUserState_Lobby)
		lobby->DisconnectToLobby(signaledObject);
	// ������ �濡 �־��ٸ�
	else if (signaledObject->GetUserState() == eUserState::eUserState_Room)
		signaledObject->GetConnectedRoom()->DisconnectToRoom(signaledObject);

	// ���� ������Ʈ�� �ʱ�ȭ�ϰ�, �ٽ� ������� �� �ֵ��� �����Ѵ�.
	signaledObject->Restore(listenObject->GetSocket());

	userManager.userCount--;

	return TRUE;
}

//=================================================
// IOReceived
// Info :  IOCP�� �����Լ�, ������ �Ͼ�� �� ����ȴ�.
// object - ������� �Ͼ CUser ��ü
// receivedByte - ���� �������� ũ��
VOID CChatServer::IOReceived(VOID* object, DWORD receivedByte)
{
	CUser* signaledObject = reinterpret_cast<CUser*>(object);

	// IOCP�κ��� ��Ŷ�� �о���δ�.
	// ��� ��Ŷ�� �޾��� �� TRUE�� ���ϵȴ�.
	if (signaledObject->GetPacketFromIOCP(receivedByte))
	{
		DWORD								packetLength = 0;
		ePACKET_PROTOCOL	protocol;
		BYTE									buffer[MAX_BUFFER_LENGTH] = { 0, };

		// ��Ŷ�� �� �ް�, ��Ŷ�� �м��Ѵ�.
		if (signaledObject->AnalysisPacket(packetLength, protocol, buffer))
		{
			// �������ݿ� ���� ��Ŷ�� ó���Ѵ�.
			switch (protocol)
			{
			case ePACKET_PROTOCOL::ePacket_NULL :

				break;

			case ePACKET_PROTOCOL::ePacket_Login_UserInfo:
				PT_PROC_LOGIN_USERINFO(signaledObject, buffer);

				break;

			case ePACKET_PROTOCOL::ePacket_Lobby_InfoUpdate:
				PT_PROC_LOBBY_INFOUPDATE(signaledObject, buffer);

				break;

			case ePACKET_PROTOCOL::ePacket_Lobby_JoinRoom:
				PT_PROC_LOBBY_JOINROOM(signaledObject, buffer);

				break;

			case ePACKET_PROTOCOL::ePacket_CreateRoom_Request:
				PT_PROC_CREATEROOM_REQUEST(signaledObject, buffer);

				break;

			case ePACKET_PROTOCOL::ePacket_Room_Message :
				PT_PROC_ROOM_MESSAGE(signaledObject, buffer);

				break;

			case ePACKET_PROTOCOL::ePacket_Room_Exit:
				PT_PROC_ROOM_EXIT(signaledObject, buffer);

				break;

			case ePACKET_PROTOCOL::ePacket_HeartBeat:
				PT_PROC_HEARTBEAT_CHECK(signaledObject, buffer);

				break;

			default:
				break;
			}
		}

		// ��Ŷ�� �� �޾������� ���۸� �ʱ�ȭ�Ѵ�.
		signaledObject->ResetPacket();
	}
	
	// �ٽ� ������ ���� �� �ֵ��� �ʱ�ȭ�Ѵ�.
	signaledObject->SetReadFromIOCP();
}

//=================================================
// PT_PROC_LOGIN_USERINFO
// Info :  Ŭ���̾�Ʈ���� �α��� ��û�� ���Դ�.
// signaledObject - ������� �Ͼ CUser ��ü
// buffer - ���Ź��� ������ ����
VOID CChatServer::PT_PROC_LOGIN_USERINFO(CUser* signaledObject, BYTE* buffer)
{
	// ���� �̸�
	std::string userName;

	// ���� �̸��� �޴´�.
	PT_READ_LOGIN_USERINFO(buffer, userName);

	// ������ �̸��� ����Ѵ�.
	signaledObject->SetSectionUserName(userName);
}

//=================================================
// PT_PROC_LOBBY_INFOUPDATE
// Info : Ŭ���̾�Ʈ�κ��� �κ��� ���� ������Ʈ ��û�� ���Դ�.
// signaledObject - ������� �Ͼ CUser ��ü
// buffer - ���Ź��� ������ ����
VOID CChatServer::PT_PROC_LOBBY_INFOUPDATE(CUser* signaledObject, BYTE* buffer)
{
	BYTE sendBuffer[MAX_BUFFER_LENGTH] = { 0, };

	// �κ��� ������ ������Ʈ�ϰ�, ������.
	signaledObject->WritePacket(PT_WRITE_LOBBY_INFOUPDATE(sendBuffer, lobby->GetRoomManager()),
		ePACKET_PROTOCOL::ePacket_Lobby_InfoUpdate,
		sendBuffer);
}


//=================================================
// PT_PROC_LOBBY_JOINROOM
// Info : Ŭ���̾�Ʈ�� �濡 �����ϰڴٴ� ��û�� �ߴ�.
// signaledObject - ������� �Ͼ CUser ��ü
// buffer - ���Ź��� ������ ����
VOID CChatServer::PT_PROC_LOBBY_JOINROOM(CUser* signaledObject, BYTE* buffer)
{
	// ���� �ε���
	INT index;

	PT_READ_LOBBY_JOINROOM(buffer, index);

	// ���� �޾ƿ´�.
	CRoom* room = lobby->GetRoomManager()->GetRoom(index);

	// ���� Ȱ��ȭ�������� �˻�.
	if (room->GetRoomActive())
	{
		// �濡 ����.
		BOOL bSuccess = room->ConnectToRoom(signaledObject);

		// �濡 ���ٸ�
		if (bSuccess)
		{
			// �κ񿡼� ������.
			lobby->DisconnectToLobby(signaledObject);

			BYTE sendBuffer[MAX_BUFFER_LENGTH] = { 0, };

			// �濡 ���������� ���ٰ� ��Ŷ�� ������.
			signaledObject->WritePacket(PT_WRITE_LOBBY_JOINROOM(sendBuffer, bSuccess), ePACKET_PROTOCOL::ePacket_Lobby_JoinRoom, sendBuffer);
		}
	}
}

//=================================================
// PT_PROC_CREATEROOM_REQUEST
// Info : Ŭ���̾�Ʈ�� ���� ����ڴٴ� ��û�� �ߴ�.
// signaledObject - ������� �Ͼ CUser ��ü
// buffer - ���Ź��� ������ ����
VOID CChatServer::PT_PROC_CREATEROOM_REQUEST(CUser* signaledObject, BYTE* buffer)
{
	// ���� �̸�
	std::string roomName;
	// ���� ����
	BOOL	bSuccess;

	// �� �̸��� �д´�.
	PT_READ_CREATEROOM_REQUEST(buffer, roomName);

	bSuccess = lobby->CreateRoom(signaledObject, roomName);

	// �� ����Ⱑ �����ߴٸ�
	if (bSuccess)
	{
		BYTE sendBuffer[MAX_BUFFER_LENGTH] = { 0, };

		// �� ����Ⱑ �����ߴٰ� ��Ŷ�� ������.
		signaledObject->WritePacket(
			PT_WRITE_CREATEROOM_REQUEST(sendBuffer, bSuccess),
			ePACKET_PROTOCOL::ePacket_CreateRoom_Request,
			sendBuffer);

		// �κ񿡼� ������.
		lobby->DisconnectToLobby(signaledObject);

		ZeroMemory(sendBuffer, sizeof(sendBuffer));

		// ���� �����Ǿ� �κ� ������ ������Ʈ�Ǿ���.
		lobby->SendToAll(
			PT_WRITE_LOBBY_INFOUPDATE(sendBuffer, lobby->GetRoomManager()),
			ePACKET_PROTOCOL::ePacket_Lobby_InfoUpdate,
			sendBuffer);
	}
}

//=================================================
// PT_PROC_ROOM_MESSAGE
// Info : �濡 �޼����� ���މ��.
// signaledObject - ������� �Ͼ CUser ��ü
// buffer - ���Ź��� ������ ����
VOID CChatServer::PT_PROC_ROOM_MESSAGE(CUser* signaledObject, BYTE* buffer)
{
	// ���ŵ� �޼���
	std::string message;

	// �޼����� �޴´�.
	PT_READ_ROOM_MESSAGE(buffer, message);

	BYTE sendBuffer[MAX_BUFFER_LENGTH] = { 0, };

	// �濡�ִ� ��� �����鿡�� �޼����� ������.
	signaledObject->GetConnectedRoom()->SendToAll(PT_WRITE_ROOM_MESSAGE(sendBuffer, signaledObject->GetSectionUserName(), message), ePACKET_PROTOCOL::ePacket_Room_Message, sendBuffer);
}

//=================================================
// PT_PROC_ROOM_EXIT
// Info : Ŭ���̾�Ʈ�� �濡�� ������ ��û�ߴ�.
// signaledObject - ������� �Ͼ CUser ��ü
// buffer - ���Ź��� ������ ����
VOID CChatServer::PT_PROC_ROOM_EXIT(CUser* signaledObject, BYTE* buffer)
{
	BYTE sendBuffer[MAX_BUFFER_LENGTH] = { 0, };

	// Ŭ���̾�Ʈ�� �������ִ� ��
	CRoom*	room = signaledObject->GetConnectedRoom();

	// ȣ��Ʈ��� ���� ���Ľ�Ų��.
	if (room->GetRoomHost() == signaledObject)
	{
		room->CloseRoom(lobby);
	}
	// ȣ��Ʈ�� �ƴ϶�� �濡�� ȥ�� ������.
	else
	{
		// �濡�� ������.
		room->DisconnectToRoom(signaledObject);

		signaledObject->WritePacket(0, ePACKET_PROTOCOL::ePacket_Room_Exit, NULL);

		lobby->ConnectToLobby(signaledObject);
	}
}

//=================================================
// PT_PROC_HEARTBEAT_CHECK
// Info : Ŭ���̾�Ʈ�� ����ִ��� ��Ʈ��Ʈ üũ�� �Ѵ�.
// signaledObject - ������� �Ͼ CUser ��ü
// buffer - ���Ź��� ������ ����
VOID CChatServer::PT_PROC_HEARTBEAT_CHECK(CUser* signaledObject, BYTE* buffer)
{
	// Ŭ���̾�Ʈ���� ��ȣ�� �ͼ� ��Ʈ��Ʈ�� �ʱ�ȭ�ߴ�.
	signaledObject->SetHeartBeat(0);
}

#include "stdafx.h"
#include "ChatServer.h"

//=================================================
// KeepThreadCallBack
// Info : KeepAlive 쓰레드를 실행해줄 콜백.
// parameter - CChatServer 객체
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
// Info : 초기화
BOOL CChatServer::Initialize(VOID)
{
	// IOCP 초기화
	if (!CIocp::Initialize())
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - CIocp::Initialize() Error!\n");

		return FALSE;
	}

	listenObject = new CNetworkObject();

	// 서버 객체 초기화
	if (!listenObject->Initialize())
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - listenObject->Initialize() Error!\n");

		return FALSE;
	}

	// TCP 소켓을 사용함
	if (!listenObject->TCPSocket())
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - listenObject->TCPSocket() Error!\n");

		return FALSE;
	}

	// 소켓에 주소 할당
	if (!listenObject->Bind(DEFAULT_PORT))
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - listenObject->Bind() Error!\n");

		return FALSE;
	}

	// 소켓을 대기상태로 전환
	if (!listenObject->Listen(SOMAXCONN))
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - listenObject->Listen() Error!\n");

		return FALSE;
	}

	// 소켓을 IOCP에 등록한다.
	if (!CIocp::RegisterSocketToIocp(listenObject->GetSocket(), reinterpret_cast<ULONG_PTR>(listenObject)))
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - CIocp::RegisterSocketToIocp Error!\n");

		return FALSE;
	}

	// KeepAlive 쓰레드 시작.
	keepAliveHandle = (HANDLE)_beginthreadex(NULL, 0, KeepThreadCallBack, this, 0, NULL);

	// KeepAlive 쓰레드를 종료시킬 이벤트 생성
	keepAliveDestroyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (!keepAliveDestroyEvent)
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - CreateEvent Error!\n");

		return FALSE;
	}

	lobby = new CLobby();

	// 로비 초기화
	if (!lobby->Initialize())
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - lobby->Initialize() Error!\n");

		return FALSE;
	}

	// 유저 매니저 초기화
	if (!userManager.Initialize(listenObject->GetSocket()))
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - userManager.Initialize() Error!\n");

		return FALSE;
	}

	return TRUE;
}

//=================================================
// Release
// Info : 메모리 해제
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
// Info : 루프
VOID CChatServer::Loop(VOID)
{
	while (TRUE)
	{
		// 서버의 상황을 보여줌. 실시간으로 업데이트
		ShowDataTable();
		system("cls");
	}
}

//=================================================
// ShowDataTable
// Info : 서버의 상황을 보여줌. 몇명의 유저가 접속했는지, 로비, 방의 상태
VOID CChatServer::ShowDataTable(VOID)
{
	COORD pos = { 0, 0 };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

	// 총 유저 수 
	printf("Total User Count : %d\n", userManager.userCount);
	// 로비에 있는 유저 수
	printf("Lobby User Count : %d\n", lobby->GetUserCount());
	// 방의 수
	printf("Room Count : %d\n", lobby->GetRoomManager()->GetActiveRoomCount());

	CRoomManager* roomManager = lobby->GetRoomManager();

	INT count = 0;
	
	// 활성화된 방이 있으면 그 방의 정보를 출력함
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
// Info : KeepAlive 쓰레드, 클라이언트가 살아있는지를 테스트한다.
VOID CChatServer::KeepAliveThread(VOID)
{
	while (TRUE)
	{
		// 1분동안 쓰레드 종료 이벤트가 발생하지 않으면 반환한다.
		DWORD result = WaitForSingleObject(keepAliveDestroyEvent, 10000);

		// Destoy 이벤트가 발생하였다.
		if (result == WAIT_OBJECT_0)
			return;

		// 하트비트 체크
		userManager.CheckAllHeartBeat(listenObject->GetSocket());

		// 하트비트 체크 패킷을 보낸다.
		userManager.SendToAll(0, ePACKET_PROTOCOL::ePacket_HeartBeat, NULL);
	}
}

//=================================================
// IOConnected
// Info : IOCP의 가상함수, 연결요청이 왔을 때 실행된다.
// object - 입출력이 일어난 CUser 객체
BOOL	CChatServer::IOConnected(VOID* object)
{
	CUser* signaledObject = reinterpret_cast<CUser*>(object);

	// 연결이 완료된 소켓을 IOCP에 등록한다.
	if (!CIocp::RegisterSocketToIocp(signaledObject->GetSocket(), reinterpret_cast<ULONG_PTR>(signaledObject)))
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - CIocp::RegisterSocketToIocp() Error!\n");

		return FALSE;
	}

	// 수신을 받기위해 초기화한다.
	if (!signaledObject->SetReadFromIOCP())
	{
		CLog::WriteLogWithDate("CChatServer : Initialize - signaledObject->SetReadFromIOCP() Error!\n");

		return FALSE;
	}

	// 연결 처리
	signaledObject->SetConnected(TRUE);

	// 로비에 연결
	lobby->ConnectToLobby(signaledObject);

	userManager.userCount++;

	return TRUE;
}

//=================================================
// IODisconnected
// Info : IOCP의 가상함수, 연결 종료가 일어났을 때 실행된다.
// object - 입출력이 일어난 CUser 객체
BOOL	CChatServer::IODisconnected(VOID* object)
{
	CUser* signaledObject = reinterpret_cast<CUser*>(object);

	// 유저가 로비에 있었다면
	if (signaledObject->GetUserState() == eUserState::eUserState_Lobby)
		lobby->DisconnectToLobby(signaledObject);
	// 유저가 방에 있었다면
	else if (signaledObject->GetUserState() == eUserState::eUserState_Room)
		signaledObject->GetConnectedRoom()->DisconnectToRoom(signaledObject);

	// 유저 오브젝트를 초기화하고, 다시 연결받을 수 있도록 복구한다.
	signaledObject->Restore(listenObject->GetSocket());

	userManager.userCount--;

	return TRUE;
}

//=================================================
// IOReceived
// Info :  IOCP의 가상함수, 수신이 일어났을 때 실행된다.
// object - 입출력이 일어난 CUser 객체
// receivedByte - 받은 데이터의 크기
VOID CChatServer::IOReceived(VOID* object, DWORD receivedByte)
{
	CUser* signaledObject = reinterpret_cast<CUser*>(object);

	// IOCP로부터 패킷을 읽어들인다.
	// 모든 패킷을 받았을 때 TRUE가 리턴된다.
	if (signaledObject->GetPacketFromIOCP(receivedByte))
	{
		DWORD								packetLength = 0;
		ePACKET_PROTOCOL	protocol;
		BYTE									buffer[MAX_BUFFER_LENGTH] = { 0, };

		// 패킷을 다 받고, 패킷을 분석한다.
		if (signaledObject->AnalysisPacket(packetLength, protocol, buffer))
		{
			// 프로토콜에 따라 패킷을 처리한다.
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

		// 패킷을 다 받았음으로 버퍼를 초기화한다.
		signaledObject->ResetPacket();
	}
	
	// 다시 수신을 받을 수 있도록 초기화한다.
	signaledObject->SetReadFromIOCP();
}

//=================================================
// PT_PROC_LOGIN_USERINFO
// Info :  클라이언트에서 로그인 요청이 들어왔다.
// signaledObject - 입출력이 일어난 CUser 객체
// buffer - 수신받은 데이터 버퍼
VOID CChatServer::PT_PROC_LOGIN_USERINFO(CUser* signaledObject, BYTE* buffer)
{
	// 유저 이름
	std::string userName;

	// 유저 이름을 받는다.
	PT_READ_LOGIN_USERINFO(buffer, userName);

	// 유저의 이름을 등록한다.
	signaledObject->SetSectionUserName(userName);
}

//=================================================
// PT_PROC_LOBBY_INFOUPDATE
// Info : 클라이언트로부터 로비의 정보 업데이트 요청이 들어왔다.
// signaledObject - 입출력이 일어난 CUser 객체
// buffer - 수신받은 데이터 버퍼
VOID CChatServer::PT_PROC_LOBBY_INFOUPDATE(CUser* signaledObject, BYTE* buffer)
{
	BYTE sendBuffer[MAX_BUFFER_LENGTH] = { 0, };

	// 로비의 정보를 업데이트하고, 보낸다.
	signaledObject->WritePacket(PT_WRITE_LOBBY_INFOUPDATE(sendBuffer, lobby->GetRoomManager()),
		ePACKET_PROTOCOL::ePacket_Lobby_InfoUpdate,
		sendBuffer);
}


//=================================================
// PT_PROC_LOBBY_JOINROOM
// Info : 클라이언트가 방에 접속하겠다는 요청을 했다.
// signaledObject - 입출력이 일어난 CUser 객체
// buffer - 수신받은 데이터 버퍼
VOID CChatServer::PT_PROC_LOBBY_JOINROOM(CUser* signaledObject, BYTE* buffer)
{
	// 방의 인덱스
	INT index;

	PT_READ_LOBBY_JOINROOM(buffer, index);

	// 방을 받아온다.
	CRoom* room = lobby->GetRoomManager()->GetRoom(index);

	// 방이 활성화상태인지 검사.
	if (room->GetRoomActive())
	{
		// 방에 들어간다.
		BOOL bSuccess = room->ConnectToRoom(signaledObject);

		// 방에 들어갔다면
		if (bSuccess)
		{
			// 로비에서 나간다.
			lobby->DisconnectToLobby(signaledObject);

			BYTE sendBuffer[MAX_BUFFER_LENGTH] = { 0, };

			// 방에 성공적으로 들어갔다고 패킷을 보낸다.
			signaledObject->WritePacket(PT_WRITE_LOBBY_JOINROOM(sendBuffer, bSuccess), ePACKET_PROTOCOL::ePacket_Lobby_JoinRoom, sendBuffer);
		}
	}
}

//=================================================
// PT_PROC_CREATEROOM_REQUEST
// Info : 클라이언트가 방을 만들겠다는 요청을 했다.
// signaledObject - 입출력이 일어난 CUser 객체
// buffer - 수신받은 데이터 버퍼
VOID CChatServer::PT_PROC_CREATEROOM_REQUEST(CUser* signaledObject, BYTE* buffer)
{
	// 방의 이름
	std::string roomName;
	// 성공 여부
	BOOL	bSuccess;

	// 방 이름을 읽는다.
	PT_READ_CREATEROOM_REQUEST(buffer, roomName);

	bSuccess = lobby->CreateRoom(signaledObject, roomName);

	// 방 만들기가 성공했다면
	if (bSuccess)
	{
		BYTE sendBuffer[MAX_BUFFER_LENGTH] = { 0, };

		// 방 만들기가 성공했다고 패킷을 보낸다.
		signaledObject->WritePacket(
			PT_WRITE_CREATEROOM_REQUEST(sendBuffer, bSuccess),
			ePACKET_PROTOCOL::ePacket_CreateRoom_Request,
			sendBuffer);

		// 로비에서 나간다.
		lobby->DisconnectToLobby(signaledObject);

		ZeroMemory(sendBuffer, sizeof(sendBuffer));

		// 방이 생성되어 로비 정보가 업데이트되었다.
		lobby->SendToAll(
			PT_WRITE_LOBBY_INFOUPDATE(sendBuffer, lobby->GetRoomManager()),
			ePACKET_PROTOCOL::ePacket_Lobby_InfoUpdate,
			sendBuffer);
	}
}

//=================================================
// PT_PROC_ROOM_MESSAGE
// Info : 방에 메세지가 전달됬다.
// signaledObject - 입출력이 일어난 CUser 객체
// buffer - 수신받은 데이터 버퍼
VOID CChatServer::PT_PROC_ROOM_MESSAGE(CUser* signaledObject, BYTE* buffer)
{
	// 수신된 메세지
	std::string message;

	// 메세지를 받는다.
	PT_READ_ROOM_MESSAGE(buffer, message);

	BYTE sendBuffer[MAX_BUFFER_LENGTH] = { 0, };

	// 방에있는 모든 유저들에게 메세지를 보낸다.
	signaledObject->GetConnectedRoom()->SendToAll(PT_WRITE_ROOM_MESSAGE(sendBuffer, signaledObject->GetSectionUserName(), message), ePACKET_PROTOCOL::ePacket_Room_Message, sendBuffer);
}

//=================================================
// PT_PROC_ROOM_EXIT
// Info : 클라이언트가 방에서 나감을 요청했다.
// signaledObject - 입출력이 일어난 CUser 객체
// buffer - 수신받은 데이터 버퍼
VOID CChatServer::PT_PROC_ROOM_EXIT(CUser* signaledObject, BYTE* buffer)
{
	BYTE sendBuffer[MAX_BUFFER_LENGTH] = { 0, };

	// 클라이언트가 접속해있던 방
	CRoom*	room = signaledObject->GetConnectedRoom();

	// 호스트라면 방을 폭파시킨다.
	if (room->GetRoomHost() == signaledObject)
	{
		room->CloseRoom(lobby);
	}
	// 호스트가 아니라면 방에서 혼자 나간다.
	else
	{
		// 방에서 나간다.
		room->DisconnectToRoom(signaledObject);

		signaledObject->WritePacket(0, ePACKET_PROTOCOL::ePacket_Room_Exit, NULL);

		lobby->ConnectToLobby(signaledObject);
	}
}

//=================================================
// PT_PROC_HEARTBEAT_CHECK
// Info : 클라이언트가 살아있는지 하트비트 체크를 한다.
// signaledObject - 입출력이 일어난 CUser 객체
// buffer - 수신받은 데이터 버퍼
VOID CChatServer::PT_PROC_HEARTBEAT_CHECK(CUser* signaledObject, BYTE* buffer)
{
	// 클라이언트에서 신호가 와서 하트비트를 초기화했다.
	signaledObject->SetHeartBeat(0);
}

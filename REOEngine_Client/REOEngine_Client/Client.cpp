#include "stdafx.h"
#include "Client.h"

//=================================================
// HeartBeatThreadCallBack
// Info : 하트비트 쓰레드를 실행해줄 콜백함수.
// parameter - CClient 객체가 전달된다.
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
// Info : 초기화
BOOL CClient::Initialize(VOID)
{
	// CClientSection 초기화 
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
// Info : 메모리 해제
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
// Info : 하트비트 체크를 위한 쓰레드. 
// 3분동안 이벤트가 발생하지 않으면 서버에 오류가 생겼다고 판단하고 종료한다.
VOID CClient::HeartBeatThread(VOID)
{
	while (TRUE)
	{
		// 3분동안 쓰레드 종료 이벤트가 발생하지 않으면 반환한다.
		DWORD result = WaitForSingleObject(heartbeatEvent, 180000);

		// 시간이 다 되어도 이벤트가 발생하지 않았다.
		if (result == WAIT_TIMEOUT)
		{
			std::string error = "서버에 오류가 발생했습니다. Enter를 누르면 종료합니다.";
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
// Info : 접속 요청이 성공했을 때 호출된다.
VOID CClient::OnIOConnect(VOID)
{
	bConnected = TRUE;

	// 접속 요청이 성공하면 하트비트 쓰레드를 실행시킨다.
	heartbeatHandle = (HANDLE)_beginthreadex(NULL, 0, HeartBeatThreadCallBack, this, 0, NULL);

	heartbeatEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (!heartbeatEvent)
	{
		CLog::WriteLogWithDate("CClient : OnIOConnect - CreateEvent() Error!\n");
	}
}

//=================================================
// OnIODisconnect
// Info : 서버에서 종료 요청 입출력이 발생했을 때 호출된다.
VOID CClient::OnIODisconnect(VOID)
{
	// 보통은 클라이언트에서 정상적인 종료를 하면 이 함수가 실행되지 않지만,
	// 서버에서 문제가 닫혀 더 이상 입출력이 불가할 경우에는 서버측에서 종료처리가 되어 이 함수를 실행한다.
	std::string error = "서버에 오류가 발생했습니다. Enter를 누르면 종료합니다.";
	COORD pos = { MAX_XPOS / 2 - error.length() / 2, 5 };

	system("cls");

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	printf("%s", error.c_str());
	getch();

	g_pSceneManager->run = FALSE;

}

//=================================================
// PacketProc
// Info : 받은 패킷을 통해 처리할 동작들을 정의함.
// protocol - 패킷의 프로토콜
// buffer - 패킷의 버퍼
VOID CClient::PacketProc(ePACKET_PROTOCOL protocol, BYTE* buffer)
{
	// 프로토콜에 따라 처리한다.
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
// Info : 로비 업데이트 패킷을 처리한다.
// buffer - 패킷의 버퍼
VOID CClient::PT_PROC_LOBBY_INFOUPDATE(BYTE* buffer)
{
	CScene_Lobby* lobby = reinterpret_cast<CScene_Lobby*>(g_pSceneManager->GetCurrentScene());

	INT roomCount = 0;
	INT roomIndex[MAX_ROOMCOUNT] = { 0, };
	std::string roomHost[MAX_ROOMCOUNT];
	std::string roomName[MAX_ROOMCOUNT];

	// 방의 갯수, 방의 인덱스, 방의 호스트 이름, 방의 이름을 받는다.
	PT_READ_LOBBY_INFOUPDATE(buffer, roomCount, roomIndex, roomHost, roomName);

	// 현재 씬이 로비인지 검사한다. (서버에서는 로비로 표시되지만 방 만들기 화면일 수도 있다.)
	if (g_pSceneManager->GetCurrentSceneType() == eSceneType::eScene_Lobby)
	{
		// 정보 업데이트를 위해 다시 그린다.
		lobby->DrawAllSection();
		lobby->DrawCursor(lobby->cursorIndex);
		lobby->DrawRoomInfo(roomCount, roomIndex, roomHost, roomName);
	}
}

//=================================================
// PT_PROC_LOBBY_JOINROOM
// Info : 방 접속 패킷을 처리한다.
// buffer - 패킷의 버퍼
VOID CClient::PT_PROC_LOBBY_JOINROOM(BYTE* buffer)
{
	CScene_Lobby* lobby = reinterpret_cast<CScene_Lobby*>(g_pSceneManager->GetCurrentScene());

	// 접속 성공 여부
	BOOL bSuccess;

	PT_READ_LOBBY_JOINROOM(buffer, bSuccess);
	
	// 접속이 성공했다면 방으로 씬을 바꾼다.
	if (bSuccess)
		g_pSceneManager->ChangeScene(eSceneType::eScene_Room);
}

//=================================================
// PT_PROC_CREATEROOM_REQUEST
// Info : 방 생성 요청을 처리한다. 
// buffer - 패킷의 버퍼
VOID CClient::PT_PROC_CREATEROOM_REQUEST(BYTE* buffer)
{
	CScene_CreateRoom* createRoom = reinterpret_cast<CScene_CreateRoom*>(g_pSceneManager->GetCurrentScene());
	
	// 생성 성공 여부
	BOOL	bSuccess;

	PT_READ_CREATEROOM_REQUEST(buffer, bSuccess);

	// 성공 여부는 함수 안에서 처리한다.
	createRoom->RequestResult(bSuccess);
}

//=================================================
// PT_PROC_ROOM_MESSAGE
// Info : 방에 메세지가 수신되었다.
// buffer - 패킷의 버퍼
VOID CClient::PT_PROC_ROOM_MESSAGE(BYTE* buffer)
{
	CScene_Room*		room = reinterpret_cast<CScene_Room*>(g_pSceneManager->GetCurrentScene());
	std::string				message;

	PT_READ_ROOM_MESSAGE(buffer, message);

	// 메세지를 방에 출력한다.
	room->ReceiveChating(message);
}

//=================================================
// PT_PROC_ROOM_EXIT
// Info : 방에서 나감 요청을 처리한다.
// buffer - 패킷의 버퍼
VOID CClient::PT_PROC_ROOM_EXIT(BYTE* buffer)
{
	printf("Enter를 누르시면 로비로 이동합니다.");
	getchar();
	g_pSceneManager->ChangeScene(eSceneType::eScene_Lobby);
}

//=================================================
// PT_PROC_HEARTBEAT_CHECK
// Info : 하트비트 처리를 한다.
// buffer - 패킷의 버퍼
VOID CClient::PT_PROC_HEARTBEAT_CHECK(BYTE* buffer)
{
	// 하트비트 패킷을 받았기 때문에, 서버에게 살아있다고 응답을 해준다.
	WritePacket(0, ePACKET_PROTOCOL::ePacket_HeartBeat, NULL);

	// 이벤트를 발생시켜 하트비트를 초기화한다.
	SetEvent(heartbeatEvent);
}


#include "Headers.h"
#include "EventSelect.h"

//====================================================
// EventSelectThreadCallBack
// Info : 쓰레드를 실행시키기 위한 CallBack 함수.
// parameter - CEventSelect 객체를 전달함.
unsigned WINAPI EventSelectThreadCallBack(LPVOID parameter)
{
	// 형변환 후, 쓰레드를 실행
	CEventSelect *workerThread = reinterpret_cast<CEventSelect*>(parameter);
	workerThread->EventSelectCallBack();

	return 0;
}

CEventSelect::CEventSelect()
{
}


CEventSelect::~CEventSelect()
{
}

//====================================================
// Initialize
// Info : 초기화
// socket - 이벤트 알림을 받을 소켓.
BOOL CEventSelect::Initialize(SOCKET socket)
{
	// 소켓이 NULL 이면 리턴.
	if (!socket)
	{
		CLog::WriteLog("CEventSelect : Initialize - socket not Allocation.\n");

		return FALSE;
	}

	// 소켓 주소 전달.
	this->socket = socket;

	// 이벤트 생성
	selectEvent = WSACreateEvent();

	if (selectEvent == WSA_INVALID_EVENT)
	{
		CLog::WriteLog("CEventSelect : Initialize - WSACreateEvent() Error!\n");

		return FALSE;
	}

	// socket에 EventSelect 알림을 받도록 설정한다. 입출력이 오면 socketEvent가 발생한다.
	// 연결, 수신, 송신, 연결 해제 알림을 받는다. 
	if (WSAEventSelect(this->socket, selectEvent, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
	{
		// WSAEWOULDBLOCK - 즉시 처리되지 않았음. 나중에 처리될 예정.
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			CLog::WriteLog("CEventSelect : Initialize - WSAEventSelect() Error!\n");

			return FALSE;
		}
	}

	// 종료시 발생할 이벤트 생성.
	destoryEvent = CreateEvent(0, FALSE, FALSE, 0);

	if (destoryEvent == NULL)
	{
		CLog::WriteLog("CEventSelect : Initialize - CreateEvent() Error!\n");

		return FALSE;
	}

	// EventSelect 쓰레드 생성.
	eventSelectThreadHandle = (HANDLE)_beginthreadex(NULL, 0, EventSelectThreadCallBack, this, 0, NULL);

	return TRUE;
}

//====================================================
// Release
// Info : 메모리 해제
BOOL CEventSelect::Release(VOID)
{
	// 쓰레드에 종료 이벤트를 발생시킨다.
	SetEvent(destoryEvent);

	if (selectEvent)
		WSACloseEvent(selectEvent);

	if (eventSelectThreadHandle)
		CloseHandle(eventSelectThreadHandle);

	if (destoryEvent)
		CloseHandle(destoryEvent);

	return TRUE;
}

//====================================================
// EventSelectThreadCallBack
// Info : EventSelect의 입출력 알림을 받는 쓰레드.
VOID CEventSelect::EventSelectCallBack(VOID)
{
	while (TRUE)
	{
		WSANETWORKEVENTS networkEvents;
		HANDLE events[2] = { selectEvent , destoryEvent };
		DWORD	 eventResult = 0;

		// EventSelect와 Destroy 이벤트를 대기한다. 하나라도 발생하면 반환한다.
		eventResult = WaitForMultipleObjects(2, events, FALSE, INFINITE);

		switch (eventResult)
		{
			// EventSelect 이벤트가 수신되었음.
		case WAIT_OBJECT_0:
			// 수신된 이벤트를 검사.
			if (WSAEnumNetworkEvents(socket, selectEvent, &networkEvents) == SOCKET_ERROR)
				continue;

			// 연결 요청.
			if (networkEvents.lNetworkEvents & FD_CONNECT)
			{
				// ErrorCode가 0이 아니면 연결이 성공되지 않은 것이다.
				if (networkEvents.iErrorCode[FD_CONNECT_BIT])
				{
					continue;
				}

				OnIOConnect();
			}
			// 송신 요청
			else if (networkEvents.lNetworkEvents & FD_WRITE)
			{
				OnIOWrite();
			}
			// 수신 요청
			else if (networkEvents.lNetworkEvents & FD_READ)
			{
				OnIORead();
			}
			// 종료 요청
			else if (networkEvents.lNetworkEvents & FD_CLOSE)
			{
				OnIODisconnect();
			}

			break;

			// 종료 이벤트가 수신되었음.
		case WAIT_OBJECT_0 + 1:

			return;
		}
	}
}

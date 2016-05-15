#include "Headers.h"
#include "EventSelect.h"

//====================================================
// EventSelectThreadCallBack
// Info : �����带 �����Ű�� ���� CallBack �Լ�.
// parameter - CEventSelect ��ü�� ������.
unsigned WINAPI EventSelectThreadCallBack(LPVOID parameter)
{
	// ����ȯ ��, �����带 ����
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
// Info : �ʱ�ȭ
// socket - �̺�Ʈ �˸��� ���� ����.
BOOL CEventSelect::Initialize(SOCKET socket)
{
	// ������ NULL �̸� ����.
	if (!socket)
	{
		CLog::WriteLog("CEventSelect : Initialize - socket not Allocation.\n");

		return FALSE;
	}

	// ���� �ּ� ����.
	this->socket = socket;

	// �̺�Ʈ ����
	selectEvent = WSACreateEvent();

	if (selectEvent == WSA_INVALID_EVENT)
	{
		CLog::WriteLog("CEventSelect : Initialize - WSACreateEvent() Error!\n");

		return FALSE;
	}

	// socket�� EventSelect �˸��� �޵��� �����Ѵ�. ������� ���� socketEvent�� �߻��Ѵ�.
	// ����, ����, �۽�, ���� ���� �˸��� �޴´�. 
	if (WSAEventSelect(this->socket, selectEvent, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
	{
		// WSAEWOULDBLOCK - ��� ó������ �ʾ���. ���߿� ó���� ����.
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			CLog::WriteLog("CEventSelect : Initialize - WSAEventSelect() Error!\n");

			return FALSE;
		}
	}

	// ����� �߻��� �̺�Ʈ ����.
	destoryEvent = CreateEvent(0, FALSE, FALSE, 0);

	if (destoryEvent == NULL)
	{
		CLog::WriteLog("CEventSelect : Initialize - CreateEvent() Error!\n");

		return FALSE;
	}

	// EventSelect ������ ����.
	eventSelectThreadHandle = (HANDLE)_beginthreadex(NULL, 0, EventSelectThreadCallBack, this, 0, NULL);

	return TRUE;
}

//====================================================
// Release
// Info : �޸� ����
BOOL CEventSelect::Release(VOID)
{
	// �����忡 ���� �̺�Ʈ�� �߻���Ų��.
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
// Info : EventSelect�� ����� �˸��� �޴� ������.
VOID CEventSelect::EventSelectCallBack(VOID)
{
	while (TRUE)
	{
		WSANETWORKEVENTS networkEvents;
		HANDLE events[2] = { selectEvent , destoryEvent };
		DWORD	 eventResult = 0;

		// EventSelect�� Destroy �̺�Ʈ�� ����Ѵ�. �ϳ��� �߻��ϸ� ��ȯ�Ѵ�.
		eventResult = WaitForMultipleObjects(2, events, FALSE, INFINITE);

		switch (eventResult)
		{
			// EventSelect �̺�Ʈ�� ���ŵǾ���.
		case WAIT_OBJECT_0:
			// ���ŵ� �̺�Ʈ�� �˻�.
			if (WSAEnumNetworkEvents(socket, selectEvent, &networkEvents) == SOCKET_ERROR)
				continue;

			// ���� ��û.
			if (networkEvents.lNetworkEvents & FD_CONNECT)
			{
				// ErrorCode�� 0�� �ƴϸ� ������ �������� ���� ���̴�.
				if (networkEvents.iErrorCode[FD_CONNECT_BIT])
				{
					continue;
				}

				OnIOConnect();
			}
			// �۽� ��û
			else if (networkEvents.lNetworkEvents & FD_WRITE)
			{
				OnIOWrite();
			}
			// ���� ��û
			else if (networkEvents.lNetworkEvents & FD_READ)
			{
				OnIORead();
			}
			// ���� ��û
			else if (networkEvents.lNetworkEvents & FD_CLOSE)
			{
				OnIODisconnect();
			}

			break;

			// ���� �̺�Ʈ�� ���ŵǾ���.
		case WAIT_OBJECT_0 + 1:

			return;
		}
	}
}

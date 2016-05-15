#include "headers.h"
#include "Iocp.h"

//==================================================
// WorkerThreadCallBack
// Info : IOCP의 쓰레드를 처리해주기 위한 콜백 메소드.
// parameter - CIocp 객체를 전달함.
unsigned WINAPI WorkerThreadCallBack(LPVOID parameter)
{
	// CIocp객체로 형변환 후 쓰레드 실행.
	CIocp* workerThread = reinterpret_cast<CIocp*>(parameter);

	workerThread->GameThreadCallBack();

	return 0;
}


CIocp::CIocp()
{
}


CIocp::~CIocp()
{
}

//==================================================
// Initialize
// Info : 초기화
BOOL CIocp::Initialize(VOID)
{
	SYSTEM_INFO SystemInfo;

	// 시스템의 정보를 가져온다.
	GetSystemInfo(&SystemInfo);

	// IOCP의 핸들을 생성한다. 마지막 인자에 0을 넣어 최대 쓰레드만큼 사용할 수 있다.
	hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (hIOCP == NULL)
	{
		CLog::WriteLog("CIocp : Initialize - CreateIoCompletionPort() Error!\n");

		return FALSE;
	}

	// 시스템의 최대 프로세스만큼 스레드를 생성한다.
	for (int i = 0; i < SystemInfo.dwNumberOfProcessors + 2; i++)
	{
		HANDLE WorkerThread;

		WorkerThread = (HANDLE)_beginthreadex(NULL, 0, WorkerThreadCallBack, this, 0, NULL);
		threadHandle.push_back(WorkerThread);
	}

	return TRUE;
}

//==================================================
// WorkerThreadCallBack
// Info : 메모리 해제
BOOL CIocp::Release(VOID)
{
	// 핸들수 만큼 루프를 돌린다.
	for each (HANDLE handle in threadHandle)
	{
		// 종료 신호를 IOCP에 발생시킨다.
		BOOL	Result = PostQueuedCompletionStatus(hIOCP, NULL, THREAD_QUIT, NULL);

		if (!Result)
		{
			return FALSE;
		}

		CloseHandle(handle);
	}

	if (hIOCP)
		CloseHandle(hIOCP);

	return TRUE;
}

//==================================================
// RegisterSocketToIocp
// Info : 소켓이 IOCP에서 알림을 받을 수 있도록 설정한다.
// socket - IOCP에 등록할 소켓
// completionKey - 소켓에 입출력이 발생하면 IOCP 에서 소켓과 함께 받을 객체.
BOOL CIocp::RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey)
{
	// IOCP 객체와 소켓을 연결한다. 
	// 소켓에서 특정 입출력이 일어나면 IOCP 객체에서 반응을 얻을 수 있다.
	hIOCP = CreateIoCompletionPort((HANDLE)socket, hIOCP, completionKey, 0);

	if (hIOCP == NULL)
	{
		INT errorCode = WSAGetLastError();

		CLog::WriteLog("CIocp : RegisterSocketToIocp - CreateIoCompletionPort() Error\n");

		return FALSE;
	}

	return TRUE;
}

//==================================================
// GameThreadCallBack
// Info : IOCP의 알림을 받고, 처리하는 쓰레드.
VOID CIocp::GameThreadCallBack(VOID)
{
	// 받은 데이터 크기, Connect, Disconnect시에는 0이 되고, 
	// Recv를 했을 경우에 크기가 할당됨.
	DWORD							transferredByte = 0;
	// IOCP에 소켓을 등록할 때 전달받기로 한 객체
	ULONG_PTR					CompletionKey = NULL;
	// 입출력의 종류를 포함하는 객체. 
	// 맨 앞의 변수가 Overlapped 인자임을 이용하여 구조체 전부를 전달받을 수 있다.
	PPER_IOCONTEXT		IoContext = NULL;
	// IOCP에 필요한 Overlapped 객체.
	OVERLAPPED*				Overlapped = NULL;
	// IOCP의 결과. 성공시 1을 리턴함.
	BOOL								result = FALSE;

	while (TRUE)
	{
		// IOCP 핸들에 신호가 올 때까지 무한정 대기한다.
		result = GetQueuedCompletionStatus(hIOCP, &transferredByte, &CompletionKey,
			(LPOVERLAPPED*)&Overlapped, INFINITE);

		// 종료 신호가 IOCP에 전달되었음.
		if (CompletionKey == THREAD_QUIT)
		{
			return;
		}

		if (result)
		{
			// 주소값을 이용해 구조체의 값을 전부 얻어오는 방법.
			IoContext = (PPER_IOCONTEXT)Overlapped;
		
			if (transferredByte == 0)
			{
				if (IoContext->ioType == IO_TYPE::IO_ACCPET)
				{
					IOConnected(IoContext->object);
				}
				// 클라이언트가 정상적인 종료를 했음.
				else
				{
					IODisconnected(IoContext->object);
				}
			}

			// IOCP에서 신호가 오고, 그 신호가 IO_READ라면,
			if (IoContext->ioType == IO_TYPE::IO_READ)
			{
				IOReceived(IoContext->object, transferredByte);
			}
		}
		// 클라이언트가 비정상적인 종료를 했음.
		else
		{
			IODisconnected(IoContext->object);
		}
	}
}
#include "headers.h"
#include "Iocp.h"

//==================================================
// WorkerThreadCallBack
// Info : IOCP�� �����带 ó�����ֱ� ���� �ݹ� �޼ҵ�.
// parameter - CIocp ��ü�� ������.
unsigned WINAPI WorkerThreadCallBack(LPVOID parameter)
{
	// CIocp��ü�� ����ȯ �� ������ ����.
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
// Info : �ʱ�ȭ
BOOL CIocp::Initialize(VOID)
{
	SYSTEM_INFO SystemInfo;

	// �ý����� ������ �����´�.
	GetSystemInfo(&SystemInfo);

	// IOCP�� �ڵ��� �����Ѵ�. ������ ���ڿ� 0�� �־� �ִ� �����常ŭ ����� �� �ִ�.
	hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (hIOCP == NULL)
	{
		CLog::WriteLog("CIocp : Initialize - CreateIoCompletionPort() Error!\n");

		return FALSE;
	}

	// �ý����� �ִ� ���μ�����ŭ �����带 �����Ѵ�.
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
// Info : �޸� ����
BOOL CIocp::Release(VOID)
{
	// �ڵ�� ��ŭ ������ ������.
	for each (HANDLE handle in threadHandle)
	{
		// ���� ��ȣ�� IOCP�� �߻���Ų��.
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
// Info : ������ IOCP���� �˸��� ���� �� �ֵ��� �����Ѵ�.
// socket - IOCP�� ����� ����
// completionKey - ���Ͽ� ������� �߻��ϸ� IOCP ���� ���ϰ� �Բ� ���� ��ü.
BOOL CIocp::RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey)
{
	// IOCP ��ü�� ������ �����Ѵ�. 
	// ���Ͽ��� Ư�� ������� �Ͼ�� IOCP ��ü���� ������ ���� �� �ִ�.
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
// Info : IOCP�� �˸��� �ް�, ó���ϴ� ������.
VOID CIocp::GameThreadCallBack(VOID)
{
	// ���� ������ ũ��, Connect, Disconnect�ÿ��� 0�� �ǰ�, 
	// Recv�� ���� ��쿡 ũ�Ⱑ �Ҵ��.
	DWORD							transferredByte = 0;
	// IOCP�� ������ ����� �� ���޹ޱ�� �� ��ü
	ULONG_PTR					CompletionKey = NULL;
	// ������� ������ �����ϴ� ��ü. 
	// �� ���� ������ Overlapped �������� �̿��Ͽ� ����ü ���θ� ���޹��� �� �ִ�.
	PPER_IOCONTEXT		IoContext = NULL;
	// IOCP�� �ʿ��� Overlapped ��ü.
	OVERLAPPED*				Overlapped = NULL;
	// IOCP�� ���. ������ 1�� ������.
	BOOL								result = FALSE;

	while (TRUE)
	{
		// IOCP �ڵ鿡 ��ȣ�� �� ������ ������ ����Ѵ�.
		result = GetQueuedCompletionStatus(hIOCP, &transferredByte, &CompletionKey,
			(LPOVERLAPPED*)&Overlapped, INFINITE);

		// ���� ��ȣ�� IOCP�� ���޵Ǿ���.
		if (CompletionKey == THREAD_QUIT)
		{
			return;
		}

		if (result)
		{
			// �ּҰ��� �̿��� ����ü�� ���� ���� ������ ���.
			IoContext = (PPER_IOCONTEXT)Overlapped;
		
			if (transferredByte == 0)
			{
				if (IoContext->ioType == IO_TYPE::IO_ACCPET)
				{
					IOConnected(IoContext->object);
				}
				// Ŭ���̾�Ʈ�� �������� ���Ḧ ����.
				else
				{
					IODisconnected(IoContext->object);
				}
			}

			// IOCP���� ��ȣ�� ����, �� ��ȣ�� IO_READ���,
			if (IoContext->ioType == IO_TYPE::IO_READ)
			{
				IOReceived(IoContext->object, transferredByte);
			}
		}
		// Ŭ���̾�Ʈ�� ���������� ���Ḧ ����.
		else
		{
			IODisconnected(IoContext->object);
		}
	}
}
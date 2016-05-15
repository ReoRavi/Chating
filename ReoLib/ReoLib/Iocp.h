#pragma once

//===============================================
// CIocp
// Info : IOCP�� ������. �� Ŭ������ ��ӹ����� IOCP�� ����ϴ� ����.
class CIocp
{
public:
	CIocp();
	virtual ~CIocp();

private :	
	// �������� �ڵ�
	std::vector<HANDLE>		threadHandle;
	// IOCP�� �ڵ�
	HANDLE								hIOCP;

protected :
	// �ʱ�ȭ
	BOOL		Initialize(VOID);
	// �޸� ����
	BOOL		Release(VOID);

	// ������ IOCP�� ����Ѵ�. ���Ͽ� ������� �߻��ϸ� IOCP�κ��� ��ȣ�� ���� �� �ִ�.
	BOOL		RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey);
	
public :
	// ����� IOCP�� ������
	VOID		GameThreadCallBack(VOID);

protected :
	// IOCP�� ����¿� �°� ����� ���������Լ���.
	virtual	BOOL	IOConnected(VOID* object) = 0;
	virtual	BOOL	IODisconnected(VOID* object) = 0;
	virtual	VOID	IOReceived(VOID* object, DWORD receivedByte) = 0;
};


#pragma once

//===============================================
// CEventSelect
// Info : WSAEventSelect�� ����ϴ� Ŭ����. �̺�Ʈ ������� ������� ������.
class CEventSelect
{  
public:
	CEventSelect();
	virtual ~CEventSelect();

protected:
	// EventSelect���� ������� �Ͼ�� �߻��� �̺�Ʈ.
	WSAEVENT	selectEvent;
	// ���� �̺�Ʈ.
	HANDLE			destoryEvent;
	// EventSelect �������� �ڵ�.
	HANDLE			eventSelectThreadHandle;
	// ����ϴ� ������ �ּ�, ���������� ClientSection�� ����
	SOCKET		socket;

protected:
	// �ʱ�ȭ
	BOOL		Initialize(SOCKET socket);
	// �޸� ����
	BOOL		Release(VOID);
	
protected:
	// ������� �߻����� �� ����� ���� �����Լ���.
	virtual VOID	OnIORead(VOID) = 0;
	virtual VOID	OnIOWrite(VOID) = 0;
	virtual VOID	OnIOConnect(VOID) = 0;
	virtual VOID	OnIODisconnect(VOID) = 0;

public :
	// EventSelect�� ����� ������.
	VOID		EventSelectCallBack(VOID);
};


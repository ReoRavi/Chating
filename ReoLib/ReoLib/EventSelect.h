#pragma once

//===============================================
// CEventSelect
// Info : WSAEventSelect를 사용하는 클래스. 이벤트 방식으로 입출력을 진행함.
class CEventSelect
{  
public:
	CEventSelect();
	virtual ~CEventSelect();

protected:
	// EventSelect에서 입출력이 일어나면 발생할 이벤트.
	WSAEVENT	selectEvent;
	// 종료 이벤트.
	HANDLE			destoryEvent;
	// EventSelect 쓰레드의 핸들.
	HANDLE			eventSelectThreadHandle;
	// 사용하는 소켓의 주소, 실제소켓은 ClientSection에 있음
	SOCKET		socket;

protected:
	// 초기화
	BOOL		Initialize(SOCKET socket);
	// 메모리 해제
	BOOL		Release(VOID);
	
protected:
	// 입출력이 발생했을 때 실행될 순수 가상함수들.
	virtual VOID	OnIORead(VOID) = 0;
	virtual VOID	OnIOWrite(VOID) = 0;
	virtual VOID	OnIOConnect(VOID) = 0;
	virtual VOID	OnIODisconnect(VOID) = 0;

public :
	// EventSelect가 실행될 쓰레드.
	VOID		EventSelectCallBack(VOID);
};


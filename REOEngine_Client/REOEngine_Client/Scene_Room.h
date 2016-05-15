#pragma once

//=================================================
// CScene_Room
// Info : 방 씬
class CScene_Room :	public CScene,
										public CRoomUI
{
public:
	CScene_Room();
	virtual ~CScene_Room();

public :
	// CScene 순수가상함수
	// 초기화
	BOOL	Initialize(VOID);
	// 루프
	VOID	Process(VOID);
	// 메모리 해제
	BOOL	Release(VOID);

public :
	// 서버로부터 메세지를 받고, 처리한다.
	VOID	ReceiveChating(std::string message);
};


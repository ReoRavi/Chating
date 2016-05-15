#pragma once

//=================================================
// CScene_CreateRoom
// info : 방 생성 씬
class CScene_CreateRoom :	public CScene, 
													public CCreateRoomUI
{
public:
	CScene_CreateRoom();
	virtual ~CScene_CreateRoom();

public :
	// 방 생성을 요청하고, 기다리는 중인가.
	BOOL	bWaiting;

public:
	// CScene 순수가상함수
	// 초기화
	BOOL	Initialize(VOID);
	// 루프
	VOID	Process(VOID);
	// 메모리 해제
	BOOL	Release(VOID);

public:
	// 서버로부터 방 생성의 결과를 받고 처리한다.
	VOID	RequestResult(BOOL bSuccess);
};


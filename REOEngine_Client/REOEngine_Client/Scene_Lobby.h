#pragma once

//=================================================
// CScene_Lobby
// Info : 로비 씬
class CScene_Lobby :	public CScene,
										public CLobbyUI
{
public:
	CScene_Lobby();
	virtual ~CScene_Lobby();

public :
	// 커서의 인덱스
	INT		cursorIndex;

public:
	// CScene 순수가상함수
	// 초기화
	BOOL	Initialize(VOID);
	// 루프
	VOID	Process(VOID);
	// 메모리 해제
	BOOL	Release(VOID);
};


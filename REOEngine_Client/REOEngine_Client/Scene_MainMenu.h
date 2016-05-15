#pragma once

//=================================================
// CScene_MainMenu
// Info : 메인메뉴 씬
class CScene_MainMenu :	public CScene,
												public CMainMenuUI
{
public:
	CScene_MainMenu();
	virtual ~CScene_MainMenu();

public :
	// 씬의 타입
	eSceneType sceneType;

public :
	// CScene 순수가상함수
	// 초기화
	BOOL	Initialize(VOID);
	// 루프
	VOID	Process(VOID);
	// 메모리 해제
	BOOL	Release(VOID);
};


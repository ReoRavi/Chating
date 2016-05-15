#pragma once

//=================================================
// CMainMenuUI
// Info : 메인메뉴 씬의 UI를 처리한다.
class CMainMenuUI : public CUI
{
public:
	CMainMenuUI();
	virtual ~CMainMenuUI();

public :
	// 커서의 상태
	INT		cursorState;

public :
	// CUI의 순수가상함수, 씬마다 다른 UI 그리기를 처리함.
	VOID	DrawUI(int x, int y);
	// 씬의 모든 부분을 그린다.
	VOID	DrawAllSection();
	// 메인메뉴 씬의 UI를 그린다.
	VOID	DrawMainSectionUI();
	// 씬의 커서를 그린다.
	VOID	DrawCursor();
	// 커서를 지운다.
	VOID	EraseCursor();
};


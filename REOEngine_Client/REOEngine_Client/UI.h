#pragma once

//=================================================
// CUI
// info : UI 의 기본 함수들과 정의해야할 가상함수들을 정의함.
class CUI
{
public:
	CUI();
	virtual ~CUI();

protected :
	// 커서 이동
	VOID	gotoxy(int x, int y);
	// 큰 박스 하나를 그려준다. (공통)
	VOID	DrawUIBox(VOID);

	// 씬마다 다른 UI처리를 구현하는 순수가상함수.
	virtual	VOID	DrawUI(int x, int y) = 0;
	// 씬의 모든 부분을 그려줄 함수
	virtual	VOID	DrawAllSection(VOID) =  0;
};


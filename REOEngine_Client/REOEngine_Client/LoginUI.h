#pragma once

//=================================================
// CLoginUI
// Info : 로그인 씬의 UI를 처리한다.
class CLoginUI : public CUI
{
public:
	CLoginUI();
	virtual ~CLoginUI();

public:
	// UI의 이름들
	std::string UIName[2];

public:
	// CUI의 순수가상함수, 씬마다 다른 UI 그리기를 처리함.
	VOID	DrawUI(int x, int y);
	// 씬의 모든 부분을 그린다.
	VOID	DrawAllSection(VOID);
	// 씬의 UI를 그린다.
	VOID	DrawUISection(VOID);
	// 메세지를 그린다.
	VOID	DrawMessage(std::string message);
};


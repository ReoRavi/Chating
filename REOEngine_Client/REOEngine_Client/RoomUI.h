#pragma once

//=================================================
// CRoomUI
// Info : 방 씬의 UI를 처리한다.
class CRoomUI : public CUI
{
public:
	CRoomUI();
	virtual ~CRoomUI();

public:
	// 체팅의 로그
	std::string		chatingLog[MAX_CHATING];
	// 체팅 로그의 카운트
	INT					chatingCount;

public:
	// CUI의 순수가상함수, 씬마다 다른 UI 그리기를 처리함.
	VOID	DrawUI(int x, int y);
	// 씬의 모든 부분을 그린다.
	VOID	DrawAllSection(VOID);
	// 체팅 출력 부분을 그린다.
	VOID	DrawChatSection(VOID);
	// 체팅 입력 부분을 그린다.
	VOID	DrawInputSection(VOID);
	// 알림을 로그에 등록한다.
	VOID	InputNotice(std::string notice);
	// 체팅을 로그에 등록한다.
	VOID	InputChating(std::string message);
	// 체팅을 그린다.
	VOID	DrawChating(VOID);
};


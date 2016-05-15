#pragma once

//=================================================
// CLobbyUI
// Info : 로비 씬의 UI를 처리한다.
class CLobbyUI : public CUI
{
public:
	CLobbyUI();
	virtual ~CLobbyUI();

public :
	// CUI의 순수가상함수, 씬마다 다른 UI 그리기를 처리함.
	VOID	DrawUI(int x, int y);
	// 씬의 모든 부분을 그린다.
	VOID	DrawAllSection(VOID);
	// 로비를 그린다.
	VOID	DrawLobbySection(VOID);
	// 로비의 UI 메뉴를 그린다.
	VOID	DrawLobbyUI(VOID);
	// 로비의 UI들을 그린다.
	VOID	DrawRoomInfo(INT roomCount, INT roomIndex[], std::string roomHost[], std::string roomName[]);
	// 커서를 그린다.
	VOID	DrawCursor(INT cursorIndex);
	// 커서를 지운다.
	VOID	EraseCursor(INT cursorIndex, INT moveDirection);
};


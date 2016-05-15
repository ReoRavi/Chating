#pragma once

//=================================================
// CCreateRoomUI
// Info : 방 생성 씬의 UI를 처리한다.
class CCreateRoomUI : public CUI
{
public:
	CCreateRoomUI();
	virtual ~CCreateRoomUI();

public :
	// CUI의 순수가상함수, 씬마다 다른 UI 그리기를 처리함.
	VOID	DrawUI(int x, int y);
	// 씬의 모든 부분을 그린다.
	VOID	DrawAllSection(VOID);
	// 씬의 UI를 그린다.
	VOID	DrawSectionUI(VOID);
	// 입력 박스를 그린다.
	VOID	DrawInputBox(VOID);
	// 알림을 그린다.
	VOID	DrawNotice(std::string notice);
};


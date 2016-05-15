#pragma once

//=================================================
// CLobbyUI
// Info : �κ� ���� UI�� ó���Ѵ�.
class CLobbyUI : public CUI
{
public:
	CLobbyUI();
	virtual ~CLobbyUI();

public :
	// CUI�� ���������Լ�, ������ �ٸ� UI �׸��⸦ ó����.
	VOID	DrawUI(int x, int y);
	// ���� ��� �κ��� �׸���.
	VOID	DrawAllSection(VOID);
	// �κ� �׸���.
	VOID	DrawLobbySection(VOID);
	// �κ��� UI �޴��� �׸���.
	VOID	DrawLobbyUI(VOID);
	// �κ��� UI���� �׸���.
	VOID	DrawRoomInfo(INT roomCount, INT roomIndex[], std::string roomHost[], std::string roomName[]);
	// Ŀ���� �׸���.
	VOID	DrawCursor(INT cursorIndex);
	// Ŀ���� �����.
	VOID	EraseCursor(INT cursorIndex, INT moveDirection);
};


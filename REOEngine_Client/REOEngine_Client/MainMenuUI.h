#pragma once

//=================================================
// CMainMenuUI
// Info : ���θ޴� ���� UI�� ó���Ѵ�.
class CMainMenuUI : public CUI
{
public:
	CMainMenuUI();
	virtual ~CMainMenuUI();

public :
	// Ŀ���� ����
	INT		cursorState;

public :
	// CUI�� ���������Լ�, ������ �ٸ� UI �׸��⸦ ó����.
	VOID	DrawUI(int x, int y);
	// ���� ��� �κ��� �׸���.
	VOID	DrawAllSection();
	// ���θ޴� ���� UI�� �׸���.
	VOID	DrawMainSectionUI();
	// ���� Ŀ���� �׸���.
	VOID	DrawCursor();
	// Ŀ���� �����.
	VOID	EraseCursor();
};


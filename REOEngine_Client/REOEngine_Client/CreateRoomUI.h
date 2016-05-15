#pragma once

//=================================================
// CCreateRoomUI
// Info : �� ���� ���� UI�� ó���Ѵ�.
class CCreateRoomUI : public CUI
{
public:
	CCreateRoomUI();
	virtual ~CCreateRoomUI();

public :
	// CUI�� ���������Լ�, ������ �ٸ� UI �׸��⸦ ó����.
	VOID	DrawUI(int x, int y);
	// ���� ��� �κ��� �׸���.
	VOID	DrawAllSection(VOID);
	// ���� UI�� �׸���.
	VOID	DrawSectionUI(VOID);
	// �Է� �ڽ��� �׸���.
	VOID	DrawInputBox(VOID);
	// �˸��� �׸���.
	VOID	DrawNotice(std::string notice);
};


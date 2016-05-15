#pragma once

//=================================================
// CRoomUI
// Info : �� ���� UI�� ó���Ѵ�.
class CRoomUI : public CUI
{
public:
	CRoomUI();
	virtual ~CRoomUI();

public:
	// ü���� �α�
	std::string		chatingLog[MAX_CHATING];
	// ü�� �α��� ī��Ʈ
	INT					chatingCount;

public:
	// CUI�� ���������Լ�, ������ �ٸ� UI �׸��⸦ ó����.
	VOID	DrawUI(int x, int y);
	// ���� ��� �κ��� �׸���.
	VOID	DrawAllSection(VOID);
	// ü�� ��� �κ��� �׸���.
	VOID	DrawChatSection(VOID);
	// ü�� �Է� �κ��� �׸���.
	VOID	DrawInputSection(VOID);
	// �˸��� �α׿� ����Ѵ�.
	VOID	InputNotice(std::string notice);
	// ü���� �α׿� ����Ѵ�.
	VOID	InputChating(std::string message);
	// ü���� �׸���.
	VOID	DrawChating(VOID);
};


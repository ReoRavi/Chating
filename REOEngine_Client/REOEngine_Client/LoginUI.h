#pragma once

//=================================================
// CLoginUI
// Info : �α��� ���� UI�� ó���Ѵ�.
class CLoginUI : public CUI
{
public:
	CLoginUI();
	virtual ~CLoginUI();

public:
	// UI�� �̸���
	std::string UIName[2];

public:
	// CUI�� ���������Լ�, ������ �ٸ� UI �׸��⸦ ó����.
	VOID	DrawUI(int x, int y);
	// ���� ��� �κ��� �׸���.
	VOID	DrawAllSection(VOID);
	// ���� UI�� �׸���.
	VOID	DrawUISection(VOID);
	// �޼����� �׸���.
	VOID	DrawMessage(std::string message);
};


#pragma once

//=================================================
// CUI
// info : UI �� �⺻ �Լ���� �����ؾ��� �����Լ����� ������.
class CUI
{
public:
	CUI();
	virtual ~CUI();

protected :
	// Ŀ�� �̵�
	VOID	gotoxy(int x, int y);
	// ū �ڽ� �ϳ��� �׷��ش�. (����)
	VOID	DrawUIBox(VOID);

	// ������ �ٸ� UIó���� �����ϴ� ���������Լ�.
	virtual	VOID	DrawUI(int x, int y) = 0;
	// ���� ��� �κ��� �׷��� �Լ�
	virtual	VOID	DrawAllSection(VOID) =  0;
};


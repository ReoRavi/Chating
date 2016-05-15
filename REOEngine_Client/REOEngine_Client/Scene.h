#pragma once

//=================================================
// CScene
// info : ���� �����ؾ��� �Լ����� ������ (�������̽�ȭ)
class CScene
{
public:
	CScene();
	virtual ~CScene();

public :
	// �ʱ�ȭ
	virtual BOOL	Initialize(VOID) = 0;
	// ����
	virtual VOID	Process(VOID) = 0;
	// �޸� ����
	virtual BOOL	Release(VOID) = 0;
};


#pragma once

//=================================================
// CScene_MainMenu
// Info : ���θ޴� ��
class CScene_MainMenu :	public CScene,
												public CMainMenuUI
{
public:
	CScene_MainMenu();
	virtual ~CScene_MainMenu();

public :
	// ���� Ÿ��
	eSceneType sceneType;

public :
	// CScene ���������Լ�
	// �ʱ�ȭ
	BOOL	Initialize(VOID);
	// ����
	VOID	Process(VOID);
	// �޸� ����
	BOOL	Release(VOID);
};


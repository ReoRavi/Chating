#pragma once

//=================================================
// CScene_Lobby
// Info : �κ� ��
class CScene_Lobby :	public CScene,
										public CLobbyUI
{
public:
	CScene_Lobby();
	virtual ~CScene_Lobby();

public :
	// Ŀ���� �ε���
	INT		cursorIndex;

public:
	// CScene ���������Լ�
	// �ʱ�ȭ
	BOOL	Initialize(VOID);
	// ����
	VOID	Process(VOID);
	// �޸� ����
	BOOL	Release(VOID);
};


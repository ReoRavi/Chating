#pragma once

//=================================================
// CScene_Room
// Info : �� ��
class CScene_Room :	public CScene,
										public CRoomUI
{
public:
	CScene_Room();
	virtual ~CScene_Room();

public :
	// CScene ���������Լ�
	// �ʱ�ȭ
	BOOL	Initialize(VOID);
	// ����
	VOID	Process(VOID);
	// �޸� ����
	BOOL	Release(VOID);

public :
	// �����κ��� �޼����� �ް�, ó���Ѵ�.
	VOID	ReceiveChating(std::string message);
};


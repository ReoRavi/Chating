#pragma once

//=================================================
// CScene_CreateRoom
// info : �� ���� ��
class CScene_CreateRoom :	public CScene, 
													public CCreateRoomUI
{
public:
	CScene_CreateRoom();
	virtual ~CScene_CreateRoom();

public :
	// �� ������ ��û�ϰ�, ��ٸ��� ���ΰ�.
	BOOL	bWaiting;

public:
	// CScene ���������Լ�
	// �ʱ�ȭ
	BOOL	Initialize(VOID);
	// ����
	VOID	Process(VOID);
	// �޸� ����
	BOOL	Release(VOID);

public:
	// �����κ��� �� ������ ����� �ް� ó���Ѵ�.
	VOID	RequestResult(BOOL bSuccess);
};


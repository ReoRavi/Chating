#pragma once

//=================================================
// CUserManager
// Info : �������� ��ü�� �����ϴ� �Ŵ���
class CUserManager
{
public:
	CUserManager();
	virtual ~CUserManager();

private :
	// ���� ��ü���� �����ϴ� Vector
	std::vector<CUser*> vUsers;

public :
	// �������ִ� ���� ����
	INT userCount;

public :
	// �ʱ�ȭ
	BOOL Initialize(SOCKET listenSocket);
	// �޸� ����
	BOOL	Release(VOID);

	// ��� �������� ��Ʈ��Ʈ�� üũ�Ѵ�.
	VOID	CheckAllHeartBeat(SOCKET listenSocket);
	// ��� �����鿡�� ��Ŷ�� ������.
	BOOL	SendToAll(DWORD length, ePACKET_PROTOCOL protocol, BYTE* buffer);
};


#include "stdafx.h"
#include "UserManager.h"


CUserManager::CUserManager()
{
}


CUserManager::~CUserManager()
{
}

//=================================================
// Initialize
// Info : �ʱ�ȭ
// listenSocket - ���� ��û�� ���� ���� ����.
BOOL CUserManager::Initialize(SOCKET listenSocket)
{
	// �Ӱ迵�� ����
	CAutoCriticalSection section;

	// �ִ� ��������ŭ ������ ������, ���� �����·� �����.
	for (int index = 0; index < MAX_USER; index++)
	{
		CUser* user = new CUser();

		if (!user->Initialize())
		{
			CLog::WriteLogWithDate("CUserManager : Initialize - user->Initialize() Error!\n");

			return FALSE;
		}

		if (!user->TCPSocket())
		{
			CLog::WriteLogWithDate("CUserManager : Initialize - user->TCPSocket() Error!\n");

			return FALSE;
		}

		vUsers.push_back(user);
	}

	// ���������� ������ ������ ������ ���ں��� ������ �Ҵ��.
	// ������ü�� ���� ��û ��� ���·� �����.
	for (int index = MAX_USER - 1; index >= 0; index--)
	{
		if (!vUsers[index]->Accept(listenSocket))
		{
			CLog::WriteLogWithDate("CUserManager : Initialize - user->Accept() Error!\n");

			return FALSE;
		}
	}

	userCount = 0;

	return TRUE;
}

//=================================================
// Release
// Info : �޸� ����
BOOL CUserManager::Release(VOID)
{
	// �Ӱ迵�� ����
	CAutoCriticalSection section;

	for (std::vector<CUser*>::iterator iter = vUsers.begin(); iter != vUsers.end(); ++iter)
	{
		if ((*iter)->Release())
		{
			CLog::WriteLogWithDate("CUserManager : Release - (*iter)->Release() Error!\n");

			return FALSE;
		}

		delete (*iter);
	}

	vUsers.clear();

	return TRUE;
}

//=================================================
// CheckAllHeartBeat
// Info : ��� �������� ��Ʈ��Ʈ�� üũ�Ѵ�.
// listenSocket - ��Ʈ��Ʈ ó���� �ʿ��� ���� ����
VOID CUserManager::CheckAllHeartBeat(SOCKET listenSocket)
{
	// �Ӱ迵�� ����
	CAutoCriticalSection section;

	// �������ִ� �������� ��Ʈ��Ʈ�� üũ�Ѵ�.
	for each (CUser* user in vUsers)
	{
		if (user->GetConnected())
			user->HeartBeatCheck(listenSocket);
	}
}

//=================================================
// SendToAll
// Info : ��� �����鿡�� ��Ŷ�� ������
// length - ��Ŷ�� ����
// protocol - ��Ŷ�� ��������
// buffer - ��Ŷ�� ����
BOOL CUserManager::SendToAll(DWORD length, ePACKET_PROTOCOL protocol, BYTE* buffer)
{
	// �Ӱ迵�� ����
	CAutoCriticalSection section;

	// �������ִ� ��� �����鿡�� ��Ŷ�� ������.
	for each (CUser* user in vUsers)
	{
		if (user->GetConnected())
			if (!user->WritePacket(length, protocol, buffer))
				return FALSE;
	}

	return TRUE;
}


#include "stdafx.h"
#include "User.h"


CUser::CUser()
{
}


CUser::~CUser()
{
}

//=================================================
// Initialize
// Info : �ʱ�ȭ
BOOL CUser::Initialize(VOID)
{
	// �Ӱ迵�� ó��
	CAutoCriticalSection section;

	// PacketHandler �ʱ�ȭ
	if (!CPacketHandler::Initialize())
	{
		CLog::WriteLogWithDate("CUser : Initialize - CPacketHandler::Initialize() Error!\n");

		return FALSE;
	}

	userName.clear();

	userState = eUserState::eUserState_NULL;

	bConnected = FALSE;

	heartBeat = 0;

	return TRUE;
}

//=================================================
// Release
// Info : �޸� ����
BOOL CUser::Release(VOID)
{
	// �Ӱ迵�� ó��
	CAutoCriticalSection section;

	connectedRoom = nullptr;

	if (!CPacketHandler::Release())
	{
		CLog::WriteLogWithDate("CUser : Release - CPacketHandler::Release() Error!\n");

		return FALSE;
	}

	return TRUE;
}

//=================================================
// Restore
// Info : User�� ����� ���� �ٽ� ������ ���� �� �ֵ��� �����Ѵ�.
// listenSocket - ���� ���� ����, �ٽ� ���ӿ�û�� ���� �� �ֵ��� �����ϴµ� ���δ�.
BOOL CUser::Restore(SOCKET listenSocket)
{
	// �Ӱ迵�� ó��
	CAutoCriticalSection section;

	if (!Release())
	{
		CLog::WriteLogWithDate("CUser : Restore - Release() Error!\n");

		return FALSE;
	}

	// �ʱ�ȭ + TCP���� ��� + ���� �ޱ�.
	return Initialize() && TCPSocket() && Accept(listenSocket);
}

//=================================================
// HeartBeatCheck
// Info : ��Ʈ��Ʈ�� �ִ�ġ�� �Ѿ�� Ŭ���̾�Ʈ�� �̻��� ����ٰ� �Ǵ�, ������ �����Ѵ�.
// listenSocket - ���� ���� ����, �ٽ� ���ӿ�û�� ���� �� �ֵ��� �����ϴµ� ���δ�.
VOID CUser::HeartBeatCheck(SOCKET listenSocket)
{	
	// �Ӱ迵�� ó��
	CAutoCriticalSection section;

	// ��Ʈ��Ʈ �ִ�ġ�� �Ѿ��ٸ�
	if (heartBeat > MAX_HEARTBEAT)
		Restore(listenSocket);
	else
		heartBeat++;
}

//=================================================
// GetSectionUserName
// Info : ������ �̸��� �����Ѵ�.
std::string CUser::GetSectionUserName(VOID)
{
	// �Ӱ迵�� ó��
	CAutoCriticalSection section;

	return userName;
}

//=================================================
// SetSectionUserName
// Info : ������ �̸��� �����Ѵ�.
// userName - ������ ������ �̸�
VOID CUser::SetSectionUserName(std::string userName)
{
	// �Ӱ迵�� ó��
	CAutoCriticalSection section;

	this->userName = userName;
}

//=================================================
// GetUserState
// Info : ������ ���¸� �����Ѵ�.
eUserState CUser::GetUserState(VOID)
{
	CAutoCriticalSection section;

	return userState;
}

//=================================================
// SetUserState
// Info : ������ ���¸� �����Ѵ�.
// userState - ������ ������ ����.
VOID CUser::SetUserState(eUserState userState)
{
	// �Ӱ迵�� ó��
	CAutoCriticalSection section;

	this->userState = userState;
}

//=================================================
// GetConnectedRoom
// Info : ����� ���� �����Ѵ�.
CRoom* CUser::GetConnectedRoom(VOID)
{
	// �Ӱ迵�� ó��
	CAutoCriticalSection section;

	return connectedRoom;
}

//=================================================
// SetConnectedRoom
// Info : ����� ���� �����Ѵ�.
// room - ������ ���� ������
VOID CUser::SetConnectedRoom(CRoom* room)
{
	// �Ӱ迵�� ó��
	CAutoCriticalSection section;

	connectedRoom = room;
}

//=================================================
// GetConnected
// Info : ���Ῡ�θ� �����Ѵ�.
BOOL CUser::GetConnected(VOID)
{
	// �Ӱ迵�� ó��
	CAutoCriticalSection section;

	return bConnected;
}

//=================================================
// SetConnected
// Info : ���� ���θ� �����Ѵ�.
// connected - ������ ���� ����
VOID CUser::SetConnected(BOOL connected)
{
	// �Ӱ迵�� ó��
	CAutoCriticalSection section;

	bConnected = connected;
}

//=================================================
// GetHeartBeat
// Info : ��Ʈ��Ʈ�� �����Ѵ�.
INT CUser::GetHeartBeat(VOID)
{
	// �Ӱ迵�� ó��
	CAutoCriticalSection section;

	return heartBeat;
}

//=================================================
// SetHeartBeat
// Info : ��Ʈ��Ʈ�� �����Ѵ�.
// count - ������ ��Ʈ��Ʈ ī��Ʈ
VOID CUser::SetHeartBeat(INT count)
{
	// �Ӱ迵�� ó��
	CAutoCriticalSection section;

	heartBeat = count;
}


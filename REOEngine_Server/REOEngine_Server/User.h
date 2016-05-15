#pragma once

class CRoom;

//=================================================
// CChatServer
// Info : ���� ��ü, ��Ʈ��ũ ó���� ���� CPacketHandler�� ��ӹ޴´�.
class CUser : public CPacketHandler
{
public:
	CUser();
	virtual ~CUser();

private :
	// ������ �̸�
	std::string		userName;
	// ������ ����
	eUserState		userState;
	// ������ ���� ������
	CRoom*			connectedRoom;

	// ���� ����
	BOOL				bConnected;
	// ������ ��Ʈ��Ʈ
	INT					heartBeat;

public :
	// �ʱ�ȭ
	BOOL	Initialize(VOID);
	// �޸� ����
	BOOL	Release(VOID);

	// User�� ����� ������ ������ ���� �� �ִ� ���·� �ǵ���.
	BOOL	Restore(SOCKET listenSocket);

	// ������ ��Ʈ��Ʈ�� üũ��.
	VOID	HeartBeatCheck(SOCKET listenSocket);

	// userName
	std::string	GetSectionUserName(VOID);
	VOID			SetSectionUserName(std::string userName);

	// userState
	eUserState	GetUserState(VOID);
	VOID			SetUserState(eUserState userState);

	// connectedRoom
	CRoom*		GetConnectedRoom(VOID);
	VOID			SetConnectedRoom(CRoom* room);

	// bConnected
	BOOL	GetConnected(VOID);
	VOID	SetConnected(BOOL connected);

	// heartBeat
	INT				GetHeartBeat(VOID);
	VOID			SetHeartBeat(INT count);
};


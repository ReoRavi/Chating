#pragma once

//=================================================
// CRoomManager
// Info : ���� �����ϴ� �Ŵ��� ��ü
class CRoomManager
{
public:
	CRoomManager();
	virtual ~CRoomManager();

private :
	// ���� �����ϴ� Vector
	std::vector<CRoom*>	vRooms;
	// Ȱ��ȭ�� ���� ��
	INT activeRoomCount;
	
public:
	// �ʱ�ȭ
	BOOL	Initialize();
	// �޸� ����
	BOOL	Release();

	// vRoom
	CRoom*	GetRoom(INT index);
	VOID		SetRoom(CRoom* room, INT index);

	// activeRoomCount
	INT			GetActiveRoomCount();
	VOID		SetActiveRoomCount(INT count);

public :
	// ���� �����.
	BOOL	CreateRoom(CUser* host, std::string roomName);
};


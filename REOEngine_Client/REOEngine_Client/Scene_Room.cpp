#include "stdafx.h"
#include "Scene_Room.h"


CScene_Room::CScene_Room()
{
}


CScene_Room::~CScene_Room()
{
}

//===============================================================
// Initialize
// Info : �ʱ�ȭ
BOOL CScene_Room::Initialize(VOID)
{
	CRoomUI::DrawAllSection();

	// �˸��� �α׿� �ְ�, ����Ѵ�.
	std::string notice[10] = {
		"����������������������������������������������������������������������������������",
		"\'(Exit)\' �� �Է����ֽø� �κ�� ���ư��ϴ�.",
		"�ִ� 20����� ü���� �����մϴ�. ü���� �� ����� 60���ڱ����� �����մϴ�.",
		"Developer : Kimyoumin(Anione High School - 3�г�)",
		"Chating Program Ver.1.. Last Developed : 2016-05-12",
		"����������������������������������������������������������������������������������",
		" ",
		" ", 
		" ", 
		" " };

	for each (std::string message in notice)
	{
		CRoomUI::InputNotice(message);
	}

	CRoomUI::DrawChating();

	return TRUE;
}

//===============================================================
// Process
// Info : ����
VOID CScene_Room::Process(VOID)
{
	std::string message;
	BYTE buffer[256] = { 0, };

	// Ű���尡 �Է� �Ǹ�
	if (kbhit())
	{
		// ���ڸ� �޴´�.
		getline(std::cin, message);

		// ġȯ �� ���� �޼����� ��Ƶ� string. 
		std::string temp = message;

		// �ҹ��ڷ� ġȯ ��,
		std::transform(message.begin(), message.end(), message.begin(), tolower);

		// ����ڰ� �濡�� �����°��� ��û��
		if (message == "(exit)")
		{
			//  ������ ������ ��û�� ������.
			g_pNetwork->WritePacket(
				0,
				ePACKET_PROTOCOL::ePacket_Room_Exit,
				buffer);
		}
		else
		{
			message = temp;

			// ������ �޼����� ������.
			g_pNetwork->WritePacket(
				PT_WRITE_ROOM_MESSAGE(buffer, message),
				ePACKET_PROTOCOL::ePacket_Room_Message,
				buffer);
		}

		CRoomUI::DrawInputSection();
	}
}

//===============================================================
// Release
// Info : �޸� ����
BOOL CScene_Room::Release(VOID)
{
	return TRUE;
}

//===============================================================
// ReceiveChating
// Info : �����κ��� ���� �޼����� ó���Ѵ�.
// message - ���� �޼���
VOID CScene_Room::ReceiveChating(std::string message)
{
	CRoomUI::DrawChatSection();
	CRoomUI::InputChating(message);
	CRoomUI::DrawChating();
}
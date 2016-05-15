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
// Info : 초기화
BOOL CScene_Room::Initialize(VOID)
{
	CRoomUI::DrawAllSection();

	// 알림을 로그에 넣고, 출력한다.
	std::string notice[10] = {
		"└───────────────────────────────────────┘",
		"\'(Exit)\' 을 입력해주시면 로비로 돌아갑니다.",
		"최대 20명까지 체팅이 가능합니다. 체팅은 한 문장당 60글자까지만 가능합니다.",
		"Developer : Kimyoumin(Anione High School - 3학년)",
		"Chating Program Ver.1.. Last Developed : 2016-05-12",
		"┌───────────────────────────────────────┐",
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
// Info : 루프
VOID CScene_Room::Process(VOID)
{
	std::string message;
	BYTE buffer[256] = { 0, };

	// 키보드가 입력 되면
	if (kbhit())
	{
		// 문자를 받는다.
		getline(std::cin, message);

		// 치환 전 원래 메세지를 담아둘 string. 
		std::string temp = message;

		// 소문자로 치환 후,
		std::transform(message.begin(), message.end(), message.begin(), tolower);

		// 사용자가 방에서 나가는것을 요청함
		if (message == "(exit)")
		{
			//  서버에 나가기 요청을 보낸다.
			g_pNetwork->WritePacket(
				0,
				ePACKET_PROTOCOL::ePacket_Room_Exit,
				buffer);
		}
		else
		{
			message = temp;

			// 서버에 메세지를 보낸다.
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
// Info : 메모리 해제
BOOL CScene_Room::Release(VOID)
{
	return TRUE;
}

//===============================================================
// ReceiveChating
// Info : 서버로부터 받은 메세지를 처리한다.
// message - 받은 메세지
VOID CScene_Room::ReceiveChating(std::string message)
{
	CRoomUI::DrawChatSection();
	CRoomUI::InputChating(message);
	CRoomUI::DrawChating();
}
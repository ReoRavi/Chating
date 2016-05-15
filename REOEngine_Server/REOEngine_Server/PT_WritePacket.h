#pragma once

//=================================================
// PT_WRITE_LOBBY_INFOUPDATE
// Info : 로비의 정보를 업데이트 하는 패킷을 만든다.
// rooms - 방들을 관리하는 매니저 객체
inline DWORD PT_WRITE_LOBBY_INFOUPDATE(BYTE* buffer, CRoomManager* rooms)
{
	CPacketStream* packetStream = new CPacketStream();
	packetStream->SetBuffer(buffer);

	*packetStream << rooms->GetActiveRoomCount();

	for (int index = 0; index < rooms->GetActiveRoomCount(); index++)
	{
		CRoom* room = rooms->GetRoom(index);

		*packetStream << room->GetRoomIndex();
		*packetStream << room->GetRoomHost()->GetSectionUserName().c_str();
		*packetStream << '\0';
		*packetStream << room->GetRoomName().c_str();
		*packetStream << '\0';
	}

	return packetStream->GetBufferLength();
}

//=================================================
// PT_WRITE_LOBBY_JOINROOM
// Info : 방에 접속 요청을 한 결과를 포함한 패킷을 만든다.
// bSuccess - 접속 요청의 결과
inline DWORD PT_WRITE_LOBBY_JOINROOM(BYTE* buffer, BOOL bSuccess)
{
	CPacketStream* packetStream = new CPacketStream();
	packetStream->SetBuffer(buffer);

	*packetStream << bSuccess;

	return packetStream->GetBufferLength();
}

//=================================================
// PT_WRITE_CREATEROOM_REQUEST
// Info : 방 만들기를 요청한 결과를 담은 패킷을 만든다.
// bSuccess - 방 만들기의 결과
inline DWORD PT_WRITE_CREATEROOM_REQUEST(BYTE* buffer, BOOL bSuccess)
{
	CPacketStream* packetStream = new CPacketStream();
	packetStream->SetBuffer(buffer);

	*packetStream << bSuccess;

	return packetStream->GetBufferLength();
}

//=================================================
// PT_WRITE_ROOM_MESSAGE
// Info : 유저의 이름과 메세지를 담은 패킷을 만든다.
// userName - 유저의 이름
// message - 수신된 메세지
inline DWORD PT_WRITE_ROOM_MESSAGE(BYTE* buffer, std::string userName, std::string message)
{
	CPacketStream* packetStream = new CPacketStream();
	packetStream->SetBuffer(buffer);

	*packetStream << userName.c_str();
	*packetStream << " : ";
	*packetStream << message.c_str();

	return packetStream->GetBufferLength();
}

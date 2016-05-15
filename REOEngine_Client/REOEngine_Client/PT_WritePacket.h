#pragma once

//================================================================
// PT_WRITE_LOGIN_USERINFO
// info - 로비 정보 업데이트 패킷을 만든다.
// buffer - 패킷의 버퍼
// userName - 유저의 이름
inline DWORD PT_WRITE_LOGIN_USERINFO(BYTE* buffer, std::string &userName)
{
	CPacketStream* packet = new CPacketStream();
	packet->SetBuffer(buffer);

	*packet << userName.c_str();

	return packet->GetBufferLength();
}

//================================================================
// PT_WRITE_LOBBY_JOINROOM
// info - 방 연결 요청 패킷을 만든다.
// buffer - 패킷의 버퍼
// cursorCount - 커서의 카운트 (방의 인덱스,)
inline DWORD PT_WRITE_LOBBY_JOINROOM(BYTE* buffer, INT cursorCount)
{
	CPacketStream* packet = new CPacketStream();
	packet->SetBuffer(buffer);

	*packet << cursorCount;

	return packet->GetBufferLength();
}

//================================================================
// PT_WRITE_CREATEROOM_REQUEST
// info - 방 생성 요청 패킷을 만든다.
// buffer - 패킷의 버퍼
// roomName - 방의 이름
inline DWORD PT_WRITE_CREATEROOM_REQUEST(BYTE* buffer, std::string &roomName)
{
	CPacketStream* packet = new CPacketStream();
	packet->SetBuffer(buffer);

	*packet << roomName.c_str();

	return packet->GetBufferLength();
}

//================================================================
// PT_WRITE_ROOM_MESSAGE
// info - 메세지 전송 패킷을 만든다.
// buffer - 패킷의 버퍼
// message - 송신할 메세지
inline DWORD PT_WRITE_ROOM_MESSAGE(BYTE* buffer, std::string &message)
{
	CPacketStream* packet = new CPacketStream();
	packet->SetBuffer(buffer);

	*packet << message.c_str();

	return packet->GetBufferLength();
}
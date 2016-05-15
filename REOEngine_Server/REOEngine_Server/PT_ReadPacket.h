#pragma once

//=================================================
// PT_READ_LOGIN_USERINFO
// Info : 유저의 이름을 받는다.
// userName - 유저의 이름
inline VOID PT_READ_LOGIN_USERINFO(BYTE* buffer, std::string &userName)
{
	CPacketStream* stream = new CPacketStream();
	stream->SetBuffer(buffer);

	char name[MAX_BUFFER_LENGTH] = { 0, };

	*stream >> name;

	userName = name;
}

//=================================================
// PT_READ_LOBBY_JOINROOM
// Info : 유저가 방에 접속을 요청했다.
// index - 방의 인덱스
inline VOID PT_READ_LOBBY_JOINROOM(BYTE* buffer, INT &index)
{
	CPacketStream* stream = new CPacketStream();
	stream->SetBuffer(buffer);

	*stream >> index;
}

//=================================================
// PT_READ_CREATEROOM_REQUEST
// Info : 유저가 방 만들기를 요청했다.
// roomName - 방의 이름
inline VOID PT_READ_CREATEROOM_REQUEST(BYTE* buffer, std::string &roomName)
{
	CPacketStream* stream = new CPacketStream();
	stream->SetBuffer(buffer);

	char name[MAX_BUFFER_LENGTH] = { 0, };

	*stream >> name;

	roomName = name;
}

//=================================================
// PT_READ_ROOM_MESSAGE
// Info : 방에 메세지가 수신되었다.
// message - 수신된 메세지
inline VOID PT_READ_ROOM_MESSAGE(BYTE* buffer, std::string &message)
{
	CPacketStream* stream = new CPacketStream();
	stream->SetBuffer(buffer);

	char msg[MAX_BUFFER_LENGTH] = { 0, };

	*stream >> msg;

	message = msg;
}

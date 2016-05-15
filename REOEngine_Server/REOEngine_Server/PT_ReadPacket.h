#pragma once

//=================================================
// PT_READ_LOGIN_USERINFO
// Info : ������ �̸��� �޴´�.
// userName - ������ �̸�
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
// Info : ������ �濡 ������ ��û�ߴ�.
// index - ���� �ε���
inline VOID PT_READ_LOBBY_JOINROOM(BYTE* buffer, INT &index)
{
	CPacketStream* stream = new CPacketStream();
	stream->SetBuffer(buffer);

	*stream >> index;
}

//=================================================
// PT_READ_CREATEROOM_REQUEST
// Info : ������ �� ����⸦ ��û�ߴ�.
// roomName - ���� �̸�
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
// Info : �濡 �޼����� ���ŵǾ���.
// message - ���ŵ� �޼���
inline VOID PT_READ_ROOM_MESSAGE(BYTE* buffer, std::string &message)
{
	CPacketStream* stream = new CPacketStream();
	stream->SetBuffer(buffer);

	char msg[MAX_BUFFER_LENGTH] = { 0, };

	*stream >> msg;

	message = msg;
}

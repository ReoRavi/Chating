#pragma once

//================================================================
// PT_WRITE_LOGIN_USERINFO
// info - �κ� ���� ������Ʈ ��Ŷ�� �����.
// buffer - ��Ŷ�� ����
// userName - ������ �̸�
inline DWORD PT_WRITE_LOGIN_USERINFO(BYTE* buffer, std::string &userName)
{
	CPacketStream* packet = new CPacketStream();
	packet->SetBuffer(buffer);

	*packet << userName.c_str();

	return packet->GetBufferLength();
}

//================================================================
// PT_WRITE_LOBBY_JOINROOM
// info - �� ���� ��û ��Ŷ�� �����.
// buffer - ��Ŷ�� ����
// cursorCount - Ŀ���� ī��Ʈ (���� �ε���,)
inline DWORD PT_WRITE_LOBBY_JOINROOM(BYTE* buffer, INT cursorCount)
{
	CPacketStream* packet = new CPacketStream();
	packet->SetBuffer(buffer);

	*packet << cursorCount;

	return packet->GetBufferLength();
}

//================================================================
// PT_WRITE_CREATEROOM_REQUEST
// info - �� ���� ��û ��Ŷ�� �����.
// buffer - ��Ŷ�� ����
// roomName - ���� �̸�
inline DWORD PT_WRITE_CREATEROOM_REQUEST(BYTE* buffer, std::string &roomName)
{
	CPacketStream* packet = new CPacketStream();
	packet->SetBuffer(buffer);

	*packet << roomName.c_str();

	return packet->GetBufferLength();
}

//================================================================
// PT_WRITE_ROOM_MESSAGE
// info - �޼��� ���� ��Ŷ�� �����.
// buffer - ��Ŷ�� ����
// message - �۽��� �޼���
inline DWORD PT_WRITE_ROOM_MESSAGE(BYTE* buffer, std::string &message)
{
	CPacketStream* packet = new CPacketStream();
	packet->SetBuffer(buffer);

	*packet << message.c_str();

	return packet->GetBufferLength();
}
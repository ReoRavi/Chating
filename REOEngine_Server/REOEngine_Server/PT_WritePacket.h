#pragma once

//=================================================
// PT_WRITE_LOBBY_INFOUPDATE
// Info : �κ��� ������ ������Ʈ �ϴ� ��Ŷ�� �����.
// rooms - ����� �����ϴ� �Ŵ��� ��ü
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
// Info : �濡 ���� ��û�� �� ����� ������ ��Ŷ�� �����.
// bSuccess - ���� ��û�� ���
inline DWORD PT_WRITE_LOBBY_JOINROOM(BYTE* buffer, BOOL bSuccess)
{
	CPacketStream* packetStream = new CPacketStream();
	packetStream->SetBuffer(buffer);

	*packetStream << bSuccess;

	return packetStream->GetBufferLength();
}

//=================================================
// PT_WRITE_CREATEROOM_REQUEST
// Info : �� ����⸦ ��û�� ����� ���� ��Ŷ�� �����.
// bSuccess - �� ������� ���
inline DWORD PT_WRITE_CREATEROOM_REQUEST(BYTE* buffer, BOOL bSuccess)
{
	CPacketStream* packetStream = new CPacketStream();
	packetStream->SetBuffer(buffer);

	*packetStream << bSuccess;

	return packetStream->GetBufferLength();
}

//=================================================
// PT_WRITE_ROOM_MESSAGE
// Info : ������ �̸��� �޼����� ���� ��Ŷ�� �����.
// userName - ������ �̸�
// message - ���ŵ� �޼���
inline DWORD PT_WRITE_ROOM_MESSAGE(BYTE* buffer, std::string userName, std::string message)
{
	CPacketStream* packetStream = new CPacketStream();
	packetStream->SetBuffer(buffer);

	*packetStream << userName.c_str();
	*packetStream << " : ";
	*packetStream << message.c_str();

	return packetStream->GetBufferLength();
}

#pragma once

//================================================================
// PT_READ_LOBBY_INFOUPDATE
// info - �κ� ���� ������Ʈ ��Ŷ�� ó���Ѵ�.
// buffer - ��Ŷ�� ����
// roomCount - ���� ����
// roomIndex - ���� ��ȣ 
// roomHost - ���� ȣ��Ʈ �̸� 
// roomName - ���� �̸� 
inline VOID PT_READ_LOBBY_INFOUPDATE(BYTE* buffer, INT &roomCount, INT roomIndex[], std::string roomHost[], std::string roomName[])
{
	CPacketStream* packetStream = new CPacketStream();
	packetStream->SetBuffer(buffer);

	// ���� ������ �޴´�.
	*packetStream >> roomCount;

	char temphostName[20] = { 0, };
	char tempRoomName[20] = { 0, };
	char temp;

	// ���� ������ŭ ������ ���´�.
	for (int index = 0; index < roomCount; index++)
	{
		ZeroMemory(temphostName, sizeof(temphostName));
		ZeroMemory(tempRoomName, sizeof(tempRoomName));

		*packetStream >> roomIndex[index];
		*packetStream >> temphostName;
		*packetStream >> temp;
		*packetStream >> tempRoomName;
		*packetStream >> temp;

		roomHost[index] = temphostName;
		roomName[index] = tempRoomName;
	}
}

//================================================================
// PT_READ_LOBBY_JOINROOM
// info - �� ���� ��û ��Ŷ�� ó���Ѵ�.
// buffer - ��Ŷ�� ����
// bSuccess - ���� ����
inline VOID PT_READ_LOBBY_JOINROOM(BYTE* buffer, BOOL &bSuccess)
{
	CPacketStream* packetStream = new CPacketStream();
	packetStream->SetBuffer(buffer);

	*packetStream >> bSuccess;
}

//================================================================
// PT_READ_CREATEROOM_REQUEST
// info - �� ���� ��û ��Ŷ�� ó���Ѵ�.
// buffer - ��Ŷ�� ����
// bSuccess - ���� ����
inline VOID PT_READ_CREATEROOM_REQUEST(BYTE* buffer, BOOL &bSuccess)
{
	CPacketStream* packetStream = new CPacketStream();
	packetStream->SetBuffer(buffer);

	*packetStream >> bSuccess;
}

//================================================================
// PT_READ_ROOM_MESSAGE
// info - �� ���� ��û ��Ŷ�� ó���Ѵ�.
// buffer - ��Ŷ�� ����
// message - ���ŵ� �޼���
inline VOID PT_READ_ROOM_MESSAGE(BYTE* buffer, std::string &message)
{
	CPacketStream* packetStream = new CPacketStream();
	packetStream->SetBuffer(buffer);

	char tempMessage[MAX_BUFFER_LENGTH] = { 0, };

	*packetStream >> tempMessage;

	message = tempMessage;
}
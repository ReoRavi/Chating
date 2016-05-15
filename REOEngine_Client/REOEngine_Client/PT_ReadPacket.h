#pragma once

//================================================================
// PT_READ_LOBBY_INFOUPDATE
// info - 로비 정보 업데이트 패킷을 처리한다.
// buffer - 패킷의 버퍼
// roomCount - 방의 갯수
// roomIndex - 방의 번호 
// roomHost - 방의 호스트 이름 
// roomName - 방의 이름 
inline VOID PT_READ_LOBBY_INFOUPDATE(BYTE* buffer, INT &roomCount, INT roomIndex[], std::string roomHost[], std::string roomName[])
{
	CPacketStream* packetStream = new CPacketStream();
	packetStream->SetBuffer(buffer);

	// 방의 갯수를 받는다.
	*packetStream >> roomCount;

	char temphostName[20] = { 0, };
	char tempRoomName[20] = { 0, };
	char temp;

	// 방의 갯수만큼 정보를 얻어온다.
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
// info - 방 연결 요청 패킷을 처리한다.
// buffer - 패킷의 버퍼
// bSuccess - 성공 여부
inline VOID PT_READ_LOBBY_JOINROOM(BYTE* buffer, BOOL &bSuccess)
{
	CPacketStream* packetStream = new CPacketStream();
	packetStream->SetBuffer(buffer);

	*packetStream >> bSuccess;
}

//================================================================
// PT_READ_CREATEROOM_REQUEST
// info - 방 생성 요청 패킷을 처리한다.
// buffer - 패킷의 버퍼
// bSuccess - 성공 여부
inline VOID PT_READ_CREATEROOM_REQUEST(BYTE* buffer, BOOL &bSuccess)
{
	CPacketStream* packetStream = new CPacketStream();
	packetStream->SetBuffer(buffer);

	*packetStream >> bSuccess;
}

//================================================================
// PT_READ_ROOM_MESSAGE
// info - 방 연결 요청 패킷을 처리한다.
// buffer - 패킷의 버퍼
// message - 수신된 메세지
inline VOID PT_READ_ROOM_MESSAGE(BYTE* buffer, std::string &message)
{
	CPacketStream* packetStream = new CPacketStream();
	packetStream->SetBuffer(buffer);

	char tempMessage[MAX_BUFFER_LENGTH] = { 0, };

	*packetStream >> tempMessage;

	message = tempMessage;
}
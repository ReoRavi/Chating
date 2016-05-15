#include "headers.h"
#include "PacketStream.h"


CPacketStream::CPacketStream()
{
	buffer = NULL;
	bufferLength = 0;
}

CPacketStream::~CPacketStream()
{
}

//====================================================
// GetBuffer
// Info : 버퍼를 리턴한다.
BYTE* CPacketStream::GetBuffer(VOID)
{
	return buffer;
}

//====================================================
// SetBuffer
// Info : 버퍼를 설정한다. 
// bufferPointer - 설정할 버퍼의 포인터
BOOL CPacketStream::SetBuffer(BYTE* bufferPointer)
{
	if (buffer)
		return FALSE;

	buffer = bufferPointer;
	bufferLength = 0;

	return TRUE;
}

//====================================================
// GetBufferLength
// Info : 버퍼의 길이를 리턴한다.
INT CPacketStream::GetBufferLength(VOID)
{
	return bufferLength;
}

//====================================================
// GetArrayLength
// Info : 배열의 길이를 구한다.
// Data - 길이를 구할 BYTE* 변수
INT CPacketStream::GetArrayLength(BYTE* Data)
{
	INT count = 0;

	while (Data[count++] != '\0')
	{

	}

	// \0을 제외하고 보낸다.
	return count - 1;
}

//====================================================
// GetArrayLength
// Info : 배열의 길이를 구한다.
// Data - 길이를 구할 Char* 변수
INT CPacketStream::GetArrayLength(char* Data)
{
	INT count = 0;

	while (Data[count++] != '\0')
	{

	}

	// \0을 제외하고 보낸다.
	return count - 1;
}

//====================================================
// operator << 
// Info : 패킷에 데이터를 넣는다. 데이터를 넣을 때마다 자동으로 길이가 갱신된다.
CPacketStream CPacketStream::operator << (bool Data)
{
	memcpy(buffer + bufferLength, &Data, sizeof(bool));

	bufferLength += sizeof(bool);

	return *this;
}

CPacketStream CPacketStream::operator << (BYTE Data)
{
	memcpy(buffer + bufferLength, &Data, sizeof(BYTE));

	bufferLength += sizeof(BYTE);

	return *this;
}

CPacketStream CPacketStream::operator << (BYTE* Data)
{
	INT dataLength = GetArrayLength(Data);

	memcpy(buffer + bufferLength, Data, dataLength);

	bufferLength += dataLength;

	return *this;
}

CPacketStream CPacketStream::operator << (char Data)
{
	memcpy(buffer + bufferLength, &Data, sizeof(char));

	bufferLength += sizeof(char);

	return *this;
}

CPacketStream CPacketStream::operator << (char* Data)
{
	INT dataLength = GetArrayLength(Data);

	memcpy(buffer + bufferLength, Data, dataLength);

	bufferLength += dataLength;

	return *this;
}

CPacketStream CPacketStream::operator << (const char* Data)
{
	INT dataLength = GetArrayLength(const_cast<char*>(Data));

	memcpy(buffer + bufferLength, Data, dataLength);

	bufferLength += dataLength;

	return *this;
}

CPacketStream CPacketStream::operator << (SHORT Data)
{
	memcpy(buffer + bufferLength, &Data, sizeof(SHORT));

	bufferLength += sizeof(SHORT);

	return *this;
}

CPacketStream CPacketStream::operator << (INT Data)
{
	memcpy(buffer + bufferLength, &Data, sizeof(INT));

	bufferLength += sizeof(INT);

	return *this;
}

CPacketStream CPacketStream::operator << (LONG Data)
{
	memcpy(buffer + bufferLength, &Data, sizeof(LONG));

	bufferLength += sizeof(LONG);

	return *this;
}

CPacketStream CPacketStream::operator << (FLOAT Data)
{
	memcpy(buffer + bufferLength, &Data, sizeof(FLOAT));

	bufferLength += sizeof(FLOAT);

	return *this;
}

CPacketStream CPacketStream::operator << (DOUBLE Data)
{
	memcpy(buffer + bufferLength, &Data, sizeof(DOUBLE));

	bufferLength += sizeof(DOUBLE);

	return *this;
}

CPacketStream CPacketStream::operator >> (bool &Data)
{
	memcpy(&Data, buffer + bufferLength, sizeof(bool));

	bufferLength += sizeof(bool);

	return *this;
}

CPacketStream CPacketStream::operator >> (BYTE &Data)
{
	memcpy(&Data, buffer + bufferLength, sizeof(BYTE));

	bufferLength += sizeof(BYTE);

	return *this;
}

CPacketStream CPacketStream::operator >> (BYTE* &Data)
{
	INT dataLength = GetArrayLength(buffer + bufferLength);

	memcpy(Data, buffer + bufferLength, dataLength);

	bufferLength += dataLength;

	return *this;
}

CPacketStream CPacketStream::operator >> (char &Data)
{
	memcpy(&Data, buffer + bufferLength, sizeof(char));

	bufferLength += sizeof(char);

	return *this;
}

CPacketStream CPacketStream::operator >> (char* Data)
{
	INT dataLength = GetArrayLength(buffer + bufferLength);

	memcpy(Data, buffer + bufferLength, dataLength);

	bufferLength += dataLength;

	return *this;
}

CPacketStream CPacketStream::operator >> (const char* Data)
{
	INT dataLength = GetArrayLength(buffer + bufferLength);

	memcpy(const_cast<char*>(Data), buffer + bufferLength, dataLength);

	bufferLength += dataLength;

	return *this;
}

CPacketStream CPacketStream::operator >> (SHORT &Data)
{
	memcpy(&Data, buffer + bufferLength, sizeof(SHORT));

	bufferLength += sizeof(SHORT);

	return *this;
}

CPacketStream CPacketStream::operator >> (INT &Data)
{
	memcpy(&Data, buffer + bufferLength, sizeof(INT));

	bufferLength += sizeof(INT);

	return *this;
}

CPacketStream CPacketStream::operator >> (LONG &Data)
{
	memcpy(&Data, buffer + bufferLength, sizeof(LONG));

	bufferLength += sizeof(LONG);

	return *this;
}

CPacketStream CPacketStream::operator >> (FLOAT &Data)
{
	memcpy(&Data, buffer + bufferLength, sizeof(FLOAT));

	bufferLength += sizeof(FLOAT);

	return *this;
}

CPacketStream CPacketStream::operator >> (DOUBLE &Data)
{
	memcpy(&Data, buffer + bufferLength, sizeof(DOUBLE));

	bufferLength += sizeof(DOUBLE);

	return *this;
}


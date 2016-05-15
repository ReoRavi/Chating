#pragma once

//===============================================
// CPacketStream
// Info : 패킷을 만들어주는 클래스.
class CPacketStream
{
public:
	CPacketStream();
	virtual ~CPacketStream();

private:
	// 패킷의 버퍼
	BYTE*			buffer;
	// 버퍼의 길이
	DWORD		bufferLength;

public:
	// 버퍼를 리턴한다.
	BYTE*		GetBuffer(VOID);
	// 버퍼를 설정한다.
	BOOL		SetBuffer(BYTE* bufferPointer);

	// 버퍼의 길이를 리턴한다.
	INT			GetBufferLength(VOID);

	// 문자열 혹은 배열 입력 시, 연산자 오버로딩은 인자값을 하나밖에 넣지 못하기 때문에 문제가 생긴다. 
	// '\0' 이 나올 때 까지 루프를 돌려 길이를 구한다.
	INT	GetArrayLength(BYTE* Data);
	INT	GetArrayLength(char* Data);

	// 연산자 오버로딩을 통해 쉽게 패킷을 제작한다.
	CPacketStream operator << (bool Data);
	CPacketStream operator << (BYTE Data);
	CPacketStream operator << (BYTE* Data);
	CPacketStream operator << (char Data);
	CPacketStream operator << (char* Data);
	CPacketStream operator << (const char* Data);
	CPacketStream operator << (SHORT Data);
	CPacketStream operator << (INT Data);
	CPacketStream operator << (LONG Data);
	CPacketStream operator << (FLOAT Data);
	CPacketStream operator << (DOUBLE Data);

	CPacketStream operator >> (bool &Data);
	CPacketStream operator >> (BYTE &Data);
	CPacketStream operator >> (BYTE* &Data);
	CPacketStream operator >> (char &Data);
	CPacketStream operator >> (char* Data);
	CPacketStream operator >> (const char* Data);
	CPacketStream operator >> (SHORT &Data);
	CPacketStream operator >> (INT &Data);
	CPacketStream operator >> (LONG &Data);
	CPacketStream operator >> (FLOAT &Data);
	CPacketStream operator >> (DOUBLE &Data);
};


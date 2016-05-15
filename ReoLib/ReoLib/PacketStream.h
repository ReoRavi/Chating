#pragma once

//===============================================
// CPacketStream
// Info : ��Ŷ�� ������ִ� Ŭ����.
class CPacketStream
{
public:
	CPacketStream();
	virtual ~CPacketStream();

private:
	// ��Ŷ�� ����
	BYTE*			buffer;
	// ������ ����
	DWORD		bufferLength;

public:
	// ���۸� �����Ѵ�.
	BYTE*		GetBuffer(VOID);
	// ���۸� �����Ѵ�.
	BOOL		SetBuffer(BYTE* bufferPointer);

	// ������ ���̸� �����Ѵ�.
	INT			GetBufferLength(VOID);

	// ���ڿ� Ȥ�� �迭 �Է� ��, ������ �����ε��� ���ڰ��� �ϳ��ۿ� ���� ���ϱ� ������ ������ �����. 
	// '\0' �� ���� �� ���� ������ ���� ���̸� ���Ѵ�.
	INT	GetArrayLength(BYTE* Data);
	INT	GetArrayLength(char* Data);

	// ������ �����ε��� ���� ���� ��Ŷ�� �����Ѵ�.
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


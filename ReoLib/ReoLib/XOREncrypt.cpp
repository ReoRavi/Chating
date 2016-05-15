#include "headers.h"
#include "XOREncrypt.h"

// Key�� XOR ������ �Ͽ� ���۸� ��ȣȭ�Ѵ�. ��ȣȭ �Ҷ��� ���� ������� ����.
const char Key[XOR_KEYCOUNT] = { '1', 'C', '8', 'A', 'Y', '6', '2', 'K', 'O', '2', 'R', '5', 'E', '8', 'A', 'R', 'D', '0', 'I', 'V', 'D', 'E', '3', 'V', '7', 'E', 'R', '8', 'S' };

//==================================================
// CreateLogFile
// Info : ��ȣȭ.
// buffer - ��ȣȭ�� ����
// length - ������ ����
BOOL CXOREncrypt::Encrypt(BYTE* buffer, DWORD length)
{
	for (int index = 0; index < length; index++)
	{
		buffer[index] = buffer[index] ^ Key[index % sizeof(Key)];
	}

	return TRUE;
}

//==================================================
// CreateLogFile
// Info : ��ȣȭ.
// buffer - ��ȣȭ�� ����
// length - ������ ����
BOOL CXOREncrypt::Decrypt(BYTE* buffer, DWORD length)
{
	for (int index = 0; index < length; index++)
	{
		buffer[index] = buffer[index] ^ Key[index % sizeof(Key)];
	}

	return TRUE;
}
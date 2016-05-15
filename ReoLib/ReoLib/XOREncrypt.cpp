#include "headers.h"
#include "XOREncrypt.h"

// Key와 XOR 연산을 하여 버퍼를 암호화한다. 복호화 할때도 같은 방식으로 진행.
const char Key[XOR_KEYCOUNT] = { '1', 'C', '8', 'A', 'Y', '6', '2', 'K', 'O', '2', 'R', '5', 'E', '8', 'A', 'R', 'D', '0', 'I', 'V', 'D', 'E', '3', 'V', '7', 'E', 'R', '8', 'S' };

//==================================================
// CreateLogFile
// Info : 암호화.
// buffer - 암호화할 버퍼
// length - 버퍼의 길이
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
// Info : 복호화.
// buffer - 암호화할 버퍼
// length - 버퍼의 길이
BOOL CXOREncrypt::Decrypt(BYTE* buffer, DWORD length)
{
	for (int index = 0; index < length; index++)
	{
		buffer[index] = buffer[index] ^ Key[index % sizeof(Key)];
	}

	return TRUE;
}
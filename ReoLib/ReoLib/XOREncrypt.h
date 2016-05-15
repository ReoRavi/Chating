#pragma once

//===============================================
// CXOREncrypt
// Info : 버퍼를 암호화하고, 복호화한다.
class CXOREncrypt
{
public :
	// 암호화
	static BOOL	Encrypt(BYTE* buffer, DWORD length);
	// 복호화
	static BOOL	Decrypt(BYTE* buffer, DWORD length);
};


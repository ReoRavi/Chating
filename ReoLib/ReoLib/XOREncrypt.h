#pragma once

//===============================================
// CXOREncrypt
// Info : ���۸� ��ȣȭ�ϰ�, ��ȣȭ�Ѵ�.
class CXOREncrypt
{
public :
	// ��ȣȭ
	static BOOL	Encrypt(BYTE* buffer, DWORD length);
	// ��ȣȭ
	static BOOL	Decrypt(BYTE* buffer, DWORD length);
};


#pragma once

//===============================================
// CAutoCriticalSection
// Info : �Ӱ迵�� ó���� �ڵ����� ���ִ� Ŭ����.
// �����ڿ� �Ҹ����� ȣ�� ������ �̿��Ͽ� �Ӱ迵�� ó���� �Ѵ�.

class CCriticalSection;

class CAutoCriticalSection
{
public:
	CAutoCriticalSection();
	~CAutoCriticalSection();

private :
	// �ϳ��� CCriticalSection ������ ������ �۵���. Static���� ����.
	static CCriticalSection criticalSection;
};
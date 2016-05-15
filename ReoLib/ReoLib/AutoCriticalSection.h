#pragma once

//===============================================
// CAutoCriticalSection
// Info : 임계영역 처리를 자동으로 해주는 클래스.
// 생성자와 소멸자의 호출 시점을 이용하여 임계영역 처리를 한다.

class CCriticalSection;

class CAutoCriticalSection
{
public:
	CAutoCriticalSection();
	~CAutoCriticalSection();

private :
	// 하나의 CCriticalSection 변수를 가지고 작동함. Static으로 선언.
	static CCriticalSection criticalSection;
};
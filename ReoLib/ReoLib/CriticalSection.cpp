#include "Headers.h"
#include "CriticalSection.h"

// 생성과 동시에 임계영역을 사용하기위한 초기화를 진행함.
CCriticalSection::CCriticalSection()
{
	::InitializeCriticalSection(&section);
}

// 소멸과 동시에 임계영역의 사용을 끝냄.
CCriticalSection::~CCriticalSection()
{
	::DeleteCriticalSection(&section);
}

//==================================================
// EnterSection
// Info : 임계영역의 처리를 시작한다.
VOID CCriticalSection::EnterSection(VOID)
{
	::EnterCriticalSection(&section);
}

//==================================================
// LeaveSection
// Info : 임계영역의 처리를 끝낸다.
VOID CCriticalSection::LeaveSection(VOID)
{
	::LeaveCriticalSection(&section);
}
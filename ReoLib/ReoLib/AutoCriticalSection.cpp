#include "Headers.h"
#include "CriticalSection.h"
#include "AutoCriticalSection.h"

// static 변수를 사용하기 위해 선언해준다.
CCriticalSection CAutoCriticalSection::criticalSection;

// 변수의 할당과 동시에 생성자가 실행되고, 해제됨과 동시에 소멸자가 실행됨.
// 임계영역 처리가 필요한 부분에 변수를 생성해서 자동으로 임계영역을 처리함.
CAutoCriticalSection::CAutoCriticalSection()
{
	// 임계영역 시작
	criticalSection.EnterSection();
}

CAutoCriticalSection::~CAutoCriticalSection()
{
	// 임계영역 끝
	criticalSection.LeaveSection();
}

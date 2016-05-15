#pragma once

//===============================================
// CCriticalSection
// Info : 임계영역 처리를 담당하는 클래스.
class CCriticalSection
{
public:
	CCriticalSection();
	~CCriticalSection();

public :
	// 임계영역 시작
	VOID	EnterSection(VOID);
	// 임계영역 끝
	VOID	LeaveSection(VOID);

private :
	// 임계영역 변수
	CRITICAL_SECTION section;
};

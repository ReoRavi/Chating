#include "Headers.h"
#include "CriticalSection.h"

// ������ ���ÿ� �Ӱ迵���� ����ϱ����� �ʱ�ȭ�� ������.
CCriticalSection::CCriticalSection()
{
	::InitializeCriticalSection(&section);
}

// �Ҹ�� ���ÿ� �Ӱ迵���� ����� ����.
CCriticalSection::~CCriticalSection()
{
	::DeleteCriticalSection(&section);
}

//==================================================
// EnterSection
// Info : �Ӱ迵���� ó���� �����Ѵ�.
VOID CCriticalSection::EnterSection(VOID)
{
	::EnterCriticalSection(&section);
}

//==================================================
// LeaveSection
// Info : �Ӱ迵���� ó���� ������.
VOID CCriticalSection::LeaveSection(VOID)
{
	::LeaveCriticalSection(&section);
}
#include "Headers.h"
#include "CriticalSection.h"
#include "AutoCriticalSection.h"

// static ������ ����ϱ� ���� �������ش�.
CCriticalSection CAutoCriticalSection::criticalSection;

// ������ �Ҵ�� ���ÿ� �����ڰ� ����ǰ�, �����ʰ� ���ÿ� �Ҹ��ڰ� �����.
// �Ӱ迵�� ó���� �ʿ��� �κп� ������ �����ؼ� �ڵ����� �Ӱ迵���� ó����.
CAutoCriticalSection::CAutoCriticalSection()
{
	// �Ӱ迵�� ����
	criticalSection.EnterSection();
}

CAutoCriticalSection::~CAutoCriticalSection()
{
	// �Ӱ迵�� ��
	criticalSection.LeaveSection();
}

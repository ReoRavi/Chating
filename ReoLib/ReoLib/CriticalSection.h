#pragma once

//===============================================
// CCriticalSection
// Info : �Ӱ迵�� ó���� ����ϴ� Ŭ����.
class CCriticalSection
{
public:
	CCriticalSection();
	~CCriticalSection();

public :
	// �Ӱ迵�� ����
	VOID	EnterSection(VOID);
	// �Ӱ迵�� ��
	VOID	LeaveSection(VOID);

private :
	// �Ӱ迵�� ����
	CRITICAL_SECTION section;
};

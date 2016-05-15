#pragma once

//===============================================
// CLog
// Info : �α׸� �����ϰ�, ����Ѵ�. ��𼭳� ����� �����ϵ��� static �Լ��� �����.
class CLog
{
public :
	static std::string logFileName;

public :
	// cpp�� �ִ� ���������� �α��� �̸��� �Ҵ����ش�.
	static VOID		CreateLogFile(const char* fileName);

	// �α׸� ����Ѵ�.
	static VOID		WriteLog(const char* log);
	// �α׸� ��¥�� �Բ� ����Ѵ�.
	static VOID		WriteLogWithDate(const char* log);
};


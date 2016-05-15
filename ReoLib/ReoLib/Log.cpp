#include "Headers.h"
#include "Log.h"

// static ��������� ����ϱ� ���� �������ش�.
std::string CLog::logFileName;

//==================================================
// CreateLogFile
// Info : ������ IOCP���� �˸��� ���� �� �ֵ��� �����Ѵ�.
// fileName - ������ ������ �̸�
VOID CLog::CreateLogFile(const char* fileName)
{
	// �α��� �̸��� �����Ѵ�.
	logFileName = fileName;
}

//==================================================
// WriteLog
// Info : �α׸� ����Ѵ�.
// log - ����� �α�
VOID CLog::WriteLog(const char* log)
{
	// �̸��� �Ҵ���� �ʾҴٸ� �α׸� ������� �ʴ´�.
	if (logFileName.empty())
		return;

	FILE*		file;

	// ������ ����. ������ �̹� �ִٸ� �̾ ����ϰ�, ������ ���� �����.
	file = fopen(logFileName.c_str(), "a");

	if (!file)
		return;

	// �α׸� ����Ѵ�.
	fprintf(file, log);

	// ���������͸� �� ����Ͽ� �����Ѵ�.
	fclose(file);
}

//==================================================
// WriteLogWithDate
// Info : �α׸� ����� �ð��� �Բ� ����Ѵ�.
// log - ����� �α�
VOID CLog::WriteLogWithDate(const char* log)
{
	// �̸��� �Ҵ���� �ʾҴٸ� �α׸� ������� �ʴ´�.
	if (logFileName.empty())
		return;

	FILE*		file;
	SYSTEMTIME	systemTime;

	// ���� �ð��� �޾ƿ´�.
	GetLocalTime(&systemTime);

	// ������ ����. ������ �̹� �ִٸ� �̾ ����ϰ�, ������ ���� �����.
	file = fopen(logFileName.c_str(), "a");

	if (!file)
		return;

	char time[60];

	// �ð� ���ڿ�
	sprintf(time, "%d�� %d�� %d�� - %d�� %d�� %d�� : ",
		systemTime.wYear, 
		systemTime.wMonth,
		systemTime.wDay,
		systemTime.wHour,
		systemTime.wMinute,
		systemTime.wSecond);
																					  											

	// �α׸� �ð��� �Բ� ����Ѵ�.
	fprintf(file, time);
	fprintf(file, log);

	// ���������͸� �� ����Ͽ� �����Ѵ�.
	fclose(file);
}

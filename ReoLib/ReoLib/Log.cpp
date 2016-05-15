#include "Headers.h"
#include "Log.h"

// static 멤버변수를 사용하기 위해 선언해준다.
std::string CLog::logFileName;

//==================================================
// CreateLogFile
// Info : 소켓이 IOCP에서 알림을 받을 수 있도록 설정한다.
// fileName - 설정할 파일의 이름
VOID CLog::CreateLogFile(const char* fileName)
{
	// 로그의 이름을 설정한다.
	logFileName = fileName;
}

//==================================================
// WriteLog
// Info : 로그를 기록한다.
// log - 기록할 로그
VOID CLog::WriteLog(const char* log)
{
	// 이름이 할당되지 않았다면 로그를 기록하지 않는다.
	if (logFileName.empty())
		return;

	FILE*		file;

	// 파일을 연다. 파일이 이미 있다면 이어서 기록하고, 없으면 새로 만든다.
	file = fopen(logFileName.c_str(), "a");

	if (!file)
		return;

	// 로그를 기록한다.
	fprintf(file, log);

	// 파일포인터를 다 사용하여 해제한다.
	fclose(file);
}

//==================================================
// WriteLogWithDate
// Info : 로그를 기록한 시간과 함께 기록한다.
// log - 기록할 로그
VOID CLog::WriteLogWithDate(const char* log)
{
	// 이름이 할당되지 않았다면 로그를 기록하지 않는다.
	if (logFileName.empty())
		return;

	FILE*		file;
	SYSTEMTIME	systemTime;

	// 현재 시간을 받아온다.
	GetLocalTime(&systemTime);

	// 파일을 연다. 파일이 이미 있다면 이어서 기록하고, 없으면 새로 만든다.
	file = fopen(logFileName.c_str(), "a");

	if (!file)
		return;

	char time[60];

	// 시간 문자열
	sprintf(time, "%d년 %d월 %d일 - %d시 %d분 %d초 : ",
		systemTime.wYear, 
		systemTime.wMonth,
		systemTime.wDay,
		systemTime.wHour,
		systemTime.wMinute,
		systemTime.wSecond);
																					  											

	// 로그를 시간과 함께 기록한다.
	fprintf(file, time);
	fprintf(file, log);

	// 파일포인터를 다 사용하여 해제한다.
	fclose(file);
}

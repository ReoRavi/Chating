#pragma once

//===============================================
// CLog
// Info : 로그를 생성하고, 기록한다. 어디서나 사용이 가능하도록 static 함수를 사용함.
class CLog
{
public :
	static std::string logFileName;

public :
	// cpp에 있는 전역변수에 로그의 이름을 할당해준다.
	static VOID		CreateLogFile(const char* fileName);

	// 로그를 기록한다.
	static VOID		WriteLog(const char* log);
	// 로그를 날짜와 함께 기록한다.
	static VOID		WriteLogWithDate(const char* log);
};


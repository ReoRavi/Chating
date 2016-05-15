#pragma once

//=================================================
// CScene_Login
// Info : 로그인 씬
class CScene_Login :	public CScene,
										public CLoginUI
{
public:
	CScene_Login();
	virtual ~CScene_Login();

private:
	// [0] - IP, [1] - 유저 이름
	std::string	context[2];
	// 연결요청을 하고 대기중인지.
	BOOL			connectRequest;
	// 연결요청을 하고 대기할 시간.
	FLOAT		requestTime, currentTime;

public :
	// CScene 순수가상함수
	// 초기화
	BOOL	Initialize(VOID);
	// 루프
	VOID	Process(VOID);
	// 메모리 해제
	BOOL	Release(VOID);

private:
	// 전송할 컨텍스트가 이상이 없는지 체크한다.
	BOOL	ContextCheck(VOID);

};


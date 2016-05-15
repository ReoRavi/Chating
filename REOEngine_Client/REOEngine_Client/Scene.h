#pragma once

//=================================================
// CScene
// info : 씬이 구현해야할 함수들을 정의함 (인터페이스화)
class CScene
{
public:
	CScene();
	virtual ~CScene();

public :
	// 초기화
	virtual BOOL	Initialize(VOID) = 0;
	// 루프
	virtual VOID	Process(VOID) = 0;
	// 메모리 해제
	virtual BOOL	Release(VOID) = 0;
};


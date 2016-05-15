#pragma once

//=================================================
// CSceneManager
// Info : 씬들을 관리하는 매니저 객체, Singleton을 상속받아 프로그램에 하나뿐인 인스턴스로 생성된다.
class CSceneManager : public ISingleton<CSceneManager>
{
public:
	CSceneManager();
	virtual ~CSceneManager();

public :	
	// 프로그램이 실행중인가.
	BOOL run;

private :
	// 현재 씬
	CScene*			currentScene;
	// 현재 씬의 타입
	eSceneType	currentSceneType;

public :
	// 초기화
	BOOL		Initialize(VOID);
	// 메모리 해제
	BOOL		Release(VOID);

	// 현재 씬을 얻는다
	CScene*			GetCurrentScene(VOID);
	// 현재 씬의 타입을 얻는다
	eSceneType	GetCurrentSceneType(VOID);
	// 씬을 바꾼다.
	BOOL				ChangeScene(eSceneType sceneType);
};

// 상수로 정의해서 어디서든 접근 가능한 인스턴스로 생성한다.
#define g_pSceneManager CSceneManager::GetInstance()



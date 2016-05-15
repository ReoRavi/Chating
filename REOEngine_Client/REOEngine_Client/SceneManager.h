#pragma once

//=================================================
// CSceneManager
// Info : ������ �����ϴ� �Ŵ��� ��ü, Singleton�� ��ӹ޾� ���α׷��� �ϳ����� �ν��Ͻ��� �����ȴ�.
class CSceneManager : public ISingleton<CSceneManager>
{
public:
	CSceneManager();
	virtual ~CSceneManager();

public :	
	// ���α׷��� �������ΰ�.
	BOOL run;

private :
	// ���� ��
	CScene*			currentScene;
	// ���� ���� Ÿ��
	eSceneType	currentSceneType;

public :
	// �ʱ�ȭ
	BOOL		Initialize(VOID);
	// �޸� ����
	BOOL		Release(VOID);

	// ���� ���� ��´�
	CScene*			GetCurrentScene(VOID);
	// ���� ���� Ÿ���� ��´�
	eSceneType	GetCurrentSceneType(VOID);
	// ���� �ٲ۴�.
	BOOL				ChangeScene(eSceneType sceneType);
};

// ����� �����ؼ� ��𼭵� ���� ������ �ν��Ͻ��� �����Ѵ�.
#define g_pSceneManager CSceneManager::GetInstance()



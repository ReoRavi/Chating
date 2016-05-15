#include "stdafx.h"
#include "SceneManager.h"


CSceneManager::CSceneManager()
{
}


CSceneManager::~CSceneManager()
{
}

//===============================================================
// Initialize
// Info : �ʱ�ȭ
BOOL CSceneManager::Initialize(VOID)
{
	currentScene = nullptr;

	run = TRUE;

	return TRUE;
}

//===============================================================
// Release
// Info : �޸� ����
BOOL CSceneManager::Release(VOID)
{
	delete currentScene;

	return TRUE;
}

//===============================================================
// GetCurrentScene
// Info : ���� ���� �����Ѵ�.
CScene* CSceneManager::GetCurrentScene(VOID)
{
	return currentScene;
}

//===============================================================
// GetCurrentSceneType
// Info : ���� ���� Ÿ���� �����Ѵ�.
eSceneType CSceneManager::GetCurrentSceneType(VOID)
{
	return currentSceneType;
}

//===============================================================
// ChangeScene
// Info : ���� �ٲ۴�.
// sceneType - �ٲ� ���� Ÿ��
BOOL CSceneManager::ChangeScene(eSceneType sceneType)
{
	// ���� �����Ѵٸ�, �޸𸮸� �����Ѵ�.
	if (currentScene)
		currentScene->Release();

	// ���� Ÿ�Կ� �°� �����Ѵ�.
	switch (sceneType)
	{
	case eSceneType::eScene_MainMenu:
		currentScene = new CScene_MainMenu();
		currentSceneType = eSceneType::eScene_MainMenu;

		break;

	case eSceneType::eScene_Info:
		//currentScene = new CScene_MainMenu();
		//currentSceneType = eSceneType::eScene_MainMenu;

		break;

	case eSceneType::eScene_Login:
		currentScene = new CScene_Login();
		currentSceneType = eSceneType::eScene_Login;

		break;

	case eSceneType::eScene_Lobby:
		currentScene = new CScene_Lobby();
		currentSceneType = eSceneType::eScene_Lobby;

		break;

	case eSceneType::eScene_CreateRoom:
		currentScene = new CScene_CreateRoom();
		currentSceneType = eSceneType::eScene_CreateRoom;

		break;

	case eSceneType::eScene_Room :
		currentScene = new CScene_Room();
		currentSceneType = eSceneType::eScene_Room;

		break;

	default:
		break;
	}

	system("cls");

	// ���� �ʱ�ȭ�Ѵ�.
	if (currentScene->Initialize())
		return FALSE;

	return TRUE;
}
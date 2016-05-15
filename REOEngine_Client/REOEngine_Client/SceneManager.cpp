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
// Info : 초기화
BOOL CSceneManager::Initialize(VOID)
{
	currentScene = nullptr;

	run = TRUE;

	return TRUE;
}

//===============================================================
// Release
// Info : 메모리 해제
BOOL CSceneManager::Release(VOID)
{
	delete currentScene;

	return TRUE;
}

//===============================================================
// GetCurrentScene
// Info : 현재 씬을 리턴한다.
CScene* CSceneManager::GetCurrentScene(VOID)
{
	return currentScene;
}

//===============================================================
// GetCurrentSceneType
// Info : 현재 씬의 타입을 리턴한다.
eSceneType CSceneManager::GetCurrentSceneType(VOID)
{
	return currentSceneType;
}

//===============================================================
// ChangeScene
// Info : 씬을 바꾼다.
// sceneType - 바꿀 씬의 타입
BOOL CSceneManager::ChangeScene(eSceneType sceneType)
{
	// 씬이 존재한다면, 메모리를 해제한다.
	if (currentScene)
		currentScene->Release();

	// 씬의 타입에 맞게 생성한다.
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

	// 씬을 초기화한다.
	if (currentScene->Initialize())
		return FALSE;

	return TRUE;
}
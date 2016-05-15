#include "stdafx.h"
#include "Scene_MainMenu.h"

CScene_MainMenu::CScene_MainMenu()
{
}


CScene_MainMenu::~CScene_MainMenu()
{
}

//===============================================================
// Initialize
// Info : 초기화
BOOL CScene_MainMenu::Initialize(VOID)
{
	CMainMenuUI::DrawAllSection();

	sceneType = eSceneType::eScene_MainMenu;

	return TRUE;
}

//===============================================================
// Process
// Info : 루프
VOID CScene_MainMenu::Process(VOID)
{
	// 키보드가 입력되면
	if (kbhit())
	{
		switch (getch())
		{
			// 위쪽 방향키 입력 처리
		case KEY_UP :
			// 커서를 위로 이동시킨다.
			if (--cursorState < 0)
				cursorState = MAIN_UICOUNT - 1;

			CMainMenuUI::DrawCursor();

			break;

			// 아래쪽 방향키 입력 처리
		case KEY_DOWN:
			// 커서를 아래로 이동시킨다.
			if (++cursorState >= MAIN_UICOUNT)
				cursorState = 0;

			CMainMenuUI::DrawCursor();

			break;

			// 엔터키 입력 처리
		case KEY_ENTER :
			// 커서에 맞는 씬으로 이동한다.
			sceneType = eSceneType(cursorState + 1);
			g_pSceneManager->ChangeScene(sceneType);

			break;

		default:
			break;
		}
	}

}

//===============================================================
// Release
// Info : 메모리 해제
BOOL CScene_MainMenu::Release(VOID)
{
	return TRUE;
}

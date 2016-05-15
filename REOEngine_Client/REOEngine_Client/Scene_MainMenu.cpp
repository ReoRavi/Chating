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
// Info : �ʱ�ȭ
BOOL CScene_MainMenu::Initialize(VOID)
{
	CMainMenuUI::DrawAllSection();

	sceneType = eSceneType::eScene_MainMenu;

	return TRUE;
}

//===============================================================
// Process
// Info : ����
VOID CScene_MainMenu::Process(VOID)
{
	// Ű���尡 �ԷµǸ�
	if (kbhit())
	{
		switch (getch())
		{
			// ���� ����Ű �Է� ó��
		case KEY_UP :
			// Ŀ���� ���� �̵���Ų��.
			if (--cursorState < 0)
				cursorState = MAIN_UICOUNT - 1;

			CMainMenuUI::DrawCursor();

			break;

			// �Ʒ��� ����Ű �Է� ó��
		case KEY_DOWN:
			// Ŀ���� �Ʒ��� �̵���Ų��.
			if (++cursorState >= MAIN_UICOUNT)
				cursorState = 0;

			CMainMenuUI::DrawCursor();

			break;

			// ����Ű �Է� ó��
		case KEY_ENTER :
			// Ŀ���� �´� ������ �̵��Ѵ�.
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
// Info : �޸� ����
BOOL CScene_MainMenu::Release(VOID)
{
	return TRUE;
}

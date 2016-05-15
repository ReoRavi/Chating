#include "stdafx.h"
#include "Scene_Login.h"


CScene_Login::CScene_Login()
{
}


CScene_Login::~CScene_Login()
{
}

//===============================================================
// Initialize
// Info : �ʱ�ȭ
BOOL CScene_Login::Initialize(VOID)
{
	CLoginUI::DrawAllSection();

	connectRequest = FALSE;

	requestTime = 0;
	currentTime = 0;
	
	return TRUE;
}

//===============================================================
// Process
// Info : ����
VOID CScene_Login::Process(VOID)
{
	// ���� ��û�� �� ���¶��
	if (connectRequest)
	{
		// ������ ������ ��ٸ�
		if (g_pNetwork->bConnected)
		{
			BYTE buffer[MAX_BUFFER_LENGTH];

			// ���� ������ ������ ����Ѵ�.
			g_pNetwork->WritePacket(PT_WRITE_LOGIN_USERINFO(buffer, context[1]),
				ePACKET_PROTOCOL::ePacket_Login_UserInfo, 
				buffer);

			// �κ�� �̵��Ѵ�.
			g_pSceneManager->ChangeScene(eSceneType::eScene_Lobby);
		}

		currentTime = GetTickCount();

		// 10�ʰ� ������ �����û�� ����ϰ� �ٽ� �����Ѵ�.
		if (currentTime - requestTime > 10000)
		{
			connectRequest = FALSE;
			CLoginUI::DrawAllSection();
			CLoginUI::DrawMessage("���ῡ �����߽��ϴ�. ���� ���¸� üũ�Ͻðų� �ٽ� �Է����ּ���.");
		}
	}
	else
	{
		// IP�� ���� �̸��� �޴´�.
		for (int index = 0; index < CONNECT_UICOUNT; index++)
		{
			gotoxy(UI_XPOS + UIName[index].length() / 2, UI_YPOS + index * 2);
			getline(std::cin, context[index]);
		}

		// ���ؽ�Ʈ�� �̻��� ���ٸ�
		if (ContextCheck())
		{
			// ������ �����û�� �Ѵ�.
			if (g_pNetwork->ConnectToServer(context[0].c_str(), DEFAULT_PORT))
			{
				connectRequest = TRUE;

				CLoginUI::DrawMessage("���� ������Դϴ�..");

				requestTime = GetTickCount();
				currentTime = GetTickCount();
			}
		}
		else
		{
			CLoginUI::DrawAllSection();
			CLoginUI::DrawMessage("���� ���� Ȯ�����ּ���.");
		}
	}
}

//===============================================================
// Release
// Info : �޸� ����
BOOL CScene_Login::Release(VOID)
{
	return TRUE;
}

//===============================================================
// ContextCheck
// Info : ������ ���� ���ؽ�Ʈ�� �̻��� ������ üũ�Ѵ�.
BOOL CScene_Login::ContextCheck(VOID)
{
	// IP�� 16�ڸ��� ���� �� ����
	if (context[0].length() >= 16)
		return FALSE;

	// �̸��� 10�ڸ��� ���� �� ����
	if (context[1].length() >= 10)
		return FALSE;

	return TRUE;
}
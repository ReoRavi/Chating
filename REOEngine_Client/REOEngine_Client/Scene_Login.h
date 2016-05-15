#pragma once

//=================================================
// CScene_Login
// Info : �α��� ��
class CScene_Login :	public CScene,
										public CLoginUI
{
public:
	CScene_Login();
	virtual ~CScene_Login();

private:
	// [0] - IP, [1] - ���� �̸�
	std::string	context[2];
	// �����û�� �ϰ� ���������.
	BOOL			connectRequest;
	// �����û�� �ϰ� ����� �ð�.
	FLOAT		requestTime, currentTime;

public :
	// CScene ���������Լ�
	// �ʱ�ȭ
	BOOL	Initialize(VOID);
	// ����
	VOID	Process(VOID);
	// �޸� ����
	BOOL	Release(VOID);

private:
	// ������ ���ؽ�Ʈ�� �̻��� ������ üũ�Ѵ�.
	BOOL	ContextCheck(VOID);

};


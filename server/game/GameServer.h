#pragma once

#include "Resource.h"		// ������

//////////////////////////////////////////////////////////////////////////////////

//�������
class CGameServerApp : public CWinApp
{
	//��������
public:
	//���캯�� 
	CGameServerApp();

	//���غ���
public:
	//��������
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//�������
extern CGameServerApp theApp;

//////////////////////////////////////////////////////////////////////////////////
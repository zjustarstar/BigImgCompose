
// ImgCompose.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CImgComposeApp: 
// �йش����ʵ�֣������ ImgCompose.cpp
//

class CImgComposeApp : public CWinApp
{
public:
	CImgComposeApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CImgComposeApp theApp;
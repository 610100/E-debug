// E-Debug.h : E-Debug DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

extern char DIRECTORY[MAX_PATH];
// CEDebugApp
// �йش���ʵ�ֵ���Ϣ������� E-Debug.cpp
//

class CEDebugApp : public CWinApp
{
public:
	CEDebugApp();

// ��д
public:
	virtual BOOL InitInstance();
	
	DECLARE_MESSAGE_MAP()
};

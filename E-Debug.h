// E-Debug.h : E-Debug DLL ����ͷ�ļ�
//
#include "stdafx.h"

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

extern char DIRECTORY[MAX_PATH];  //����ȫ�ֱ���,���Ŀ¼

// CEDebugApp
// �йش���ʵ�ֵ���Ϣ������� E-Debug.cpp
//

static HANDLE GethProcess() {           //���������Խ��̵ľ��
	return (HANDLE)*(DWORD*)0x4D5A68;
}

class CEDebugApp : public CWinApp
{
public:
	CEDebugApp();

// ��д
public:
	virtual BOOL InitInstance();
	
	DECLARE_MESSAGE_MAP()
};

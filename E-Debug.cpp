// E-Debug.cpp : ���� DLL �ĳ�ʼ�����̡�
//



/*��������������������������������������������������������
E-debug   DLL��OD�Ľ����ӿ�
��������������������������������������������������������*/




#include "stdafx.h"
#include "E-Debug.h"
#include "MainWindow.h"   //���봰����Ҫ

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HINSTANCE	g_hInstace;


extc int _export cdecl ODBG_Plugindata(char shortname[32])
{
	strcpy(shortname, "E-Debug 3.1");
	return PLUGIN_VERSION;
}

extc int _export cdecl ODBG_Plugininit(int ollydbgversion, HWND hw, DWORD *features)
{
	char tmpbuffer[MAX_PATH] = {};
	GetCurrentDirectoryA(MAX_PATH, tmpbuffer);
	StrCpyA((char*)DIRECTORY, tmpbuffer);
	
	if (ollydbgversion < PLUGIN_VERSION)
	{
		Addtolist(0, 0, "��ʾ: ����汾��OD��ƥ��!");
		return -1;
	}
	Addtolist(0, 0, "%s", "E-Debug 3.1");
	Addtolist(0, -1, "%s","  by:fjqisba");

	//�������޸�һ��OD ����ѡ��BUG
	int* Fix = (int*)34152;
	if (*(DWORD*)4647851 == 99688) {
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)4647851, &Fix, 4, NULL);
	}
	return 0;
}

extc int _export cdecl ODBG_Pluginmenu(int origin, CHAR data[4096], VOID *item)
{
	if (origin == PM_MAIN)
	{
		strcpy(data, "0&�򿪷�������|1&�������");
	}
	return 1;
}

extc void _export cdecl ODBG_Pluginaction(int origin, int action, VOID *item)
{
	if (origin == PM_MAIN) {
		switch (action) {
		case 0:
		{
			if (Getcputhreadid() == 0) {
				AfxMessageBox(L"δ�������������������ת�������������!");
				Flash("δ�������!");
				break;
			}
			CMainWindow *pMainDlg = new CMainWindow;
			pMainDlg->Create(IDD_MainWindow,NULL);
			pMainDlg->ShowWindow(SW_SHOW);
			break;
		}
		case 1:
		{
			CString szInfo;
			szInfo += "Plugin:E-Debug\r\n";
			szInfo += "Verion:3.1\r\n";
			szInfo += "Bug:fjqisba@sohu.com\r\n";
			szInfo += " Thanks to Xjun";
			MessageBox(NULL, szInfo, L"About", MB_ICONINFORMATION);
			break;
		}
		default:
			break;
		}
		

	}
}

BEGIN_MESSAGE_MAP(CEDebugApp, CWinApp)
END_MESSAGE_MAP()


// CEDebugApp ����

CEDebugApp::CEDebugApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CEDebugApp ����

CEDebugApp theApp;


// CEDebugApp ��ʼ��

BOOL CEDebugApp::InitInstance()
{
	CWinApp::InitInstance();
	g_hInstace = AfxGetInstanceHandle();
	return TRUE;
}


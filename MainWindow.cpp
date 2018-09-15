// MainWindow.cpp : ʵ���ļ�
//


/*
	MainWindow,�������潻���Ľӿ�
*/


#include "stdafx.h"
#include "E-Debug.h"
#include "afxdialogex.h"
#include "MainWindow.h"
#include <vector>
#include "EAnalyEngine.h"

extern HINSTANCE g_hInstace;

CMainWindow *pMaindlg;
EAnalysis	*pEAnalysisEngine;

// CMainWindow �Ի���

IMPLEMENT_DYNAMIC(CMainWindow, CDialog)

CMainWindow::CMainWindow(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MainWindow, pParent)
{

}

CMainWindow::~CMainWindow()
{

}

void CMainWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	DDX_Control(pDX, IDC_LOG, m_output);
}


BEGIN_MESSAGE_MAP(CMainWindow, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMainWindow::OnTcnSelchangeTab1)
	ON_LBN_SELCHANGE(IDC_LOG, &CMainWindow::OnLbnSelchangeLog)
END_MESSAGE_MAP()


// CMainWindow ��Ϣ�������


BOOL CMainWindow::OnInitDialog() {
	CDialog::OnInitDialog();
	pMaindlg = this;   //�õ�����ָ��
	
	
	HICON hIcon = LoadIcon(g_hInstace, MAKEINTRESOURCE(IDI_ICON));
	SetIcon(hIcon, FALSE);
	SetIcon(hIcon, TRUE);

	m_Tab.InsertItem(0, _T("֧�ֿ�����"));
	m_Tab.InsertItem(1, _T("API����"));
	m_Tab.InsertItem(2, _T("�������"));


	ULONG uBase, uSize;
	INT nPos;
	Getdisassemblerrange(&uBase, &uSize);
	outputInfo("->��ʼ������ǰ����....  ������ַ: % 08X  �ڴ��С: % 08X", uBase, uSize);


	pEAnalysisEngine = new EAnalysis(uBase,uSize);   //��ʼ���ڴ�

	if (pEAnalysisEngine->EStaticLibInit()) {    //�����Ծ�̬����ʶ��+��ʼ��,ʶ��ʧ�ܷ���false

		std::vector<std::string> krnlCmd =
		{ "����ص�", "DLL����", "����֧�ֿ�����", "����֧�ֿ�����",
			"��ȡ�������", "�����������", "�����ڴ�", "���·����ڴ�",
			"�ͷ��ڴ�", "����", "������Ϣѭ��", "������������", "��ʼ��" };

		nPos = outputInfo("->  �û���д����ο�ʼ : %08X     ", pEAnalysisEngine->dwUsercodeStart);
		m_output.SetItemData(nPos, pEAnalysisEngine->dwUsercodeStart);
		Insertname(pEAnalysisEngine->dwUsercodeStart, NM_COMMENT, "�û�����ο�ʼ");

		if (!pEAnalysisEngine->GetUserEntryPoint()) {
			pEAnalysisEngine->dwUsercodeEnd = uBase + uSize -1;
			nPos = outputInfo("->  �û���д����ν��� : %08X     ", pEAnalysisEngine->dwUsercodeEnd);
		}
		else {
			nPos = outputInfo("->  �û���д����ν��� : %08X     ", pEAnalysisEngine->dwUsercodeEnd);
			m_output.SetItemData(nPos, pEAnalysisEngine->dwUsercodeEnd);
			Insertname(pEAnalysisEngine->dwUsercodeEnd, NM_COMMENT, "�����Գ������");
		}

		DWORD	dwKrnlEntry = pEAnalysisEngine->pEnteyInfo->dwEString;
		if (dwKrnlEntry == 0) {
			dwKrnlEntry = pEAnalysisEngine->pEnteyInfo->dwEWindow;
		}
		DWORD	dwPoint;

		UINT index = pEAnalysisEngine->FindSection(dwKrnlEntry);

		if (index == -1) {           //�����п��ܱ��и�,�����µ�����
			index = pEAnalysisEngine->AddSection(dwKrnlEntry);
		}
	
		for (int i = krnlCmd.size() - 1; i >= 0; i--)
		{
			dwKrnlEntry -= sizeof(DWORD);
			dwPoint = pEAnalysisEngine->GetPoint(pEAnalysisEngine->O2V(dwKrnlEntry,index));
			Insertname(dwPoint, NM_LABEL, (char*)krnlCmd[i].c_str());
		}


		//To do Here,ɨ�����������


		outputInfo("->  ö��������<_krnl_fn_>����֧������ɹ�...");
	}

	else {
		outputInfo("->  �ó����п��ܲ��������Գ���...");
		return true;
	}

	

	CRect rc;
	m_Tab.GetClientRect(&rc);
	rc.top += 26;
	rc.left += 4;
	rc.right -= 6;
	rc.bottom -= 6;

	m_page1.Create(IDD_PAGE1, &m_Tab);
	m_page1.MoveWindow(&rc);

	

    m_page2.Create(IDD_PAGE2, &m_Tab);
	m_page2.MoveWindow(&rc);

	m_page3.Create(IDD_PAGE3, &m_Tab);
	m_page3.MoveWindow(&rc);


	m_page1.ShowWindow(true);

	return true;
}

void CMainWindow::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nCursel = m_Tab.GetCurSel()+1;
	switch (nCursel)
	{
	case 1:
		if (IsWindow(m_page1.m_hWnd)) {
			m_page1.ShowWindow(true);
		}
		if (IsWindow(m_page2.m_hWnd)) {
			m_page2.ShowWindow(false);
		}
		if (IsWindow(m_page3.m_hWnd)) {
			m_page3.ShowWindow(false);
		}
		break;
	case 2:
		if (IsWindow(m_page1.m_hWnd)) {
			m_page1.ShowWindow(false);
		}
		if (IsWindow(m_page2.m_hWnd)) {
			m_page2.ShowWindow(true);
		}
		if (IsWindow(m_page3.m_hWnd)) {
			m_page3.ShowWindow(false);
		}
		break;
	case 3:
		if (IsWindow(m_page1.m_hWnd)) {
			m_page1.ShowWindow(false);
		}
		if (IsWindow(m_page2.m_hWnd)) {
			m_page2.ShowWindow(false);
		}
		if (IsWindow(m_page3.m_hWnd)) {
			m_page3.ShowWindow(true);
		}
		break;
	default:
		break;
	}
	*pResult = 0;
}

int CMainWindow::outputInfo(char *formatText, ...)
{
	USES_CONVERSION;
	char buf[MAX_PATH] = { 0 };
	va_list st;
	va_start(st, formatText);
	vsprintf_s(buf, formatText, st);
	va_end(st);
	return m_output.InsertString(-1, A2W(buf));
}


void CMainWindow::OnLbnSelchangeLog()
{
	int		nCurSel;
	DWORD	dwItemData;
	nCurSel = m_output.GetCurSel();
	dwItemData = m_output.GetItemData(nCurSel);
	if (dwItemData != 0)
	{
		Setcpu(NULL, dwItemData, NULL, NULL, CPU_NOFOCUS);
	}
}

void ScanCode(byte* pSrc,byte* pTrait,int nStrlen,int nTraitlen ) {
	Search_Bin(pSrc,pTrait,nStrlen,nTraitlen);
}


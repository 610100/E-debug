// MainWindow.cpp : ʵ���ļ�
//

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
	pMaindlg = this;
	
	
	HICON hIcon = LoadIcon(g_hInstace, MAKEINTRESOURCE(IDI_ICON));
	SetIcon(hIcon, FALSE);
	SetIcon(hIcon, TRUE);

	m_Tab.InsertItem(0, _T("֧�ֿ�����"));
	m_Tab.InsertItem(1, _T("API����"));
	m_Tab.InsertItem(2, _T("�������"));

	if (Getcputhreadid() == 0) {
		return true;
	}

	ULONG uBase, uSize;
	BOOL bInit;
	INT nPos;
	
	Getdisassemblerrange(&uBase, &uSize);
	
	outputInfo("->��ʼ������ǰ����....  ������ַ: % 08X  �ڴ��С: % 08X", uBase, uSize);

	pEAnalysisEngine = new EAnalysis(uBase, uSize);

	bInit = pEAnalysisEngine->EStaticLibInit();

	if (bInit) {      //��Ծ�̬����������Գ���

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


		if ( dwKrnlEntry > (pEAnalysisEngine->rdata_dwBase+pEAnalysisEngine->rdata_dwSize)) {     //���ּӿǳ���,���α��и�
			ULONG tempSection=(DWORD)LocalAlloc(LMEM_ZEROINIT, krnlCmd.size()*4);
			Readmemory((DWORD*)tempSection,dwKrnlEntry - krnlCmd.size() * 4, krnlCmd.size() * 4, MM_RESILENT);
			for (int i = 0; i < krnlCmd.size(); i++) {
				dwPoint = pEAnalysisEngine->GetPoint((DWORD)tempSection);
				Insertname(dwPoint, NM_LABEL, (char*)krnlCmd[i].c_str());
				tempSection +=sizeof(DWORD);
			}
			LocalFree((HLOCAL)tempSection);
		}
		else {
			for (int i = krnlCmd.size() - 1; i >= 0; i--)
			{
				dwKrnlEntry -= sizeof(DWORD);
				dwPoint = pEAnalysisEngine->GetPoint(pEAnalysisEngine->R_O2V(dwKrnlEntry));
				Insertname(dwPoint, NM_LABEL, (char*)krnlCmd[i].c_str());
			}

		}





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
	int nCursel = m_Tab.GetCurSel();
	switch (nCursel)
	{
	case 0:
		m_page1.ShowWindow(true);
		m_page2.ShowWindow(false);
		m_page3.ShowWindow(false);
		break;
	case 1:
		m_page1.ShowWindow(false);
		m_page2.ShowWindow(true);
		m_page3.ShowWindow(false);
		break;
	case 2:
		m_page1.ShowWindow(false);
		m_page2.ShowWindow(false);
		m_page3.ShowWindow(true);
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

DWORD Search_Bin(byte *pSrc, byte *pTrait, int nSrcLen, int nTraitLen)
{
	if (IsBadReadPtr(pSrc, 4) == TRUE)
	{
		return 0;
	}
	int i, j, k;
	for (i = 0; i <= (nSrcLen - nTraitLen); i++)
	{
		if (pSrc[i] == pTrait[0])
		{
			k = i;
			j = 0;
			while (j < nTraitLen)
			{
				k++; j++;
				if (pTrait[j] == 0x90)
				{
					continue;
				}
				if (pSrc[k] != pTrait[j])
				{
					break;
				}
			}

			if (j == nTraitLen)
			{
				return i;
			}

		}

	}
	return 0;
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
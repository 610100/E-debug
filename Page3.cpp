// Page3.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "E-Debug.h"
#include "Page3.h"
#include "afxdialogex.h"
#include "MainWindow.h"
#include "EAnalyEngine.h"

extern 	CMainWindow *pMaindlg;
extern  EAnalysis	*pEAnalysisEngine;

// CPage3 �Ի���

IMPLEMENT_DYNAMIC(CPage3, CDialog)

HTREEITEM hroot;

CPage3::CPage3(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PAGE3, pParent)
{

}

CPage3::~CPage3()
{
}

void CPage3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
}


BEGIN_MESSAGE_MAP(CPage3, CDialog)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE1, &CPage3::OnTvnItemexpandingTree1)
END_MESSAGE_MAP()


// CPage3 ��Ϣ�������

BOOL CPage3::OnInitDialog() {
	CDialog::OnInitDialog();


	m_Tree.SetLineColor(RGB(0, 0, 255));

	hroot=m_Tree.InsertItem(_T("�����Դ���"));

	//��������������������������������
	WindowInfo m_WindowInfo;
	DWORD EipAddr = pEAnalysisEngine->pEnteyInfo->dwEWindow; //Eip��ַΪ��ǰ������ַ
	if (EipAddr == 0) {
		return TRUE;
	}

	UINT r_index = pEAnalysisEngine->FindSection(EipAddr);
	if (r_index == -1) {
		r_index = pEAnalysisEngine->AddSection(EipAddr);
	}

	m_WindowInfo.WindowCount =*(unsigned long*)pEAnalysisEngine->O2V(EipAddr,r_index)>>3;
	if (m_WindowInfo.WindowCount == 0) {
		return TRUE;
	}


	CString WinID;
	CString ControlID;

	
	for (int i = 0;i < m_WindowInfo.WindowCount;i++) {
		EipAddr = EipAddr + 4;
		WinID.Format(L"0x%X", *(DWORD*)(pEAnalysisEngine->O2V(EipAddr,r_index)));
		m_WindowInfo.WindowId.push_back(m_Tree.InsertItem(WinID,hroot));
	}

	EipAddr = EipAddr + 4 * (m_WindowInfo.WindowCount+1); //����WindowAddr

	WindowPropery m_WindowPropery;

	for (int i = 0;i < m_WindowInfo.WindowCount;i++) {   //�������ν���
		EipAddr = EipAddr + 16;
		m_WindowPropery.ControlCount = *(unsigned long*)pEAnalysisEngine->O2V(EipAddr,r_index);
		if (m_WindowPropery.ControlCount == 0) {
			return true;
		}
		EipAddr = EipAddr + 4;
		m_WindowPropery.ControlSize= *(unsigned long*)pEAnalysisEngine->O2V(EipAddr,r_index);
		for (int j = 0;j < m_WindowPropery.ControlCount;j++) {
			EipAddr = EipAddr + 4;
			m_WindowPropery.ControlID.push_back(*(unsigned long*)pEAnalysisEngine->O2V(EipAddr,r_index));
			ControlID.Format(L"0x%X",m_WindowPropery.ControlID[j]);
			m_Tree.InsertItem(ControlID, m_WindowInfo.WindowId[i]);
		}
		EipAddr = 4 + (EipAddr - m_WindowPropery.ControlCount * 4) + m_WindowPropery.ControlSize;
	}


	

	for (int i = 0;i < m_WindowInfo.WindowCount;i++) {
		m_Tree.Expand(m_WindowInfo.WindowId[i],TVE_EXPAND);
	}
	m_Tree.Expand(hroot,TVE_EXPAND);
	
	return TRUE;
}



void CPage3::OnTvnItemexpandingTree1(NMHDR *pNMHDR, LRESULT *pResult)   //��������Դ��ڵ�ʱ��,��������
{
	USES_CONVERSION;
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	if (pNMTreeView->itemNew.hItem == hroot && pNMTreeView->action==TVE_COLLAPSE) {
		*pResult = TRUE;
	}
	else {
		*pResult = 0;
	}


}

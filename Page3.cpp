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
}


BEGIN_MESSAGE_MAP(CPage3, CDialog)
END_MESSAGE_MAP()


// CPage3 ��Ϣ�������

BOOL CPage3::OnInitDialog() {
	CDialog::OnInitDialog();

	WindowInfo m_WindowInfo;
	DWORD EipAddr = pEAnalysisEngine->pEnteyInfo->dwEWindow; //��ǰ�������ݽṹ�ĵ�ַ,��ʵ��ַ
	m_WindowInfo.WindowCount =*(unsigned long*)pEAnalysisEngine->R_O2V(EipAddr)>>3;
	if (m_WindowInfo.WindowCount == 0) {
		return TRUE;
	}

	for (int i = 0;i < m_WindowInfo.WindowCount;i++) {
		EipAddr = EipAddr + 4;
		m_WindowInfo.WindowId.push_back(*(DWORD*)(pEAnalysisEngine->R_O2V(EipAddr)));
		pMaindlg->outputInfo("%X", m_WindowInfo.WindowId[i]);
	}
	for (int i = 0;i < m_WindowInfo.WindowCount;i++) {
		EipAddr = EipAddr + 4;
		//m_WindowInfo.WindowAddr.push_back(*(DWORD*)(pEAnalysisEngine->R_O2V(EipAddr))); //û��ʲô�õ�����
	}

	EipAddr = EipAddr + 20;
	m_WindowInfo.ControlCount = *(unsigned long*)pEAnalysisEngine->R_O2V(EipAddr);
	if (m_WindowInfo.ControlCount == 0) {      //��ʵ��Ĭ���Դ�һ���ؼ�ID��
		return true;  
	}

	EipAddr = EipAddr + 4;
	m_WindowInfo.ControlSize = *(unsigned long*)pEAnalysisEngine->R_O2V(EipAddr);

	for (int i = 0;i < m_WindowInfo.ControlCount;i++) {
		EipAddr = EipAddr + 4;
		m_WindowInfo.ControlID.push_back(*(DWORD*)(pEAnalysisEngine->R_O2V(EipAddr)));
		pMaindlg->outputInfo("%X", m_WindowInfo.ControlID[i]);
	}

	EipAddr = EipAddr + 4; 

	for (int i = 0;i < m_WindowInfo.ControlCount;i++) {
		EipAddr = EipAddr + 4;
		m_WindowInfo.Controloffset.push_back(*(DWORD*)(pEAnalysisEngine->R_O2V(EipAddr)));
	}

	return TRUE;
}


//���ڸ���
//����ID
//��Ӧ�ڴ��ַ
//Unknow1
//Unknow2
//0000000
//0000000
//�ؼ�����
//�ؼ���С?
//�ؼ�ID
//00000000
//��Ӧ�ؼ�ƫ��

//��һ���ؼ��ռ��С
//01 00 01 00
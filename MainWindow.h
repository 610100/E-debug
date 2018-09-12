#pragma once
#include "afxcmn.h"
#include "Page1.h"
#include "Page2.h"
#include "Page3.h"
#include "afxwin.h"

// CMainWindow �Ի���

class CMainWindow : public CDialog
{
	DECLARE_DYNAMIC(CMainWindow)

public:
	CMainWindow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainWindow();
	int outputInfo(char *formatText, ...);
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MainWindow };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL Initsucceed;  //Ϊ�����˳�����
	virtual BOOL OnInitDialog();
	CTabCtrl m_Tab;
	CPage1 m_page1;
	CPage2 m_page2;
	CPage3 m_page3;

	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CListBox m_output;
	afx_msg void OnLbnSelchangeLog();
};

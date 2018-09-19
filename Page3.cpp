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

	
	m_ICO.Create(16,16,ILC_COLOR32,255,255);
	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_UnKnow));  //Ĭ��ͼ��

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_Window));  //����  1

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_EDIT));   //�༭�� 2

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_Pic));    //ͼƬ�� 3 

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_ShapeBox));    //���ο� 4

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_DrawPanel));    //���� 5

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_GroupBox));    //����� 6

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_Label));   //��ǩ 7

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_BUTTON));  //��ť 8

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_CheckBox));  //ѡ��� 9

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_RadioBox));  //��ѡ��

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_ComboBox));  //��Ͽ�

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_ListBox));  //�б��
	
	m_Tree.SetImageList(&m_ICO,TVSIL_NORMAL);

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

	WindowPropery m_WindowPropery = {};
	vector<WindowPropery> Window;
	//��������������������������������������������������������������������

	

	DWORD NextWindowProery = EipAddr + 24; //������ʼ��ַ


	for (int i = 0;i < m_WindowInfo.WindowCount;i++) {      //��������
		Window.push_back(m_WindowPropery);
		EipAddr = EipAddr + 16;

		Window[i].ControlCount = *(unsigned long*)pEAnalysisEngine->O2V(EipAddr,r_index);  //�õ��ؼ�����
		
		if (Window[i].ControlCount == 0) {
			return true;
		}
		
		EipAddr = EipAddr + 4;
		Window[i].ControlSize= *(unsigned long*)pEAnalysisEngine->O2V(EipAddr,r_index);   //�õ��ؼ��ܴ�С
		
		
		for (int j = 0;j < Window[i].ControlCount;j++) {
			EipAddr = EipAddr + 4;
			ControlID.Format(L"0x%X", *(unsigned long*)pEAnalysisEngine->O2V(EipAddr, r_index));
			Window[i].ControlID.push_back(m_Tree.InsertItem(ControlID,m_WindowInfo.WindowId[i])); //����ؼ�ID
		}
		
		for (int t = 0;t < Window[i].ControlCount;t++) {          //�����ؼ�
			EipAddr = EipAddr + 4;
			Window[i].Controloffset.push_back(*(unsigned long*)pEAnalysisEngine->O2V(EipAddr, r_index));
			
		}



		DWORD ControlStartAddr = EipAddr + 8;  //��ʼ���ؼ���ַ

		for (int t = 0;t < Window[i].ControlCount;t++) {

			EipAddr = ControlStartAddr+ Window[i].Controloffset[t];
			DWORD TYPEID= *(unsigned long*)pEAnalysisEngine->O2V(EipAddr, r_index);
			
			switch (TYPEID)
			{
			case 0x10001:  //����
				m_Tree.SetItemImage(Window[i].ControlID[t], 1, 1);
				break;
			case 0x10005: //�༭��
				m_Tree.SetItemImage(Window[i].ControlID[t], 2, 2);
				break;
			case 0x10006: //ͼƬ��
				m_Tree.SetItemImage(Window[i].ControlID[t], 3, 3);
				break;
			case 0x10007: //���ο�
				m_Tree.SetItemImage(Window[i].ControlID[t], 4, 4);
				break;
			case 0x10008: //����
				m_Tree.SetItemImage(Window[i].ControlID[t], 5, 5);
				break;
			case 0x10009: //�����
				m_Tree.SetItemImage(Window[i].ControlID[t], 6, 6);
				break;
			case 0x1000A: //��ǩ
				m_Tree.SetItemImage(Window[i].ControlID[t], 7, 7);
				break;
			case 0x1000B: //��ť
				m_Tree.SetItemImage(Window[i].ControlID[t], 8, 8);
				break;
			case 0x1000C: //ѡ���
				m_Tree.SetItemImage(Window[i].ControlID[t], 9, 9);
				break;
			default:
				pMaindlg->outputInfo("%X", TYPEID);
				break;
			}
		}

		NextWindowProery = NextWindowProery + Window[i].ControlSize +24;
		EipAddr = NextWindowProery -24 ;
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

#pragma once
#include "afxcmn.h"
#include "stdafx.h"

#include <string>
#include <map>

using namespace std;

typedef struct LIBMAP
{
	vector<string>  Command_name;
	vector<DWORD>  Command_addr;
}*LibMap;


#define MAX_ESIZE 256
typedef struct
{
	char	m_CommandName[64];				//��������
	int  	m_CallType;					    //��������,0�������ظ�CALL,1������Ҫ�жϵڶ���CALL��2������Ҫ�ж�IAT������CALL
											//3�����ж�CALL֮ǰ��һ���������
	ULONG   m_CallOffset;			        //��¼��Ҫ�жϵ�call��ƫ��
	int		m_size;							//����һ�׺������ֽڴ�С
	UCHAR   m_opcode[MAX_ESIZE];	        //ƥ����ֽ�
	int     m_size2;                        //������׺������ֽڴ�С
	UCHAR	m_opcode2[MAX_ESIZE];	        //����1��3ʱ��opcode
	char    m_IATEAT[128];                   //callΪ����2ʱΪIAT��EAT
}ESTATICLIBOPCODE, *PESTATICLIBOPCODE;

// CPage1 �Ի���

class CPage1 : public CDialog
{
	DECLARE_DYNAMIC(CPage1)

public:
	CPage1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPage1();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	PESTATICLIBOPCODE ReadMap(const char *lpMapPath, int *nCount);
	bool MatchCode(unsigned char* pSrc1, unsigned char* pSrc2, int nLen);
	bool MatchCode_UnEx(unsigned char* pSrc1, unsigned char* pSrc2, int nLen);//
	CListCtrl m_lib;
	CListCtrl m_command;
	map<int, LIBMAP> m_map;
	afx_msg void OnNMClickListlib(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickListcommand(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnNMDblclkListcommand(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListcommand(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On32771();
};

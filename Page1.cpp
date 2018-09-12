// Page1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "E-Debug.h"
#include "afxdialogex.h"
#include "Page1.h"
#include "EAnalyEngine.h"
#include "Elib.h"
#include "MainWindow.h"
#include <vector>

// CPage1 �Ի���
extern  EAnalysis	*pEAnalysisEngine;
extern 	CMainWindow *pMaindlg;

char DIRECTORY[MAX_PATH];
static int addrtype;
static int nametype;

IMPLEMENT_DYNAMIC(CPage1, CDialog)

CPage1::CPage1(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PAGE1, pParent)
{
	
}

CPage1::~CPage1()
{
	m_map.clear();
}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTLib, m_lib);
	DDX_Control(pDX, IDC_LISTCommand, m_command);
}


BEGIN_MESSAGE_MAP(CPage1, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LISTLib, &CPage1::OnNMClickListlib)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LISTCommand, &CPage1::OnLvnColumnclickListcommand)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCommand, &CPage1::OnNMDblclkListcommand)
	ON_NOTIFY(NM_RCLICK, IDC_LISTCommand, &CPage1::OnNMRClickListcommand)
	ON_COMMAND(ID_32771, &CPage1::On32771)
END_MESSAGE_MAP()


// CPage1 ��Ϣ�������
BOOL CPage1::OnInitDialog() {
	CDialog::OnInitDialog();
	
	LONG lStyle;

	lStyle = GetWindowLong(m_lib.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	lStyle |= LVS_REPORT; //����style
	SetWindowLong(m_lib.m_hWnd, GWL_STYLE, lStyle);//����style

	lStyle = GetWindowLong(m_command.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	lStyle |= LVS_REPORT; //����style
	SetWindowLong(m_command.m_hWnd, GWL_STYLE, lStyle);//����style

	DWORD dwStyle = m_lib.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
									//dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	m_lib.SetExtendedStyle(dwStyle); //������չ���

	dwStyle = m_command.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	m_command.SetExtendedStyle(dwStyle); //������չ���


	m_lib.InsertColumn(0, _T("֧�ֿ���Ϣ"), LVCFMT_LEFT, 250);

	m_command.InsertColumn(0, L"��ַ", LVCFMT_LEFT, 65);
	m_command.InsertColumn(1, L"��������", LVCFMT_CENTER, 205);

	int			nPos = 0;
	DWORD		pFirst = pEAnalysisEngine->pEnteyInfo->pLibEntey;

	PLIB_INFO	pLibInfo = NULL;
	CString		strLib, strGuid;
	CString		str;

	for (int i = 0; i < pEAnalysisEngine->pEnteyInfo->dwLibNum; i++)  //���ڽ���������ÿ��֧�ֿ�
	{
		pLibInfo = (PLIB_INFO)pEAnalysisEngine->R_O2V(pEAnalysisEngine->R_GetOriginPoint(pFirst));
		strLib.Format(L"---->%s (Ver:%1d.%1d)",
			(CString)(char*)pEAnalysisEngine->R_O2V((DWORD)pLibInfo->m_szName),
			pLibInfo->m_nMajorVersion,
			pLibInfo->m_nMinorVersion);
		strGuid.Format(L"        %s", (CString)(char*)pEAnalysisEngine->R_O2V((DWORD)pLibInfo->m_szGuid));

		m_lib.InsertItem(nPos, strLib); nPos++;   //��ʾLib����(Ver:�汾��)
		m_lib.InsertItem(nPos, strGuid); nPos++;  //��ʾLib��GUID

		str.Empty();
		str.Format(L"   -> �������� (����:%d)", pLibInfo->m_nCmdCount); //��ʾ������������

		int	nCountCmd = 0;    // MAP�е��������
		DWORD		pFunc = pEAnalysisEngine->R_O2V((DWORD)pLibInfo->m_pCmdsFunc);
		DWORD		dwAddress;

		char szLibVer[12] = { 0 };
		wsprintfA(szLibVer, "\\%1d.%1d", pLibInfo->m_nMajorVersion, pLibInfo->m_nMinorVersion);

		char szDirectory[MAX_PATH] = {};
		StrCpyA(szDirectory, DIRECTORY);
		
		strcat_s(szDirectory, "\\Plugin\\EMap\\");strcat_s(szDirectory, (char*)pEAnalysisEngine->R_O2V((DWORD)pLibInfo->m_szGuid));
		strcat_s(szDirectory, szLibVer);strcat_s(szDirectory, ".Emap");
		
		PESTATICLIBOPCODE pMap = ReadMap(szDirectory, &nCountCmd); //����MAP�е��������
		LIBMAP m_Libmap;

		m_Libmap.Command_addr.clear();
		m_Libmap.Command_name.clear();
		if (pMap == NULL) {    //�����ȡ����MAP�ļ�
			for (int n = 0;n < pLibInfo->m_nCmdCount;n++) {
				dwAddress = pEAnalysisEngine->GetPoint(pFunc);
				m_Libmap.Command_addr.push_back(dwAddress);
				m_Libmap.Command_name.push_back("Emap Not Founded");
				pFunc += sizeof(int);
			}
		}
		else {
			for (int n = 0;n < pLibInfo->m_nCmdCount;n++) {     //���ڳ����е�ÿ������,����һ�ξ�ȷƥ��
				dwAddress = pEAnalysisEngine->GetPoint(pFunc);
				m_Libmap.Command_addr.push_back(dwAddress);
				PESTATICLIBOPCODE pTmpMap = pMap;
				BOOL bMatchCom = false;
				for (int i = 0;i < nCountCmd;i++) {    //��ȷƥ�� 
					if (MatchCode((unsigned char*)pEAnalysisEngine->T_O2V(dwAddress), (unsigned char*)pTmpMap->m_opcode, pTmpMap->m_size))
					{
						if (pTmpMap->m_CallType == 0) {
							pTmpMap->m_CallType = -1;//��������
							bMatchCom = true;
							break;
						}
						else if (pTmpMap->m_CallType == 1) {
							unsigned long SecondFuncAddr = *(long*)(pEAnalysisEngine->T_O2V(dwAddress) + pTmpMap->m_CallOffset + 1) + pEAnalysisEngine->T_O2V(dwAddress) + pTmpMap->m_CallOffset + 5;
							if (MatchCode((unsigned char*)SecondFuncAddr, (unsigned char*)pTmpMap->m_opcode2, pTmpMap->m_size2)) {
								pTmpMap->m_CallType = -1;
								bMatchCom = true;
								break;
							}
						}
						else if (pTmpMap->m_CallType == 2) {
							char IAT[256] = { 0 };
							unsigned long IATFuncAddr = *(long*)(pEAnalysisEngine->T_O2V(dwAddress) + pTmpMap->m_CallOffset + 2);
							char Func[MAX_ESIZE];
							strcpy(Func, pTmpMap->m_IATEAT);
							char *IATFunc = strtok(Func, "||");
							char *EATFunc = strtok(NULL, "||");
							if (Findname(IATFuncAddr, NM_IMPORT, IAT) != 0 && strcmp(IAT, IATFunc) == 0) {    //IAT̽Ѱ����
								pTmpMap->m_CallType = -1;
								bMatchCom = true;
								break;
							}
							else if (Findname(*(long*)pEAnalysisEngine->R_O2V(IATFuncAddr), NM_EXPORT, IAT) != 0 && strcmp(IAT, EATFunc) == 0) { //EAT̽Ѱ����	
								pTmpMap->m_CallType = -1;
								bMatchCom = true;
								break;
							}
						}
						else if (pTmpMap->m_CallType == 3) {
							unsigned long JmpAddr = *(long*)(pEAnalysisEngine->T_O2V(dwAddress + pTmpMap->m_size + 1)) + pEAnalysisEngine->T_O2V(dwAddress) + pTmpMap->m_size + 5;
							if (MatchCode((unsigned char*)JmpAddr, (unsigned char*)pTmpMap->m_opcode2, pTmpMap->m_size2)) {
								pTmpMap->m_CallType = -1;
								bMatchCom = true;
								break;
							}
						}
					}
					pTmpMap++;
				}
			if (!bMatchCom)   //����ƥ��ʧ�ܵ�����,�������ƶ�ƥ��
			{
				m_Libmap.Command_name.push_back("Error");
				Insertname(dwAddress, NM_LABEL, "δ֪����");
			}
			else
			{
				m_Libmap.Command_name.push_back(pTmpMap->m_CommandName);
				Insertname(dwAddress, NM_LABEL, pTmpMap->m_CommandName);
			}
			pFunc += sizeof(int);
		}
	}

		USES_CONVERSION;
		pFunc = pEAnalysisEngine->R_O2V((DWORD)pLibInfo->m_pCmdsFunc);  //��ȷƥ�������,�ٽ���һ��ģ��ƥ��
		for (int m = 0;m < m_Libmap.Command_name.size();m++) {
			dwAddress = pEAnalysisEngine->GetPoint(pFunc);
			PESTATICLIBOPCODE pTmpMap = pMap;
			if (m_Libmap.Command_name[m] == "Error") {      //ƥ��ʧ��
				for (int n = 0;n < nCountCmd;n++) {
					if (MatchCode_UnEx((unsigned char*)pEAnalysisEngine->T_O2V(dwAddress), (unsigned char*)pTmpMap->m_opcode, pTmpMap->m_size)) {
						if (pTmpMap->m_CallType == 0) {
							pTmpMap->m_CallType = -1;
							strcat(pTmpMap->m_CommandName, "(ģ��)");
							m_Libmap.Command_name[m] = pTmpMap->m_CommandName;
							Insertname(dwAddress, NM_LABEL, pTmpMap->m_CommandName);
							break;
						}
						else if (pTmpMap->m_CallType == 1 || pTmpMap->m_CallType==2) {
							unsigned long SecondFuncAddr = *(long*)(pEAnalysisEngine->T_O2V(dwAddress) + pTmpMap->m_CallOffset + 1) + pEAnalysisEngine->T_O2V(dwAddress) + pTmpMap->m_CallOffset + 5;
							if (MatchCode_UnEx((unsigned char*)SecondFuncAddr, (unsigned char*)pTmpMap->m_opcode2, pTmpMap->m_size2)) {
								pTmpMap->m_CallType = -1;
								strcat(pTmpMap->m_CommandName, "(ģ��)");
								m_Libmap.Command_name[m] = pTmpMap->m_CommandName;								
								Insertname(dwAddress, NM_LABEL, pTmpMap->m_CommandName);
								break;
							}
						}
					}
					pTmpMap++;
				}
			}
			pFunc += sizeof(int);
		}

		if (pMap) {
			free(pMap);
		}
		
		m_map.insert(map<int,LIBMAP>::value_type(nPos,m_Libmap));
		m_lib.InsertItem(nPos, str);nPos++;
		m_lib.InsertItem(nPos, L"============================="); nPos++;
		pFirst += sizeof(DWORD);
	}

	pMaindlg->outputInfo("->  ����������<KrnlLibCmd>&&<LibCmd>���...");
	return true;
}


bool CPage1::MatchCode(unsigned char* pSrc1, unsigned char* pSrc2, int nLen)
{
	if (nLen == 0)
	{
		return FALSE;
	}
	for (int i = 0; i < nLen; i++)
	{
		if (pSrc2[i] == 0x90)//ģ��ƥ��
			continue;
		if (pSrc1[i] != pSrc2[i])
			return FALSE;
	}
	return TRUE;
}

bool CPage1::MatchCode_UnEx(unsigned char* pSrc1, unsigned char* pSrc2, int nLen)
{
	float Count = 0;
	if (nLen == 0)
	{
		return FALSE;
	}
	for (int i = 0; i < nLen; i++)
	{
		if (pSrc2[i] == 0x90)//ģ��ƥ��
		{
			Count++;
			continue;
		}
		if (pSrc1[i] != pSrc2[i])
		{
			continue;
		}
		Count++;
	}
	if (Count / (float)nLen > 0.75f) {   
		return TRUE;
	}
	else {
		return false;
	}
}

PESTATICLIBOPCODE CPage1::ReadMap(const char *lpMapPath, int *nCount)
{
	HANDLE hFile = CreateFileA(lpMapPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		*nCount = 0;
		return NULL;
	}
	DWORD	dwHitSize = 0;
	DWORD	dwSize = GetFileSize(hFile, &dwHitSize);
	DWORD	dwReadSize;
	*nCount = dwSize / sizeof(ESTATICLIBOPCODE);//�õ��������
	PESTATICLIBOPCODE pMap = (PESTATICLIBOPCODE)calloc(dwSize, sizeof(char));
	ReadFile(hFile, pMap, dwSize, &dwReadSize, NULL);
	CloseHandle(hFile);
	return pMap;
}

void CPage1::OnNMClickListlib(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	USES_CONVERSION;
	CString		strAddress;
	int	nPos= m_lib.GetSelectionMark();
	int nCmdCount = m_map[nPos].Command_addr.size();

	if (nCmdCount) {
		m_command.DeleteAllItems();
		for (int n = 0;n < nCmdCount;n++) {
			strAddress.Format(L"%08X", m_map[nPos].Command_addr[n]);
			m_command.InsertItem(n, strAddress);
			m_command.SetItemData(n, m_map[nPos].Command_addr[n]);
			m_command.SetItemText(n, 1,A2W(m_map[nPos].Command_name[n].c_str()));
		}
	}

	addrtype = 0;
	nametype = 0;
	*pResult = 0;
}

static int CALLBACK CompareAddr(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
	if (lParamSort == 0) {
		return lParam1 > lParam2;
	}
	else if (lParamSort == 1) {
		return lParam2 > lParam1;
	}
	return 0;
}

static int CALLBACK CompareName(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
	if (lParamSort == 0) {          //�����ı�ASCII�Ƚ�
		UCHAR x1 = *(UCHAR*)pMaindlg->m_page1.m_command.GetItemText(static_cast<int>(lParam1), 1).GetBuffer();
		UCHAR x2 = *(UCHAR*)pMaindlg->m_page1.m_command.GetItemText(static_cast<int>(lParam2), 1).GetBuffer();
		return x1 > x2;
	}
	else if (lParamSort == 1) {     //�����ı����ȱȽ�
		return pMaindlg->m_page1.m_command.GetItemText(static_cast<int>(lParam1), 1).GetLength() > pMaindlg->m_page1.m_command.GetItemText(static_cast<int>(lParam2), 1).GetLength();
	}
	return 0;
}

void CPage1::OnLvnColumnclickListcommand(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->iSubItem == 0) {
		m_command.SortItems(CompareAddr, (DWORD_PTR)addrtype);
		if (addrtype == 0) {  //��������
			addrtype = 1;
		}
		else {                //��������
			addrtype = 0;
		}
	}
	else if (pNMLV->iSubItem == 1) {
		m_command.SortItemsEx(CompareName, (DWORD_PTR)nametype);
		if (nametype == 0) {       //����ASCII����
			nametype = 1;
		}
		else {                    //���ճ�������
			nametype = 0;
		}
	}
	*pResult = 0;
}



void CPage1::OnNMDblclkListcommand(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	if (m_command.GetSelectedCount() <= 0) {    //˵��δѡȡ������
		return;
	}
	int nPos = m_command.GetSelectionMark();
	if (nPos == -1) {
		return;
	}
	int dwData = m_command.GetItemData(nPos);
	if (!dwData)
		return;

	Setcpu(0, dwData, 0, 0, CPU_NOFOCUS);

	*pResult = 0;
}


void CPage1::OnNMRClickListcommand(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	CMenu Menu;
	CMenu *pSubMenu;
	if (pNMItemActivate->iItem != -1) {
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		Menu.LoadMenu(IDR_MENU1);
		pSubMenu = Menu.GetSubMenu(0);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	*pResult = 0;
}

void CPage1::On32771()   //�������ð�ť
{
	USES_CONVERSION;
	int nPos = m_command.GetSelectionMark();
	if (nPos == -1) {
		return;
	}
	DWORD dwData = m_command.GetItemData(nPos);
	if (!dwData) {
		return;
	}

	CString	strCom= m_command.GetItemText(nPos, 1);

	pMaindlg->m_output.ResetContent();
	pMaindlg->outputInfo("-> ִ������   --==��������==--");
	byte ComCall[5] = { 0xBB, 0, 0, 0, 0 };
	memcpy(&ComCall[1], &dwData, sizeof(DWORD));
	byte *pTmp = (byte*)pEAnalysisEngine->T_O2V(pEAnalysisEngine->dwUsercodeStart);

	DWORD	dwSecSize = pEAnalysisEngine->dwUsercodeEnd - pEAnalysisEngine->dwUsercodeStart;
	DWORD	dwResult = pEAnalysisEngine->dwUsercodeStart;    //��Ѱ�����ַ
	DWORD   dwCount = 0;   //�������ֽ���
	while (true) {
		DWORD offset = pEAnalysisEngine->Search_BinEx(pTmp, ComCall, dwSecSize, sizeof(ComCall));//�õ�ƫ�Ƶ�ַ
		if (offset == 0)
			break;
		dwResult += offset;
		int index= pMaindlg->outputInfo("%08X    mov ebx,%08X    //%s", dwResult, dwData, W2A(strCom)); //��ʾ�������ַ
		pMaindlg->m_output.SetItemData(index, dwResult);
		dwResult += sizeof(ComCall);
		pTmp += offset+sizeof(ComCall);		
		dwSecSize -= offset + sizeof(ComCall);
	}
}

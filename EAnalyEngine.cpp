#include "stdafx.h"
#include "E-Debug.h"
#include "MainWindow.h"
#include "EAnalyEngine.h"

extern CMainWindow *pMaindlg;

UINT EAnalysis::FindSection(DWORD addr) {
	for (int i = 0;i < SectionMap.size();i++) {
		if (addr >= SectionMap[i].dwBase && addr < (SectionMap[i].dwBase + SectionMap[i].dwSize)) {
			return i;
		}
	}
	return -1;
}

UINT EAnalysis::AddSection(DWORD addr) {
	_MEMORY_BASIC_INFORMATION MB;
	sectionAlloc addsection;

	if (!VirtualQueryEx(GethProcess(), (LPCVOID*)addr, &MB, sizeof(MEMORY_BASIC_INFORMATION))) {
		pMaindlg->outputInfo("��ѯ�ڴ�ʧ��!");
		return FALSE;
	}
	
	addsection.dwBase = (ULONG)MB.BaseAddress;
	addsection.dwSize = MB.RegionSize;

	addsection.SectionAddr = (BYTE *)VirtualAlloc(NULL, addsection.dwSize, MEM_COMMIT, PAGE_READWRITE);
	if (addsection.SectionAddr == NULL) {
		pMaindlg->outputInfo("��������ڴ�ʧ��!");
		return -1;
	}

	Readmemory(addsection.SectionAddr, addsection.dwBase, addsection.dwSize, MM_RESILENT);
	SectionMap.push_back(addsection);
	return SectionMap.size()-1;
}

EAnalysis::EAnalysis(ULONG dwVBase,ULONG dwVsize)
{
	sectionAlloc textSection;

	textSection.SectionAddr = (BYTE *)VirtualAlloc(NULL, dwVBase, MEM_COMMIT, PAGE_READWRITE); //��������Ĵ���οռ�
	textSection.dwBase = dwVBase;
	textSection.dwSize = dwVsize;

	Readmemory(textSection.SectionAddr, dwVBase, dwVsize, MM_RESILENT);

	SectionMap.push_back(textSection);
}

EAnalysis::~EAnalysis()
{

}

BOOL EAnalysis::EStaticLibInit() {    //�����Ծ�̬���� ʶ���ʼ��

	DWORD	dwResult;
	DWORD	dwCalc;

	BYTE SearchCode[17] = { 0x50,0x64,0x89,0x25,0x00,0x00,0x00,0x00,
						  0x81,0xEC,0xAC,0x01,0x00,0x00,0x53,0x56,0x57 };

	
	dwResult = Search_Bin(SectionMap[0].SectionAddr, SearchCode, SectionMap[0].dwSize, sizeof(SearchCode));

	if (dwResult == 0)
	{
		return FALSE;
	}

	dwCalc = dwResult + 0x26 + SectionMap[0].dwBase;
	
	DWORD EntryAddr = GetPoint(O2V(dwCalc,0));

	if (EntryAddr - SectionMap[0].dwBase > SectionMap[0].dwSize)  //�����ڵ�ַ����һ������,�����һ�ݱ�
	{
		UINT index=AddSection(EntryAddr);
		pEnteyInfo = (PEENTRYINFO)O2V(EntryAddr, index);
	}
	else {
		pEnteyInfo = (PEENTRYINFO)O2V(EntryAddr, 0);
	}

	dwUsercodeStart = pEnteyInfo->dwUserCodeStart;

	return TRUE;
}

BOOL EAnalysis::GetUserEntryPoint() {
	BYTE data[21] = {
		0x55, 0x8B, 0xEC, 0x51, 0x53, 0x56, 0x8B, 0xF1, 0x57, 0x8B, 0x4E, 0x68,
		0x8D, 0x86, 0xD8, 0x00, 0x00, 0x00, 0x50, 0x51, 0xE8 };

	DWORD	dwResult;
	DWORD	dwStartEntry;

	dwResult = Search_Bin(SectionMap[0].SectionAddr, data, SectionMap[0].dwSize, sizeof(data));
	if (dwResult == 0)
	{
		return false;
	}
	dwStartEntry = dwResult + 0x37 + SectionMap[0].dwBase; //call addr
	dwUsercodeEnd = GetPoint(O2V(dwStartEntry,0) + 1) + dwStartEntry + 5;

	return true;
}


DWORD EAnalysis::O2V(DWORD dwOaddr, UINT index)  //ʵ�ʵ�ַ�������ַ
{
	return dwOaddr - SectionMap[index].dwBase + (DWORD)SectionMap[index].SectionAddr;
}
DWORD EAnalysis::V2O(DWORD dwVaddr, UINT index) //�����ַ��ʵ�ʵ�ַ
{
	return dwVaddr - (DWORD)SectionMap[index].SectionAddr + SectionMap[index].dwBase;
}


DWORD EAnalysis::GetPoint(DWORD dwAddr)
{
	return *(DWORD*)dwAddr;
}
DWORD EAnalysis::GetOriginPoint(DWORD dwAddr,UINT index)
{
	DWORD	pDwData;
    pDwData = GetPoint(O2V(dwAddr, index));
	return pDwData;
}


DWORD EAnalysis::Search_BinEx(byte *pSrc, byte *pTrait, int nSrcLen, int nTraitLen) //��ȷ�汾,��ģ��ƥ��
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


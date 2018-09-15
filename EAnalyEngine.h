#include "stdafx.h"

#pragma once

static DWORD Search_Bin(byte *pSrc, byte *pTrait, int nSrcLen, int nTraitLen) //��������������,��0x90������ģ������,����ƫ�ƴ�С
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




typedef struct sectionAlloc
{
	BYTE* SectionAddr;  //������ڴ�ռ��ַ
	DWORD dwBase;       //ԭʼ�������εĻ�ַ
	DWORD dwSize;       //ԭʼ�������εĴ�С
}*psectionAlloc;

typedef struct _ENTRYINFO // �����������Ϣ
{
	DWORD	dwMagic;		//<- δ֪
	DWORD	dwUnkown1;		//+4 δ֪
	DWORD	dwUnkown2;		//+8 δ֪
	DWORD	dwUserCodeStart;//+c �û����뿪ʼ
	DWORD	dwEString;		//+10 �ַ��� ��Դ       ���û���ַ�����Դ,��Ϊ0
	DWORD	dwEStringSize;	//+14 �ַ��� ��Դ��С   ���û���ַ�����Դ,��Ϊ0
	DWORD	dwEWindow;		//+18 ���������Ϣ      �������ڡ���ǩ�ȿؼ�
	DWORD	dwEWindowSize;	//+1c ��С
	DWORD	dwLibNum;		//+20 ֧�ֿ�����
	DWORD	pLibEntey;		//+24 ֧�ֿ���Ϣ���
	DWORD	dwApiCount;     //+28 Api����
	DWORD	pLibName;		//+2C ָ�������
	DWORD	pApiName;		//+30 ָ��Api����
}*PEENTRYINFO;



class EAnalysis
{
public:
	EAnalysis(ULONG dwVBase, ULONG dwVsize);
	~EAnalysis();
	
	BOOL EStaticLibInit();     //��̬����--��ʼ��
	BOOL GetUserEntryPoint();  //��̬����--ȡ�û�������ַ

	UINT FindSection(DWORD addr); //Ѱ�ҵ�ַ�Ƿ������α���,����index
	UINT AddSection(DWORD addr);  //�ڴ濽��������������,�����µ�index

	DWORD   Search_BinEx(byte *pSrc, byte *pTrait, int nSrcLen, int nTraitLen);
	DWORD	O2V(DWORD dwVaddr, UINT index);//origin addr to virtual addr
	DWORD	V2O(DWORD dwOaddr, UINT index);


	DWORD	GetPoint(DWORD dwAddr);
	DWORD	GetOriginPoint(DWORD dwAddr, UINT index);
	DWORD dwUsercodeStart; //�û��������ʼ��ַ
	DWORD dwUsercodeEnd;   //�û�����Ľ�����ַ

	vector<sectionAlloc> SectionMap;    //ά��һ���ڴ濽����

	PEENTRYINFO pEnteyInfo; // entry info
private:
protected:
};
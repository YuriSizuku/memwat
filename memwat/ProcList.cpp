// ProcList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "memwat.h"
#include "ProcList.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CProcList, CProcList)
// CProcList �Ի���
CProcList::CProcList(CWnd* pParent /*=NULL*/)
{

}

CProcList::~CProcList()
{
}

// CProcList ��Ϣ�������
BEGIN_MESSAGE_MAP(CProcList, CListBox)
ON_LBN_SELCHANGE(IDC_PROCLIST,&CProcList::OnLbnDblclkList) 
END_MESSAGE_MAP()

void CProcList::OnLbnDblclkList()
{
	MessageBox("hellp");
}
// ProcList.cpp : 实现文件
//

#include "stdafx.h"
#include "memwat.h"
#include "ProcList.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CProcList, CProcList)
// CProcList 对话框
CProcList::CProcList(CWnd* pParent /*=NULL*/)
{

}

CProcList::~CProcList()
{
}

// CProcList 消息处理程序
BEGIN_MESSAGE_MAP(CProcList, CListBox)
ON_LBN_SELCHANGE(IDC_PROCLIST,&CProcList::OnLbnDblclkList) 
END_MESSAGE_MAP()

void CProcList::OnLbnDblclkList()
{
	MessageBox("hellp");
}
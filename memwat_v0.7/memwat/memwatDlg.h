
// memwatDlg.h : 头文件
//
#include "ProcList.h"
#include <tlhelp32.h>
#include <Psapi.h>
#pragma comment (lib,"Psapi.lib")
#pragma once


// CmemwatDlg 对话框
class CmemwatDlg : public CDialogEx
{
// 构造
public:
	CmemwatDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MEMWAT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CProcList m_proclist;
	CTreeCtrl m_vmemtree;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
	void getPhysicalMemInfo();//获取物理内存信息
	void getPerformanceInfo();
	void getProclist();
	void getProcinfo(DWORD pid);
public:
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnLbnSelchangeProclist();
};

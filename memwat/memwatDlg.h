
// memwatDlg.h : ͷ�ļ�
//
#include "ProcList.h"
#include <tlhelp32.h>
#include <Psapi.h>
#pragma comment (lib,"Psapi.lib")
#pragma once


// CmemwatDlg �Ի���
class CmemwatDlg : public CDialogEx
{
// ����
public:
	CmemwatDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MEMWAT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CProcList m_proclist;
	CTreeCtrl m_vmemtree;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
	void getPhysicalMemInfo();//��ȡ�����ڴ���Ϣ
	void getPerformanceInfo();
	void getProclist();
	void getProcinfo(DWORD pid);
public:
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnLbnSelchangeProclist();
};

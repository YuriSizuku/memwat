
// memwatDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "memwat.h"
#include "memwatDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
void EnableDebugPriv(void)//����Ȩ��
{
	HANDLE hToken;  
	LUID sedebugnameValue;  
	TOKEN_PRIVILEGES tkp;  

	if (!OpenProcessToken(GetCurrentProcess(),  
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {  
			return;  
	}  
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue)) {  
		CloseHandle(hToken);  
		return;  
	}  
	tkp.PrivilegeCount = 1;  
	tkp.Privileges[0].Luid = sedebugnameValue;  
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;  
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL)) {  
		CloseHandle(hToken);  
		return;  
	}   
}
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CmemwatDlg �Ի���




CmemwatDlg::CmemwatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CmemwatDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MAINICO);
}

void CmemwatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CmemwatDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CmemwatDlg::OnBnClickedButtonRefresh)
	ON_LBN_SELCHANGE(IDC_PROCLIST, &CmemwatDlg::OnLbnSelchangeProclist)
END_MESSAGE_MAP()


// CmemwatDlg ��Ϣ�������

BOOL CmemwatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MINIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	EnableDebugPriv();
	m_proclist.SubclassDlgItem(IDC_PROCLIST,this);
	m_vmemtree.SubclassDlgItem(IDC_VMEMTREE,this);
	getProclist();
	getPhysicalMemInfo();
	getPerformanceInfo();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CmemwatDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CmemwatDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CmemwatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CmemwatDlg::getProclist()
{
	m_proclist.ResetContent();

	int i=0;
	HANDLE hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	PROCESSENTRY32 pe32={sizeof(pe32)};
	char buf[1000];
	bool more;
	more=Process32First(hSnapshot,&pe32);
	while(more)
	{
		sprintf(buf,"%d,%s",pe32.th32ProcessID,pe32.szExeFile);
		this->m_proclist.AddString(buf);
		more=Process32Next(hSnapshot,&pe32);
	}
}
void CmemwatDlg::getPhysicalMemInfo()
{
	MEMORYSTATUSEX memo={sizeof(memo)};
	GlobalMemoryStatusEx(&memo);
	this->SetDlgItemInt(IDC_EDIT_MEMLOAD,memo.dwMemoryLoad,false);
	this->SetDlgItemInt(IDC_EDIT_TOTALPHY,memo.ullTotalPhys,false);
	this->SetDlgItemInt(IDC_EDIT_AVAILPHY,memo.ullAvailPhys,false);
	this->SetDlgItemInt(IDC_EDIT_TOTALVIRTUAL,memo.ullTotalVirtual,false);
	this->SetDlgItemInt(IDC_EDIT_AVAILVIRTUAL,memo.ullAvailVirtual,false);
}
void CmemwatDlg::getPerformanceInfo()
{
	PERFORMANCE_INFORMATION pinfo={sizeof(pinfo)};
	GetPerformanceInfo(&pinfo,sizeof(pinfo));
	this->SetDlgItemInt(IDC_EDIT_COMITTOTAL,pinfo.CommitTotal,false);
	this->SetDlgItemInt(IDC_EDIT_COMITLIMIT,pinfo.CommitLimit,false);
	this->SetDlgItemInt(IDC_EDIT_COMITPEAK,pinfo.CommitPeak,false);
	this->SetDlgItemInt(IDC_EDIT_SYSCACHE,pinfo.SystemCache,false);
	this->SetDlgItemInt(IDC_EDIT_PHYSAVAIL,pinfo.PhysicalAvailable,false);
	this->SetDlgItemInt(IDC_EDIT_PHYSTOTAL,pinfo.PhysicalTotal,false);
	this->SetDlgItemInt(IDC_EDIT_KERPAGED,pinfo.KernelPaged,false);
	this->SetDlgItemInt(IDC_EDIT_KERUNPAGED,pinfo.KernelNonpaged,false);
	this->SetDlgItemInt(IDC_EDIT_KERTOTAL,pinfo.KernelTotal,false);
	this->SetDlgItemInt(IDC_EDIT_PAGESIZE,pinfo.PageSize,false);
	this->SetDlgItemInt(IDC_EDIT_HANDLECOUNT,pinfo.HandleCount,false);
	this->SetDlgItemInt(IDC_EDIT_PROCCOUNT,pinfo.ProcessCount,false);
	this->SetDlgItemInt(IDC_EDIT_THREADCOUNT,pinfo.ThreadCount,false);
}
void CmemwatDlg::getProcinfo(DWORD pid)
{
	char buf[1000];
	HANDLE hProcess;
	SYSTEM_INFO si;
	MEMORY_BASIC_INFORMATION mbi;
	FILETIME t1,t2,t3,t4;
	PVOID paddr;
	_int64 time;

	hProcess=OpenProcess(PROCESS_ALL_ACCESS,false,pid);
	//pid
	this->SetDlgItemInt(IDC_EDIT_PROCPID,pid,false);
	//path
	memset(buf,0,sizeof(buf));
	GetModuleFileNameEx(hProcess,NULL,buf,sizeof(buf));
	if(hProcess==NULL) strcpy(buf,"NULL");
	this->SetDlgItemText(IDC_EDIT_PROCPATH,buf);
	//time
	memset(buf,0,sizeof(buf));
	GetProcessTimes(hProcess,&t1,&t2,&t3,&t4);
	time=(((_int64)(t3.dwHighDateTime+t4.dwHighDateTime)<<32)+
			t3.dwLowDateTime+t4.dwLowDateTime)/1000;
	sprintf(buf,"%llu",time);
	this->SetDlgItemText(IDC_EDIT_PROCTIME,buf);
	//vmem
	m_vmemtree.DeleteAllItems();

	HTREEITEM subroot,tnode,t;
	int i=0;
	GetSystemInfo(&si);
	paddr=si.lpMinimumApplicationAddress;
	while(paddr < si.lpMaximumApplicationAddress)
	{
		VirtualQueryEx(hProcess,paddr,&mbi,sizeof(mbi));
		sprintf(buf,"rigion %d",i);
		subroot=m_vmemtree.InsertItem(buf);
		sprintf(buf,"BaseAddress:0x%08x",mbi.BaseAddress);
		tnode=m_vmemtree.InsertItem(buf,subroot);
		sprintf(buf,"AllocBase:0x%08x",mbi.AllocationBase);
		tnode=m_vmemtree.InsertItem(buf,subroot,tnode);
		sprintf(buf,"RegionSize:0x%08x",mbi.RegionSize);
		tnode=m_vmemtree.InsertItem(buf,subroot,tnode);
		paddr=(PVOID)((DWORD)paddr+mbi.RegionSize);
		i++;
	}

	CloseHandle(hProcess);
}

void CmemwatDlg::OnBnClickedButtonRefresh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	getPhysicalMemInfo();
	getPerformanceInfo();
	getProclist();
}

void CmemwatDlg::OnLbnSelchangeProclist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DWORD pid;
	int cur;
	char buf[1000];
	cur=m_proclist.GetCurSel();
	m_proclist.GetText(cur,buf);
	sscanf(buf,"%d,",&pid);
	getProcinfo(pid);
}

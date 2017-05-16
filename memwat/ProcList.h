#pragma once

// CProcList 对话框

class CProcList : public CListBox
{
	DECLARE_DYNAMIC(CProcList)
public:
	CProcList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProcList();
protected:
	DECLARE_MESSAGE_MAP()

	//callbacks
	void OnLbnDblclkList();
};

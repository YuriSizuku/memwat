#pragma once

// CProcList �Ի���

class CProcList : public CListBox
{
	DECLARE_DYNAMIC(CProcList)
public:
	CProcList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProcList();
protected:
	DECLARE_MESSAGE_MAP()

	//callbacks
	void OnLbnDblclkList();
};

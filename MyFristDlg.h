#pragma once
#include "afxwin.h"


// CMyFristDlg 对话框

class CMyFristDlg : public CDialog
{
	DECLARE_DYNAMIC(CMyFristDlg)

public:
	CMyFristDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyFristDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnSave();
	CEdit m_Edit;
};

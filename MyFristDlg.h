#pragma once
#include "afxwin.h"


// CMyFristDlg �Ի���

class CMyFristDlg : public CDialog
{
	DECLARE_DYNAMIC(CMyFristDlg)

public:
	CMyFristDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyFristDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnSave();
	CEdit m_Edit;
};

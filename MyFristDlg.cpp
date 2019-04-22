// MyFristDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OCT.h"
#include "MyFristDlg.h"
#include "afxdialogex.h"
#include "fstream"
using namespace std;

// CMyFristDlg 对话框

IMPLEMENT_DYNAMIC(CMyFristDlg, CDialog)

CMyFristDlg::CMyFristDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyFristDlg::IDD, pParent)
{

}

CMyFristDlg::~CMyFristDlg()
{
}

void CMyFristDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Edit);
}


BEGIN_MESSAGE_MAP(CMyFristDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMyFristDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMyFristDlg 消息处理程序


void CMyFristDlg::OnBnClickedOk()//提交按钮
{
	// TODO:  在此添加控件通知处理程序代码

	CDialog::OnOK();
	
	if (MessageBox(_T("确定提交吗?"), _T("提示"), MB_YESNO | MB_ICONWARNING) == IDYES)
	{
		
		OnSave();
			return;
	}
		


}
void CMyFristDlg::OnSave()
{
	/*CString str;//另存为的方法
	CFileDialog   FileDlg(TRUE, "txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);
	FileDlg.m_ofn.lpstrInitialDir = "feedback.txt";                   //可设置其他初始路径
	if (FileDlg.DoModal() == IDOK)
	{
		ofstream ofs(FileDlg.GetPathName());
		CStatic*pst = (CStatic*)GetDlgItem(IDC_EDIT1);            //你的控件ID
		pst->GetWindowText(str);
		ofs << str;
	}*/
	UpdateData();
	fstream OutFile("feedback.txt", ios::out);
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	if (OutFile.is_open())
	{
		OutFile << str << endl;
	}
	OutFile.close();

}


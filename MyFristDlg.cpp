// MyFristDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OCT.h"
#include "MyFristDlg.h"
#include "afxdialogex.h"
#include "fstream"
using namespace std;

// CMyFristDlg �Ի���

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


// CMyFristDlg ��Ϣ�������


void CMyFristDlg::OnBnClickedOk()//�ύ��ť
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CDialog::OnOK();
	
	if (MessageBox(_T("ȷ���ύ��?"), _T("��ʾ"), MB_YESNO | MB_ICONWARNING) == IDYES)
	{
		
		OnSave();
			return;
	}
		


}
void CMyFristDlg::OnSave()
{
	/*CString str;//���Ϊ�ķ���
	CFileDialog   FileDlg(TRUE, "txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);
	FileDlg.m_ofn.lpstrInitialDir = "feedback.txt";                   //������������ʼ·��
	if (FileDlg.DoModal() == IDOK)
	{
		ofstream ofs(FileDlg.GetPathName());
		CStatic*pst = (CStatic*)GetDlgItem(IDC_EDIT1);            //��Ŀؼ�ID
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


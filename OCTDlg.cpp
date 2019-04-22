
// OCTDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OCT.h"
#include "OCTDlg.h"
#include "afxdialogex.h"
#include "CvvImage.h"
#include<cv.h>
#include"resource.h"
#include "MyButton.h"
#include "svm.h" 
#include <vector>  
#include <list>  
#include <iostream>  
#include <fstream>
#include <sstream> 
#include <afx.h>
#include<algorithm>  
#include<functional>  
#include<string> 
#include <opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include "stdafx.h"  
#include <stdlib.h> 
#include<math.h>
#include<cstdlib>
#include<ctime>
#include "ADOConn.h"
#define random(a,b) (rand()%(b-a+1)+a)
#define MAX_FILE_SIZE 1024  
#import "C:\Program Files\Common Files\system\ado\msado15.dll"  no_namespace rename("EOF", "adoEOF")  

using namespace std;
using namespace cv;

int FEATUREDIM = 800;
//ͼ�����ֵĲ���
Mat srcImage;
Mat  m_srcImage;
CRect Trect1;          //������Ƥ���ľ���
IplImage* TheImage;

cv::String route;
int total_picture = 1;
int coor[100];//��������
int total = 0;//�߿�����

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/***************************************************************************************************/
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class ClassificationSVM
{
public:
	ClassificationSVM();
	void train(const std::string& modelFileName);
	void predict(const std::string& featureaFileName, const std::string& modelFileName);


private:
	void setParam();
	void readTrainData(const std::string& featureFileName);
private:
	svm_parameter param;
	svm_problem  prob;//all the data for train  
	std::list<svm_node*> dataList;//list of features of all the samples   
	std::list<double>  typeList;//list of type of all the samples   
	int sampleNum;
	//bool* judgeRight;  
};

ClassificationSVM::ClassificationSVM()
{
	setParam();
}

void ClassificationSVM::setParam()
{
	param.svm_type = C_SVC; // ONE_CLASS;
	param.kernel_type = RBF;
	param.degree = 3;
	param.gamma = 2.2;//0.5(69%) 1.2,1.4,1.7(��ѡ),1.7
	param.coef0 = 0;
	param.nu = 0.7;
	param.cache_size = 40;
	param.C = 4000;
	param.eps = 1e-5;
	param.p = 0.1;
	param.shrinking = 1;
	param.nr_weight = 0;
	param.weight = NULL;
	param.weight_label = NULL;
}
/***************************************************************************************************/
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


// COCTDlg �Ի���



COCTDlg::COCTDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COCTDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME); 
	
}

void COCTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRAM_LIST, m_programLangList);
	DDX_Control(pDX, IDC_BUTTON1, m_leadbtn);

	DDX_Control(pDX, IDC_BUTTON2, m_trainstb);
	DDX_Control(pDX, IDC_BUTTON3, m_labelbtn);
	DDX_Control(pDX, IDC_BUTTON4, m_showbtn);
	DDX_Control(pDX, IDOK, m_exitbtn);


	DDX_Control(pDX, IDC_STATIC_PIC, m_image);
}

BEGIN_MESSAGE_MAP(COCTDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &COCTDlg::OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDCLOSE, &COCTDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDOK, &COCTDlg::OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON3, &COCTDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &COCTDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &COCTDlg::OnBnClickedButton4)
	ON_WM_TIMER()
	ON_COMMAND(ID_32771, &COCTDlg::On32771)
	ON_COMMAND(ID_32772, &COCTDlg::On32772)
	ON_COMMAND(ID_32773, &COCTDlg::On32773)
	ON_COMMAND(ID_32774, &COCTDlg::On32774)
	ON_COMMAND(ID_32785, &COCTDlg::On32785)
	
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_32784, &COCTDlg::On32784)
	ON_COMMAND(ID_32786, &COCTDlg::On32786)
	ON_COMMAND(ID_32787, &COCTDlg::On32787)
	ON_COMMAND(ID_32777, &COCTDlg::On32777)
	ON_COMMAND(ID_32779, &COCTDlg::On32779)
	ON_COMMAND(ID_32781, &COCTDlg::On32781)
	ON_COMMAND(ID_32778, &COCTDlg::On32778)
	ON_COMMAND(ID_32780, &COCTDlg::On32780)
	
	ON_COMMAND(ID_32788, &COCTDlg::On32788)
//	ON_WM_NCLBUTTONDOWN()
ON_WM_LBUTTONDOWN()
//ON_WM_SETCURSOR()
//ON_COMMAND(ID_32790, &COCTDlg::On32790)
ON_COMMAND(ID_32791, &COCTDlg::On32791)
END_MESSAGE_MAP()


// COCTDlg ��Ϣ�������

BOOL COCTDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	ShowWindow(SW_MAXIMIZE);

	GetClientRect(&m_rect); //��ȡ�Ի���Ĵ�С
	//���   
	m_programLangList.GetClientRect(&m_rect);

	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_programLangList.SetExtendedStyle(m_programLangList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// Ϊ�б���ͼ�ؼ��������   
	m_programLangList.InsertColumn(0, _T("�������"), LVCFMT_CENTER, m_rect.Width() / 3, 0);
	m_programLangList.InsertColumn(1, _T("����߿�����"), LVCFMT_CENTER, m_rect.Width() / 3, 1);
	m_programLangList.InsertColumn(2, _T("�߿�����"), LVCFMT_CENTER, m_rect.Width() / 3, 2);
	
	

	
	// TODO:  �ڴ���Ӷ���ĳ�ʼ������


	

	m_leadbtn.ModifyStyle(0, BS_OWNERDRAW, 0);
	//����Button Down�ı���ɫ
	m_leadbtn.SetDownColor(RGB(255, 0, 0));
	//����Button Up�ı���ɫ
	m_leadbtn.SetUpColor(RGB(255,222,246));

	m_trainstb.ModifyStyle(0, BS_OWNERDRAW, 0);
	//����Button Down�ı���ɫ
	m_trainstb.SetDownColor(RGB(255, 0, 0));
	//����Button Up�ı���ɫ
	m_trainstb.SetUpColor(RGB(255, 222, 246));

	m_labelbtn.ModifyStyle(0, BS_OWNERDRAW, 0);
	//����Button Down�ı���ɫ
	m_labelbtn.SetDownColor(RGB(255, 0, 0));
	//����Button Up�ı���ɫ
	m_labelbtn.SetUpColor(RGB(255, 235, 244));

	m_showbtn.ModifyStyle(0, BS_OWNERDRAW, 0);
	//����Button Down�ı���ɫ
	m_showbtn.SetDownColor(RGB(255, 0, 0));
	//����Button Up�ı���ɫ
	m_showbtn.SetUpColor(RGB(255, 198, 199));

	m_exitbtn.ModifyStyle(0, BS_OWNERDRAW, 0);
	//����Button Down�ı���ɫ
	m_exitbtn.SetDownColor(RGB(255, 0, 0));
	//����Button Up�ı���ɫ
	m_exitbtn.SetUpColor(RGB(230, 151, 157));
	
	/*
	   m_rctCurTracker.m_rect.SetRect(0, 0, 0, 0);//���þ��������С  
	   m_rctCurTracker.m_nStyle = CRectTracker::dottedLine | CRectTracker::resizeInside;
	   m_rctCurTracker.m_nHandleSize = 6;
	   for (int i = 0; i<MAX_RECT_NUM; i++)
	    {
	        m_rctTracker[i].m_rect.SetRect(0, 0, 0, 0);//���þ��������С  
	        m_rctTracker[i].m_nStyle = CRectTracker::dottedLine | CRectTracker::resizeInside;
	        m_rctTracker[i].m_nHandleSize = 6;
	    }
	
	    m_IsChose = FALSE;//��ʾδѡ��  
	    m_IsDraw = false;
	    m_rectNum = 0;
	    m_rctChoseNum = 0;
	    m_FlaMoveStep = 2;
	    dirct = 0;
	*/


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
void COCTDlg::LIST()
{
	

	// ���б���ͼ�ؼ��в����б���������б������ı� 
	CString strr;
	CString strr2;
	CString strr3;
	CString strr4;
	CString strr5;
	strr.Format(_T("%d"), total);
	strr5.Format(_T("%d"), total_picture);
	m_programLangList.InsertItem(0, _T(strr5));
	m_programLangList.SetItemText(0, 1, _T(strr));
	for (int i = 0; i < total*2; i+=2)
	{
		strr2.Format(_T("%d"), coor[i]);
		strr3.Format(_T("%d"), coor[i+1]);
		strr4 += "(" + strr2 + "," + strr3 + ")" + " ";
		coor[i] = 0;
		coor[i + 1] = 0;
	}
	m_programLangList.SetItemText(0, 2, _T(strr4));
	

}
void COCTDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void COCTDlg::OnPaint()//����
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
		//CDialogEx::OnPaint();
	    CPaintDC   dc(this);
		CRect   rect;
		GetClientRect(&rect);    //��ȡ�Ի��򳤿�
		CDC   dcBmp;              //���岢����һ���ڴ��豸����
		dcBmp.CreateCompatibleDC(&dc);             //����������DC
		CBitmap   bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP1);    //������Դ��ͼƬ
		BITMAP   m_bitmap;                         //ͼƬ����
		bmpBackground.GetBitmap(&m_bitmap);       //��ͼƬ����λͼ��
		//��λͼѡ����ʱ�ڴ��豸����
		CBitmap  *pbmpOld = dcBmp.SelectObject(&bmpBackground);
		//���ú�����ʾͼƬStretchBlt��ʾ��״�ɱ�
		dc.SetStretchBltMode(HALFTONE);
		dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcBmp, 0, 0, m_bitmap.bmWidth, m_bitmap.bmHeight, SRCCOPY);
		
		

		 /* if (m_IsChose)
		         m_rctCurTracker.Draw(&dc);//��ѡ���˸���������ʾ�߿��Լ�8��������  
		  CPen pen(PS_SOLID, 1, RGB(100, 255, 200));
		  dc.SelectObject(&pen);
		
		  CBrush *pbrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		  dc.SelectObject(pbrush);
		 CRect m_rect;
		   m_rctCurTracker.GetTrueRect(&m_rect);//�õ���������Ĵ�С  
		   dc.Rectangle(&m_rect);//��������  
		   CSize rct_size;
		  for (int i = 0; i<MAX_RECT_NUM; i++)
		   {
		     m_rctTracker[i].GetTrueRect(&m_rect);//�õ���������Ĵ�С  
		     rct_size = m_rctTracker[i].m_rect.Size();
		       if (rct_size.cx * rct_size.cy == 0 || i == m_rctChoseNum)
		       {
		        continue;
		       }
		      dc.Rectangle(&m_rect);//��������  
		   }*/
		                

	}
	if (TheImage != NULL)
	{
		ShowImage(TheImage, IDC_STATIC_PIC);

	}
	
	
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR COCTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void COCTDlg::OnBnClickedButton1()//���밴ť
{
	CString m_strPicPath;
	

	CFileDialog dlg(true, _T("*.bmp"), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image file(*.bmp;*.jpg)|*.bmp;*.jpg|All Files(*.*)|*.*|"), NULL);
	//����ѡ��ͼƬ�Ի���
	dlg.m_ofn.lpstrTitle = _T("open Image");
	if (dlg.DoModal() != IDOK)
		return;
	m_strPicPath = dlg.GetPathName();//��ȡͼƬ��·��
	route = m_strPicPath;
	m_strPicPath.Replace(_T("//"), _T("////"));
	TheImage = cvLoadImage((CT2CA)m_strPicPath, 1);
	if (TheImage != NULL)
	{
		ShowImage(TheImage, IDC_STATIC_PIC);

	}

}
void COCTDlg::ShowImage(IplImage* img, UINT ID)
{

	CDC* pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	SetRect(rect, rect.left, rect.top, rect.right, rect.bottom);
	CvvImage cimg;
	cimg.CopyOf(img);
	cimg.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);

}

void COCTDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (MessageBox(_T("ȷ���˳���?"), _T("��ʾ"), MB_YESNO | MB_ICONWARNING) == IDNO)
		return;

	CDialogEx::OnClose();
}


void COCTDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	

	// TODO:  �ڴ˴������Ϣ����������
	if (0 == m_rect.left && 0 == m_rect.right && 0 == m_rect.top && 0 == m_rect.bottom)//��һ�������Ի���ʱ�Ĵ�С�仯��������  
		{
		}
	else
	{

		if (0 == cx && 0 == cy)//����ǰ�������С�����򴥷���������ʱ������Ի�������  
		{
			return;
		}
		CRect rectDlgChangeSize;
		GetClientRect(&rectDlgChangeSize);//�洢�Ի����С�ı��Ի����С����  

		repaint(IDC_BUTTON1, m_rect.Width(), rectDlgChangeSize.Width(), m_rect.Height(), rectDlgChangeSize.Height());//�ػ溯�������Ը��¶Ի����Ͽؼ���λ�úʹ�С 
		repaint(IDC_BUTTON2, m_rect.Width(), rectDlgChangeSize.Width(), m_rect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_BUTTON3, m_rect.Width(), rectDlgChangeSize.Width(), m_rect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_BUTTON4, m_rect.Width(), rectDlgChangeSize.Width(), m_rect.Height(), rectDlgChangeSize.Height());
		repaint(IDOK, m_rect.Width(), rectDlgChangeSize.Width(), m_rect.Height(), rectDlgChangeSize.Height());
		repaint(IDCLOSE, m_rect.Width(), rectDlgChangeSize.Width(), m_rect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_STATIC_PIC, m_rect.Width(), rectDlgChangeSize.Width(), m_rect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_PROGRAM_LIST, m_rect.Width(), rectDlgChangeSize.Width(), m_rect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_STATIC_CAPTION, m_rect.Width(), rectDlgChangeSize.Width(), m_rect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_STATIC, m_rect.Width(), rectDlgChangeSize.Width(), m_rect.Height(), rectDlgChangeSize.Height());
	}
	GetClientRect(&m_rect); //save size of dialog  
	Invalidate();//���´��� 
	

	

}


void COCTDlg::repaint(UINT ID, int last_Width, int now_Width, int last_Height, int now_Height)//���¿ؼ�λ�úʹ�С���� 
{
	CRect rect;
	CWnd *wnd = NULL;
	wnd = GetDlgItem(ID);


	if (NULL == wnd)
	{
		// MessageBox(_T("��Ӧ�ؼ�������"));
	}
	else
	{
		/*wnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		double proportion_x = now_Width / (double)last_Width;
		double proportion_y = now_Height / (double)last_Height;

		rect.left = (long)(rect.left*proportion_x + 0.5);
		rect.right = (long)(rect.right*proportion_x + 0.5);
		rect.top = (long)(rect.top *proportion_y + 0.5);
		rect.bottom = (long)(rect.bottom *proportion_y + 0.5);
		wnd->MoveWindow(&rect);*/
	}
	
}

void COCTDlg::OnBnClickedClose()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
}


void COCTDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}





HBRUSH COCTDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	CFont font;
	LOGFONT lf;
	if (!pDC)
		return NULL;
	HBRUSH hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	// TODO:  �ڴ˸��� DC ���κ�����
	pDC->SetBkMode(TRANSPARENT);
	//�������ֵ���ɫΪ��ɫ    
	pDC->SetTextColor(RGB(0, 0, 0));
	::ZeroMemory(&lf, sizeof(lf));
	//�����߼�����ĸ߶�  
	lf.lfHeight = 20;
	//�����߼�����Ϊ������    
	lf.lfWeight = FW_BOLD;
	//��������Ϊ����    
	::lstrcpy(lf.lfFaceName, _T("����"));
	//���߼����崴��һ��CFont����    
	font.CreateFontIndirect(&lf);
	//Ӧ������    
	pDC->SelectObject(&font);

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void COCTDlg::OnBnClickedButton3()//Ӱ����
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();
	ostringstream oss;
	ostringstream ss;
	int m, q = 0;
	oss << format("%04d", total_picture);
	regionExtraction(0, 46, 720, 306, route , "D:\\OCT\\pic\\Go to dry pic\\" + oss.str() + ".png");//ȥ��
	srcImage(Rect(0, 0, 720, 352)).copyTo(m_srcImage);
	imwrite("D:\\OCT\\pic\\signpic\\" + oss.str() + ".png", m_srcImage);

	//first_picture++;
	srcImage = imread("D:\\OCT\\pic\\Go to dry pic\\" + oss.str() + ".png");
	
	m = srcImage.cols / 5; //һ��ͼ��ָ�Ĵ���
	for (int n = 1; n <= m; n++){
		ss << format("%04d", n);
		regionExtraction(q, 0, 5, 306, "D:\\OCT\\pic\\Go to dry pic\\" + oss.str() + ".png", "D:\\OCT\\pic\\five pixel\\" + ss.str() + ".png");
		q = q + 5;
		ss.str("");
	}

	_bstr_t	strSql = "Delete from tb_Training where 1 = 1";
	ADOConn con;
	con.OnInitADOConn();
	//con.m_pConnection->Execute(strSql, &ra, adCmdText);
	con.ExecuteSQL(strSql);
	for (int j = 1; j <= 144; j++){
		oss.str("");
		ss.str("");
		oss << format("%04d", j);
		ss << format("%d", j);
		fft2(oss.str(), ss.str(),j);
	}
	EndWaitCursor();
	MessageBox("Ӱ������ɣ�");
	

}

void COCTDlg::regionExtraction(int xRoi, int yRoi, int widthRoi, int heightRoi, String path1, String path2)//ȥ��
{        //path1Ϊ��Ҫ�����ͼƬ��ַ��path2Ϊ������ŵĵ�ַ
	//ѡ����TrainingData�����1��ͼ
	srcImage = imread(path1);                                                      //�������ĳ���ѡͼƬ�ĵ�ַ������
	Mat roiImage(srcImage.rows, srcImage.cols, CV_8UC3);
	srcImage(Rect(xRoi, yRoi, widthRoi, heightRoi)).copyTo(roiImage);
	imwrite(path2, roiImage);
	//std::cout << roiImage.rows << " " << roiImage.cols << std::endl;
}
Mat COCTDlg::fft2(String s, String mm,int num)
{
	String ID;
	Mat I = imread("D:\\OCT\\pic\\five pixel\\" + s + ".png", CV_LOAD_IMAGE_GRAYSCALE);      //�����ͼƬ·��

	Mat padded;                            //expand input image to optimal size
	int m = getOptimalDFTSize(I.rows);
	int n = getOptimalDFTSize(I.cols); // on the border add zero values
	copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat complexI;
	merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

	dft(complexI, complexI);            // this way the result may fit in the source matrix

	// compute the magnitude and switch to logarithmic scale
	// => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
	split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
	Mat magI = planes[0];

	magI += Scalar::all(1);                    // switch to logarithmic scale
	log(magI, magI);

	// crop the spectrum, if it has an odd number of rows or columns
	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

	// rearrange the quadrants of Fourier image  so that the origin is at the image center
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

	Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);
	cout << mm << endl;
	ofstream outf;
	String strdata = "";
	//outf.open("C:\\Users\\52983\\Desktop\\" + mm + ".txt");               //������ı�
	for (int r = 0; r < magI.rows; r++)
	{
		for (int c = 0; c < magI.cols; c++)
		{
			if (r<100 || r>=220)
			{
				float data = magI.at<float>(r, c);  //��ȡ���ݣ�at<type> - type �Ǿ���Ԫ�صľ������ݸ�ʽ  
				ostringstream ff;
				ff.str("");
				ff << format("%.5f", data); //������λС��
				outf << ff.str() << " ";   //ÿ�������� tab ����  
				strdata = strdata + " " + ff.str();//������������strdata
			}

		}
		outf << endl;  //����  

	}

	ID = to_string(num);
	_bstr_t strSql = "";
	_variant_t ra;
	outf.close();
    String CstrSql = "INSERT INTO [tb_Training] (data) VALUES ('";	
	CstrSql = CstrSql += strdata += "')";
	char* charSource = (char*)CstrSql.c_str();
	strSql = charSource;
	ADOConn con;
	con.OnInitADOConn();
	//con.m_pConnection->Execute(strSql, &ra, adCmdText);
	con.ExecuteSQL(strSql);
	outf.close();
	normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
	// viewable image form (float between values 0 and 1).
	return magI;
}


bool stringready(string str)
{
	int i = 0;
	if ((unsigned)(str.at(i)) == 45)
		i++;
	for (; i<str.length(); i++)
	{
		unsigned one = (unsigned)(str.at(i));
		if (one >= 48 && one <= 57 || one == 46)
			;
		else
			return 0;
	}
	return 1;
}

double stringtodouble(string str)
{
	int i = 0, sign;
	double integer = 0;
	double decimal = 0;
	int dot = str.length();
	if ((unsigned)(str.at(i)) == 45)
	{
		sign = -1;
		i++;
	}
	else
		sign = 1;

	for (; i<str.length(); i++)
	{
		unsigned one = (unsigned)(str.at(i));
		if (one >= 48 && one <= 57)
		if (i<dot)
			integer = (one - 48) + integer * 10;
		else
			decimal = decimal + (one - 48) / pow(10, i - dot);
		else
		if (one == 46)
			dot = i;
	}
	return sign*(integer + decimal);
}
/*void ClassificationSVM::setParam(double gamma, double C)
{
param.gamma = gamma;
param.C     = C;
}*/
void permutation(int n, int *z_array)
{
	int i, j, k, z;
	int buffer[6000];

	/* ��ʼ������ */
	for (i = 0; i<n; i++)
		buffer[i] = 0;

	/* ׼�����������,�Ե�ǰʱ��Ϊ���� */
	srand((unsigned)time(NULL));

	/* ��ò��ظ���������� */
	for (i = 0; i<n; i++) {
		/* ���0~��n-i����������� */
		z = rand() % (n - i);
		j = 0; k = 0;
		while (j <= z) {
			if (buffer[j + k] == 0) j++;
			else k++;
		}
		buffer[j + k - 1] = 1;
		z_array[i] = j + k - 1;
	}
	return;
}
void ClassificationSVM::readTrainData(const string& SqlTableName)
{
	const int Num = 6000;//��ȡ����
	string StringValue;	 //�䵱Temp
	_bstr_t  strSql = "SELECT * FROM tb_PositiveFeatures";//ѡ����SQL��� 
	if (SqlTableName == "PositiveFeatures")
	{
		strSql = "SELECT data FROM tb_PositiveFeatures";
	}
	if (SqlTableName == "NegativeFeatures")
	{
		strSql = "SELECT data FROM tb_NegativeFeatures1";
	}
	ADOConn con;
	con.OnInitADOConn();
	con.ExecuteSQL(strSql);
	for (size_t i = 1; i <= Num; i++)
	{
		int k = 0;
		string res;
		string Name;
		//cout << "Sql has opened...." << endl;
		res = (_bstr_t)con.m_pRecordset->GetFields()->GetItem("data")->Value;   //����
		//Name = (_bstr_t)con.m_pRecordset->GetFields()->GetItem("txtName")->Value;//�ļ���
		svm_node* features = new svm_node[FEATUREDIM + 1];
		istringstream is(res);//���տո���ȡValue
		while (is >> StringValue)
		{
			double value = 0;
			features[k].index = k + 1;//������ţ���1��ʼ  
			features[k].value = stringtodouble(StringValue);//����ֵ  
			k++;
		}
		features[FEATUREDIM].index = -1;//�������  
		//negative sample type is 0  
		int type = 0;
		//positive sample type is 1  
		if (SqlTableName == "PositiveFeatures")                //����
			type = 1;
		dataList.push_back(features);
		typeList.push_back(type);
		sampleNum++;
		con.m_pRecordset->MoveNext();
		//cout << "sql done" << endl;
		


	}


	/*int ID[5999]; //�洢�����������
	permutation(5999, ID); //���ò���������Ĺ��ܺ���
	int Num =500;// �����ȡ����
	
	for (size_t i = 1; i <= Num; i++)
	{
		string StringValue;	 //�䵱Temp		
		String CstrID = to_string(ID[i] + 1);//int->Stringת����ʽ
		_bstr_t strSql = "";

		if (SqlTableName == "PositiveFeatures")
		{
			String CstrSql = "SELECT * FROM tb_PositiveFeatures Where sign=0 and ID=";
			CstrSql = CstrSql += CstrID;
			char* charSource = (char*)CstrSql.c_str(); //�õ�sqlָ�����
			strSql = charSource;
		}
		if (SqlTableName == "NegativeFeatures")
		{
			String CstrSql = "SELECT * FROM tb_NegativeFeatures1 Where sign=0 and ID=";
			CstrSql = CstrSql += CstrID;
			char* charSource = (char*)CstrSql.c_str(); //�õ�sqlָ�����
			strSql = charSource;
		}
		ADOConn con;
		con.OnInitADOConn();//��ʼ��sql����
		con.ExecuteSQL(strSql);//ִ��command���
		
		int k = 0;
		string res;
		string Name;
	    string sign = "0";//�Ƿ񱻶�ȡ�ı�ʶ
		_variant_t ra;
		//cout << "Sql has opened...." << endl;
		res = (_bstr_t)con.m_pRecordset->GetFields()->GetItem("data")->Value;   //����
		//Name = (_bstr_t)con.m_pRecordset->GetFields()->GetItem("txtName")->Value;//�ļ���
		sign = (_bstr_t)con.m_pRecordset->GetFields()->GetItem("sign")->Value;//��ñ�ʶ
		//con.m_pRecordset->MoveNext();
		if (SqlTableName == "PositiveFeatures")
		{
			String CstrSql = "UPDATE tb_PositiveFeatures SET sign=1 WHERE  ID=";
			CstrSql = CstrSql += CstrID;
			char* charSource = (char*)CstrSql.c_str();
			strSql = charSource;
			con.m_pConnection->Execute(strSql, &ra, adCmdText); //����ȡ�������ݵı�ʶ��1
		}
		if (SqlTableName == "NegativeFeatures")
		{
			String CstrSql = "UPDATE tb_NegativeFeatures1 SET sign=1 WHERE  ID=";
			CstrSql = CstrSql += CstrID;
			char* charSource = (char*)CstrSql.c_str();
			strSql = charSource;
			con.m_pConnection->Execute(strSql, &ra, adCmdText);
		}


		svm_node* features = new svm_node[FEATUREDIM + 1];//��Ϊ��Ҫ������ǣ��������ռ�ʱ����ά��+1  		

		istringstream is(res);//���տո���ȡValue
		while (is >> StringValue)
		{
			double value = 0;
			features[k].index = k + 1;//������ţ���1��ʼ  
			features[k].value = stringtodouble(StringValue);//����ֵ  
			k++;
		}
		features[FEATUREDIM].index = -1;//�������  
		//negative sample type is 0  
		int type = 0;
		//positive sample type is 1  
		if (SqlTableName == "PositiveFeatures")                //����
			type = 1;
		dataList.push_back(features);
		typeList.push_back(type);
		sampleNum++;


	}
	_bstr_t strSql = "";
	String CstrSql = "UPDATE tb_NegativeFeatures1   SET sign=0 UPDATE tb_PositiveFeatures   SET sign=0";
	char* charSource = (char*)CstrSql.c_str();
	strSql = charSource;
	ADOConn con;
	con.OnInitADOConn();
	con.ExecuteSQL(strSql);    */        //��ʶ��0�������´�ѵ��
}


void ClassificationSVM::train(const string& modelFileName)
{
	cout << "reading positivie features..." << endl;
	readTrainData("PositiveFeatures");
	cout << "reading negative features..." << endl;
	readTrainData("NegativeFeatures");
	cout << sampleNum << endl;
	prob.l = sampleNum;//number of training samples  
	prob.x = new svm_node *[prob.l];//features of all the training samples  
	prob.y = new double[prob.l];//type of all the training samples  
	int index = 0;
	while (!dataList.empty())
	{
		prob.x[index] = dataList.front();
		prob.y[index] = typeList.front();
		dataList.pop_front();
		typeList.pop_front();
		index++;
	}

	const svm_problem* pProb = &prob;
	svm_model *svmModel = svm_train(const_cast<svm_problem*>(&prob), &param);

	svm_save_model(modelFileName.c_str(), svmModel);
	//cout << prob.l;
}


void COCTDlg::OnBnClickedButton2()//ѵ����ť����
{
	BeginWaitCursor();
	ClassificationSVM a; 
	SetTimer(1, 1000, NULL);
	int ret = MessageBox("ѵ���С�����", "MyMessageBox");
	a.train("result.txt");
	EndWaitCursor();
	MessageBox("ѵ����ɣ�");
	

}


int Draw_line(int x1, int x2, string s){
	srcImage = imread("D:\\OCT\\pic\\signpic\\" + s + ".png");
	cv::Point start = cv::Point(x1, 0);
	cv::Point end = cv::Point(x1, 352);
	cv::line(srcImage, start, end, cv::Scalar(0, 0, 255));
	cv::Point start_2 = cv::Point(x2, 0);
	cv::Point end_2 = cv::Point(x2, 352);
	cv::line(srcImage, start_2, end_2, cv::Scalar(0, 0, 255));
	imwrite("D:\\OCT\\pic\\signpic\\" + s + ".png", srcImage);
	//	cout << s;
	return 0;
}


void ClassificationSVM::predict(const string& SqlTableName, const string& modelFileName)
{
	
	std::vector<bool> judgeRight;
	svm_model *svmModel = svm_load_model(modelFileName.c_str());
	const int Num = 144;//��ȡ����
	string StringValue;	 //�䵱Temp
	_bstr_t  strSql = "SELECT * FROM tb_Training";//ѡ����SQL��� 
	ADOConn con;
	con.OnInitADOConn();
	con.ExecuteSQL(strSql);
	for (size_t i = 0; i < Num; i++)
	{

		int k = 0;
		string res;
		string Name;
		//cout << "Sql has opened...." << endl;
		res = (_bstr_t)con.m_pRecordset->GetFields()->GetItem("data")->Value;   //����
		//Name = (_bstr_t)con.m_pRecordset->GetFields()->GetItem("txtName")->Value;//�ļ���
		svm_node* input = new svm_node[FEATUREDIM + 1];
		istringstream is(res);//���տո���ȡValue
		while (is >> StringValue)
		{
			double value = 0;
			input[k].index = k + 1;//������ţ���1��ʼ  
			input[k].value = stringtodouble(StringValue);//����ֵ  
			k++;
		}
		input[FEATUREDIM].index = -1;//�������  

		int predictValue = svm_predict(svmModel, input);
		if (SqlTableName == "positive_test")
		{
			if (predictValue == 0)
				judgeRight.push_back(false);
			else
				judgeRight.push_back(true);
		}
		else if (SqlTableName == "negative_test")
		{
			if (predictValue == 1)
				judgeRight.push_back(false);
			else
				judgeRight.push_back(true);
		}
		//cout << "sql done" << endl;
		con.m_pRecordset->MoveNext();

	}
	/*
	FILE *fp;
	if ((fp = fopen(featureFileName.c_str(), "rt")) == NULL)
	return;

	fseek(fp, 0L, SEEK_END);
	long end = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	long start = ftell(fp);
	while (start != end)
	{
	svm_node* input = new svm_node[FEATUREDIM + 1];
	for (int k = 0; k<FEATUREDIM; k++)
	{
	double value = 0;
	fscanf(fp, "%lf", &value);
	input[k].index = k + 1;
	input[k].value = value;
	}
	char c;
	fscanf(fp, "\n", &c);
	char name[100];
	fgets(name, 100, fp);
	name[strlen(name) - 1] = '\0';

	input[FEATUREDIM].index = -1;
	int predictValue = svm_predict(svmModel, input);
	if (featureFileName == "positive_test.txt")
	{
	if (predictValue == 0)
	judgeRight.push_back(false);
	else
	judgeRight.push_back(true);
	}
	else if (featureFileName == "negative_test.txt")
	{
	if (predictValue == 1)
	judgeRight.push_back(false);
	else
	judgeRight.push_back(true);
	}
	start = ftell(fp);

	}
	fclose(fp);*/

	int correctNum = 0;
	int totalNum = judgeRight.size();
	int x1 = 0, x2 = 0, x3 = -1, flag = 1, count = 0;
	int count2 = 0;
	ostringstream oss;
	oss << format("%04d", total_picture);
	for (int i = 0; i < totalNum; i++)
	{
		if (judgeRight[i] == false){                        //���ɨ�赽���߿�
			if (flag == 1){
				x1 = i * 5;
				//	Draw_line(x1, x1, oss.str());
				flag = 0;
			}
			else {
				x2 = 5 * i;
				count = 0;
			}
		}
		else	if (flag == 0 && judgeRight[i] == true){                 //���ɨ�赽���߿�
			count++;
			if (count > 14){
				if (x2 != x3&&abs(x1 - x2)>10){
					Draw_line(x1, x2, oss.str());
					coor[count2] = x1;
					count2++;
					coor[count2] = x2;
					count2++;
					total++;
				}

				x3 = x2;
				flag = 1;
			}
		}
		if (i == totalNum - 1 && flag == 0){
			x2 = 720;
			if (abs(x1 - x2) > 10){
				Draw_line(x1, x2, oss.str());
				coor[count2] = x1;
				count2++;
				coor[count2] = x2;
				count2++;
				total++;
			}

		}

	}

	/*for (int i = 0; i < totalNum; i++)
	{
		if (judgeRight[i] == true)
			correctNum++;
	}*/
	//cout << precent << endl;
	
	
}


void COCTDlg::OnBnClickedButton4()//��ǰ߿�
{
	BeginWaitCursor();
	ClassificationSVM b;
	ostringstream oss;
	oss << format("%04d", total_picture);
	b.predict("negative_test", "result.txt");
	
	srcImage = imread("D:\\OCT\\pic\\signpic\\" +oss.str()+ ".png");
	IplImage *input = &IplImage(srcImage); //ǳ����
	TheImage = cvCloneImage(input);//��

	if (TheImage != NULL)
	{
		ShowImage(TheImage, IDC_STATIC_PIC);

	}
	LIST();
	total_picture++;
	total = 0;
	EndWaitCursor();
	
}


void COCTDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1)
	{
		HANDLE hWnd = ::FindWindowEx(NULL, NULL, NULL,  "MyMessageBox");
		//HANDLE hWnd = ::GetForegroundWindow();  
		if (hWnd)
		{
			TRACE("������MyMessageBox����\n");
			::SendMessage((HWND)hWnd, WM_CLOSE, NULL, NULL);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void COCTDlg::On32771()//ֱ��ͼ���⻯
{
	// TODO:  �ڴ���������������
	Mat image = imread(route, 1);
	if (image.empty())
	{
		
	}
	//imshow("ԭͼ��", image);
	Mat imageRGB[3];
	split(image, imageRGB);
	for (int i = 0; i < 3; i++)
	{
		equalizeHist(imageRGB[i], imageRGB[i]);
	}
	merge(imageRGB, 3, image);
	//imshow("ֱ��ͼ���⻯ͼ����ǿЧ��", image);
	IplImage *input = &IplImage(image); //ǳ����
	TheImage = cvCloneImage(input);//��
	if (TheImage != NULL)
	{
		ShowImage(TheImage, IDC_STATIC_PIC);

	}
	waitKey();

}


void COCTDlg::On32772()//������˹
{
	// TODO:  �ڴ���������������
	Mat image = imread(route, 1);
	if (image.empty())
	{
		
	}
	//imshow("ԭͼ��", image);
	Mat imageEnhance;
	Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, 0, 5, 0, 0, -1, 0);
	filter2D(image, imageEnhance, CV_8UC3, kernel);
	//imshow("������˹����ͼ����ǿЧ��", imageEnhance);
	IplImage *input = &IplImage(imageEnhance); //ǳ����
	TheImage = cvCloneImage(input);//��
	if (TheImage != NULL)
	{
		ShowImage(TheImage, IDC_STATIC_PIC);

	}
	waitKey();

}


void COCTDlg::On32773()//���ڶ���Log�任��ͼ����ǿ
{
	// TODO:  �ڴ���������������
	Mat image = imread(route, 1);
	Mat imageLog(image.size(), CV_32FC3);
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			imageLog.at<Vec3f>(i, j)[0] = log(1 + image.at<Vec3b>(i, j)[0]);
			imageLog.at<Vec3f>(i, j)[1] = log(1 + image.at<Vec3b>(i, j)[1]);
			imageLog.at<Vec3f>(i, j)[2] = log(1 + image.at<Vec3b>(i, j)[2]);
		}
	}
	//��һ����0~255    
	normalize(imageLog, imageLog, 0, 255, CV_MINMAX);
	//ת����8bitͼ����ʾ    
	convertScaleAbs(imageLog, imageLog);
	//imshow("ԭͼ", image);
	//imshow("����Log�任��ͼ����ǿЧ��", imageLog);
	IplImage *input = &IplImage(imageLog); //ǳ����
	TheImage = cvCloneImage(input);//��
	if (TheImage != NULL)
	{
		ShowImage(TheImage, IDC_STATIC_PIC);

	}
	waitKey();

}


void COCTDlg::On32774()//����٤��任��ͼ����ǿ
{
	// TODO:  �ڴ���������������
	Mat image = imread(route, 1);
	Mat imageGamma(image.size(), CV_32FC3);
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			imageGamma.at<Vec3f>(i, j)[0] = (image.at<Vec3b>(i, j)[0])*(image.at<Vec3b>(i, j)[0])*(image.at<Vec3b>(i, j)[0]);
			imageGamma.at<Vec3f>(i, j)[1] = (image.at<Vec3b>(i, j)[1])*(image.at<Vec3b>(i, j)[1])*(image.at<Vec3b>(i, j)[1]);
			imageGamma.at<Vec3f>(i, j)[2] = (image.at<Vec3b>(i, j)[2])*(image.at<Vec3b>(i, j)[2])*(image.at<Vec3b>(i, j)[2]);
		}
	}
	//��һ����0~255    
	normalize(imageGamma, imageGamma, 0, 255, CV_MINMAX);
	//ת����8bitͼ����ʾ    
	convertScaleAbs(imageGamma, imageGamma);
	//imshow("ԭͼ", image);
	//imshow("٤��任ͼ����ǿЧ��", imageGamma);
	IplImage *input = &IplImage(imageGamma); //ǳ����
	TheImage = cvCloneImage(input);//��
	if (TheImage != NULL)
	{
		ShowImage(TheImage, IDC_STATIC_PIC);

	}
	waitKey();
}


void COCTDlg::On32785()//��ǿ��ԭ
{
	// TODO:  �ڴ���������������
	Mat image = imread(route, 1);
	IplImage *input = &IplImage(image); //ǳ����
	TheImage = cvCloneImage(input);//��
	if (TheImage != NULL)
	{
		ShowImage(TheImage, IDC_STATIC_PIC);

	}
}





void COCTDlg::On32784()//�˳�
{
	// TODO:  �ڴ���������������
	CDialogEx::OnOK();
}


void COCTDlg::On32786()//����ʱ��ͼ��
{
	// TODO:  �ڴ���������������
	CString m_strPicPath;


	CFileDialog dlg(true, _T("*.bmp"), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image file(*.bmp;*.jpg)|*.bmp;*.jpg|All Files(*.*)|*.*|"), NULL);
	//����ѡ��ͼƬ�Ի���
	dlg.m_ofn.lpstrTitle = _T("open Image");
	if (dlg.DoModal() != IDOK)
		return;
	m_strPicPath = dlg.GetPathName();//��ȡͼƬ��·��
	route = m_strPicPath;
	m_strPicPath.Replace(_T("//"), _T("////"));
	TheImage = cvLoadImage((CT2CA)m_strPicPath, 1);
	/*if (TheImage != NULL)
	{
		ShowImage(TheImage, IDC_STATIC_PIC);
		

	}*/
	Mat image = imread(route, 1);
	IplImage *input = &IplImage(image); //ǳ����
	TheImage = cvCloneImage(input);//��
	imshow("����ͼƬ", image);
}


void COCTDlg::On32787()//�ɹ�ͼչʾ
{
	// TODO:  �ڴ���������������
	CString m_strPicPath;


	CFileDialog dlg(true, _T("*.bmp"), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image file(*.bmp;*.jpg)|*.bmp;*.jpg|All Files(*.*)|*.*|"), NULL);
	//����ѡ��ͼƬ�Ի���
	dlg.m_ofn.lpstrTitle = _T("open Image");
	if (dlg.DoModal() != IDOK)
		return;
	m_strPicPath = dlg.GetPathName();//��ȡͼƬ��·��
	route = m_strPicPath;
	m_strPicPath.Replace(_T("//"), _T("////"));
	TheImage = cvLoadImage((CT2CA)m_strPicPath, 1);
	/*if (TheImage != NULL)
	{
	ShowImage(TheImage, IDC_STATIC_PIC);


	}*/
	Mat image = imread(route, 1);
	IplImage *input = &IplImage(image); //ǳ����
	TheImage = cvCloneImage(input);//��
	imshow("�ɹ�ͼ", image);
}


void COCTDlg::On32777()//�鿴����
{
	// TODO:  �ڴ���������������
	ShellExecute(NULL, _T("Open"), _T("�û��ֲ�.docx"), NULL, NULL, SW_SHOW);
}


void COCTDlg::On32779()//ʾ����Ƶ
{
	// TODO:  �ڴ���������������
	ShellExecute(NULL, _T("Open"), _T("��ʾ��Ƶ.mov"), NULL, NULL, SW_SHOW);
}


void COCTDlg::On32781()//�û�����
{
	// TODO:  �ڴ���������������
	CMyFristDlg dlg;
	dlg.DoModal();
}


void COCTDlg::On32778()//����֧�� �����ü��������뻻���ı���ʽ��ȡ
{
	// TODO:  �ڴ���������������
	MessageBox("���������Լ�������\nVisual Studio2010\nSQL Server Management Studio\nMFC\n���ü�����\nOpencv��SVM������Ҷ�任");
}


void COCTDlg::On32780()//����OCT
{
	// TODO:  �ڴ���������������
	MessageBox("��ѧ��ɶϲ�ɨ�裨���OCT������һ��������20����90�������ά����������ԭ�����Գ������������Ϊ��Դ��ͨ����������ķ��䡢ɢ���������ϳ��µĹ�������������ĺ�Ȳ�ͬ��������ǿ�Ⱥ�ʱ���Ӹ�Ҳ���������죬�����γ�ͼ�����ſƼ��ķ�չ, OCT���㷺Ӧ����ҽ����ҵ��");
	CAboutDlg dlg;
	dlg.DoModal();
}




void COCTDlg::On32788()//������Ա
{
	// TODO:  �ڴ���������������
	MessageBox("�㷨��Ա�����ӽ�\nUI��ƣ��ڳ���\n���ݿ�Ӧ�ã��쿡��\n");
}


//void COCTDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
//{
//	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	CDialogEx::OnNcLButtonDown(nHitTest, point);
//}


void COCTDlg::OnLButtonDown(UINT nFlags, CPoint point)//��Ƥ�������� �ƶ����ܼ�ע�� �Ժ��Ż��ɲο�
{
	CRectTracker m_tracker;
	m_tracker.m_nStyle = CRectTracker::resizeInside | CRectTracker::solidLine | CRectTracker::hitNothing;
	int nIn;                         //����һ�����ĵ��ֵ��
	nIn = m_tracker.HitTest(point);       //�����㵽������

	CRect Prect1;          //����ͼƬ�ľ���
	CRect Trect;          //������Ƥ���ľ���
	CRect Prect;         //ͼƬ���ο�
	//CRect AbsRect;
	GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&Prect1);    //�õ�ͼƬ�ľ�//�δ�С

	ScreenToClient(&Prect1);   //��ͼƬ��ľ��Ծ��δ�С

	//�ж��Ƿ���ͼƬ���ڣ���������ͼƬ���ڵĵ��
	if (point.x<Prect1.left || point.x>Prect1.right || point.y<Prect1.top || point.y>Prect1.bottom)
		return;

	m_tracker.SetCursor(this, nFlags); //�ı�������״
	if (nIn<0)                 //�����ı��������ڣ�
	{
		Invalidate(true);
		m_tracker.TrackRubberBand(this, point, false);   //���ھ��ο�������Ƥ///���


		GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&Prect); //�õ�ͼƬ��ľ���
		ScreenToClient(&Prect);
		Trect = m_tracker.m_rect;   //�õ����õ���Ƥ���

		//������С
		Trect.top = (Trect.top<Prect.top ? Prect.top : Trect.top);
		Trect.left = (Trect.left<Prect.left ? Prect.left : Trect.left);
		Trect.bottom = (Trect.bottom>Prect.bottom ? Prect.bottom : Trect.bottom);
		Trect.right = (Trect.right>Prect.right ? Prect.right : Trect.right);

		m_tracker.m_rect.SetRect(Trect.left, Trect.top, Trect.right, Trect.bottom); //���������õľ��ο�
		Trect1 = m_tracker.m_rect;
		CClientDC dc(this);
		m_tracker.Draw(&dc);    //���þ��ο�

	}
	else
		//���ı��������ڣ�
	{
		Invalidate(); //�ػ�����

		CClientDC dc(this);
		GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&Prect);


		ClipCursor(&Prect); //api�����������������ͼƬ����

		m_tracker.Draw(&dc);
		m_tracker.Track(this, point, false);

		ScreenToClient(&Prect);

		Trect = m_tracker.m_rect;   //�õ����õ���Ƥ���

		//�������ο��λ��
		if (Trect.top<Prect.top)
		{//����ͼƬ�򶥲���λ��
			Trect.bottom = Prect.top - Trect.top + Trect.bottom;
			Trect.top = Prect.top;
		}
		if (Trect.bottom>Prect.bottom)
		{//�����ײ���λ��
			Trect.top = Prect.bottom - Trect.bottom + Trect.top;
			Trect.bottom = Prect.bottom;
		}
		if (Trect.right>Prect.right)
		{//�����ұ�
			Trect.left = Prect.right - Trect.right + Trect.left;
			Trect.right = Prect.right;
		}
		if (Trect.left<Prect.left)
		{//�������
			Trect.right = Prect.left - Trect.left + Trect.right;
			Trect.left = Prect.left;
		}
		//���þ��ο��С
		m_tracker.m_rect.SetRect(Trect.left, Trect.top, Trect.right, Trect.bottom);
		Trect1 = m_tracker.m_rect;
		m_tracker.Draw(&dc);
		// Track()��CRectTracker��������ĺ�������ʱʱ�ĸı�����ߵ�m_rect;
		ClipCursor(NULL);   //�ͷŶ���������
	}

	/*AbsRect.left = abs(Trect.left - Prect.left);
	AbsRect.right = abs(Trect.right - Prect.right);
	AbsRect.top = abs(Trect.top - Prect.top);
	AbsRect.bottom = abs(Trect.bottom - Prect.bottom);*/
	

		/*// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		bool IsInRct = false;
		int i = 0;
		do
		{
			if (m_rctTracker[i].HitTest(point)<0)
			{
				IsInRct = false;
			}
			else
			{
				IsInRct = true;
				m_rctChoseNum = i;
				m_rctCurTracker = m_rctTracker[m_rctChoseNum];
				m_IsChose = true;
				break;
			}
			i++;

		} while (i<m_rectNum);
		if (!IsInRct)
		{
			CRectTracker tempRectTracker;
			CRect rect;
			tempRectTracker.TrackRubberBand(this, point);
			tempRectTracker.m_rect.NormalizeRect();
			if (rect.IntersectRect(tempRectTracker.m_rect, m_rctCurTracker.m_rect))
				m_IsChose = TRUE;
			else
			{
				m_IsChose = false;
				if (m_IsDraw)
				{
					//m_IsChose=FALSE;
					m_rctTracker[m_rectNum].m_rect = tempRectTracker.m_rect;
					m_rctCurTracker.m_rect = m_rctTracker[m_rectNum].m_rect;
					CClientDC dc(this);
					m_rctCurTracker.Draw(&dc);
					//ע�⣡��������һ��Ҫ���û��Ʊ߿�ĳ��򣬷���ƾonpaint�л��ƣ�������ʾ����
								m_rctChoseNum = m_rectNum;
					m_rectNum++;
					if (m_rectNum >= MAX_RECT_NUM)
					{
						m_rectNum = MAX_RECT_NUM;
						MessageBox( "�ѻ����γ������ޣ������ٻ���������");
					}
					m_IsChose = TRUE;
					m_IsDraw = false;

					Invalidate();
				}

			}


			Invalidate();
		}
		else
		{
			CClientDC dc(this);
			m_rctCurTracker.Draw(&dc);
			m_rctCurTracker.Track(this, point);
			m_rctCurTracker.m_rect.NormalizeRect();
			m_rctTracker[m_rctChoseNum] = m_rctCurTracker;
			m_IsChose = TRUE;

			Invalidate();
		}*/
		

	    CDialogEx::OnLButtonDown(nFlags, point);
}



/*BOOL COCTDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (pWnd == this && m_rctCurTracker.SetCursor(this, nHitTest))
		       return TRUE;



	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


BOOL COCTDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
	switch (pMsg->wParam)
     {
		case VK_LEFT:
		dirct = 1;
		break;
		case VK_RIGHT:
		 dirct = 2;
		 break;
		case VK_UP:
		dirct = 3;
	    break;
	     case VK_DOWN:
	      dirct = 4;
	      break;
	       case VK_DELETE:
		  dirct = 5;
		  break;
		   default:
		  dirct = 0;
	   }
    }

   ChangeRectPt(dirct);

	return CDialogEx::PreTranslateMessage(pMsg);
}
void COCTDlg::ChangeRectPt(int ChangeDirct)
{
	CRect rct;
	rct = m_rctCurTracker.m_rect;
	switch (ChangeDirct)
	{
	case 1://����
		rct.TopLeft().x -= m_FlaMoveStep;
		rct.BottomRight().x -= m_FlaMoveStep;
		break;
	case 2://����
		rct.TopLeft().x += m_FlaMoveStep;
		rct.BottomRight().x += m_FlaMoveStep;
		break;
	case 3://����
		rct.TopLeft().y -= m_FlaMoveStep;
		rct.BottomRight().y -= m_FlaMoveStep;
		break;
	case 4://����
		rct.TopLeft().y += m_FlaMoveStep;
		rct.BottomRight().y += m_FlaMoveStep;
		break;
	case 5://ɾ��
		m_rctCurTracker.m_rect.SetRect(0, 0, 0, 0);
		m_rctTracker[m_rctChoseNum] = m_rctCurTracker;
		dirct = 0;
		Invalidate();
		return;
	}
	m_rctCurTracker.m_rect.SetRect(rct.TopLeft(), rct.BottomRight());
	m_rctTracker[m_rctChoseNum] = m_rctCurTracker;
	if (ChangeDirct != 0)
	{
		Invalidate();
	}
	dirct = 0;

}


void COCTDlg::On32790()//ѡ��
{
	// TODO:  �ڴ���������������
	m_IsDraw = true;
}*/

/*m_nHandleSize ȷ��������С���ֵĳߴ�
m_rect ���ε������ر�ʾ�ĵ�ǰλ��
m_sizeMin ȷ�����ο�Ⱥ͸߶ȵ���Сֵ
m_nStyle �������ĵ�ǰ���
����
CRectTracker ����һ��CRectTracker����
����
Draw ��ʾ���Σ�����ʾ�˸��������֣�����ʱ��Ҫ������ʾ�������֣�����ֻ�����ػ溯���е��ã�����Ҫ�ڳ���ǰλ�õ���һ�Σ�
GetTrueRect ���ؾ��εĿ�Ⱥ͸߶ȣ������ı��С���
HitTest ������CRectTracker��������Ĺ��ĵ�ǰλ��(����ֵ >= 0���ھ����ڣ�С��0��������)
NormalizeHit �淶��һ���������Դ���
SetCursor ���ݹ���ھ����Ϸ���λ�������ù��
Track ֧���û��������Σ��Ჶ׽������������Ϣ��һֱ������������Ż�ִ��������ĳ���˫�������������ᵱ����ӦTrack��ϣ���ִ��һ���������һ�Σ��������һ��
TrackRubberBand ֧���û�����Ƥ��Ƶ�����ѡ�񣨴˺�����Ҫ������ȡ���Σ�����ѡ�������ڵľ�������
AdjustRect �����α��ı��Сʱ�˺���������
DrawTrackerRect ����һ��CRectTracker����ı߿�ʱ�˺���������
OnChangedRect �����α��ı��С���ƶ�ʱ���˺���������
GetHandleMask ���ô˺��������һ��CRectTracker��ĵ�����С���ֵ�����*/


void COCTDlg::On32791()//�Ŵ�
{
	// TODO:  �ڴ���������������
/*Mat img = cvarrToMat(TheImage, true);
	//roi �Ǳ�ʾ img �� Rect(10,10,100,100)����Ķ���
	Mat roi = img(Range(Trect1.left, Trect1.right), Range(Trect1.top, Trect1.bottom));
	IplImage im = IplImage(roi);
	IplImage *input = &IplImage(im); //ǳ����
	TheImage = cvCloneImage(input);//��
	ShowImage(TheImage, IDC_STATIC_PIC);*/


}

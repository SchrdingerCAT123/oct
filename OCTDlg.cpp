
// OCTDlg.cpp : 实现文件
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
//图像处理部分的参数
Mat srcImage;
Mat  m_srcImage;
CRect Trect1;          //定义橡皮筋框的矩形
IplImage* TheImage;

cv::String route;
int total_picture = 1;
int coor[100];//存入坐标
int total = 0;//斑块总数

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/***************************************************************************************************/
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
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
	param.gamma = 2.2;//0.5(69%) 1.2,1.4,1.7(备选),1.7
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

// 对话框数据
	
	 
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// COCTDlg 对话框



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


// COCTDlg 消息处理程序

BOOL COCTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	ShowWindow(SW_MAXIMIZE);

	GetClientRect(&m_rect); //获取对话框的大小
	//表格   
	m_programLangList.GetClientRect(&m_rect);

	// 为列表视图控件添加全行选中和栅格风格   
	m_programLangList.SetExtendedStyle(m_programLangList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 为列表视图控件添加三列   
	m_programLangList.InsertColumn(0, _T("样本编号"), LVCFMT_CENTER, m_rect.Width() / 3, 0);
	m_programLangList.InsertColumn(1, _T("易损斑块数量"), LVCFMT_CENTER, m_rect.Width() / 3, 1);
	m_programLangList.InsertColumn(2, _T("斑块坐标"), LVCFMT_CENTER, m_rect.Width() / 3, 2);
	
	

	
	// TODO:  在此添加额外的初始化代码


	

	m_leadbtn.ModifyStyle(0, BS_OWNERDRAW, 0);
	//设置Button Down的背景色
	m_leadbtn.SetDownColor(RGB(255, 0, 0));
	//设置Button Up的背景色
	m_leadbtn.SetUpColor(RGB(255,222,246));

	m_trainstb.ModifyStyle(0, BS_OWNERDRAW, 0);
	//设置Button Down的背景色
	m_trainstb.SetDownColor(RGB(255, 0, 0));
	//设置Button Up的背景色
	m_trainstb.SetUpColor(RGB(255, 222, 246));

	m_labelbtn.ModifyStyle(0, BS_OWNERDRAW, 0);
	//设置Button Down的背景色
	m_labelbtn.SetDownColor(RGB(255, 0, 0));
	//设置Button Up的背景色
	m_labelbtn.SetUpColor(RGB(255, 235, 244));

	m_showbtn.ModifyStyle(0, BS_OWNERDRAW, 0);
	//设置Button Down的背景色
	m_showbtn.SetDownColor(RGB(255, 0, 0));
	//设置Button Up的背景色
	m_showbtn.SetUpColor(RGB(255, 198, 199));

	m_exitbtn.ModifyStyle(0, BS_OWNERDRAW, 0);
	//设置Button Down的背景色
	m_exitbtn.SetDownColor(RGB(255, 0, 0));
	//设置Button Up的背景色
	m_exitbtn.SetUpColor(RGB(230, 151, 157));
	
	/*
	   m_rctCurTracker.m_rect.SetRect(0, 0, 0, 0);//设置矩形区域大小  
	   m_rctCurTracker.m_nStyle = CRectTracker::dottedLine | CRectTracker::resizeInside;
	   m_rctCurTracker.m_nHandleSize = 6;
	   for (int i = 0; i<MAX_RECT_NUM; i++)
	    {
	        m_rctTracker[i].m_rect.SetRect(0, 0, 0, 0);//设置矩形区域大小  
	        m_rctTracker[i].m_nStyle = CRectTracker::dottedLine | CRectTracker::resizeInside;
	        m_rctTracker[i].m_nHandleSize = 6;
	    }
	
	    m_IsChose = FALSE;//表示未选中  
	    m_IsDraw = false;
	    m_rectNum = 0;
	    m_rctChoseNum = 0;
	    m_FlaMoveStep = 2;
	    dirct = 0;
	*/


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void COCTDlg::LIST()
{
	

	// 在列表视图控件中插入列表项，并设置列表子项文本 
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COCTDlg::OnPaint()//背景
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialogEx::OnPaint();
	    CPaintDC   dc(this);
		CRect   rect;
		GetClientRect(&rect);    //获取对话框长宽
		CDC   dcBmp;              //定义并创建一个内存设备环境
		dcBmp.CreateCompatibleDC(&dc);             //创建兼容性DC
		CBitmap   bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP1);    //载入资源中图片
		BITMAP   m_bitmap;                         //图片变量
		bmpBackground.GetBitmap(&m_bitmap);       //将图片载入位图中
		//将位图选入临时内存设备环境
		CBitmap  *pbmpOld = dcBmp.SelectObject(&bmpBackground);
		//调用函数显示图片StretchBlt显示形状可变
		dc.SetStretchBltMode(HALFTONE);
		dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcBmp, 0, 0, m_bitmap.bmWidth, m_bitmap.bmHeight, SRCCOPY);
		
		

		 /* if (m_IsChose)
		         m_rctCurTracker.Draw(&dc);//若选择了该区域，则显示边框以及8个调整点  
		  CPen pen(PS_SOLID, 1, RGB(100, 255, 200));
		  dc.SelectObject(&pen);
		
		  CBrush *pbrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		  dc.SelectObject(pbrush);
		 CRect m_rect;
		   m_rctCurTracker.GetTrueRect(&m_rect);//得到矩形区域的大小  
		   dc.Rectangle(&m_rect);//画出矩形  
		   CSize rct_size;
		  for (int i = 0; i<MAX_RECT_NUM; i++)
		   {
		     m_rctTracker[i].GetTrueRect(&m_rect);//得到矩形区域的大小  
		     rct_size = m_rctTracker[i].m_rect.Size();
		       if (rct_size.cx * rct_size.cy == 0 || i == m_rctChoseNum)
		       {
		        continue;
		       }
		      dc.Rectangle(&m_rect);//画出矩形  
		   }*/
		                

	}
	if (TheImage != NULL)
	{
		ShowImage(TheImage, IDC_STATIC_PIC);

	}
	
	
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COCTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void COCTDlg::OnBnClickedButton1()//导入按钮
{
	CString m_strPicPath;
	

	CFileDialog dlg(true, _T("*.bmp"), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image file(*.bmp;*.jpg)|*.bmp;*.jpg|All Files(*.*)|*.*|"), NULL);
	//弹出选择图片对话框
	dlg.m_ofn.lpstrTitle = _T("open Image");
	if (dlg.DoModal() != IDOK)
		return;
	m_strPicPath = dlg.GetPathName();//获取图片的路径
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
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (MessageBox(_T("确定退出吗?"), _T("提示"), MB_YESNO | MB_ICONWARNING) == IDNO)
		return;

	CDialogEx::OnClose();
}


void COCTDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	

	// TODO:  在此处添加消息处理程序代码
	if (0 == m_rect.left && 0 == m_rect.right && 0 == m_rect.top && 0 == m_rect.bottom)//第一次启动对话框时的大小变化不做处理  
		{
		}
	else
	{

		if (0 == cx && 0 == cy)//如果是按下了最小化，则触发条件，这时不保存对话框数据  
		{
			return;
		}
		CRect rectDlgChangeSize;
		GetClientRect(&rectDlgChangeSize);//存储对话框大小改变后对话框大小数据  

		repaint(IDC_BUTTON1, m_rect.Width(), rectDlgChangeSize.Width(), m_rect.Height(), rectDlgChangeSize.Height());//重绘函数，用以更新对话框上控件的位置和大小 
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
	Invalidate();//更新窗口 
	

	

}


void COCTDlg::repaint(UINT ID, int last_Width, int now_Width, int last_Height, int now_Height)//更新控件位置和大小函数 
{
	CRect rect;
	CWnd *wnd = NULL;
	wnd = GetDlgItem(ID);


	if (NULL == wnd)
	{
		// MessageBox(_T("相应控件不存在"));
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
	// TODO:  在此添加控件通知处理程序代码
	
}


void COCTDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}





HBRUSH COCTDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	CFont font;
	LOGFONT lf;
	if (!pDC)
		return NULL;
	HBRUSH hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	// TODO:  在此更改 DC 的任何特性
	pDC->SetBkMode(TRANSPARENT);
	//设置文字的颜色为白色    
	pDC->SetTextColor(RGB(0, 0, 0));
	::ZeroMemory(&lf, sizeof(lf));
	//设置逻辑字体的高度  
	lf.lfHeight = 20;
	//设置逻辑字体为粗体字    
	lf.lfWeight = FW_BOLD;
	//设置字体为宋体    
	::lstrcpy(lf.lfFaceName, _T("宋体"));
	//用逻辑字体创建一个CFont对象    
	font.CreateFontIndirect(&lf);
	//应用字体    
	pDC->SelectObject(&font);

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void COCTDlg::OnBnClickedButton3()//影像处理
{
	// TODO:  在此添加控件通知处理程序代码

	BeginWaitCursor();
	ostringstream oss;
	ostringstream ss;
	int m, q = 0;
	oss << format("%04d", total_picture);
	regionExtraction(0, 46, 720, 306, route , "D:\\OCT\\pic\\Go to dry pic\\" + oss.str() + ".png");//去燥
	srcImage(Rect(0, 0, 720, 352)).copyTo(m_srcImage);
	imwrite("D:\\OCT\\pic\\signpic\\" + oss.str() + ".png", m_srcImage);

	//first_picture++;
	srcImage = imread("D:\\OCT\\pic\\Go to dry pic\\" + oss.str() + ".png");
	
	m = srcImage.cols / 5; //一个图像分割的次数
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
	MessageBox("影像处理完成！");
	

}

void COCTDlg::regionExtraction(int xRoi, int yRoi, int widthRoi, int heightRoi, String path1, String path2)//去燥
{        //path1为需要处理的图片地址，path2为处理后存放的地址
	//选择了TrainingData里面的1号图
	srcImage = imread(path1);                                                      //这里后面改成所选图片的地址！！！
	Mat roiImage(srcImage.rows, srcImage.cols, CV_8UC3);
	srcImage(Rect(xRoi, yRoi, widthRoi, heightRoi)).copyTo(roiImage);
	imwrite(path2, roiImage);
	//std::cout << roiImage.rows << " " << roiImage.cols << std::endl;
}
Mat COCTDlg::fft2(String s, String mm,int num)
{
	String ID;
	Mat I = imread("D:\\OCT\\pic\\five pixel\\" + s + ".png", CV_LOAD_IMAGE_GRAYSCALE);      //读入的图片路径

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
	//outf.open("C:\\Users\\52983\\Desktop\\" + mm + ".txt");               //存入的文本
	for (int r = 0; r < magI.rows; r++)
	{
		for (int c = 0; c < magI.cols; c++)
		{
			if (r<100 || r>=220)
			{
				float data = magI.at<float>(r, c);  //读取数据，at<type> - type 是矩阵元素的具体数据格式  
				ostringstream ff;
				ff.str("");
				ff << format("%.5f", data); //保留五位小数
				outf << ff.str() << " ";   //每列数据用 tab 隔开  
				strdata = strdata + " " + ff.str();//将浮点数传入strdata
			}

		}
		outf << endl;  //换行  

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

	/* 初始化数组 */
	for (i = 0; i<n; i++)
		buffer[i] = 0;

	/* 准备生成随机数,以当前时间为种子 */
	srand((unsigned)time(NULL));

	/* 获得不重复的随机数据 */
	for (i = 0; i<n; i++) {
		/* 获得0~（n-i）的随机数据 */
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
	const int Num = 6000;//读取个数
	string StringValue;	 //充当Temp
	_bstr_t  strSql = "SELECT * FROM tb_PositiveFeatures";//选择表的SQL语句 
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
		res = (_bstr_t)con.m_pRecordset->GetFields()->GetItem("data")->Value;   //数据
		//Name = (_bstr_t)con.m_pRecordset->GetFields()->GetItem("txtName")->Value;//文件名
		svm_node* features = new svm_node[FEATUREDIM + 1];
		istringstream is(res);//按照空格提取Value
		while (is >> StringValue)
		{
			double value = 0;
			features[k].index = k + 1;//特征标号，从1开始  
			features[k].value = stringtodouble(StringValue);//特征值  
			k++;
		}
		features[FEATUREDIM].index = -1;//结束标记  
		//negative sample type is 0  
		int type = 0;
		//positive sample type is 1  
		if (SqlTableName == "PositiveFeatures")                //表名
			type = 1;
		dataList.push_back(features);
		typeList.push_back(type);
		sampleNum++;
		con.m_pRecordset->MoveNext();
		//cout << "sql done" << endl;
		


	}


	/*int ID[5999]; //存储随机数的数组
	permutation(5999, ID); //调用产生随机数的功能函数
	int Num =500;// 定义读取个数
	
	for (size_t i = 1; i <= Num; i++)
	{
		string StringValue;	 //充当Temp		
		String CstrID = to_string(ID[i] + 1);//int->String转换形式
		_bstr_t strSql = "";

		if (SqlTableName == "PositiveFeatures")
		{
			String CstrSql = "SELECT * FROM tb_PositiveFeatures Where sign=0 and ID=";
			CstrSql = CstrSql += CstrID;
			char* charSource = (char*)CstrSql.c_str(); //得到sql指令语句
			strSql = charSource;
		}
		if (SqlTableName == "NegativeFeatures")
		{
			String CstrSql = "SELECT * FROM tb_NegativeFeatures1 Where sign=0 and ID=";
			CstrSql = CstrSql += CstrID;
			char* charSource = (char*)CstrSql.c_str(); //得到sql指令语句
			strSql = charSource;
		}
		ADOConn con;
		con.OnInitADOConn();//初始化sql连接
		con.ExecuteSQL(strSql);//执行command语句
		
		int k = 0;
		string res;
		string Name;
	    string sign = "0";//是否被读取的标识
		_variant_t ra;
		//cout << "Sql has opened...." << endl;
		res = (_bstr_t)con.m_pRecordset->GetFields()->GetItem("data")->Value;   //数据
		//Name = (_bstr_t)con.m_pRecordset->GetFields()->GetItem("txtName")->Value;//文件名
		sign = (_bstr_t)con.m_pRecordset->GetFields()->GetItem("sign")->Value;//获得标识
		//con.m_pRecordset->MoveNext();
		if (SqlTableName == "PositiveFeatures")
		{
			String CstrSql = "UPDATE tb_PositiveFeatures SET sign=1 WHERE  ID=";
			CstrSql = CstrSql += CstrID;
			char* charSource = (char*)CstrSql.c_str();
			strSql = charSource;
			con.m_pConnection->Execute(strSql, &ra, adCmdText); //将读取过的数据的标识置1
		}
		if (SqlTableName == "NegativeFeatures")
		{
			String CstrSql = "UPDATE tb_NegativeFeatures1 SET sign=1 WHERE  ID=";
			CstrSql = CstrSql += CstrID;
			char* charSource = (char*)CstrSql.c_str();
			strSql = charSource;
			con.m_pConnection->Execute(strSql, &ra, adCmdText);
		}


		svm_node* features = new svm_node[FEATUREDIM + 1];//因为需要结束标记，因此申请空间时特征维度+1  		

		istringstream is(res);//按照空格提取Value
		while (is >> StringValue)
		{
			double value = 0;
			features[k].index = k + 1;//特征标号，从1开始  
			features[k].value = stringtodouble(StringValue);//特征值  
			k++;
		}
		features[FEATUREDIM].index = -1;//结束标记  
		//negative sample type is 0  
		int type = 0;
		//positive sample type is 1  
		if (SqlTableName == "PositiveFeatures")                //表名
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
	con.ExecuteSQL(strSql);    */        //标识置0，方便下次训练
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


void COCTDlg::OnBnClickedButton2()//训练按钮调用
{
	BeginWaitCursor();
	ClassificationSVM a; 
	SetTimer(1, 1000, NULL);
	int ret = MessageBox("训练中···", "MyMessageBox");
	a.train("result.txt");
	EndWaitCursor();
	MessageBox("训练完成！");
	

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
	const int Num = 144;//读取个数
	string StringValue;	 //充当Temp
	_bstr_t  strSql = "SELECT * FROM tb_Training";//选择表的SQL语句 
	ADOConn con;
	con.OnInitADOConn();
	con.ExecuteSQL(strSql);
	for (size_t i = 0; i < Num; i++)
	{

		int k = 0;
		string res;
		string Name;
		//cout << "Sql has opened...." << endl;
		res = (_bstr_t)con.m_pRecordset->GetFields()->GetItem("data")->Value;   //数据
		//Name = (_bstr_t)con.m_pRecordset->GetFields()->GetItem("txtName")->Value;//文件名
		svm_node* input = new svm_node[FEATUREDIM + 1];
		istringstream is(res);//按照空格提取Value
		while (is >> StringValue)
		{
			double value = 0;
			input[k].index = k + 1;//特征标号，从1开始  
			input[k].value = stringtodouble(StringValue);//特征值  
			k++;
		}
		input[FEATUREDIM].index = -1;//结束标记  

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
		if (judgeRight[i] == false){                        //如果扫描到正斑块
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
		else	if (flag == 0 && judgeRight[i] == true){                 //如果扫描到负斑块
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


void COCTDlg::OnBnClickedButton4()//标记斑块
{
	BeginWaitCursor();
	ClassificationSVM b;
	ostringstream oss;
	oss << format("%04d", total_picture);
	b.predict("negative_test", "result.txt");
	
	srcImage = imread("D:\\OCT\\pic\\signpic\\" +oss.str()+ ".png");
	IplImage *input = &IplImage(srcImage); //浅拷贝
	TheImage = cvCloneImage(input);//深

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
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		HANDLE hWnd = ::FindWindowEx(NULL, NULL, NULL,  "MyMessageBox");
		//HANDLE hWnd = ::GetForegroundWindow();  
		if (hWnd)
		{
			TRACE("发现了MyMessageBox窗口\n");
			::SendMessage((HWND)hWnd, WM_CLOSE, NULL, NULL);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void COCTDlg::On32771()//直方图均衡化
{
	// TODO:  在此添加命令处理程序代码
	Mat image = imread(route, 1);
	if (image.empty())
	{
		
	}
	//imshow("原图像", image);
	Mat imageRGB[3];
	split(image, imageRGB);
	for (int i = 0; i < 3; i++)
	{
		equalizeHist(imageRGB[i], imageRGB[i]);
	}
	merge(imageRGB, 3, image);
	//imshow("直方图均衡化图像增强效果", image);
	IplImage *input = &IplImage(image); //浅拷贝
	TheImage = cvCloneImage(input);//深
	if (TheImage != NULL)
	{
		ShowImage(TheImage, IDC_STATIC_PIC);

	}
	waitKey();

}


void COCTDlg::On32772()//拉普拉斯
{
	// TODO:  在此添加命令处理程序代码
	Mat image = imread(route, 1);
	if (image.empty())
	{
		
	}
	//imshow("原图像", image);
	Mat imageEnhance;
	Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, 0, 5, 0, 0, -1, 0);
	filter2D(image, imageEnhance, CV_8UC3, kernel);
	//imshow("拉普拉斯算子图像增强效果", imageEnhance);
	IplImage *input = &IplImage(imageEnhance); //浅拷贝
	TheImage = cvCloneImage(input);//深
	if (TheImage != NULL)
	{
		ShowImage(TheImage, IDC_STATIC_PIC);

	}
	waitKey();

}


void COCTDlg::On32773()//基于对数Log变换的图像增强
{
	// TODO:  在此添加命令处理程序代码
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
	//归一化到0~255    
	normalize(imageLog, imageLog, 0, 255, CV_MINMAX);
	//转换成8bit图像显示    
	convertScaleAbs(imageLog, imageLog);
	//imshow("原图", image);
	//imshow("对数Log变换的图像增强效果", imageLog);
	IplImage *input = &IplImage(imageLog); //浅拷贝
	TheImage = cvCloneImage(input);//深
	if (TheImage != NULL)
	{
		ShowImage(TheImage, IDC_STATIC_PIC);

	}
	waitKey();

}


void COCTDlg::On32774()//基于伽马变换的图像增强
{
	// TODO:  在此添加命令处理程序代码
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
	//归一化到0~255    
	normalize(imageGamma, imageGamma, 0, 255, CV_MINMAX);
	//转换成8bit图像显示    
	convertScaleAbs(imageGamma, imageGamma);
	//imshow("原图", image);
	//imshow("伽马变换图像增强效果", imageGamma);
	IplImage *input = &IplImage(imageGamma); //浅拷贝
	TheImage = cvCloneImage(input);//深
	if (TheImage != NULL)
	{
		ShowImage(TheImage, IDC_STATIC_PIC);

	}
	waitKey();
}


void COCTDlg::On32785()//增强还原
{
	// TODO:  在此添加命令处理程序代码
	Mat image = imread(route, 1);
	IplImage *input = &IplImage(image); //浅拷贝
	TheImage = cvCloneImage(input);//深
	if (TheImage != NULL)
	{
		ShowImage(TheImage, IDC_STATIC_PIC);

	}
}





void COCTDlg::On32784()//退出
{
	// TODO:  在此添加命令处理程序代码
	CDialogEx::OnOK();
}


void COCTDlg::On32786()//处理时的图像
{
	// TODO:  在此添加命令处理程序代码
	CString m_strPicPath;


	CFileDialog dlg(true, _T("*.bmp"), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image file(*.bmp;*.jpg)|*.bmp;*.jpg|All Files(*.*)|*.*|"), NULL);
	//弹出选择图片对话框
	dlg.m_ofn.lpstrTitle = _T("open Image");
	if (dlg.DoModal() != IDOK)
		return;
	m_strPicPath = dlg.GetPathName();//获取图片的路径
	route = m_strPicPath;
	m_strPicPath.Replace(_T("//"), _T("////"));
	TheImage = cvLoadImage((CT2CA)m_strPicPath, 1);
	/*if (TheImage != NULL)
	{
		ShowImage(TheImage, IDC_STATIC_PIC);
		

	}*/
	Mat image = imread(route, 1);
	IplImage *input = &IplImage(image); //浅拷贝
	TheImage = cvCloneImage(input);//深
	imshow("处理图片", image);
}


void COCTDlg::On32787()//成果图展示
{
	// TODO:  在此添加命令处理程序代码
	CString m_strPicPath;


	CFileDialog dlg(true, _T("*.bmp"), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image file(*.bmp;*.jpg)|*.bmp;*.jpg|All Files(*.*)|*.*|"), NULL);
	//弹出选择图片对话框
	dlg.m_ofn.lpstrTitle = _T("open Image");
	if (dlg.DoModal() != IDOK)
		return;
	m_strPicPath = dlg.GetPathName();//获取图片的路径
	route = m_strPicPath;
	m_strPicPath.Replace(_T("//"), _T("////"));
	TheImage = cvLoadImage((CT2CA)m_strPicPath, 1);
	/*if (TheImage != NULL)
	{
	ShowImage(TheImage, IDC_STATIC_PIC);


	}*/
	Mat image = imread(route, 1);
	IplImage *input = &IplImage(image); //浅拷贝
	TheImage = cvCloneImage(input);//深
	imshow("成果图", image);
}


void COCTDlg::On32777()//查看帮助
{
	// TODO:  在此添加命令处理程序代码
	ShellExecute(NULL, _T("Open"), _T("用户手册.docx"), NULL, NULL, SW_SHOW);
}


void COCTDlg::On32779()//示例视频
{
	// TODO:  在此添加命令处理程序代码
	ShellExecute(NULL, _T("Open"), _T("演示视频.mov"), NULL, NULL, SW_SHOW);
}


void COCTDlg::On32781()//用户反馈
{
	// TODO:  在此添加命令处理程序代码
	CMyFristDlg dlg;
	dlg.DoModal();
}


void COCTDlg::On32778()//技术支持 如运用技术过多请换成文本形式读取
{
	// TODO:  在此添加命令处理程序代码
	MessageBox("开发工具以及环境：\nVisual Studio2010\nSQL Server Management Studio\nMFC\n运用技术：\nOpencv、SVM、傅里叶变换");
}


void COCTDlg::On32780()//关于OCT
{
	// TODO:  在此添加命令处理程序代码
	MessageBox("光学相干断层扫描（简称OCT），是一种兴起于20世纪90年代的三维成像技术。其原理是以超发光二极管作为光源，通过光在物体的反射、散射等重新组合成新的光束，由于物体的厚度不同，光束的强度和时间延搁也会有所差异，进而形成图像。随着科技的发展, OCT正广泛应用于医疗行业。");
	CAboutDlg dlg;
	dlg.DoModal();
}




void COCTDlg::On32788()//技术人员
{
	// TODO:  在此添加命令处理程序代码
	MessageBox("算法人员：冯子健\nUI设计：贲成阳\n数据库应用：朱俊杰\n");
}


//void COCTDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
//{
//	// TODO:  在此添加消息处理程序代码和/或调用默认值
//
//	CDialogEx::OnNcLButtonDown(nHitTest, point);
//}


void COCTDlg::OnLButtonDown(UINT nFlags, CPoint point)//橡皮筋初步完成 移动功能见注释 以后优化可参考
{
	CRectTracker m_tracker;
	m_tracker.m_nStyle = CRectTracker::resizeInside | CRectTracker::solidLine | CRectTracker::hitNothing;
	int nIn;                         //定义一个鼠标的点击值；
	nIn = m_tracker.HitTest(point);       //看看点到了哪了

	CRect Prect1;          //定义图片的矩形
	CRect Trect;          //定义橡皮筋框的矩形
	CRect Prect;         //图片矩形框
	//CRect AbsRect;
	GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&Prect1);    //得到图片的矩//形大小

	ScreenToClient(&Prect1);   //将图片框的绝对矩形大小

	//判断是否在图片框内，不处理不在图片框内的点击
	if (point.x<Prect1.left || point.x>Prect1.right || point.y<Prect1.top || point.y>Prect1.bottom)
		return;

	m_tracker.SetCursor(this, nFlags); //改变鼠标的形状
	if (nIn<0)                 //不在四边形区域内；
	{
		Invalidate(true);
		m_tracker.TrackRubberBand(this, point, false);   //不在矩形框内则画橡皮///筋框


		GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&Prect); //得到图片框的矩形
		ScreenToClient(&Prect);
		Trect = m_tracker.m_rect;   //得到画好的橡皮筋框

		//调整大小
		Trect.top = (Trect.top<Prect.top ? Prect.top : Trect.top);
		Trect.left = (Trect.left<Prect.left ? Prect.left : Trect.left);
		Trect.bottom = (Trect.bottom>Prect.bottom ? Prect.bottom : Trect.bottom);
		Trect.right = (Trect.right>Prect.right ? Prect.right : Trect.right);

		m_tracker.m_rect.SetRect(Trect.left, Trect.top, Trect.right, Trect.bottom); //画出调整好的矩形框
		Trect1 = m_tracker.m_rect;
		CClientDC dc(this);
		m_tracker.Draw(&dc);    //画好矩形框

	}
	else
		//在四边形区域内：
	{
		Invalidate(); //重画界面

		CClientDC dc(this);
		GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&Prect);


		ClipCursor(&Prect); //api函数，将鼠标限制在图片框内

		m_tracker.Draw(&dc);
		m_tracker.Track(this, point, false);

		ScreenToClient(&Prect);

		Trect = m_tracker.m_rect;   //得到画好的橡皮筋框

		//调整矩形框的位置
		if (Trect.top<Prect.top)
		{//超出图片框顶部的位置
			Trect.bottom = Prect.top - Trect.top + Trect.bottom;
			Trect.top = Prect.top;
		}
		if (Trect.bottom>Prect.bottom)
		{//超出底部的位置
			Trect.top = Prect.bottom - Trect.bottom + Trect.top;
			Trect.bottom = Prect.bottom;
		}
		if (Trect.right>Prect.right)
		{//超出右边
			Trect.left = Prect.right - Trect.right + Trect.left;
			Trect.right = Prect.right;
		}
		if (Trect.left<Prect.left)
		{//超出左边
			Trect.right = Prect.left - Trect.left + Trect.right;
			Trect.left = Prect.left;
		}
		//设置矩形框大小
		m_tracker.m_rect.SetRect(Trect.left, Trect.top, Trect.right, Trect.bottom);
		Trect1 = m_tracker.m_rect;
		m_tracker.Draw(&dc);
		// Track()是CRectTracker中最富魅力的函数。它时时的改变调用者的m_rect;
		ClipCursor(NULL);   //释放对鼠标的限制
	}

	/*AbsRect.left = abs(Trect.left - Prect.left);
	AbsRect.right = abs(Trect.right - Prect.right);
	AbsRect.top = abs(Trect.top - Prect.top);
	AbsRect.bottom = abs(Trect.bottom - Prect.bottom);*/
	

		/*// TODO: 在此添加消息处理程序代码和/或调用默认值
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
					//注意！！在这里一定要调用绘制边框的程序，否则单凭onpaint中绘制，不能显示出来
								m_rctChoseNum = m_rectNum;
					m_rectNum++;
					if (m_rectNum >= MAX_RECT_NUM)
					{
						m_rectNum = MAX_RECT_NUM;
						MessageBox( "已画矩形超过上限，不能再画矩形区域");
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
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (pWnd == this && m_rctCurTracker.SetCursor(this, nHitTest))
		       return TRUE;



	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


BOOL COCTDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
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
	case 1://左移
		rct.TopLeft().x -= m_FlaMoveStep;
		rct.BottomRight().x -= m_FlaMoveStep;
		break;
	case 2://右移
		rct.TopLeft().x += m_FlaMoveStep;
		rct.BottomRight().x += m_FlaMoveStep;
		break;
	case 3://下移
		rct.TopLeft().y -= m_FlaMoveStep;
		rct.BottomRight().y -= m_FlaMoveStep;
		break;
	case 4://上移
		rct.TopLeft().y += m_FlaMoveStep;
		rct.BottomRight().y += m_FlaMoveStep;
		break;
	case 5://删除
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


void COCTDlg::On32790()//选中
{
	// TODO:  在此添加命令处理程序代码
	m_IsDraw = true;
}*/

/*m_nHandleSize 确定调整大小把手的尺寸
m_rect 矩形的以像素表示的当前位置
m_sizeMin 确定矩形宽度和高度的最小值
m_nStyle 跟踪器的当前风格
构造
CRectTracker 构造一个CRectTracker对象
操作
Draw 显示矩形，并显示八个调整把手（调用时若要正常显示调整把手，不能只是在重绘函数中调用，还需要在程序当前位置调用一次）
GetTrueRect 返回矩形的宽度和高度，包括改变大小句柄
HitTest 返回与CRectTracker对象关联的光标的当前位置(返回值 >= 0：在矩形内，小于0：矩形外)
NormalizeHit 规范化一个单击测试代码
SetCursor 根据光标在矩形上方的位置来设置光标
Track 支持用户操作矩形（会捕捉鼠标左键弹起消息，一直到鼠标左键弹起才会执行其下面的程序；双击鼠标左键，将会当做响应Track完毕，并执行一次左键按下一次，左键弹起一起）
TrackRubberBand 支持用户“橡皮筋”似的拉伸选择（此函数主要用于拉取矩形，用于选择区域内的矩形区域）
AdjustRect 当矩形被改变大小时此函数被调用
DrawTrackerRect 当画一个CRectTracker对象的边框时此函数被调用
OnChangedRect 当矩形被改变大小或被移动时，此函数被调用
GetHandleMask 调用此函数来获得一个CRectTracker项的调整大小把手的掩码*/


void COCTDlg::On32791()//放大
{
	// TODO:  在此添加命令处理程序代码
/*Mat img = cvarrToMat(TheImage, true);
	//roi 是表示 img 中 Rect(10,10,100,100)区域的对象
	Mat roi = img(Range(Trect1.left, Trect1.right), Range(Trect1.top, Trect1.bottom));
	IplImage im = IplImage(roi);
	IplImage *input = &IplImage(im); //浅拷贝
	TheImage = cvCloneImage(input);//深
	ShowImage(TheImage, IDC_STATIC_PIC);*/


}

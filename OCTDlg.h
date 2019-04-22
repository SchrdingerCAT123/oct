
// OCTDlg.h : 头文件
//

#pragma once
#include<highgui.h>
#include "afxwin.h"
#include "afxcmn.h"
#include "MyButton.h"
#include<string> 
#include "MyFristDlg.h"
using namespace std;
using namespace cv;
// COCTDlg 对话框
class COCTDlg : public CDialogEx
{
// 构造
public:
	COCTDlg(CWnd* pParent = NULL);	// 标准构造函数
	

// 对话框数据
	enum { IDD = IDD_OCT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	/*CRectTracker m_rctCurTracker;   //当前选中的矩形区域  
    CRectTracker m_rctTracker[MAX_RECT_NUM]; //用于存储已画的矩形区域  
    bool m_IsChose;  //标记是否被选中  
    bool m_IsDraw;   //标记“绘制”按钮是否按下  
    int m_rectNum;   //当前实际已经画的矩形的个数  
    int m_rctChoseNum;//当前选中的矩形的编号  
    int m_FlaMoveStep;//键盘方向键每响应一次的图像移动的像素单位上的步长  
    int dirct;     //用于标记那个方向键按下。1：左，2：右，3：上，4：下，5：delete（删除）
	*/
	

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	CRect m_rect;
	afx_msg void OnBnClickedButton1();
	afx_msg void ShowImage(IplImage* img, UINT ID);
	CListCtrl m_programLangList;
	afx_msg void OnClose();
	afx_msg void repaint(UINT ID, int last_Width, int now_Width, int last_Height, int now_Height);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedOk();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CMyButton m_leadbtn;
	
	
	CMyButton m_trainstb;
	CMyButton m_labelbtn;
	CMyButton m_showbtn;
	CMyButton m_exitbtn;
	afx_msg void OnBnClickedButton3();
	afx_msg void regionExtraction(int xRoi, int yRoi, int widthRoi, int heightRoi, String path1, String path2);
	afx_msg  Mat fft2(String s, String mm,int num);
	
	afx_msg void OnBnClickedButton2();
	CProgressCtrl m_Progress;
	afx_msg void LIST();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void On32771();
	afx_msg void On32772();
	afx_msg void On32773();
	afx_msg void On32774();
	afx_msg void On32785();	
	afx_msg void On32784();
	afx_msg void On32786();
	afx_msg void On32787();
	afx_msg void On32777();
	afx_msg void On32779();
	afx_msg void On32781();
	afx_msg void On32778();
	afx_msg void On32780();
	afx_msg void On32788();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CStatic m_image;
	//afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	//void ChangeRectPt(int ChangeDirct);
	//afx_msg void On32790();

	afx_msg void On32791();
};


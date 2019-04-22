
// OCTDlg.h : ͷ�ļ�
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
// COCTDlg �Ի���
class COCTDlg : public CDialogEx
{
// ����
public:
	COCTDlg(CWnd* pParent = NULL);	// ��׼���캯��
	

// �Ի�������
	enum { IDD = IDD_OCT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	/*CRectTracker m_rctCurTracker;   //��ǰѡ�еľ�������  
    CRectTracker m_rctTracker[MAX_RECT_NUM]; //���ڴ洢�ѻ��ľ�������  
    bool m_IsChose;  //����Ƿ�ѡ��  
    bool m_IsDraw;   //��ǡ����ơ���ť�Ƿ���  
    int m_rectNum;   //��ǰʵ���Ѿ����ľ��εĸ���  
    int m_rctChoseNum;//��ǰѡ�еľ��εı��  
    int m_FlaMoveStep;//���̷����ÿ��Ӧһ�ε�ͼ���ƶ������ص�λ�ϵĲ���  
    int dirct;     //���ڱ���Ǹ���������¡�1����2���ң�3���ϣ�4���£�5��delete��ɾ����
	*/
	

	// ���ɵ���Ϣӳ�亯��
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


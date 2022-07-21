
// MFCGameLine98Dlg.h : header file
//

#pragma once
#include <map>

using namespace std;


constexpr auto N = 9; // total pos in each row
constexpr auto M = 3; // amount pos random per turn
constexpr auto MIN = 5; 

constexpr auto TCOLOR = 3;
constexpr auto RED = 0;
constexpr auto BLUE = 1;
constexpr auto GREEN = 2;
constexpr auto SHOW = 1;
constexpr auto HIDE = 0;
constexpr auto LARGE = 1;
constexpr auto SMALL = 0;

// CMFCGameLine98Dlg dialog
class CMFCGameLine98Dlg : public CDialogEx
{
	// Construction
public:
	CMFCGameLine98Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCGAMELINE98_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	UINT_PTR eTime, nTime;
	CBitmap mbmlarge, mbmsmall;
	CBitmap mbmBkrSel;
	CPoint npos[M];
	int ncolor[M];
	bool bselect;
	CPoint mposSel;
	

	map<int, CPoint> mapPosFree;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void randomPoint(CPoint pos[], int color[]);
	void setPosFree();
	void setPosStatus(CPoint pos[], int color[], int stat, int type);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void findHor();
	void findVer();
};

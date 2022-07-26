
// MFCGameLine98Dlg.h : header file
//

#pragma once
#include <map>
#include <set>
#include <vector>

using namespace std;

struct Ballcell {
	int color;  // 1 red/ 2 blue/ 3 green
	int type;   // 1 large / 0 small
	int status; // 0 hide / 1 show
};

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
constexpr auto P0x = 0;
constexpr auto P0y = 37;
constexpr auto SGAME = 900;
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
	CBrush m_redbrush, m_brush;
	COLORREF m_background, m_textcolor;
	CMenu m_MenuSys;
	CToolBar  m_ToolBar;
	HICON m_hIcon;
	UINT_PTR eTime, nTime;
	CBitmap mbmlarge, mbmsmall;
	CBitmap mbmBkGame;
	CBitmap mbmBkSel;
	CBitmap mbmBkBox;

	Ballcell mposTbl[N][N];
	Ballcell mposTblBef[N][N][N];
	Ballcell postblNull;
	int idxCur;
	int idxUndoFirst;
	int idxUndo;
	int totalgoes;

	CPoint nposRand[M + 1];
	int ncolorRand[M + 1];
	bool bselect;
	CPoint mposSel;
	CPoint mposSel2;
	int nScores;
	int nScoresUndo[N];
	int nDispTime[3];

	vector<CPoint>vtPosKills;
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
	void setPosKills();
	void setPosStatus(CPoint pos[], int color[], int stat, int type);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void findHor();
	void findVer();
	void findrightDiagonal();
	void findLeftDiagonal();
	bool checkPathExist(CPoint src, CPoint des);
	bool isSafe(int i, int j);
	bool isPath(int i, int j, bool visited[][N], CPoint des);
	afx_msg void OnBtnNew();
	afx_msg void OnBtnExit();
	CEdit mEdxPoint;
	CEdit mEdxTime;
	void calPoints();
	void resetGame();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnRedo();
	afx_msg void OnBtnUndo();
	void saveGameStt();
	void checkUndoRedo();
};

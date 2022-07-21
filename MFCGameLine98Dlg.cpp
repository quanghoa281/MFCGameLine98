
// MFCGameLine98Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFCGameLine98.h"
#include "MFCGameLine98Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct pointtable {
	int color;  // 1 red/ 2 blue/ 3 green
	int type;   // 1 large / 0 small
	int status; // 0 hide / 1 show
};
// CMFCGameLine98Dlg dialog
pointtable mposTbl[9][9];
pointtable postblNull;

CMFCGameLine98Dlg::CMFCGameLine98Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCGAMELINE98_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	eTime = 100;
	nTime = 100;
	postblNull.color = RED;
	postblNull.type = SMALL;
	postblNull.status = HIDE;
}

void CMFCGameLine98Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCGameLine98Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CMFCGameLine98Dlg message handlers

BOOL CMFCGameLine98Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	MoveWindow(0, 0, 466 + 450, 489 + 450, TRUE);
	// TODO: Add extra initialization here
	SetTimer(eTime, nTime, NULL);
	mbmlarge.LoadBitmap(IDB_BM_SP);
	mbmsmall.LoadBitmap(IDB_BM_SP);
	mbmBkrSel.LoadBitmap(IDB_BM_SEL);
	memset(mposTbl, 0, sizeof(mposTbl));
	randomPoint(npos, ncolor);
	setPosStatus(npos, ncolor, SHOW, LARGE);
	randomPoint(npos, ncolor);
	setPosStatus(npos, ncolor, SHOW, SMALL);
	bselect = false;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCGameLine98Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // device context for painting

		CRect crect;
		CBitmap* bmObj, * bmBkr;
		BITMAP bmsp, bmbklr;
		CDC memDC;
		this->GetClientRect(crect);

		int recx = crect.Width() / N;
		int recy = crect.Height() / N;
		CPen cpenx(PS_SOLID, 3, RGB(0, 0, 0));
		CPen cpeny(PS_SOLID, 3, RGB(255, 0, 0));
		for (int i = 1; i < N; i++) {
			dc.SelectObject(cpenx);
			dc.MoveTo(i * recx, 0);
			dc.LineTo(i * recx, crect.Height());

			dc.SelectObject(cpeny);
			dc.MoveTo(0, i * recy);
			dc.LineTo(crect.Width(), i * recy);
		}
		//
		mbmBkrSel.GetBitmap(&bmbklr);
		memDC.CreateCompatibleDC(&dc);
		bmBkr = memDC.SelectObject(&mbmBkrSel);
		if (bselect) {
			dc.StretchBlt(mposSel.y * recx, mposSel.x * recy, recx, recy, &memDC, 0, 0, bmbklr.bmWidth, bmbklr.bmHeight, SRCCOPY);
		}

		mbmlarge.GetBitmap(&bmsp);
		bmObj = memDC.SelectObject(&mbmlarge);
		int lensp = bmsp.bmWidth / 3;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (mposTbl[i][j].status == SHOW) {
					int idxcolor = mposTbl[i][j].color;
					if (mposTbl[i][j].type == LARGE) {
						dc.StretchBlt(j * recx + 10, i * recy + 10, recx - 20, recy - 20, &memDC, idxcolor * lensp, 0, lensp, bmsp.bmHeight, SRCCOPY);
					}
					else {
						dc.StretchBlt(j * recx + 40, i * recy + 40, recx - 80, recy - 80, &memDC, idxcolor * lensp, 0, lensp, bmsp.bmHeight, SRCCOPY);
					}
				}
			}
		}
		memDC.SelectObject(bmObj);
		memDC.SelectObject(bmBkr);
		memDC.DeleteDC();
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCGameLine98Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCGameLine98Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == eTime) {
		//Invalidate(true);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CMFCGameLine98Dlg::randomPoint(CPoint pos[], int color[])
{
	srand(time(NULL));
	setPosFree();
	int amount = mapPosFree.size();
	if (amount <= 3)
	{
		int ID = AfxMessageBox(L"You are losted, Please press YES to new game!", MB_YESNO);
		if (ID == IDYES) {
			memset(mposTbl, 0, sizeof(mposTbl));
			randomPoint(npos, ncolor);
			setPosStatus(npos, ncolor, SHOW, LARGE);
			randomPoint(npos, ncolor);
			setPosStatus(npos, ncolor, SHOW, SMALL);
		}
		return;
	}
	int p[M];
	for (int i = 0; i < M; i++) {
		do {
			p[i] = rand() % amount; // create point
			// check pos is exist
			int chk = 1;
			for (int k = 0; k < i; k++)
			{
				if (p[i] == p[k])
				{
					chk = 0;
					break;
				}
			}
			if (chk)break;
		} while (true);
		pos[i] = mapPosFree.find(p[i])->second;
		do {
			color[i] = rand() % TCOLOR;
			if (i > 0 && color[i] != color[i - 1] || i == 0)break;
		} while (true);
	}
}

void CMFCGameLine98Dlg::setPosFree()
{
	mapPosFree.clear();
	int idx = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (mposTbl[i][j].status == HIDE) {
				mapPosFree[idx++] = CPoint(i, j);
			}
		}
	}
}

void CMFCGameLine98Dlg::setPosStatus(CPoint pos[], int color[], int stat, int type)
{
	for (int i = 0; i < M; i++) {
		mposTbl[pos[i].x][pos[i].y].status = stat;
		mposTbl[pos[i].x][pos[i].y].type = type;
		mposTbl[pos[i].x][pos[i].y].color = color[i];
	}
}


void CMFCGameLine98Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect crect;
	this->GetClientRect(crect);
	int recx = crect.Width() / N;
	int recy = crect.Height() / N;
	int i = point.y / recy;
	int j = point.x / recx;
	if (bselect == false) {
		if (mposTbl[i][j].status == SHOW && mposTbl[i][j].type == LARGE) {
			bselect = true;
			mposSel = CPoint(i, j);
			Invalidate(true);
		}
		else {
			AfxMessageBox(L"Please select the box exist!", MB_OK);
			return;
		}
	}
	else {
		if (mposTbl[i][j].status == HIDE) {
			// swap box selected
			bselect = false;
			pointtable postbl = mposTbl[i][j];
			mposTbl[i][j] = mposTbl[mposSel.x][mposSel.y];
			mposTbl[mposSel.x][mposSel.y] = postbl;
			// change small to large
			for (int i = 0; i < M; i++) {
				mposTbl[npos[i].x][npos[i].y].type = LARGE;
			}
			//set three pos to show small
			randomPoint(npos, ncolor);
			setPosStatus(npos, ncolor, SHOW, SMALL);
			// repaint
			Invalidate(true);
			findHor();
			findVer();
		}
		else if (mposTbl[i][j].status == SHOW && mposTbl[i][j].type == LARGE) {
			bselect = true;
			mposSel = CPoint(i, j);
			Invalidate(true);
		}
		else {
			//AfxMessageBox(L"Please select the empty box!", MB_OK);
			return;
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMFCGameLine98Dlg::findHor()
{
	map<int, map<int, int>> m;
	for (size_t i = 0; i < N; i++)
	{
		map<int, int> n;
		for (size_t j = 0, k = 1; j < N && k <= N; k++)
		{
			if (k == N) {
				if ((k - j) >= MIN) {
					n[j] = k - j;
				}
				break;
			}
			if (mposTbl[i][j].status != SHOW || mposTbl[i][j].type != LARGE) {
				j = k; continue;
			}
			if (mposTbl[i][k].status != SHOW || mposTbl[i][k].type != LARGE || mposTbl[i][k].color != mposTbl[i][j].color) {
				if ((k - j) >= MIN) {
					n[j] = k - j;
				}
				j = k;
				continue;
			}
		}
		if (n.size() > 0) {
			m[i] = n;
		}
	}
	for (auto& n : m)
	{
		for (auto& p : n.second)
		{
			//cout <<  << " " << p.first << " " << p.second << "  ";
			int i = n.first;
			for (size_t j = p.first; j < (p.first + p.second); j++)
			{
				mposTbl[i][j] = postblNull;
			}
		}
	}
	//Sleep(1000);
	Invalidate(true);
}

void CMFCGameLine98Dlg::findVer()
{
	map<int, map<int, int>> m;
	for (size_t i = 0; i < N; i++)
	{
		map<int, int> n;
		for (size_t j = 0, k = 1; j < N && k <= N; k++)
		{
			if (k == N) {
				if ((k - j) >= MIN) {
					n[j] = k - j;
				}
				break;
			}
			if (mposTbl[i][j].status != SHOW || mposTbl[i][j].type != LARGE) {
				j = k; continue;
			}
			if (mposTbl[i][k].status != SHOW || mposTbl[i][k].type != LARGE || mposTbl[i][k].color != mposTbl[i][j].color) {
				if ((k - j) >= MIN) {
					n[j] = k - j;
				}
				j = k;
				continue;
			}
		}
		if (n.size() > 0) {
			m[i] = n;
		}
	}
	for (auto& n : m)
	{
		for (auto& p : n.second)
		{
			//cout <<  << " " << p.first << " " << p.second << "  ";
			int i = n.first;
			for (size_t j = p.first; j < (p.first + p.second); j++)
			{
				mposTbl[i][j] = postblNull;
			}
		}
	}
	//Sleep(1000);
	Invalidate(true);
}

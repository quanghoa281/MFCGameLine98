
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
constexpr auto P0x = 0;
constexpr auto P0y = 37;
constexpr auto SGAME = 900;

CMFCGameLine98Dlg::CMFCGameLine98Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCGAMELINE98_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	eTime = 1;
	nTime = 1000;
	postblNull.color = RED;
	postblNull.type = SMALL;
	postblNull.status = HIDE;
	//
	memset(nposRand, 0, sizeof(nposRand));
	memset(ncolorRand, 0, sizeof(ncolorRand));
	memset(nDispTime, 0, sizeof(nDispTime));
	bselect = 0;
	mposSel = CPoint(0, 0);
	nScores = 0;

}

void CMFCGameLine98Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDX_POINT, mEdxPoint);
	DDX_Control(pDX, IDC_EDX_TIME, mEdxTime);
}

BEGIN_MESSAGE_MAP(CMFCGameLine98Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_BTN_NEW, &CMFCGameLine98Dlg::OnBtnNew)
	ON_COMMAND(ID_BTN_EXIT, &CMFCGameLine98Dlg::OnBtnExit)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMFCGameLine98Dlg message handlers

BOOL CMFCGameLine98Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	//MoveWindow(0, 0, 466 + 450, 489 + 450 /*+ 20 */+ P0, TRUE);
	this->SetWindowTextW(L"Balls Game");
	//
	TCHAR szDirectory[MAX_PATH];
	::GetCurrentDirectory(sizeof(szDirectory) - 1, szDirectory);
	CString m_szFontFile = CString(szDirectory) + L"\\font\\SFDigitalReadout-MediumObli.ttf";
	int nResults = AddFontResourceEx(m_szFontFile, FR_PRIVATE, NULL);
	CFont* m_pFont = new CFont();
	CFont* m_pF = new CFont();
	m_pF->CreateFontW(60,           // nHeight 
		30,                         // nWidth 
		0,                          // nEscapement 
		0,                          // nOrientation 
		FW_LIGHT,                   // nWeight 
		FALSE,                      // bItalic 
		FALSE,                      // bUnderline 
		FALSE,                      // cStrikeOut 
		ANSI_CHARSET,               // nCharSet 
		OUT_DEFAULT_PRECIS,         // nOutPrecision 
		CLIP_DEFAULT_PRECIS,        // nClipPrecision 
		DEFAULT_QUALITY,            // nQuality 
		DEFAULT_PITCH | FF_SWISS,   // nPitchAndFamily 
		_T("SF Digital Readout"));
	m_pFont->CreatePointFont(250, _T("SF Digital Readout")); //consolas
	m_background = RGB(0, 0, 0);
	m_textcolor = RGB(255, 0, 0);
	m_brush.CreateSolidBrush(m_background);
	// menu
	m_MenuSys.LoadMenuW(IDR_MENU_SYS);
	SetMenu(&m_MenuSys);
	// toolbar
	if (!m_ToolBar.CreateEx(this, TBSTYLE_BUTTON, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_ToolBar.LoadToolBar(IDR_TOOLBAR_SYS))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	// edit control 
	GetDlgItem(IDC_EDX_POINT)->SetFont(m_pF, TRUE);
	GetDlgItem(IDC_EDX_TIME)->SetFont(m_pFont, TRUE);
	mEdxPoint.ModifyStyle(0, WS_DISABLED);
	mEdxTime.ModifyStyle(0, WS_DISABLED);
	CString csPoint; csPoint.Format(L"%d", nScores);
	mEdxPoint.SetWindowTextW(csPoint);
	CString timeshow; timeshow.Format(L"%02d:%02d:%02d", nDispTime[0], nDispTime[1], nDispTime[2]);
	mEdxTime.SetWindowTextW(timeshow);
	// TODO: Add extra initialization here
	SetTimer(eTime, nTime, NULL);
	mbmlarge.LoadBitmap(IDB_BM_SP);
	mbmsmall.LoadBitmap(IDB_BM_SP);
	mbmBkSel.LoadBitmap(IDB_BM_SEL);
	mbmBkGame.LoadBitmap(IDB_BM_BKGGAME);
	resetGame();
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
		CBitmap* pbmSphere, * pbmSel, * pbmGame;
		BITMAP BMSphe, BMSel, BMGame;
		CDC memDC;
		GetClientRect(&crect);
		int rexbox = SGAME / N;
		int boxlarge = rexbox - rexbox / 10;
		int boxsmall = rexbox - rexbox / 5 * 2;

		//
		memDC.CreateCompatibleDC(&dc);
		//
		mbmBkGame.GetBitmap(&BMGame);
		pbmGame = memDC.SelectObject(&mbmBkGame);
		dc.StretchBlt(P0x, P0y, SGAME, SGAME, &memDC, 0, 0, BMGame.bmWidth, BMGame.bmHeight, SRCCOPY);
		//
		CPen cpenrow(PS_SOLID, 3, RGB(214, 214, 214));
		CPen cpencol(PS_SOLID, 3, RGB(214, 214, 214));
		for (int i = 0; i <= N; i++) {
			// line col
			dc.SelectObject(cpencol);
			dc.MoveTo(P0x + i * rexbox, P0y);
			dc.LineTo(P0x + i * rexbox, P0y + SGAME);

			// line row
			dc.SelectObject(cpenrow);
			dc.MoveTo(P0x, P0y + i * rexbox);
			if (i == 0 || i == N) {
				dc.LineTo(crect.Width(), P0y + i * rexbox);
			}
			else {
				dc.LineTo(SGAME, P0y + i * rexbox);
			}
		}
		const int px1 = 60;
		const int py1 = 200;
		for (size_t i = 0; i <= M; i++)
		{
			// line row
			dc.MoveTo(SGAME + px1, py1 + i * rexbox);
			dc.LineTo(SGAME + px1 + rexbox, py1 + i * rexbox);
			// line col
			if (i < 2) {
				dc.MoveTo(SGAME + px1 + i * rexbox, py1);
				dc.LineTo(SGAME + px1 + i * rexbox, py1 + 3 * rexbox);
			}
		}
		mbmBkSel.GetBitmap(&BMSel);
		pbmSel = memDC.SelectObject(&mbmBkSel);
		if (bselect) {
			dc.StretchBlt(P0x + mposSel.y * rexbox, P0y + mposSel.x * rexbox, rexbox, rexbox, &memDC, 0, 0, BMSel.bmWidth, BMSel.bmHeight, SRCCOPY);
		}

		mbmlarge.GetBitmap(&BMSphe);
		pbmSphere = memDC.SelectObject(&mbmlarge);
		int lensp = BMSphe.bmWidth / 3;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (mposTbl[i][j].status == SHOW) {
					int idxcolor = mposTbl[i][j].color;
					if (mposTbl[i][j].type == LARGE) {
						dc.StretchBlt(P0x + j * rexbox + boxlarge, P0y + i * rexbox + boxlarge, rexbox - boxlarge * 2, rexbox - boxlarge * 2, &memDC, idxcolor * lensp, 0, lensp, BMSphe.bmHeight, SRCCOPY);
					}
					else {
						dc.StretchBlt(P0x + j * rexbox + boxsmall, P0y + i * rexbox + boxsmall, rexbox - boxsmall * 2, rexbox - boxsmall * 2, &memDC, idxcolor * lensp, 0, lensp, BMSphe.bmHeight, SRCCOPY);
					}
				}
			}
		}
		for (int i = 0; i < M; i++) {
			int colorball = ncolorRand[i];
			boxlarge = 5;
			dc.StretchBlt(SGAME + px1 + boxlarge, py1 + i * rexbox + boxlarge, rexbox - boxlarge * 2, rexbox - boxlarge * 2, &memDC, colorball * lensp, 0, lensp, BMSphe.bmHeight, SRCCOPY);
		}
		memDC.SelectObject(pbmSphere);
		memDC.SelectObject(pbmSel);
		memDC.SelectObject(pbmGame);
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
		nDispTime[2]++;
		if (nDispTime[2] == 60) {
			nDispTime[1]++;
			nDispTime[2] = 0;
		}
		if (nDispTime[1] == 60) {
			nDispTime[0]++;
			nDispTime[1] = 0;
		}
		CString timeshow; timeshow.Format(L"%02d:%02d:%02d", nDispTime[0], nDispTime[1], nDispTime[2]);
		mEdxTime.SetWindowTextW(timeshow);
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
			resetGame();
		}
		return;
	}
	int p[M + 1];
	for (int i = 0; i < M + 1; i++) {
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

void CMFCGameLine98Dlg::setPosKills()
{
	for each (auto pos in vtPosKills)
	{
		mposTbl[pos.x][pos.y] = postblNull;
	}
	vtPosKills.clear();
	Invalidate(true);
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
	/*CRect crect;
	this->GetClientRect(crect);
	int recx = crect.Width() / N;
	int recy = crect.Height() / N;*/
	int rexbox = SGAME / N;
	if ((point.y < P0y) || (point.y - P0y > SGAME) || (point.x < P0x) || (point.x - P0x > SGAME))
	{
		return;
	}
	int i = (point.y - P0y) / rexbox;
	int j = (point.x - P0x) / rexbox;

	// select first
	if (bselect == false) {
		if (mposTbl[i][j].status == SHOW && mposTbl[i][j].type == LARGE) {
			bselect = true;
			mposSel = CPoint(i, j);
			Invalidate(true);
		}
		else {
			//AfxMessageBox(L"Please select the box exist!", MB_OK);
			return;
		}
	}
	// select second
	else {
		// clear select
		if (i == mposSel.x && j == mposSel.y) {
			bselect = false;
			Invalidate(true);
		}
		else {
			if (mposTbl[i][j].status == HIDE || mposTbl[i][j].type == SMALL) {
				if (checkPathExist(mposSel, CPoint(i, j)) == false) {
					return;
				}
				// move pos small to other
				if (mposTbl[i][j].type == SMALL)
				{
					for (auto& pos : nposRand)
					{
						if (pos.x == i && pos.y == j)
						{
							pos = nposRand[M];
						}
					}
				}
				// swap box selected
				bselect = false;
				//pointtable postbl = mposTbl[i][j];
				mposTbl[i][j] = mposTbl[mposSel.x][mposSel.y];
				mposTbl[mposSel.x][mposSel.y] = postblNull;

				// change small to large
				for (int i = 0; i < M; i++) {
					mposTbl[nposRand[i].x][nposRand[i].y].type = LARGE;
					mposTbl[nposRand[i].x][nposRand[i].y].status = SHOW;
					mposTbl[nposRand[i].x][nposRand[i].y].color = ncolorRand[i];
				}
				//create three small ball 
				randomPoint(nposRand, ncolorRand);
				setPosStatus(nposRand, ncolorRand, SHOW, SMALL);
				// repaint
				Invalidate(true);
				findHor();
				findVer();
				findrightDiagonal();
				findLeftDiagonal();
				calPoints();
				setPosKills();
			}
			// select again
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
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMFCGameLine98Dlg::findHor()
{
	map<int, map<int, int>> m;
	for (int i = 0; i < N; i++)
	{
		map<int, int> n;
		for (int j = 0, k = 1; j < N && k <= N; k++)
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
			for (int j = p.first; j < (p.first + p.second); j++)
			{
				//mposTbl[i][j] = postblNull;
				vtPosKills.push_back(CPoint(i, j));
			}
		}
	}
	//Sleep(1000);
	//Invalidate(true);
}


void CMFCGameLine98Dlg::findVer()
{
	map<int, map<int, int>> m;
	for (int i = 0; i < N; i++)
	{
		map<int, int> n;
		for (int j = 0, k = 1; j < N && k <= N; k++)
		{
			if (k == N) {
				if ((k - j) >= MIN) {
					n[j] = k - j;
				}
				break;
			}
			if (mposTbl[j][i].status != SHOW || mposTbl[j][i].type != LARGE) {
				j = k; continue;
			}
			if (mposTbl[k][i].status != SHOW || mposTbl[k][i].type != LARGE || mposTbl[k][i].color != mposTbl[j][i].color) {
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
			for (int j = p.first; j < (p.first + p.second); j++)
			{
				//mposTbl[j][i] = postblNull;
				vtPosKills.push_back(CPoint(j, i));
			}
		}
	}
	//Sleep(1000);
	//Invalidate(true);
}
void CMFCGameLine98Dlg::findrightDiagonal() {
	//  - right diagonal
	map<int, map<int, int>> ml;
	map<int, map<int, int>> mr;
	for (int p = 0; p <= N - MIN; p++)
	{
		map<int, int> n;
		for (int i = p, j = 0, k = 1; (j < N - p) && k <= N; k++) {
			if (k == N) {
				if ((k - j) >= MIN) {
					n[j] = k - j;
				}
				break;
			}
			if (mposTbl[i][j].status != 1 || mposTbl[i][j].type != 1) {
				j = k;
				i = p + k;
				continue;
			}
			if (mposTbl[i + k - j][k].status != 1 || mposTbl[i + k - j][k].type != 1 || mposTbl[i + k - j][k].color != mposTbl[i][j].color) {
				if ((k - j) >= MIN) {
					n[j] = k - j;
				}
				j = k;
				i = p + k;
				continue;
			}
		}
		if (n.size() > 0)
			ml[p] = n;
	}
	for (int p = 0; p <= N - MIN; p++)
	{
		map<int, int> n;
		for (int i = p, j = 0, k = 1; (i < N - p) && k <= N; k++) {
			if (k == N) {
				if ((k - j) >= MIN) {
					n[j] = k - j;
				}
				break;
			}
			if (mposTbl[j][i].status != 1 || mposTbl[j][i].type != 1) {
				j = k;
				i = p + k;
				continue;
			}
			if (mposTbl[k][i + k - j].status != 1 || mposTbl[k][i + k - j].type != 1 || mposTbl[k][i + k - j].color != mposTbl[j][i].color) {
				if ((k - j) >= MIN) {
					n[j] = k - j;
				}
				j = k;
				i = p + k;
				continue;
			}
		}
		if (n.size() > 0)
			mr[p] = n;
	}
	for (auto& n : ml)
	{
		for (auto& p : n.second)
		{
			int l = n.first;
			int r = p.second;
			int j = p.first;
			int i = l + j;
			for (int k = 0; k < r; k++)
			{
				vtPosKills.push_back(CPoint(i + k, j + k));
			}
		}
	}
	for (auto& n : mr)
	{
		for (auto& p : n.second)
		{
			int l = n.first;
			int r = p.second;
			int j = p.first;
			int i = l + j;
			for (int k = 0; k < r; k++)
			{
				vtPosKills.push_back(CPoint(j + k, i + k));
			}
		}
	}
}
void CMFCGameLine98Dlg::findLeftDiagonal() {
	//  - right diagonal
	map<int, map<int, int>> ml;
	map<int, map<int, int>> mr;
	for (int p = N - 1; p >= MIN - 1; p--)
	{
		map<int, int> n;
		for (int i = 0, j = p, k = 1; i <= p && k <= p + 1; k++) {
			if (k == p + 1) {
				if ((k - i) >= MIN) {
					n[i] = k - i;
				}
				break;
			}
			if (mposTbl[i][j].status != 1 || mposTbl[i][j].type != 1) {
				i = k;
				j = p - k;
				continue;
			}
			if (mposTbl[k][p - k].status != 1 || mposTbl[k][p - k].type != 1 || mposTbl[k][p - k].color != mposTbl[i][j].color) {
				if ((k - i) >= MIN) {
					n[i] = k - i;
				}
				i = k;
				j = p - k;
				continue;
			}
		}
		if (n.size() > 0)
			ml[p] = n;
	}
	for (int p = 1; p <= N - MIN; p++)
	{
		map<int, int> n;
		for (int i = p, j = N - 1, k = 1 + p; k <= N; k++) {
			if (k == N) {
				if ((k - i) >= MIN) {
					n[i] = k - i;
				}
				break;
			}
			if (mposTbl[i][j].status != 1 || mposTbl[i][j].type != 1) {
				i = k;
				j = N - 1 - (k - p);
				continue;
			}
			if (mposTbl[k][N - 1 - (k - p)].status != 1 || mposTbl[k][N - 1 - (k - p)].type != 1 || mposTbl[k][N - 1 - (k - p)].color != mposTbl[i][j].color) {
				if ((k - i) >= MIN) {
					n[i] = k - i;
				}
				j = N - 1 - (k - p);
				i = k;
				continue;
			}
		}
		if (n.size() > 0)
			mr[p] = n;
	}
	for (auto& n : ml)
	{
		for (auto& p : n.second)
		{
			int l = n.first;
			int r = p.second;
			int i = p.first;
			int j = l - i;
			for (int k = 0; k < r; k++)
			{
				vtPosKills.push_back(CPoint(i + k, j - k));
			}
		}
	}
	for (auto& n : mr)
	{
		for (auto& p : n.second)
		{
			int l = n.first;
			int r = p.second;
			int i = p.first;
			int j = N - 1 - (i - l);
			for (int k = 0; k < r; k++)
			{
				vtPosKills.push_back(CPoint(i + k, j - k));
			}
		}
	}
}

void CMFCGameLine98Dlg::OnBtnNew()
{
	// TODO: Add your command handler code here
	int ID = AfxMessageBox(L"Are you want to new game?", MB_YESNO);
	if (ID == IDYES) {
		resetGame();
	}
	return;
}


void CMFCGameLine98Dlg::OnBtnExit()
{
	// TODO: Add your command handler code here
	CDialogEx::OnOK();
}

void CMFCGameLine98Dlg::calPoints()
{
	map<int, int>point;
	for (auto pos : vtPosKills)
	{
		int idx = pos.x * N + pos.y;
		if (point.find(idx) != point.end()) {
			point.find(idx)->second++;
		}
		else {
			point[idx] = 1;
		}
	}
	for (auto posp : point) {
		nScores += posp.second * posp.second;
	}
	CString csPoint; csPoint.Format(L"%d", nScores);
	mEdxPoint.SetWindowTextW(csPoint);
}

void CMFCGameLine98Dlg::resetGame()
{
	memset(mposTbl, 0, sizeof(mposTbl));
	memset(nDispTime, 0, sizeof(nDispTime));
	nScores = 0;
	bselect = false;
	vtPosKills.clear();
	randomPoint(nposRand, ncolorRand);
	setPosStatus(nposRand, ncolorRand, SHOW, LARGE);
	randomPoint(nposRand, ncolorRand);
	setPosStatus(nposRand, ncolorRand, SHOW, SMALL);
	calPoints();
	Invalidate(true);
}


HBRUSH CMFCGameLine98Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	switch (nCtlColor)
	{

	case CTLCOLOR_EDIT:
		switch (pWnd->GetDlgCtrlID()) {
		case IDC_EDX_POINT:
			pDC->SetBkColor(m_background);    // change the background
			pDC->SetTextColor(m_textcolor);  // change the text color
			hbr = (HBRUSH)m_brush;    // apply the rose brush
			break;
		case IDC_EDX_TIME:
			pDC->SetBkColor(m_background);    // change the background
			pDC->SetTextColor(RGB(255, 255, 255));  // change the text color
			hbr = (HBRUSH)m_brush;    // apply the rose brush
			break;
		}
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
bool CMFCGameLine98Dlg::isSafe(int i, int j)
{
	if (i >= 0 && i < N && j >= 0 && j < N)
		return true;
	return false;
}
bool CMFCGameLine98Dlg::isPath(int i, int j, bool visited[][N], CPoint des)
{
	// Checking the boundaries, walls and
	// whether the cell is unvisited
	if (isSafe(i, j) && mposTbl[i][j].type != SHOW
		&& !visited[i][j])
	{
		// Make the cell visited
		visited[i][j] = true;

		// if the cell is the required
		// destination then return true
		if (i == des.x && j == des.y)
			return true;

		// traverse up
		bool up = isPath(i - 1, j, visited, des);

		// if path is found in up
		// direction return true
		if (up)
			return true;

		// traverse left
		bool left = isPath(i, j - 1, visited, des);

		// if path is found in left
		// direction return true
		if (left)
			return true;

		// traverse down
		bool down = isPath(i + 1, j, visited, des);

		// if path is found in down
		// direction return true
		if (down)
			return true;

		// traverse right
		bool right = isPath(i, j + 1, visited, des);

		// if path is found in right
		// direction return true
		if (right)
			return true;
	}

	// no path has been found
	return false;
}
bool CMFCGameLine98Dlg::checkPathExist(CPoint src, CPoint des)
{
	// Checking the boundaries, walls and
	// whether the cell is unvisited
	bool visited[N][N];
	memset(visited, false, sizeof(visited));
	if (src == des)return true;
	return (isPath(src.x+1, src.y, visited, des) || 
		isPath(src.x-1, src.y, visited, des) ||
		isPath(src.x, src.y+1, visited, des) ||
		isPath(src.x, src.y-1, visited, des));
}

/***************************************************************/
//FileName:	RainplayDlg.cpp
//Function:	Implement the rainplay main dialog
//Author:	Bill Yuan
//EMail:	rainman_yuan@hotmail.com
//			rainplay@263.net
//WWW:		http://rainman_yuan.yeah.net
//			http://rainplay.yeah.net
//Date:		Aug/18/1998
/***************************************************************/
// RainplayDlg.cpp : implementation file
//
#include "stdafx.h"
#include "Rainplay.h"
#include "RainplayDlg.h"
#include "MainFaceDescribe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRainplayDlg dialog

CRainplayDlg::CRainplayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRainplayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRainplayDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
#ifdef	__FREEAMP_UI__
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif	//__FREEAMP_UI__

	bInHotKey = FALSE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pCMainUp = new CBmpSize(IDB_MAINUP);
	m_pCMainDown = new CBmpSize(IDB_MAINDOWN);
}
CRainplayDlg::~CRainplayDlg()
{
	delete m_pCMainUp;
	delete m_pCMainDown;
}

void CRainplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRainplayDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRainplayDlg, CDialog)
	//{{AFX_MSG_MAP(CRainplayDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTEISCHANGING()
	ON_WM_PALETTECHANGED()
	ON_MESSAGE( WM_HOTKEY, OnHotKey )
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRainplayDlg message handlers
BOOL CRainplayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	SetWindowRgn(m_pCMainUp->MakeRegion(), FALSE);
	SetWindowPos(NULL, 0,0,
		m_pCMainUp->m_iWidth, m_pCMainUp->m_iHeight,
		SWP_SHOWWINDOW);
	CenterWindow(NULL);
	m_pCMainUp->BmpBlt( FALSE, this, 0, 0, 
		m_pCMainUp->m_iWidth,	m_pCMainUp->m_iHeight,
		0,0);

	CString szAppname;
	szAppname.LoadString(IDS_APPNAME);
	SetWindowText(szAppname);

	//注册热键
	::RegisterHotKey(m_hWnd, ALT_S, MOD_ALT, 'S' );		//Alt+S
	::RegisterHotKey(m_hWnd, ALT_A, MOD_ALT, 'A' );		//Alt+A
	::RegisterHotKey(m_hWnd, ALT_X, MOD_ALT, 'X' );		//Alt+A

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRainplayDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		//重画Rainplay对话框
		CRect sRect;
		if ( GetUpdateRect( &sRect, FALSE )==0 )
			return;

		m_pCMainUp->BmpBlt( TRUE, this,
			sRect.left, sRect.top,
			sRect.right-sRect.left+1,	sRect.bottom-sRect.top+1,
			sRect.left, sRect.top);

		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRainplayDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


/***************************************************************/
//FuncName:	OnMouseMove
//Class:	CRainplayDlg
//Function:	使用户可以任意拖动Rainplay 对话框
//Author:	Bill Yuan
//Para:		nFlags, point
//Return:	void
//Date:		Jan/24/1998
/***************************************************************/
void CRainplayDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( nFlags & MK_LBUTTON && m_bNotInHotArea )
	{
		SendMessage( WM_SYSCOMMAND, SC_MOVE | HTCLIENT, 0 );
	}
	
	CDialog::OnMouseMove(nFlags, point);
}


void CRainplayDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetCapture();	//Allow to capture the mouse with full screen

	m_iCurAreaIdx = -1;
	m_bNotInHotArea = TRUE;
	m_AcutalPoint.x = point.x;
	m_AcutalPoint.y = point.y;

	for( int i=0; i<MAX_AREA; i++ ) {
		m_rectHotArea.left = coordinatesMain[i].x;
		m_rectHotArea.top = coordinatesMain[i].y;
		m_rectHotArea.right = coordinatesMain[i].x+
							coordinatesMain[i].iWidth;
		m_rectHotArea.bottom = coordinatesMain[i].y+
							coordinatesMain[i].iHeight;

		if (m_rectHotArea.PtInRect(point)&&
			coordinatesMain[i].bIsButton) {
			m_bNotInHotArea = FALSE;
			m_iCurAreaIdx = i;
			break;
		}
	}

	if (m_iCurAreaIdx == -1 )
		ReleaseCapture();
	else
		m_pCMainDown->BmpBlt(FALSE, this,
			m_rectHotArea.left, m_rectHotArea.top, 
			coordinatesMain[i].iWidth,
			coordinatesMain[i].iHeight,
			m_rectHotArea.left, m_rectHotArea.top);

	CDialog::OnLButtonDown(nFlags, point);
}

void CRainplayDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture();
	m_bNotInHotArea = TRUE;

	if ( m_iCurAreaIdx!=-1 )
		m_pCMainUp->BmpBlt(FALSE, this,
			m_rectHotArea.left, m_rectHotArea.top, 
			coordinatesMain[m_iCurAreaIdx].iWidth,
			coordinatesMain[m_iCurAreaIdx].iHeight,
			m_rectHotArea.left, m_rectHotArea.top);

	if (m_rectHotArea.PtInRect(point) &&
		coordinatesMain[m_iCurAreaIdx].bIsButton ) {
		switch(m_iCurAreaIdx) {
		case AREA_ABOUT:
			break;
		case AREA_MINIMIZE:
			ShowWindow(SW_MINIMIZE);
			break;
		case AREA_QUIT:
			EndDialog(TRUE);
			break;
		default:
			break;
		}	
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CRainplayDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch( nChar ) {
	case VK_F1:
		if (GetKeyState(VK_SHIFT)<0 ) {
			////////////
		}
	default:
		break;
	}
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

//焦点到来，重置色盘，并刷新
BOOL CRainplayDlg::OnQueryNewPalette()
{
	m_pCMainUp->BmpBlt( FALSE, this, 0, 0, 
		m_pCMainUp->m_iWidth, m_pCMainUp->m_iHeight, 0, 0 );
	return TRUE;
}

//色盘改变时，重置色盘
void CRainplayDlg::OnPaletteIsChanging( CWnd* pRealizeWnd )
{
	if (pRealizeWnd!=this)
		InvalidateRect(NULL, FALSE);
}

void CRainplayDlg::OnPaletteChanged( CWnd* pFocusWnd )
{
	if (pFocusWnd!=this)
		InvalidateRect(NULL, FALSE);
}

//拦截热键Alt+S( Skin browser )
LRESULT CRainplayDlg::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	int idHotKey = (int) wParam;              // identifier of hot key 
	UINT fuModifiers = (UINT) LOWORD(lParam);  // key-modifier flags 
	UINT uVirtKey = (UINT) HIWORD(lParam);     // virtual-key code 

	if ( idHotKey==IDHOT_SNAPDESKTOP ||
		idHotKey==IDHOT_SNAPWINDOW ||
		bInHotKey)
		return FALSE;

	if (fuModifiers==MOD_ALT) {
		switch(uVirtKey) {
		case 'S':
			SelectSkin();
			break;
		case 'A':
			SelectOldSkin();
			break;
		case 'X':
			EndDialog(TRUE);
			break;
		default:
			break;
		}
	}
	return TRUE;
}

void CRainplayDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMenu cPopupMenu;
	POINT curPoint;
	BOOL bFlag;


	HMENU hMenu, hPopupMenu;

	hMenu = LoadMenu( AfxGetInstanceHandle(),MAKEINTRESOURCE(IDM_MAIN) );
	hPopupMenu = GetSubMenu(hMenu,0);
//	cPopupMenu.LoadMenu(IDM_MAIN);
//	cPopupMenu.GetSubMenu(0);
	::GetCursorPos( &curPoint );

	bFlag = TrackPopupMenu( hPopupMenu,
							TPM_BOTTOMALIGN | TPM_RETURNCMD,
							curPoint.x, curPoint.y, 0, this->m_hWnd, NULL);
	//Display the PopupMenu at the Mouse-Cursor pos.
//	bFlag = cPopupMenu.TrackPopupMenu(TPM_BOTTOMALIGN | TPM_RETURNCMD,
//						curPoint.x, curPoint.y,
//						this, NULL );
	switch( bFlag )	{
	case IDM_OLDSKIN:
		SelectOldSkin();
		break;
	case IDM_SKIN:
		SelectSkin();
		break;
	case IDM_EXIT:
		EndDialog(TRUE);
		break;
	default:
		break;
	}
	CDialog::OnRButtonDown(nFlags, point);
}


BOOL CRainplayDlg::GetSkinSettings(CString szFile)
{
	char szSetting[256];

	//Common settings
	for( int i=0; i<MAX_AREA; i++ ) {
		GetPrivateProfileString(_T("Coordinates"), coordinatesMain[i].szAreaName,
			_T("0,0,0,0"), szSetting, 256, szFile );
		if (_tcslen(szSetting)==0) {
			_tcscpy(szSetting, _T("0,0,0,0"));
		}
		coordinatesMain[i].x = coordinatesMain[i].y = 
		coordinatesMain[i].iWidth = coordinatesMain[i].iHeight = 0;
		sscanf( szSetting, _T("%d,%d,%d,%d"),
				&coordinatesMain[i].x,
				&coordinatesMain[i].y,
				&coordinatesMain[i].iWidth,
				&coordinatesMain[i].iHeight );
	}

	//Display font name
	GetPrivateProfileString(_T("Coordinates"), _T("DisplayFont"),
		_T("Verdana"), szSetting, 256, szFile );
	if (_tcslen(szSetting)==0)
		_tcscpy(szSetting, _T("Verdana"));
	szFontName=szSetting;

	//Display color
	BYTE r,g,b;
	GetPrivateProfileString(_T("Coordinates"), _T("DisplayColour"),
		_T("0,0,0"), szSetting, 256, szFile );
	if (_tcslen(szSetting)==0)
		_tcscpy(szSetting, _T("0,0,0"));
	r = g = b = 0;
	sscanf( szSetting, _T("%d,%d,%d"), &r, &g, &b );
	rgbDisplayColur = RGB(r,g,b);

	return TRUE;
}


BOOL CRainplayDlg::SelectSkin()
{
	//求启动路径
	char lpTemp[MAX_PATH];
	CString szStartUpDir;

	bInHotKey = TRUE;

	GetModuleFileName(NULL, lpTemp, MAX_PATH );
	szStartUpDir = lpTemp;
	szStartUpDir = szStartUpDir.Left(szStartUpDir.ReverseFind(_T('\\'))+1);

	CFileDialog *dialog;
	dialog = new CFileDialog(TRUE, 
				"ini",
				"",
				OFN_FILEMUSTEXIST    | 
				OFN_HIDEREADONLY     |
				OFN_EXPLORER,
				"Rainplay skin settings (*.ini)|*.ini|");
	dialog->m_ofn.lpstrInitialDir = LPCTSTR(szStartUpDir);

	//打开File open dialog
	if (dialog->DoModal()==IDOK) {
		GetSkinSettings(dialog->GetPathName());
		CString szTemp = dialog->GetPathName();
		szTemp = szTemp.Left(szTemp.ReverseFind(_T('\\'))+1);

		CBmpSize *cTmpSkinUp, *cTmpSkinDown;
		cTmpSkinUp = new CBmpSize;
		cTmpSkinDown = new CBmpSize;

		if (cTmpSkinUp->LoadBitmap(szTemp + "MainUp.bmp") &&
			cTmpSkinDown->LoadBitmap(szTemp + "MainDown.bmp") ) {
			
			delete m_pCMainUp;
			delete m_pCMainDown;
			m_pCMainUp = cTmpSkinUp;
			m_pCMainDown = cTmpSkinDown;

			SetWindowRgn(m_pCMainUp->MakeRegion(), TRUE);
			SetWindowPos(NULL, 0,0,
				m_pCMainUp->m_iWidth, m_pCMainUp->m_iHeight,
				SWP_SHOWWINDOW | SWP_NOMOVE );
			m_pCMainUp->BmpBlt( FALSE, this, 0, 0, 
				m_pCMainUp->m_iWidth, m_pCMainUp->m_iHeight, 0, 0 );
		} else {
			delete cTmpSkinUp;
			delete cTmpSkinDown;
			AfxMessageBox("Can't change to this skin");
			SelectOldSkin();
		}
	}
	delete dialog;

	bInHotKey = FALSE;
	return TRUE;
}


/***************************************************************/
//FuncName:	SelectOldSkin
//Class:	CRainplayDlg
//Function:	恢复Old Skin设定
//Author:	Bill Yuan
//Para:		void
//Return:	BOOL
//Date:		Oct/21/1998
/***************************************************************/
BOOL CRainplayDlg::SelectOldSkin()
{
	delete m_pCMainUp;
	delete m_pCMainDown;

	m_pCMainUp = new CBmpSize(IDB_MAINUP);
	m_pCMainDown = new CBmpSize(IDB_MAINDOWN);

	SetWindowRgn(m_pCMainUp->MakeRegion(), TRUE);
	SetWindowPos(NULL, 0,0,
		m_pCMainUp->m_iWidth, m_pCMainUp->m_iHeight,
		SWP_SHOWWINDOW | SWP_NOMOVE);

	m_pCMainUp->BmpBlt( FALSE, this, 0, 0, 
		m_pCMainUp->m_iWidth,	m_pCMainUp->m_iHeight,
		0,0);

	//Restore old skin settings
	for( int i=0; i<MAX_AREA; i++ ) {
		coordinatesMain[i].x = oldSkinCoord[i].left;
		coordinatesMain[i].y = oldSkinCoord[i].top;
		coordinatesMain[i].iWidth = oldSkinCoord[i].right;
		coordinatesMain[i].iHeight = oldSkinCoord[i].bottom;
	}
	return TRUE;
}
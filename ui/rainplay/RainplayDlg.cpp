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
#include "afxpriv.h"			//Just for WM_KICKIDLE	private message
#include "Rainplay.h"
#include "RainplayUI.h"
#include "RainplayDlg.h"
#include "PlayListDlg.h"
#include "EQDlg.h"
#include "VisualView.h"
#include "AboutDlg.h"
#include "MainFaceDescribe.h"
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CRainplayUI *g_ui;
extern BOOL EnumUIFont(CClientDC *dc, LPLOGFONT lpLogFont, CString szFont);
extern BOOL ReadPlaylistFromFile(CString szFile, PlayListManager *plm);
extern BOOL SavePlayListToFile(CString szFile, PlayListManager *plm);
/////////////////////////////////////////////////////////////////////////////
// CRainplayDlg dialog

CRainplayDlg::CRainplayDlg(CWnd* pParent /*=NULL*/)
:	m_bInHotKey(FALSE),
	m_bRepeatControlDisplay(FALSE),
	m_bShuffleControlDisplay(FALSE),
	m_bInitUIOk(FALSE),
	m_hIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME)),
	CDialog(CRainplayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRainplayDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

	//求启动路径
	char lpTemp[MAX_PATH];
	GetModuleFileName(NULL, lpTemp, MAX_PATH );
	m_szStartupDir = lpTemp;
	m_szStartupDir = m_szStartupDir.Left(m_szStartupDir.ReverseFind(_T('\\'))+1);

	//Load skin
	m_pCMainUp = new CBmpSize(IDB_MAINUP);
	m_pCMainDown = new CBmpSize(IDB_MAINDOWN);

	//About dialog
	m_aboutDlg = new CAboutDlg();

	//建立字体
	m_uifont.font1 = new CFont();
	m_uifont.font2 = new CFont();
	m_uifont.font3 = new CFont();
	m_uifont.bAvailable = FALSE;

	//信号量
	m_Mutex = new Mutex();

	GetUILock();
	m_i.szBitrate = "128";			//bitrate
	m_i.szSamplerate = "44";		//sample rate
	m_i.szSongname = "";			//song name string
	m_i.szTime = "00:00";		//current time string
	m_i.indexOfSong = 0;			//index of the song which current playing
	m_i.totalSongs = 0;				//total songs number
	m_i.scrollOffset = 0;			//song name string scroll offset
	m_i.length = 0;					//total length of the song name string displaying in pixel
	m_i.currenttime = 0;			//current time decimal in second
	m_i.totaltime = 32767;			//total time decimal in second
	m_i.visualmode = oscilliscope;	//waveform display as default
	m_i.fps = 25;					//25 fps visual display as default
	m_i.analyzer = bars;			//lines spectrum;
	m_i.scope = dot;				//dot scope default
	ReleaseUILock();
}
CRainplayDlg::~CRainplayDlg()
{
	if (m_Mutex) {
		delete m_Mutex;
		m_Mutex = NULL;
	}

	delete m_vis;
	delete m_aboutDlg;
	delete m_uifont.font1;
	delete m_uifont.font2;
	delete m_uifont.font3;
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
	ON_MESSAGE( WM_KICKIDLE,				OnKickIdle )
	ON_MESSAGE( WM_RP_CONTROL_OPEN,			OnClickOpen )
	ON_MESSAGE( WM_RP_CONTROL_PLAY,			OnClickPlay )
	ON_MESSAGE( WM_RP_CONTROL_PAUSE,		OnClickPause )
	ON_MESSAGE( WM_RP_CONTROL_STOP,			OnClickStop )
	ON_MESSAGE( WM_RP_CONTROL_PREV,			OnClickPrev )
	ON_MESSAGE( WM_RP_CONTROL_NEXT,			OnClickNext )
	ON_MESSAGE( WM_RP_CONTROL_SHUFFLE,		OnClickShuffle )
	ON_MESSAGE( WM_RP_CONTROL_REPEAT,		OnClickRepeat )
	ON_MESSAGE( WM_RP_CONTROL_PLAYLIST,		OnClickPlayList )
	ON_MESSAGE( WM_RP_CONTROL_EQ,			OnClickEQ )
	ON_MESSAGE( WM_RP_CONTROL_MINIMIZE,		OnClickMinimize )
	ON_MESSAGE( WM_RP_CONTROL_ABOUT,		OnClickAbout )
	ON_MESSAGE( WM_RP_CONTROL_QUIT,			OnClickQuit )
	ON_MESSAGE(	WM_RP_CONTROL_VISUALMODE,	OnClickVisualMode)
	ON_MESSAGE( WM_RP_MENU_ANALYZERMODE,	OnChangeAnalyzerMode)
	ON_MESSAGE(	WM_RP_CONTROL_SEEK,			OnClickSeek )

	ON_MESSAGE( WM_RP_MENU_SCOPEMODE,		OnScopeMode )
	ON_MESSAGE( WM_RP_MENU_FPS,				OnChangeFps )

	ON_MESSAGE( WM_RP_NOTIFY_TIME,			OnNotifyTime )
	ON_MESSAGE( WM_RP_NOTIFY_SONGNAME,		OnNotifySongName )
	ON_MESSAGE( WM_RP_NOTIFY_MPEGINFO,		OnNotifyMPEGInfo )
	ON_MESSAGE(	WM_RP_NOTIFY_VISDATA,		OnNotifyVisData )
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_DROPFILES()
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

	CString szAppname;
	szAppname.LoadString(IDS_APPNAME);
	SetWindowText(szAppname);

	//注册热键
	::RegisterHotKey(m_hWnd, ALT_S, MOD_ALT, 'S' );		//Alt+S
	::RegisterHotKey(m_hWnd, ALT_A, MOD_ALT, 'A' );		//Alt+A
	::RegisterHotKey(m_hWnd, ALT_X, MOD_ALT, 'X' );		//Alt+X
	::RegisterHotKey(m_hWnd, ALT_G,	MOD_ALT, 'G' );		//Alt+G
	::RegisterHotKey(m_hWnd, ALT_E,	MOD_ALT, 'E' );		//Alt+E

	//Perpare UI fonts
	if (PerpareUIFont()) {
		m_uifont.bAvailable = TRUE;
	}
	
	//Init visual view control
	ASSERT(this->m_hWnd!=NULL);
	m_vis = new CVisualView(this, 
		m_i.fps, m_i.visualmode, m_i.analyzer, m_i.scope,
		coordinatesMain[AREA_DISPLAYSPECTRUM].x,
		coordinatesMain[AREA_DISPLAYSPECTRUM].y,
		coordinatesMain[AREA_DISPLAYSPECTRUM].iWidth,
		coordinatesMain[AREA_DISPLAYSPECTRUM].iHeight,
		m_pCMainUp);

	//UI OK
	m_bEQDlgActive = FALSE;
	m_bPlDlgActive = FALSE;
	m_bInitUIOk = TRUE;
	g_ui->m_uiSemaphore->Signal();

	//hack code
	Sleep(200);
	//Read song from FREEAMP.m3u
	//::ReadPlaylistFromFile(m_szStartupDir + "FREEAMP.M3U", g_ui->m_plm);

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
		if ( GetUpdateRect( &sRect, FALSE )!=0 ) {
			m_pCMainUp->BmpBlt( TRUE, this,
				sRect.left, sRect.top,
				sRect.right-sRect.left+1,	sRect.bottom-sRect.top+1,
				sRect.left, sRect.top);

			//Refresh the repeat mark
			//Notice:the BeginPaint/EndPaint had been call at above BmpBlt,
			//So, we use FALSE(not in paint) instead.
			if ( m_bRepeatControlDisplay ) {
				m_pCMainDown->BmpBlt(FALSE, this,
					coordinatesMain[AREA_DISPLAYREPEAT].x,
					coordinatesMain[AREA_DISPLAYREPEAT].y,
					coordinatesMain[AREA_DISPLAYREPEAT].iWidth,
					coordinatesMain[AREA_DISPLAYREPEAT].iHeight,
					coordinatesMain[AREA_DISPLAYREPEAT].x,
					coordinatesMain[AREA_DISPLAYREPEAT].y );
			} else {
				m_pCMainUp->BmpBlt(FALSE, this,
					coordinatesMain[AREA_DISPLAYREPEAT].x,
					coordinatesMain[AREA_DISPLAYREPEAT].y,
					coordinatesMain[AREA_DISPLAYREPEAT].iWidth,
					coordinatesMain[AREA_DISPLAYREPEAT].iHeight,
					coordinatesMain[AREA_DISPLAYREPEAT].x,
					coordinatesMain[AREA_DISPLAYREPEAT].y );
			}
			//Refresh the shuffle mark
			if ( m_bShuffleControlDisplay ) {
				m_pCMainDown->BmpBlt(FALSE, this,
					coordinatesMain[AREA_DISPLAYSHUFFLE].x,
					coordinatesMain[AREA_DISPLAYSHUFFLE].y,
					coordinatesMain[AREA_DISPLAYSHUFFLE].iWidth,
					coordinatesMain[AREA_DISPLAYSHUFFLE].iHeight,
					coordinatesMain[AREA_DISPLAYSHUFFLE].x,
					coordinatesMain[AREA_DISPLAYSHUFFLE].y );
			} else {
				m_pCMainUp->BmpBlt(FALSE, this,
					coordinatesMain[AREA_DISPLAYSHUFFLE].x,
					coordinatesMain[AREA_DISPLAYSHUFFLE].y,
					coordinatesMain[AREA_DISPLAYSHUFFLE].iWidth,
					coordinatesMain[AREA_DISPLAYSHUFFLE].iHeight,
					coordinatesMain[AREA_DISPLAYSHUFFLE].x,
					coordinatesMain[AREA_DISPLAYSHUFFLE].y );
			}

			//Refresh all string.
			OnNotifyTime(0,0);
			OnNotifySongName(0,0);
			OnNotifyMPEGInfo(0,0);
			CDialog::OnPaint();
		}
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
		m_rectHotArea = AREA_RECT(i);
		if (m_rectHotArea.PtInRect(point)&&
			coordinatesMain[i].bIsButton) {
			m_bNotInHotArea = FALSE;
			m_iCurAreaIdx = i;
			break;
		}
	}

	switch(m_iCurAreaIdx) {
	//Click on !botton
	case -1:
	case AREA_DISPLAYSPECTRUM:
		ReleaseCapture();
		break;
	case AREA_SEEKBAR:				//Seek within the song
		SendMessage(WM_RP_CONTROL_SEEK, 0, (LPARAM)point.x);
		ReleaseCapture();
		break;
	default:
		m_pCMainDown->BmpBlt(FALSE, this,
			m_rectHotArea.left, m_rectHotArea.top, 
			coordinatesMain[i].iWidth,
			coordinatesMain[i].iHeight,
			m_rectHotArea.left, m_rectHotArea.top);
		break;
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CRainplayDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture();
	m_bNotInHotArea = TRUE;

	if ( m_iCurAreaIdx!=-1 ) {
		m_pCMainUp->BmpBlt(FALSE, this,
			m_rectHotArea.left, m_rectHotArea.top, 
			coordinatesMain[m_iCurAreaIdx].iWidth,
			coordinatesMain[m_iCurAreaIdx].iHeight,
			m_rectHotArea.left, m_rectHotArea.top);
	}

	if (m_rectHotArea.PtInRect(point) &&
		coordinatesMain[m_iCurAreaIdx].bIsButton ) {
		switch(m_iCurAreaIdx) {
		case AREA_DISPLAYSPECTRUM:
			SendMessage(WM_RP_CONTROL_VISUALMODE, 0,(LPARAM)flip);
			break;
		case AREA_OPEN:
			SendMessage(WM_RP_CONTROL_OPEN,0,0);
			break;
		case AREA_PLAY:
			SendMessage(WM_RP_CONTROL_PLAY,0,0);
			break;
		case AREA_PAUSE:
			SendMessage(WM_RP_CONTROL_PAUSE,0,0);
			break;
		case AREA_STOP:
			SendMessage(WM_RP_CONTROL_STOP,0,0);
			break;
		case AREA_PREV:
			SendMessage(WM_RP_CONTROL_PREV,0,0);
			break;
		case AREA_NEXT:
			SendMessage(WM_RP_CONTROL_NEXT,0,0);
			break;
		case AREA_SHUFFLE:
			SendMessage(WM_RP_CONTROL_SHUFFLE,0,0);
			break;
		case AREA_REPEAT:
			SendMessage(WM_RP_CONTROL_REPEAT,0,0);
			break;
		case AREA_EQ:
			SendMessage(WM_RP_CONTROL_EQ,0,0);
			break;
		case AREA_PLAYLIST:
			SendMessage(WM_RP_CONTROL_PLAYLIST,0,0);
			break;
		case AREA_MINIMIZE:
			SendMessage(WM_RP_CONTROL_MINIMIZE,0,0);
			break;
		case AREA_ABOUT:
			SendMessage(WM_RP_CONTROL_ABOUT,0,0);
			break;
		case AREA_QUIT:
			SendMessage(WM_RP_CONTROL_QUIT,0,0);
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
		m_bInHotKey)
		return FALSE;

	if (fuModifiers==MOD_ALT) {
		switch(uVirtKey) {
		case 'S':					//Select Skin
			SelectSkin();
			break;
		case 'A':					//Restore default skin
			SelectOldSkin();
			break;
		case 'X':					//Exit
			EndDialog(TRUE);
			break;
		case 'G':					//Equalizer dialog
			SendMessage(WM_RP_CONTROL_EQ, 0,0);
			break;
		case 'E':					//Playlist dialog
			SendMessage(WM_RP_CONTROL_PLAYLIST, 0,0);
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
	CMenu cMenu, *cPopupMenu;
	BOOL bFlag;
	POINT curPoint;

	cMenu.LoadMenu(IDM_MAIN);
	cPopupMenu = cMenu.GetSubMenu(0);
	::GetCursorPos(&curPoint);

	cPopupMenu->CheckMenuItem(IDM_MAIN_REPEAT,						//Repeat
		m_bRepeatControlDisplay ? MF_CHECKED : MF_UNCHECKED);
	cPopupMenu->CheckMenuItem(IDM_MAIN_SHUFFLE,						//Shuffle
		m_bShuffleControlDisplay ? MF_CHECKED : MF_UNCHECKED);
	cPopupMenu->CheckMenuItem(IDM_MAIN_VISMODE_WAVEFORM,			//Waveform
		m_i.visualmode==oscilliscope ? MF_CHECKED : MF_UNCHECKED);
	cPopupMenu->CheckMenuItem(IDM_MAIN_VISMODE_SPEC,				//Spectrum
		m_i.visualmode==spectrum ? MF_CHECKED : MF_UNCHECKED);
	cPopupMenu->CheckMenuItem(IDM_MAIN_VISMODE_DISABLE,				//Vis disable
		m_i.visualmode==disable ? MF_CHECKED : MF_UNCHECKED);
	cPopupMenu->CheckMenuItem(IDM_MAIN_VISMODE_DOTSCOPE,			//Dot scope
		m_i.scope==dot ? MF_CHECKED : MF_UNCHECKED);
	cPopupMenu->CheckMenuItem(IDM_MAIN_VISMODE_LINESCOPE,			//Line scope
		m_i.scope==line ? MF_CHECKED : MF_UNCHECKED);
	cPopupMenu->CheckMenuItem(IDM_MAIN_VISMODE_SOLIDSCOPE,			//Solid scope
		m_i.scope==solid ? MF_CHECKED : MF_UNCHECKED);
	cPopupMenu->CheckMenuItem(IDM_MAIN_VISMODE_LINES,				//Lines spectrum
		m_i.analyzer==lines ? MF_CHECKED : MF_UNCHECKED);
	cPopupMenu->CheckMenuItem(IDM_MAIN_VISMODE_BARS,				//Bars spectrum
		m_i.analyzer==bars ? MF_CHECKED : MF_UNCHECKED);
	cPopupMenu->CheckMenuItem(IDM_MAIN_VISMODE_REFRESHRATE_50,		//50 fps
		m_i.fps==50 ? MF_CHECKED : MF_UNCHECKED);
	cPopupMenu->CheckMenuItem(IDM_MAIN_VISMODE_REFRESHRATE_25,		//25 fps
		m_i.fps==25 ? MF_CHECKED : MF_UNCHECKED);
	cPopupMenu->CheckMenuItem(IDM_MAIN_VISMODE_REFRESHRATE_12,		//12 fps
		m_i.fps==12 ? MF_CHECKED : MF_UNCHECKED);
	bFlag = cPopupMenu->TrackPopupMenu(TPM_BOTTOMALIGN | TPM_RETURNCMD,
						curPoint.x, curPoint.y,
						this, NULL );
	switch( bFlag )	{
	case IDM_MAIN_PREFERENCES:		//Options dialog
		m_preferencesDlg = new CPreferencesDlg();
		m_preferencesDlg->DoModal();
		delete m_preferencesDlg;
		break;
	case IDM_MAIN_OLDSKIN:			//Restore default skin
		SelectOldSkin();
		break;
	case IDM_MAIN_SKIN:				//Select Skin
		SelectSkin();
		break;
	case IDM_MAIN_EQ:				//Equalizer dialog
		SendMessage(WM_RP_CONTROL_EQ, 0,0);
		break;
	case IDM_MAIN_EXIT:				//Exit
		EndDialog(TRUE);
		break;
	case IDM_MAIN_PLAYLIST:			//Playlist dialog
		SendMessage(WM_RP_CONTROL_PLAYLIST, 0,0);
		break;

	case IDM_MAIN_PREV:				//Prev song
		SendMessage(WM_RP_CONTROL_PREV,0,0);
		break;
	case IDM_MAIN_PLAY:				//Play
		SendMessage(WM_RP_CONTROL_PLAY,0,0);
		break;
	case IDM_MAIN_PAUSE:			//Pause
		SendMessage(WM_RP_CONTROL_PAUSE,0,0);
		break;
	case IDM_MAIN_STOP:				//Stop
		SendMessage(WM_RP_CONTROL_STOP,0,0);
		break;
	case IDM_MAIN_NEXT:				//Next song
		SendMessage(WM_RP_CONTROL_NEXT,0,0);
		break;

	case IDM_MAIN_REPEAT:			//Repeat
		SendMessage(WM_RP_CONTROL_REPEAT,0,0);
		break;
	case IDM_MAIN_SHUFFLE:			//Shuffle
		SendMessage(WM_RP_CONTROL_SHUFFLE,0,0);
		break;

	case IDM_MAIN_VISMODE_WAVEFORM:				//Visualization mode
		SendMessage(WM_RP_CONTROL_VISUALMODE, 0,(LPARAM)oscilliscope);
		break;
	case IDM_MAIN_VISMODE_SPEC:
		SendMessage(WM_RP_CONTROL_VISUALMODE, 0,(LPARAM)spectrum);
		break;
	case IDM_MAIN_VISMODE_DISABLE:
		SendMessage(WM_RP_CONTROL_VISUALMODE, 0,(LPARAM)disable);
		break;

	case IDM_MAIN_VISMODE_REFRESHRATE_50:		//Refresh rate
		SendMessage(WM_RP_MENU_FPS, 0, (LPARAM)50);
		break;
	case IDM_MAIN_VISMODE_REFRESHRATE_25:
		SendMessage(WM_RP_MENU_FPS, 0, (LPARAM)25);
		break;
	case IDM_MAIN_VISMODE_REFRESHRATE_12:
		SendMessage(WM_RP_MENU_FPS, 0, (LPARAM)12);
		break;

	case IDM_MAIN_VISMODE_DOTSCOPE:				//Scope mode
		SendMessage(WM_RP_MENU_SCOPEMODE, 0, (LPARAM)dot);
		break;
	case IDM_MAIN_VISMODE_LINESCOPE:
		SendMessage(WM_RP_MENU_SCOPEMODE, 0, (LPARAM)line);
		break;
	case IDM_MAIN_VISMODE_SOLIDSCOPE:
		SendMessage(WM_RP_MENU_SCOPEMODE, 0, (LPARAM)solid);
		break;

	case IDM_MAIN_VISMODE_LINES:				//Analyzer mode
		SendMessage(WM_RP_MENU_ANALYZERMODE, 0, (LPARAM)lines);
		break;
	case IDM_MAIN_VISMODE_BARS:
		SendMessage(WM_RP_MENU_ANALYZERMODE, 0, (LPARAM)bars);
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
	m_bInHotKey = TRUE;

	CFileDialog *dialog;
	dialog = new CFileDialog(TRUE, 
				"ini",
				"",
				OFN_FILEMUSTEXIST    | 
				OFN_HIDEREADONLY     |
				OFN_EXPLORER,
				"Rainplay skin settings (*.ini)|*.ini|");
	//设定缺省寻找路径为Freeamp启动路径
	dialog->m_ofn.lpstrInitialDir = LPCTSTR(m_szStartupDir);

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

			//Change skin
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

			//Change font
			delete m_uifont.font1;
			delete m_uifont.font2;
			delete m_uifont.font3;
			m_uifont.font1 = new CFont();
			m_uifont.font2 = new CFont();
			m_uifont.font3 = new CFont();

			delete m_vis;
			m_vis = new CVisualView(this,
				m_i.fps, m_i.visualmode, m_i.analyzer, m_i.scope,
				coordinatesMain[AREA_DISPLAYSPECTRUM].x,
				coordinatesMain[AREA_DISPLAYSPECTRUM].y,
				coordinatesMain[AREA_DISPLAYSPECTRUM].iWidth,
				coordinatesMain[AREA_DISPLAYSPECTRUM].iHeight,
				m_pCMainUp);
			//Perpare UI fonts
			if (PerpareUIFont())
				m_uifont.bAvailable = TRUE;
			else
				m_uifont.bAvailable = FALSE;

			NotifySongName(m_i.szSongname);
		} else {
			delete cTmpSkinUp;
			delete cTmpSkinDown;
			AfxMessageBox("Can't change to this skin");
			SelectOldSkin();
		}
	}
	delete dialog;

	m_bInHotKey = FALSE;
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

	szFontName = oldszFontName;
	rgbDisplayColur = oldrgbDisplayColur;

	//Change font
	delete m_uifont.font1;
	delete m_uifont.font2;
	delete m_uifont.font3;
	m_uifont.font1 = new CFont();
	m_uifont.font2 = new CFont();
	m_uifont.font3 = new CFont();

	delete m_vis;
	m_vis = new CVisualView(this,
		m_i.fps, m_i.visualmode, m_i.analyzer, m_i.scope,
		coordinatesMain[AREA_DISPLAYSPECTRUM].x,
		coordinatesMain[AREA_DISPLAYSPECTRUM].y,
		coordinatesMain[AREA_DISPLAYSPECTRUM].iWidth,
		coordinatesMain[AREA_DISPLAYSPECTRUM].iHeight,
		m_pCMainUp);

	//Perpare UI fonts
	if (PerpareUIFont())
		m_uifont.bAvailable = TRUE;
	else
		m_uifont.bAvailable = FALSE;

	NotifySongName(m_i.szSongname);
	return TRUE;
}


LRESULT CRainplayDlg::OnKickIdle(WPARAM wParam, LPARAM lParam)
{
#if 0
	float percent,a,b;
	DWORD volume;
	unsigned short l,r;
	waveOutGetVolume( (HWAVEOUT)WAVE_MAPPER , &volume);
	l = HIWORD(volume);		r = LOWORD(volume);
	percent = (float)(l>r ? l : r)/(float)0xFFFF;
	a = (float)(l)/(float)0xFFFF;
	b = (float)(r)/(float)0xFFFF;
#endif
	return TRUE;
}


LRESULT CRainplayDlg::OnClickOpen(WPARAM wParam, LPARAM lParam)
{
	CFileDialog *dialog;
	CString szTemp;

	dialog = new CFileDialog(TRUE, 
				"mp3",
				"",
				OFN_FILEMUSTEXIST    | 
				OFN_HIDEREADONLY     |
				OFN_ALLOWMULTISELECT |
				OFN_EXPLORER,
				"All support files (.mp3, .m3u)|*.mp3;*.m3u|"
				"Mpeg Layer 3 Audio (.mp3)|*.mp3|"
				"Playlists (.m3u)|*.m3u|");
	dialog->m_ofn.lpstrTitle = LPCTSTR("Open Mpeg Audio File");

	//打开File open dialog
	if (dialog->DoModal()==IDOK) {
		POSITION pos = dialog->GetStartPosition();
		if (pos) {
			g_ui->m_plm->RemoveAll();
			do {
				szTemp = dialog->GetNextPathName(pos);
				if (szTemp.Right(3)=="M3U" ||
					szTemp.Right(3)=="m3u" ) {
					ReadPlaylistFromFile(szTemp, g_ui->m_plm);
				} else
					g_ui->m_plm->AddItem((char *)LPCTSTR(szTemp),0);
			} while(pos!=NULL);
			g_ui->m_plm->SetFirst();
			GetUILock();
			m_i.szSongname = "";
			ReleaseUILock();
			SendMessage(WM_RP_NOTIFY_SONGNAME, 0,0);
			if(g_ui->m_state == PlayerState_Playing ||
				g_ui->m_state == PlayerState_Paused) {
				g_ui->m_target->AcceptEvent(new Event(CMD_Stop));
				g_ui->m_target->AcceptEvent(new Event(CMD_Play));
			}
		}
	}
	delete dialog;
	return TRUE;
}


LRESULT CRainplayDlg::OnClickPlay(WPARAM wParam, LPARAM lParam)
{
	g_ui->m_target->AcceptEvent(new Event(CMD_Play));
	return TRUE;
}


LRESULT CRainplayDlg::OnClickPause(WPARAM wParam, LPARAM lParam)
{
	g_ui->m_target->AcceptEvent(new Event(CMD_TogglePause));
	return TRUE;
}


LRESULT CRainplayDlg::OnClickStop(WPARAM wParam, LPARAM lParam)
{
	g_ui->m_target->AcceptEvent(new Event(CMD_Stop));
	return TRUE;
}


LRESULT CRainplayDlg::OnClickPrev(WPARAM wParam, LPARAM lParam)
{
	GetUILock();
	if (m_i.szTime.Left(2)=="00" &&
		atoi(LPCTSTR(m_i.szTime.Right(2))) <=5) {
		//已经播放<=5秒
		if (m_i.indexOfSong > 1) {
			//不是song 1.
			g_ui->m_target->AcceptEvent(new Event(CMD_PrevMediaPiece));
		} else {
			if (g_ui->m_plm->GetRepeat() != REPEAT_NOT) {
				//是song 1.但不repeat
				g_ui->m_target->AcceptEvent(new Event(CMD_PrevMediaPiece));
			} else {
				//是song 1.同时repeat
				g_ui->m_target->AcceptEvent(new Event(CMD_Play));
			}
		}
	} else {
		g_ui->m_target->AcceptEvent(new Event(CMD_Play));
	}

	m_i.szTime = "00:00";
	ReleaseUILock();

	return TRUE;
}


LRESULT CRainplayDlg::OnClickNext(WPARAM wParam, LPARAM lParam)
{
	GetUILock();
	if ((m_i.indexOfSong != m_i.totalSongs)) {
		g_ui->m_target->AcceptEvent(new Event(CMD_NextMediaPiece));
		m_i.szTime = "00:00";
	} else if (g_ui->m_plm->GetRepeat() !=REPEAT_NOT ) {
		g_ui->m_target->AcceptEvent(new Event(CMD_NextMediaPiece));
		m_i.szTime = "00:00";
	}
	ReleaseUILock();
	return TRUE;
}


LRESULT CRainplayDlg::OnClickShuffle(WPARAM wParam, LPARAM lParam)
{
	g_ui->m_plm->ToggleShuffle();
	m_bShuffleControlDisplay = !m_bShuffleControlDisplay;
	InvalidateRect(AREA_RECT(AREA_DISPLAYSHUFFLE), FALSE);	
	return TRUE;
}


LRESULT CRainplayDlg::OnClickRepeat(WPARAM wParam, LPARAM lParam)
{
	if ( !m_bRepeatControlDisplay )
		g_ui->m_plm->SetRepeat(REPEAT_ALL);
	else
		g_ui->m_plm->SetRepeat(REPEAT_NOT);
	m_bRepeatControlDisplay = !m_bRepeatControlDisplay;
	InvalidateRect(AREA_RECT(AREA_DISPLAYREPEAT), FALSE);
	return TRUE;
}



LRESULT CRainplayDlg::OnClickEQ(WPARAM wParam, LPARAM lParam)
{
	if (!m_bEQDlgActive) {
		m_EQDlg = new CEQDlg(&m_bEQDlgActive);
		m_EQDlg->Create(IDD_EQ);
		m_bEQDlgActive = TRUE;
	}
	return TRUE;
}


LRESULT CRainplayDlg::OnClickPlayList(WPARAM wParam, LPARAM lParam)
{
	if (!m_bPlDlgActive) {
		m_playlistDlg = new CPlayListDlg(&m_bPlDlgActive);
		m_playlistDlg->Create(IDD_PLAYLIST);
		m_bPlDlgActive = TRUE;
	}
	return TRUE;
}


LRESULT CRainplayDlg::OnClickMinimize(WPARAM wParam, LPARAM lParam)
{
	ShowWindow(SW_MINIMIZE);
	return TRUE;
}


LRESULT CRainplayDlg::OnClickAbout(WPARAM wParam, LPARAM lParam)
{
	m_aboutDlg->DoModal();
	return TRUE;
}


LRESULT CRainplayDlg::OnClickQuit(WPARAM wParam, LPARAM lParam)
{
	EndDialog(TRUE);
	return TRUE;
}


LRESULT CRainplayDlg::OnClickVisualMode(WPARAM wParam, LPARAM lParam)
{
	if(g_ui->m_state!=PlayerState_Playing)
		return TRUE;
	switch((VISMODE)lParam) {
	case flip:						//User click area spectrum to filp the display mode
		switch(m_i.visualmode) {
		case disable:
			m_i.visualmode = oscilliscope;
			m_vis->Start();
			break;
		case oscilliscope:
			m_i.visualmode = spectrum;
			break;
		case spectrum:
			m_i.visualmode = disable;
			m_vis->Stop();
			InvalidateRect(AREA_RECT(AREA_DISPLAYSPECTRUM), FALSE);
			break;
		}
		break;
	case oscilliscope:				//Waveform display
		m_vis->Start();
		m_i.visualmode = oscilliscope;
		break;
	case spectrum:					//Spectrum display
		m_vis->Start();
		m_i.visualmode = spectrum;
		break;
	case disable:					//display visual display
		m_vis->Stop();
		m_i.visualmode = disable;
		InvalidateRect(AREA_RECT(AREA_DISPLAYSPECTRUM), FALSE);
		break;
	default:
		break;
	}
	m_vis->SetVisualMode(m_i.visualmode);
	return TRUE;
}


LRESULT	CRainplayDlg::OnClickSeek(WPARAM wParam, LPARAM lParam)
{
	int pos = (int)lParam - coordinatesMain[AREA_SEEKBAR].x;
	pos = (int)((float)m_i.totalframe*((float)pos/(float)coordinatesMain[AREA_SEEKBAR].iWidth));
	if(g_ui->m_state == PlayerState_Playing)
		g_ui->m_target->AcceptEvent(new ChangePositionEvent(pos));
	return TRUE;
}


LRESULT CRainplayDlg::OnScopeMode(WPARAM wParam, LPARAM lParam)
{
	GetUILock();
	m_i.scope = (SCOPEMODE)lParam;
	ReleaseUILock();
	m_vis->SetScopemode(m_i.scope);
	return TRUE;
}


LRESULT CRainplayDlg::OnChangeAnalyzerMode(WPARAM wParam, LPARAM lParam)
{
	GetUILock();
	m_i.analyzer = (ANALYZERMODE)lParam;
	m_vis->SetAnalyzerMode(m_i.analyzer);
	ReleaseUILock();
	return TRUE;
}



LRESULT CRainplayDlg::OnChangeFps(WPARAM wParam, LPARAM lParam)
{
	GetUILock();
	m_i.fps = (int)lParam;
	m_vis->SetFps(m_i.fps);
	ReleaseUILock();
	return TRUE;
}


LRESULT CRainplayDlg::OnNotifyTime(WPARAM wParam, LPARAM lParam)
{
	GetUILock();
	UpdateSeekBar();
	TransParentDisplay(
		coordinatesMain[AREA_DISPLAYTIME].x,
		coordinatesMain[AREA_DISPLAYTIME].y,
		coordinatesMain[AREA_DISPLAYTIME].iWidth,
		coordinatesMain[AREA_DISPLAYTIME].iHeight,
		0,
		m_uifont.font2, m_i.szTime, rgbDisplayColur );
	ReleaseUILock();
	return TRUE;
}


LRESULT CRainplayDlg::OnNotifySongName(WPARAM wParam, LPARAM lParam)
{
	GetUILock();
	TransParentDisplay(
		coordinatesMain[AREA_DISPLAYBAR].x,
		coordinatesMain[AREA_DISPLAYBAR].y,
		coordinatesMain[AREA_DISPLAYBAR].iWidth,
		coordinatesMain[AREA_DISPLAYBAR].iHeight,
		m_i.scrollOffset,
		m_uifont.font1, m_i.szSongname, rgbDisplayColur );
	ReleaseUILock();
	return TRUE;
}


LRESULT CRainplayDlg::OnNotifyMPEGInfo(WPARAM wParam, LPARAM lParam)
{
	GetUILock();
	TransParentDisplay(
		coordinatesMain[AREA_DISPLAYBITRATE].x,
		coordinatesMain[AREA_DISPLAYBITRATE].y,
		coordinatesMain[AREA_DISPLAYBITRATE].iWidth,
		coordinatesMain[AREA_DISPLAYBITRATE].iHeight,
		0,
		m_uifont.font3, m_i.szBitrate, rgbDisplayColur );
	TransParentDisplay(
		coordinatesMain[AREA_DISPLAYKHZ].x,
		coordinatesMain[AREA_DISPLAYKHZ].y,
		coordinatesMain[AREA_DISPLAYKHZ].iWidth,
		coordinatesMain[AREA_DISPLAYKHZ].iHeight,
		0,
		m_uifont.font3, m_i.szSamplerate, rgbDisplayColur );
	ReleaseUILock();
	return TRUE;
}


LRESULT CRainplayDlg::OnNotifyVisData(WPARAM wParam, LPARAM lParam)
{
	return(m_vis->VisDataReady((unsigned int)lParam, (unsigned char *)wParam));
}


void CRainplayDlg::OnDestroy() 
{
	// TODO: Add your message handler code here and/or call default
	//Kill visual view control
	m_vis->Kill();

	//SavePlayListToFile(m_szStartupDir+"FREEAMP.M3U", g_ui->m_plm );
	m_bInitUIOk = FALSE;
	m_uifont.bAvailable = FALSE;

	CDialog::OnDestroy();
}


void CRainplayDlg::OnTimer(UINT nIDEvent) 
{
	static int increment = 1;
	// TODO: Add your message handler code here and/or call default
#define	AREAWIDTH	(UINT)coordinatesMain[AREA_DISPLAYBAR].iWidth

	switch(nIDEvent) {
	case TIMER_SONGNAME_SCROLL:
		GetUILock();
		if( increment > 0 &&
			m_i.length - m_i.scrollOffset + 3< AREAWIDTH)
		{
			increment = -1;
		}
		else if( increment < 0 &&
				 m_i.scrollOffset <= 0)
		{   
			increment = 1;
		}

		m_i.scrollOffset += increment;
		ReleaseUILock();

		//UpdateDisplay
		OnNotifySongName(0,0);
		break;
	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
#undef	AREAWIDTH
}


void CRainplayDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default
	char szTemp[MAX_PATH];

	g_ui->m_plm->RemoveAll();
	for (UINT i=0; i<::DragQueryFile((HDROP) hDropInfo, 0xFFFFFFFF, NULL, 0); i++) {
		::DragQueryFile(hDropInfo, i, szTemp, MAX_PATH);
		g_ui->m_plm->AddItem(szTemp,0);
	}
	g_ui->m_plm->SetFirst();
	GetUILock();
	m_i.szSongname = "";
	ReleaseUILock();
	SendMessage(WM_RP_NOTIFY_SONGNAME, 0,0);
	if(g_ui->m_state == PlayerState_Playing ||
		g_ui->m_state == PlayerState_Paused) {
		g_ui->m_target->AcceptEvent(new Event(CMD_Stop));
		g_ui->m_target->AcceptEvent(new Event(CMD_Play));
	}

	CDialog::OnDropFiles(hDropInfo);
}


BOOL CRainplayDlg::PerpareUIFont()
{
	//准备字体
	CClientDC dc(this);
	if (!EnumUIFont(&dc, &m_uifont.logfont, szFontName))
		return FALSE;

	//Large
	m_uifont.logfont.lfHeight = (long)(coordinatesMain[AREA_DISPLAYBAR].iHeight);
	m_uifont.logfont.lfWidth =  0;
	m_uifont.logfont.lfWeight = FW_BOLD;
	m_uifont.font1->CreateFontIndirect(&m_uifont.logfont);

	//Medium
	m_uifont.logfont.lfHeight = (long)(coordinatesMain[AREA_DISPLAYTIME].iHeight);
	m_uifont.logfont.lfWidth = 0;
	m_uifont.logfont.lfWeight = FW_BOLD;
	m_uifont.font2->CreateFontIndirect(&m_uifont.logfont);

	//Small
	m_uifont.logfont.lfHeight = (long)(coordinatesMain[AREA_DISPLAYBITRATE].iHeight);
	m_uifont.logfont.lfWidth = 0;
	m_uifont.logfont.lfWeight = FW_NORMAL;
	m_uifont.font3->CreateFontIndirect(&m_uifont.logfont);

	return TRUE;
}


BOOL CRainplayDlg::TransParentDisplay(int srcX, int srcY,
									  int width, int height, int scrollOffset, 
									  CFont *font, CString string, COLORREF rgb)
{
	//为避免在UI杀掉后，掉用此函数。
	//hack code
	if (!m_bInitUIOk)// ||
		//this->m_hWnd==NULL)
		return FALSE;

	CDC *memDC, *memBMDC;
	CClientDC *dc;
	CBitmap *memBM, *oldBM, *oldBM1;
	CFont *oldfont;

	dc = new CClientDC(this);
	memDC = new CDC();
	memBMDC = new CDC();
	memBM = new CBitmap();

	//准备back surface
	memDC->CreateCompatibleDC(dc);
	memBM->CreateCompatibleBitmap(dc, width, height);
	oldBM = memDC->SelectObject(memBM);

	//准备background
	memBMDC->CreateCompatibleDC(dc);
	oldBM1 = memBMDC->SelectObject(&(m_pCMainUp->m_cBmp));

	//向memDC复制背景
	memDC->BitBlt(	0,0, width,	height,
					memBMDC,
					srcX,srcY,
					SRCCOPY );

	//向memDC显示偏移字符串
	if (m_uifont.bAvailable) {
		CRect rect(0-scrollOffset, 0, width, height);
		oldfont = memDC->SelectObject(font);
		memDC->SetTextColor(rgb);
		memDC->SetBkMode(TRANSPARENT);
		memDC->DrawText(string, LPRECT(rect),
						DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		memDC->SelectObject(oldfont);
	}

	//Flip Surface:Backsurface -> Frontsurface
	dc->BitBlt(	srcX, srcY, width, height,
				memDC,
				0,0,
				SRCCOPY);

	memBMDC->SelectObject(oldBM1);
	delete memBMDC;
	memDC->SelectObject(oldBM);
	delete memBM;
	delete memDC;
	delete dc;

	return TRUE;
}


BOOL CRainplayDlg::SongNameNeedToScroll(void)
{
	CFont *oldfont;
	CClientDC dc(this);
#define	AREAWIDTH	(UINT)coordinatesMain[AREA_DISPLAYBAR].iWidth

	if (m_uifont.bAvailable && !m_i.szSongname.IsEmpty()) {
		oldfont = dc.SelectObject(m_uifont.font1);
		CSize size = dc.GetTextExtent(LPCTSTR(m_i.szSongname));
		dc.SelectObject(oldfont);
		m_i.length = size.cx;
		return m_i.length>(AREAWIDTH) ? TRUE : FALSE;
	} else
		return FALSE;
#undef	AREAWIDTH
}


BOOL CRainplayDlg::NotifyTotalTime(int seconds)
{
	GetUILock();
	m_i.totaltime = seconds;
	ReleaseUILock();
	return TRUE;
}


BOOL CRainplayDlg::NotifyTime(int seconds)
{
	int iHour = seconds / 3600;
	int iMin = (seconds-iHour*3600) / 60;
	int iSec = seconds - iHour*3600 - iMin*60;

	CString szTemp = "00:00";
	szTemp.Format("%02d:%02d", iMin,iSec);
	GetUILock();
	if (szTemp!=m_i.szTime) {
		m_i.szTime = szTemp;
		m_i.currenttime = seconds;
		ReleaseUILock();
		if (m_bInitUIOk)
			SendMessage(WM_RP_NOTIFY_TIME, 0,0);
		return TRUE;
	}
	else {
		ReleaseUILock();
		return FALSE;
	}
}


BOOL CRainplayDlg::UpdateSeekBar(void)
{
	float dx = (float)m_i.currenttime/(float)m_i.totaltime;
	dx *= (float)coordinatesMain[AREA_SEEKBAR].iWidth;
	m_pCMainDown->BmpBlt(FALSE, this,
		coordinatesMain[AREA_SEEKBAR].x,
		coordinatesMain[AREA_SEEKBAR].y,
		(int)dx,
		coordinatesMain[AREA_SEEKBAR].iHeight,
		coordinatesMain[AREA_SEEKBAR].x,
		coordinatesMain[AREA_SEEKBAR].y );
	m_pCMainUp->BmpBlt(FALSE, this,
		coordinatesMain[AREA_SEEKBAR].x+(int)dx,
		coordinatesMain[AREA_SEEKBAR].y,
		coordinatesMain[AREA_SEEKBAR].iWidth - (int)dx,
		coordinatesMain[AREA_SEEKBAR].iHeight,
		coordinatesMain[AREA_SEEKBAR].x+(int)dx,
		coordinatesMain[AREA_SEEKBAR].y );

	return TRUE;
}


BOOL CRainplayDlg::NotifySongName(CString string)
{
	GetUILock();
	m_i.szSongname = string;
	ReleaseUILock();
	
	if (m_bInitUIOk) {
		if (SongNameNeedToScroll())
			SetTimer(TIMER_SONGNAME_SCROLL, 200, NULL);
		else {
			KillTimer(TIMER_SONGNAME_SCROLL);
			m_i.scrollOffset = 0;
		}
		SendMessage(WM_RP_NOTIFY_SONGNAME, 0,0);
	}
	return TRUE;
}


BOOL CRainplayDlg::NotifyMPEGInfo(int bitrate, long samplerate, unsigned int totalframe)
{
	CString szTemp = "128";
	szTemp.Format("%03d",bitrate);
	GetUILock();
	m_i.totalframe = totalframe;
	m_i.szBitrate = szTemp;
	szTemp.Format("%02d",samplerate/1000);
	m_i.szSamplerate = szTemp;
	ReleaseUILock();
	if (m_bInitUIOk) {
		SendMessage(WM_RP_NOTIFY_MPEGINFO, 0,0);
		m_vis->SetSampleRate(samplerate);
	}
	return TRUE;
}


BOOL CRainplayDlg::NotifyNumSongs(int indexOfSong, int totalSongs)
{
	GetUILock();
	m_i.indexOfSong = indexOfSong;
	m_i.totalSongs = totalSongs;
	ReleaseUILock();
	return TRUE;
}

BOOL CRainplayDlg::NotifyVisDataReady(unsigned int length, unsigned char *buf)
{
	if (m_bInitUIOk)
		SendMessage(WM_RP_NOTIFY_VISDATA, (WPARAM)buf,(LPARAM)length);
	return TRUE;
}
// RainplayDlg.h : header file
//

#if !defined(AFX_RAINPLAYDLG_H__FE778D28_2E43_11D2_B244_B8AECE919F11__INCLUDED_)
#define AFX_RAINPLAYDLG_H__FE778D28_2E43_11D2_B244_B8AECE919F11__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Bmpsize.h"
#include "PlayListDlg.h"
#include "EQDlg.h"
#include "AboutDlg.h"
#include "VisualView.h"

//Scroll songname timer
#define	TIMER_SONGNAME_SCROLL	0x01

//Define the hotkeys
#define	ALT_S		0x0000
#define	ALT_A		0x0001
#define	ALT_X		0x0002
#define	ALT_G		0x0003
#define	ALT_E		0x0004

#define	WM_RAINPLAY_CONTROL_MESSAGE			WM_USER + 0
#define	WM_RP_CONTROL_OPEN			AREA_OPEN				+WM_RAINPLAY_CONTROL_MESSAGE
#define	WM_RP_CONTROL_PLAY			AREA_PLAY				+WM_RAINPLAY_CONTROL_MESSAGE
#define	WM_RP_CONTROL_PAUSE			AREA_PAUSE				+WM_RAINPLAY_CONTROL_MESSAGE
#define	WM_RP_CONTROL_STOP			AREA_STOP				+WM_RAINPLAY_CONTROL_MESSAGE
#define	WM_RP_CONTROL_PREV			AREA_PREV				+WM_RAINPLAY_CONTROL_MESSAGE
#define	WM_RP_CONTROL_NEXT			AREA_NEXT				+WM_RAINPLAY_CONTROL_MESSAGE
#define	WM_RP_CONTROL_SHUFFLE		AREA_SHUFFLE			+WM_RAINPLAY_CONTROL_MESSAGE
#define	WM_RP_CONTROL_REPEAT		AREA_REPEAT				+WM_RAINPLAY_CONTROL_MESSAGE
#define	WM_RP_CONTROL_EQ			AREA_EQ					+WM_RAINPLAY_CONTROL_MESSAGE
#define	WM_RP_CONTROL_PLAYLIST		AREA_PLAYLIST			+WM_RAINPLAY_CONTROL_MESSAGE
#define	WM_RP_CONTROL_MINIMIZE		AREA_MINIMIZE			+WM_RAINPLAY_CONTROL_MESSAGE
#define	WM_RP_CONTROL_ABOUT			AREA_ABOUT				+WM_RAINPLAY_CONTROL_MESSAGE
#define	WM_RP_CONTROL_QUIT			AREA_QUIT				+WM_RAINPLAY_CONTROL_MESSAGE

//Click on !botton
//WPARAM=0				LPARAM=enum VISMODE
#define	WM_RP_CONTROL_VISUALMODE	AREA_DISPLAYSPECTRUM	+WM_RAINPLAY_CONTROL_MESSAGE
//WPARAM=0				LPARAM=new position
#define	WM_RP_CONTROL_SEEK			AREA_SEEKBAR			+WM_RAINPLAY_CONTROL_MESSAGE

//Menu message
#define	WM_RAINPLAY_MENU_MESSAGE			WM_USER + 100
//WPARAM=0				LPARAM=enum SCOPEMODE
#define	WM_RP_MENU_SCOPEMODE	0				+WM_RAINPLAY_MENU_MESSAGE
//WPARAM=0				LPARAM=fps(50,25,12)
#define	WM_RP_MENU_FPS			1				+WM_RAINPLAY_MENU_MESSAGE

//Notify from UI to Dlg
#define	WM_RAINPLAY_NOTIFY_MESSAGE			WM_USER + 200
#define	WM_RP_NOTIFY_TIME		0				+WM_RAINPLAY_NOTIFY_MESSAGE
#define	WM_RP_NOTIFY_SONGNAME	1				+WM_RAINPLAY_NOTIFY_MESSAGE
#define	WM_RP_NOTIFY_MPEGINFO	2				+WM_RAINPLAY_NOTIFY_MESSAGE

//WPARAM = *buf		LPARAM = *length
#define	WM_RP_NOTIFY_VISDATA	3				+WM_RAINPLAY_NOTIFY_MESSAGE

/////////////////////////////////////////////////////////////////////////////
// CRainplayDlg dialog

class CRainplayDlg : public CDialog
{
// Construction
public:
	CRainplayDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CRainplayDlg();

	typedef struct _tagUIFONT {
		BOOL bAvailable;
		LOGFONT logfont;
		CFont *font1;			//Large
		CFont *font2;			//Medium
		CFont *font3;			//Small
	} UIFONT, *LPUIFONT;

	typedef enum _tagVisMode {
		flip,
		oscilliscope,
		spectrum,
		disable
	} VISMODE;

	typedef struct _tagDispInfo {
		CString szBitrate;
		CString	szSamplerate;
		CString szTime;
		CString szSongname;
		UINT indexOfSong;
		UINT totalSongs;
		UINT scrollOffset;
		UINT length;
		int	totaltime;
		int currenttime;
		unsigned int totalframe;
		VISMODE visualmode;
		int fps;
		SCOPEMODE scope;
	} DISPINFO, *LPDISPINFO;
	BOOL NotifyTotalTime(int seconds);
	BOOL NotifyTime(int seconds);
	BOOL NotifySongName(CString string);
	BOOL NotifyMPEGInfo(int bitrate, float samplerate, unsigned int totalframe);
	BOOL NotifyNumSongs(int indexOfSong, int totalSongs);
	BOOL NotifyVisDataReady(unsigned int length, unsigned char *buf);

// Dialog Data
	//{{AFX_DATA(CRainplayDlg)
	enum { IDD = IDD_RAINPLAY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRainplayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void OnCancel() { return; }			//To override the shit mfc's ESC process.
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL SongNameNeedToScroll(void);
	BOOL TransParentDisplay(int srcX, int srcY,
							int width, int height, int scrollOffset, 
							CFont *font, CString string, COLORREF rgb);
	BOOL UpdateSeekBar(void);
	BOOL SelectSkin(void);
	BOOL SelectOldSkin(void);
	BOOL GetSkinSettings(CString szFile);
	BOOL PerpareUIFont();
	BOOL SetVisualizationMode( VISMODE mode );

	// Generated message map functions
	//{{AFX_MSG(CRainplayDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteIsChanging( CWnd* pRealizeWnd );
	afx_msg void OnPaletteChanged( CWnd* pFocusWnd );
	afx_msg LRESULT OnHotKey(WPARAM, LPARAM);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT	OnKickIdle(WPARAM, LPARAM);
	afx_msg LRESULT OnClickOpen(WPARAM, LPARAM);
	afx_msg LRESULT OnClickPlay(WPARAM, LPARAM);
	afx_msg LRESULT OnClickPause(WPARAM, LPARAM);
	afx_msg LRESULT OnClickStop(WPARAM, LPARAM);
	afx_msg LRESULT OnClickPrev(WPARAM, LPARAM);
	afx_msg LRESULT OnClickNext(WPARAM, LPARAM);
	afx_msg LRESULT OnClickShuffle(WPARAM, LPARAM);
	afx_msg LRESULT OnClickRepeat(WPARAM, LPARAM);
	afx_msg LRESULT OnClickPlayList(WPARAM, LPARAM);
	afx_msg LRESULT OnClickEQ(WPARAM, LPARAM);
	afx_msg LRESULT OnClickMinimize(WPARAM, LPARAM);
	afx_msg LRESULT OnClickAbout(WPARAM, LPARAM);
	afx_msg LRESULT OnClickQuit(WPARAM, LPARAM);
	afx_msg LRESULT OnClickVisualMode(WPARAM, LPARAM);
	afx_msg LRESULT	OnClickSeek(WPARAM, LPARAM);

	afx_msg LRESULT	OnScopeMode(WPARAM, LPARAM);
	afx_msg LRESULT OnChangeFps(WPARAM, LPARAM);

	afx_msg LRESULT OnNotifyTime(WPARAM, LPARAM);
	afx_msg LRESULT OnNotifySongName(WPARAM, LPARAM);
	afx_msg LRESULT OnNotifyMPEGInfo(WPARAM, LPARAM);
	afx_msg LRESULT OnNotifyVisData(WPARAM, LPARAM);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CEQDlg * m_EQDlg;						//Equalizer dialog
	CPlayListDlg * m_playlistDlg;			//Play list manager dialog
	CAboutDlg * m_aboutDlg;					//About dialog
	CVisualView * m_vis;					//Visual display

	BOOL m_bInitUIOk;
	BOOL m_bEQDlgActive;
	BOOL m_bPlDlgActive;
	UIFONT m_uifont;
	DISPINFO m_i;
	CBmpSize * m_pCMainDown;
	CBmpSize * m_pCMainUp;
	HICON m_hIcon;
	BOOL m_bInHotKey;
	BOOL m_bRepeatControlDisplay;		//Does the repeat string displayed?
	BOOL m_bShuffleControlDisplay;		//Does the shuffle string displayed?
	POINT m_AcutalPoint;
	CRect m_rectHotArea;
	int m_iCurAreaIdx;
	BOOL m_bNotInHotArea;
	CString m_szStartupDir;			//the current directory when the Freeamp starting up
	
	Mutex * m_Mutex;
	void GetUILock() { m_Mutex->Acquire(WAIT_FOREVER); }
	void ReleaseUILock() { m_Mutex->Release(); }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAINPLAYDLG_H__FE778D28_2E43_11D2_B244_B8AECE919F11__INCLUDED_)

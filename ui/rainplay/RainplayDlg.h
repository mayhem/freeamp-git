// RainplayDlg.h : header file
//

#if !defined(AFX_RAINPLAYDLG_H__FE778D28_2E43_11D2_B244_B8AECE919F11__INCLUDED_)
#define AFX_RAINPLAYDLG_H__FE778D28_2E43_11D2_B244_B8AECE919F11__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Bmpsize.h"

//Define the hotkeys
#define	ALT_S		0x0000
#define	ALT_A		0x0001
#define	ALT_X		0x0002

/////////////////////////////////////////////////////////////////////////////
// CRainplayDlg dialog

class CRainplayDlg : public CDialog
{
// Construction
public:
	CRainplayDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CRainplayDlg();
// Dialog Data
	//{{AFX_DATA(CRainplayDlg)
	enum { IDD = IDD_RAINPLAY_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRainplayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL SelectSkin(void);
	BOOL SelectOldSkin(void);
	CBmpSize * m_pCMainDown;
	CBmpSize * m_pCMainUp;
	HICON m_hIcon;

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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL bInHotKey;
	BOOL GetSkinSettings(CString szFile);
	POINT m_AcutalPoint;
	CRect m_rectHotArea;
	int m_iCurAreaIdx;
	BOOL m_bNotInHotArea;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAINPLAYDLG_H__FE778D28_2E43_11D2_B244_B8AECE919F11__INCLUDED_)

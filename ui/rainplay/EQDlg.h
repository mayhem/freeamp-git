#if !defined(AFX_EQDLG_H__FDC908C4_A8C5_11D2_B245_D4C1E452A178__INCLUDED_)
#define AFX_EQDLG_H__FDC908C4_A8C5_11D2_B245_D4C1E452A178__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// EQDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEQDlg dialog

class CEQDlg : public CDialog
{
// Construction
public:
	CEQDlg(BOOL* bActive, CWnd* pParent = NULL);   // standard constructor
#define	_RAINPLAY_UI_
#ifdef	_RAINPLAY_UI_
	float equalizer[32];
#endif	//_RAINPLAY_UI_
#undef	_RAINPLAY_UI_
private:
	BOOL* m_pbActive;
	CSliderCtrl m_eq0, m_eq1, m_eq2, m_eq3, m_eq4,
				m_eq5, m_eq6, m_eq7, m_eq8, m_eq9;
// Dialog Data
	//{{AFX_DATA(CEQDlg)
	enum { IDD = IDD_EQ };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEQDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEQDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEqzero();
	afx_msg void OnClose();
	afx_msg void OnEnableEQ();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EQDLG_H__FDC908C4_A8C5_11D2_B245_D4C1E452A178__INCLUDED_)

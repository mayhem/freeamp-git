#if !defined(AFX_PLAYLISTDLG_H__B3210120_A496_11D2_A20E_0000E810C1B3__INCLUDED_)
#define AFX_PLAYLISTDLG_H__B3210120_A496_11D2_A20E_0000E810C1B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlayListDlg dialog

class CPlayListDlg : public CDialog
{
// Construction
public:
	CPlayListDlg(BOOL* bActive, CWnd* pParent = NULL);   // standard constructor
private:
	BOOL* m_pbActive;
// Dialog Data
	//{{AFX_DATA(CPlayListDlg)
	enum { IDD = IDD_PLAYLIST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlayListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPlaylistClose();
	afx_msg void OnPlaylistLoad();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYLISTDLG_H__B3210120_A496_11D2_A20E_0000E810C1B3__INCLUDED_)

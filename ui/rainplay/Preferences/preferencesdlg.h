#if !defined(AFX_PREFERENCESDLG_H__FDC51B00_B132_11D2_B134_0000E8631F1D__INCLUDED_)
#define AFX_PREFERENCESDLG_H__FDC51B00_B132_11D2_B134_0000E8631F1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesDlg.h : header file
//
#include "optionsetupdlg.h"
#include "preferences.h"

#define	TOTAL_PAGES				4
#define	ID_PAGE_SETUP			0
#define	ID_PAGE_OPTIONS			1
#define	ID_PAGE_VISUALIZATION	2
#define	ID_PAGE_PLUGINS			3

/////////////////////////////////////////////////////////////////////////////
// CPreferencesDlg dialog

class CPreferencesDlg : public CDialog
{
// Construction
public:
	CPreferencesDlg(CWnd* pParent = NULL);   // standard constructor
    void SetPreferences(Preferences* prefs) { m_prefs = prefs;}
protected:
	COptionSetupDlg tabpage_setup;
	CDialog		tabpage_options;
	CDialog		tabpage_visualization;
	CDialog		tabpage_plugins;
    Preferences* m_prefs;
// Dialog Data
	//{{AFX_DATA(CPreferencesDlg)
	enum { IDD = IDD_PREFERENCES };
	CTabCtrl	m_options;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL SetDefaultUI(CString szDefaultUI);
	// Generated message map functions
	//{{AFX_MSG(CPreferencesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeOptions(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnApply();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESDLG_H__FDC51B00_B132_11D2_B134_0000E8631F1D__INCLUDED_)

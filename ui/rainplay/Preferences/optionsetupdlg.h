#if !defined(AFX_OPTIONSETUPDLG_H__57FF4663_B13B_11D2_B134_0000E8631F1D__INCLUDED_)
#define AFX_OPTIONSETUPDLG_H__57FF4663_B13B_11D2_B134_0000E8631F1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// optionsetupdlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionSetupDlg dialog

class COptionSetupDlg : public CDialog
{
// Construction
public:
	COptionSetupDlg(CWnd* pParent = NULL);   // standard constructor
	CString m_szDefaultUI;
private:
	BOOL RegisterMPxFile(CString fileext, CString filetype, CString descri);
	BOOL WriteStrRegister(HKEY kMainKey, CString szSubKey, CString szValue);
// Dialog Data
	//{{AFX_DATA(COptionSetupDlg)
	enum { IDD = IDD_OPTION_SETUP };
	CComboBox	m_plugins;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionSetupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRegisterFiles();
	afx_msg void OnSelchangeSelUi();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSETUPDLG_H__57FF4663_B13B_11D2_B134_0000E8631F1D__INCLUDED_)

// PreferencesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rainplay.h"
#include "PreferencesDlg.h"
#include "preferences.h"
#include "errors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferencesDlg dialog


CPreferencesDlg::CPreferencesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPreferencesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreferencesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPreferencesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesDlg)
	DDX_Control(pDX, IDC_OPTIONS, m_options);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreferencesDlg, CDialog)
	//{{AFX_MSG_MAP(CPreferencesDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_OPTIONS, OnSelchangeOptions)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesDlg message handlers

BOOL CPreferencesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CenterWindow();

	TC_ITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_TEXT;
	TabCtrlItem.pszText = "Setup";
	m_options.InsertItem( ID_PAGE_SETUP, &TabCtrlItem );
	TabCtrlItem.pszText = "Options";
	m_options.InsertItem( ID_PAGE_OPTIONS, &TabCtrlItem );
	TabCtrlItem.pszText = "Visualization";
	m_options.InsertItem( ID_PAGE_VISUALIZATION, &TabCtrlItem );
	TabCtrlItem.pszText = "Plugins";
	m_options.InsertItem( ID_PAGE_PLUGINS, &TabCtrlItem );

	tabpage_setup.Create(IDD_OPTION_SETUP, this);
	tabpage_options.Create(IDD_OPTION_OPTIONS, this);
	tabpage_visualization.Create(IDD_OPTION_VISUALIZATION, this);
	tabpage_plugins.Create(IDD_OPTION_PLUGINS, this);

	tabpage_setup.SetWindowPos(NULL,25,35,0,0, SWP_NOSIZE | SWP_NOZORDER);
	tabpage_options.SetWindowPos(NULL,25,35,0,0, SWP_NOSIZE | SWP_NOZORDER);
	tabpage_visualization.SetWindowPos(NULL,25,35,0,0, SWP_NOSIZE | SWP_NOZORDER);
	tabpage_plugins.SetWindowPos(NULL,25,35,0,0, SWP_NOSIZE | SWP_NOZORDER);

	tabpage_setup.ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferencesDlg::OnSelchangeOptions(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nOption = m_options.GetCurSel();
	switch(nOption) {
	case ID_PAGE_SETUP:
		ASSERT(tabpage_setup.m_hWnd!=NULL);
		tabpage_setup.ShowWindow(SW_SHOW);
		tabpage_options.ShowWindow(SW_HIDE);
		tabpage_visualization.ShowWindow(SW_HIDE);
		tabpage_plugins.ShowWindow(SW_HIDE);
		break;
	case ID_PAGE_OPTIONS:
		ASSERT(tabpage_options.m_hWnd!=NULL);
		tabpage_setup.ShowWindow(SW_HIDE);
		tabpage_options.ShowWindow(SW_SHOW);
		tabpage_visualization.ShowWindow(SW_HIDE);
		tabpage_plugins.ShowWindow(SW_HIDE);
		break;
	case ID_PAGE_VISUALIZATION:
		ASSERT(tabpage_visualization.m_hWnd!=NULL);
		tabpage_setup.ShowWindow(SW_HIDE);
		tabpage_options.ShowWindow(SW_HIDE);
		tabpage_visualization.ShowWindow(SW_SHOW);
		tabpage_plugins.ShowWindow(SW_HIDE);
		break;
	case ID_PAGE_PLUGINS:
		ASSERT(tabpage_plugins.m_hWnd!=NULL);
		tabpage_setup.ShowWindow(SW_HIDE);
		tabpage_options.ShowWindow(SW_HIDE);
		tabpage_visualization.ShowWindow(SW_HIDE);
		tabpage_plugins.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}
	*pResult = 0;
}

void CPreferencesDlg::OnApply() 
{
	// TODO: Add your control notification handler code here
	SetDefaultUI(tabpage_setup.m_szDefaultUI);
	return;
}

BOOL CPreferencesDlg::SetDefaultUI(CString szDefaultUI)
{
    int32 length = szDefaultUI.GetLength();
    char* ui = szDefaultUI.GetBuffer(length);

    if(IsError(m_prefs->SetDefaultUI(ui)))
    {
        return FALSE;
    }

	return TRUE;
}

void CPreferencesDlg::OnOK() 
{
	// TODO: Add extra validation here
	OnApply();	
	CDialog::OnOK();
}

/***************************************************************/
//FileName:	AboutDlg.cpp
//Function:	Implement the rainplay ui about
//Author:	Bill Yuan
//EMail:	rainman_yuan@hotmail.com
//			rainplay@263.net
//WWW:		http://rainman_yuan.yeah.net
//			http://rainplay.yeah.net
//Date:		Jan/07/1999
/***************************************************************/
// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rainplay.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog


CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCAbout = new CBmpSize(IDB_MAINABOUT);
}
CAboutDlg::~CAboutDlg()
{
	delete m_pCAbout;
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowPos(NULL, 0,0,
		m_pCAbout->m_iWidth, m_pCAbout->m_iHeight,
		SWP_SHOWWINDOW);
	CenterWindow(NULL);
	m_pCAbout->BmpBlt( FALSE, this, 0, 0, 
		m_pCAbout->m_iWidth, m_pCAbout->m_iHeight,
		0,0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	EndDialog(TRUE);
	//CDialog::OnLButtonDown(nFlags, point);
}

void CAboutDlg::OnPaint() 
{
	//CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	m_pCAbout->BmpBlt( TRUE, this, 0, 0, 
	m_pCAbout->m_iWidth, m_pCAbout->m_iHeight,
	0,0);
	// Do not call CDialog::OnPaint() for painting messages
}

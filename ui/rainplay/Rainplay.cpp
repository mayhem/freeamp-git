// Rainplay.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Rainplay.h"
#include "RainplayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRainplayApp

BEGIN_MESSAGE_MAP(CRainplayApp, CWinApp)
	//{{AFX_MSG_MAP(CRainplayApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRainplayApp construction

CRainplayApp::CRainplayApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRainplayApp object

CRainplayApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CRainplayApp initialization

BOOL CRainplayApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	return TRUE;
}
// Rainplay.h : main header file for the RAINPLAY application
//

#if !defined(AFX_RAINPLAY_H__FE778D26_2E43_11D2_B244_B8AECE919F11__INCLUDED_)
#define AFX_RAINPLAY_H__FE778D26_2E43_11D2_B244_B8AECE919F11__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRainplayApp:
// See Rainplay.cpp for the implementation of this class
//

class CRainplayApp : public CWinApp
{
public:
	CRainplayApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRainplayApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRainplayApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAINPLAY_H__FE778D26_2E43_11D2_B244_B8AECE919F11__INCLUDED_)

// optionsetupdlg.cpp : implementation file
//

#include "config.h"
#include "stdafx.h"
#include "..\rainplay.h"
#include "optionsetupdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionSetupDlg dialog


COptionSetupDlg::COptionSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionSetupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COptionSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionSetupDlg)
	DDX_Control(pDX, IDC_SEL_UI, m_plugins);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionSetupDlg, CDialog)
	//{{AFX_MSG_MAP(COptionSetupDlg)
	ON_BN_CLICKED(IDC_REGISTER_FILES, OnRegisterFiles)
	ON_CBN_SELCHANGE(IDC_SEL_UI, OnSelchangeSelUi)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionSetupDlg message handlers

BOOL COptionSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	char lpTemp[MAX_PATH];
	GetModuleFileName(NULL, lpTemp, MAX_PATH );
	CString StartupDir = lpTemp;
	StartupDir = StartupDir.Left(StartupDir.ReverseFind(_T('\\'))+1);
	StartupDir += "plugins\\";

	CFileFind finder;
	BOOL bWorking = finder.FindFile(StartupDir+"*.ui");
	while (bWorking) {
		bWorking = finder.FindNextFile();
		m_plugins.InsertString(0, finder.GetFileName());
	}
	m_plugins.SetCurSel(m_plugins.FindString(-1, "rainplay.ui"));
	m_plugins.GetLBText(m_plugins.GetCurSel(),m_szDefaultUI);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionSetupDlg::OnRegisterFiles() 
{
	// TODO: Add your control notification handler code here
	if (RegisterMPxFile(".mp3", "MPEG File", BRANDING " MPEG File"))
		AfxMessageBox(BRANDING " had been set as the default player of .mp3");
	if (RegisterMPxFile(".m3u", "Playlist File", "MPEG Playlist file"))
		AfxMessageBox(BRANDING " had been set as the default player of .m3u");
}

BOOL COptionSetupDlg::RegisterMPxFile(CString fileext, CString filetype, CString descri)
{
	const HKEY  kMainKey = HKEY_CLASSES_ROOT;
	char kiconKeyValue[MAX_PATH+3];
	char kcommandKeyValue[MAX_PATH+3];
	GetModuleFileName(NULL, kiconKeyValue, MAX_PATH );
	strcpy(kcommandKeyValue, kiconKeyValue);
	strcat(kiconKeyValue, ", 1");
	strcat(kcommandKeyValue, " %1");

	CString szTemp, szTemp1;
	szTemp = filetype + "\\DefaultIcon";
	szTemp1 = filetype + "\\shell\\open\\command";
	//HKEY_CLASSES_ROOT\.mp3
	//HKEY_CLASSES_ROOT\MPEG File
	//HKEY_CLASSES_ROOT\MPEG File\DefaultIcon
	//HKEY_CLASSES_ROOT\MPEG File\shell\open\command
	return (
	WriteStrRegister(HKEY_CLASSES_ROOT,LPCTSTR(fileext),LPCTSTR(filetype)) &&
	WriteStrRegister(HKEY_CLASSES_ROOT,LPCTSTR(filetype),LPCTSTR(descri)) &&
	WriteStrRegister(HKEY_CLASSES_ROOT,LPCTSTR(szTemp),kiconKeyValue) &&
	WriteStrRegister(HKEY_CLASSES_ROOT,LPCTSTR(szTemp1),kcommandKeyValue) );
}

BOOL COptionSetupDlg::WriteStrRegister(HKEY kMainKey, CString szSubKey, CString szValue)
{
	HKEY key;
	DWORD disposition;
	CString szTemp;
	if (RegCreateKeyEx(kMainKey, LPCTSTR(szSubKey), NULL, "",
						REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,
						NULL, &key, &disposition)!=ERROR_SUCCESS) {
		szTemp = "Error on create register key \"" + szSubKey + "\"" + ".";
		AfxMessageBox(szTemp);
		return FALSE;
	}
	if (RegSetValueEx(key, "", NULL, REG_SZ,
					(LPBYTE)LPCTSTR(szValue), szValue.GetLength()+1)!=ERROR_SUCCESS) {
		szTemp = "Error on set value \"" + szValue + "\"" + "to subkey \"" + szSubKey + "\"" + ".";
		AfxMessageBox(szTemp);
		return FALSE;
	}
	RegCloseKey(key);
	return TRUE;
}

void COptionSetupDlg::OnSelchangeSelUi() 
{
	// TODO: Add your control notification handler code here
	m_plugins.GetLBText(m_plugins.GetCurSel(),m_szDefaultUI);
	AfxMessageBox("This will take results at next time you startup the freeamp");
}

// PlaylistDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rainplay.h"
#include "PlaylistDlg.h"
#include "playlist.h"
#include "RainplayUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CRainplayUI *g_ui;

BOOL SavePlaylistToFile(CString szFile, PlaylistManager *plm)
{
	if (szFile.GetLength()==0 || !plm || plm->Total()==0)
		return FALSE;

	PlaylistItem *pli, *oldpli;
	FILE *fp;
	RepeatMode oldRepeatMode;

	if ((fp=fopen(LPCTSTR(szFile), "wt"))!=NULL) {
		//plm->DumpPlaylist();
		oldRepeatMode = plm->GetRepeat();
		plm->SetRepeat(REPEAT_NOT);
		oldpli = plm->GetCurrent();
		plm->SetFirst();
		while((pli=plm->GetCurrent())) {
			CString szTemp = pli->GetMediaInfo()->m_filename;
			fprintf(fp, "%s\n", pli->GetMediaInfo()->m_filename);
			plm->SetNext();
		};
		fclose(fp);

		//plm->SetCurrent(oldpli);
		while( (plm->GetCurrent())!= oldpli ) {
			plm->SetNext();
		};
		plm->SetRepeat(oldRepeatMode);
		return TRUE;
	} else {
		CString sz;
		sz.Format("Can't open %s for write", szFile);
		AfxMessageBox(sz);
	}
	return FALSE;
}
BOOL ReadPlaylistFromFile(CString szFile, PlaylistManager *plm)
{
	if (szFile.GetLength()==0 || !plm)
		return FALSE;
	FILE *fp;
	CString szTemp('\0',MAX_PATH);

	if ((fp=fopen(LPCTSTR(szFile), "rt"))!=NULL) {
		plm->RemoveAll();
		while(fscanf(fp,"%s",szTemp)!=EOF) {
			if (!szTemp.IsEmpty()) {
				if (szTemp.Find(":\\")<0)
					szTemp = szFile.Left(szFile.ReverseFind(_T('\\'))+1) + szTemp;
				plm->Add((char *)LPCTSTR(szTemp),0);
			}
		}
		fclose(fp);
		plm->SetFirst();
		return TRUE;
	} else {
		CString sz;
		sz.Format("Can't read songs from %s.",szFile);
		AfxMessageBox(sz);
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CPlaylistDlg dialog


CPlaylistDlg::CPlaylistDlg(BOOL* bActive, CWnd* pParent /*=NULL*/)
	:	m_pbActive(bActive),
		CDialog(CPlaylistDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlaylistDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPlaylistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlaylistDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlaylistDlg, CDialog)
	//{{AFX_MSG_MAP(CPlaylistDlg)
	ON_COMMAND(IDM_PLAYLIST_CLOSE, OnPlaylistClose)
	ON_COMMAND(IDM_PLAYLIST_LOAD, OnPlaylistLoad)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlaylistDlg message handlers

BOOL CPlaylistDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlaylistDlg::OnPlaylistClose() 
{
	// TODO: Add your command handler code here
	DestroyWindow();
}

void CPlaylistDlg::OnPlaylistLoad() 
{
	// TODO: Add your command handler code here
	CFileDialog *dialog;
	dialog = new CFileDialog(TRUE, 
				"m3u",
				"",
				OFN_FILEMUSTEXIST    | 
				OFN_HIDEREADONLY     |
				OFN_EXPLORER,
				"playlist file (*.m3u)|*.m3u|");

	//´ò¿ªFile open dialog
	if (dialog->DoModal()==IDOK) {
		CString szTemp = dialog->GetPathName();
		ReadPlaylistFromFile(dialog->GetPathName(), g_ui->m_plm);
		if(g_ui->m_state == STATE_Playing ||
		g_ui->m_state == STATE_Paused) {
			g_ui->m_target->AcceptEvent(new Event(CMD_Stop));
			g_ui->m_target->AcceptEvent(new Event(CMD_Play));
		}

	}
	delete dialog;
}


void CPlaylistDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	*m_pbActive = FALSE;
	CDialog::OnClose();
	delete this;
}
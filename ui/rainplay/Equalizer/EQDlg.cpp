// EQDlg.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "rainplay.h"
#include "EQDlg.h"
#include "RainplayUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//+-12dB = 24dB total
#define	LOWER_LIMIT_DB	-10
#define	UPPER_LIMIT_DB	10
#define	NUM_TICKS		10
#define	LIMIT_DB_RATIO	3
#define	LOWER_LIMIT		LOWER_LIMIT_DB*LIMIT_DB_RATIO
#define	UPPER_LIMIT		UPPER_LIMIT_DB*LIMIT_DB_RATIO
#define	TOTAL_LIMIT		(UPPER_LIMIT-(LOWER_LIMIT))

#define	_EQUALIZER_ENABLE_
#ifdef	_EQUALIZER_ENABLE_
extern CRainplayUI *g_ui;
#endif	//_EQUALIZER_ENABLE_
#undef	_EQUALIZER_ENABLE_
/////////////////////////////////////////////////////////////////////////////
// CEQDlg dialog


CEQDlg::CEQDlg(BOOL* bActive, CWnd* pParent /*=NULL*/)
	:	m_pbActive(bActive),
		CDialog(CEQDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEQDlg)
	//}}AFX_DATA_INIT
#define	__EQUALIZER_ENABLE__
#ifdef	__EQUALIZER_ENABLE__
	for(int i=0; i<32; i++)
		equalizer[i] = 1.0;
#endif	//__EQUALIZER_ENABLE__
#undef	__EQUALIZER_ENABLE__
}


void CEQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEQDlg)
	DDX_Control(pDX, IDC_EQ0, m_eq0);
	DDX_Control(pDX, IDC_EQ1, m_eq1);
	DDX_Control(pDX, IDC_EQ2, m_eq2);
	DDX_Control(pDX, IDC_EQ3, m_eq3);
	DDX_Control(pDX, IDC_EQ4, m_eq4);
	DDX_Control(pDX, IDC_EQ5, m_eq5);
	DDX_Control(pDX, IDC_EQ6, m_eq6);
	DDX_Control(pDX, IDC_EQ7, m_eq7);
	DDX_Control(pDX, IDC_EQ8, m_eq8);
	DDX_Control(pDX, IDC_EQ9, m_eq9);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEQDlg, CDialog)
	//{{AFX_MSG_MAP(CEQDlg)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_EQZERO, OnEqzero)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_ENABLEEQ, OnEnableEQ)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEQDlg message handlers

BOOL CEQDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//hack code, just suit for VC6's Class wizard
	m_eq0.SetRange(LOWER_LIMIT, UPPER_LIMIT);
	m_eq0.SetTicFreq(TOTAL_LIMIT/ NUM_TICKS);
	m_eq0.SetPos(0);

	m_eq1.SetRange(LOWER_LIMIT, UPPER_LIMIT);
	m_eq1.SetTicFreq(TOTAL_LIMIT/ NUM_TICKS);
	m_eq1.SetPos(0);

	m_eq2.SetRange(LOWER_LIMIT, UPPER_LIMIT);
	m_eq2.SetTicFreq(TOTAL_LIMIT/ NUM_TICKS);
	m_eq2.SetPos(0);

	m_eq3.SetRange(LOWER_LIMIT, UPPER_LIMIT);
	m_eq3.SetTicFreq(TOTAL_LIMIT/ NUM_TICKS);
	m_eq3.SetPos(0);

	m_eq4.SetRange(LOWER_LIMIT, UPPER_LIMIT);
	m_eq4.SetTicFreq(TOTAL_LIMIT/ NUM_TICKS);
	m_eq4.SetPos(0);

	m_eq5.SetRange(LOWER_LIMIT, UPPER_LIMIT);
	m_eq5.SetTicFreq(TOTAL_LIMIT/ NUM_TICKS);
	m_eq5.SetPos(0);

	m_eq6.SetRange(LOWER_LIMIT, UPPER_LIMIT);
	m_eq6.SetTicFreq(TOTAL_LIMIT/ NUM_TICKS);
	m_eq6.SetPos(0);

	m_eq7.SetRange(LOWER_LIMIT, UPPER_LIMIT);
	m_eq7.SetTicFreq(TOTAL_LIMIT/ NUM_TICKS);
	m_eq7.SetPos(0);

	m_eq8.SetRange(LOWER_LIMIT, UPPER_LIMIT);
	m_eq8.SetTicFreq(TOTAL_LIMIT/ NUM_TICKS);
	m_eq8.SetPos(0);

	m_eq9.SetRange(LOWER_LIMIT, UPPER_LIMIT);
	m_eq9.SetTicFreq(TOTAL_LIMIT/ NUM_TICKS);
	m_eq9.SetPos(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEQDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	//                         power
	//power gain(db) = 10 * log 
	//                         10
	//
	//						     voltage
	//voltage gain(db) = 20 * log
	//                           10
	//1,1,1,1,1,1,2,4,8,12
	switch(pScrollBar->GetDlgCtrlID()) {
	case IDC_EQ0:
		equalizer[0] = (float)pow(10,(double)(0-m_eq0.GetPos()/LIMIT_DB_RATIO)/10);
		break;
	case IDC_EQ1:
		equalizer[1] = (float)pow(10,(double)(0-m_eq1.GetPos()/LIMIT_DB_RATIO)/10);
		break;
	case IDC_EQ2:
		equalizer[2] = (float)pow(10,(double)(0-m_eq2.GetPos()/LIMIT_DB_RATIO)/10);
		break;
	case IDC_EQ3:
		equalizer[3] = (float)pow(10,(double)(0-m_eq3.GetPos()/LIMIT_DB_RATIO)/10);
		break;
	case IDC_EQ4:
		equalizer[4] = (float)pow(10,(double)(0-m_eq4.GetPos()/LIMIT_DB_RATIO)/10);
		break;
	case IDC_EQ5:
		equalizer[5] = (float)pow(10,(double)(0-m_eq5.GetPos()/LIMIT_DB_RATIO)/10);
		break;
	case IDC_EQ6:
		equalizer[6] = (float)pow(10,(double)(0-m_eq6.GetPos()/LIMIT_DB_RATIO)/10);
		equalizer[7] = equalizer[6];
		break;
	case IDC_EQ7:
		equalizer[8] = (float)pow(10,(double)(0-m_eq7.GetPos()/LIMIT_DB_RATIO)/10);
		equalizer[9] = equalizer[8];
		equalizer[10] = equalizer[8];
		equalizer[11] = equalizer[8];
		break;
	case IDC_EQ8:
		equalizer[12] = (float)pow(10,(double)(0-m_eq8.GetPos()/LIMIT_DB_RATIO)/10);
		equalizer[13] = equalizer[12];
		equalizer[14] = equalizer[12];
		equalizer[15] = equalizer[12];
		equalizer[16] = equalizer[12];
		equalizer[17] = equalizer[12];
		equalizer[18] = equalizer[12];
		equalizer[19] = equalizer[12];
		break;
	case IDC_EQ9:
		equalizer[20] = (float)pow(10,(double)(0-m_eq9.GetPos()/LIMIT_DB_RATIO)/10);
		equalizer[21] = equalizer[20];
		equalizer[22] = equalizer[20];
		equalizer[23] = equalizer[20];
		equalizer[24] = equalizer[20];
		equalizer[25] = equalizer[20];
		equalizer[26] = equalizer[20];
		equalizer[27] = equalizer[20];
		equalizer[28] = equalizer[20];
		equalizer[29] = equalizer[20];
		equalizer[30] = equalizer[20];
		equalizer[31] = equalizer[20];

		break;
	default:
		break;
	}
#define	__EQUALIZER_ENABLE__
#ifdef	__EQUALIZER_ENABLE__
	g_ui->m_target->AcceptEvent(new SetEqualizerDataEvent((float *)&equalizer));
#endif	//__EQUALIZER_ENABLE__
#undef	__EQUALIZER_ENABLE__
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CEQDlg::OnEqzero() 
{
	// TODO: Add your control notification handler code here
	for(int i=0; i<32; i++)
		equalizer[i] = 1.0;
	m_eq0.SetPos(0);
	m_eq1.SetPos(0);
	m_eq2.SetPos(0);
	m_eq3.SetPos(0);
	m_eq4.SetPos(0);
	m_eq5.SetPos(0);
	m_eq6.SetPos(0);
	m_eq7.SetPos(0);
	m_eq8.SetPos(0);
	m_eq9.SetPos(0);
#define	__EQUALIZER_ENABLE__
#ifdef	__EQUALIZER_ENABLE__
	g_ui->m_target->AcceptEvent(new SetEqualizerDataEvent((float *)&equalizer));
#endif	//__EQUALIZER_ENABLE__
#undef	__EQUALIZER_ENABLE__
}

void CEQDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	*m_pbActive = FALSE;
	CDialog::OnClose();
	delete this;
}

void CEQDlg::OnEnableEQ() 
{
	// TODO: Add your control notification handler code here
#define	__EQUALIZER_ENABLE__
#ifdef	__EQUALIZER_ENABLE__
	CButton *enable = (CButton *)GetDlgItem(IDC_ENABLEEQ);
	g_ui->m_target->AcceptEvent(new SetEqualizerDataEvent(enable->GetCheck() ? 1: 0));
#endif	//__EQUALIZER_ENABLE__
#undef	__EQUALIZER_ENABLE__
	
}

#include "stdafx.h"

#include "config.h"
#include "thread.h"
#include "event.h"
#include "eventdata.h"
#include "Rainplay.h"
#include "RainplayUI.h"

extern "C" CRainplayUI *Initialize()
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	return new CRainplayUI();
}

CRainplayUI::
CRainplayUI():
UserInterface()
{
	//m_uiSemaphore = new Semaphore();

	m_uiThread = Thread::CreateThread();
	m_uiThread->Create(UIThreadFunc,this);
	//m_uiSemaphore->Wait();
}

CRainplayUI::
~CRainplayUI()
{
	delete m_Dlg;
	//delete m_uiSemaphore;
}

void 
CRainplayUI::
SetPlayListManager(PlayListManager *plm) {
	m_plm = plm;
}
void 
CRainplayUI::
SetTarget(EventQueue* eq)
{
    m_target = eq;
}

int32 
CRainplayUI::
AcceptEvent(Event* event)
{
    int32 result = 255;

    if (event) 
    {
        
        switch (event->Type()) 
        {
            case INFO_Playing: 
	            break; 

            case INFO_Paused: 
	            break; 

            case INFO_Stopped: 
	            break; 

			case INFO_MPEGInfo:
				break;

			case INFO_MediaInfo:
				break;

			case INFO_MediaTimeInfo:
				break;

			case INFO_DoneOutputting:
				break;

			case INFO_PlayListDonePlay:
				break;

			case CMD_Cleanup:
				m_target->AcceptEvent(new Event(INFO_ReadyToDieUI));
				break;

			default:
	            break;
	    }

	    result = 0;

    } 

    return result;
}

void  
CRainplayUI::
SetArgs(int32 argc, char** argv)
{
}

void
CRainplayUI::
CreateUI()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Dlg = new CRainplayDlg;
	m_Dlg->DoModal();
	
	//AfxMessageBox("Use rbutton to select menu\n");
	m_target->AcceptEvent(new Event(CMD_QuitPlayer));
}

void 
CRainplayUI::
UIThreadFunc(void* arg)
{
    CRainplayUI* ui = (CRainplayUI*)arg;

    ui->CreateUI();
}
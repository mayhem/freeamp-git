#include "stdafx.h"

#include "math.h"			//for ceil funtcion
#include "config.h"
#include "thread.h"
#include "event.h"
#include "eventdata.h"
#include "Rainplay.h"
#include "RainplayUI.h"

CRainplayUI *g_ui;

extern BOOL ReadPlaylistFromFile(CString szFile, PlayListManager *plm);

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
	m_uiSemaphore = new Semaphore();

	g_ui = this;
	m_plm = NULL;
	m_state = STATE_Stopped;
	m_uiThread = Thread::CreateThread();
	m_uiThread->Create(UIThreadFunc,this);
	m_uiSemaphore->Wait();
}

CRainplayUI::
~CRainplayUI()
{
	delete m_Dlg;
	delete m_uiSemaphore;
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
		    case INFO_PlayListRepeat:
				break;

		    case INFO_PlayListShuffle:
				break;

            case INFO_Playing:
				//((Player *)(g_ui->m_target))->m_lmc->m_decoderThread->SetPriority(High);
				m_state = STATE_Playing;
	            break; 

            case INFO_Paused:
				m_state = STATE_Paused;
	            break; 

            case INFO_Stopped:
				m_state = STATE_Stopped;
	            break; 

			case INFO_MPEGInfo:
				{
					MpegInfoEvent *info = (MpegInfoEvent *)event;
					int32 bitrate = info->GetBitRate()/1000;
					unsigned short sample = (unsigned short)(info->GetSampleRate());
					m_Dlg->NotifyMPEGInfo(bitrate,sample,info->GetTotalFrames());
				}
				break;

			case INFO_ID3TagInfo:
				{
					ID3TagEvent *info = (ID3TagEvent *)event;
					CString szTemp;
					if (info->GetId3Tag().m_containsInfo) {
						szTemp = info->GetId3Tag().m_artist;
						szTemp.TrimRight();
						szTemp += " - ";
						szTemp += info->GetId3Tag().m_songName;
						szTemp.TrimRight();
					} else
						szTemp = g_ui->m_plm->GetCurrent()->GetMediaInfo()->m_filename;
					m_Dlg->NotifySongName(szTemp);
				}
				break;

            case INFO_MediaInfo:
				{
					MediaInfoEvent *info = (MediaInfoEvent*)event;
					int32 seconds = (int32)ceil(info->m_totalSeconds);
					m_Dlg->NotifyTotalTime(seconds);
					m_Dlg->NotifyNumSongs(info->m_indexOfSong,info->m_totalSongs);
				}
				break; 

            case INFO_MediaTimeInfo:
				{
					MediaTimeInfoEvent* info = (MediaTimeInfoEvent*)event;
					int32 iTimerValue = (int32)ceil(info->m_totalSeconds);
					m_Dlg->NotifyTime(iTimerValue);
				}
				break; 
            
            case INFO_DoneOutputting:
	            break; 

            case INFO_PlayListDonePlay:
                break;

#define	_VISUAL_ENABLE_
#ifdef	_VISUAL_ENABLE_
			case CMD_SendVisBuf:
				{
					SendVisBufEvent* visbuf = (SendVisBufEvent*)event;
					m_Dlg->NotifyVisDataReady(visbuf->GetLength(), visbuf->GetBuf());
				}
				break;
#endif	//_VISUAL_ENABLE_
#undef	_VISUAL_ENABLE_

	        case CMD_Cleanup:
				{
			      m_target->AcceptEvent(new Event(INFO_ReadyToDieUI));
				}
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
    char *arg = NULL;
    bool shuffle = false;
    bool autoplay = false;
    int32 count = 0;

    for(int32 i = 1;i < argc; i++) 
    {
	    arg = argv[i];

	    if (arg[0] == '-') 
        {
	        switch (arg[1]) 
            {
		        case 's':
                {
                    shuffle = true;
		            break;
	            } 

                case 'p':
                {
                    autoplay = true;
		            break;
	            } 
            }
        }
        else 
        {
			CString szTemp = arg;
			if (szTemp.Right(3)=="M3U" ||
				szTemp.Right(3)=="m3u" ) {
				ReadPlaylistFromFile(szTemp, m_plm);
			} else
				m_plm->AddItem(arg,0);
            count++;
	    }
    }

    m_plm->SetFirst();

    if(shuffle) 
        m_plm->SetShuffle(SHUFFLE_RANDOM);
    
    //if(autoplay)
       //m_target->AcceptEvent(m_target, new Event(CMD_Play));
}

void
CRainplayUI::
CreateUI()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Dlg = new CRainplayDlg;
	m_Dlg->DoModal();

	g_ui->m_target->AcceptEvent(new Event(CMD_QuitPlayer));
}

void 
CRainplayUI::
UIThreadFunc(void* arg)
{
	CRainplayUI* ui = (CRainplayUI*)arg;

	ui->CreateUI();
}
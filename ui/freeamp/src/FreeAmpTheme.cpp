/*____________________________________________________________________________
        
   FreeAmp - The Free MP3 Player

   Copyright (C) 1999 EMusic.com
   Portions Copyright (C) 1999 Valters Vingolds

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
        
   $Id$
____________________________________________________________________________*/

#include <stdio.h> 
#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#include <unistd.h>
#define _stat stat
#define _S_IFDIR S_IFDIR
#endif
#include "config.h"
#include "downloadmanager.h"

#ifdef HAVE_GTK
#include "GTKUtility.h"
#include "GTKPreferenceWindow.h"
#elif defined(WIN32)
#include "Win32Window.h"
#include "Win32PreferenceWindow.h"
#include "win32updatemanager.h"
#include "resource.h"
extern HINSTANCE g_hinst;
#elif defined(__BEOS__)
#include "BeOSPreferenceWindow.h"
#include "win32impl.h"
#endif

#include "FreeAmpTheme.h"
#include "MessageDialog.h"
#include "eventdata.h"
#include "event.h"
#include "debug.h"
#include "PreferenceWindow.h"
#include "playlist.h"
#include "player.h"
#include "help.h"
#include "properties.h"

void WorkerThreadStart(void* arg);

#define DB Debug_v("%s:%d\n", __FILE__, __LINE__);

const char *szWelcomeMsg = "Welcome to "the_BRANDING"!";
const char *szParseError = "Parsing the Theme description failed. Cause: ";
const char *szCantFindHelpError = "Cannot find the help files. Please make "
                                  "sure that the help files are properly "
                                  "installed, and you are not running "
                                  the_BRANDING" from the build directory.";
const char *szKeepThemeMessage = "Would you like to keep this theme?";
const int iVolumeChangeIncrement = 10;

struct OptionsArgs
{
   FreeAmpTheme *pThis;
   uint32        uDefaultPage;
}; 

extern    "C"
{
   UserInterface *Initialize(FAContext * context)
   {
      //Debug_v("##Clear");
      return new FreeAmpTheme(context);
   }
}

FreeAmpTheme::FreeAmpTheme(FAContext * context)
             :Theme(context)
{
   char   szTemp[255];
   uint32 iLen = 255;
   
   m_pContext = context;
   m_iCurrentSeconds = 0;
   m_iTotalSeconds = -1;
   m_iSeekSeconds = 0;
   m_bSeekInProgress = false;
   m_iVolume = -1;
   m_iSeekPos = -1;
   m_bPlayShown = true;
   m_oTitle = string("");
   m_eTimeDisplayState = kNormal;
   m_oStreamInfo = string("");
   m_pUpdateMan = NULL;
   m_pUpdateThread = NULL;
   m_pOptionsThread = NULL;
   m_bInOptions = false;
   m_bPaused = false;

#if defined( WIN32 )
    m_pUpdateMan = new Win32UpdateManager(m_pContext);
    m_pUpdateMan->DetermineLocalVersions();
    m_pUpdateMan->SetPlatform(string("WIN32"));
#if defined( _M_ALPHA )
     m_pUpdateMan->SetArchitecture(string("ALPHA"));
#else
     m_pUpdateMan->SetArchitecture(string("X86"));
#endif // _M_ALPHA

#endif // WIN32

   m_pContext->prefs->GetPrefString(kWindowModePref, szTemp, &iLen);
   if (iLen > 0)
      m_oCurrentWindow = string(szTemp);
   else   
      m_oCurrentWindow = string("MainWindow");

   LoadFreeAmpTheme();
}

FreeAmpTheme::~FreeAmpTheme()
{
    if (m_pOptionsThread)
    {  
        delete m_pOptionsThread;
        m_pOptionsThread = NULL;
    }
#if defined( WIN32 )
    delete m_pUpdateMan;
#endif // WIN32
}

Error FreeAmpTheme::Init(int32 startup_type)
{
    assert(this);

    m_iStartupType = startup_type;

    m_uiThread = Thread::CreateThread();
    m_uiThread->Create(WorkerThreadStart, this);

    return kError_NoErr;
}

void FreeAmpTheme::WorkerThread(void)
{
    char   szTemp[255];
    uint32 iLen = 255;
    Error  eRet;
    int32  iValue;

    m_pContext->prefs->GetTimeDisplay(&iValue);
    if (iValue)
       m_eTimeDisplayState = kTimeRemaining;

    m_pContext->prefs->GetPrefString(kMainWindowPosPref, szTemp, &iLen);
    sscanf(szTemp, " %d , %d", &m_oWindowPos.x, &m_oWindowPos.y);

#ifdef WIN32

    bool checkForUpdates = false;

    m_pContext->prefs->GetCheckForUpdates(&checkForUpdates);

    if(checkForUpdates)
    {
        m_pUpdateThread = Thread::CreateThread();
        m_pUpdateThread->Create(update_thread, this);
    }

#endif

    eRet = Theme::Run(m_oWindowPos);
    if (!IsError(eRet))
    {
       sprintf(szTemp, "%d,%d", m_oWindowPos.x, m_oWindowPos.y);
       m_pContext->prefs->SetPrefString(kMainWindowPosPref, szTemp);
       m_pContext->prefs->SetPrefString(kWindowModePref, m_oCurrentWindow.c_str());
    }
    else     
       m_pContext->target->AcceptEvent(new Event(CMD_QuitPlayer));
   
    m_pContext->prefs->SetTimeDisplay(m_eTimeDisplayState == kTimeRemaining);
}

void WorkerThreadStart(void* arg)
{
    FreeAmpTheme* ui = (FreeAmpTheme*)arg;
    ui->WorkerThread();
}

void FreeAmpTheme::LoadFreeAmpTheme(void)
{
   char          szTemp[255];
   uint32        iLen = 255;
   string        oThemePath;
   Error         eRet;

   m_pContext->prefs->GetPrefString(kThemePathPref, szTemp, &iLen);
   oThemePath = szTemp;
   
   if (strchr(szTemp, DIR_MARKER) == NULL)
   {
       string oBase = oThemePath;
       map<string, string> oThemeList;
 
       m_pThemeMan->GetThemeList(oThemeList);

       char *dot;
       if ((dot = strchr(oBase.c_str(), '.')))
           dot = '\0';

       oThemePath = oThemeList[oBase]; 
   }
  
   iLen = 255; 
   m_pContext->prefs->GetPrefString(kThemeDefaultFontPref, szTemp, &iLen);
   SetDefaultFont(string(szTemp));

   eRet = LoadTheme(oThemePath, m_oCurrentWindow);
   if (IsError(eRet) && eRet != kError_InvalidParam)					   
   {
       MessageDialog oBox(m_pContext);
       string        oErr, oMessage(szParseError);

       GetErrorString(oErr);
       oMessage += oErr;
       oBox.Show(oMessage.c_str(), string(BRANDING), kMessageOk);
       m_pContext->target->AcceptEvent(new Event(CMD_QuitPlayer));
   }
   if (eRet == kError_InvalidParam)
       m_pContext->target->AcceptEvent(new Event(CMD_QuitPlayer));
}


Error FreeAmpTheme::Close(void)
{
    Theme::Close();
    
    if (!m_uiThread)
       return kError_NoErr;
    
    m_uiThread->Join();
    delete m_uiThread;
    m_uiThread = NULL;

    return kError_NoErr;
}

int32 FreeAmpTheme::AcceptEvent(Event * e)
{
   switch (e->Type())
   {
      case INFO_ErrorMessage:
      {
         MessageDialog      oBox(m_pContext);
         ErrorMessageEvent *pEvent = (ErrorMessageEvent *)e;
         
         string oDesc(pEvent->GetErrorMessage());
  
         oBox.Show(oDesc.c_str(), string(BRANDING), kMessageOk);
         break;
      }
      case INFO_StatusMessage:
      {
         StatusMessageEvent *pEvent = (StatusMessageEvent *)e;
         
         string oDesc(pEvent->GetStatusMessage());
         m_pWindow->ControlStringValue("Info", true, oDesc);
         break;
      }
      case CMD_Cleanup:
      {
         string oName("MainWindow");
      	 Close();
         m_pContext->target->AcceptEvent(new Event(INFO_ReadyToDieUI));
         
         break;
      }   
      case INFO_Playing:
      {
         bool bEnable;
         int  iState;
         
         iState = 1;
         m_pWindow->ControlIntValue(string("PlayPause"), true, iState);
         iState = 1;
         m_pWindow->ControlIntValue(string("PlayStop"), true, iState);
         iState = 0;
         m_pWindow->ControlIntValue(string("MPause"), true, iState);
         bEnable = false;
         m_pWindow->ControlEnable(string("Play"), true, bEnable);
         bEnable = true;
         m_pWindow->ControlEnable(string("Pause"), true, bEnable);
         bEnable = true;
         m_pWindow->ControlEnable(string("Stop"), true, bEnable);
         
         m_bPlayShown = false;
         m_bPaused = false;
         
         break;
      }   
      case INFO_Paused:
      case INFO_Stopped:
      {
         int iState = 0;
         bool bEnable = true;
         string oEmpty("");
         
         iState = 0;
         m_pWindow->ControlIntValue(string("PlayPause"), true, iState);
         m_bPaused = e->Type() == INFO_Paused;
         iState = e->Type() == INFO_Paused ? 1 : 0;
         m_pWindow->ControlIntValue(string("PlayStop"), true, iState);
         iState = e->Type() == INFO_Paused ? 1 : 0;
         m_pWindow->ControlIntValue(string("MPause"), true, iState);
         bEnable = true;
         m_pWindow->ControlEnable(string("Play"), true, bEnable);
         
         
         bEnable = false;
         m_pWindow->ControlEnable(string("Pause"), true, bEnable);
         bEnable = false;
         m_pWindow->ControlEnable(string("Stop"), true, bEnable);
         m_bPlayShown = true;
         m_pWindow->ControlStringValue("BufferInfo", true, oEmpty);
         m_oStreamInfo = "";
         m_pWindow->ControlStringValue("StreamInfo", true, oEmpty);
         
         if (e->Type() == INFO_Stopped)
         {
            m_iSeekPos = 0;
            m_iTotalSeconds = -1;
            m_iCurrentSeconds = -1;
            m_pWindow->ControlIntValue(string("Seek"), true, m_iSeekPos);
            UpdateTimeDisplay(m_iCurrentSeconds);
            
            if ((int32)m_pContext->plm->GetCurrentIndex() < 0)
            {
                m_oTitle = szWelcomeMsg;
                
                m_pWindow->ControlStringValue("Title", true, m_oTitle);
                
                string title = BRANDING;
                m_pWindow->SetTitle(title);
            }    
         }
         
         break;
      }   
      case INFO_DoneOutputting:
      {
         m_iSeekPos = 0;
         m_iTotalSeconds = -1;
         m_iCurrentSeconds = -1;
         m_pWindow->ControlIntValue(string("Seek"), true, m_iSeekPos);
         UpdateTimeDisplay(m_iCurrentSeconds);
         
         break;
      }   
      case INFO_MediaInfo:
      {
         MediaInfoEvent *info = (MediaInfoEvent *) e;
         string oName("Title"), oText;
         char  *pFoo = strrchr(info->m_filename, '/');
         bool   bSet = true;

         if (m_oTitle.length() == 0)
         {
             pFoo = (pFoo ? ++pFoo : info->m_filename);
             m_oTitle = string(pFoo);
             m_pWindow->ControlStringValue(oName, true, m_oTitle);
             oText = string(BRANDING": ") + string(pFoo);
             m_pWindow->SetTitle(oText);
         }

		 // Enable/disable the seek slider
         m_iTotalSeconds = (int32) info->m_totalSeconds;
         bSet = (m_iTotalSeconds < 0) ? false : true;
         oName = string("Seek");
         m_pWindow->ControlEnable(oName, true, bSet);

         m_pContext->target->AcceptEvent(new Event(CMD_GetVolume));

         bSet = true;
         m_pWindow->ControlEnable(string("Volume"), true, bSet);
         m_pWindow->ControlEnable(string("VolumePlus"), true, bSet);
         m_pWindow->ControlEnable(string("VolumeMinus"), true, bSet);

         break;
      }
 
      case INFO_PlaylistItemUpdated :
      {
         int                       i;
         PlaylistItemUpdatedEvent *pInfo = 
            (PlaylistItemUpdatedEvent *)e;

         i = m_pContext->plm->GetCurrentIndex();
         if (i >= 0)
         {
             if (m_pContext->plm->ItemAt(i) == pInfo->Item())
                UpdateMetaData(pInfo->Item());
         }       
         break;
      }
      case INFO_PlaylistCurrentItemInfo:
      {
         PlaylistCurrentItemInfoEvent *pInfo = 
            (PlaylistCurrentItemInfoEvent *)e;
 
         if (m_pContext->plm->GetCurrentIndex() != kInvalidIndex)
             UpdateMetaData(pInfo->Item());
         break;
      }
      
      case INFO_StreamInfo:
      {
         char szTitle[100];
         string oName("Title"), oText;

         StreamInfoEvent *pInfo = (StreamInfoEvent *) e;

         pInfo->GetTitle(szTitle, 100);
         m_oTitle = string(szTitle);
         m_pWindow->ControlStringValue(oName, true, m_oTitle);
         oText = string(BRANDING": ") + string(szTitle);
         m_pWindow->SetTitle(oText);

         break;
      }
      case INFO_BufferStatus:
      {
         StreamBufferEvent *info = (StreamBufferEvent *) e;
         char szTemp[100];
         string oName("BufferInfo"), oText;

	 sprintf(szTemp, "I: %3ld O: %3ld %c", 
                 (long int)info->GetInputPercent(),
                 (long int)info->GetOutputPercent(),
                 info->IsBufferingUp() ? '^' : ' ');
         oText = string(szTemp);
         m_pWindow->ControlStringValue(oName, true, oText);

         break;
      }
         
      case INFO_MediaTimeInfo:
      {
         MediaTimeInfoEvent *info = (MediaTimeInfoEvent *) e;
         int                 iSeconds;
         string              oName("Time"), oText;

         iSeconds = (info->m_hours * 3600) + (info->m_minutes * 60) + 
                     info->m_seconds;
         if (iSeconds == m_iCurrentSeconds || m_bSeekInProgress)
             break;

         m_iCurrentSeconds = iSeconds;            
         UpdateTimeDisplay(m_iCurrentSeconds);

         if (m_iTotalSeconds > 0)
         {
            m_iSeekPos = (iSeconds * 100) / m_iTotalSeconds;
            oName = string("Seek");
            m_pWindow->ControlIntValue(oName, true, m_iSeekPos);
         }   

         break;
      }
      case INFO_MPEGInfo:
      {
         MpegInfoEvent *info = (MpegInfoEvent *) e;
         char           text[100];

         m_fSecondsPerFrame = info->GetSecondsPerFrame();
         if (info->GetBitRate() == 0)
              sprintf(text, "VBR %ldkhz %s", 
                   (long int)(info->GetSampleRate() / 1000), 
                   info->GetChannels() ? "Stereo" : "Mono");
         else
              sprintf(text, "%ldkbps %ldkhz %s", 
                   (long int)(info->GetBitRate() / 1000),
                   (long int)(info->GetSampleRate() / 1000), 
                   info->GetChannels() ? "Stereo" : "Mono");

         m_oStreamInfo = text;
         m_pWindow->ControlStringValue("StreamInfo", true, m_oStreamInfo);
      
         break;
      }

      case INFO_PlaylistRepeat:
      {
         PlaylistRepeatEvent *plre = (PlaylistRepeatEvent *) e;
         int iState;

         switch (plre->GetRepeatMode())
         {
             case kPlaylistMode_RepeatNone:
                iState = 0;
                break;
             case kPlaylistMode_RepeatOne:
                iState = 1;
                break;
             case kPlaylistMode_RepeatAll:
                iState = 2;
                break;
             default:
                break;
         }
         m_pWindow->ControlIntValue(string("Repeat"), true, iState);
         break;
      }
      case INFO_PlaylistShuffle:
      {
         PlaylistShuffleEvent *plse = (PlaylistShuffleEvent *) e;
         int iState;

         iState = plse->GetShuffleMode() ? 1 : 0;
         m_pWindow->ControlIntValue(string("Shuffle"), true, iState);
         break;
      }
      case INFO_VolumeInfo:
      {
         m_iVolume = ((VolumeEvent *) e)->GetVolume();
         m_pWindow->ControlIntValue(string("Volume"), true, m_iVolume);
         break;
      }

      case INFO_PrefsChanged:
      {
         bool bValue;

         ReloadTheme();

         m_pContext->prefs->GetStayOnTop(&bValue);
         m_pWindow->SetStayOnTop(bValue);
         m_pContext->prefs->GetLiveInTray(&bValue);
         m_pWindow->SetLiveInToolbar(bValue);
         
      	break;
      }
      case CMD_ShowPreferences:
      {
         ShowPreferencesEvent* prefsEvent = (ShowPreferencesEvent*)e;
      	 ShowOptions(prefsEvent->GetDefaultPage());
      	 break;
      }

      case CMD_LoadTheme:
      {
          char          szSavedTheme[_MAX_PATH], szNewTheme[_MAX_PATH];
          uint32        iLen = _MAX_PATH;
          string        oThemePath;
          MessageDialog oBox(m_pContext);
	      string        oMessage(szKeepThemeMessage);

          LoadThemeEvent *pInfo = (LoadThemeEvent *)e;
          URLToFilePath(pInfo->URL(), szNewTheme, &iLen);

          iLen = _MAX_PATH;
          m_pContext->prefs->GetPrefString(kThemePathPref, szSavedTheme, &iLen);

          if (strcmp(szSavedTheme, szNewTheme))
          {
              m_pContext->prefs->SetPrefString(kThemePathPref, szNewTheme);
              ReloadTheme();
          }
          else
              strcpy(szSavedTheme, pInfo->SavedTheme());
  
          if (oBox.Show(oMessage.c_str(), string(BRANDING), kMessageYesNo) == 
              kMessageReturnYes)
          {
              ThemeManager oMan(m_pContext);
              string       oThemePath(szNewTheme);
              
              if (!IsError(oMan.AddTheme(oThemePath)))
                  m_pContext->prefs->SetPrefString(kThemePathPref, oThemePath.c_str());
          }
          else
          {
              m_pContext->prefs->SetPrefString(kThemePathPref, szSavedTheme);
              ReloadTheme();
          }
          break;
      }
      
      default:
         break;
   }
   return 0;
}

Error FreeAmpTheme::HandleControlMessage(string &oControlName, 
                                         ControlMessageEnum eMesg)
{
   if (m_bInOptions)
   {
       m_pWindow->BringWindowToFront();
       return kError_NoErr;
   }    

   if (eMesg == CM_MouseEnter)
   {
       string oName("Info"), oDesc("");

       if (oControlName == string("Time") ||
           oControlName == string("TimeRemaining"))
       {
           if (m_eTimeDisplayState == kTimeRemaining && 
               m_pWindow->DoesControlExist("TimeRemaining"))
               m_pWindow->ControlGetDesc("TimeRemaining", oDesc);
           else
               m_pWindow->ControlGetDesc("Time", oDesc);
       }
       else
           m_pWindow->ControlGetDesc(oControlName, oDesc);

       m_pWindow->ControlStringValue(oName, true, oDesc);
                                
       return kError_NoErr;
   }    
   if (eMesg == CM_MouseLeave)
   {
       string oName("Info"), oEmpty("");
      
       m_pWindow->ControlStringValue(oName, true, oEmpty);
                                
       return kError_NoErr;
   }    
   if (oControlName == string("Minimize") && eMesg == CM_Pressed)
   {
       m_pWindow->Minimize();
       
       return kError_NoErr;
   }
   if (oControlName == string("Volume") && 
       (eMesg == CM_ValueChanged || eMesg == CM_SliderUpdate))
   {
       int iVol;
       
       m_pWindow->ControlIntValue(oControlName, false, iVol);
       SetVolume(iVol);
           
       return kError_NoErr;
   }    
   if (oControlName == string("VolumePlus") && eMesg == CM_Pressed)
   {
       int iVol;
       
       iVol = min(m_iVolume + iVolumeChangeIncrement, 100);
       SetVolume(iVol);
           
       return kError_NoErr;
   }    
   if (oControlName == string("VolumeMinus") && eMesg == CM_Pressed)
   {
       int iVol;
       
       iVol = max(m_iVolume - iVolumeChangeIncrement, 0);
       SetVolume(iVol);
           
       return kError_NoErr;
   }    
   
   if (oControlName == string("Seek") && eMesg == CM_ValueChanged)
   {
       string oName("Info"), oEmpty("");
       int    iValue, iFrame;
       
       m_pWindow->ControlIntValue(oControlName, false, iValue);
       
       iFrame = (int)(((float)iValue * (float)m_iTotalSeconds) / 
                      ((float)100 * m_fSecondsPerFrame));
       m_bSeekInProgress = false;
       
       m_pContext->target->AcceptEvent(new 
              VolumeEvent(CMD_ChangePosition, iFrame));
       
       m_pWindow->ControlStringValue(oName, true, oEmpty);
              
       return kError_NoErr;
   }    
   if (oControlName == string("Seek") && eMesg == CM_SliderUpdate)
   {
       string oName("Time"), oText("");
       int    iValue, iTime;
       
       m_pWindow->ControlIntValue(oControlName, false, iValue);
       
       iTime = (iValue * m_iTotalSeconds) / 100;
       UpdateTimeDisplay(iTime);
       m_bSeekInProgress = true;
              
       return kError_NoErr;
   }    
   if (oControlName == string("PlayPause") && eMesg == CM_Pressed)
   {
       int iState = 0;

       if (m_pContext->plm->CountItems() == 0)
       {
           m_pContext->target->AcceptEvent(new Event(CMD_ToggleMusicBrowserUI));
           return kError_NoErr;
       }
       m_pWindow->ControlIntValue(oControlName, false, iState);
       if (iState == 0)
           m_pContext->target->AcceptEvent(new Event(CMD_Play));
       else
           m_pContext->target->AcceptEvent(new Event(CMD_Pause));
       return kError_NoErr;
   }
   
   if (oControlName == string("PlayStop") && eMesg == CM_Pressed)
   {
       int iState = 0;

       if (m_pContext->plm->CountItems() == 0)
       {
           m_pContext->target->AcceptEvent(new Event(CMD_ToggleMusicBrowserUI));
           return kError_NoErr;
       }
       m_pWindow->ControlIntValue(oControlName, false, iState);
       if (iState > 1)
          iState = 1;
       
       if (iState == 0)
           m_pContext->target->AcceptEvent(new Event(CMD_Play));
	   else
           m_pContext->target->AcceptEvent(new Event(CMD_Stop));
       return kError_NoErr;
   }
   
   if (oControlName == string("Play") && eMesg == CM_Pressed)
   {
       if (m_pContext->plm->CountItems() == 0)
       {
           m_pContext->target->AcceptEvent(new Event(CMD_ToggleMusicBrowserUI));
           return kError_NoErr;
       }
       m_pContext->target->AcceptEvent(new Event(CMD_Play));
       return kError_NoErr;
   }
   if (oControlName == string("MPause") && eMesg == CM_Pressed)
   {
   	   int iState = 0;
       m_pWindow->ControlIntValue(oControlName, false, iState);
       if (iState == 0)
           m_pContext->target->AcceptEvent(new Event(CMD_Pause));
	   else
           m_pContext->target->AcceptEvent(new Event(CMD_Play));
       return kError_NoErr;
   }
   if (oControlName == string("Pause") && eMesg == CM_Pressed)
   {
       m_pContext->target->AcceptEvent(new Event(CMD_Pause));
       return kError_NoErr;
   }
   if (oControlName == string("Stop") && eMesg == CM_Pressed)
   {
       m_pContext->target->AcceptEvent(new Event(CMD_Stop));
       return kError_NoErr;
   }
   if (oControlName == string("Next") && eMesg == CM_Pressed)
   {
       m_pContext->target->AcceptEvent(new Event(CMD_NextMediaPiece));
       return kError_NoErr;
   }
   if (oControlName == string("Prev") && eMesg == CM_Pressed)
   {
       m_pContext->target->AcceptEvent(new Event(CMD_PrevMediaPiece));
       return kError_NoErr;
   }
   if (oControlName == string("MyMusic") && eMesg == CM_Pressed)
   {
       m_pContext->target->AcceptEvent(new Event(CMD_ToggleMusicBrowserUI));
       return kError_NoErr;
   }
   if (oControlName == string("Files") && eMesg == CM_Pressed)
   {
       m_pContext->target->AcceptEvent(new Event(CMD_AddFiles));
       return kError_NoErr;
   }
   if (oControlName == string("Download") && eMesg == CM_Pressed)
   {
       m_pContext->target->AcceptEvent(new Event(CMD_ToggleDownloadUI));
       return kError_NoErr;
   }
   if (oControlName == string("Options") && eMesg == CM_Pressed)
   {
       m_pContext->target->AcceptEvent(new ShowPreferencesEvent(0));
       return kError_NoErr;
   }
   if (oControlName == string("Quit") && eMesg == CM_Pressed)
   {
       m_pContext->target->AcceptEvent(new Event(CMD_QuitPlayer));
       return kError_NoErr;
   }
   if (eMesg == CM_ChangeWindow)
   {
       m_pWindow->ControlStringValue(oControlName, false, m_oCurrentWindow);
       SwitchWindow(m_oCurrentWindow);
   
       return kError_NoErr;
   }
   if (oControlName == string("ReloadTheme") && eMesg == CM_Pressed)
   {
       ReloadTheme();
       return kError_NoErr;
   }
   if (oControlName == string("Shuffle") && eMesg == CM_Pressed)
   {
       int iState = 0;
       string     oName("Info"), oDesc("");

       m_pWindow->ControlIntValue(oControlName, false, iState);
       iState = (iState + 1) % 2;
       m_pContext->plm->SetShuffleMode(iState == 1);
       m_pWindow->ControlIntValue(oControlName, true, iState);
       
       m_pWindow->ControlGetDesc(oControlName, oDesc);
       m_pWindow->ControlStringValue(oName, true, oDesc);
       
       return kError_NoErr;
   }    
   if (oControlName == string("Repeat") && eMesg == CM_Pressed)
   {
       int        iState = 0;
       RepeatMode eMode = kPlaylistMode_RepeatNone;
       string     oName("Info"), oDesc("");

       m_pWindow->ControlIntValue(oControlName, false, iState);
       iState = (iState + 1) % 3;
       m_pWindow->ControlIntValue(oControlName, true, iState);
       
       switch(iState)
       {
          case 0:  
              eMode = kPlaylistMode_RepeatNone;
              break;
          case 1:  
              eMode = kPlaylistMode_RepeatOne;
              break;
          case 2:  
              eMode = kPlaylistMode_RepeatAll;
              break;
       }
       m_pContext->plm->SetRepeatMode(eMode);
       
       m_pWindow->ControlGetDesc(oControlName, oDesc);
       m_pWindow->ControlStringValue(oName, true, oDesc);
       
       return kError_NoErr;
   }    
   if (oControlName == string("Time") && eMesg == CM_Pressed)
   {
       string oName("Info"), oDesc("");
       
       if (m_iTotalSeconds < 0)
           return kError_NoErr;       
          
       if (m_eTimeDisplayState == kNormal)
       {
           m_eTimeDisplayState = kTimeRemaining;
           if (m_pWindow->DoesControlExist("TimeRemaining"))
               m_pWindow->ControlStringValue("Time", true, oDesc);
       }    
       else     
       { 
           m_eTimeDisplayState = kNormal;
           if (m_pWindow->DoesControlExist("TimeRemaining"))
               m_pWindow->ControlStringValue("TimeRemaining", true, oDesc);
       }    
       
       m_pWindow->ControlStringValue("Info", true, oDesc);
       UpdateTimeDisplay(m_iCurrentSeconds);                      
   }
   if (oControlName == string("Logo") && eMesg == CM_Pressed)
   {
       string oUrl;
       
       m_pWindow->ControlStringValue("Logo", false, oUrl);
       if (oUrl.length() == 0)
          oUrl = BRANDING_URL;
       
#ifdef WIN32   
       Int32PropValue *pProp;
       HWND            hWnd;
       if (IsError(m_pContext->props->GetProperty("MainWindow", 
                   (PropValue **)&pProp)))
          hWnd = NULL;
       else
          hWnd = (HWND)pProp->GetInt32();
             
       ShellExecute(hWnd, "open", oUrl.c_str(),
                    NULL, NULL, SW_SHOWNORMAL);
#else
       LaunchBrowser((char *)oUrl.c_str());
#endif
       return kError_NoErr;
   }
   if (oControlName == string("Help") && eMesg == CM_Pressed)
   {
       ShowHelp();
       return kError_NoErr;
   }
   if (oControlName == string("Credits") && eMesg == CM_Pressed)
   {
       ShowThemeCredits();
       return kError_NoErr;
   }
  
   return kError_NoErr;
}

void FreeAmpTheme::InitControls(void)
{
	bool   bSet, bEnable;
    int    iState;
    string oWelcome(szWelcomeMsg);
    
    assert(m_pWindow);
    
    // Set the volume control
	if (m_iVolume >= 0)
    {  
       bSet = true;
       m_pWindow->ControlIntValue(string("Volume"), true, m_iVolume);
    }
    else 
       bSet = false;   
       
    m_pWindow->ControlEnable(string("Volume"), true, bSet);
    m_pWindow->ControlEnable(string("VolumePlus"), true, bSet);
    m_pWindow->ControlEnable(string("VolumeMinus"), true, bSet);

    // Set the seek control
    bSet = (m_iTotalSeconds < 0) ? false : true;
    if (bSet)
       m_pWindow->ControlIntValue(string("Seek"), true, m_iSeekPos);
    m_pWindow->ControlEnable(string("Seek"), true, bSet);
    
    // Set the Play/Pause buttons
    iState = m_bPlayShown ? 0 : 1;
    m_pWindow->ControlIntValue(string("PlayPause"), true, iState);
    m_pWindow->ControlIntValue(string("PlayStop"), true, iState);
    
    iState = m_bPaused ? 1 : 0;
    m_pWindow->ControlIntValue(string("MPause"), true, iState);
    bEnable = m_bPlayShown;
    m_pWindow->ControlEnable(string("Play"), true, bEnable);
    bEnable = !m_bPlayShown;
    m_pWindow->ControlEnable(string("Pause"), true, bEnable);
    m_pWindow->ControlEnable(string("Stop"), true, bEnable);

    // Set the title text field to the current meta data, or if no
    // is available, show the welcome message
    if (m_oTitle.length() == 0)
        m_pWindow->ControlStringValue(string("Title"), true, oWelcome);
    else    
        m_pWindow->ControlStringValue(string("Title"), true, m_oTitle);

    // Ask the playlist manager what the current repeat mode is
    // and set the controls appropriately.    
    switch (m_pContext->plm->GetRepeatMode())
    {
        case kPlaylistMode_RepeatNone:
           iState = 0;
           break;
        case kPlaylistMode_RepeatOne:
           iState = 1;
           break;
        case kPlaylistMode_RepeatAll:
           iState = 2;
           break;
        default:
           break;
    }
    m_pWindow->ControlIntValue(string("Repeat"), true, iState);

    iState = m_pContext->plm->GetShuffleMode() ? 1 : 0;
    m_pWindow->ControlIntValue(string("Shuffle"), true, iState);

    m_pWindow->ControlStringValue("StreamInfo", true, m_oStreamInfo);
}

// This function gets called after the window object is created,
// but before the window itself gets created
void FreeAmpTheme::InitWindow(void)
{        
	bool   bValue;
    
    m_pContext->prefs->GetStayOnTop(&bValue);
    m_pWindow->SetStayOnTop(bValue);

    m_pContext->prefs->GetLiveInTray(&bValue);
    m_pWindow->SetLiveInToolbar(bValue);
}

void FreeAmpTheme::ReloadTheme(void)
{
    char          szTemp[255];
    uint32        iLen = 255;
    string        oThemePath, oThemeFile("theme.xml");
    Error         eRet;

    m_pContext->prefs->GetPrefString(kThemePathPref, szTemp, &iLen);
    oThemePath = szTemp;

    iLen = 255;
    m_pContext->prefs->GetPrefString(kThemeDefaultFontPref, szTemp, &iLen);
    SetDefaultFont(string(szTemp));

    eRet = LoadTheme(oThemePath, m_oCurrentWindow);
    if (IsError(eRet))					   
    {
        MessageDialog oBox(m_pContext);
	    string        oErr, oMessage(szParseError);
  
        GetErrorString(oErr);
        oMessage += oErr;
        oBox.Show(oMessage.c_str(), string(BRANDING), kMessageOk);
    }	
}

void FreeAmpTheme::SetVolume(int iVolume)
{       
    string oVol("Volume: ");
    char   szPercent[10];
    
    m_iVolume = iVolume;
    m_pContext->target->AcceptEvent(new 
        VolumeEvent(CMD_SetVolume, m_iVolume));

    m_pWindow->ControlIntValue(string("Volume"), true, m_iVolume);

    sprintf(szPercent, "%d%%", iVolume);
    oVol += string(szPercent);
    m_pWindow->ControlStringValue(string("Info"), true, oVol);
}    

void FreeAmpTheme::HandleKeystroke(unsigned char cKey)
{
    switch(cKey)
    {
     case 'p':
     case 'P':
        m_pContext->target->AcceptEvent(new Event(CMD_Play));
        break;

     case 'S':
     case 's':
        m_pContext->target->AcceptEvent(new Event(CMD_Stop));
        break;

     case 'u':
     case 'U':
        m_pContext->target->AcceptEvent(new Event(CMD_Pause));
        break;

     case 'M':
     case 'm':
        m_pContext->target->AcceptEvent(new Event(CMD_ToggleMusicBrowserUI));
        break;

     case 'n':
     case 'N':
        m_pContext->target->AcceptEvent(new Event(CMD_NextMediaPiece));
        break;

     case 'r':
     case 'R':
        m_pContext->target->AcceptEvent(new Event(CMD_PrevMediaPiece));
        break;

     case 'd':
     case 'D':
        m_pContext->target->AcceptEvent(new Event(CMD_ToggleDownloadUI));
        break;
        
     case 'o':
     case 'O':
        m_pContext->target->AcceptEvent(new ShowPreferencesEvent(0));
        break;

     case 'c':
     case 'C':
        ShowThemeCredits();
        break;
 
     case 't':
     case 'T':
        ReloadTheme();
        break;
       
     case 'h':
     case 'H':
     	ShowHelp();
        break;
    }
}

bool FreeAmpTheme::HandleMenuCommand(uint32 uCommand)
{
    switch(uCommand)
    {
        case kMCMyMusic:
            m_pContext->target->AcceptEvent(
                       new Event(CMD_ToggleMusicBrowserUI));
            return true;           

        case kMCPlay:
            m_pContext->target->AcceptEvent(
                       new Event(CMD_Play));
            return true;           

        case kMCStop:
            m_pContext->target->AcceptEvent(
                       new Event(CMD_Stop));
            return true;           

        case kMCPause:
            m_pContext->target->AcceptEvent(
                       new Event(CMD_Pause));
            return true;           

        case kMCNext:
            m_pContext->target->AcceptEvent(
                       new Event(CMD_NextMediaPiece));
            return true;           

        case kMCPrev:
            m_pContext->target->AcceptEvent(
                       new Event(CMD_PrevMediaPiece));
            return true;           

        case kMCExit:
            m_pContext->target->AcceptEvent(
                       new Event(CMD_QuitPlayer));
            return true;           
    }
    
    return false;
}

void FreeAmpTheme::UpdateTimeDisplay(int iCurrentSeconds)
{
    string oText;
    char   szText[20];
    int    iSeconds;
    
    if (m_eTimeDisplayState == kTimeRemaining && m_iTotalSeconds >= 0)
    {    
        iSeconds = m_iTotalSeconds - iCurrentSeconds - 1;
        if (iSeconds > 3600)
            sprintf(szText, "-%d:%02d:%02d", 
                    iSeconds / 3600,
                    (iSeconds % 3600) / 60,
                    iSeconds % 60);
        else                
            sprintf(szText, "-%d:%02d", 
                    (iSeconds % 3600) / 60,
                    iSeconds % 60);
                
    }    
    else    
    if (iCurrentSeconds >= 0)
    {
        if (iCurrentSeconds > 3600)
           sprintf(szText, "%d:%02d:%02d", 
                   iCurrentSeconds / 3600,
                   (iCurrentSeconds % 3600) / 60,
                   iCurrentSeconds % 60);
        else           
           sprintf(szText, "%d:%02d", 
                   (iCurrentSeconds % 3600) / 60,
                   iCurrentSeconds % 60);
    }
    else    
        sprintf(szText, "0:00");
            
    oText = string(szText);
    if (m_eTimeDisplayState == kTimeRemaining && 
        m_pWindow->DoesControlExist("TimeRemaining") &&
        m_iTotalSeconds >= 0)
        m_pWindow->ControlStringValue("TimeRemaining", true, oText);
    else
        m_pWindow->ControlStringValue("Time", true, oText);
}

void FreeAmpTheme::UpdateMetaData(const PlaylistItem *pItem)
{
    if (pItem->GetMetaData().Title().length() > 0 || 
        pItem->GetMetaData().Artist().length() > 0)
    {
        string oText;
        
        m_oTitle = pItem->GetMetaData().Title();
        if (pItem->GetMetaData().Artist().length() > 0)
           m_oTitle += string(" - ") + pItem->GetMetaData().Artist();

        oText = string(BRANDING": ") + m_oTitle;
        m_pWindow->SetTitle(oText);
    }    
    else
        m_oTitle = "";
        
    m_pWindow->ControlStringValue(string("Title"), true, m_oTitle);
}

void FreeAmpTheme::DropFiles(vector<string> *pFileList)
{
    char                     ext[_MAX_PATH];
    char                     url[_MAX_PATH + 7];
    uint32                   length, countbefore;
    vector<string>::iterator i;
    
    countbefore = m_pContext->plm->CountItems();
    for(i = pFileList->begin(); i != pFileList->end(); i++)
    {
        char          *pExtension = NULL;
        vector<char*>  fileList;
        struct _stat   st;

        _stat((*i).c_str(), &st);
        if(st.st_mode & _S_IFDIR)
        {
            HANDLE          findFileHandle = NULL;
            WIN32_FIND_DATA findData;
            char            findPath[_MAX_PATH + 1];
            char*           file;
            vector<PlaylistItem*> oList;

            strcpy(findPath, (*i).c_str());
            strcat(findPath, DIR_MARKER_STR);
            strcat(findPath, "*.*");

            file = strrchr(findPath, DIR_MARKER) + 1;

            findFileHandle = FindFirstFile(findPath, &findData);
            if(findFileHandle != INVALID_HANDLE_VALUE)
            {
                do
                {
                    pExtension = strrchr(findData.cFileName, '.');
                    if (!pExtension)
                       continue;
                    
                    strcpy(ext, pExtension + 1);
                    ToUpper(ext);   
                    if (m_pContext->player->IsSupportedExtension(ext))
                    {   
                        strcpy(findPath, (*i).c_str());
                        strcat(findPath, DIR_MARKER_STR);
                        strcat(findPath, findData.cFileName);
                        
                        length = sizeof(url);
                        FilePathToURL(findPath, url, &length);
                        PlaylistItem* item = new PlaylistItem(url);
                        oList.push_back(item);
                    }   

                }while(FindNextFile(findFileHandle, &findData));

                FindClose(findFileHandle);
            }
            if (oList.size() > 0)
               m_pContext->plm->AddItems(&oList);
        }
        else
        {
            PlaylistFormatInfo     oInfo;              
            char                   ext[_MAX_PATH];
            int                    j;
            Error                  eRet = kError_NoErr;
            
            pExtension = strrchr((*i).c_str(), '.');
            if (!pExtension)
               continue;
            
            strcpy(ext, pExtension + 1);
            ToUpper(ext);
            for(j = 0; ; j++)
            {
               eRet = m_pContext->plm->GetSupportedPlaylistFormats(&oInfo, j);
               if (IsError(eRet))
                  break;
            
               if (strcasecmp(oInfo.GetExtension(), ext) == 0)
                  break;   
            }
            if (!IsError(eRet))
            {
                length = sizeof(url);
                FilePathToURL((*i).c_str(), url, &length);
                
                m_pContext->plm->ReadPlaylist(url);
            }   
            else   
                if (m_pContext->player->IsSupportedExtension(ext))
                {
                    length = sizeof(url);
                    FilePathToURL((*i).c_str(), url, &length);
                
                    m_pContext->plm->AddItem(url);
                }    
        }
    }
    
    if (countbefore == 0)
        m_pContext->target->AcceptEvent(new Event(CMD_Play));
}

void FreeAmpTheme::PostWindowCreate(void)
{
#ifdef WIN32
    Int32PropValue *pProp;
    
    pProp = new Int32PropValue((int)((Win32Window *)m_pWindow)->GetWindowHandle());
    m_pContext->props->SetProperty("MainWindow", pProp);
#endif
    string winTitle = string(BRANDING);
    m_pWindow->SetTitle(winTitle);
}

void FreeAmpTheme::ShowHelp(void)
{
    string            oHelpFile;
    char              dir[_MAX_PATH];
    uint32            len = sizeof(dir);
    
    m_pContext->prefs->GetInstallDirectory(dir, &len);
    oHelpFile = string(dir);
    oHelpFile += string(DIR_MARKER_STR);    
#ifdef unix
    oHelpFile += string("../share/");
#endif
    oHelpFile += string(HELP_FILE);    
    
#ifdef WIN32   
    Int32PropValue *pProp;
    HWND            hWnd;
    if (IsError(m_pContext->props->GetProperty("MainWindow", 
                (PropValue **)&pProp)))
       hWnd = NULL;
    else
       hWnd = (HWND)pProp->GetInt32();
    
    WinHelp(hWnd, oHelpFile.c_str(), HELP_FINDER, FreeAmp_Main_Window);
#endif
#ifdef HAVE_GTK   
    struct _stat   st;

    if (_stat(oHelpFile.c_str(), &st) == 0 && st.st_mode & S_IFREG)
        LaunchBrowser((char *)oHelpFile.c_str());
    else
    {
          MessageDialog oBox(m_pContext);
          string        oMessage(szCantFindHelpError);

          oBox.Show(oMessage.c_str(), string(BRANDING), kMessageOk, true);
    }
#endif
}

void FreeAmpTheme::update_thread(void* arg)
{
    FreeAmpTheme* _this = (FreeAmpTheme*)arg;

    _this->UpdateThread();
}

void FreeAmpTheme::UpdateThread()
{
#ifdef WIN32

    m_pUpdateMan->RetrieveLatestVersionInfo();

    if(m_pUpdateMan->IsUpdateAvailable())
    {
        if(0 < DialogBoxParam(g_hinst, 
                              MAKEINTRESOURCE(IDD_UPDATEAVAILABLE),
                              NULL, 
                              (int (__stdcall *)(void))::UpdateAvailableDlgProc, 
                              (LPARAM) 0))
        {
            ShowOptions(4);
        }
    }

#endif

    delete m_pUpdateThread;
}

void FreeAmpTheme::ShowOptions(uint32 defaultPage)
{
    OptionsArgs *oArgs = new OptionsArgs;
    
    if (m_bInOptions)
       return;
       
    if (m_pOptionsThread)
    {  
        delete m_pOptionsThread;
        m_pOptionsThread = NULL;
    }

    oArgs->pThis = this;
    oArgs->uDefaultPage = defaultPage;

    m_pOptionsThread = Thread::CreateThread();
    m_pOptionsThread->Create(options_thread, oArgs);
}

void FreeAmpTheme::options_thread(void* arg)
{
    OptionsArgs *pArgs = (OptionsArgs *)arg;
  
    pArgs->pThis->OptionsThread(pArgs->uDefaultPage);

    delete pArgs;
}

void FreeAmpTheme::OptionsThread(uint32 defaultPage)
{
    PreferenceWindow *pWindow;

    m_bInOptions = true;

#ifdef WIN32
    pWindow = new Win32PreferenceWindow(m_pContext, m_pThemeMan, m_pUpdateMan, defaultPage);
#elif defined(__BEOS__)
    pWindow = new BeOSPreferenceWindow(m_pContext, m_pThemeMan);
#else
    pWindow = new GTKPreferenceWindow(m_pContext, m_pThemeMan, defaultPage); 
#endif

    pWindow->Show(m_pWindow);

    delete pWindow;
   
    m_bInOptions = false;
}

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
#elif defined(__BEOS__)
#include "BeOSPreferenceWindow.h"
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

const char *szParseError = "Parsing the Theme description failed. Cause: ";
const int iVolumeChangeIncrement = 10;

extern    "C"
{
   UserInterface *Initialize(FAContext * context)
   {
      Debug_v("##Clear");
      return new FreeAmpTheme(context);
   }
}

FreeAmpTheme::FreeAmpTheme(FAContext * context)
             :Theme(context)
{
   m_pContext = context;
   m_iCurrentSeconds = 0;
   m_iTotalSeconds = -1;
   m_iSeekSeconds = 0;
   m_bSeekInProgress = false;
   m_oCurrentWindow = string("MainWindow");
   m_iVolume = -1;
   m_iSeekPos = -1;
   m_bPlayShown = true;
   m_oTitle = string("");
   m_eTimeDisplayState = kNormal;

   LoadFreeAmpTheme();
}

FreeAmpTheme::~FreeAmpTheme()
{
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

   m_pContext->prefs->GetPrefString(kMainWindowPosPref, szTemp, &iLen);
   sscanf(szTemp, " %d , %d", &m_oWindowPos.x, &m_oWindowPos.y);

   eRet = Theme::Run(m_oWindowPos);
   if (!IsError(eRet))
   {
       sprintf(szTemp, "%d,%d", m_oWindowPos.x, m_oWindowPos.y);
       m_pContext->prefs->SetPrefString(kMainWindowPosPref, szTemp);
   }
   else     
       m_pContext->target->AcceptEvent(new Event(CMD_QuitPlayer));
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
  
   iLen = 255; 
   m_pContext->prefs->GetPrefString(kThemeDefaultFontPref, szTemp, &iLen);
   SetDefaultFont(string(szTemp));

   eRet = LoadTheme(oThemePath, m_oCurrentWindow);
   if (IsError(eRet))					   
   {
       MessageDialog oBox;
       string        oErr, oMessage(szParseError);

       GetErrorString(oErr);
       oMessage += oErr;
       oBox.Show(oMessage.c_str(), string(BRANDING), kMessageOk);
   }
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
         MessageDialog      oBox;
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
         bool bEnable = false;
         
         int iState = 1;
         m_pWindow->ControlIntValue(string("Play"), true, iState);
         m_pWindow->ControlEnable(string("IPlay"), true, bEnable);
         bEnable = true;
         m_pWindow->ControlEnable(string("IPause"), true, bEnable);
         
         m_bPlayShown = true;
         break;
      }   
      case INFO_Paused:
      case INFO_Stopped:
      {
         int iState = 0;
         bool bEnable = true;
         
         m_pWindow->ControlIntValue(string("Play"), true, iState);
         m_pWindow->ControlEnable(string("IPlay"), true, bEnable);
         bEnable = false;
         m_pWindow->ControlEnable(string("IPause"), true, bEnable);
         m_bPlayShown = false;
         
         if (e->Type() == INFO_Stopped)
         {
            m_iSeekPos = 0;
            m_iTotalSeconds = -1;
            m_pWindow->ControlIntValue(string("Seek"), true, m_iSeekPos);
            UpdateTimeDisplay();
         }
         
         break;
      }   
      case INFO_DoneOutputting:
      {
         m_iSeekPos = 0;
         m_iTotalSeconds = -1;
         m_pWindow->ControlIntValue(string("Seek"), true, m_iSeekPos);
         UpdateTimeDisplay();
         
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

		 sprintf(szTemp, "I: %3d O: %3d %c", 
                 info->GetInputPercent(),
                 info->GetOutputPercent(),
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
         UpdateTimeDisplay();

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

         m_fSecondsPerFrame = info->GetSecondsPerFrame();
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
      
      default:
         break;
   }
   return 0;
}

Error FreeAmpTheme::HandleControlMessage(string &oControlName, 
                                         ControlMessageEnum eMesg)
{
   char szTemp[255];

   if (eMesg == CM_MouseEnter)
   {
       string oName("Info"), oDesc("");
       
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
       sprintf(szTemp, "%d:%02d:%02d", iTime / 3600,
               (iTime % 3600) / 60, iTime % 60);
       oText = string(szTemp);        
       m_bSeekInProgress = true;
       
       m_pWindow->ControlStringValue(oName, true, oText);
              
       return kError_NoErr;
   }    
   if (oControlName == string("Play") && eMesg == CM_Pressed)
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
   if (oControlName == string("IPlay") && eMesg == CM_Pressed)
   {
       if (m_pContext->plm->CountItems() == 0)
       {
           m_pContext->target->AcceptEvent(new Event(CMD_ToggleMusicBrowserUI));
           return kError_NoErr;
       }
       m_pContext->target->AcceptEvent(new Event(CMD_Play));
       return kError_NoErr;
   }
   if (oControlName == string("IPause") && eMesg == CM_Pressed)
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
   if (oControlName == string("Playlist") && eMesg == CM_Pressed)
   {
       m_pContext->target->AcceptEvent(new Event(CMD_TogglePlaylistUI));
       return kError_NoErr;
   }
   if (oControlName == string("Download") && eMesg == CM_Pressed)
   {
       m_pContext->target->AcceptEvent(new Event(CMD_ToggleDownloadUI));
       return kError_NoErr;
   }
   if (oControlName == string("Options") && eMesg == CM_Pressed)
   {
   	   ShowOptions();

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
       string oName("Info"), oDesc;
       
       if (m_iTotalSeconds < 0)
           return kError_NoErr;       
          
       if (m_eTimeDisplayState == kNormal)
       {
           m_eTimeDisplayState = kTimeRemaining;
           oDesc = "Time remaining";
       }    
       else     
       { 
           m_eTimeDisplayState = kNormal;
           oDesc = "Current time";
       }    
       
       m_pWindow->ControlStringValue("Info", true, oDesc);
       UpdateTimeDisplay();                      
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
   }
   if (oControlName == string("Help") && eMesg == CM_Pressed)
   {
       ShowHelp();
       
   }
   
   return kError_NoErr;
}

void FreeAmpTheme::InitControls(void)
{
	bool   bSet, bEnable;
    int    iState;
    string oWelcome("Welcome to the "BRANDING" player!");
    
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
    m_pWindow->ControlIntValue(string("Play"), true, iState);
    bEnable = m_bPlayShown;
    m_pWindow->ControlEnable(string("IPlay"), true, bEnable);
    bEnable = !m_bPlayShown;
    m_pWindow->ControlEnable(string("IPause"), true, bEnable);
    

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
        MessageDialog oBox;
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
     	ShowOptions();
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

void FreeAmpTheme::ShowOptions(void)
{
    PreferenceWindow *pWindow;
       
#ifdef WIN32
    pWindow = new Win32PreferenceWindow(m_pContext, m_pThemeMan);
#elif defined(__BEOS__)
    pWindow = new BeOSPreferenceWindow(m_pContext, m_pThemeMan);
#else
    pWindow = new GTKPreferenceWindow(m_pContext, m_pThemeMan);
#endif       
    pWindow->Show(m_pWindow);
          
    delete pWindow;
}

void FreeAmpTheme::UpdateTimeDisplay(void)
{
    string oText;
    char   szText[20];
    int    iSeconds;
    
    if (m_eTimeDisplayState == kTimeRemaining &&
        m_iTotalSeconds > 0)
    {    
        iSeconds = m_iTotalSeconds - m_iCurrentSeconds;
        if (iSeconds > 3600)
            sprintf(szText, "-%d:%02d:%02d", 
                    iSeconds / 3600,
                    (iSeconds % 3600) / 60,
                    iSeconds % 60);
        else                
            sprintf(szText, "-%02d:%02d", 
                    (iSeconds % 3600) / 60,
                    iSeconds % 60);
                
    }    
    else    
    {
        if (m_iCurrentSeconds > 3600)
           sprintf(szText, "%d:%02d:%02d", 
                   m_iCurrentSeconds / 3600,
                   (m_iCurrentSeconds % 3600) / 60,
                   m_iCurrentSeconds % 60);
        else           
           sprintf(szText, "%02d:%02d", 
                   (m_iCurrentSeconds % 3600) / 60,
                   m_iCurrentSeconds % 60);
    }
            
    oText = string(szText);
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
           m_oTitle += string(" -- ") + pItem->GetMetaData().Artist();

        m_pWindow->ControlStringValue(string("Title"), true, m_oTitle);
        oText = string(BRANDING": ") + m_oTitle;
        m_pWindow->SetTitle(oText);
    }    
    else
        m_oTitle = "";
}

void FreeAmpTheme::DropFiles(vector<string> *pFileList)
{
    char                     ext[MAX_PATH];
    char                     url[MAX_PATH + 7];
    uint32                   length;
    vector<string>::iterator i;
    
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
            char            findPath[MAX_PATH + 1];
            char*           file;

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
                       //strcpy(findPath, (*i).c_str());
                       //strcat(findPath, DIR_MARKER_STR);
                       //strcat(findPath, findData.cFileName);
                        strcpy(file, findData.cFileName);
                        
                        length = sizeof(url);
                        FilePathToURL(file, url, &length);
                        
                        m_pContext->plm->AddItem(url);
                    }   

                }while(FindNextFile(findFileHandle, &findData));

                FindClose(findFileHandle);
            }
        }
        else
        {
            PlaylistFormatInfo     oInfo;              
            char                   ext[MAX_PATH];
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
                    //string url;

                    //url = string("file://") + (*i);
                    //m_pContext->plm->AddItem(url.c_str(),
                    //     m_pContext->plm->CountItems());
                    length = sizeof(url);
                    FilePathToURL((*i).c_str(), url, &length);
                
                    m_pContext->plm->AddItem(url);
                }    
        }
    }
}

void FreeAmpTheme::PostWindowCreate(void)
{
#ifdef WIN32
    Int32PropValue *pProp;
    
    pProp = new Int32PropValue((int)((Win32Window *)m_pWindow)->GetWindowHandle());
    m_pContext->props->SetProperty("MainWindow", pProp);
#endif
    m_pWindow->SetTitle(string(BRANDING));
}

void FreeAmpTheme::ShowHelp(void)
{
    string            oHelpFile;
    char              dir[MAX_PATH];
    uint32            len = sizeof(dir);
    
    m_pContext->prefs->GetInstallDirectory(dir, &len);
    oHelpFile = string(dir);
    oHelpFile += string("\\");    
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
}

/*____________________________________________________________________________
        
        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 1998 GoodNoise

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

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "config.h"
#include "thread.h"
#include "event.h"
#include "queue.h"
#include "ui.h"
#include "list.h"
#include "mutex.h"
#include "playlist.h"
#include "semaphore.h"
#include "lmcregistry.h"
#include "pmiregistry.h"
#include "pmoregistry.h"
#include "uiregistry.h"
#include "preferences.h"
#include "properties.h"
#include "propimpl.h"

#include "lmc.h"

typedef enum
{
   STATE_Paused = 1,
   STATE_Playing,
   STATE_Stopped,
}
PlayerState;

const int iID3TitleLength = 30;
const int iID3ArtistLength = 30;
const int iID3AlbumLength = 30;
struct ID3Tag
{
   char szTag[3];
   char szTitle[iID3TitleLength];
   char szArtist[iID3ArtistLength];
   char szAlbum[iID3AlbumLength];
   char szYear[4];
   char szComment[30];
   char cGenre;
}; 

class     Player:public EventQueue, Properties, PropertyWatcher
{

   public:
   // Player();
   static Player *GetPlayer();
            ~Player();

   int32     RegisterActiveUI(UserInterface * ui);
   int32     RegisterLMCs(LMCRegistry * registry);
   int32     RegisterPMIs(PMIRegistry * registry);
   int32     RegisterPMOs(PMORegistry * registry);
   int32     RegisterUIs(UIRegistry * registry);

   void      Run();
   bool      SetArgs(int32 argc, char **argv);
   void      SetPreferences(Preferences *);
   void      SetTerminationSemaphore(Semaphore *);
   void      testQueue();
   static void EventServiceThreadFunc(void *);
   virtual int32 AcceptEvent(Event *);
   virtual RegistryItem *ChoosePMI(char *szUrl, char *szTitle = NULL);

   // Properties
   virtual Error GetProperty(const char *, PropValue **);
   virtual Error SetProperty(const char *, PropValue *);
   virtual Error RegisterPropertyWatcher(const char *, PropertyWatcher *);
   virtual Error RemovePropertyWatcher(const char *, PropertyWatcher *);

   virtual Error PropertyChange(const char *, PropValue *);
             protected:
             Player();
   void      GetUIManipLock();
   void      ReleaseUIManipLock();
   int32     CompareNames(const char *, const char *);
   void      SendToUI(Event *);
   void      Usage(const char *);
   bool      SetState(PlayerState);
   int32     ServiceEvent(Event *);
   void      CreateLMC(PlayListItem * pc, Event * pC);

   private:

   // These are event loop handling functions
   void DoneOutputting(Event *pEvent) ;
   void Stop(Event *pEvent);
   void ChangePosition(Event *pEvent);
   void GetMediaInfo(Event *pEvent) ;
   void Play(Event *pEvent);
   void Next(Event *pEvent);
   void Previous(Event *pEvent);
   void Pause(Event *pEvent);
   void UnPause(Event *pEvent);
   void TogglePause(Event *pEvent);
   int  Quit(Event *pEvent);
   int  ReadyToDieUI(Event *pEvent);
   void UserMessage(Event *pEvent);
   void HandleMediaInfo(Event *pEvent);
   void HandleMediaTimeInfo(Event *pEvent);
   void SendEventToUI(Event *pEvent);
   void LMCError(Event *pEvent);
   void GetMediaTitle(Event *pEvent);

   #define _EQUALIZER_ENABLE_
   #ifdef  _EQUALIZER_ENABLE_
   void Player::SetEQData(Event *pEvent);
   #endif // _EQUALIZER_ENABLE_
   #undef  _EQUALIZER_ENABLE_    

   #define _VISUAL_ENABLE_
   #ifdef  _VISUAL_ENABLE_
   void Player::SendVisBuf(Event *pEvent); 
   #endif // _VISUAL_ENABLE_
   #undef  _VISUAL_ENABLE_  

   PropertiesImpl m_props;
   bool      m_didUsage;
   bool      m_autoplay;
   Preferences *m_prefs;
   Semaphore *m_pTermSem;
   static Player *m_thePlayer;
   Semaphore *m_eventSem;
   PlayerState m_playerState;
   Queue < Event * >*m_eventQueue;
   Thread   *m_eventServiceThread;
   int32     m_quitWaitingFor;  // keeps track of how many CIO's 
   // and COO's haven't sent in 
   // their "Ready To Die" infos.

   int32     m_imQuitting;
   List < UserInterface * >*m_uiList;

   Mutex    *m_uiManipLock;
   Mutex    *m_lmcMutex;
   Mutex    *m_pmiMutex;
   Mutex    *m_pmoMutex;
   Mutex    *m_uiMutex;
   PlayListManager *m_plm;

   LogicalMediaConverter *m_lmc;
   UserInterface *m_ui;

   LMCRegistry *m_lmcRegistry;
   PMIRegistry *m_pmiRegistry;
   PMORegistry *m_pmoRegistry;
   UIRegistry *m_uiRegistry;

    List < char *>*m_argUIList;

   int32     m_argc;
   char    **m_argv;

};

#endif // _PLAYER_H_

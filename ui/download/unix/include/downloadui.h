/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1999 EMusic.com

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

#ifndef INCLUDED_DOWNLOAD_UI_H_
#define INCLUDED_DOWNLOAD_UI_H_

/* system headers */
#include <stdlib.h>
#include <gtk/gtk.h>

/* project headers */
#include "config.h"
#include "ui.h"
#include "semaphore.h"
#include "event.h"
#include "thread.h"
#include "mutex.h"
#include "queue.h"
#include "facontext.h"
#include "preferences.h"
#include "downloadmanager.h"


class DownloadUI : public UserInterface {
 public:
    DownloadUI(FAContext *context);
    ~DownloadUI();

    virtual Error Init(int32 startup_type);
    virtual Error AcceptEvent(Event*);
 
    void GTKEventService(void);
    void ToggleVisEvent(void);
    void UpdateDownloadList(void);
    void CancelEvent(void);
    void PauseEvent(void);
    void ResumeEvent(void);
    void SelChangeEvent(int row);
    void ShowHelp(void);
    void CloseWindow(void);
    void SetRunning(void);

    bool doQuitNow;

 protected:

    void ParseArgs(int32 argc, char** argv);

    static void UIThreadFunc(void *);

 private:
    FAContext*	        m_context;
    PlaylistManager*    m_plm;
    Preferences*        m_prefs;
    Properties*         m_propManager;
    Thread*             gtkThread;
    EventQueue*         m_target;
    DownloadManager*    m_dlm;

    bool weAreGTK;
    bool m_initialized;
    bool isVisible;
   
    uint32 m_currentindex;
    
    void CreateDownloadUI(void);
    void CreateDownloadList(GtkWidget *box);
    void UpdateOverallProgress(void);
    void UpdateInfo(void);
    
    GtkWidget *m_downloadUI;
    GtkWidget *m_List;
    GtkWidget *m_Info;
    GtkWidget *m_PauseButton;
    GtkWidget *m_CancelButton;
    GtkWidget *m_ResumeButton;
    GtkWidget *m_ResumeLabel;
    GtkWidget *m_CloseButton;
    GtkWidget *m_hwndProgress;
    GtkWidget *m_helpButton;

    GtkWidget *artist;
    GtkWidget *album;
    GtkWidget *title;
    GtkWidget *genre;
    GtkWidget *playlist;
    GtkWidget *name;
    GtkWidget *size;
   
    vector<DownloadItem *> downloadList;
};

#endif // INCLUDED_DOWNLOAD_UI_H_

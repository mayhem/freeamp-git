/*____________________________________________________________________________

        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 1999-2000 EMusic.com

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

#include "musicbrowserui.h"
#include "gtkmusicbrowser.h" 
#include "infoeditor.h"
#include "eventdata.h"
#include "player.h"
#include "pmo.h"

#include <algorithm>
using namespace std;

extern "C" {

UserInterface *Initialize(FAContext *context) {
    return new MusicBrowserUI(context);
}
           }

MusicBrowserUI::MusicBrowserUI(FAContext *context)
{
    m_context = context;
    searching = NULL;
    wiz = NULL;
}

MusicBrowserUI::~MusicBrowserUI()
{
}

Error MusicBrowserUI::Init(int32 startup_level) 
{
    if ((m_startupType = startup_level) != SECONDARY_UI_STARTUP) {
        cout << "The musicbrowser cannot run as the primary ui\n"; 
        return kError_InitFailedSafely;
    }

    if (!getenv("DISPLAY"))
        return kError_InitFailedSafely;

    m_playerEQ = m_context->target;

    m_argc = m_context->argc;
    m_argv = m_context->argv;

    string URL = string("file://") + FreeampDir(NULL) +
                 string("/currentlist.m3u");
    mainBrowser = new GTKMusicBrowser(m_context, this, URL);

    gtkThread = Thread::CreateThread();
    gtkThread->Create(MusicBrowserUI::gtkServiceFunction, this);

    return kError_NoErr;
}

void MusicBrowserUI::gtkServiceFunction(void *p)
{
    assert(p);
    ((MusicBrowserUI *)p)->GTKEventService();
}

void MusicBrowserUI::SetRunning(void)
{
    m_context->gtkRunning = true;
}

static int musicbrowser_timeout(MusicBrowserUI *p)
{
    p->SetRunning();
    if (p->doQuitNow)
        gtk_main_quit();

    return TRUE;
}

void MusicBrowserUI::DoCDCheck(void)
{
    Registry *pmoRegistry = m_context->player->GetPMORegistry();
    RegistryItem *pmo_item = NULL;
    int32 i = 0;

    while (NULL != (pmo_item = pmoRegistry->GetItem(i++))) {
        if (!strcmp("cd.pmo", pmo_item->Name())) {
            break;
        }
    }

    if (!pmo_item)
        return;

    PhysicalMediaOutput *pmo;
    pmo = (PhysicalMediaOutput *)pmo_item->InitFunction()(m_context);
    pmo->SetPropManager((Properties *)(m_context->player));

    if (IsError(pmo->Init(NULL))) {
        CDInfoEvent *cie = new CDInfoEvent(0, 0, "");
        m_playerEQ->AcceptEvent(cie);
    }

    delete pmo;
}
    
static int cd_check_timeout(MusicBrowserUI *p)
{
    p->DoCDCheck();

    return TRUE;
}


void MusicBrowserUI::GTKEventService(void)
{
    weAreGTK = false;
    doQuitNow = false;
 
    m_context->gtkLock.Acquire();
    if (!m_context->gtkInitialized) {
        m_context->gtkInitialized = true;
        g_thread_init(NULL);
        gtk_init(&m_argc, &m_argv);
        gdk_rgb_init();
        weAreGTK = true;
    }
    m_context->gtkLock.Release();

    gtk_timeout_add(5000, cd_check_timeout, this);

    if (weAreGTK) {
        gtk_timeout_add(250, musicbrowser_timeout, this);
        gtk_main();
        gdk_threads_leave();
    }
}

Error MusicBrowserUI::AcceptEvent(Event *event)
{
    switch (event->Type()) {
        case CMD_Cleanup: {
            mainBrowser->Close(false);
            delete mainBrowser;

            while (browserWindows.size() > 0) {
                browserWindows[0]->Close(false);
                delete browserWindows[0];
                browserWindows.erase(browserWindows.begin());
            }

            if (weAreGTK) 
                doQuitNow = true;

            if (searching)
                searching->Close();
            if (wiz)
                wiz->Close();

            gtkThread->Join();
            m_playerEQ->AcceptEvent(new Event(INFO_ReadyToDieUI));
            break; }
        case INFO_SearchMusicDone:
        case INFO_BrowserMessage: {
            mainBrowser->AcceptEvent(event);
            vector<GTKMusicBrowser *>::iterator i = browserWindows.begin();
            for (; i != browserWindows.end(); i++)
                (*i)->AcceptEvent(event);
            if (searching)
                searching->AcceptEvent(event);
            if (wiz)
	        wiz->AcceptEvent(event);
            if (event->Type() == INFO_SearchMusicDone) {
	        if (searching) {
                    searching->Close();
		}
		if (wiz) {
		    wiz->Close();
		}
            }
            break; }
        case CMD_TogglePlaylistUI: {
            if (mainBrowser->Visible())
                mainBrowser->Close();
            else
                mainBrowser->ShowPlaylist();
            break; } 
        case CMD_ToggleMusicBrowserUI: {
            if (mainBrowser->Visible())
                mainBrowser->Close();
            else
                mainBrowser->ShowMusicBrowser();
            break; }
        case CMD_AddFiles:
        case INFO_MusicCatalogRegenerating:
        case INFO_MusicCatalogDoneRegenerating:
        case INFO_PlaylistCurrentItemInfo:
        case INFO_PlaylistItemUpdated:
        case INFO_PlaylistItemMoved:
        case INFO_CDDiscStatus: 
        case INFO_PrefsChanged: {
            mainBrowser->AcceptEvent(event);
            vector<GTKMusicBrowser *>::iterator i = browserWindows.begin();
            for (; i != browserWindows.end(); i++)
                    (*i)->AcceptEvent(event);
            break; }
        case INFO_PlaylistItemAdded:
        case INFO_PlaylistItemsAdded:
        case INFO_PlaylistItemRemoved:
        case INFO_PlaylistRepeat:
        case INFO_PlaylistShuffle:
        case INFO_PlaylistSorted:
        case INFO_Paused:
        case INFO_Stopped:
        case INFO_Playing:
        case INFO_MusicCatalogCleared:
        case INFO_MusicCatalogPlaylistAdded:
        case INFO_MusicCatalogPlaylistRemoved:
        case INFO_MusicCatalogStreamAdded:
        case INFO_MusicCatalogStreamRemoved:
        case INFO_MusicCatalogTrackChanged:
        case INFO_MusicCatalogTrackRemoved: 
        case INFO_MusicCatalogTrackAdded: {
            if (mainBrowser->Visible())
                mainBrowser->AcceptEvent(event);
            vector<GTKMusicBrowser *>::iterator i = browserWindows.begin();
            for (; i != browserWindows.end(); i++)
                if ((*i)->Visible())
                    (*i)->AcceptEvent(event);
            break; }
        default:
            break;
    }
    return kError_NoErr;
}

void MusicBrowserUI::CreateNewEditor(string &newPlaylist, bool cd_mode)
{
    GTKMusicBrowser *newUI = new GTKMusicBrowser(m_context, this, newPlaylist,
                                                 cd_mode);
    gdk_threads_leave();
    newUI->ShowMusicBrowser();
    gdk_threads_enter();
    browserWindows.push_back(newUI);
}

void MusicBrowserUI::WindowClose(GTKMusicBrowser *oldUI)
{
    vector<GTKMusicBrowser *>::iterator
        loc = find(browserWindows.begin(), browserWindows.end(), oldUI);

    if (loc != browserWindows.end()) {
        delete *loc;
        browserWindows.erase(loc);
    }
}

void MusicBrowserUI::StartSearch(bool runMain, bool intro)
{
    if (wiz || searching)
        return;

    if (intro) {
        wiz = new IntroWizardUI(m_context, this);
	wiz->Show(runMain);
    }
    else {
        searching = new musicsearchUI(m_context, this);
        searching->Show(runMain);
    }    
}

void MusicBrowserUI::SearchClose(void)
{
    if (!searching)
        return;

    delete searching;
    searching = NULL;
}

void MusicBrowserUI::WizardClose(void)
{
    if (!wiz)
        return;

    delete wiz;
    wiz = NULL;
}

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

#include "gtkmusicbrowser.h" 
#include "infoeditor.h"
#include "eventdata.h"

extern "C" {

UserInterface *Initialize(FAContext *context) {
    return new MusicBrowserUI(context);
}
           }

MusicBrowserUI::MusicBrowserUI(FAContext *context)
{
    m_context = context;
    m_initialized = false;
    isVisible = false;
    m_currentindex = 0;
    m_currentListName = "";

    m_state = STATE_COLLAPSED;
    m_plm = NULL;
    m_browserCreated = false;
    statusContext = 0;
    playlistList = NULL;
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

    m_playerEQ = m_context->target;
    m_plm = m_context->plm;

    m_argc = m_context->argc;
    m_argv = m_context->argv;
    m_musicCatalog = NULL;

    gtkThread = Thread::CreateThread();
    gtkThread->Create(MusicBrowserUI::gtkServiceFunction, this);

    return kError_NoErr;
}

int32 MusicBrowserUI::AcceptEvent(Event *event)
{
    switch (event->Type()) {
        case CMD_Cleanup: {

            if (weAreGTK) {
                gdk_threads_enter();
                gtk_main_quit();
                gdk_threads_leave();
            }
            gtkThread->Join();
            SaveCurrentPlaylist(NULL);
            
            string lastPlaylist = FreeampDir(m_context->prefs);
            lastPlaylist += "/currentlist.m3u";

            SaveCurrentPlaylist((char *)lastPlaylist.c_str());  
       
            m_playerEQ->AcceptEvent(new Event(INFO_ReadyToDieUI));

            break; }

        case INFO_SearchMusicDone: {
            if (m_initialized) {
                gdk_threads_enter();
                UpdateCatalog();
                SetStatusText("");
                gdk_threads_leave();
            }
            break; }
        case INFO_BrowserMessage: {
            if (m_initialized) {
                gdk_threads_enter();
                SetStatusText(((BrowserMessageEvent *)event)->GetBrowserMessage());
                gdk_threads_leave();
            }
            break; }
        case CMD_TogglePlaylistUI: {
            gdk_threads_enter();
            if (m_initialized && isVisible) {
                gtk_widget_hide(musicBrowser);
                isVisible = false;
            }
            else {
                isVisible = true;
                if (m_initialized)
                {
                    gtk_widget_show(musicBrowser);
                }
                else 
                {
                    CreatePlaylist();
                    m_initialized = true;
                }
            }
            if (m_state == STATE_EXPANDED)
                ExpandCollapseEvent();
            gdk_threads_leave();
            break; }
            
        case CMD_ToggleMusicBrowserUI: {
            gdk_threads_enter();
            if (m_initialized && isVisible) {
                gtk_widget_hide(musicBrowser);
                isVisible = false;
            }
            else {
                isVisible = true;
                if (m_initialized)
                {
                    gtk_widget_show(musicBrowser);
                }
                else 
                {
                    CreatePlaylist();
                    m_initialized = true;
                }
            }
            if (m_state == STATE_COLLAPSED)
                ExpandCollapseEvent();
            gdk_threads_leave();
            break; }
        default:
            break;
    }
    return 0;
}

void MusicBrowserUI::DeleteListEvent(void)
{
    m_plm->RemoveAll();
    UpdatePlaylistList();
    m_currentindex = kInvalidIndex;
}

void MusicBrowserUI::DeleteEvent(void)
{
    m_plm->RemoveItem(m_currentindex);
    m_currentindex = m_plm->GetCurrentIndex();
    UpdatePlaylistList();
}

void MusicBrowserUI::MoveUpEvent(void)
{
    if (m_currentindex == 0)
        return;

    m_plm->SwapItems(m_currentindex, m_currentindex - 1);
    m_currentindex--;
    UpdatePlaylistList();
}

void MusicBrowserUI::MoveDownEvent(void)
{
    if (m_currentindex == m_plm->CountItems() - 1)
        return;

    m_plm->SwapItems(m_currentindex, m_currentindex + 1);
    m_currentindex++;
    UpdatePlaylistList();
}

void MusicBrowserUI::MoveItemEvent(int source, int dest)
{
    m_plm->MoveItem(source, dest);
    UpdatePlaylistList();
}

void MusicBrowserUI::AddTrackPlaylistEvent(char *path)
{
    char *tempurl = new char[_MAX_PATH];
    uint32 length = _MAX_PATH;

    if (m_currentindex == kInvalidIndex) 
        m_currentindex = 0;

    if (IsntError(FilePathToURL(path, tempurl, &length))) {
        m_plm->AddItem(tempurl, m_currentindex);
        UpdatePlaylistList();
    }
   
    delete [] tempurl;
}

void MusicBrowserUI::AddTrackPlaylistEvent(PlaylistItem *newitem)
{
    if (m_currentindex == kInvalidIndex)
        m_currentindex = 0;

    m_plm->AddItem(newitem, m_currentindex, false);
    UpdatePlaylistList();
}

void MusicBrowserUI::AddTracksPlaylistEvent(vector<PlaylistItem *> *newlist)
{
    if (m_currentindex == kInvalidIndex)
        m_currentindex = 0;

    m_plm->AddItems(newlist, m_currentindex, false);
    UpdatePlaylistList();
}

void MusicBrowserUI::PlayEvent(void)
{
    m_plm->SetCurrentIndex(m_currentindex);
    m_playerEQ->AcceptEvent(new Event(CMD_Play));
}

void MusicBrowserUI::StartMusicSearch(void)
{
    vector<string> oPathList;
    
    oPathList.push_back(string(ROOT_DIR));
    m_context->browser->SearchMusic(oPathList);
}

void MusicBrowserUI::SortPlaylistEvent(PlaylistSortKey order, PlaylistSortType
                                       type)
{
    if (order == kPlaylistSortKey_Random)
        m_plm->SetShuffleMode(true);
    else
        m_plm->Sort(order, type);
    UpdatePlaylistList();
}

void MusicBrowserUI::PopUpInfoEditor(void)
{
    if (m_currentindex == kInvalidIndex)
        return;

    infoeditorUI *infoedit = new infoeditorUI(m_context, 
                                              m_plm->ItemAt(m_currentindex));
    
    infoedit->DisplayInfo();
}

void MusicBrowserUI::SaveCurrentPlaylist(char *path)
{
    if (path)
        m_currentListName = path;

    if (m_currentListName.length() == 0)
        return;

    char *ext = strrchr(m_currentListName.c_str(), '.');
    if (ext)
        ext = ext + 1;
    Error result = kError_NoErr;
    int i = 0;
    bool found = false;

    PlaylistFormatInfo format;
    while (ext && result == kError_NoErr) {
        result = m_plm->GetSupportedPlaylistFormats(&format, i);
        if (!strcmp(ext, format.GetExtension())) {
            found = true;
            break;
        }
        i++;
    }
    if (!found) {
        m_plm->GetSupportedPlaylistFormats(&format, 0);
        m_currentListName += "." ;
        m_currentListName += format.GetExtension();
    }

    uint32 urlLength = m_currentListName.length() + 20;
    char *writeURL = new char[urlLength];

    Error err = FilePathToURL(m_currentListName.c_str(), writeURL, &urlLength);
    if (IsntError(err))
        m_plm->WritePlaylist(writeURL, &format);

    delete [] writeURL;
}

void MusicBrowserUI::ImportPlaylist(char *path)
{
    if (!path)
        return;

    uint32 length = strlen(path) + 10;
    char *url = new char[length];

    if (IsntError(FilePathToURL(path, url, &length)))
        m_context->browser->m_catalog->AddPlaylist(path);

    delete [] url;
    UpdateCatalog();
}

void MusicBrowserUI::ReadPlaylist(char *path, vector<PlaylistItem *> *plist)
{
    m_plm->ReadPlaylist(path, plist);
}


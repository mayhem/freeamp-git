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

// The debugger can't handle symbols more than 255 characters long.
// STL often creates symbols longer than that.
// When symbols are longer than 255 characters, the warning is disabled.
#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <windows.h>
#include <windowsx.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>

#include <algorithm>

using namespace std;

#include "config.h"
#include "utility.h"
#include "resource.h"
#include "Win32MusicBrowser.h"
#include "debug.h"
#include "eventdata.h"

HINSTANCE g_hinst = NULL;
const int iSpacer = 15;

#define DB Debug_v("%d", __LINE__);


bool operator<(const TreeData &A, const TreeData &b)
{
    assert(0);
    return 0;
}
bool operator==(const TreeData &A, const TreeData &b)
{
    assert(0);
    return 0;
}

INT WINAPI DllMain (HINSTANCE hInstance,
                    ULONG ul_reason_being_called,
                    LPVOID lpReserved)
{
	switch(ul_reason_being_called)
	{
		case DLL_PROCESS_ATTACH:
			g_hinst = hInstance;
	    	break;

		case DLL_THREAD_ATTACH:
		    break;

		case DLL_THREAD_DETACH:
		    break;

		case DLL_PROCESS_DETACH:
		    break;
	}

    return 1;                 
}

extern "C" {

UserInterface *Initialize(FAContext *context) {
    return new MusicBrowserUI(context, NULL, NULL, string(""));
}
           }

MusicBrowserUI::MusicBrowserUI(FAContext      *context, 
                               MusicBrowserUI *parent,
                               HWND            hParent,
                               DeviceInfo     *pDevice)
{
    m_context = context;
    m_pParent = parent;
    m_hParent = hParent;

    Init();

    if (m_pParent == NULL)
        m_plm = m_context->plm;
    else
    {
        m_plm = new PlaylistManager(m_context);  
        m_portableDevice = pDevice;

        string displayString;

        displayString = pDevice->GetManufacturer();
        displayString += " ";
        displayString += pDevice->GetDevice();

        if(displayString.size())
        {
            m_currentListName = displayString;
        }
        else
        {
           m_currentListName = pDevice->GetPluginName();
        }
    }    
}

MusicBrowserUI::MusicBrowserUI(FAContext      *context, 
                               MusicBrowserUI *parent,
                               HWND            hParent,
                               const string   &oPlaylistName)
{
    m_context = context;
    m_pParent = parent;
    m_hParent = hParent;

    Init();

    if(m_pParent == NULL)
    {
       m_plm = m_context->plm;

       if(!m_plm->CountItems())
       {
            bool savePlaylist = true;
            uint32 index = 0;

            m_context->prefs->GetSaveCurrentPlaylistOnExit(&savePlaylist);
            m_context->prefs->GetSavedPlaylistPosition(&index);

            if(savePlaylist)
            {
                char path[MAX_PATH];
                char url[MAX_PATH + 7];
                uint32 length = sizeof(path);

                m_context->prefs->GetInstallDirectory(path, &length);

                strcat(path, "\\freeamp.m3u");

                length = sizeof(url);
                FilePathToURL(path, url, &length);

                vector<PlaylistItem*> items;

                m_plm->ReadPlaylist(url, &items);

                m_initialCount = items.size();
                m_autoPlayHack = true;

                if(m_initialCount)
                {
                    m_plm->AddItems(&items);
                    m_plm->SetCurrentIndex(index);
                }
            }           
       }
    }
    else
    {
       m_plm = new PlaylistManager(m_context);   
       m_currentListName = oPlaylistName;
    }    
}

void MusicBrowserUI::Init()
{
    m_initialized = false;
    isVisible = false;
    m_currentListName = "";
    m_state = STATE_EXPANDED;
    m_hWnd = NULL;    
    m_sMinSize.x = -1;
    m_bListChanged = false;
    m_bSearchInProgress = false;
    m_currentplaying = -1;

    m_bDragging = false;
    m_uiThread = NULL;
    m_overSplitter = false;
    m_trackSplitter = false;

    m_plm = NULL;
    m_portableDevice = NULL;

    short pattern[8];
    HBITMAP bmp;

	for (int32 i = 0; i < 8; i++)
		pattern[i] = (WORD)(0x5555 << (i & 1));

	bmp = CreateBitmap(8, 8, 1, 1, &pattern);

    m_splitterBrush = CreatePatternBrush(bmp);
    DeleteObject(bmp);

    m_hSplitterCursor = LoadCursor(g_hinst, MAKEINTRESOURCE(IDC_SPLITTER));
    m_hPointerCursor = LoadCursor(NULL, IDC_ARROW);
    m_hCurrentCursor = m_hPointerCursor;

    m_playerState = PLAYERSTATE_STOPPED;
    m_initialCount = 0;
    m_itemsAddedBeforeWeWereCreated = 0;

    m_hMusicView = NULL;
    m_hPlaylistView = NULL;

    m_hMyMusicItem = NULL;
    m_hAllItem = NULL;
    m_hUncatItem = NULL; 
    m_hPlaylistItem = NULL; 
    m_hNewPlaylistItem = NULL;
    m_hStreamsItem = NULL;
    m_hCDItem = NULL;
    
    m_hMusicView = NULL;
    m_hPortableItem = NULL;
    m_hNewPortableItem = NULL;

    m_autoPlayHack = false;

    m_cdId = 0;

    if(m_pParent == NULL)
        m_cdTracks = new vector<PlaylistItem*>;
    else
        m_cdTracks = NULL;
}

MusicBrowserUI::~MusicBrowserUI()
{
    DeleteObject(m_splitterBrush);
    DeleteObject(m_hSplitterCursor);
    DeleteObject(m_hPointerCursor);

    if (m_pParent)
    {
        delete m_plm;
        if(m_cdTracks)
            delete m_cdTracks;
    }
    else
    {
       vector<MusicBrowserUI *>::iterator i;
       
       for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
          delete (*i);
    }   
    
    if(!m_pParent)
        m_context->prefs->SetViewMusicBrowser(m_state == STATE_EXPANDED);

    CloseMainDialog();

    delete m_uiThread;
}

// These two functions should never get called on non-parent music browsers
void MusicBrowserUI::AddMusicBrowserWindow(MusicBrowserUI *pWindow)
{
    assert(m_pParent == NULL);
    m_oWindowList.push_back(pWindow);
}

void MusicBrowserUI::RemoveMusicBrowserWindow(MusicBrowserUI *pWindow)
{
    vector<MusicBrowserUI *>::iterator i;

    assert(m_pParent == NULL);
    
    i = find(m_oWindowList.begin(), m_oWindowList.end(), pWindow);
    if (i != m_oWindowList.end())
        m_oWindowList.erase(i);
}

Error MusicBrowserUI::Init(int32 startup_level) 
{
    if ((m_startupType = startup_level) != SECONDARY_UI_STARTUP) {
        cout << "The musicbrowser cannot run as the primary ui\n"; 
        return kError_InitFailedSafely;
    }

    m_playerEQ = m_context->target;
    m_uiThread = Thread::CreateThread();
    m_uiThread->Create(MusicBrowserUI::UIThreadFunc, this);

    return kError_NoErr;
}


void MusicBrowserUI::MusicSearchDone()
{
    HMENU        hMenu;
    MENUITEMINFO sItem;
    
    SendMessage(m_hStatus, SB_SETTEXT, 0, (LPARAM)"Music search completed.");

    hMenu = GetMenu(m_hWnd);
    hMenu = GetSubMenu(hMenu, 0);
    sItem.cbSize = sizeof(MENUITEMINFO);
    sItem.fMask = MIIM_TYPE;
    sItem.fType = MFT_STRING;
    sItem.dwTypeData = "Search Computer for &Music...";
    sItem.cch = strlen(sItem.dwTypeData);
    SetMenuItemInfo(hMenu, ID_FILE_SEARCHFORMUSIC, false, &sItem);
                    
    m_bSearchInProgress = false;
                
    TreeView_Expand(m_hMusicView,m_hPlaylistItem, TVE_EXPAND);
    TreeView_Expand(m_hMusicView,m_hMyMusicItem, TVE_EXPAND);
}

void MusicBrowserUI::DisplayBrowserMessage(const char* msg)
{
    SendMessage(m_hStatus, SB_SETTEXT, 0, (LPARAM)msg);
}

void MusicBrowserUI::SaveCurrentPlaylist()
{
    bool savePlaylist = true;

    m_context->prefs->GetSaveCurrentPlaylistOnExit(&savePlaylist);

    if(savePlaylist)
    {
        char path[MAX_PATH];
        char url[MAX_PATH + 7];
        uint32 length = sizeof(path);

        m_context->prefs->GetInstallDirectory(path, &length);

        strcat(path, "\\freeamp.m3u");

        length = sizeof(url);
        FilePathToURL(path, url, &length);

        m_plm->WritePlaylist(url);
        m_context->prefs->SetSavedPlaylistPosition(m_plm->GetCurrentIndex());
    }
}

Error MusicBrowserUI::AcceptEvent(Event *event)
{
    switch(event->Type()) 
    {
        case INFO_CDDiscStatus:
        {
            CDInfoEvent* cie = (CDInfoEvent*)event;
            uint32 numTracks = cie->GetNumTracks();

            if(cie->GetCDDB() != m_cdId)
            {
                m_cdId = cie->GetCDDB();

                m_cdTracks->erase(m_cdTracks->begin(), m_cdTracks->end()); 

                for(uint32 track = 1; track <= numTracks; track++)
                {
                    char url[40];

                    sprintf(url, "file://%d.cda", track);

                    PlaylistItem* item = new PlaylistItem(url);
                
                    m_cdTracks->push_back(item);
                }

                if(m_cdTracks->size() > 0) 
                {
                    vector<PlaylistItem*>* metalist =
                                       new vector<PlaylistItem*>(m_cdTracks->size());

                    copy(m_cdTracks->begin(), m_cdTracks->end(), metalist->begin());
                    m_plm->RetrieveMetaData(metalist);
                }

                vector<MusicBrowserUI *>::iterator i;

                for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
                {
                    (*i)->RefreshCDList(m_cdTracks);
                }

                RefreshCDList(m_cdTracks);
            }

            break;
        }

        case INFO_PrefsChanged:
        {
            bool useTextLabels, useImages;
            m_context->prefs->GetShowToolbarTextLabels(&useTextLabels);
            m_context->prefs->GetShowToolbarImages(&useImages);

            vector<MusicBrowserUI *>::iterator i;

            for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
            {
                (*i)->AddToolbarButtons(useTextLabels, useImages);
                (*i)->UpdateButtonStates();
            }

            AddToolbarButtons(useTextLabels, useImages);
            UpdateButtonStates();

            if(TreeView_GetChild(m_hMusicView, m_hPortableItem) != NULL)
            {    
                FillPortables();
            }
            break;
        }

        case CMD_Cleanup: 
        {
            SaveCurrentPlaylist();

            CloseMainDialog();
            m_uiThread->Join();
            
            m_playerEQ->AcceptEvent(new Event(INFO_ReadyToDieUI));
            break; 
        }

        case INFO_MusicCatalogCleared:
        {
            vector<MusicBrowserUI *>::iterator i;

            for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
            {
                (*i)->MusicCatalogCleared();
            }

            MusicCatalogCleared();

            break;
        }

        case INFO_MusicCatalogTrackChanged:
        {
            MusicCatalogTrackChangedEvent* pie = (MusicCatalogTrackChangedEvent*)event;

            vector<MusicBrowserUI *>::iterator i;

            for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
            {
                (*i)->MusicCatalogTrackChanged(pie->OldArtist(), 
                                               pie->NewArtist(), 
                                               pie->OldAlbum(), 
                                               pie->NewAlbum(), 
                                               pie->OldItem(),
                                               pie->NewItem());
            }

            MusicCatalogTrackChanged(pie->OldArtist(), 
                                     pie->NewArtist(), 
                                     pie->OldAlbum(), 
                                     pie->NewAlbum(), 
                                     pie->OldItem(),
                                     pie->NewItem());
            
            break; 
        }

        case INFO_MusicCatalogTrackAdded:
        {
            MusicCatalogTrackAddedEvent* pie = (MusicCatalogTrackAddedEvent*)event;

            vector<MusicBrowserUI *>::iterator i;

            for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
            {
                (*i)->MusicCatalogTrackAdded(pie->Artist(), pie->Album(), pie->Item());
            }

            MusicCatalogTrackAdded(pie->Artist(), pie->Album(), pie->Item());
            
            break; 
        }

        case INFO_MusicCatalogTrackRemoved:
        {
            MusicCatalogTrackRemovedEvent* pie = (MusicCatalogTrackRemovedEvent*)event;

            vector<MusicBrowserUI *>::iterator i;

            for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
            {
                (*i)->MusicCatalogTrackRemoved(pie->Artist(), pie->Album(), pie->Item());
            }

            MusicCatalogTrackRemoved(pie->Artist(), pie->Album(), pie->Item());
            
            break; 
        }

        case INFO_MusicCatalogPlaylistAdded:
        {
            MusicCatalogPlaylistAddedEvent* pie = (MusicCatalogPlaylistAddedEvent*)event;
            
            vector<MusicBrowserUI *>::iterator i;

            for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
            {
                (*i)->MusicCatalogPlaylistAdded(pie->Item());
            }

            MusicCatalogPlaylistAdded(pie->Item());
            
            break; 
        }

        case INFO_MusicCatalogPlaylistRemoved:
        {
            MusicCatalogPlaylistRemovedEvent* pie = (MusicCatalogPlaylistRemovedEvent*)event;
            
            vector<MusicBrowserUI *>::iterator i;

            for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
            {
                (*i)->MusicCatalogPlaylistRemoved(pie->Item());
            }

            MusicCatalogPlaylistRemoved(pie->Item());
            
            break; 
        }

        case INFO_PlaylistItemsAdded:
        {
            PlaylistItemsAddedEvent* pie = (PlaylistItemsAddedEvent*)event;

            if(pie->Manager() == m_plm)
                PlaylistListItemsAdded(pie->Items());
            else
            {
                vector<MusicBrowserUI *>::iterator i;

                for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
                {
                    if((*i)->PLManager() == pie->Manager())
                    {
                        (*i)->PlaylistListItemsAdded(pie->Items());
                        break;
                    }
                }
            }
            
            break; 
        }

        case INFO_PlaylistItemAdded:
        {
            PlaylistItemAddedEvent* pie = (PlaylistItemAddedEvent*)event;

            if(pie->Manager() == m_plm)
                PlaylistListItemAdded(pie->Item());
            else
            {
                vector<MusicBrowserUI *>::iterator i;

                for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
                {
                    if((*i)->PLManager() == pie->Manager())
                    {
                        (*i)->PlaylistListItemAdded(pie->Item());
                        break;
                    }
                }
            }
            
            break; 
        }

        case INFO_PlaylistItemUpdated:
        {
            PlaylistItemUpdatedEvent* pie = (PlaylistItemUpdatedEvent*)event;

            if(pie->Manager() == m_plm)
                PlaylistListItemUpdated(pie->Item());
            else
            {
                vector<MusicBrowserUI *>::iterator i;

                for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
                {
                    if((*i)->PLManager() == pie->Manager())
                    {
                        (*i)->PlaylistListItemUpdated(pie->Item());
                        break;
                    }
                }
            }
            
            break; 
        }

        case INFO_PlaylistItemMoved:
        {
            PlaylistItemMovedEvent* pie = (PlaylistItemMovedEvent*)event;

            if(pie->Manager() == m_plm)
                PlaylistListItemMoved(pie->Item(), pie->OldIndex(), pie->NewIndex());
            else
            {
                vector<MusicBrowserUI *>::iterator i;

                for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
                {
                    if((*i)->PLManager() == pie->Manager())
                    {
                        (*i)->PlaylistListItemMoved(pie->Item(), pie->OldIndex(), pie->NewIndex());
                        break;
                    }
                }
            }
          
            break; 
        }

        case INFO_PlaylistItemRemoved:
        {
            PlaylistItemRemovedEvent* pie = (PlaylistItemRemovedEvent*)event;

            if(pie->Manager() == m_plm)
                PlaylistListItemRemoved(pie->Items(), pie->Indices());
            else
            {
                vector<MusicBrowserUI *>::iterator i;

                for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
                {
                    if((*i)->PLManager() == pie->Manager())
                    {
                        (*i)->PlaylistListItemRemoved(pie->Items(), pie->Indices());
                        break;
                    }
                }
            }
          
            break; 
        }
        
        case INFO_PlaylistSorted:
        {
            PlaylistSortedEvent* pie = (PlaylistSortedEvent*)event;

            if(pie->Manager() == m_plm)
                PlaylistListSorted();
            else
            {
                vector<MusicBrowserUI *>::iterator i;

                for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
                {
                    if((*i)->PLManager() == pie->Manager())
                    {
                        (*i)->PlaylistListSorted();
                        break;
                    }
                }
            }
          
            break; 
        }

        case INFO_PlaylistCurrentItemInfo:
        {
            ListView_RedrawItems(m_hPlaylistView, 
                                 m_currentplaying,
                                 m_currentplaying);

            m_currentplaying = m_plm->GetCurrentIndex();

            ListView_RedrawItems(m_hPlaylistView, 
                                 m_currentplaying,
                                 m_currentplaying);
            break; 
        }

        case INFO_SearchMusicDone: 
        {
            vector<MusicBrowserUI *>::iterator i;

            for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
            {
                (*i)->MusicSearchDone();
            }

            MusicSearchDone();
            break; 
        }

        case INFO_BrowserMessage: 
        {
            const char* cancelMsg = "(Press ESC to Cancel)  ";
            string message;

            if(m_bSearchInProgress)
            {
                message = cancelMsg;
            }

            message += ((BrowserMessageEvent *)event)->GetBrowserMessage();

            vector<MusicBrowserUI *>::iterator i;

            for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
            {
                (*i)->DisplayBrowserMessage(message.c_str());
            }

            DisplayBrowserMessage(message.c_str());

            break; 
        }

        case CMD_TogglePlaylistUI: 
        {
            if(m_initialized && isVisible)
            {
                isVisible = false;
            }
            else 
            {
                isVisible = true;

                if(m_initialized)
                {
					ShowBrowser(true);
                }
            }

            if (m_state == STATE_EXPANDED)
                ExpandCollapseEvent();

            break; 
        }

        case CMD_ToggleMusicBrowserUI: 
        {
            ShowBrowser(true);
            break; 
        }

        case INFO_PlaylistRepeat:
		{
            PlaylistRepeatEvent* plre = (PlaylistRepeatEvent*)event;

            vector<MusicBrowserUI *>::iterator i;

            for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
            {
                (*i)->ChangeRepeatMode(plre->GetRepeatMode());
            }

            ChangeRepeatMode(plre->GetRepeatMode());
			break;
		}

		case INFO_PlaylistShuffle:
		{
            PlaylistShuffleEvent* plse = (PlaylistShuffleEvent*)event;            

            vector<MusicBrowserUI *>::iterator i;

            for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
            {
                (*i)->ChangeShuffleMode(plse->GetShuffleMode());
            }

            ChangeShuffleMode(plse->GetShuffleMode());
            
			break;
		}

        case INFO_Playing: 
        case INFO_Paused: 
        case INFO_Stopped: 
        {   
            vector<MusicBrowserUI *>::iterator i;

            for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
            {
                (*i)->ChangePlayerState(event->Type());
            }

            ChangePlayerState(event->Type());
                        
	        break; 
        }

        case CMD_AddFiles:
        {
            Int32PropValue *prop;

            if(IsntError(m_context->props->GetProperty("MainWindow", (PropValue **)&prop)))
            {
                HWND hwnd = (HWND)prop->GetInt32();
    
                AddFileEvent(hwnd);
            }
            
            break;
        }

        default:
            break;
    }
    return kError_NoErr;
}




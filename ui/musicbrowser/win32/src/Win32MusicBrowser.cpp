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

#include <algorithm>
#include <windows.h>
#include <windowsx.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>

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
	switch (ul_reason_being_called)
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
                               const string   &oPlaylistName)
{
    m_context = context;
    m_initialized = false;
    isVisible = false;
    m_currentindex = 0;
    m_currentListName = "";
    m_state = STATE_EXPANDED;
    m_hWnd = NULL;    
    m_sMinSize.x = -1;
    m_bListChanged = false;
    m_bSearchInProgress = false;
    m_currentplaying = -1;
    m_bDragging = false;
    m_pParent = parent;
    m_hParent = hParent;
    m_uiThread = NULL;
    if (parent == NULL)
       m_oPlm = m_context->plm;
    else
    {
       m_oPlm = new PlaylistManager(context);   
       m_currentListName = oPlaylistName;
    }   

    m_overSplitter = false;
    m_trackSplitter = false;

    short pattern[8];
    HBITMAP bmp;

	for (int32 i = 0; i < 8; i++)
		pattern[i] = (WORD)(0x5555 << (i & 1));

	bmp = CreateBitmap(8, 8, 1, 1, &pattern);

    m_splitterBrush = CreatePatternBrush(bmp);
    DeleteObject(bmp);
}

MusicBrowserUI::~MusicBrowserUI()
{
    DeleteObject(m_splitterBrush);

    if (m_pParent)
    {
       delete m_oPlm;
    }
    else
    {
       vector<MusicBrowserUI *>::iterator i;
       
       for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
          delete (*i);
    }   
    
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
    m_initialized = true;
    m_uiThread = Thread::CreateThread();
    m_uiThread->Create(MusicBrowserUI::UIThreadFunc, this);

    return kError_NoErr;
}


int32 MusicBrowserUI::AcceptEvent(Event *event)
{
    switch (event->Type()) 
    {
        case CMD_Cleanup: 
        {
            CloseMainDialog();
            m_uiThread->Join();
            
            m_playerEQ->AcceptEvent(new Event(INFO_ReadyToDieUI));
            break; 
        }

        case INFO_PlaylistItemUpdated:
        {
            vector<MusicBrowserUI *>::iterator i;
            
            for(i = m_oWindowList.begin(); i != m_oWindowList.end(); i++)
               (*i)->UpdatePlaylistList();
               
            UpdatePlaylistList();
            break; 
        }

        case INFO_PlaylistCurrentItemInfo:
        {
            m_currentplaying = m_oPlm->GetCurrentIndex();
            UpdatePlaylistList();
            break; 
        }

        case INFO_SearchMusicDone: 
        {
            HMENU        hMenu;
            MENUITEMINFO sItem;
            
            if (m_bSearchInProgress)
                SendMessage(m_hStatus, SB_SETTEXT, 0, 
                            (LPARAM)"Music search completed.");
            else                
                SendMessage(m_hStatus, SB_SETTEXT, 0, 
                            (LPARAM)"Music search interrupted -- your database may be incomplete.");

            hMenu = GetMenu(m_hWnd);
            hMenu = GetSubMenu(hMenu, 0);
            sItem.cbSize = sizeof(MENUITEMINFO);
            sItem.fMask = MIIM_TYPE;
            sItem.fType = MFT_STRING;
            sItem.dwTypeData = "Search for &Music...";
            sItem.cch = strlen(sItem.dwTypeData);
            SetMenuItemInfo(hMenu, ID_FILE_SEARCHFORMUSIC, false, &sItem);
                            
            SetWindowText(GetDlgItem(m_hWnd, IDC_SEARCH), "Music Search");
            m_bSearchInProgress = false;
                        
            InitTree();
            TreeView_Expand(GetDlgItem(m_hWnd, IDC_MUSICTREE), 
                            m_hPlaylistItem, TVE_EXPAND);
            TreeView_Expand(GetDlgItem(m_hWnd, IDC_MUSICTREE), 
                            m_hCatalogItem, TVE_EXPAND);
            
            break; 
        }
        case INFO_BrowserMessage: 
        {
            SendMessage(m_hStatus, SB_SETTEXT, 0, (LPARAM)
                          ((BrowserMessageEvent *)event)->GetBrowserMessage());
            break; 
        }
        case CMD_TogglePlaylistUI: 
        {
            if (m_initialized && isVisible) 
            {
                isVisible = false;
            }
            else 
            {
                isVisible = true;
                if (m_initialized)
                {
					ShowBrowser(true);
                }
                else 
                {
                    m_initialized = true;
                }
            }
            if (m_state == STATE_EXPANDED)
                ExpandCollapseEvent();
            break; 
        }
        case CMD_ToggleMusicBrowserUI: 
        {
            if (m_initialized && isVisible) 
            {
				HideBrowser();
                isVisible = false;
            }
            else 
            {
                isVisible = true;
                if (m_initialized)
                {
					ShowBrowser(true);
                }
                else 
                {
                    m_initialized = true;
                }
            }
            if (m_state == STATE_COLLAPSED)
                ExpandCollapseEvent();
            break; 
        }

        default:
            break;
    }
    return 0;
}




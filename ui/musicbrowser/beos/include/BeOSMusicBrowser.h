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

#ifndef INCLUDED_BEOSMUSICBROWSER_H__
#define INCLUDED_BEOSMUSICBROWSER_H__

#include "config.h"
#include <be/interface/Window.h>
#include <vector>
#include <string>

using namespace std;

// Message codes
enum {
    // These are simply BMessage wrappers for the similarly named
    // FreeAmp events (those dispatched to/from AcceptEvent())
    MBMSG_PLAYLIST_CURRENT_ITEM_INFO = 'plci',
    MBMSG_PLAYLIST_ITEM_UPDATED = 'piud',
    MBMSG_PLAYLIST_SORTED = 'plsd',
    MBMSG_PLAYLIST_UPDATED = 'plud',
    MBMSG_SEARCH_MUSIC_DONE = 'smdn',
    MBMSG_MUSIC_CATALOG_PLAYLIST_ADDED = 'mcla',
    MBMSG_MUSIC_CATALOG_PLAYLIST_REMOVED = 'mclr',

    // Other messages.
    MBMSG_ADD_FILES = 'addf',
    MBMSG_ADD_FILES_REPLY = 'adfr',
    MBMSG_CREATE_NEW_PLAYLIST = 'newp',
    MBMSG_SAVE_CURRENT_PLAYLIST = 'scpl',
    MBMSG_SAVE_CURRENT_PLAYLIST_AS = 'scpa',
    MBMSG_SAVE_CURRENT_PLAYLIST_REPLY = 'scpr',
    MBMSG_SEARCH_DIALOG = 'sdlg',
    MBMSG_INTRO_WIZARD = 'intr',
    MBMSG_UPDATE_MUSIC_CATALOG_VIEW = 'umcv',
    MBMSG_UPDATE_PLAYLIST_VIEW = 'uplv',
    MBMSG_MUSIC_TREE_INVOKED = 'mtiv',
    MBMSG_PLAYLIST_VIEW_INVOKED = 'pliv',
    MBMSG_EDIT_INFO = 'edif',
    MBMSG_IMPORT_ITEMS = 'impt',
    MBMSG_REMOVE_ITEMS = 'rmim',
    MBMSG_MOVE_UP = 'upup',
    MBMSG_MOVE_DOWN = 'down',
    MBMSG_SORT_PLAYLIST = 'sort',
    MBMSG_TOOLTIP_MESSAGE = 'tips',
    MBMSG_WIZARD_GONE = 'wzgn',
    MBMSG_STOP_SEARCH = 'stps',
    MBMSG_EDIT_CATALOG_ITEM = 'edci',

    // For debug.
    MBMSG_DEBUG_1 = 'dbg1',
    MBMSG_DEBUG_2 = 'dbg2',
    MBMSG_DEBUG_3 = 'dbg3',
};

class FAContext;
class PlaylistManager;
class MusicBrowserUI;
class ToolBar;
class MusicTreeView;
class PlaylistView;
class Placard;
class CatalogItem;
class CollectionItem;
class IntroductionWizard;
class BMenuBar;

class BeOSMusicBrowser : public BWindow
{
public:
                        BeOSMusicBrowser( FAContext* context,
                                          MusicBrowserUI* ui,
                                          const string& playlistURL,
                                          bool master );
    virtual             ~BeOSMusicBrowser();
    virtual void        MenusBeginning( void );
    virtual void        MessageReceived( BMessage* message );
    virtual void        Quit( void );
    virtual bool        QuitRequested( void );

    void                LoadPlaylist( const string& playlist );
    void                SaveCurrentPlaylist( const char* path );
    void                UpdateCatalogView( void );
    void                UpdateCatalogPlaylistGroup( void );
    void                UpdatePlaylistView( void );
    void                MovePlaylistItem( uint32 oldIndex, uint32 newIndex );

protected:
    void                ClearPlaylistView( void );
    void                CatalogItemInvoked( CatalogItem* item );
    uint32              GetURLsUnder( CollectionItem* item,
                                      vector<string>* urls );
    void                SaveCurrentPlaylistPanel( BMessage* message,
                                                  bool reply );
    void                EditCatalogItem( CatalogItem* item );

    void                BuildMenu( BMenuBar* menuBar );
    void                BuildToolBar( ToolBar* toolBar );

private:
    void                InitViews( void );

    FAContext*          m_context;
    PlaylistManager*    m_plm;
    MusicBrowserUI*     m_ui;
    bool                m_master;
    MusicTreeView*      m_musicTreeView;
    PlaylistView*       m_playlistView;
    Placard*            m_placard;
    IntroductionWizard* m_wizard;
    string              m_currentListName;

    BMenuItem*          m_searchControl;
};

#endif // INCLUDED_BEOSMUSICBROWSER_H__

/*____________________________________________________________________________

        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 1999, 2000 EMusic.com

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

#ifndef INCLUDED_MUSICBROWSERUI_H__
#define INCLUDED_MUSICBROWSERUI_H__

#include "config.h"
#include "ui.h"
#include "mutex.h"

#include <string>
#include <vector>

using namespace std;

class FAContext;
class EventQueue;
class PlaylistManager;
class BeOSMusicBrowser;

class MusicBrowserUI : public UserInterface
{
public:
                        MusicBrowserUI( FAContext* context );
    virtual             ~MusicBrowserUI();
    virtual Error       AcceptEvent( Event* event );
    virtual Error       Init( int32 startup_level );

    BeOSMusicBrowser*   MainBrowser( void ) const { return m_mainBrowser; }
    void                ToggleBrowser( void );
    void                CreateNewEditor( const string& playlistURL );
    void                AddFiles( const vector<string>& files );
    void                AddTrackToPlaylist( const char* url );
    void                AddTracksToPlaylist( const vector<string>& urls );
    void                StartSearch( const char* searchPath );
    void                SaveCurrentPlaylist( const char* path );
    void                ShowMessage( const char* text );
    bool                CheckEmptyDatabase( void );
    bool                IsSearching( void ) const { return m_searching; }

protected:
    void                BroadcastToAllBrowsers( BMessage* message );
    void                BroadcastToAllBrowsers( uint32 what,
                                                Event* event = NULL );
    void                BroadcastToSlaveBrowsers( BMessage* message );

private:
    friend class BeOSMusicBrowser;
    void                EditorDone( BeOSMusicBrowser* editor );

    FAContext*          m_context;
    EventQueue*         m_playerEQ;
    PlaylistManager*    m_plm;
    BeOSMusicBrowser*   m_mainBrowser;
    vector<BeOSMusicBrowser*>
                        m_slaveBrowsers;
    Mutex               m_lock;
    bool                m_searching;
};

#endif // INCLUDED_MUSICBROWSERUI_H__

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

#define DEBUG 1

#include "MusicBrowserUI.h"
#include "BeOSMusicBrowser.h"
#include "eventdata.h"
#include "player.h"
#include "utility.h"
#include <be/app/Message.h>
#include <be/support/Debug.h>

extern "C" {

UserInterface* Initialize( FAContext* context )
{
    return new MusicBrowserUI( context );
}

}

MusicBrowserUI::MusicBrowserUI( FAContext* context )
:   m_context( context ),
    m_playerEQ( context->target ),
    m_plm( context->plm ),
    m_mainBrowser( NULL ),
    m_searching( false )
{
}

MusicBrowserUI::~MusicBrowserUI()
{
}

Error
MusicBrowserUI::Init( int32 startup_level )
{
    // player not ready until init'ed?
    m_playerEQ = m_context->target;

    return kError_NoErr;
}

Error
MusicBrowserUI::AcceptEvent( Event* event )
{
    switch ( event->Type() )
    {
        case CMD_TogglePlaylistUI:
            break;
        case CMD_ToggleMusicBrowserUI:
            ToggleBrowser();
            break;
        case CMD_Cleanup:
        {
            if ( m_mainBrowser )
            {
                m_mainBrowser->Lock();
                m_mainBrowser->Quit();
            }
            vector<BeOSMusicBrowser*>::iterator i;
            for ( i = m_slaveBrowsers.begin(); i != m_slaveBrowsers.end(); i++ )
            {
                (*i)->Lock();
                (*i)->Quit();
            }
            m_playerEQ->AcceptEvent( new Event( INFO_ReadyToDieUI ) );
            break;
        }
        case CMD_AddFiles:
            if ( !m_mainBrowser ) ToggleBrowser();
            m_mainBrowser->PostMessage( MBMSG_ADD_FILES );
            break;
        case INFO_SearchMusicDone:
            m_lock.Acquire();
            m_searching = false;
            puts( "music search done" );
            m_mainBrowser->PostMessage( MBMSG_SEARCH_MUSIC_DONE );
            ShowMessage( "Music search done." );
            m_lock.Release();
            break;
        case INFO_BrowserMessage:
        {
            const char* str
                = ((BrowserMessageEvent*)event)->GetBrowserMessage();
            PRINT(( "BrowserMessage: %s\n", str ));
            ShowMessage( str );
            break;
        }
        case INFO_PlaylistCurrentItemInfo:
            printf( "current index = %ld\n", m_plm->GetCurrentIndex() );
            {
                BMessage msg( MBMSG_PLAYLIST_CURRENT_ITEM_INFO );
                msg.AddInt32( "index", m_plm->GetCurrentIndex() );
                m_mainBrowser->PostMessage( &msg );
            }
            break;
        case INFO_PlaylistItemUpdated:
            PRINT(( "INFO_PlaylistItemUpdated\n" ));
            BroadcastToAllBrowsers( MBMSG_PLAYLIST_ITEM_UPDATED, event );
            break;
        case INFO_PlaylistUpdated: // not used?
            PRINT(( "INFO_PlaylistUpdated\n" ));
            BroadcastToAllBrowsers( MBMSG_PLAYLIST_UPDATED, event );
            break;
        case INFO_PlaylistItemsAdded:
            PRINT(( "INFO_PlaylistItemsAdded\n" ));
            break;
        case INFO_PlaylistItemMoved:
            PRINT(( "INFO_PlaylistItemMoved\n" ));
            m_mainBrowser->PostMessage( MBMSG_UPDATE_PLAYLIST_VIEW );
            break;
        case INFO_MusicCatalogPlaylistAdded:
            PRINT(( "INFO_MusicCatalogPlaylistAdded/Removed\n" ));
            BroadcastToAllBrowsers( MBMSG_MUSIC_CATALOG_PLAYLIST_ADDED, event );
            break;
        case INFO_MusicCatalogPlaylistRemoved:
            PRINT(( "INFO_MusicCatalogPlaylistAdded/Removed\n" ));
            BroadcastToAllBrowsers( MBMSG_MUSIC_CATALOG_PLAYLIST_REMOVED,
                                    event );
            break;
        case INFO_PlaylistSorted:
            PRINT(( "INFO_PlaylistSorted\n" ));
            BroadcastToAllBrowsers( MBMSG_PLAYLIST_SORTED, event );
            break;
        default:
            break;
    }
}

void
MusicBrowserUI::ToggleBrowser( void )
{
    bool flag;

    if ( !m_mainBrowser )
        flag = true;
    else
    {
        m_mainBrowser->Lock();
        flag = m_mainBrowser->IsHidden();
        m_mainBrowser->Unlock();
    }

    if ( flag )
    {
        if ( !m_mainBrowser )
        {
            string url = string( "file://" ) +
                         FreeampDir( NULL ) +
                         string( "/currentlist.m3u" );
            m_mainBrowser = new BeOSMusicBrowser( m_context, this, url, true );
            CheckEmptyDatabase();
        }
        m_mainBrowser->Show();
    }
    else
    {
        if ( m_mainBrowser )
        {
            m_mainBrowser->Hide();
        }
    }
}

void
MusicBrowserUI::CreateNewEditor( const string& playlistURL )
{
    BeOSMusicBrowser* editor = new BeOSMusicBrowser( m_context, this,
                                                     playlistURL, false );
    m_slaveBrowsers.push_back( editor );
    editor->Show();
}

void
MusicBrowserUI::EditorDone( BeOSMusicBrowser* editor )
{
    vector<BeOSMusicBrowser*>::iterator loc;
    loc = find( m_slaveBrowsers.begin(), m_slaveBrowsers.end(), editor );
    if ( loc != m_slaveBrowsers.end() )
    {
        m_slaveBrowsers.erase( loc );
    }
}

void
MusicBrowserUI::AddFiles( const vector<string>& files )
{
    bool playNow = true;
    m_context->prefs->GetPrefBoolean(kPlayImmediatelyPref,  &playNow );

    vector<string>::const_iterator i;
    for ( i = files.begin(); i != files.end(); i++ )
    {
        printf( "adding %s\n", (*i).c_str() );
        char* ext = m_context->player->GetExtension( (*i).c_str() );
        if ( ext )
        {
            uint32 length = (*i).size() + 10;
            char* tempurl = new char[length];
            if ( IsntError( FilePathToURL( (*i).c_str(), tempurl, &length ) ) )
            {
                if ( playNow )
                    //DeleteListEvent() //FIXME

                if ( m_plm->IsSupportedPlaylistFormat( ext ) )
                {
                    string tobeloaded = tempurl;
                    //LoadPlaylist( tobeloaded ); //FIXME
                }
                else
                {
                }
            }
        }
    }
}

void
MusicBrowserUI::AddTrackToPlaylist( const char* url )
{
    m_plm->AddItem( url );
}

void
MusicBrowserUI::AddTracksToPlaylist( const vector<string>& urls )
{
    m_plm->AddItems( urls );
}

void
MusicBrowserUI::StartSearch( const char* searchPath )
{
    if ( !m_lock.Acquire() ) return;
    if ( m_searching ) return;

    size_t len = strlen( searchPath );
    if ( searchPath[len - 1] == DIR_MARKER ) len--;

    string tempPath( searchPath, len );
    PRINT(( "MusicBrowserUI::StartSearch in %s\n", tempPath.c_str() ));

    vector<string> pathList;
    pathList.push_back( tempPath );
    m_context->catalog->SearchMusic( pathList );

    m_searching = true;

    m_lock.Release();
}

void
MusicBrowserUI::SaveCurrentPlaylist( const char* path )
{
    PRINT(( "FIXME: SaveCurrentPlaylist, NOT USED, SHOULD GO AWAY\n" ));
}

void
MusicBrowserUI::ShowMessage( const char* text )
{
    BMessage bm( MBMSG_TOOLTIP_MESSAGE );
    bm.AddString( "text", text );
    BroadcastToAllBrowsers( &bm );
}

bool
MusicBrowserUI::CheckEmptyDatabase( void )
{
    if ( m_context->catalog->GetPlaylists()->size() > 0 ||
         m_context->catalog->GetMusicList()->size() > 0 ||
         m_context->catalog->GetUnsortedMusic()->size() > 0 )
    {
        return false;
    }

    bool welcome = false;
    m_context->prefs->GetPrefBoolean( kWelcomePref, &welcome );

    if ( welcome )
    {
        m_mainBrowser->PostMessage( MBMSG_INTRO_WIZARD );
        m_context->prefs->SetPrefBoolean( kWelcomePref, false );
    }

    return true;
}

void
MusicBrowserUI::BroadcastToAllBrowsers( BMessage* message )
{
    m_mainBrowser->PostMessage( message );
    BroadcastToSlaveBrowsers( message );
}

void
MusicBrowserUI::BroadcastToAllBrowsers( uint32 what, Event* event )
{
    BMessage msg( what );
    if ( event )
    {
        // Wraps a FreeAmp event in BMessage.
        msg.AddPointer( "event", event );
    }
    BroadcastToAllBrowsers( &msg );
}

void
MusicBrowserUI::BroadcastToSlaveBrowsers( BMessage* message )
{
    vector<BeOSMusicBrowser*>::iterator i;
    for ( i = m_slaveBrowsers.begin(); i != m_slaveBrowsers.end(); i++ )
    {
        (*i)->PostMessage( message );
    }
}

// vi: set ts=4:

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

#include "BeOSMusicBrowser.h"
#include "MusicBrowserUI.h"
#include "ToolBar.h"
#include "ToolBarButton.h"
#include "Separator.h"
#include "Placard.h"
#include "MusicTreeView.h"
#include "PlaylistView.h"
#include "TooltipFilter.h"
#include "TrackItem.h"
#include "CollectionItem.h"
#include "PlaylistListItem.h"
#include "ResourceManager.h"
#include "MusicBrowserResources.h"
#include "IntroductionWizard.h"
#include "InfoEditor.h"
#include "event.h"
#include "eventdata.h"
#include "musiccatalog.h"
#include "playlist.h"
#include <be/interface/Button.h>
#include <be/interface/ScrollView.h>
#include <be/interface/MenuBar.h>
#include <be/interface/MenuItem.h>
#include <be/interface/Bitmap.h>
#include <be/interface/StringView.h>
#include <be/translation/TranslatorFormats.h>
#include <be/storage/FilePanel.h>
#include <be/storage/Entry.h>
#include <be/storage/Path.h>
#include <string>
#include <vector>
#define DEBUG 1
#include <be/support/Debug.h>

#define DEBUG_MENU 1

const char* MENU_LABEL_START_SEARCH = "Search Computer for Music";
const char* MENU_LABEL_STOP_SEARCH = "Stop Music Search";

BeOSMusicBrowser::BeOSMusicBrowser( FAContext* context, MusicBrowserUI* ui,
                                    const string& playlistURL, bool master )
:   BWindow( BRect( 10, 200, 400, 500 ), "Music Browser",
             B_DOCUMENT_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, 0 ),
    m_context( context ),
    m_plm( NULL ),
    m_ui( ui ),
    m_master( master ),
    m_selectedView( NULL )
{
    if ( m_master )
    {
        PRINT(( "i'm the master.\n" ));
        m_plm = m_context->plm;
    }
    else
    {
        PRINT(( "i'm not the master.\n" ));
        m_plm = new PlaylistManager( m_context );
        LoadPlaylist( playlistURL );
    }

    InitViews();
}

BeOSMusicBrowser::~BeOSMusicBrowser()
{
}

void
BeOSMusicBrowser::InitViews( void )
{
    float separator = Bounds().Width() * 0.4;
    const float TOOL_BAR_HEIGHT = 42;
    BRect rect;
    BStringView* label;
    BView* left;
    BView* right;
    TooltipFilter* tip;

    rect = Bounds();
    BMenuBar* topMenu = new BMenuBar( rect, "TopMenu" );
    AddChild( topMenu );
    BuildMenu( topMenu );

    // Tool bar.
    rect = Bounds();
    rect.top = topMenu->Frame().bottom + 1;
    rect.bottom = rect.top + TOOL_BAR_HEIGHT;
    ToolBar* toolBar = new ToolBar( rect, "ToolBar",
                                    B_FOLLOW_TOP | B_FOLLOW_LEFT_RIGHT );
    AddChild( toolBar );
    BuildToolBar( toolBar );

    // Separator.
    rect = Bounds();
    rect.top = toolBar->Frame().bottom + 1;
    rect.left = separator + 1;
    rect.bottom -= B_H_SCROLL_BAR_HEIGHT - 2;
    Separator* sep = new Separator( rect, "Separator",
                                    B_FOLLOW_LEFT | B_FOLLOW_TOP_BOTTOM );
    sep->ResizeToPreferred();
    AddChild( sep );

    // Static string labels.
    rect = Bounds();
    rect.top = toolBar->Frame().bottom + 1;
    rect.right = separator;
    label = new BStringView( rect, "My Music Collection",
                             "My Music Collection" );
    label->SetViewColor( ui_color( B_PANEL_BACKGROUND_COLOR ) );
    label->SetLowColor( label->ViewColor() );
    font_height fontHeight;
    label->GetFontHeight( &fontHeight );
    label->ResizeTo( rect.Width(),
                     fontHeight.ascent + fontHeight.descent + 2 );
    AddChild( label );
    left = label;

    rect = Bounds();
    rect.top = toolBar->Frame().bottom + 1;
    rect.left = sep->Frame().right + 1;
    rect.bottom = rect.top + fontHeight.ascent + fontHeight.descent + 2;
    label = new BStringView( rect, "PlaylistViewLabel",
                             m_master ? "Currently Listening To"
                                      : "Editting",
                             B_FOLLOW_TOP | B_FOLLOW_LEFT_RIGHT );
    label->SetViewColor( ui_color( B_PANEL_BACKGROUND_COLOR ) );
    label->SetLowColor( label->ViewColor() );
    AddChild( label );
    right = label;

    sep->AddAttachment( left, right );

    // My Music tree
    rect = Bounds();
    //rect.top = toolBar->Frame().bottom + 1;
    rect.top = label->Frame().bottom + 1;
    rect.right = separator;
    rect.bottom -= B_H_SCROLL_BAR_HEIGHT;
    rect.top += 2;
    rect.right -= B_V_SCROLL_BAR_WIDTH + 2;
    m_musicTreeView = new MusicTreeView( rect, "MusicTreeView",
                                         B_SINGLE_SELECTION_LIST,
                                         B_FOLLOW_ALL );
    m_musicTreeView->
        SetInvocationMessage( new BMessage( MBMSG_MUSIC_TREE_INVOKED ) );
    tip = new TooltipFilter( new BMessage( MBMSG_TOOLTIP_MESSAGE ),
                             "Double click to add to the current playlist" );
    m_musicTreeView->AddFilter( tip );
    left = new BScrollView( "scroll", m_musicTreeView,
                            B_FOLLOW_LEFT | B_FOLLOW_TOP_BOTTOM, 0,
                            false, true, B_FANCY_BORDER );
    AddChild( left );

    // Current playlist view
    rect = Bounds();
    rect.top = label->Frame().bottom + 1;
    rect.left = sep->Frame().right + 1;
    rect.bottom -= B_H_SCROLL_BAR_HEIGHT;
    // for scroll view
    rect.top += 2;
    rect.left += 2;
    rect.right -= B_V_SCROLL_BAR_WIDTH;
    rect.bottom -= B_H_SCROLL_BAR_HEIGHT;
    m_playlistView =
        new PlaylistView( this, rect, "PlaylistView", B_FOLLOW_ALL );
    m_playlistView->
        SetInvocationMessage( new BMessage( MBMSG_PLAYLIST_VIEW_INVOKED ) );
    right = new BScrollView( "scroll", m_playlistView, B_FOLLOW_ALL,
                             false, true, B_FANCY_BORDER );
    AddChild( right );

    sep->AddAttachment( left, right );

    // Placard (also acts as a tooltips placeholder)
    rect = Bounds();
    rect.top = rect.bottom - B_H_SCROLL_BAR_HEIGHT + 2 + 1;
    rect.right -= B_V_SCROLL_BAR_WIDTH;
    m_placard = new Placard( rect, "Placard",
                             B_FOLLOW_BOTTOM | B_FOLLOW_LEFT_RIGHT );
    m_placard->SetText( "Welcome to Music Browser" );
    AddChild( m_placard );

    UpdateCatalogView();
    UpdatePlaylistView();
}

void
BeOSMusicBrowser::MenusBeginning( void )
{
    if ( m_ui->IsSearching() )
    {
        m_searchControl->SetLabel( MENU_LABEL_STOP_SEARCH );
        m_searchControl->SetMessage( new BMessage( MBMSG_STOP_SEARCH ) );
    }
    else
    {
        m_searchControl->SetLabel( MENU_LABEL_START_SEARCH );
        m_searchControl->SetMessage( new BMessage( MBMSG_SEARCH_DIALOG ) );
    }
}

void
BeOSMusicBrowser::MessageReceived( BMessage* message )
{
    switch ( message->what )
    {
        case MBMSG_UPDATE_MUSIC_CATALOG_VIEW:
            UpdateCatalogView();
            break;
        case MBMSG_UPDATE_PLAYLIST_VIEW:
            UpdatePlaylistView();
            break;
        case MBMSG_ADD_FILES:
        {
            BFilePanel* filePanel =
                new BFilePanel( B_OPEN_PANEL,
                                new BMessenger( this ),
                                NULL, // panel_directory
                                B_FILE_NODE,
                                true, // allow_multiple_selection
                                NULL // message to be set below
                                );
            BMessage reply( MBMSG_ADD_FILES_REPLY );
            reply.AddPointer( "file_panel", filePanel );
            filePanel->SetMessage( &reply ); // message copied by file panel
            filePanel->Show();
            break;
        }
        case MBMSG_ADD_FILES_REPLY:
        {
            vector<string> files;
            int32 countFound = 0;
            type_code typeFound;
            message->GetInfo( "refs", &typeFound, &countFound );
            for ( int32 i = 0; i < countFound; i++ )
            {
                entry_ref ref;
                if ( message->FindRef( "refs", i, &ref ) == B_OK )
                {
                    BEntry entry( &ref );
                    if ( entry.InitCheck() == B_OK )
                    {
                        BPath path;
                        if ( entry.GetPath( &path ) == B_OK )
                        {
                            files.push_back( string( path.Path() ) );
                        }
                    }
                }
            }

            m_ui->AddFiles( files );

            // then delete the file panel.
            BFilePanel* filePanel;
            if ( message->FindPointer( "file_panel", (void**)&filePanel )
                 == B_OK )
            {
                delete filePanel;
            }
            break;
        }
        case MBMSG_SEARCH_DIALOG:
        case MBMSG_INTRO_WIZARD:
        {
            m_wizard = new IntroductionWizard( BRect( 100, 200, 600, 600 ),
                                              "Search Music", m_ui, m_context );
            m_wizard->Show();
            break;
        }
        case MBMSG_WIZARD_GONE:
            m_wizard = NULL;
            break;
        case MBMSG_STOP_SEARCH:
            m_context->catalog->StopSearchMusic();
            break;
        case MBMSG_SEARCH_MUSIC_DONE:
            if ( m_wizard )
            {
                m_wizard->PostMessage( MBMSG_SEARCH_MUSIC_DONE );
            }
            PostMessage( MBMSG_UPDATE_MUSIC_CATALOG_VIEW );
            break;
        case MBMSG_PLAYLIST_CURRENT_ITEM_INFO:
            m_playlistView->SetCurrentlyPlaying(
                                message->FindInt32( "index" ) );
            break;
        case MBMSG_PLAYLIST_ITEM_REMOVED:
        {
            PlaylistItemRemovedEvent* pire;
            if ( message->FindPointer( "event", (void**)&pire ) == B_OK &&
                 pire->Manager() == m_plm )
            {
                UpdatePlaylistView();
            }
            break;
        }
        case MBMSG_PLAYLIST_ITEMS_UPDATED:
        {
            PlaylistItemsUpdatedEvent* piue;
            if ( message->FindPointer( "event", (void**)&piue ) == B_OK &&
                 piue->Manager() == m_plm )
            {
                UpdatePlaylistView();
            }
            break;
        }
        case MBMSG_PLAYLIST_SORTED:
        {
            PlaylistSortedEvent* pse;
            if ( message->FindPointer( "event", (void**)&pse ) == B_OK &&
                 pse->Manager() == m_plm )
            {
                UpdatePlaylistView();
            }
            break;
        }
        case MBMSG_PLAYLIST_UPDATED: // not used?
            UpdatePlaylistView();
            break;
        case MBMSG_MUSIC_CATALOG_PLAYLIST_ADDED:
        case MBMSG_MUSIC_CATALOG_PLAYLIST_REMOVED:
            UpdateCatalogPlaylistGroup();
            break;
        case MBMSG_CREATE_NEW_PLAYLIST:
            m_ui->CreateNewEditor( string( "" ) );
            break;
        case MBMSG_SAVE_CURRENT_PLAYLIST:
            if ( !m_master && m_currentListName.length() > 0 )
            {
                SaveCurrentPlaylist( NULL );
            }
            else
            {
                SaveCurrentPlaylistPanel( message, false );
            }
            break;
        case MBMSG_SAVE_CURRENT_PLAYLIST_AS:
            SaveCurrentPlaylistPanel( message, false );
            break;
        case MBMSG_SAVE_CURRENT_PLAYLIST_REPLY:
            SaveCurrentPlaylistPanel( message, true );
            break;
        case MBMSG_IMPORT_ITEMS:
            break;
        case MBMSG_MUSIC_TREE_INVOKED:
        {
            PRINT(( "invoked" ));
            int32 index = message->FindInt32( "index" );
            CatalogItem* item = dynamic_cast<CatalogItem*>
                                ( m_musicTreeView->ItemAt( index ) );
            if ( item )
            {
                CatalogItemInvoked( item );
            }
            break;
        }
        case MBMSG_PLAYLIST_VIEW_INVOKED:
            PRINT(( "goto %d\n", message->FindInt32( "index" ) ));
            m_plm->SetCurrentIndex( message->FindInt32( "index" ) );
            break;
        case MBMSG_EDIT_CATALOG_ITEM:
        {
            CatalogItem* item;
            message->FindPointer( "item", (void**)&item );
            if ( item )
            {
                EditCatalogItem( item );
            }
            break;
        }
        case MBMSG_EDIT_INFO:
        {
            if ( !m_selectedView ) break;
            BListItem* li =
                m_selectedView->ItemAt( m_selectedView->CurrentSelection() );
            CatalogItem* item = dynamic_cast<CatalogItem*>( li );
            if ( item )
            {
                EditCatalogItem( item );
            }
            break;
        }
        case MBMSG_REMOVE_ITEMS:
        {
            int32 index = m_playlistView->CurrentSelection();
            if ( index >= 0 )
            {
                m_plm->RemoveItem( index );
            }
            break;
        }
        case MBMSG_MOVE_UP:
        {
            int32 index = m_playlistView->CurrentSelection();
            MovePlaylistItem( index, index - 1 );
            break;
        }
        case MBMSG_MOVE_DOWN:
        {
            int32 index = m_playlistView->CurrentSelection();
            MovePlaylistItem( index, index + 1 );
            break;
        }
        case MBMSG_SORT_PLAYLIST:
        {
            PlaylistSortKey key
                = (PlaylistSortKey)message->FindInt32( "PlaylistSortKey" );
            PRINT(( "Sorting by %ld\n", key ));
            m_plm->Sort( key );
            break;
        }
        case MBMSG_SELECTION_CHANGED:
        {
            BListView* lv;
            if ( message->FindPointer( "source", (void**)&lv ) < B_OK ) break;
            if ( m_selectedView == NULL )
            {
                m_selectedView = lv;
            }
            else if ( lv != m_selectedView && lv->CurrentSelection() >= 0 )
            {
                m_selectedView->DeselectAll();
                m_selectedView = lv;
            }
            break;
        }
        case MBMSG_TOOLTIP_MESSAGE:
        {
            const char* str;
            if ( message->FindString( "text", &str ) == B_OK )
            {
                m_placard->SetText( str );
            }
            break;
        }
#if DEBUG_MENU
        case MBMSG_DEBUG_1:
            m_musicTreeView->ClearMyMusicGroup();
            break;
        case MBMSG_DEBUG_2:
            m_musicTreeView->ClearPlaylistGroup();
            break;
        case MBMSG_DEBUG_3:
            UpdateCatalogView();
            break;
        case MBMSG_DEBUG_4:
            break;
#endif
        default:
            BWindow::MessageReceived( message );
            break;
    }
}

void
BeOSMusicBrowser::Quit( void )
{
    if ( !m_master )
    {
        PRINT(( "telling musicbrowser ui to dispose of the handle pointing to me\n" ));
        m_ui->EditorDone( this );
    }
    BWindow::Quit();
}

bool
BeOSMusicBrowser::QuitRequested( void )
{
    if ( m_master )
    {
        Hide();
        return false;
    }
    else
    {
        return true;
    }
}

void
BeOSMusicBrowser::LoadPlaylist( const string& playlist )
{
    if ( playlist == m_currentListName || playlist.length() == 0 ) return;

    if ( m_currentListName.length() > 0 && !m_master )
    {
        PRINT(( "FIXME: do you want to save the current playlist?\n" ));
    }

    if ( strncmp( "file://", playlist.c_str(), 7 ) == 0 )
    {
        m_plm->ReadPlaylist( playlist.c_str() );
        m_currentListName = playlist;
    }
    else
    {
        uint32 length = B_PATH_NAME_LENGTH;
        char* url = new char[ length ];
        if ( IsntError( FilePathToURL( playlist.c_str(), url, &length ) ) )
        {
            m_plm->ReadPlaylist( url );
            m_currentListName = url;
        }
        delete[] url;
    }
}

void
BeOSMusicBrowser::SaveCurrentPlaylist( const char* path )
{
    if ( path != NULL )
    {
        m_currentListName = path;
    }

    if ( m_currentListName.length() == 0 ) return;

	char* ext = strrchr( m_currentListName.c_str(), '.' );
    if ( ext ) ext++;
    PRINT(( "ext = %s\n", ext ));

    int i = 0;
    bool found = false;
    PlaylistFormatInfo format;
    while ( ext &&
            m_plm->GetSupportedPlaylistFormats( &format, i ) == kError_NoErr )
    {
        PRINT(( "supported format ext = %s\n", format.GetExtension() ));
        if ( strcmp( ext, format.GetExtension() ) == 0 )
        {
            found = true;
            break;
        }
        i++;
    }

    if ( !found )
    {
        // if no format is supported, just use the first supported format.
        m_plm->GetSupportedPlaylistFormats( &format, 0 );
        m_currentListName += ".";
        m_currentListName += format.GetExtension();
    }

    PRINT(( "saving as list = %s\n", m_currentListName.c_str() ));

    if ( strncmp( "file://", m_currentListName.c_str(), 7 ) == 0 )
    {
        m_plm->WritePlaylist( m_currentListName.c_str(), &format );
        m_context->catalog->AddPlaylist( m_currentListName.c_str() );
    }
    else
    {
        size_t len = m_currentListName.length() + 20;
        char* writeURL = new char[ len ];
        Error err = FilePathToURL( m_currentListName.c_str(), writeURL, &len );
        if ( IsntError( err ) )
        {
            PRINT(( "Saving current playlist as %s\n", writeURL ));
            m_plm->WritePlaylist( writeURL, &format );
            m_context->catalog->AddPlaylist( writeURL );
        }
        delete[] writeURL;
    }
}

// ------------------------------------------
// Edit whatever item the user has selected.
//   Playlist -> Spawn new editor.
//   TrackItem -> Spawn meta data editor.
// ------------------------------------------
void
BeOSMusicBrowser::EditCatalogItem( CatalogItem* item )
{
    switch ( item->Type() )
    {
        case CatalogItem::ITEM_PLAYLIST:
        {
            PRINT(( "This is playlist\n" ));
            PlaylistListItem* pi = dynamic_cast<PlaylistListItem*>( item );
            m_ui->CreateNewEditor( string( pi->URL() ) );
            break;
        }
        case CatalogItem::ITEM_TRACK:
        {
            PRINT(( "Editing the meta data\n" ));
            TrackItem* ti = dynamic_cast<TrackItem*>( item );
            InfoEditor* editor =
                new InfoEditor( BRect( 100, 100, 400, 300 ), "Edit Info",
                                ti->Item() );
            editor->Show();
            const MetaData& md = ti->Item()->GetMetaData();
            PRINT(( "%s, %s, %s\n", md.Artist().c_str(), md.Album().c_str(), md.Title().c_str() ));
            break;
        }
        default:
            break;
    }
}

// ----------------------------------------------------
// Rebuild the music catalog tree view from the catalog
// ----------------------------------------------------
void
BeOSMusicBrowser::UpdateCatalogView( void )
{
    MusicCatalog* catalog = m_context->catalog;

    catalog->GetCatalogLock();

    const vector<PlaylistItem*>* unsorted  = catalog->GetUnsortedMusic();
    const vector<ArtistList*>*   artists   = catalog->GetMusicList();

    PRINT(( "there're %ld unsorted items in catalog\n", unsorted->size() ));

    m_musicTreeView->ClearMyMusicGroup();

    int32 artistOutlineLevel = m_musicTreeView->ArtistOutlineLevel();
    vector<PlaylistItem*>::const_iterator i;
    for ( i = unsorted->begin(); i != unsorted->end(); i++ )
    {
        TrackItem* uncatItem = new TrackItem( *i );
        m_musicTreeView->AddUnderUncategorizedGroup( uncatItem );
    }

    vector<ArtistList*>::const_iterator artist;
    for ( artist = artists->begin(); artist != artists->end(); artist++ )
    {
        CollectionItem* artistItem
            = new CollectionItem( (*artist)->name.c_str(), artistOutlineLevel );
        m_musicTreeView->AddArtistItem( artistItem );
        vector<AlbumList*>* albums = (*artist)->m_albumList;
        vector<AlbumList*>::iterator album;
        for ( album = albums->begin(); album != albums->end(); album++ )
        {
            CollectionItem* albumItem
                = new CollectionItem( (*album)->name.c_str() );
            m_musicTreeView->AddUnder( albumItem, artistItem );
            vector<PlaylistItem*>* tracks = (*album)->m_trackList;
            vector<PlaylistItem*>::iterator track;
            for ( track = tracks->begin(); track != tracks->end(); track++ )
            {
                TrackItem* item = new TrackItem( *track );
                m_musicTreeView->AddUnder( item, albumItem );
                TrackItem* allItem = new TrackItem( *item );
                m_musicTreeView->AddUnderAllTracksGroup( allItem );
            }
        }
    }

    UpdateCatalogPlaylistGroup();

    catalog->ReleaseCatalogLock();

    m_musicTreeView->FindArtistGroup( "Headboard" );
}

// --------------------------------------------
// Rebuild the playlist group only
// --------------------------------------------
void
BeOSMusicBrowser::UpdateCatalogPlaylistGroup( void )
{
    MusicCatalog* catalog = m_context->catalog;

    catalog->GetCatalogLock(); // on beos mutex == BLocker so it can be nested.

    const vector<string>* playlists = catalog->GetPlaylists();

    m_musicTreeView->ClearPlaylistGroup();

    // Fill playlists.
    vector<string>::const_iterator pi;
    for ( pi = playlists->begin(); pi != playlists->end(); pi++ )
    {
        PlaylistListItem* item = new PlaylistListItem( (*pi) );
        m_musicTreeView->AddPlaylistListItem( item );
    }

    catalog->ReleaseCatalogLock();
}

// -------------------------------------------
// Rebuild the playlist view from the catalog
// -------------------------------------------
void
BeOSMusicBrowser::UpdatePlaylistView( void )
{
    Lock();
    BeginViewTransaction();

    // Remember the selected index.
    uint32 selected = m_playlistView->CurrentSelection();

    // Populate the playlist view.
    uint32 itemsInPlaylist = m_playlistView->CountItems();
    for ( uint32 i = 0; i < m_plm->CountItems(); i++ )
    {
        bool addItReally = false;
        PlaylistItem* item = m_plm->ItemAt( i );
        if ( i < itemsInPlaylist )
        {
            TrackItem* oldItem = (TrackItem*)m_playlistView->ItemAt( i );
            if ( *oldItem != *item )
            {
                delete m_playlistView->RemoveItem( i );
                addItReally = true;
            }
        }
        else
        {
            addItReally = true;
        }

        if ( addItReally )
        {
            TrackItem* listItem = new TrackItem( item );
            m_playlistView->AddItem( listItem, i );
        }
    }

    // If there's anything left in the playlist view, it should be removed.
    int32 removeBegin = m_plm->CountItems();
    int32 removeEnd = m_playlistView->CountItems();
    if ( removeBegin < removeEnd )
    {
        m_playlistView->RemoveItems( removeBegin, removeEnd - removeBegin );
    }

    // Set the currently playing item.
    m_playlistView->SetCurrentlyPlaying( m_plm->GetCurrentIndex() );

    // Set the current selection which might have been unselected during
    // the list update process.
    m_playlistView->Select( selected );

    EndViewTransaction();
    Unlock();
}

void
BeOSMusicBrowser::ClearPlaylistView( void )
{
    for ( int32 i = m_playlistView->CountItems() - 1; i >= 0; i-- )
    {
        BListItem* item = m_playlistView->RemoveItem( i );
        delete item;
    }
}

// --------------------------------------------------------------------
// User has invoked (double clicked) an item in the music catalog tree
// --------------------------------------------------------------------
void
BeOSMusicBrowser::CatalogItemInvoked( CatalogItem* item )
{
    switch ( item->Type() )
    {
        case CatalogItem::ITEM_TRACK:
        {
            TrackItem* ti = dynamic_cast<TrackItem*>( item );
            ti->PrintToStream();
            m_plm->AddItem( ti->URL() );
            break;
        }
        case CatalogItem::ITEM_COLLECTION:
        {
            CollectionItem* ci = dynamic_cast<CollectionItem*>( item );
            ci->PrintToStream();
            vector<string> urls;
            uint32 len = GetURLsUnder( ci, &urls );
            PRINT(( "there are %d items\n", len ));
            m_plm->AddItems( urls );
            break;
        }
        case CatalogItem::ITEM_PLAYLIST:
        {
            PlaylistListItem* plli = dynamic_cast<PlaylistListItem*>( item );
            if ( !plli ) break;
            PRINT(( "playlist %s\n", plli->URL() ));
            m_plm->ReadPlaylist( plli->URL() );
            break;
        }
        default:
            break;
    }
}

uint32
BeOSMusicBrowser::GetURLsUnder( CollectionItem* item, vector<string>* urls )
{
    int32 count = m_musicTreeView->CountItemsUnder( item, false );

    for ( int32 i = 0; i < count; i++ )
    {
        BListItem* listItem = m_musicTreeView->ItemUnderAt( item, false, i );
        TrackItem* trackItem = dynamic_cast<TrackItem*>( listItem );
        if ( trackItem )
        {
            PRINT(( "url: %s\n", trackItem->URL() ));
            urls->push_back( string( trackItem->URL() ) );
        }
    }

    return urls->size();
}

void
BeOSMusicBrowser::SaveCurrentPlaylistPanel( BMessage* message, bool reply )
{
    if ( reply )
    {
        entry_ref dir;
        const char* name;
        string path;

        if ( message->FindRef( "directory", &dir ) < B_OK ) return;
        name =  message->FindString( "name" );
        if ( !name ) return;

        BEntry entry( &dir );
        BPath bpath;
        entry.GetPath( &bpath );

        path = string( bpath.Path() ) + string( "/" ) + string( name );
        PRINT(( "User chose the name %s\n", path.c_str() ));
        SaveCurrentPlaylist( path.c_str() );
    }
    else
    {
        BFilePanel* filePanel =
            new BFilePanel( B_SAVE_PANEL,
                            new BMessenger( this ),
                            NULL, // panel_directory
                            B_FILE_NODE,
                            false, // allow_multiple_selection
                            NULL // message to be set below
                            );
        BMessage reply( MBMSG_SAVE_CURRENT_PLAYLIST_REPLY );
        reply.AddPointer( "file_panel", filePanel );
        filePanel->SetMessage( &reply ); // message copied by file panel
        filePanel->Show();
    }
}

void
BeOSMusicBrowser::MovePlaylistItem( uint32 oldIndex, uint32 newIndex )
{
    PRINT(( "Moving %d to %d\n", oldIndex, newIndex ));
    if ( IsntError( m_plm->MoveItem( oldIndex, newIndex ) ) )
    {
        // Selection needs to follow this item.
        m_playlistView->Select( newIndex );
        // At this point, the selection and the actual item selected is NOT
        // in sync, but the list view will be updated shortly later
        // through PlaylistItemMoved event.
    }
}

// -----------------------------------------------------------------
// Init & build UI parts.
// -----------------------------------------------------------------
void
BeOSMusicBrowser::BuildMenu( BMenuBar* menuBar )
{
    BMenuItem* item;
    BMenu* menu;
    BMessage* message;

    // File Menu ------------------------------------------------------------
    menu = new BMenu( "File" );

    item = new BMenuItem( "New Playlist", 
                          new BMessage( MBMSG_CREATE_NEW_PLAYLIST ) , 'N' );
    menu->AddItem( item );
    item = new BMenuItem( "Open Playlist",
                          NULL, 'O' );
    menu->AddItem( item );
    item = new BMenuItem( "Save Playlist",
                          new BMessage( MBMSG_SAVE_CURRENT_PLAYLIST ), 'S' );
    menu->AddItem( item );
    item = new BMenuItem( "Save Playlist As",
                          new BMessage( MBMSG_SAVE_CURRENT_PLAYLIST_AS ),
                          'S', B_SHIFT_KEY );
    menu->AddItem( item );
    menu->AddSeparatorItem();
    item = new BMenuItem( "Import Tracks and Playlists",
                          new BMessage( MBMSG_IMPORT_ITEMS ), 'I' );
    menu->AddItem( item );
    item = new BMenuItem( "Export Playlist", NULL );
    menu->AddItem( item );
    menu->AddSeparatorItem();
    // Modified later if neccesary (if searching is in progress.)
    item = new BMenuItem( MENU_LABEL_START_SEARCH,
                          new BMessage( MBMSG_SEARCH_DIALOG ), 'M' );
    menu->AddItem( item );
    m_searchControl = item;
    menu->AddSeparatorItem();
    item = new BMenuItem( "Close", new BMessage( B_QUIT_REQUESTED ), 'Q' );
    menu->AddItem( item );

    menuBar->AddItem( menu );

    // Edit. ----------------------------------------------------------------
    menu = new BMenu( "Edit" );

    item = new BMenuItem( "Add Items to Playlist", NULL );
    menu->AddItem( item );
    item = new BMenuItem( "Add Tracks or Playlists from Disk", NULL );
    menu->AddItem( item );
    item = new BMenuItem( "Remove Items from My Music",
                           new BMessage( MBMSG_REMOVE_ITEMS ) );
    menu->AddItem( item );
    menu->AddSeparatorItem();
    item = new BMenuItem( "Move Up", new BMessage( MBMSG_MOVE_UP ) );
    menu->AddItem( item );
    item = new BMenuItem( "Move Down", new BMessage( MBMSG_MOVE_DOWN ) );
    menu->AddItem( item );
    menu->AddSeparatorItem();
    item = new BMenuItem( "Clear Playlist",
                          new BMessage( MBMSG_CLEAR_PLAYLIST ) );
    menu->AddItem( item );
    item = new BMenuItem( "Edit Info", new BMessage( MBMSG_EDIT_INFO ) );
    menu->AddItem( item );

    menuBar->AddItem( menu );

    // View. -----------------------------------------------------------------
    menu = new BMenu( "View" );

    item = new BMenuItem( "View Playlist Only", NULL );
    menu->AddItem( item );
    item = new BMenuItem( "Options", NULL );

    menuBar->AddItem( menu );

    // Control. --------------------------------------------------------------
    menu = new BMenu( "Control" );

    item = new BMenuItem( "Play", NULL );
    menu->AddItem( item );
    item = new BMenuItem( "Stop", NULL );
    menu->AddItem( item );
    menu->AddSeparatorItem();
    item = new BMenuItem( "Next Track", NULL );
    menu->AddItem( item );
    item = new BMenuItem( "Previous Track", NULL );
    menu->AddItem( item );
    menu->AddSeparatorItem();
    item = new BMenuItem( "Play Tracks in Normal Order", NULL );
    menu->AddItem( item );
    item = new BMenuItem( "Play Tracks in Random Order", NULL );
    menu->AddItem( item );
    menu->AddSeparatorItem();
    item = new BMenuItem( "Repeat No Tracks", NULL );
    menu->AddItem( item );
    item = new BMenuItem( "Repeat One Track", NULL );
    menu->AddItem( item );
    item = new BMenuItem( "Repeat All Tracks", NULL );
    menu->AddItem( item );

    menuBar->AddItem( menu );

    // Sort. ----------------------------------------------------------------
    menu = new BMenu( "Sort" );

    message = new BMessage( MBMSG_SORT_PLAYLIST );
    message->AddInt32( "PlaylistSortKey", kPlaylistSortKey_Artist );
    item = new BMenuItem( "Sort Playlist by Artist", message, 'A' );
    menu->AddItem( item );
    message = new BMessage( MBMSG_SORT_PLAYLIST );
    message->AddInt32( "PlaylistSortKey", kPlaylistSortKey_Album );
    item = new BMenuItem( "Sort Playlist by Album", message, 'L' );
    menu->AddItem( item );
    message = new BMessage( MBMSG_SORT_PLAYLIST );
    message->AddInt32( "PlaylistSortKey", kPlaylistSortKey_Title );
    item = new BMenuItem( "Sort Playlist by Title", message, 'T' );
    menu->AddItem( item );
    message = new BMessage( MBMSG_SORT_PLAYLIST );
    message->AddInt32( "PlaylistSortKey", kPlaylistSortKey_Year );
    item = new BMenuItem( "Sort Playlist by Year", message, 'Y' );
    menu->AddItem( item );
    message = new BMessage( MBMSG_SORT_PLAYLIST );
    message->AddInt32( "PlaylistSortKey", kPlaylistSortKey_Track );
    item = new BMenuItem( "Sort Playlist by Track Number", message, 'R' );
    menu->AddItem( item );
    message = new BMessage( MBMSG_SORT_PLAYLIST );
    message->AddInt32( "PlaylistSortKey", kPlaylistSortKey_Genre );
    item = new BMenuItem( "Sort Playlist by Genre", message );
    menu->AddItem( item );
    message = new BMessage( MBMSG_SORT_PLAYLIST );
    message->AddInt32( "PlaylistSortKey", kPlaylistSortKey_Time );
    item = new BMenuItem( "Sort Playlist by Length", message );
    menu->AddItem( item );
    message = new BMessage( MBMSG_SORT_PLAYLIST );
    message->AddInt32( "PlaylistSortKey", kPlaylistSortKey_Location );
    item = new BMenuItem( "Sort Playlist by Location", message );
    menu->AddItem( item );
    message = new BMessage( MBMSG_SORT_PLAYLIST );
    message->AddInt32( "PlaylistSortKey", kPlaylistSortKey_Random );
    item = new BMenuItem( "Sort Playlist Randomly", message );
    menu->AddItem( item );

    menuBar->AddItem( menu );

    // Help. -------------------------------------------------------------
    menu = new BMenu( "Help" );
    item = new BMenuItem( "FreeAmp Web Site", NULL );
    menu->AddItem( item );
    item = new BMenuItem( "EMusic.com Web Site", NULL );
    menu->AddItem( item );
    menuBar->AddItem( menu );

#if DEBUG_MENU
    menu = new BMenu( "Debug" );
    item = new BMenuItem( "Clear My Music", new BMessage( MBMSG_DEBUG_1 ) );
    menu->AddItem( item );
    item = new BMenuItem( "Clear My Playlist", new BMessage( MBMSG_DEBUG_2 ) );
    menu->AddItem( item );
    item = new BMenuItem( "Rebuild Catalog View", new BMessage( MBMSG_DEBUG_3 ) );
    menu->AddItem( item );
    menuBar->AddItem( menu );
#endif
}

void
BeOSMusicBrowser::BuildToolBar( ToolBar* toolBar )
{
    TooltipFilter* tip;
    ResourceManager rsrcs( "musicbrowser.ui" );

    ToolBarButton* button;
    BBitmap* bitmap;

    bitmap = rsrcs.GetBitmap( B_TRANSLATOR_BITMAP, TOOLBAR_NEW_PLAYLIST );
    button = new ToolBarButton( bitmap, "New Playlist",
                                new BMessage( MBMSG_CREATE_NEW_PLAYLIST ) );
    tip = new TooltipFilter( new BMessage( MBMSG_TOOLTIP_MESSAGE ),
                             "Create new playlist" );
    button->AddFilter( tip );
    toolBar->AddItem( button );

    bitmap = rsrcs.GetBitmap( B_TRANSLATOR_BITMAP, TOOLBAR_SAVE_PLAYLIST );
    button = new ToolBarButton( bitmap, "Save Playlist",
                                new BMessage( MBMSG_SAVE_CURRENT_PLAYLIST ) );
    tip = new TooltipFilter( new BMessage( MBMSG_TOOLTIP_MESSAGE ),
                             "Save playlist" );
    button->AddFilter( tip );
    toolBar->AddItem( button );

    bitmap = rsrcs.GetBitmap( B_TRANSLATOR_BITMAP, TOOLBAR_IMPORT_ITEMS );
    button = new ToolBarButton( bitmap, "Import Items",
                                new BMessage( MBMSG_IMPORT_ITEMS ) );
    tip = new TooltipFilter( new BMessage( MBMSG_TOOLTIP_MESSAGE ),
                             "Import items from disk" );
    button->AddFilter( tip );
    toolBar->AddItem( button );

    bitmap = rsrcs.GetBitmap( B_TRANSLATOR_BITMAP, TOOLBAR_REMOVE_ITEMS );
    button = new ToolBarButton( bitmap, "Remove Items",
                                new BMessage( MBMSG_REMOVE_ITEMS ) );
    tip = new TooltipFilter( new BMessage( MBMSG_TOOLTIP_MESSAGE ),
                             "Remove Items" );
    button->AddFilter( tip );
    toolBar->AddItem( button );

    bitmap = rsrcs.GetBitmap( B_TRANSLATOR_BITMAP, TOOLBAR_EDIT_INFO );
    button = new ToolBarButton( bitmap, "Edit Info",
                                new BMessage( MBMSG_EDIT_INFO ) );
    tip = new TooltipFilter( new BMessage( MBMSG_TOOLTIP_MESSAGE ),
                             "Edit Info" );
    button->AddFilter( tip );
    toolBar->AddItem( button );

    bitmap = rsrcs.GetBitmap( B_TRANSLATOR_BITMAP, TOOLBAR_ADD_TRACKS );
    button = new ToolBarButton( bitmap, "Add Tracks",
                                NULL );
    tip = new TooltipFilter( new BMessage( MBMSG_TOOLTIP_MESSAGE ),
                             "Add Tracks" );
    button->AddFilter( tip );
    toolBar->AddItem( button );

    bitmap = rsrcs.GetBitmap( B_TRANSLATOR_BITMAP, TOOLBAR_ADD_FILES );
    button = new ToolBarButton( bitmap, "Add Files",
                                NULL );
    tip = new TooltipFilter( new BMessage( MBMSG_TOOLTIP_MESSAGE ),
                             "Add Files" );
    button->AddFilter( tip );
    toolBar->AddItem( button );

    bitmap = rsrcs.GetBitmap( B_TRANSLATOR_BITMAP, TOOLBAR_UP );
    button = new ToolBarButton( bitmap, "Up",
                                new BMessage( MBMSG_MOVE_UP ) );
    tip = new TooltipFilter( new BMessage( MBMSG_TOOLTIP_MESSAGE ),
                             "Up" );
    button->AddFilter( tip );
    toolBar->AddItem( button );

    bitmap = rsrcs.GetBitmap( B_TRANSLATOR_BITMAP, TOOLBAR_DOWN );
    button = new ToolBarButton( bitmap, "Down",
                                new BMessage( MBMSG_MOVE_DOWN ) );
    tip = new TooltipFilter( new BMessage( MBMSG_TOOLTIP_MESSAGE ),
                             "Down" );
    button->AddFilter( tip );
    toolBar->AddItem( button );

    toolBar->ResizeToPreferred();
}

// vi: set ts=4:

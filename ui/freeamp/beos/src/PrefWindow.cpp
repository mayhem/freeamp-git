/*____________________________________________________________________________

   FreeAmp - The Free MP3 Player

   Copyright (C) 1999 EMusic
   Portions Copyright (C) 1999 Hiromasa Kato (hiro@eng.iastate.edu)

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

#include "PrefWindow.h"
#include "PrefView.h"
#include "PrefViews.h"
#include "facontext.h"
#include "eventdata.h"
#include <be/interface/ListView.h>
#include <be/interface/ListItem.h>
#include <be/interface/Box.h>
#include <be/interface/Button.h>
#include <be/interface/ScrollBar.h>
#define DEBUG 1
#include <be/support/Debug.h>
#include <assert.h>

int32 PrefWindow::s_running = 0;

PrefWindow::PrefWindow( BRect frame, const char* title, FAContext* context,
                        ThemeManager* themeManager )
:   BWindow( frame, title, B_DOCUMENT_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, 0 ),
    m_context( context ),
    m_themeManager( themeManager )
{
    // make sure no other instance is running.
    int32 old = atomic_or( &s_running, 1 );
    assert( old == 0 );

    m_okToQuitSem = create_sem( 0, "PrefWindowOkToQuit" );

    m_originalValues.GetPrefsValues( m_context->prefs, m_themeManager );
    m_proposedValues = m_currentValues = m_originalValues;

    InitViews();
}

void
PrefWindow::InitViews( void )
{
    BRect frame( Frame() );
    frame.OffsetTo( B_ORIGIN );

#if 1
    // Construct the views.

    BView* root = new BView( frame, "GreyRoot", B_FOLLOW_ALL, B_WILL_DRAW );
    root->SetViewColor( ui_color( B_PANEL_BACKGROUND_COLOR ) );
    AddChild( root );

    // Buttons.

    BButton* button;
    float left, top;
    button = new BButton( BRect(0,0,0,0), "CancelButton", "Cancel",
                          new BMessage( MSG_CANCEL ),
                          B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM );
    button->ResizeToPreferred();
    button->MoveTo( frame.right - button->Frame().Width()
                    - 10 - B_V_SCROLL_BAR_WIDTH,
                    frame.bottom - button->Frame().Height() - 10 );
    root->AddChild( button );

    top = button->Frame().top;
    left = button->Frame().left;
    button = new BButton( BRect(0,0,0,0), "ApplyButton", "Apply",
                          new BMessage( MSG_APPLY ),
                          B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM );
    button->ResizeToPreferred();
    button->MoveTo( left - button->Frame().Width() - 10, top );
    root->AddChild( button );

    left = button->Frame().left;
    button = new BButton( BRect(0,0,0,0), "OkButton", "OK",
                          new BMessage( MSG_OK ),
                          B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM );
    button->ResizeToPreferred();
    button->MoveTo( left - button->Frame().Width() - 10, top );
    root->AddChild( button );

    // Pref pane list.

    frame = BRect( 2, 2, 2, 2 );
    BListView* paneList =
        new BListView( frame, "PaneList", B_SINGLE_SELECTION_LIST,
                       B_FOLLOW_LEFT | B_FOLLOW_TOP_BOTTOM );
    paneList->SetSelectionMessage( new BMessage( MSG_SELECT_PANE ) );
    paneList->AddItem( new BStringItem( "General" ) );
    paneList->AddItem( new BStringItem( "Themes" ) );
    paneList->AddItem( new BStringItem( "Directories" ) );
    paneList->AddItem( new BStringItem( "Streaming" ) );
    paneList->AddItem( new BStringItem( "Plugins" ) );
    paneList->AddItem( new BStringItem( "Advanced" ) );
    paneList->AddItem( new BStringItem( "About" ) );

    float width = paneList->StringWidth( "Directories" ) + 20;

    paneList->ResizeTo( width, paneList->Bounds().Height() );

    frame = BRect( 10, 10, 10 + width, top - 10 );
    BBox* box = new BBox( frame, "Box", B_FOLLOW_LEFT | B_FOLLOW_TOP_BOTTOM );
    paneList->ResizeTo( frame.Width() - 4, frame.Height() - 4 );
    box->AddChild( paneList );

    root->AddChild( box );

    // Preferences Panes
    m_prefViews.UsePrefs( &m_currentValues );

    BView* pane;
    frame = Bounds();
    frame.left = box->Frame().right + 1;
    frame.InsetBy( 10, 10 );

    pane = m_prefViews.BuildGeneralPane( frame, "General" );
    root->AddChild( pane );
    m_panes.push_back( pane );

    pane = m_prefViews.BuildThemesPane( frame, "Themes" );
    root->AddChild( pane );
    pane->Hide();
    m_panes.push_back( pane );

    m_currentPane = 0;
#else
    PrefView* prefView = new PrefView( context, themeManager,
                                       frame, "PrefView" );
    AddChild( prefView );
#endif
}

PrefWindow::~PrefWindow()
{
    delete_sem( m_okToQuitSem );
}

void
PrefWindow::MessageReceived( BMessage* message )
{
    switch ( message->what )
    {
        case MSG_SELECT_PANE:
            SelectPane( message->FindInt32( "index" ) );
            break;
        case MSG_APPLY:
            PRINT(( "Apply\n" ));
            Apply();
            break;
        case MSG_OK:
            PRINT(( "OK\n" ));
            Apply();
            PostMessage( B_QUIT_REQUESTED );
            break;
        case MSG_CANCEL:
            PRINT(( "Cancel\n" ));
            PostMessage( B_QUIT_REQUESTED );
            break;
        case MSG_SET_TEXT_ONLY:
            m_proposedValues.useTextLabels = true;
            m_proposedValues.useImages = false;
            break;
        case MSG_SET_IMAGES_ONLY:
            m_proposedValues.useTextLabels = false;
            m_proposedValues.useImages = true;
            break;
        case MSG_SET_TEXT_AND_IMAGES:
            m_proposedValues.useTextLabels = true;
            m_proposedValues.useImages = true;
            break;
        default:
            BWindow::MessageReceived( message );
            break;
    }
}

bool
PrefWindow::QuitRequested( void )
{
    int32 old = atomic_and( &s_running, 0 );
    assert( old == 1 );

    release_sem( m_okToQuitSem );

    return true;
}

bool
PrefWindow::WaitForQuit( bigtime_t timeout )
{
    status_t err;
    err = acquire_sem_etc( m_okToQuitSem, 1, B_RELATIVE_TIMEOUT, timeout );
    return ( err == B_NO_ERROR );
}

bool
PrefWindow::IsRunning( void )
{
    int32 old = atomic_and( &s_running, 1 );
    return ( old == 1 );
}

void
PrefWindow::SelectPane( uint32 index )
{
    PRINT(( "Page %d\n", index ));
    if ( m_currentPane == index || index >= m_panes.size() ) return;

    m_panes[m_currentPane]->Hide();
    m_panes[index]->Show();
    m_currentPane = index;
}

void
PrefWindow::Apply( void )
{
    if ( m_proposedValues != m_currentValues )
    {
        m_proposedValues.SavePrefsValues( m_context->prefs, m_themeManager );
        m_currentValues = m_proposedValues;
        m_context->target->AcceptEvent( new Event( INFO_PrefsChanged ) );
    }
}

// vi: set ts=4:

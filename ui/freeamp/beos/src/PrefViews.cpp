/*____________________________________________________________________________

   FreeAmp - The Free MP3 Player

   Copyright (C) 1999 EMusic
   Copyright (C) 1999 Hiromasa Kato

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

#include "PrefViews.h"
#include "PrefWindow.h"
#include "Prefs.h"
#include "ThemeManager.h"
#include <be/interface/TextControl.h>
#include <be/interface/Button.h>
#include <be/interface/RadioButton.h>
#include <be/interface/CheckBox.h>
#define DEBUG 1
#include <be/support/Debug.h>
#include <string>
#include <map>
using namespace std;

PrefViews::PrefViews()
{
}

PrefViews::~PrefViews()
{
}

BView*
PrefViews::BuildGeneralPane( BRect frame, const char* name )
{
    BRect bounds( frame );
    BRect rect;
    BView* pane;
    BView* dummy;
    BoxView* box;
    BTextControl* textCtrl;
    BButton* button;
    BRadioButton* radioButton;
    BCheckBox* checkBox;

    pane = new BView( frame, name, B_FOLLOW_ALL, B_WILL_DRAW );
    pane->SetViewColor( ui_color( B_PANEL_BACKGROUND_COLOR ) );

    bounds.OffsetTo( B_ORIGIN );
    rect = bounds;

    // Save Music Folder Box Group.
    box = new BoxView( rect, "SaveMusicFolderBox", NULL );
    box->SetLabel( "Save Music Folder" );

    rect.right = box->ContentWidth();
    dummy = new BView( rect, "Dummy", B_FOLLOW_ALL, 0 );
    dummy->SetViewColor( pane->ViewColor() );

    button = new BButton( BRect(0,0,0,0), "Browse", "Browse" B_UTF8_ELLIPSIS,
                          NULL,
                          B_FOLLOW_RIGHT | B_FOLLOW_TOP );
    button->ResizeToPreferred();
    button->MoveTo( dummy->Bounds().right - button->Frame().Width(), 0 );
    dummy->AddChild( button );

    rect.right = button->Frame().left - 10;
    textCtrl = new BTextControl( rect, "SaveMusicFolder", NULL, NULL, NULL,
                                 B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP );
    textCtrl->ResizeToPreferred();
    textCtrl->SetText( m_prefs->saveMusicDirectory.c_str() );
    textCtrl->MoveTo( 0, 0.5 * button->Frame().Height() -
                      0.5 * textCtrl->Frame().Height() );

    dummy->AddChild( textCtrl );
    dummy->ResizeTo( dummy->Frame().Width(), button->Frame().bottom );

    box->SetContentView( dummy );

    pane->AddChild( box );

    // Update Manager box

    // File Association Box
    rect = bounds;
    rect.top = box->Frame().bottom + 10;
    box = new BoxView( rect, "FileAssociationBox", NULL );
    box->SetLabel( "File Association" );

    rect = box->Bounds();
    rect.right = box->ContentWidth();
    dummy = new BView( rect, "Dummy", B_FOLLOW_ALL, B_WILL_DRAW );
    dummy->SetViewColor( pane->ViewColor() );

    checkBox = new BCheckBox( rect, "ReclaimFileTypes", "Reclaim file types",
                              NULL,
                              B_FOLLOW_LEFT | B_FOLLOW_TOP );
    checkBox->SetValue( m_prefs->reclaimFiletypes ? B_CONTROL_ON
                                                  : B_CONTROL_OFF );
    checkBox->ResizeToPreferred();
    dummy->AddChild( checkBox );

    rect.top = checkBox->Frame().bottom + 10;
    checkBox = new BCheckBox( rect, "AskToReclaimFileTypes",
                              "Ask to reclaim file types",
                              NULL,
                              B_FOLLOW_LEFT | B_FOLLOW_TOP );
    checkBox->SetValue( m_prefs->askReclaimFiletypes ? B_CONTROL_ON
                                                     : B_CONTROL_OFF );
    checkBox->ResizeToPreferred();
    dummy->AddChild( checkBox );

    dummy->ResizeTo( dummy->Bounds().Width(), checkBox->Frame().bottom );

    box->SetContentView( dummy );
    pane->AddChild( box );

    // My Music Toolbar Appearance Box Group.
    rect = bounds;
    rect.top = box->Frame().bottom + 10;
    box = new BoxView( rect, "ToolbarAppearanceBox", NULL );
    box->SetLabel( "Show 'My Music' Toolbars As" );

    rect = box->Bounds();
    rect.right = box->ContentWidth();
    dummy = new BView( rect, "Dummy", B_FOLLOW_ALL, B_WILL_DRAW );
    dummy->SetViewColor( pane->ViewColor() );

    radioButton = new BRadioButton( rect, "TextOnly", "Text Only",
                                    new BMessage( MSG_SET_TEXT_ONLY ) );
    radioButton->ResizeToPreferred();
    dummy->AddChild( radioButton );
    radioButton->SetValue( m_prefs->useTextLabels ? B_CONTROL_ON
                                                 : B_CONTROL_OFF );

    rect.left = radioButton->Frame().right + 10;
    radioButton = new BRadioButton( rect, "ImagesOnly", "Images Only",
                                    new BMessage( MSG_SET_IMAGES_ONLY ) );
    radioButton->ResizeToPreferred();
    dummy->AddChild( radioButton );
    radioButton->SetValue( m_prefs->useImages ? B_CONTROL_ON
                                              : B_CONTROL_OFF );

    rect.left = radioButton->Frame().right + 10;
    radioButton = new BRadioButton( rect, "TextAndImages", "Text and Images",
                                    new BMessage( MSG_SET_TEXT_AND_IMAGES ) );
    radioButton->ResizeToPreferred();
    dummy->AddChild( radioButton );
    radioButton->SetValue( ( m_prefs->useTextLabels &&
                             m_prefs->useImages )
                           ? B_CONTROL_ON : B_CONTROL_OFF );

    dummy->ResizeTo( dummy->Bounds().Width(), radioButton->Frame().bottom );

    box->SetContentView( dummy );

    pane->AddChild( box );

    // Miscellaneous Box Group.
    rect = bounds;
    rect.top = box->Frame().bottom + 10;
    box = new BoxView( rect, "MiscellaneousBox", NULL );
    box->SetLabel( "Miscellaneous" );

    rect = box->Bounds();
    rect.right = box->ContentWidth();
    dummy = new BView( rect, "Dummy", B_FOLLOW_ALL, B_WILL_DRAW );
    dummy->SetViewColor( pane->ViewColor() );

    checkBox = new BCheckBox( rect, "SaveCurrentPlaylistOnExit",
                              "Save current playlist when exiting "
                              "the application",
                              NULL,
                              B_FOLLOW_LEFT | B_FOLLOW_TOP );
    checkBox->SetValue( m_prefs->savePlaylistOnExit ? B_CONTROL_ON
                                                    : B_CONTROL_OFF );
    checkBox->ResizeToPreferred();
    dummy->AddChild( checkBox );

    rect = checkBox->Frame();
    rect.top = rect.bottom + 10;
    checkBox = new BCheckBox( rect, "PlayImmediately",
                              "By default queue tracks rather than play "
                              "them immediately",
                              NULL,
                              B_FOLLOW_LEFT | B_FOLLOW_TOP );
    checkBox->SetValue( m_prefs->playImmediately ? B_CONTROL_ON
                                                 : B_CONTROL_OFF );
    checkBox->ResizeToPreferred();
    dummy->AddChild( checkBox );

    dummy->ResizeTo( dummy->Bounds().Width(), checkBox->Frame().bottom );

    box->SetContentView( dummy );

    pane->AddChild( box );

    return pane;
}

BView*
PrefViews::BuildThemesPane( BRect frame, const char* name )
{
    BView* pane;

    pane = new BView( frame, name, B_FOLLOW_ALL, B_WILL_DRAW );
    pane->SetViewColor( ui_color( B_PANEL_BACKGROUND_COLOR ) );

    string current;
    map<string, string> themeList;
    map<string, string>::iterator i;
    int32 index;

    m_prefs->themeManager->GetCurrentTheme( current );


    return pane;
}

const float BoxView::MARGIN = 5;

BoxView::BoxView( BRect frame, const char* name, BView* view,
                  uint32 resizingMode )
:   BBox( frame, name, resizingMode )
{
    SetContentView( view );
}

BoxView::~BoxView()
{
}

void
BoxView::SetContentView( BView* view )
{
    if ( !view ) return;

    font_height fontHeight;
    GetFontHeight( &fontHeight );
    view->MoveBy( MARGIN, fontHeight.ascent + fontHeight.descent );
    view->ResizeTo( ContentWidth(), view->Frame().Height() );
    AddChild( view );

    ResizeTo( Frame().Width(), view->Frame().bottom + MARGIN );
}

float
BoxView::ContentWidth( void ) const
{
    return Bounds().Width() - MARGIN * 2;
}

// vi: set ts=4:

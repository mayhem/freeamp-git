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

#include "InfoEditor.h"
#include "playlist.h"
#include "metadata.h"
#include <be/interface/View.h>
#include <be/interface/TextControl.h>
#define DEBUG 1
#include <be/support/Debug.h>
#include <vector>

using namespace std;

const float GRID_UNIT = 10;

BTextControl* add_text_control( BView* parent, BRect* rect, const char* name,
                                const char* label, const char* text,
                                float* labelWidth )
{
    float w, h;
    BTextControl* ctrl;

    ctrl = new BTextControl( *rect, name, label, text, NULL,
                             B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP );
    ctrl->GetPreferredSize( &w, &h );
    ctrl->ResizeTo( rect->Width(), h );
    parent->AddChild( ctrl );
    *labelWidth = ctrl->StringWidth( ctrl->Label() );
    *rect = ctrl->Frame();
    rect->top = rect->bottom + GRID_UNIT;

    return ctrl;
}

InfoEditor::InfoEditor( BRect frame, const char* title, PlaylistItem* item,
                        window_look look, window_feel feel, uint32 flags )
:   BWindow( frame, title, look, feel, flags ),
    m_item( item )
{
    BRect rect( Bounds() );

    BView* root = new BView( rect, "Root", B_FOLLOW_ALL, B_WILL_DRAW );
    root->SetViewColor( ui_color( B_PANEL_BACKGROUND_COLOR ) );
    AddChild( root );

    vector<BTextControl*> ctrls;

    const MetaData& md = m_item->GetMetaData();
    float labelWidth, maxLabelWidth = 0;
    rect.InsetBy( GRID_UNIT, GRID_UNIT );

    ctrls.push_back( add_text_control( root, &rect, "Title", "Song Title:",
                                       md.Title().c_str(), &labelWidth ) );
    maxLabelWidth = MAX( maxLabelWidth, labelWidth );
    ctrls.push_back( add_text_control( root, &rect, "Artist", "Artist:",
                                       md.Artist().c_str(), &labelWidth ) );
    maxLabelWidth = MAX( maxLabelWidth, labelWidth );
    ctrls.push_back( add_text_control( root, &rect, "Album", "Album:",
                                       md.Album().c_str(), &labelWidth ) );
    maxLabelWidth = MAX( maxLabelWidth, labelWidth );
    char buf[10];
    uint32 val = md.Year() > 9999 ? 0 : md.Year();
    sprintf( buf, "%ld", val );
    ctrls.push_back( add_text_control( root, &rect, "Year", "Year:",
                                       buf, &labelWidth ) );
    maxLabelWidth = MAX( maxLabelWidth, labelWidth );
    ctrls.push_back( add_text_control( root, &rect, "Genre", "Genre:",
                                       md.Genre().c_str(), &labelWidth ) );
    maxLabelWidth = MAX( maxLabelWidth, labelWidth );
    val = md.Track() > 32765 ? 0 : md.Track();
    sprintf( buf, "%ld", val );
    ctrls.push_back( add_text_control( root, &rect, "Track", "Track:",
                                       buf, &labelWidth ) );
    maxLabelWidth = MAX( maxLabelWidth, labelWidth );

    vector<BTextControl*>::iterator i;
    for ( i = ctrls.begin(); i != ctrls.end(); i++ )
    {
        (*i)->SetDivider( maxLabelWidth + 0.5 * GRID_UNIT );
    }
}

InfoEditor::~InfoEditor()
{
}

void
InfoEditor::MessageReceived( BMessage* message )
{
    switch ( message->what )
    {
        default:
            BWindow::MessageReceived( message );
            break;
    }
}

bool
InfoEditor::QuitRequested( void )
{
    return true;
}

// vi: set ts=4:


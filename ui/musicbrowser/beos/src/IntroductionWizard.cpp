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

#include "config.h"
#include "IntroductionWizard.h"
#include "BeOSMusicBrowser.h"
#include "MusicBrowserResources.h"
#include "ResourceManager.h"
#include "MusicBrowserUI.h"
#include "musiccatalog.h"
#include <be/interface/View.h>
#include <be/interface/Bitmap.h>
#include <be/interface/TextView.h>
#include <be/interface/TextControl.h>
#include <be/interface/Button.h>
#include <be/translation/TranslatorFormats.h>
#include <be/support/TypeConstants.h>
#include <be/support/String.h>
#define DEBUG 1
#include <be/support/Debug.h>
#include <assert.h>
#include <string>

class BitmapView : public BView
{
public:
                    BitmapView( BRect frame, const char* name, BBitmap* bitmap,
                                uint32 resizingMode = B_FOLLOW_NONE,
                                uint32 flags = B_WILL_DRAW )
                    :   BView( frame, name, resizingMode, flags ),
                        m_bitmap( bitmap ) // takes ownership
                    {
                        ResizeTo( m_bitmap->Bounds().Width(),
                                  m_bitmap->Bounds().Height() );
                    }

    virtual         ~BitmapView()
                    {
                        delete m_bitmap;
                    }

    virtual void    Draw( BRect updateRect )
                    {
                        DrawBitmapAsync( m_bitmap );
                    }

private:
    BBitmap*        m_bitmap;
};

class SimpleTextView : public BTextView
{
public:
                    SimpleTextView( BRect frame, const char* name,
                                    uint32 resizingMode = B_FOLLOW_NONE,
                                    uint32 flags = B_WILL_DRAW )
                    :   BTextView( frame, name, frame, resizingMode, flags )
                    {
                        frame.OffsetTo( B_ORIGIN );
                        SetTextRect( frame );
                    }

    virtual         ~SimpleTextView()
                    {
                    }

    virtual void    GetPreferredSize( float* width, float* height )
                    {
                        *width = Bounds().Width();
                        *height = TextHeight( 0, TextLength() - 1 );
                    }
};

const float MARGIN = 10.0;
const uint32 MSG_WIZARD_NEXT           = 'next';
const uint32 MSG_WIZARD_BACK           = 'back';
const uint32 MSG_WIZARD_START_SEARCH   = 'strt';
const uint32 MSG_WIZARD_STOP_SEARCH    = 'stop';

IntroductionWizard::IntroductionWizard( BRect frame, const char* title,
                                        MusicBrowserUI* ui, FAContext* context )
:   BWindow( frame, title, B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, 0 ),
    m_ui( ui ),
    m_context( context ),
    m_rsrcs( "musicbrowser.ui" ),
    m_currentPage( 0 )
{
    BRect rect( Bounds() );

    BView* topRoot = new BView( rect, "TopRoot", B_FOLLOW_ALL, B_WILL_DRAW );
    topRoot->SetViewColor( ui_color( B_PANEL_BACKGROUND_COLOR ) );
    AddChild( topRoot );

    BBitmap* bitmap = m_rsrcs.GetBitmap( B_TRANSLATOR_BITMAP,
                                         WIZARD_IMAGE );
    BitmapView* pic = new BitmapView( BRect( MARGIN, MARGIN, 0, 0 ),
                                      "WizardPic", bitmap );
    AddChild( pic );

    rect = Bounds();
    rect.left = pic->Frame().right + 1;
    rect.InsetBy( MARGIN, MARGIN );
    BView* pageRoot = new BView( rect, "PageRoot", B_FOLLOW_NONE, B_WILL_DRAW );
    pageRoot->SetViewColor( topRoot->ViewColor() );
    topRoot->AddChild( pageRoot );

    float height;
    BuildIntroPage( pageRoot, &height );

    // Adjust page height.
    pageRoot->ResizeTo( pageRoot->Bounds().Width(), height );

    // Back/Next buttons.
    m_next = new BButton( rect, "Next", "Next",
                          new BMessage( MSG_WIZARD_NEXT ) );
    m_next->ResizeToPreferred();
    m_next->MoveTo( topRoot->Bounds().right - m_next->Bounds().Width() - MARGIN,
                    pageRoot->Frame().bottom + MARGIN );
    topRoot->AddChild( m_next );

    m_back = new BButton( rect, "Back", "Back",
                          new BMessage( MSG_WIZARD_BACK ) );
    m_back->ResizeToPreferred();
    m_back->MoveTo( m_next->Frame().left - m_back->Bounds().Width() - MARGIN,
                    m_next->Frame().top );
    m_back->SetEnabled( false );
    topRoot->AddChild( m_back );

    ResizeTo( Bounds().Width(), m_back->Frame().bottom + MARGIN );
}

IntroductionWizard::~IntroductionWizard()
{
}

void
IntroductionWizard::MessageReceived( BMessage* message )
{
    switch ( message->what )
    {
        case MSG_WIZARD_NEXT:
            GotoPage( m_currentPage + 1 );
            break;
        case MSG_WIZARD_BACK:
            GotoPage( m_currentPage - 1 );
            break;
        case MSG_WIZARD_START_SEARCH:
        {
            BView* pageTop = FindView( "TopRoot" )->FindView( "PageRoot" );
            if ( !pageTop ) break;
            BTextControl* folder = dynamic_cast<BTextControl*>
                                   ( pageTop->FindView( "SearchDirectory" ) );
            if ( !folder ) break;
            PRINT(( "Start search in %s\n", folder->Text() ));
            m_ui->StartSearch( folder->Text() );
            m_next->SetLabel( "Stop" );
            m_next->SetMessage( new BMessage( MSG_WIZARD_STOP_SEARCH ) );
            break;
        }
        case MSG_WIZARD_STOP_SEARCH:
            m_context->catalog->StopSearchMusic();
            break;
        case MBMSG_SEARCH_MUSIC_DONE:
            if ( m_currentPage == 1 )
            {
                m_next->SetLabel( "Start" );
                m_next->SetMessage( new BMessage( MSG_WIZARD_START_SEARCH ) );
            }
            break;
        default:
            BWindow::MessageReceived( message );
            break;
    }
}

void
IntroductionWizard::Quit( void )
{
    m_ui->MainBrowser()->PostMessage( MBMSG_WIZARD_GONE );
    BWindow::Quit();
}

status_t
IntroductionWizard::GotoPage( int32 page )
{
    if ( page < 0 || page > 1 ) return B_BAD_VALUE;

    BView* topRoot = FindView( "TopRoot" );
    assert( topRoot );
    BView* oldPageRoot = topRoot->FindView( "PageRoot" );
    assert( oldPageRoot );

    BView* newPageRoot = new BView( oldPageRoot->Frame(), "PageRoot",
                                    oldPageRoot->ResizingMode(),
                                    oldPageRoot->Flags() );
    newPageRoot->SetViewColor( oldPageRoot->ViewColor() );

    m_next->SetEnabled( true );
    m_next->SetLabel( "Next" );
    m_next->SetMessage( new BMessage( MSG_WIZARD_NEXT ) );
    m_back->SetEnabled( true );
    m_back->SetLabel( "Back" );
    m_back->SetMessage( new BMessage( MSG_WIZARD_BACK ) );
    switch ( page )
    {
        case 0:
            BuildIntroPage( newPageRoot );
            m_back->SetEnabled( false );
            break;
        case 1:
            BuildSearchPage( newPageRoot );
            m_next->SetLabel( "Start" );
            m_next->SetMessage( new BMessage( MSG_WIZARD_START_SEARCH ) );
            break;
        default:
            // NOT REACHED.
            break;
    }

    topRoot->RemoveChild( oldPageRoot );
    delete oldPageRoot;
    topRoot->AddChild( newPageRoot );
    m_currentPage = page;
}

void
IntroductionWizard::BuildIntroPage( BView* root, float* height )
{
    BRect rect;

    // Intro text.
    const char* text = m_rsrcs.GetString( WIZARD_INTRO_PAGE_TEXT );
    BString* parsedText = SimpleParse( text );

    rect = root->Bounds();
    BRect textRect( rect );
    textRect.OffsetTo( B_ORIGIN );
    SimpleTextView* textView = new SimpleTextView( rect, "Text" );
    textView->SetViewColor( ui_color( B_PANEL_BACKGROUND_COLOR ) );
    textView->SetText( parsedText->String() );
    textView->ResizeToPreferred();
    root->AddChild( textView );

    delete parsedText;

    if ( height )
    {
        *height = textView->Frame().Height();
    }
}

void
IntroductionWizard::BuildSearchPage( BView* root )
{
    const char* text = m_rsrcs.GetString( WIZARD_SEARCH_PAGE_TEXT );
    BString* parsedText = SimpleParse( text );

    BRect rect( root->Bounds() );
    SimpleTextView* textView = new SimpleTextView( rect, "Text" );
    textView->SetViewColor( ui_color( B_PANEL_BACKGROUND_COLOR ) );
    textView->SetText( parsedText->String() );
    textView->ResizeToPreferred();
    root->AddChild( textView );

    delete parsedText;

    // Folder control.
    float w, h;
    rect = root->Bounds();
    BTextControl* folder
        = new BTextControl( rect, "SearchDirectory",
                            "Look for music in ", "/boot/home/",
                            NULL );
    folder->SetModificationMessage( NULL );
    folder->GetPreferredSize( &w, &h );
    folder->ResizeTo( rect.Width(), h );
    folder->MoveTo( folder->Frame().left,
                    rect.bottom - folder->Frame().Height() );
    folder->SetViewColor( ui_color( B_PANEL_BACKGROUND_COLOR ) );
    folder->SetLowColor( folder->ViewColor() );
    folder->SetDivider( folder->StringWidth( folder->Label() ) );
    root->AddChild( folder );
}

BString*
IntroductionWizard::SimpleParse( const char* src )
{
    BString* dest = new BString( src );
    dest->ReplaceAll( "<the_BRANDING/>", the_BRANDING );
}

// vi: set ts=4:

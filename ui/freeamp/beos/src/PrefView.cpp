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

#include "PrefView.h"
#include "preferences.h"
#include "ThemeManager.h"
#include "facontext.h"
#include "event.h"
#include <be/interface/Window.h>
#include <be/interface/TabView.h>
#include <be/interface/Button.h>
#include <be/interface/TextView.h>
#include <be/interface/Box.h>
#include <be/interface/TextControl.h>
#include <be/interface/CheckBox.h>
#include <be/interface/RadioButton.h>
#include <be/interface/ListView.h>
#include <be/interface/ScrollView.h>
#include <be/interface/Slider.h>
#include <be/storage/FilePanel.h>
#include <be/storage/Entry.h>
#include <be/storage/Path.h>
#define DEBUG 1
#include <be/support/Debug.h>
#include <string>
#include <map>

const char* FREEAMP_ABOUT_TEXT =
    "FreeAmp is an Open Source effort to build the best digital audio player "
    "available.  In the interest of supporting the free software community, "
    "while at the same time fostering the growth of the online delivery of "
    "music, EMusic.com, THE Source for Downloadable Music, is funding both "
    "the FreeAmp.org domain and the efforts of the FreeAmp team.  The FreeAmp "
    "team consists of:\n\n"
    "Mark B. Elrod, Robert Kaye, Isaac Richards, Brett Thomas, and "
    "Jason Woodward.\n\n"
    "Other people have also contributed to FreeAmp:\n\n"
    "William Bull, Alan Cutter, Gabor Fleischer, Jean-Michel HERVE, "
    "Hiromasa Kato, Michael Bruun Petersen, Sylvain Rebaud, "
    "The Snowblind Alliance, Tom Spindler, Valters Vingolds, Bill Yuan."; 

const float GRID_UNIT = 10.0;
const float BOX_INSET = 13.0;

// Message constants
const uint32 MSG_OK = 'okay';
const uint32 MSG_CANCEL = 'cncl';
const uint32 MSG_APPLY = 'aply';
const uint32 MSG_THEME_SELECTED = 'thms';
const uint32 MSG_BROWSE_DIR = 'brws';
const uint32 MSG_SET_MYMUSIC_DIR = 'mmdr';
const uint32 MSG_SET_CHECK_FOR_UPDATES = 'cfud';
const uint32 MSG_SET_RECLAIM_FILETYPES = 'rclm';
const uint32 MSG_SET_ASK_RECLAIM_FILETYPES = 'askr';
const uint32 MSG_SET_TEXT_ONLY = 'txtl';
const uint32 MSG_SET_IMAGES_ONLY = 'imgs';
const uint32 MSG_SET_TEXT_AND_IMAGES = 'txim';
const uint32 MSG_SET_SAVE_PLAYLIST_ON_EXIT = 'spoe';
const uint32 MSG_SET_PLAY_IMMEDIATELY = 'plyi';
const uint32 MSG_SET_DECODER_PRIORITY = 'dcdr';
const uint32 MSG_SET_INPUT_BUFFER_SIZE = 'inpb';
const uint32 MSG_SET_OUTPUT_BUFFER_SIZE = 'outb';
const uint32 MSG_SET_PRE_BUFFER_LENGTH = 'preb';

PrefView::PrefView( FAContext* context, ThemeManager* themeMan,
                    BRect frame, const char* name )
:   BView( frame, name, B_FOLLOW_ALL, B_WILL_DRAW ),
    m_prefs( context->prefs ),
    m_context( context ),
    m_themeMan( themeMan )
{
    GetPrefsValues( m_prefs, &m_originalValues );
    m_proposedValues = m_currentValues = m_originalValues;

    SetViewColor( ui_color( B_PANEL_BACKGROUND_COLOR ) );

    // Buttons
    float buttonWidth, buttonHeight;
    BRect buttonRect( 0, 0, 100, 50 );

    BButton* okButton = new BButton( buttonRect, "OKButton", "OK",
                                     new BMessage( MSG_OK ),
                                     B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM );
    BButton* cancelButton = new BButton( buttonRect, "CancelButton", "Cancel",
                                         new BMessage( MSG_CANCEL ),
                                         B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM );
    BButton* applyButton = new BButton( buttonRect, "ApplyButton", "Apply",
                                        new BMessage( MSG_APPLY ),
                                        B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM );
    okButton->ResizeToPreferred();
    cancelButton->ResizeToPreferred();
    applyButton->ResizeToPreferred();
    cancelButton->GetPreferredSize( &buttonWidth, &buttonHeight );
    okButton->MoveTo( Bounds().right - buttonWidth - 2 * GRID_UNIT,
                      Bounds().bottom - buttonHeight - GRID_UNIT );
    applyButton->MoveTo( okButton->Frame().left - buttonWidth - GRID_UNIT,
                         okButton->Frame().top );
    cancelButton->MoveTo( applyButton->Frame().left - buttonWidth - GRID_UNIT,
                          applyButton->Frame().top );
    okButton->MakeDefault( true );
    AddChild( okButton );
    AddChild( applyButton );
    AddChild( cancelButton );
    m_controls.push_back( okButton );
    m_controls.push_back( applyButton );
    m_controls.push_back( cancelButton );

    // Tab container
    BRect r( Bounds() );
    r.bottom -= buttonHeight + 2 * GRID_UNIT;
    PrefTabView* tabView = new PrefTabView( r, "TabView" );

    // Tabbed panes
    r.InsetBy( 5, 5 );
    r.bottom -= tabView->TabHeight();

    m_general = new PrefPane( r, "General" );
    tabView->AddTab( m_general );
    m_themes = new PrefPane( r, "Themes" );
    tabView->AddTab( m_themes );
    m_advanced = new PrefPane( r, "Advanced" );
    tabView->AddTab( m_advanced );
    m_about = new PrefPane( r, "About" );
    tabView->AddTab(m_about);

    CreateGeneralPane();
    CreateThemesPane();
    CreateAdvancedPane();
    CreateAboutPane();

    AddChild( tabView );
}

PrefView::~PrefView()
{
}

void
PrefView::AttachedToWindow( void )
{
    BView::AttachedToWindow();
    m_about->FindView( "AboutText" )
           ->SetViewColor( ui_color( B_PANEL_BACKGROUND_COLOR ) );
    ((BListView*)m_themes->FindView( "ThemeList" ))->SetTarget( this );

    vector<BControl*>::iterator i;
    for ( i = m_controls.begin(); i != m_controls.end(); i++ )
    {
        printf( "setting up the target for %s\n", (*i)->Name() );
        (*i)->SetTarget( this );
    }
}

void
PrefView::MessageReceived( BMessage* message )
{
    switch ( message->what )
    {
    case MSG_OK:
        Apply();
        Window()->PostMessage( B_QUIT_REQUESTED );
        break;
    case MSG_APPLY:
        Apply();
        break;
    case MSG_CANCEL:
        Window()->PostMessage( B_QUIT_REQUESTED );
        break;
    case MSG_BROWSE_DIR:
        {
            if ( !m_filePanel )
            {
                PRINT(( "constructing BFilePanel\n" ));
                BMessage reply( MSG_SET_MYMUSIC_DIR );
                m_filePanel = new BFilePanel(
                                B_OPEN_PANEL,
                                new BMessenger( this ),
                                NULL,
                                B_DIRECTORY_NODE, // node_flavors,
                                false,            // allow_multiple_selection
                                &reply            // message is set below
                                );
            }
            m_filePanel->Show();
        }
        break;
    case MSG_SET_MYMUSIC_DIR:
        {
            entry_ref ref;
            if ( message->FindRef( "refs", &ref ) < B_NO_ERROR ) break;
            BEntry entry( &ref );
            BPath path;
            entry.GetPath( &path );
            SetPrefString(kSaveMusicDirPref,  path.Path() );

            // need to delete file panel myself.
            delete m_filePanel;
            m_filePanel = NULL;
        }
        break;
    case MSG_SET_CHECK_FOR_UPDATES:
        message->PrintToStream();
        SetPrefBoolean(kCheckForUpdatesPref,  (message->FindInt32( "be:value" ) == B_CONTROL_ON) );
        break;
    case MSG_SET_RECLAIM_FILETYPES:
        m_proposedValues.reclaimFiletypes =
            ( message->FindInt32( "be:value" ) == B_CONTROL_ON );
        break;
    case MSG_SET_ASK_RECLAIM_FILETYPES:
        m_proposedValues.askReclaimFiletypes =
            ( message->FindInt32( "be:value" ) == B_CONTROL_ON );
        printf( "val = %d\n", m_proposedValues.askReclaimFiletypes );
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
    case MSG_SET_SAVE_PLAYLIST_ON_EXIT:
        m_proposedValues.savePlaylistOnExit =
            ( message->FindInt32( "be:value" ) == B_CONTROL_ON );
        break;
    case MSG_SET_PLAY_IMMEDIATELY:
        m_proposedValues.playImmediately =
            ( message->FindInt32( "be:value" ) == B_CONTROL_ON );
        break;
    case MSG_THEME_SELECTED:
        {
            BListView* list;
            if ( message->FindPointer( "source", (void**)&list ) == B_OK )
            {
                BStringItem* item =
                    (BStringItem*)list->ItemAt( list->CurrentSelection() );
                m_proposedValues.currentTheme = item->Text();
            }
        }
        break;
    case MSG_SET_INPUT_BUFFER_SIZE:
        {
            BTextControl* ctrl;
            int32 size = m_originalValues.inputBufferSize;;
            if ( message->FindPointer( "source", (void**)&ctrl ) < B_OK ) break;
            sscanf( ctrl->Text(), "%ld", &size );
            m_proposedValues.inputBufferSize = size;
        }
        break;
    case MSG_SET_OUTPUT_BUFFER_SIZE:
        {
            BTextControl* ctrl;
            int32 size = m_originalValues.outputBufferSize;;
            if ( message->FindPointer( "source", (void**)&ctrl ) < B_OK ) break;
            sscanf( ctrl->Text(), "%ld", &size );
            m_proposedValues.outputBufferSize = size;
        }
        break;
    case MSG_SET_DECODER_PRIORITY:
        SetPrefInt32(kDecoderThreadPriorityPref,  message->FindInt32( "be:value" ) );
        break;
    default:
        BView::MessageReceived( message );
        break;
    }
}

void
PrefView::Apply( void )
{
    if ( m_proposedValues != m_currentValues )
    {
       SavePrefsValues( m_prefs, &m_proposedValues );
    }
}

void
PrefView::GetPrefsValues(Preferences* prefs, 
                         PrefsStruct* values)
{
    uint32 bufferSize = 1;
    uint32 size;

    // get the string prefs
    char* buffer = (char*)malloc(bufferSize);

    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetPrefString(kPMOPref, buffer, &size))
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetPrefString(kPMOPref, buffer, &size);
    }

    values->defaultPMO = buffer;
    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetPrefString(kUIPref, buffer, &size))
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetPrefString(kUIPref, buffer, &size);
    }
    
    values->defaultUI = buffer;
    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetPrefString(kProxyHostPref, buffer, &size))
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetPrefString(kProxyHostPref, buffer, &size);
    }

    values->proxyServer = buffer;
    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetPrefString(kSaveStreamsDirPref, buffer, &size))
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetPrefString(kSaveStreamsDirPref, buffer, &size);
    }

    values->saveStreamsDirectory = buffer;
    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetPrefString(kAlternateNICAddressPref, buffer, &size))
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetPrefString(kAlternateNICAddressPref, buffer, &size);
    }

    values->alternateIP = buffer;
    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetPrefString(kThemeDefaultFontPref, buffer, &size))
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetPrefString(kThemeDefaultFontPref, buffer, &size);
    }

    values->defaultFont = buffer;
    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetPrefString(kSaveMusicDirPref, buffer, &size))
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetPrefString(kSaveMusicDirPref, buffer, &size);
    }

    values->saveMusicDirectory = buffer;
    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetPrefString(kUsersPortablePlayersPref, buffer, &size))
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetPrefString(kUsersPortablePlayersPref, buffer, &size);
    }

    char* cp = buffer;
    char* name = cp;

    while((cp = strchr(cp, ';')))
    {
        *cp = 0x00;
        values->portablePlayers.insert(string(name));
        //MessageBox(NULL, name, "name", MB_OK);

        cp++;
        name = cp;
    }

    if(*name)
    {
        values->portablePlayers.insert(string(name));
        //MessageBox(NULL, name, "name", MB_OK);
    }

    //values->portablePlayers = buffer;
    size = bufferSize;

    m_themeMan->GetCurrentTheme(values->currentTheme);

    // get the other prefs
    prefs->GetPrefInt32(kDecoderThreadPriorityPref, &values->decoderThreadPriority);
    prefs->GetPrefInt32(kInputBufferSizePref, &values->inputBufferSize);
    prefs->GetPrefInt32(kOutputBufferSizePref, &values->outputBufferSize);
    prefs->GetPrefInt32(kPreBufferPref, &values->preBufferLength);
    prefs->GetPrefBoolean(kStayOnTopPref, &values->stayOnTop);
    prefs->GetPrefBoolean(kLiveInTrayPref, &values->liveInTray);
    prefs->GetPrefInt32(kStreamBufferIntervalPref, &values->streamInterval);
    prefs->GetPrefBoolean(kSaveStreamsPref, &values->saveStreams);
    prefs->GetPrefBoolean(kUseProxyPref, &values->useProxyServer);
    prefs->GetPrefBoolean(kUseAlternateNICPref, &values->useAlternateIP);
    prefs->GetPrefBoolean(kUseDebugLogPref, &values->enableLogging);
    prefs->GetPrefBoolean(kLogMainPref, &values->logMain);
    prefs->GetPrefBoolean(kLogDecodePref, &values->logDecoder);
    prefs->GetPrefBoolean(kLogInputPref, &values->logInput);
    prefs->GetPrefBoolean(kLogOutputPref, &values->logOutput);
    prefs->GetPrefBoolean(kLogPerformancePref, &values->logPerformance);
    prefs->GetPrefBoolean(kCheckForUpdatesPref, &values->checkForUpdates);
    prefs->GetPrefBoolean(kAskToReclaimFiletypesPref, &values->askReclaimFiletypes);
    prefs->GetPrefBoolean(kReclaimFiletypesPref, &values->reclaimFiletypes);
    prefs->GetPrefBoolean(kShowToolbarTextLabelsPref, &values->useTextLabels);
    prefs->GetPrefBoolean(kShowToolbarImagesPref, &values->useImages);
    prefs->GetPrefBoolean(kSaveCurrentPlaylistOnExitPref, &values->savePlaylistOnExit);
    prefs->GetPrefBoolean(kPlayImmediatelyPref,  &values->playImmediately );

    free(buffer);
}

void
PrefView::SavePrefsValues( Preferences* prefs,
                           PrefsStruct*  values )
{
    prefs->SetPrefString(kPMOPref, values->defaultPMO.c_str());
    prefs->SetPrefString(kUIPref, values->defaultUI.c_str());
    prefs->SetPrefInt32(kDecoderThreadPriorityPref, values->decoderThreadPriority);
    prefs->SetPrefInt32(kInputBufferSizePref, values->inputBufferSize);
    prefs->SetPrefInt32(kOutputBufferSizePref, values->outputBufferSize);
    prefs->SetPrefInt32(kPreBufferPref, values->preBufferLength);
    prefs->SetPrefBoolean(kStayOnTopPref, values->stayOnTop);
    prefs->SetPrefBoolean(kLiveInTrayPref, values->liveInTray);

    prefs->SetPrefInt32(kStreamBufferIntervalPref, values->streamInterval);
    prefs->SetPrefBoolean(kSaveStreamsPref, values->saveStreams);
    prefs->SetPrefString(kSaveStreamsDirPref, values->saveStreamsDirectory.c_str());
    prefs->SetPrefString(kProxyHostPref, values->proxyServer.c_str());
    prefs->SetPrefBoolean(kUseProxyPref, values->useProxyServer);
    prefs->SetPrefString(kAlternateNICAddressPref, values->alternateIP.c_str());
    prefs->SetPrefBoolean(kUseAlternateNICPref, values->useAlternateIP);

    prefs->SetPrefBoolean(kUseDebugLogPref, values->enableLogging);
    prefs->SetPrefBoolean(kLogMainPref, values->logMain);
    prefs->SetPrefBoolean(kLogDecodePref, values->logDecoder);
    prefs->SetPrefBoolean(kLogInputPref, values->logInput);
    prefs->SetPrefBoolean(kLogOutputPref, values->logOutput);
    prefs->SetPrefBoolean(kLogPerformancePref, values->logPerformance);

    prefs->SetPrefString(kThemeDefaultFontPref, values->defaultFont.c_str());
    if (m_themeList[values->currentTheme].length() > 0)
       m_themeMan->UseTheme(m_themeList[values->currentTheme]);

    prefs->SetPrefBoolean(kCheckForUpdatesPref, values->checkForUpdates);
    prefs->SetPrefString(kSaveMusicDirPref, values->saveMusicDirectory.c_str());
    prefs->SetPrefBoolean(kAskToReclaimFiletypesPref, values->askReclaimFiletypes);
    prefs->SetPrefBoolean(kReclaimFiletypesPref, values->reclaimFiletypes);

    PortableSet::const_iterator i;
    string portableList;

    for(i = values->portablePlayers.begin(); i != values->portablePlayers.end(); i++)
    {
        portableList += *i;
        portableList += ";";
    }

    prefs->SetPrefString(kUsersPortablePlayersPref, portableList.c_str());
    prefs->SetPrefBoolean(kShowToolbarTextLabelsPref, values->useTextLabels);
    prefs->SetPrefBoolean(kShowToolbarImagesPref, values->useImages);

    prefs->SetPrefBoolean(kSaveCurrentPlaylistOnExitPref, values->savePlaylistOnExit);
    prefs->SetPrefBoolean(kPlayImmediatelyPref,  values->playImmediately );

    // this gets called by each page unfortunately
    // so save some effort by only doing it once
    if(*values != m_currentValues) 
    {
        PRINT(( "PrefsChanged:\n" ));
        m_context->target->AcceptEvent(new Event(INFO_PrefsChanged));
        m_currentValues = m_proposedValues = *values;
    }
}

void
PrefView::CreateGeneralPane( void )
{
    BBox* box;
    BButton* button;
    BRect rect;
    BRect bounds( m_general->Bounds() );
    BRect rect1( bounds );
    rect1.InsetBy( GRID_UNIT, GRID_UNIT );
    float w, h;

    // --------------------- Save Music Folder Box ----------------------------
    box = new BBox( rect1, "SaveMusicFolderBox",
                    B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP );
    box->SetLabel( "Save Music Folder" );
    button = new BButton( BRect(0,0,0,0), "Browse", "Browse"B_UTF8_ELLIPSIS,
                          new BMessage( MSG_BROWSE_DIR ),
                          B_FOLLOW_RIGHT|B_FOLLOW_TOP );
    button->ResizeToPreferred();
    button->GetPreferredSize( &w, &h );
    button->MoveTo( rect1.right - GRID_UNIT - w, rect1.top + GRID_UNIT );
    m_general->AddChild( button );
    m_controls.push_back( button );

    rect.Set( 0, 0, button->Frame().left, 0 );
    rect.InsetBy( GRID_UNIT, GRID_UNIT );
    m_saveMusicFolder = new BTextControl( rect, "SaveMusicFolder",
                                          NULL, NULL, NULL,
                                          B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP );
    m_saveMusicFolder->SetText( m_originalValues.saveMusicDirectory.c_str() );
    m_saveMusicFolder->GetPreferredSize( &w, &h );
    rect1.bottom = rect1.top + h + 2 * GRID_UNIT;
    box->ResizeTo( rect1.Width(), rect1.Height() );
    box->AddChild( m_saveMusicFolder );
    m_general->AddChild( box );

    // ------------------ Update Manager box ---------------------------------
    rect1.top = rect1.bottom + GRID_UNIT;
    box = new BBox( rect1, "UpdateManager", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP );
    box->SetLabel( "Update Manager" );

    rect1 = box->Bounds();
    rect1.InsetBy( BOX_INSET, BOX_INSET );

    // Check for updates
    BCheckBox* checkBox;
    checkBox = new BCheckBox( rect1, "CheckForUpdates", "Check for updates",
                              new BMessage( MSG_SET_CHECK_FOR_UPDATES ),
                              B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP );
    checkBox->SetValue( m_originalValues.checkForUpdates ? B_CONTROL_ON
                                                         : B_CONTROL_OFF );
    box->AddChild( checkBox );
    m_controls.push_back( checkBox );
    rect1 = checkBox->Frame();

    box->ResizeTo( box->Frame().Width(), rect1.bottom + BOX_INSET );

    m_general->AddChild( box );
    rect1 = box->Frame();

    // ------------------- File Association box ----------------------------
    rect1.top = rect1.bottom + GRID_UNIT;
    box = new BBox( rect1, "FileAssociationBox",
                    B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP );
    box->SetLabel( "File Association" );

    rect1 = box->Bounds();
    rect1.InsetBy( BOX_INSET, BOX_INSET );

    // Reclaim file types
    checkBox = new BCheckBox( rect1, "ReclaimFileTypes", "Reclaim file types",
                              new BMessage( MSG_SET_RECLAIM_FILETYPES ),
                              B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP );
    checkBox->SetValue( m_originalValues.reclaimFiletypes ? B_CONTROL_ON
                                                          : B_CONTROL_OFF );
    box->AddChild( checkBox );
    m_controls.push_back( checkBox );
    rect1 = checkBox->Frame();

    // Ask reclaim file types
    rect1.top = rect1.bottom + GRID_UNIT;
    checkBox = new BCheckBox( rect1, "AskToReclaimFiletypes",
                              "Ask to reclaim file types",
                              new BMessage( MSG_SET_ASK_RECLAIM_FILETYPES ),
                              B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP );
    checkBox->SetValue( m_originalValues.askReclaimFiletypes ? B_CONTROL_ON
                                                             : B_CONTROL_OFF );
    box->AddChild( checkBox );
    m_controls.push_back( checkBox );
    rect1 = checkBox->Frame();

    box->ResizeTo( box->Frame().Width(), rect1.bottom + BOX_INSET );
    m_general->AddChild( box );
    rect1 = box->Frame();

    // ------------- My Music Toolbar appearance box group ------------------
    rect1.top = rect1.bottom + GRID_UNIT;
    box = new BBox( rect1, "ToolbarAppearance",
                    B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP );
    box->SetLabel( "Show 'My Music' Toolbars As" );

    BRadioButton* radioButton;

    rect1 = box->Bounds();
    rect1.InsetBy( GRID_UNIT, GRID_UNIT );

    radioButton = new BRadioButton( rect1, "TextOnly", "Text Only",
                                    new BMessage( MSG_SET_TEXT_ONLY ) );
    radioButton->ResizeToPreferred();
    box->AddChild( radioButton );
    radioButton->SetValue( m_originalValues.useTextLabels ? B_CONTROL_ON
                                                          : B_CONTROL_OFF );
    m_controls.push_back( radioButton );

    radioButton->GetPreferredSize( &w, &h );
    rect1.left += w + GRID_UNIT;

    radioButton = new BRadioButton( rect1, "ImagesOnly", "Images Only",
                                    new BMessage( MSG_SET_IMAGES_ONLY ) );
    radioButton->ResizeToPreferred();
    box->AddChild( radioButton );
    radioButton->SetValue( m_originalValues.useImages ? B_CONTROL_ON
                                                      : B_CONTROL_OFF );
    m_controls.push_back( radioButton );

    radioButton->GetPreferredSize( &w, &h );
    rect1.left += w + GRID_UNIT;

    radioButton = new BRadioButton( rect1, "TextAndImages", "Text and Images",
                                    new BMessage( MSG_SET_TEXT_AND_IMAGES ) );
    radioButton->ResizeToPreferred();
    box->AddChild( radioButton );
    radioButton->SetValue( ( m_originalValues.useTextLabels &&
                             m_originalValues.useImages )
                           ? B_CONTROL_ON : B_CONTROL_OFF );
    m_controls.push_back( radioButton );

    box->ResizeTo( box->Frame().Width(), GRID_UNIT * 2 +h );

    m_general->AddChild( box );
    rect1 = box->Frame();

    // -------------- Miscellaneous box group -------------------------------
    rect1.top = rect1.bottom + GRID_UNIT;
    box = new BBox( rect1, "Miscellaneous",
                    B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP );
    box->SetLabel( "Miscellaneous" );

    // Save current playlist on exit
    rect1 = box->Bounds();
    rect1.InsetBy( GRID_UNIT, GRID_UNIT );
    checkBox = new BCheckBox( rect1, "SaveCurrentPlaylistOnExit",
                              "Save current playlist when exiting "
                              "the application",
                              new BMessage( MSG_SET_SAVE_PLAYLIST_ON_EXIT ),
                              B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP );
    checkBox->SetValue( m_originalValues.savePlaylistOnExit ? B_CONTROL_ON
                                                            : B_CONTROL_OFF );
    box->AddChild( checkBox );
    m_controls.push_back( checkBox );
    rect1 = checkBox->Frame();

    // Play immediately, or queue?
    rect1.top = rect1.bottom + GRID_UNIT;
    checkBox = new BCheckBox( rect1, "PlayImmediately",
                              "By default queue tracks rather than play "
                              "them immediately",
                              new BMessage( MSG_SET_PLAY_IMMEDIATELY ),
                              B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP );
    checkBox->SetValue( m_originalValues.playImmediately ? B_CONTROL_ON
                                                         : B_CONTROL_OFF );
    box->AddChild( checkBox );
    m_controls.push_back( checkBox );
    rect1 = checkBox->Frame();

    box->ResizeTo( box->Frame().Width(), rect1.bottom + GRID_UNIT );
    m_general->AddChild( box );
}

void
PrefView::CreateThemesPane( void )
{
    string current;
    map<string, string>::iterator i;
    int32 index;

    m_themeMan->GetCurrentTheme( current );
    PRINT(( "current theme %s\n", current.c_str() ));
    m_themeMan->GetThemeList( m_themeList );
    for ( i = m_themeList.begin(); i != m_themeList.end(); i++ )
    {
        PRINT(( "\"%s\"->\"%s\"\n", (*i).first.c_str(), (*i).second.c_str() ));
    }

    BRect rect( m_themes->Bounds() );
    rect.InsetBy( GRID_UNIT, GRID_UNIT );
    rect.right -= B_V_SCROLL_BAR_WIDTH;
    BListView* themeListView = new BListView( rect, "ThemeList",
                                              B_SINGLE_SELECTION_LIST,
                                              B_FOLLOW_ALL );
    themeListView->SetSelectionMessage( new BMessage( MSG_THEME_SELECTED ) );
    themeListView->SetInvocationMessage( new BMessage( MSG_THEME_SELECTED ) );
    index = 0;
    for ( i = m_themeList.begin(); i != m_themeList.end(); i++, index++ )
    {
        themeListView->AddItem( new BStringItem( (*i).first.c_str() ) );
        PRINT(( "adding %s, current theme %s\n", (*i).first.c_str(), current.c_str() ));
        if ( (*i).first == current )
        {
            themeListView->Select( index );
        }
    }
    m_themes->AddChild( new BScrollView( "Scroll", themeListView,
                                         B_FOLLOW_ALL, 0, false, true ) );
}
 
// ----------------------------------------------------
// Advanced settings, decoder prio, etc.
// ----------------------------------------------------
void
PrefView::CreateAdvancedPane( void )
{
    BRect rect( m_advanced->Bounds() );
    BBox* box;
    float w, h;

    // --------------------- Decoder box ---------------------------------
    rect.InsetBy( GRID_UNIT, GRID_UNIT );
    box = new BBox( rect, "DecoderThreadPriority",
                    B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP );
    box->SetLabel( "Decoder" );
    rect = box->Bounds();
    rect.InsetBy( BOX_INSET, BOX_INSET );

    // Decoder thread priority.
    BSlider* slider = new BSlider( rect, "DecoderPriority",
                                   "Decoder Thread Priority",
                                   new BMessage( MSG_SET_DECODER_PRIORITY ),
                                   2, 30, B_BLOCK_THUMB,
                                   B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP );
    slider->SetLimitLabels( "Low", "High" );
    slider->ResizeToPreferred();
    slider->SetValue( m_originalValues.decoderThreadPriority );
    box->AddChild( slider );
    m_controls.push_back( slider );
    rect = slider->Frame();

    box->ResizeTo( box->Frame().Width(), rect.bottom + GRID_UNIT );
    rect = box->Frame();
    m_advanced->AddChild( box );

    // ------------------- Buffer Size box ---------------------------
    rect.top = rect.bottom + GRID_UNIT;
    box = new BBox( rect, "BufferSizeBox", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP );
    box->SetLabel( "Buffer Sizes" );
    rect = box->Bounds();
    rect.InsetBy( BOX_INSET, BOX_INSET );

    // Input Buffer Size
    BTextControl* textCtrl;
    char buf[256];
    textCtrl = new BTextControl( rect, "InputBufferSize",
                                 "Input Buffer Size (Kilobytes)",
                                 "",
                                 new BMessage( MSG_SET_INPUT_BUFFER_SIZE ),
                                 B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP );
    textCtrl->SetModificationMessage(
        new BMessage( MSG_SET_INPUT_BUFFER_SIZE ) );
    sprintf( buf, "%ld", m_originalValues.inputBufferSize );
    textCtrl->SetText( buf );
    textCtrl->GetPreferredSize( &w, &h );
    box->AddChild( textCtrl );
    m_controls.push_back( textCtrl );
    rect = textCtrl->Frame();

    // Output Buffer Size
    rect.top = rect.bottom + GRID_UNIT;
    textCtrl = new BTextControl( rect, "OutputBufferSize",
                                 "Output Buffer Size (Kilobytes)",
                                 "",
                                 new BMessage( MSG_SET_OUTPUT_BUFFER_SIZE ),
                                 B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP );
    textCtrl->SetModificationMessage(
        new BMessage( MSG_SET_OUTPUT_BUFFER_SIZE ) );
    sprintf( buf, "%ld", m_originalValues.outputBufferSize );
    textCtrl->SetText( buf );
    textCtrl->GetPreferredSize( &w, &h );
    box->AddChild( textCtrl );
    m_controls.push_back( textCtrl );
    rect = textCtrl->Frame();

    // Pre-buffer Length
    rect.top = rect.bottom + GRID_UNIT;
    textCtrl = new BTextControl( rect, "PreBufferLength",
                                 "Pre-buffer Length (Seconds)",
                                 "",
                                 new BMessage( MSG_SET_PRE_BUFFER_LENGTH ),
                                 B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP );
    textCtrl->SetModificationMessage(
        new BMessage( MSG_SET_PRE_BUFFER_LENGTH ) );
    sprintf( buf, "%ld", m_originalValues.preBufferLength );
    textCtrl->SetText( buf );
    textCtrl->GetPreferredSize( &w, &h );
    box->AddChild( textCtrl );
    m_controls.push_back( textCtrl );
    rect = textCtrl->Frame();

    box->ResizeTo( box->Frame().Width(), rect.bottom + BOX_INSET );
    m_advanced->AddChild( box );
    rect = box->Frame();
}

// ----------------------------------------------------
// About pane
// ----------------------------------------------------
void
PrefView::CreateAboutPane( void )
{
    BRect r = m_about->Bounds();
    r.InsetBy( 10, 10 );
    BRect textRect = r;
    textRect.OffsetTo( B_ORIGIN );
    BTextView* textView = new BTextView( r, "AboutText", textRect,
                                         B_FOLLOW_NONE, B_WILL_DRAW );
    textView->MakeSelectable( false );
    textView->MakeEditable( false );
    textView->SetText( FREEAMP_ABOUT_TEXT );
    m_about->AddChild( textView );
}

// ----------------------------------------------------
// methods for setting pref values
// ----------------------------------------------------

void
PrefView::SetPrefString(kSaveMusicDirPref,  const char* path )
{
    m_proposedValues.saveMusicDirectory = string( path );
    m_saveMusicFolder->SetText( path );
}

void
PrefView::SetPrefBoolean(kCheckForUpdatesPref,  bool flag )
{
    m_proposedValues.checkForUpdates = flag;
}

void
PrefView::SetPrefInt32(kDecoderThreadPriorityPref,  int32 prio )
{
    m_proposedValues.decoderThreadPriority = prio;
}

// ---------------------------------------------------
// PrefTabView
// ---------------------------------------------------

PrefTabView::PrefTabView( BRect r, const char* name )
:   BTabView( r, name )
{
}

PrefTabView::~PrefTabView()
{
}

void
PrefTabView::Select( int32 tab )
{
    BTabView::Select( tab );

    BView* pane = TabAt( tab )->View();
    pane->ResizeTo( Bounds().Width()-10.0, Bounds().Height()-10.0-TabHeight() );
    pane->Invalidate();
}

PrefPane::PrefPane( BRect r, const char* name )
:   BView( r, name, B_FOLLOW_ALL, B_WILL_DRAW )
{
}

PrefPane::~PrefPane()
{
}

void
PrefPane::AttachedToWindow( void )
{
    SetViewColor( Parent()->ViewColor() );
    BView::AttachedToWindow();
}

// vi: set ts=4:

/*____________________________________________________________________________

   FreeAmp - The Free MP3 Player

   Copyright (C) 1999 EMusic

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

#include <stdio.h>
#define DEBUG 0
#include <be/support/Debug.h>
#include "Theme.h"
#include "BeOSWindow.h"
#include "BeOSCanvas.h"
#include "MainWindow.h"

#define CHECK_POINT CHECK_POINT_MSG("")
#define CHECK_POINT_MSG(a) PRINT(( "File %s Line %d: %s\n", __FILE__, __LINE__, a ))

BeOSWindow::BeOSWindow( Theme* pTheme, string& oName )
:   Window( pTheme, oName ),
    m_mainWindow( NULL ),
    m_canvasView( NULL )
{
    CHECK_POINT;
    m_pCanvas = new BeOSCanvas( this );
}

BeOSWindow::~BeOSWindow()
{
    CHECK_POINT_MSG( "~BeOSWindow" );
    delete m_pCanvas;
    m_pCanvas = NULL;
    CHECK_POINT;
}

Error
BeOSWindow::Run( Pos& oWindowPos )
{
    CHECK_POINT_MSG( "Run" );

    m_pCanvas->Init();

    Rect rect;
    GetCanvas()->GetBackgroundRect( rect );
    BRect brect( rect.x1, rect.y1, rect.x2, rect.y2 );
    brect.OffsetTo( oWindowPos.x, oWindowPos.y );
    m_mainWindow = new MainWindow( brect, m_oName.c_str() );

    // set the root view
    m_canvasView = ((BeOSCanvas*)GetCanvas())->GetBView();
    assert( m_canvasView );
    m_mainWindow->AddChild( m_canvasView );
    m_mainWindow->Show();

    Init();

    m_mainWindow->WaitForQuit();

    return kError_NoErr;
}

Error
BeOSWindow::Close( void )
{
    CHECK_POINT_MSG( "Close" );
    m_mainWindow->PostMessage( B_QUIT_REQUESTED );

    return kError_NoErr;
}

Error
BeOSWindow::Show( void )
{
    CHECK_POINT_MSG( "Show" );
    m_mainWindow->Show();

    return kError_NoErr;
}

Error
BeOSWindow::Hide( void )
{
    CHECK_POINT_MSG( "Hide" );
    m_mainWindow->Hide();

    return kError_NoErr;
}

Error
BeOSWindow::Enable( void )
{
    CHECK_POINT_MSG( "Enable" );
    return kError_NoErr;
}

Error
BeOSWindow::Disable( void )
{
    CHECK_POINT_MSG( "Disable" );
    return kError_NoErr;
}

Error
BeOSWindow::SetTitle( string& oTitle )
{
    CHECK_POINT_MSG( "SetTitle" );
    return kError_NoErr;
}

Error
BeOSWindow::CaptureMouse( bool bCapture )
{
    CHECK_POINT_MSG( "CaptureMouse" );

    // currently mouse tracking outside the window is turned on
    // in the CanvasView::MouseDown thru SetMouseEventMask.

    if ( bCapture )
    {
    }
    else
    {
    }

    return kError_NoErr;
}

Error
BeOSWindow::HideMouse( bool bHide )
{
    CHECK_POINT_MSG( "HideMouse" );
    return kError_NoErr;
}

Error
BeOSWindow::Minimize( void )
{
    CHECK_POINT_MSG( "Minimize" );
    return kError_NoErr;
}

Error
BeOSWindow::Restore( void )
{
    CHECK_POINT_MSG( "Restore" );
    return kError_NoErr;
}

void
BeOSWindow::HandleMouseLButtonDown( Pos& oPos )
{
    Window::HandleMouseLButtonDown( oPos );
}

void
BeOSWindow::HandleMouseLButtonUp( Pos& oPos )
{
    Window::HandleMouseLButtonUp( oPos );
}

Error
BeOSWindow::SetMousePos( Pos& oMousePos )
{
    CHECK_POINT_MSG( "SetMousePos" );
    assert( 0 );
    return kError_NoErr;
}

Error
BeOSWindow::GetMousePos( Pos& oMousePos )
{
    CHECK_POINT_MSG( "GetMousePos" );
    BPoint p;
    uint32 buttons;

    m_mainWindow->Lock();
    m_canvasView->GetMouse( &p, &buttons, false );
    m_mainWindow->Unlock();

    oMousePos.x = int( p.x );
    oMousePos.y = int( p.y );

    CHECK_POINT_MSG( "GetMousePos done" );
    return kError_NoErr;
}

Error
BeOSWindow::SetWindowPosition( Rect& oWindowRect )
{
    CHECK_POINT_MSG( "SetWindowPosition" );

    m_mainWindow->MoveTo( float(oWindowRect.x1), float(oWindowRect.y1) );
    m_mainWindow->Sync();

    return kError_NoErr;
}

Error
BeOSWindow::GetWindowPosition( Rect& oWindowRect )
{
    CHECK_POINT_MSG( "GetWindowPosition" );

    m_mainWindow->Lock();
    BRect frame = m_mainWindow->Frame();
    m_mainWindow->Unlock();
    oWindowRect.x1 = int( frame.left );
    oWindowRect.y1 = int( frame.top );
    oWindowRect.x2 = int( frame.right );
    oWindowRect.y2 = int( frame.bottom );

    return kError_NoErr;
}

// vi: set ts=4:

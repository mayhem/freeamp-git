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

#include "MainWindow.h"
#include "BeOSWindow.h"
#include "semaphore.h"
#include <stdio.h>
#include <be/support/Locker.h>
#include <be/app/MessageRunner.h>

const uint32 FAWINDOW_TIMER_MSG = 'timr';

MainWindow::MainWindow( BRect frame, const char* title, BeOSWindow* parent )
:   BWindow( frame, title, B_NO_BORDER_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL,
             B_WILL_ACCEPT_FIRST_CLICK ),
    m_parent( parent ),
    m_quitSem( NULL ),
    m_timer( NULL )
{
    m_quitSem = new Semaphore();
    m_timer = new BMessageRunner( BMessenger( this, this ),
                                  new BMessage( FAWINDOW_TIMER_MSG ),
                                  250000 );
    if ( m_timer->InitCheck() < B_NO_ERROR )
    {
        printf( "BeOSWindow: failed to init timer\n" );
    }
}

MainWindow::~MainWindow()
{
    delete m_timer;

    m_quitSem->Signal();
    delete m_quitSem;
    m_quitSem = NULL;
}

bool
MainWindow::QuitRequested( void )
{
    m_frame = Frame();
    m_quitSem->Signal();
    return true;
}

void
MainWindow::MessageReceived( BMessage* message )
{
    switch ( message->what )
    {
        case FAWINDOW_TIMER_MSG:
            m_parent->TimerEvent();
            break;
        default:
            BWindow::MessageReceived( message );
            break;
    }
}

void
MainWindow::WaitForQuit( BRect* windowRectOnExit )
{
    m_quitSem->Wait();

    *windowRectOnExit = m_frame;
}

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
#define DEBUG 1
#include <be/support/Debug.h>
#include <assert.h>

int32 PrefWindow::s_running = 0;

PrefWindow::PrefWindow( BRect frame, const char* title )
//:   BWindow( frame, title, B_DOCUMENT_WINDOW_LOOK, B_MODAL_APP_WINDOW_FEEL, 0 )
:   BWindow( frame, title, B_DOCUMENT_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, 0 )
{
    int32 old = atomic_or( &s_running, 1 );
    assert( old == 0 );

    m_okToQuitSem = create_sem( 0, "PrefWindowOkToQuit" );
}

PrefWindow::~PrefWindow()
{
    delete_sem( m_okToQuitSem );
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

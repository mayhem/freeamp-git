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

#include "BeOSPreferenceWindow.h"
#include "BeOSWindow.h"
#include "PrefWindow.h"
#include <be/support/Debug.h>

#define CHECK_POINT CHECK_POINT_MSG("")
#define CHECK_POINT_MSG(a) PRINT(( "File:%s Line:%d :%s\n", __FILE__, __LINE__, a ))

BeOSPreferenceWindow::BeOSPreferenceWindow( FAContext* context,
                                            ThemeManager* themeMan )
:   PreferenceWindow( context, themeMan ),
    m_prefWindow( NULL )
{
}

BeOSPreferenceWindow::~BeOSPreferenceWindow()
{
    CHECK_POINT;
}

bool
BeOSPreferenceWindow::Show( Window* parent )
{
    if ( PrefWindow::IsRunning() ) return true;

    if ( !m_prefWindow )
    {
        BRect r( 100, 100, 600, 500 );
        m_prefWindow = new PrefWindow( r, "Preference",
                                       m_pContext, m_pThemeMan );
    }

    m_prefWindow->Show();

    return true;
}

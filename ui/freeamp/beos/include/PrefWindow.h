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

#ifndef INCLUDED_PREF_WINDOW_H__
#define INCLUDED_PREF_WINDOW_H__

#include <be/interface/Window.h>

class PrefWindow : public BWindow
{
public:
                        PrefWindow( BRect frame, const char* title );
    virtual             ~PrefWindow();
    virtual bool        QuitRequested( void );
    bool                WaitForQuit( bigtime_t timeout = B_INFINITE_TIMEOUT );
    static bool         IsRunning( void );

protected:

private:
    sem_id              m_okToQuitSem;
    static int32        s_running;
};

#endif // INCLUDED_PREF_WINDOW_H__

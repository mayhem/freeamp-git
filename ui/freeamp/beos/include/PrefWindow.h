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
#include "PrefViews.h"
#include "Prefs.h"
#include <vector>

using namespace std;

class FAContext;
class ThemeManager;

enum {
    MSG_SELECT_PANE = 'slpn',
    MSG_OK = 'okay',
    MSG_CANCEL = 'cncl',
    MSG_APPLY = 'aply',
    MSG_SET_TEXT_ONLY = 'txtl',
    MSG_SET_IMAGES_ONLY = 'imgs',
    MSG_SET_TEXT_AND_IMAGES = 'txim',
};

class PrefWindow : public BWindow
{
public:
                        PrefWindow( BRect frame, const char* title,
                                    FAContext* context,
                                    ThemeManager* themeManager );
    virtual             ~PrefWindow();
    virtual void        MessageReceived( BMessage* message );
    virtual bool        QuitRequested( void );
    bool                WaitForQuit( bigtime_t timeout = B_INFINITE_TIMEOUT );
    static bool         IsRunning( void );

    void                SelectPane( uint32 index );
    void                Apply( void );

protected:
    void                InitViews( void );

private:
    sem_id              m_okToQuitSem;
    static int32        s_running;

    FAContext*          m_context;
    ThemeManager*       m_themeManager;
    Prefs               m_originalValues;
    Prefs               m_proposedValues;
    Prefs               m_currentValues;
    PrefViews           m_prefViews;
    uint32              m_currentPane;
    vector<BView*>      m_panes;
};

#endif // INCLUDED_PREF_WINDOW_H__

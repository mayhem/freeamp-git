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

#ifndef INCLUDED_BEOSWINDOW_H
#define INCLUDED_BEOSWINDOW_H

#include <string>
#include "Window.h"

class MainWindow;

class BeOSWindow : public Window
{
public:
                    BeOSWindow( Theme* pTheme, string& oName );
    virtual         ~BeOSWindow();

    // VulcanMinkMeld is called when this window should 'become' the
    // other window. 
    virtual Error   VulcanMindMeld( Window* other );

    virtual Error   Run( Pos& oWindowPos );
    virtual Error   Close( void );
    virtual Error   Show( void );
    virtual Error   Hide( void );
    virtual Error   Enable( void );
    virtual Error   Disable( void );
    virtual Error   SetTitle( string& oTitle );
    virtual Error   CaptureMouse( bool bCapture );
    virtual Error   HideMouse( bool bHide );
    virtual Error   Minimize( void );
    virtual Error   Restore( void );
    virtual bool  LButtonDown(void);
	virtual Error GetDesktopSize(int32 &iX, int32 &iY);

    // These functions should be called from the OS dependent event
    // loop to handle mouse messages
    virtual void    HandleMouseLButtonDown( Pos& oPos );
    virtual void    HandleMouseLButtonUp( Pos& oPos );

    // Mouse position is in screen coordinates
    virtual Error   SetMousePos( Pos& oMousePos );
    virtual Error   GetMousePos( Pos& oMousePos );
    virtual Error   SetWindowPosition( Rect& oWindowRect );
    virtual Error   GetWindowPosition( Rect& oWindowRect );

protected:

private:
    MainWindow*     m_mainWindow;
    BView*          m_canvasView;
};

#endif // INCLUDED_BEOSWINDOW_H

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

#ifndef INCLUDED_WINDOW__H_
#define INCLUDED_WINDOW__H_

// The debugger can't handle symbols more than 255 characters long.
// STL often creates symbols longer than that.
// When symbols are longer than 255 characters, the warning is disabled.
#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <string> 
#include <vector>
#include <map>

using namespace std;

#include "Bitmap.h"
#include "Control.h"
#include "Canvas.h"
#include "thread.h"
#include "semaphore.h"
#include "mutex.h"

typedef map<string, Control *> ControlMap;
typedef map<string, Control *>::iterator ControlMapIterator;

class Theme;

class Window
{
    public:

               Window(Theme *pTheme, string &oName);
      virtual ~Window(void);

      void    GetName(string &oName);
      Canvas *GetCanvas(void);
      void    AddControl(Control *pControl);
      void    ClearControls(void);
      void    Keystroke(unsigned char cKey);

      virtual void SetStayOnTop(bool bStay);
      virtual void SetLiveInToolbar(bool bLive);

      // The following functions are designed to give access to the 
      // controls via the control's name. 
      Error   SendControlMessage(Control *pControl, ControlMessageEnum eMsg);

      Error   ControlEnable(const string &oControlName, bool bSet, bool &bEnable);
      Error   ControlShow(const string &oControlName, bool bSet, bool &bShow);
      Error   ControlIntValue(const string &oControlName, bool bSet, 
                             int &iValue);
      Error   ControlStringValue(const string &oControlName, bool bSet,
                                 string &oValue);
      Error   ControlGetTip(const string &oName, string &oTip);
      Error   ControlGetDesc(const string &oName, string &oDesc);
      bool    DoesControlExist(const string &oName);

      // These functions should be called from the OS dependent event
      // loop to handle mouse messages
      virtual void  HandleMouseMove(Pos &oPos);
      virtual void  HandleMouseLButtonDown(Pos &oPos);
      virtual void  HandleMouseLButtonUp(Pos &oPos);
	   virtual void  MouseHasLeftWindow(void);

      virtual Error StartMouseCapture(Control *);
      virtual Error EndMouseCapture(void);
     
      // This init call is made after the window is created
      virtual void  Init(void);
      virtual void  TimerEvent(void);
      
      // VulcanMinkMeld is called when this window should 'become' the
      // other window. 
	  virtual Error VulcanMindMeld(Window *pOther);
              void  VulcanMindMeldHost(bool bIsHost);

      // Run handles OS dependent messages and calls the functions below
      // or passes the messages on to child controls
      virtual Error Run(Pos &oWindowPos) = 0;
      virtual Error Close(void) = 0;
      virtual Error Show(void) = 0;
      virtual Error Hide(void) = 0;
      virtual Error Enable(void) = 0;
      virtual Error Disable(void) = 0;
      virtual Error SetTitle(string &oTitle) = 0;
      virtual Error CaptureMouse(bool bCapture) = 0;
      virtual Error HideMouse(bool bHide) = 0;
      virtual Error Minimize(void) = 0;
      virtual Error Restore(void) = 0;
	  virtual bool  LButtonDown(void) = 0;
      
      // Mouse position is in screen coordinates
      virtual Error SetMousePos(Pos &oMousePos) = 0;
      virtual Error GetMousePos(Pos &oMousePos) = 0;
      virtual Error SetWindowPosition(Rect &oWindowRect) = 0;
      virtual Error GetWindowPosition(Rect &oWindowRect) = 0;

    protected:

      Control *ControlFromPos(Pos &oPos);
     
      string                    m_oName;
      vector<Control *>         m_oControls;
      ControlMap                m_oControlMap;
      Canvas                   *m_pCanvas;
      bool                      m_bExit, m_bWindowMove, m_bLButtonDown;
      Theme                    *m_pTheme;
      Control                  *m_pMouseInControl, *m_pCaptureControl;
      Pos                       m_oMovePos;
      bool                      m_bStayOnTop, m_bLiveInToolbar;
      bool                      m_bIsVulcanMindMeldHost;
      Rect                      m_oMoveStart;
};

#endif

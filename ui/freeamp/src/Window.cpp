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
#include <algorithm>
#include "Window.h"
#include "Theme.h"
#include "debug.h"

#define DB Debug_v("%s:%d\n", __FILE__, __LINE__);

Window::Window(Theme *pTheme, string &oName)
{
    m_oName = oName;
    m_bExit = false;
    m_pTheme = pTheme;
    m_bMouseButtonDown = false;
    m_bStayOnTop = false;
    m_bLiveInToolbar = false;

    m_pCanvas = NULL;
    m_pMouseInControl = NULL;
    m_pCaptureControl = NULL;
    m_bIsVulcanMindMeldHost = false;
}

Window::~Window(void)
{
    if (m_bIsVulcanMindMeldHost)
    {
       ClearControls();
       delete m_pCanvas;
    }   
}

void Window::VulcanMindMeldHost(bool bHost)
{
    m_bIsVulcanMindMeldHost = bHost;
}

Error Window::VulcanMindMeld(Window *pOther)
{
    m_oName = pOther->m_oName;
    m_pTheme = pOther->m_pTheme;

    m_bMouseButtonDown = pOther->m_bMouseButtonDown;
    m_bStayOnTop = pOther->m_bLiveInToolbar;
    
    m_pMouseInControl = NULL;
    m_pCaptureControl = NULL;
    m_oControls = pOther->m_oControls;
    m_oControlMap = pOther->m_oControlMap;

    m_pCanvas = pOther->m_pCanvas;
    Init();   
    
    return kError_NoErr;
}

void Window::Init(void)
{
    vector<Control *>::iterator i;

    m_pCanvas->Init();

    for(i = m_oControls.begin(); i != m_oControls.end(); i++)
    {
        (*i)->SetParent(this);
        (*i)->Init();
    }    
        
    m_pTheme->InitControls();    
}

void Window::AddControl(Control *pControl)
{
    string oName;

    pControl->GetName(oName);
    m_oControlMap[oName] = pControl;
    m_oControls.push_back(pControl);
}

void Window::ClearControls(void)
{
    while(m_oControls.size() > 0)
    {
        delete m_oControls[0];
        m_oControls.erase(m_oControls.begin());
    }
}

Canvas *Window::GetCanvas(void)
{
    return m_pCanvas;
}

void Window::GetName(string &oName)
{
    oName = m_oName;
}

Error Window::ControlEnable(const string &oTarget, bool bSet, bool &bEnable)
{
    Control *pControl;

    pControl = m_oControlMap[oTarget];
    if (pControl == NULL)
       return kError_InvalidParam;

    return pControl->Enable(bSet, bEnable);
}

Error Window::ControlShow(const string &oTarget, bool bSet, bool &bShow)
{
    Control *pControl;
    Pos      oPos;
    Rect     oRect;
    Error    eRet;

    pControl = m_oControlMap[oTarget];
    if (pControl == NULL)
       return kError_InvalidParam;

    eRet = pControl->Show(bSet, bShow);

    GetMousePos(oPos);
    GetWindowPosition(oRect);
    oPos.x -= oRect.x1;
    oPos.y -= oRect.y1;
    if (bSet && bShow && pControl->PosInControl(oPos))
    	pControl->AcceptTransition(CT_MouseEnter);

    return eRet;
}

Error Window::ControlIntValue(const string &oTarget, bool bSet, int &iValue)
{
    Control *pControl;

    pControl = m_oControlMap[oTarget];
    if (pControl == NULL)
       return kError_InvalidParam;

    return pControl->IntValue(bSet, iValue);
}

Error Window::ControlStringValue(const string &oTarget, bool bSet, string &oValue)
{
    Control *pControl;

    pControl = m_oControlMap[oTarget];
    if (pControl == NULL)
       return kError_InvalidParam;

    return pControl->StringValue(bSet, oValue);
}

Error Window::ControlGetDesc(const string &oTarget, string &oDesc)
{
    Control *pControl;

    pControl = m_oControlMap[oTarget];
    if (pControl == NULL)
       return kError_InvalidParam;

    pControl->GetDesc(oDesc);
    
    return kError_NoErr;
}

Error Window::ControlGetTip(const string &oTarget, string &oTip)
{
    Control *pControl;

    pControl = m_oControlMap[oTarget];
    if (pControl == NULL)
       return kError_InvalidParam;

    pControl->GetTip(oTip);
    
    return kError_NoErr;
}

Error Window::SendControlMessage(Control *pControl, 
                                 ControlMessageEnum eMesg)
{
    string oControlName;
    
    pControl->GetName(oControlName);

    return m_pTheme->HandleControlMessage(oControlName, eMesg);
}

Control *Window::ControlFromPos(Pos &oPos)
{
    vector<Control *>::iterator i;
    bool                        bShown;

    for(i = m_oControls.begin(); i != m_oControls.end(); i++)
    {
        (*i)->Show(false, bShown);
        if ((*i)->PosInControl(oPos) && bShown) 
            return (*i);
    }        

    return NULL;
}

Error Window::StartMouseCapture(Control *pControl)
{
    m_pCaptureControl = pControl;
    return CaptureMouse(true);
}

Error Window::EndMouseCapture(void)
{
    m_pCaptureControl = NULL;
    return CaptureMouse(false);
}

void Window::HandleMouseMove(Pos &oScreenPos)
{
    Control *pControl;
    Pos      oPos;
    Rect     oRect;

    if (m_bMouseButtonDown)
    {
       m_oMoveStart.x1 += (oScreenPos.x - m_oMovePos.x);
       m_oMoveStart.x2 += (oScreenPos.x - m_oMovePos.x);
       m_oMoveStart.y1 += (oScreenPos.y - m_oMovePos.y);
       m_oMoveStart.y2 += (oScreenPos.y - m_oMovePos.y);

       m_oMovePos = oScreenPos;
       SetWindowPosition(m_oMoveStart);
       
       return; 
    }

    GetWindowPosition(oRect);
    oPos.x = oScreenPos.x - oRect.x1;
    oPos.y = oScreenPos.y - oRect.y1;
    
    if (m_pCaptureControl)
    {
       m_pCaptureControl->AcceptTransition(CT_MouseMove, &oPos);
       return;
    }

    pControl = ControlFromPos(oPos);
    if (pControl)
    {
       if (m_pMouseInControl && m_pMouseInControl != pControl)
       {
           m_pMouseInControl->AcceptTransition(CT_MouseLeave);
           m_pMouseInControl = pControl;
           m_pMouseInControl->AcceptTransition(CT_MouseEnter);
       }

       if (m_pMouseInControl == NULL)
       {
           m_pMouseInControl = pControl;
           m_pMouseInControl->AcceptTransition(CT_MouseEnter);
       }

       pControl->AcceptTransition(CT_MouseMove, &oPos);
       return;
    }
    else
    {
       if (m_pMouseInControl)
       {
           m_pMouseInControl->AcceptTransition(CT_MouseLeave);
           m_pMouseInControl = NULL;
       }
    }

    return;
}

void Window::HandleMouseLButtonDown(Pos &oScreenPos)
{
    Control *pControl;
    Rect     oRect;
    Pos      oPos;

    GetWindowPosition(oRect);
    oPos.x = oScreenPos.x - oRect.x1;
    oPos.y = oScreenPos.y - oRect.y1;

    if (m_pCaptureControl)
    {
       m_pCaptureControl->AcceptTransition(CT_MouseLButtonDown, &oPos);
       return;
    }

    pControl = ControlFromPos(oPos);
    if (pControl)
    {
       pControl->AcceptTransition(CT_MouseLButtonDown, &oPos);
       return;
    }

    m_bMouseButtonDown = true;
#ifndef HAVE_GTK
    CaptureMouse(true);
#endif
       
    GetWindowPosition(m_oMoveStart);
    m_oMovePos = oScreenPos;

    return;
}

void Window::HandleMouseLButtonUp(Pos &oScreenPos)
{
    Control *pControl;
    Pos      oPos;
    Rect     oRect;

    GetWindowPosition(oRect);
    oPos.x = oScreenPos.x - oRect.x1;
    oPos.y = oScreenPos.y - oRect.y1;

    if (m_bMouseButtonDown)
    {
       m_bMouseButtonDown = false;
#ifndef HAVE_GTK
       CaptureMouse(false);
#endif        
  
       return; 
    }
    
    if (m_pCaptureControl)
    {
       m_pCaptureControl->AcceptTransition(CT_MouseLButtonUp, &oPos);
       return;
    }

    pControl = ControlFromPos(oPos);
    if (pControl)
    {
       pControl->AcceptTransition(CT_MouseLButtonUp, &oPos);
       return;
    }

    return;
}

void Window::TimerEvent(void)
{
    vector<Control *>::iterator i;

    for(i = m_oControls.begin(); i != m_oControls.end(); i++)
    {
        if ((*i)->WantsTimingMessages())
            (*i)->AcceptTransition(CT_Timer);
    }        
}

void Window::SetStayOnTop(bool bStay)
{
    m_bStayOnTop = bStay;
}

void Window::SetLiveInToolbar(bool bLive)
{
    m_bLiveInToolbar = bLive;
}

void Window::Keystroke(unsigned char cKey)
{
    m_pTheme->HandleKeystroke(cKey);
}

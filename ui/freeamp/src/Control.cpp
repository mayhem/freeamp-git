/*____________________________________________________________________________

   FreeAmp - The Free MP3 Player

   Portions Copyright (C) 1999 EMusic

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
#include "Control.h"
#include "Window.h"
#include "debug.h"

#define DB Debug_v("%s:%d\n", __FILE__, __LINE__);

// I HATE MICROSOFT   I HATE MICROSOFT   I HATE MICROSOFT   I HATE MICROSOFT
bool operator<(const TransitionInfo &A, const TransitionInfo &b)
{
    assert(0);
    return 0;
}
bool operator==(const TransitionInfo &A, const TransitionInfo &b)
{
    assert(0);
    return 0;
}
// I HATE MICROSOFT   I HATE MICROSOFT   I HATE MICROSOFT   I HATE MICROSOFT

Control::Control(Window *pWindow, string &oName, TransitionInfo *pInfo)
{
    int iLoop;

    m_pParent = pWindow;
    m_eCurrentState = CS_Normal;
    m_eLastState = CS_LastState; 
    m_oName = oName; 
    m_bShow = true;
    m_bEnable = true;
    m_iValue = 0;
    m_oValue = string("");
    m_pBitmap = NULL;
    m_bWantsTimingMessages = false;
    
    for(iLoop = 0; ; iLoop++, pInfo++)
    {
        if (pInfo->eState == CS_LastState)
            break;

        m_oTransitions.push_back(*pInfo);
    }
}

Control::~Control(void)
{
}

Error Control::Show(bool bSet, bool &bShow)
{
    if (bSet)
    {
        m_bShow = bShow;
        AcceptTransition(bShow ? CT_Show : CT_Hide);
    }
    else
        bShow = m_bShow;

    return kError_NoErr;
}

Error Control::Enable(bool bSet, bool &bEnable)
{
    if (bSet)
    {
        m_bEnable = bEnable;
        AcceptTransition(bEnable ? CT_Enable : CT_Disable);
    }
    else
        bEnable = m_bEnable;

    return kError_NoErr;
}

Error Control::IntValue(bool bSet, int &iValue)
{
    if (bSet)
    {
    	if (m_iValue != iValue)
        {
            m_iValue = iValue;
            AcceptTransition(CT_SetValue);
        }   
    }
    else
        iValue = m_iValue;

    return kError_NoErr;
}

Error Control::StringValue(bool bSet, string &oValue)
{
    if (bSet)
    {
    	if (m_oValue.length() == 0 || m_oValue != oValue)
        {
            m_oValue = oValue;
            AcceptTransition(CT_SetValue);
        }   
    }
    else
        oValue = m_oValue;

    return kError_NoErr;
}

void Control::AcceptTransition(ControlTransitionEnum eTrans, Pos *pPos)
{
    vector<TransitionInfo>::iterator i;
    
    for(i = m_oTransitions.begin(); i != m_oTransitions.end(); i++)
    {
        if (((*i).eState == m_eCurrentState || (*i).eState == CS_Any) && 
            (*i).eAction == eTrans)   
        {
        	if ((*i).eNextState != CS_Same)
            {
                m_eLastState = m_eCurrentState;
                m_eCurrentState = (*i).eNextState;
            }    
            Transition(eTrans, pPos);

            return;
        }
    }
}

void Control::SetParent(Window *pParent)
{
	m_pParent = pParent;
}

void Control::SetRect(Rect &oRect)
{
    m_oRect = oRect;
}

void Control::GetRect(Rect &oRect)
{
    oRect = m_oRect;
}

void Control::SetPos(Pos &oPos)
{
    m_oRect.x1 = oPos.x;
    m_oRect.y1 = oPos.y;
    m_oRect.x2 = -1;
    m_oRect.y2 = -1;
}

void Control::SetBitmap(Bitmap *pBitmap, Rect &oBitmapRect, bool bHoriz)
{
    m_pBitmap = pBitmap;
    m_oBitmapRect = oBitmapRect;
    m_bHorizontalBitmap = bHoriz;
}

void Control::GetName(string &oName)
{
    oName = m_oName;
};

void Control::Erase(void)
{
    Canvas *pCanvas;

    pCanvas = m_pParent->GetCanvas();
    pCanvas->Erase(m_oRect);
}

void Control::BlitFrame(int iFrame, int iNumFramesInBitmap, Rect *pRect)
{
	if (m_bHorizontalBitmap)
		BlitFrameHoriz(iFrame,iNumFramesInBitmap,pRect);
	else
		BlitFrameVert(iFrame,iNumFramesInBitmap,pRect);
}

void Control::BlitFrameHoriz(int iFrame, int iNumFramesInBitmap, Rect *pRect)
{
    Canvas *pCanvas;
    int     iFrameWidth;
    Rect    oFrameRect, oDestRect;

    if (pRect == NULL)
        oDestRect = m_oRect;
    else
        oDestRect = *pRect;

    iFrameWidth = (m_oBitmapRect.Width() + 1) / iNumFramesInBitmap;
    oFrameRect = m_oBitmapRect;
    oFrameRect.x1 += iFrame * iFrameWidth;
    oFrameRect.x2 = oFrameRect.x1 + iFrameWidth - 1;

    oFrameRect.x2++;
    oFrameRect.y2++;

    oDestRect.x2++;
    oDestRect.y2++;

    pCanvas = m_pParent->GetCanvas();
    pCanvas->MaskBlitRect(m_pBitmap, oFrameRect, oDestRect);
    
    pCanvas->Invalidate(oDestRect);
    pCanvas->Update();
}

void Control::BlitFrameVert(int iFrame, int iNumFramesInBitmap, Rect *pRect)
{
    Canvas *pCanvas;
    int     iFrameHeight;
    Rect    oFrameRect, oDestRect;

    if (pRect == NULL)
        oDestRect = m_oRect;
    else
        oDestRect = *pRect;

    iFrameHeight = (m_oBitmapRect.Height() + 1) / iNumFramesInBitmap;
    oFrameRect = m_oBitmapRect;
    oFrameRect.y1 += iFrame * iFrameHeight;
    oFrameRect.y2 = oFrameRect.y1 + iFrameHeight - 1;

    oFrameRect.y2++;
    oFrameRect.x2++;

    oDestRect.y2++;
    oDestRect.x2++;

    pCanvas = m_pParent->GetCanvas();
    pCanvas->MaskBlitRect(m_pBitmap, oFrameRect, oDestRect);
    
    pCanvas->Invalidate(oDestRect);
    pCanvas->Update();
}

void Control::BlitMultiStateFrame(int iFrame, int iNumFramesInBitmap,
                                  int iRow, int iNumRowsInBitmap)  
{
    Canvas *pCanvas;
    int     iFrameWidth, iFrameHeight;
    Rect    oFrameRect, oDestRect;

    oDestRect = m_oRect;

    iFrameWidth = (m_oBitmapRect.Width() + 1) / iNumFramesInBitmap;
    iFrameHeight = (m_oBitmapRect.Height() + 1) / iNumRowsInBitmap;

    oFrameRect = m_oBitmapRect;
    oFrameRect.x1 += iFrame * iFrameWidth;
    oFrameRect.x2 = oFrameRect.x1 + iFrameWidth + 1;
    oFrameRect.y1 += iRow * iFrameHeight;
    oFrameRect.y2 = oFrameRect.y1 + iFrameHeight + 1;

    oDestRect.x2++;
    oDestRect.y2++;

    pCanvas = m_pParent->GetCanvas();
    pCanvas->MaskBlitRect(m_pBitmap, oFrameRect, oDestRect);
    
    pCanvas->Invalidate(oDestRect);
    pCanvas->Update();
}


bool Control::PosInControl(Pos &oPos)
{
    return m_oRect.IsPosInRect(oPos);
}        

void Control::SetDesc(const string &oDesc)
{
    m_oDesc = oDesc;
}    

void Control::SetTip(const string &oTip)
{
    m_oToolTip = oTip;
}    

void Control::GetDesc(string &oDesc)
{
    oDesc = m_oDesc;
}    

void Control::GetTip(string &oTip)
{
    oTip = m_oToolTip;
}    

bool Control::WantsTimingMessages(void)
{
	return m_bWantsTimingMessages;
}

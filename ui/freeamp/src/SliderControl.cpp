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

#include "stdio.h"
#include "SliderControl.h"
#include "Window.h"
#include "debug.h"

#define DB Debug_v("%s:%d\n", __FILE__, __LINE__);

// Bitmap Info:
// Frame 0: Normal slider
// Frame 1: Selected slider
// Frame 2: Disabled slider

static TransitionInfo pTransitions[] =
{  
    { CS_Normal,     CT_MouseEnter,       CS_MouseOver  }, 
    { CS_Normal,     CT_Disable,          CS_Disabled   }, 
    { CS_Normal,     CT_Hide,             CS_Hidden     },
    { CS_MouseOver,  CT_MouseLeave,       CS_Normal     }, 
    { CS_MouseOver,  CT_MouseLButtonDown, CS_Dragging   }, 
    { CS_MouseOver,  CT_Disable,          CS_Disabled   }, 
    { CS_Dragging,   CT_MouseMove,        CS_Dragging   }, 
    { CS_Dragging,   CT_MouseLButtonUp,   CS_MouseOver  }, 
    { CS_Disabled ,  CT_Enable,           CS_Normal     },
    { CS_Disabled ,  CT_MouseEnter,       CS_DisabledMO },
    { CS_DisabledMO, CT_MouseLeave,       CS_Disabled   },
    { CS_Hidden,     CT_Show,             CS_Normal     },
    { CS_Any,        CT_SetValue,         CS_Same       },
    { CS_LastState,  CT_LastTransition,   CS_LastState  }
};

SliderControl::SliderControl(Window *pWindow, string &oName) :
               Control(pWindow, oName, pTransitions)
{
     m_iRange = -1;
     m_iCurrentPos = 0;
     m_oOrigin.x = -1;
     m_bIsDrag = false;
};

SliderControl::~SliderControl(void)
{

}

void SliderControl::Init(void)
{
    m_iThumbWidth = m_oBitmapRect.Width() / 3;
    
    m_iRange = m_oRect.Width() - m_iThumbWidth;

    Transition(CT_None, NULL);
}

void SliderControl::Transition(ControlTransitionEnum  eTrans,
                               Pos                   *pPos)
{
    Rect oRect;

    switch(eTrans)
    {
       case CT_MouseEnter:
           m_pParent->SendControlMessage(this, CM_MouseEnter);
           break;
       case CT_MouseLeave:
           m_pParent->SendControlMessage(this, CM_MouseLeave);
           break;

	   case CT_SetValue:
       {
       	   int iNewPos;	


           if (m_iValue < 0 || m_iValue > 100)
              return;

           iNewPos = (m_iValue * m_iRange) / 100;
           if (iNewPos == m_iCurrentPos)
               return;
               
           MoveThumb(m_iCurrentPos, iNewPos);
           m_iCurrentPos = iNewPos;
           return;
       }   

       default:
          break;
    }  

    oRect.x1 = m_oRect.x1 + m_iCurrentPos;
    oRect.x2 = oRect.x1 + m_iThumbWidth;
    oRect.y1 = m_oRect.y1;
    oRect.y2 = m_oRect.y2;

    if (m_eCurrentState != CS_Dragging && 
        m_eLastState == CS_Dragging)
    {    
        if (m_oOrigin.x != -1)
	    {
	    	Rect oRect;
	 
	        m_oOrigin.x = m_oRect.x1 + m_iCurrentPos + (m_iThumbWidth / 2);
	        m_pParent->GetWindowPosition(oRect);
	        m_oOrigin.x += oRect.x1;
	        m_oOrigin.y += oRect.y1;
	        
//	        m_pParent->SetMousePos(m_oOrigin);
	        m_pParent->EndMouseCapture();
	        m_pParent->HideMouse(false);
	        m_oOrigin.x = -1;
	    }    
        m_iValue = (m_iCurrentPos * 100) / m_iRange;
        m_pParent->SendControlMessage(this, CM_ValueChanged);
    }    

    if (m_eCurrentState == CS_Dragging && 
        m_eLastState != CS_Dragging)
	   m_bIsDrag = oRect.IsPosInRect(*pPos);

    switch(m_eCurrentState)
    {
       case CS_Normal:
          BlitFrame(0, 3, &oRect);
          break;

       case CS_MouseOver:
          BlitFrame(1, 3, &oRect);
          break;

       case CS_Dragging:
          if (m_bIsDrag)
              HandleDrag(eTrans, pPos); 
          else
              HandleJump(eTrans, pPos);    
          break;

       case CS_Disabled:
          BlitFrame(2, 3, &oRect);
          break;

       default:
          break;
    }
}

void SliderControl::HandleJump(ControlTransitionEnum  eTrans,
                               Pos                   *pPos)
{
    int     iNewPos;

//    Debug_v("Handle jump: %d (%d - %d)", pPos->x, m_oRect.x1, m_oRect.x2);

    iNewPos = pPos->x - m_oRect.x1 - (m_iThumbWidth / 2);
    iNewPos = min(max(iNewPos, 0), m_iRange);
    if (iNewPos == m_iCurrentPos)
       return;

//    Debug_v("iNewPos: %d Range: %d", iNewPos, m_iRange);
    MoveThumb(m_iCurrentPos, iNewPos);

    m_iCurrentPos = iNewPos;
    m_oLastPos = *pPos;
    
    m_iValue = (m_iCurrentPos * 100) / m_iRange;
//    Debug_v("new value: %d", m_iValue);
    m_bIsDrag = true;
}

void SliderControl::HandleDrag(ControlTransitionEnum  eTrans,
                               Pos                   *pPos)
{
    int     iDelta, iNewPos;

    // Is this the beginning of a drag?
    if (m_oOrigin.x == -1)
    {
        m_oLastPos = m_oOrigin = *pPos;

        m_pParent->HideMouse(true);
        m_pParent->StartMouseCapture(this);

        return;
    }

	if (pPos->x < 0)
        return;

    iDelta = pPos->x - m_oLastPos.x; 
    iNewPos = min(max(m_iCurrentPos + iDelta, 0), m_iRange);
    
    if (iNewPos == m_iCurrentPos)
       return;

	MoveThumb(m_iCurrentPos, iNewPos);

    m_iCurrentPos = iNewPos;
    m_oLastPos = *pPos;
    
	m_iValue = (m_iCurrentPos * 100) / m_iRange;
    m_pParent->SendControlMessage(this, CM_SliderUpdate);
}

void SliderControl::MoveThumb(int iCurrentPos, int iNewPos)
{
    Canvas *pCanvas;
    Rect    oRect;

    oRect.x1 = m_oRect.x1 + iCurrentPos;
    oRect.x2 = oRect.x1 + m_iThumbWidth + 1;
    oRect.y1 = m_oRect.y1;
    oRect.y2 = m_oRect.y2 + 1;
    
    pCanvas = m_pParent->GetCanvas();
    pCanvas->Erase(oRect);
    pCanvas->Invalidate(oRect);

    oRect.x1 = m_oRect.x1 + iNewPos;
    oRect.x2 = oRect.x1 + m_iThumbWidth;
    oRect.y1 = m_oRect.y1;
    oRect.y2 = m_oRect.y2;
    
    switch(m_eCurrentState)
    {
       case CS_Normal:
          BlitFrame(0, 3, &oRect);
          break;

       case CS_Dragging:
       case CS_MouseOver:
          BlitFrame(1, 3, &oRect);
          break;

       case CS_Disabled:
          BlitFrame(2, 3, &oRect);
          break;

       default:
          break;
    }
}

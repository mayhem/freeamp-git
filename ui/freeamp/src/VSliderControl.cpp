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
#include "VSliderControl.h"
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

VSliderControl::VSliderControl(Window *pWindow, string &oName) :
               Control(pWindow, oName, pTransitions)
{
     m_iRange = -1;
     m_iCurrentPos = 0;
     m_oOrigin.y = -1;
     m_bIsDrag = false;
     m_bInUpdate = false;
};

VSliderControl::~VSliderControl(void)
{

}

void VSliderControl::Init(void)
{
    m_oMutex.Acquire();
    
    m_iThumbHeight = m_oBitmapRect.Height();
    m_iRange = m_oRect.Height() - m_iThumbHeight;

    m_oMutex.Release();
    
    Transition(CT_None, NULL);
}

void VSliderControl::Transition(ControlTransitionEnum  eTrans,
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
       	   int iNewPos, iOldPos;	

           if (m_iValue < 0 || m_iValue > 100)
               return;

           m_oMutex.Acquire();
           if (m_bInUpdate)
           {
              m_oMutex.Release();
              return;
           }
           m_bInUpdate = true;

           iNewPos = ((100 - m_iValue) * m_iRange) / 100;
           if (iNewPos == m_iCurrentPos)
           {
              m_bInUpdate = false;
              m_oMutex.Release();
              return;
           }

           iOldPos = m_iCurrentPos;
           m_iCurrentPos = iNewPos;
           m_bInUpdate = false;
           m_oMutex.Release();

           MoveThumb(iOldPos, iNewPos);
           
           return;
       }   

       default:
          break;
    }  

    oRect.x1 = m_oRect.x1;
    oRect.x2 = m_oRect.x2;
    oRect.y1 = m_oRect.y1 + m_iCurrentPos;
    oRect.y2 = oRect.y1 + m_iThumbHeight;

    m_oMutex.Acquire();
    if (m_eCurrentState != CS_Dragging && 
        m_eLastState == CS_Dragging)
    {    
        if (m_oOrigin.y != -1)
	    {
	    	Rect oRect;
	 
            m_oMutex.Release();
	        m_pParent->GetWindowPosition(oRect);
            m_oMutex.Acquire();
            
	        m_oOrigin.y = m_oRect.y1 + m_iCurrentPos + (m_iThumbHeight / 2);
	        m_oOrigin.y += oRect.y1;
	        m_oOrigin.x += oRect.x1;
	        
            m_oMutex.Release();
	        m_pParent->EndMouseCapture();
	        m_pParent->HideMouse(false);
            m_oMutex.Acquire();
            
	        m_oOrigin.y = -1;
	    }    
        m_iValue = 100 - ((m_iCurrentPos * 100) / m_iRange);

        m_oMutex.Release();
        m_pParent->SendControlMessage(this, CM_ValueChanged);
        m_oMutex.Acquire();
    }    

    if (m_eCurrentState == CS_Dragging && 
        m_eLastState != CS_Dragging)
	   m_bIsDrag = oRect.IsPosInRect(*pPos);

    m_oMutex.Release();

    switch(m_eCurrentState)
    {
       case CS_Normal:
          BlitFrame(0, 3, &oRect);
          break;

       case CS_MouseOver:
          BlitFrame(1, 3, &oRect);
          break;

       case CS_Dragging:
       
          m_oMutex.Acquire();
          if (m_bInUpdate)
          {
              m_oMutex.Release();
              return;
          }
              
          m_bInUpdate = true;
          m_oMutex.Release();

          if (m_bIsDrag)
              HandleDrag(eTrans, pPos); 
          else
              HandleJump(eTrans, pPos);    

          m_oMutex.Acquire();
          m_bInUpdate = false;
          m_oMutex.Release();
          break;
          
       case CS_Disabled:
          BlitFrame(2, 3, &oRect);
          break;

       default:
          break;
    }
}

void VSliderControl::HandleJump(ControlTransitionEnum  eTrans,
                               Pos                   *pPos)
{
    int     iNewPos;

    m_oMutex.Acquire();

    iNewPos = pPos->y - m_oRect.y1 - (m_iThumbHeight / 2);
    iNewPos = min(max(iNewPos, 0), m_iRange);

    if (iNewPos == m_iCurrentPos)
    {
       m_oMutex.Release();
       return;
    }   
    m_oMutex.Release();

    MoveThumb(m_iCurrentPos, iNewPos);

    m_oMutex.Acquire();

    m_iCurrentPos = iNewPos;
    m_oLastPos = *pPos;
    
    m_iValue = 100 - ((m_iCurrentPos * 100) / m_iRange);
    m_bIsDrag = true;
    
    m_oMutex.Release();
}

void VSliderControl::HandleDrag(ControlTransitionEnum  eTrans,
                               Pos                   *pPos)
{
    int     iDelta, iNewPos;

    m_oMutex.Acquire();

    // Is this the beginning of a drag?
    if (m_oOrigin.y == -1)
    {
        m_oLastPos = m_oOrigin = *pPos;

        m_oMutex.Release();

        m_pParent->HideMouse(true);
        m_pParent->StartMouseCapture(this);

        return;
    }

	if (pPos->y < 0)
    {
        m_oMutex.Release();
        return;
    }    

    iDelta = pPos->y - m_oLastPos.y; 
    iNewPos = min(max(m_iCurrentPos + iDelta, 0), m_iRange);
    
    if (iNewPos == m_iCurrentPos)
    {
        m_oMutex.Release();
        return;
    }    
    m_oMutex.Release();

	MoveThumb(m_iCurrentPos, iNewPos);

    m_oMutex.Acquire();
    
    m_iCurrentPos = iNewPos;
    m_oLastPos = *pPos;
    
	m_iValue = 100 - ((m_iCurrentPos * 100) / m_iRange);
    m_oMutex.Release();
    
    m_pParent->SendControlMessage(this, CM_SliderUpdate);
}

void VSliderControl::MoveThumb(int iCurrentPos, int iNewPos)
{
    Canvas *pCanvas;
    Rect    oEraseRect, oRect;

    oEraseRect.y1 = m_oRect.y1 + iCurrentPos;
    oEraseRect.y2 = oEraseRect.y1 + m_iThumbHeight + 1;
    oEraseRect.x1 = m_oRect.x1;
    oEraseRect.x2 = m_oRect.x2 + 1;
    
    pCanvas = m_pParent->GetCanvas();
    pCanvas->Erase(oEraseRect);

    oRect.y1 = m_oRect.y1 + iNewPos;
    oRect.y2 = oRect.y1 + m_iThumbHeight;
    oRect.x1 = m_oRect.x1;
    oRect.x2 = m_oRect.x2;
    
    switch(m_eCurrentState)
    {
       case CS_Normal:
          BlitFrame(0, 3, &oRect, false);
          break;

       case CS_Dragging:
       case CS_MouseOver:
          BlitFrame(1, 3, &oRect, false);
          break;

       case CS_Disabled:
          BlitFrame(2, 3, &oRect, false);
          break;

       default:
          break;
    }
    oEraseRect.Union(oRect);
    pCanvas->Invalidate(oEraseRect);
}

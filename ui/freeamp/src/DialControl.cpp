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
#include "DialControl.h"

static TransitionInfo pTransitions[] =
{  
    { CS_Normal,    CT_MouseEnter,       CS_MouseOver },
    { CS_Normal,    CT_Disable,          CS_Disabled  },
    { CS_Normal,    CT_Hide,             CS_Hidden    },
    { CS_MouseOver, CT_MouseLeave,       CS_Normal    },
    { CS_MouseOver, CT_MouseLButtonDown, CS_Dragging  },
    { CS_MouseOver, CT_Disable,          CS_Disabled  },
    { CS_Dragging,  CT_MouseMove,        CS_Dragging  },
    { CS_Dragging,  CT_MouseLButtonUp,   CS_MouseOver },
    { CS_Disabled,  CT_Enable,           CS_Normal    },
    { CS_Hidden,    CT_Show,             CS_Normal    },
    { CS_LastState, CT_LastTransition,   CS_LastState }
};

DialControl::DialControl(Window *pWindow, string &oName) :
             Control(pWindow, oName, pTransitions)
{
}

DialControl::~DialControl(void)
{

}

void DialControl::Init(void)
{
}

void DialControl::Transition(ControlTransitionEnum  eTrans,
                             Pos                   *pMousePos)

{
    switch(eTrans)
    {
       case CS_Normal:
          break;

       case CS_MouseOver:
          break;

       case CS_Dragging:
          break;

       case CS_Disabled:
          break;

       default:
          break;
    }
}

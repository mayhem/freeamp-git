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

#ifndef INCLUDED_SLIDERCONTROL_H__
#define INCLUDED_SLIDERCONTROL_H__

#include "Control.h"

class SliderControl : public Control
{
    public:

               SliderControl(Window *pWindow, string &oName);
      virtual ~SliderControl(void);

              void Transition(ControlTransitionEnum eTrans, Pos *pMousePos);
      virtual void Init(void);
              void HandleDrag(ControlTransitionEnum  eTrans,
                              Pos                   *pPos);
              void HandleJump(ControlTransitionEnum  eTrans,
                              Pos                   *pPos);

    private:

              void MoveThumb(int iCurrentPos, int iNewPos);
					  
      Pos  m_oOrigin, m_oLastPos;
      int  m_iRange;
      int  m_iCurrentPos;
      int  m_iThumbWidth;
      bool m_bIsDrag;
};

#endif
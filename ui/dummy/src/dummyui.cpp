/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998 GoodNoise

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

#include "dummyui.h"

DummyUI::
DummyUI(Semaphore *sem) 
{
    m_termSemaphore = sem;
}

DummyUI::
~DummyUI() 
{
    if(m_target)
    {
        delete m_target;
    }
}

void 
DummyUI::
SetTarget(EventQueue* eq)
{
    m_target = eq;
}

int32 
DummyUI::
AcceptEventStub(UIRef pUI, Event *pe) 
{
    DummyUI* ui = (DummyUI*)pUI->ref;

    return ui->AcceptEvent(pe);
}

void 
DummyUI::
Cleanup(UIRef pUI)
{
    DummyUI* ui = (DummyUI*)pUI->ref;
    
    delete ui;
}

int32 DummyUI::AcceptEvent(Event *pe) 
{
    int32 result = 255;

    if (pe) 
    {
        switch (pe->GetEvent()) 
        {
	        case CMD_Terminate:
            {
	            m_termSemaphore->Signal();
	            break;
            }

	        case CMD_Cleanup: 
            {
	            Event *pE = new Event(INFO_ReadyToDieUI,m_ref);
	            m_target->AcceptEvent(m_target, pE);
	            break; 
            }

	        default:
	            break;
	    }

	    result = 0;

    } 

    return result;
}

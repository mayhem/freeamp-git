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

#ifndef _LMCLIB_H_
#define _LMCLIB_H_

/* project headers */
#include "config.h"
#include "event.h"
#include "pmo.h"
#include "pmi.h"
#include "lmc.h"

extern "C" {

typedef struct LMCSTRUCT{
    void*   ref;
    void    (*SetPMI)            (struct LMCSTRUCT*, PhysicalMediaInput*);
    void    (*SetPMO)            (struct LMCSTRUCT*, PhysicalMediaOutput*);
    void    (*SetInfoEventQueue) (struct LMCSTRUCT*, EventQueue*);

    bool    (*Decode)            (struct LMCSTRUCT*);
    void    (*Stop)              (struct LMCSTRUCT*);
    void    (*Pause)             (struct LMCSTRUCT*);
    void    (*Resume)            (struct LMCSTRUCT*);
    void    (*Reset)             (struct LMCSTRUCT*);
    bool    (*ChangePosition)    (struct LMCSTRUCT*, int32);

}LMCSTRUCT, *LMCRef;


void Initialize(LMCRef ref);


void SetPMI(LMCRef ref, PhysicalMediaInput*);
void SetPMO(LMCRef ref, PhysicalMediaOutput*);
void SetInfoEventQueue(LMCRef ref, EventQueue*);

bool Decode(LMCRef ref);
void Stop(LMCRef ref);
void Pause(LMCRef ref);
void Resume(LMCRef ref);
void Reset(LMCRef ref);
bool ChangePosition(LMCRef ref, int32 pos);

}

#endif /* _LMCLIB_H_ */







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

typedef void* LMCRef;

typedef void (*SETPMIFUNC)  (LMCRef, PhysicalMediaInput*);
typedef void (*SETPMOFUNC)  (LMCRef, PhysicalMediaOutput*);
typedef void (*SETINFOFUNC) (LMCRef, EventQueue*);

typedef bool (*DECODEFUNC)  (LMCRef);
typedef void (*STOPFUNC)    (LMCRef);
typedef void (*PAUSEFUNC)   (LMCRef);
typedef void (*RESUMEFUNC)  (LMCRef);
typedef void (*RESETFUNC)   (LMCRef);
typedef bool (*CHANGEPOSFUNC)  (LMCRef, int32);

typedef struct LMCFunctionTable{
    SETPMIFUNC      SetPMI;
    SETPMOFUNC      SetPMO;
    SETINFOFUNC     SetInfoEventQueue;
    DECODEFUNC      Decode;
    STOPFUNC        Stop;
    PAUSEFUNC       Pause;
    RESUMEFUNC      Resume;
    RESETFUNC       Reset;
    CHANGEPOSFUNC   ChangePosition;

}LMCFunctionTable;

void Initialize(LMCRef ref, LMCFunctionTable*);


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







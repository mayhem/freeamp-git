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

#ifndef _PMOLIB_H_
#define _PMOLIB_H_

#include <stdlib.h>

/* project headers */
#include "config.h"
#include "pmo.h"

extern "C" {

typedef struct PMOSTRUCT {
    void*   ref;
    bool    (*Init)         (struct PMOSTRUCT*, OutputInfo*);
    bool    (*Reset)        (struct PMOSTRUCT*, bool);
    int32   (*Write)        (struct PMOSTRUCT*);
    int32   (*WriteThis)    (struct PMOSTRUCT*, void*, int32);
    void    (*Clear)        (struct PMOSTRUCT*);

}PMOSTRUCT, *PMORef;

void Initialize(PMORef ref);

bool Init(PMORef ref, OutputInfo* info);
bool Reset(PMORef ref, bool user_stop);
int32 Write(PMORef ref);
int32 WriteThis(PMORef ref, void* buf, int32 len);
void Clear(PMORef ref);

}

#endif /* _PMOLIB_H_ */







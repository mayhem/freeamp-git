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

#ifndef _UILIB_H_
#define _UILIB_H_

/* project headers */
#include "config.h"
#include "event.h"
#include "ctrlobj.h"

#ifdef __cplusplus
extern "C" {
#endif

void Initialize(UIRef ref);

void SetTarget(UIRef ref, EventQueue* eq);
void SetArgs(UIRef ref, int32 argc, char **argv);
int32 AcceptEvent(UIRef ref, Event* event);
void Cleanup(UIRef ref);

#ifdef __cplusplus
} //extern "C"
#endif



#endif /* _UILIB_H_ */







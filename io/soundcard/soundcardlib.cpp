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

/* project headers */
#include "pmolib.h"
#include "soundcardpmo.h"


void Initialize(PMORef ref)
{
    if(ref)
    {
        PhysicalMediaOutput* pmo = new SoundCardPMO;
        ref->ref = pmo;

        ref->Init = Init;
        ref->Reset = Reset;
        ref->Write = Write;
        ref->Pause = Pause;
        ref->Resume = Resume;
        ref->Clear = Clear;
        ref->Cleanup = Cleanup;
    }
}

bool Init(PMORef ref, OutputInfo* info)
{
    PhysicalMediaOutput* pmo = (PhysicalMediaOutput*)ref->ref;

    return pmo->Init(info);
}

bool Reset(PMORef ref, bool user_stop)
{
    PhysicalMediaOutput* pmo = (PhysicalMediaOutput*)ref->ref;

    return pmo->Reset(user_stop);
}


int32 Write(PMORef ref, void* buf, int32 len)
{ 
    PhysicalMediaOutput* pmo = (PhysicalMediaOutput*)ref->ref;

    return pmo->Write(buf, len);
}

void Pause(PMORef ref)
{
    PhysicalMediaOutput* pmo = (PhysicalMediaOutput*)ref->ref;

    pmo->Pause();
}

void Resume(PMORef ref)
{
    PhysicalMediaOutput* pmo = (PhysicalMediaOutput*)ref->ref;

    pmo->Resume();
}

void Clear(PMORef ref)
{
    PhysicalMediaOutput* pmo = (PhysicalMediaOutput*)ref->ref;

    pmo->Clear();
}

void Cleanup(PMORef ref)
{
    PhysicalMediaOutput* pmo = (PhysicalMediaOutput*)ref->ref;

    delete pmo;
}
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
#include "pmilib.h"
#include "localfileinput.h"

void Initialize(PMIRef ref)
{
    if(ref)
    {
        PhysicalMediaInput* pmi = new LocalFileInput;
        ref->ref = pmi;

        ref->Read = Read;
        ref->Seek = Seek;
        ref->SetTo = SetTo;
        ref->Close = Close;
        ref->Url = Url;
        ref->Cleanup = Cleanup;
    }
}

int32 Read(PMIRef ref, void* buf, size_t len)
{
    PhysicalMediaInput* pmi = (PhysicalMediaInput*)ref->ref;

    return pmi->Read(buf, len);
}

int32 Seek(PMIRef ref, int32 offset, int32 origin)
{
    PhysicalMediaInput* pmi = (PhysicalMediaInput*)ref->ref;

    return pmi->Seek(offset, origin);
}

Error SetTo(PMIRef ref, char* url)
{
    PhysicalMediaInput* pmi = (PhysicalMediaInput*)ref->ref;

    return pmi->SetTo(url);
}

Error Close(PMIRef ref)
{ 
    PhysicalMediaInput* pmi = (PhysicalMediaInput*)ref->ref;

    return pmi->Close();
}

const char* Url(PMIRef ref)
{
    PhysicalMediaInput* pmi = (PhysicalMediaInput*)ref->ref;

    return pmi->Url();
}

void Cleanup(PMIRef ref)
{
    PhysicalMediaInput* pmi = (PhysicalMediaInput*)ref->ref;

    delete pmi;
}

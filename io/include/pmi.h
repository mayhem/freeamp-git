
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



#ifndef _PMI_H_
#define _PMI_H_

/* system headers */
#include <stdlib.h>


#if HAVE_UNISTD_H
#include <unistd.h>
#elif HAVE_IO_H
#include <io.h>
#else 
#error Must have unistd.h or io.h!
#endif // HAVE_UNISTD_H

/* project headers */
#include "config.h"

#define SEEK_FROM_START		SEEK_SET
#define SEEK_FROM_CURRENT	SEEK_CUR
#define SEEK_FROM_END		SEEK_END

class PhysicalMediaInput {

public:
    virtual ~PhysicalMediaInput() { }
    virtual int32 Read(void* /*buf*/, size_t /*numbytes*/) {return -1;}
    virtual int32 Seek(int32 offset, int32 origin) {return -1;}
    virtual bool SetTo(char* url) = 0;
    virtual bool Close(void) = 0;
    virtual const char* Url(void) const = 0;
};

extern "C" {

typedef struct PMI {
    void*       ref;
    int32       (*Read)     (struct PMI*, void*, size_t);
    int32       (*Seek)     (struct PMI*, int32, int32);
    bool        (*SetTo)    (struct PMI*, char*);
    bool        (*Close)    (struct PMI*);
    const char* (*Url)      (struct PMI*);
    void        (*Cleanup)  (struct PMI*);

}PMI, *PMIRef;


} // extern "C"

#endif /* _PMI_H_ */


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
#include "errors.h"

#define SEEK_FROM_START		SEEK_SET
#define SEEK_FROM_CURRENT	SEEK_CUR
#define SEEK_FROM_END		SEEK_END

class PhysicalMediaInput {

public:
    virtual ~PhysicalMediaInput() { }
    virtual Error Read(int32 &/*bytes read*/,void* /*buf*/, size_t /*numbytes*/) {return kError_GotDefaultMethod;}
    virtual Error Seek(int32 &/*seeked to*/,int32 offset, int32 origin) {return kError_GotDefaultMethod;}
    virtual Error SetTo(char* url) = 0;
    virtual Error Close(void) = 0;
    virtual const char* Url(void) const = 0;
    virtual const char *GetErrorString(int32) = 0;
};

#endif /* _PMI_H_ */










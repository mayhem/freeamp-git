
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

#ifndef _PMO_H_
#define _PMO_H_

/* system headers */
#include <stdlib.h>

#include <config.h>


#if HAVE_UNISTD_H
#include <unistd.h>
#elif HAVE_IO_H
#include <io.h>
#else 
#error Must have unistd.h or io.h!
#endif // HAVE_UNISTD_H


/* project headers */
#include "config.h"

#define MAXCHANNELS		2


typedef struct OutputInfo
{
	uint32 bits_per_sample;
	uint32 number_of_channels;
	uint32 samples_per_second;
	uint32 max_buffer_size;

}OutputInfo;

class PhysicalMediaOutput{

public:
	virtual ~PhysicalMediaOutput() { }
	virtual bool Initialize(OutputInfo* /*info*/){ return false; }
	virtual bool Reset(bool /*user_stop*/){ return false; }
	virtual void Append(uint32 /*channel*/, int16 /*value*/) { }
	virtual int32 Write() { return -1; }
	virtual int32 WriteThis(void * /* pBuffer */, int32 /* bufflength */) { return -1; }
	virtual void Clear(){ }
};

#endif /* _PMO_H_ */













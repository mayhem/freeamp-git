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
#include <assert.h>


#if HAVE_UNISTD_H
#include <unistd.h>
#elif HAVE_IO_H
#include <io.h>
#else 
#error Must have unistd.h or io.h!
#endif // HAVE_UNISTD_H


/* project headers */
#include "eventdata.h"
#include "config.h"
#include "errors.h"
#include "properties.h"

#define MAXCHANNELS		2


typedef struct OutputInfo
{
	uint32 bits_per_sample;
	uint32 number_of_channels;
	uint32 samples_per_second;
	uint32 max_buffer_size;

}OutputInfo;

class PhysicalMediaOutput
{
public:
            PhysicalMediaOutput() { m_target = NULL; }
    virtual ~PhysicalMediaOutput() { }
    virtual Error Init(OutputInfo* /*info*/){ return kError_GotDefaultMethod; }

    virtual Error BeginWrite(void *&pBuffer, size_t &iBytesToWrite)
        { return kError_GotDefaultMethod; };
    virtual Error EndWrite  (size_t iNumBytesWritten)
        { return kError_GotDefaultMethod; };
    virtual Error AcceptEvent(Event *)
        { return kError_GotDefaultMethod; };
    virtual int32 GetBufferPercentage()
	               {return 0;};
                 
    virtual Error SetTarget(EventQueue *target)
                  {m_target = target; return kError_NoErr; }
    virtual Error Pause(){ return kError_GotDefaultMethod;}
    virtual Error Resume(){ return kError_GotDefaultMethod; }
    virtual Error Break(){ return kError_GotDefaultMethod; }
    virtual Error Clear(){ return kError_GotDefaultMethod; }
    virtual void  WaitToQuit(){ };
    virtual const char *GetErrorString(int32) { return NULL; }
    virtual Error SetPropManager(Properties *) = 0;
    virtual void  SetVolume(int32) { ; };
    virtual int32 GetVolume(void) { return 0; };
    virtual void  ReportError(const char *szError)
                  {
                     assert(m_target);

                     m_target->AcceptEvent(new LMCErrorEvent(szError));
                  };

protected:

    EventQueue *m_target;
};

#endif /* _PMO_H_ */



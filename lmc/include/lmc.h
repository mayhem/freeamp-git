
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

#ifndef _LMC_H_
#define _LMC_H_

#include "errors.h"
#include "event.h"
#include "pmo.h"
#include "pmi.h"
#include "properties.h"

class MediaInfoEvent;

class LogicalMediaConverter {
 public:
    virtual ~LogicalMediaConverter() {}
    virtual Error Decode() = 0;
    virtual Error Stop() = 0;
    virtual Error Pause() = 0;
    virtual Error Resume() = 0;
    virtual Error Reset() = 0;
    virtual Error ChangePosition(int32) = 0;

    virtual bool CanDecode() = 0;
    virtual bool IsStreaming() = 0;
    virtual Error ExtractMediaInfo(MediaInfoEvent **) = 0;

    virtual Error SetPMI(PhysicalMediaInput *) = 0;
    virtual Error SetPMO(PhysicalMediaOutput *) = 0;
    virtual Error SetPropManager(Properties *) = 0;
    virtual Error SetTarget(EventQueue *) = 0;
    virtual Error InitDecoder() = 0;

    virtual Error SetEQData(float *) = 0;
    virtual Error SetEQData(bool) = 0;

    virtual const char *GetErrorString(int32 /*error*/) = 0;
};

#endif // _LMC_H_

/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998-1999 EMusic.com

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

#ifndef INCLUDED_LMC_H_
#define INCLUDED_LMC_H_

#include <vector>

using namespace std;

#include "pipeline.h"
#include "errors.h"
#include "eventdata.h"



class MediaInfoEvent;
class PullBuffer;
class EventBuffer;
class PhysicalMediaInput;
class PhysicalMediaOutput;

class LogicalMediaConverter : public PipelineUnit
{
 public:
            LogicalMediaConverter(FAContext *context) :
                      PipelineUnit(context) { };
    virtual ~LogicalMediaConverter() {}

    virtual Error Prepare(PullBuffer *pInput, PullBuffer *&pOutput) = 0;
    virtual Error ChangePosition(int32) = 0;

    virtual Error InitDecoder() = 0;

    virtual void  SetPMI(PhysicalMediaInput *pmi) { m_pmi = pmi; };
    virtual void  SetPMO(PhysicalMediaOutput *pmo) { m_pmo = pmo; };

    virtual Error SetEQData(float *) = 0;
    virtual Error SetEQData(bool) = 0;
    
    virtual vector<char *> *GetExtensions(void) = 0;

    virtual uint32 CalculateSongLength(const char *) = 0;

    protected:

      virtual Error CanDecode() = 0;
      virtual Error ExtractMediaInfo() = 0;
     
      PhysicalMediaInput    *m_pmi;
      PhysicalMediaOutput   *m_pmo;
};

#endif // _LMC_H_

/*____________________________________________________________________________
        
        FreeAmp - The Free MP3 Player
        Portions copyright (C) 1998 GoodNoise

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

#ifndef _ESOUNDDPMO_H_
#define _ESOUNDDPMO_H_

/* system headers */
#include <stdlib.h>

/* project headers */
#include <config.h>
#include "thread.h"
#include "mutex.h"
#include "pmo.h"
#include "pmoevent.h"
#include "eventbuffer.h"
#include "esdvolume.h"

#define BIT_SELECT  0x1f
#define SLEEPTIME   256

static const uint32 OBUFFERSIZE = 2 * 1152;

enum
{
   pmoError_MinimumError = 4000,
   pmoError_DeviceOpenFailed,
   pmoError_MaximumError
};

class FAContext;

class EsounDPMO:public PhysicalMediaOutput
{
 public:

              EsounDPMO(FAContext *context);
     virtual ~EsounDPMO();

     virtual Error Init(OutputInfo * info);
     virtual VolumeManager *GetVolumeManager();

     static void   StartWorkerThread(void *);

   private:
     void          WorkerThread(void);
     virtual Error Reset(bool user_stop);
     void          HandleTimeInfoEvent(PMOTimeInfoEvent *pEvent);

     bool         m_properlyInitialized;
     uint32       channels;
     int   audio_fd;
     OutputInfo  *myInfo;
     Thread      *m_pBufferThread;
     Mutex       *m_pPauseMutex;
     int          m_iOutputBufferSize, m_iBytesPerSample, m_iTotalFragments;
     long long    m_iTotalBytesWritten;
     int          m_iLastFrame;
     unsigned     m_iDataSize;
};

#endif /* _ESOUNDPMO_H_ */ 

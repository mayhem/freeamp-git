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


#ifndef _XINGLMC_H_
#define _XINGLMC_H_

/* system headers */
#include <stdlib.h>

/* project headers */
#include "config.h"

#include "pmi.h"
#include "pmo.h"
#include "mutex.h"
#include "event.h"
#include "lmc.h"
#include "thread.h"
#include "mutex.h"
#include "queue.h"

#define BS_BUFBYTES 60000U
#define PCM_BUFBYTES 60000U

typedef enum {
    XING_Stop = 1,
    XING_Pause,
    XING_Resume
} XingCommand;


class XingLMC : public LMC {

public:
//	XingLMC(PhysicalMediaInput* input, PhysicalMediaOutput* output,EventCallback callback, void* cookie);
    XingLMC();
    
    virtual ~XingLMC();
    
    virtual bool Decode();
    virtual void Stop();
    virtual void Pause();
    virtual void Resume();
    virtual void Reset();
    virtual bool ChangePosition(int32 position);

    virtual void SetPMI(PhysicalMediaInput *);
    virtual void SetPMO(PhysicalMediaOutput *);
    virtual void SetInfoEventQueue(EventQueue *);
    virtual void Init();
//	bool SetCallback(EventCallback callback, void* cookie);

    void DecodeWork();
private:
    static THREAD_RETURN THREAD_LINKAGE DecodeWorkerThreadFunc(void *);

    int32 bs_fill();
    void bs_clear();
private:
    EventQueue *myEQ;
    Mutex *seek_mutex;
    Queue<XingCommand *> *xcqueue;
    PhysicalMediaInput*				m_input;
    PhysicalMediaOutput*				m_output;
    
    int32 bs_bufbytes;
    int32 bs_trigger;
    unsigned char *bs_buffer;
    unsigned char *bs_bufptr;

    unsigned char *pcm_buffer;
    uint32 pcm_bufbytes;
    uint32 pcm_trigger;

    int32 framebytes;
    bool isPaused;
    Thread *decoderThread;
};

#endif /* _XINGLMC_H */







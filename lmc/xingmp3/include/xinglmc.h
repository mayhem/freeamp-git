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
#include "semaphore.h"

extern "C" {
#include "mhead.h"
#include "port.h"
	   }

#define BS_BUFBYTES 60000U
#define PCM_BUFBYTES 60000U

typedef enum {
    XING_Stop = 1,
    XING_Pause,
    XING_Resume
} XingCommand;

typedef struct {
    int (*decode_init) (MPEG_HEAD * h, int framebytes_arg,
			int reduction_code, int transform_code,
			int convert_code, int freq_limit);
    void (*decode_info) (DEC_INFO * info);
    IN_OUT(*decode) (unsigned char *bs, short *pcm);
} AUDIO;

enum {
    lmcError_MinimumError = 0x00010000,
    lmcError_HeadInfoReturnedZero,
    lmcError_AudioDecodeInitFailed,
    lmcError_BSFillFailed,
    lmcError_DecoderThreadFailed,
    lmcError_OutputWriteFailed,
    lmcError_DecodeDidntDecode,
    lmcError_ID3ReadFailed,
    lmcError_OutputInitializeFailed,
    lmcError_MaximumError
};

class XingLMC : public LogicalMediaConverter {

public:
//	XingLMC(PhysicalMediaInput* input, PhysicalMediaOutput* output,EventCallback callback, void* cookie);
    XingLMC();
    
    virtual ~XingLMC();
    
    virtual Error Decode();
    virtual Error Stop();
    virtual Error Pause();
    virtual Error Resume();
    virtual Error Reset();
    virtual Error ChangePosition(int32 position);

    virtual Error SetPMI(PhysicalMediaInput *);
    virtual Error SetPMO(PhysicalMediaOutput *);
    virtual Error SetTarget(EventQueue *);
    virtual Error InitDecoder();
    virtual const char *GetErrorString(int32);

private:
    static void DecodeWorkerThreadFunc(void *);
    void DecodeWork();

    int32 bs_fill();
    void bs_clear();
private:
	uint32					m_originalSearchAhead; // holds orignal bytes skipped for use when doing a ChangePosition in the poor manner
	uint32					m_searchAhead;  // holds bytes to skip in bsbuffer when calling audio.decode
    bool                    m_properlyInitialized;
    int32                   m_frameWaitTill;
    Semaphore *             m_pauseSemaphore;
    AUDIO                   m_audioMethods;
    EventQueue *            m_target;
    Mutex*                  m_seekMutex;
    Queue<XingCommand*>*    m_xcqueue;
    PhysicalMediaInput *    m_input;
    PhysicalMediaOutput *   m_output;
    
    int32                   m_bsBufBytes;
    int32                   m_bsTrigger;
    unsigned char*          m_bsBuffer;
    unsigned char*          m_bsBufPtr;

    unsigned char*          m_pcmBuffer;
    uint32                  m_pcmBufBytes;
    uint32                  m_pcmTrigger;

    int32                   m_frameBytes;
    bool                    m_isPaused;
    Thread*                 m_decoderThread;

    int32                   m_frameCounter;
};

#endif /* _XINGLMC_H */







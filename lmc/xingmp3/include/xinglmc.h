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
#include <time.h>

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

extern    "C"
{
#include "mhead.h"
#include "port.h"
}

#define BS_BUFBYTES 60000U
#define PCM_BUFBYTES 60000U

typedef enum
{
   XING_Stop = 1,
   XING_Pause,
   XING_Resume
}
XingCommand;

typedef struct
{
   int       (*decode_init) (MPEG_HEAD * h, int framebytes_arg,
              int reduction_code, int transform_code,
              int convert_code, int freq_limit);
   void      (*decode_info) (DEC_INFO * info);
             IN_OUT(*decode) (unsigned char *bs, short *pcm);
}
AUDIO;

enum
{
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

class     XingLMC:public LogicalMediaConverter
{

   public:
//      XingLMC(PhysicalMediaInput* input, PhysicalMediaOutput* output,EventCallback callback, void* cookie);
   XingLMC();

   virtual ~ XingLMC();

   virtual Error Decode();
   virtual Error Stop();
   virtual Error Pause();
   virtual Error Resume();
   virtual Error Reset();
   virtual Error ChangePosition(int32 position);

   virtual bool  CanDecode();
   virtual bool  IsStreaming();
   virtual Error ExtractMediaInfo(MediaInfoEvent **);

   virtual Error SetPMI(PhysicalMediaInput *);
   virtual Error SetPMO(PhysicalMediaOutput *);
   virtual Error SetTarget(EventQueue *);
   virtual Error InitDecoder();
   virtual const char *GetErrorString(int32);


   virtual Error SetEQData(float *);
   virtual Error SetEQData(bool);


   virtual Error SetPropManager(Properties *p) { m_propManager = p; if (p) return kError_NoErr; else return kError_UnknownErr; }
 private:
   Properties *m_propManager;

   static void DecodeWorkerThreadFunc(void *);
   void        DecodeWork();
	Error       BeginRead(void *&pBuffer, unsigned int iBytesNeeded);
	Error       AdvanceBufferToNextFrame();
	Error       GetHeadInfo();

   bool        m_properlyInitialized;
   int32       m_frameWaitTill;
   Semaphore  *m_pauseSemaphore;
   AUDIO       m_audioMethods;
   EventQueue *m_target;
   Mutex      *m_seekMutex;
   Queue <XingCommand *> *m_xcqueue;
   PhysicalMediaInput *m_input;
   PhysicalMediaOutput *m_output;

   unsigned char *m_pcmBuffer;
   uint32    m_pcmBufBytes;
   uint32    m_pcmTrigger;

   int       m_frameBytes;
	MPEG_HEAD m_sMpegHead;
	int32     m_iBitRate;
   bool      m_isPaused, m_bBufferingUp;
   Thread   *m_decoderThread;

   int32     m_frameCounter;
	time_t    m_iBufferUpdate;

    bool m_enableEQ;
    float m_equalizer[32];

};

#endif /* _XINGLMC_H */





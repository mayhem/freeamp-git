
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


#ifndef _SOUNDCARDPMO_H_
#define _SOUNDCARDPMO_H_

/* system headers */
#include <stdlib.h>
#include <windows.h>

/* project headers */
#include "config.h"
#include "pmo.h"
#include "thread.h"
#include "mutex.h"
#include "properties.h"
#include "eventbuffer.h"

#define BIT_SELECT  0x1f
#define SLEEPTIME   256

static const uint32 OBUFFERSIZE = 2 * 1152;


class SoundCardPMO : public PhysicalMediaOutput, public EventBuffer
{

public:
    SoundCardPMO();
    virtual ~SoundCardPMO();
    
    virtual Error Init(OutputInfo* info);
    virtual Error Write(int32&,void*,int32);
    virtual Error Pause();
    virtual Error Resume();
    virtual void  WaitToQuit();
    virtual Error Clear();

    virtual Error SetPropManager(Properties *p);

    static void   StartWorkerThread(void *);
    virtual Error BeginWrite(void *&pBuffer, size_t &iBytesToWrite);
    virtual Error EndWrite  (size_t iNumBytesWritten);
    virtual Error AcceptEvent(Event *);
    virtual int   GetBufferPercentage();

    
 private:
	WAVEHDR* NextHeader();

	void          WorkerThread(void); 
    virtual Error Reset(bool user_stop);
    void          HandleTimeInfoEvent(PMOTimeInfoEvent *pEvent);

 private:
	Properties *    m_propManager;
	WAVEFORMATEX*	m_wfex;
	LPWAVEHDR*		m_wavehdr_array;
	HWAVEOUT		m_hwo;

	uint32			m_index;
	uint32			m_buffer[MAXCHANNELS];
	uint32			m_channels;
	uint32          m_samples_per_second;
	uint32			m_buffer_count;
	uint32			m_hdr_size;
	uint32			m_fillup;
	uint32			m_data_size;
	uint32			m_num_headers;
	bool			m_user_stop;
	bool			m_initialized;

    Thread         *m_pBufferThread;
    Semaphore      *m_pPauseSem;
    bool            m_bPause;
    int             m_iOutputBufferSize, m_iTotalBytesWritten, m_iBytesPerSample;
    int             m_iLastFrame;
};

#endif /* _SOUNDCARDPMO_H_ */




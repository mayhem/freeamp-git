
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


#define BIT_SELECT  0x1f
#define SLEEPTIME   256

static const uint32 OBUFFERSIZE = 2 * 1152;


class SoundCardPMO : public PhysicalMediaOutput{

public:
    SoundCardPMO();
    virtual ~SoundCardPMO();
    
    virtual Error Init(OutputInfo* info);
    virtual Error Reset(bool user_stop);
    virtual int32 Write(void*,int32);
    virtual Error Pause();
    virtual Error Resume();
    
    
 private:
	WAVEHDR* NextHeader();

private:
	WAVEFORMATEX*	m_wfex;
	LPWAVEHDR*		m_wavehdr_array;
	HWAVEOUT		m_hwo;

	uint32			m_index;
	uint32			m_buffer[MAXCHANNELS];
	uint32			m_channels;
	uint32			m_buffer_count;
	uint32			m_hdr_size;
	uint32			m_fillup;
	uint32			m_data_size;
	uint32			m_num_headers;
	bool			m_user_stop;
	bool			m_initialized;


    
};

#endif /* _SOUNDCARDPMO_H_ */




/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998 GoodNoise
	Portions Copyright (C) 1997 Jeff Tsay (ctsay@pasteur.eecs.berkeley.edu)

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

/* system headers */
#include <windows.h>
#include <stdlib.h>

/* project headers */
#include "config.h"
#include "SoundCardPMO.h"

HANDLE MCISemaphore;

extern "C" {

PhysicalMediaOutput* Initialize() 
{
    return new SoundCardPMO();
}

}

static 
void 
CALLBACK 
MCICallBack(HWAVEOUT hwo, 
			UINT msg,
			DWORD dwInstance,
			DWORD dwParam1,
			DWORD dwParam2)
{
	switch(msg)
	{
		case WOM_DONE:
		{
			LPWAVEHDR lpWaveHdr = (LPWAVEHDR) dwParam1;
			waveOutUnprepareHeader(hwo, lpWaveHdr, sizeof(WAVEHDR));
			//cerr << "Releasing: WAVHDR #" << lpWaveHdr->dwUser << endl;
			lpWaveHdr->dwUser = 0;
			ReleaseSemaphore(MCISemaphore, 1, NULL);
            break;
		}
	}
}

SoundCardPMO::
SoundCardPMO()
{
	m_wfex				= NULL;
	m_wavehdr_array		= NULL;
	m_hwo				= NULL;

	m_index				= 0;
	m_channels			= 0;
	m_buffer_count		= 0;			
	m_hdr_size			= 0;
	m_fillup			= 0;
	m_data_size			= 0;
	m_user_stop			= false;
	m_initialized		= false;
}

SoundCardPMO::
~SoundCardPMO()
{

    if(m_initialized)
	{
        char* buf = new char [m_data_size];
        int32 wrote;

        memset(buf, 0x00, m_data_size);

        //for(int32 i = 0; i < 5; i++)
       // {
            Write(wrote, buf, m_data_size);
        //}

        delete [] buf;

        while(waveOutClose(m_hwo) == WAVERR_STILLPLAYING)
		{
			Sleep(SLEEPTIME);
		}

		CloseHandle(MCISemaphore);

		// Unprepare and free the header memory.
		for (uint32 j = 0; j < m_num_headers; j++) 
		{
			delete [] m_wavehdr_array[j]->lpData;
			delete m_wavehdr_array[j];
		}

		delete [] m_wavehdr_array;
		delete m_wfex;
	}
}


Error 
SoundCardPMO::
Init(OutputInfo* info)
{
	Error		result		= kError_UnknownErr;
	MMRESULT	mmresult	= 0;

	m_channels				= info->number_of_channels;
	m_data_size				= info->max_buffer_size;

	m_num_headers			= 20;
	m_hdr_size				= sizeof(WAVEHDR);
	m_wavehdr_array			= new LPWAVEHDR[m_num_headers];

	m_fillup				= 0;
	m_user_stop				= false;

	m_wfex					= new WAVEFORMATEX;

	m_wfex->wBitsPerSample  = info->bits_per_sample;
	m_wfex->wFormatTag      = WAVE_FORMAT_PCM;
	m_wfex->nChannels       = (WORD) m_channels;
	m_wfex->nSamplesPerSec  = info->samples_per_second;
	m_wfex->nAvgBytesPerSec = m_channels * info->samples_per_second << 1;
	m_wfex->nBlockAlign     = (WORD) (m_channels << 1);
	m_wfex->cbSize          = 0;

	mmresult = waveOutOpen(	&m_hwo, 
							WAVE_MAPPER, 
							m_wfex,
							(DWORD) MCICallBack, 
							NULL,
							WAVE_ALLOWSYNC | CALLBACK_FUNCTION);

	if(mmresult == MMSYSERR_NOERROR)
	{
		result = kError_NoErr;
	}

	uint32 i;

	for(i = 0; i < m_num_headers; i++) 
	{
		m_wavehdr_array[i] = new WAVEHDR;

		LPWAVEHDR temp = m_wavehdr_array[i];

		if(!temp) 
		{
			result = kError_OutOfMemory;
			//cerr << "error allocating WAVEHDR" << endl;
			break;
		}

		temp->lpData = new char[m_data_size];

		if(!temp->lpData)
		{
			result = kError_OutOfMemory;
			break;
		}

		temp->dwBufferLength  = m_data_size;
		temp->dwBytesRecorded = 0;
		temp->dwUser          = 0;  // If played, dwUser = 1
		temp->dwLoops         = 0;
		temp->dwFlags         = NULL;

		//cerr << i << ": dwUser = " << temp->dwUser << endl;
	}

	for(i = 0; i < m_channels; i++)
	{
	   m_buffer[i] = i * m_channels;
	}

	MCISemaphore = CreateSemaphore(NULL, m_num_headers, m_num_headers, "MCISemaphore");

	m_initialized = true; 

	return result;
}

Error 
SoundCardPMO::
Reset(bool user_stop)
{
	Error result = kError_NoErr;

	if(user_stop)
	{
		m_user_stop = user_stop;

		waveOutReset(m_hwo);
	}

	return result;
}

Error 
SoundCardPMO::
Write(int32& wrote, void *pBuffer,int32 length) 
{
    Error result = kError_NoErr;

	//cerr << "WriteThis: " << length << " bytes" << endl;
	WaitForSingleObject(MCISemaphore, 10000);

	WAVEHDR* wavhdr = NULL;

	wavhdr = NextHeader();

	if(length > (int32)m_data_size)
    {
		length = m_data_size;
        result = kError_UnknownErr;
    }

	wavhdr->dwBufferLength = length;
	memcpy(wavhdr->lpData, pBuffer, length);

	// Prepare & write newest header
	waveOutPrepareHeader(m_hwo, wavhdr, m_hdr_size);
	waveOutWrite(m_hwo, wavhdr, m_hdr_size);
	
	//cerr << "Wrote: " << length << " bytes" << endl;
    wrote = length;
    return result;
}

WAVEHDR* 
SoundCardPMO::
NextHeader()
{
	WAVEHDR* result = NULL;


	do
	{
		if(m_index >= m_num_headers)
			m_index = 0;
		
		if(!m_wavehdr_array[m_index]->dwUser)
		{
			result = m_wavehdr_array[m_index++];
			result->dwUser = m_index;
			break;
		}

	}while(m_wavehdr_array[m_index++]->dwUser);


	return result;
}

Error 
SoundCardPMO::
Pause()
{
     waveOutPause(m_hwo);

     return kError_NoErr;
}

Error 
SoundCardPMO::
Resume()
{
    waveOutRestart(m_hwo);

    return kError_NoErr;
}
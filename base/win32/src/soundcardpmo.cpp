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
			waveOutUnprepareHeader(&hwo, lpWaveHdr, sizeof(WAVEHDR));
			//cerr << "Releasing: WAVHDR #" << lpWaveHdr->dwUser << endl;
			lpWaveHdr->dwUser = 0;
			ReleaseSemaphore(MCISemaphore, 1, NULL);
		}
      break;
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
	if(m_user_stop) 
	{
		waveOutReset(m_hwo);
	} 
	else if(m_initialized) 
	{
   		if (m_fillup == 1) 
		{

			// Write the last header calculated (at the top of the array).
			waveOutPrepareHeader(m_hwo, m_wavehdr_array[0], m_hdr_size);
			waveOutWrite(m_hwo, m_wavehdr_array[0], m_hdr_size);

			// Header has been written.
			m_wavehdr_array[0]->dwUser = 1;
		}

		if (m_buffer_count) 
		{

			// Write the last wave header (probably not be written due to buffer
			//	size increase.)

			for(uint32 i = m_buffer[m_channels - 1]; i < m_data_size; i++)
			{
				 m_wavehdr_array[2]->lpData[i] = (char) 0;
			}

			waveOutPrepareHeader(m_hwo, m_wavehdr_array[2], m_hdr_size);
			waveOutWrite(m_hwo, m_wavehdr_array[2], m_hdr_size);

			// Header has been written.
			m_wavehdr_array[2]->dwUser = 1;
			wave_swap();
  		}
	}

	if(m_initialized)
	{
		// Unprepare and free the header memory.
		for (int32 j = 2; j >= 0; j--) 
		{
			if (m_wavehdr_array[j]->dwUser && !m_user_stop)
			{
				WaitForSingleObject(MCISemaphore, 10000);
			}

			delete [] m_wavehdr_array[j]->lpData;
			delete m_wavehdr_array[j];
		}

		delete [] m_wavehdr_array;
		delete m_wfex;

		while(waveOutClose(m_hwo) == WAVERR_STILLPLAYING)
		{
			Sleep(SLEEPTIME);
		}

		CloseHandle(MCISemaphore);
	}
}


bool 
SoundCardPMO::
Initialize(OutputInfo* info)
{
	bool		result		= false;
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
		result = true;
	}

	uint32 i;

	for(i = 0; i < m_num_headers; i++) 
	{
		m_wavehdr_array[i] = new WAVEHDR;

		LPWAVEHDR temp = m_wavehdr_array[i];

		if(!temp) 
		{
			result = false;
			cerr << "error allocating WAVEHDR" << endl;
			break;
		}

		temp->lpData = new char[m_data_size];

		if(!temp->lpData)
		{
			result = false;
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

	MCISemaphore = CreateSemaphore(NULL, m_num_headers, 1024, "MCISemaphore");

	m_initialized = true; 

	return result;
}

bool 
SoundCardPMO::
Reset(bool user_stop)
{
	bool result = false;

	if(user_stop)
	{
		m_user_stop = user_stop;

		//waveOutReset(m_hwo);
	}

	return result;
}

void 
SoundCardPMO::
Append(uint32 channel, int16 value)
{
	char* temp = m_wavehdr_array[2]->lpData;

	temp[m_buffer[channel]]		= (char) (value & 0xff);
	temp[m_buffer[channel] + 1] = (char) (value >> 8);

    m_buffer[channel] += m_channels << 1;
}

int32 
SoundCardPMO::
WriteThis(void *pBuffer,int32 length) 
{

	cerr << "WriteThis: " << length << " bytes" << endl;
	WaitForSingleObject(MCISemaphore, 10000);

	WAVEHDR* wavhdr = NULL;

	wavhdr = NextHeader();

	if(length > (int32)m_data_size)
		length = m_data_size;

	wavhdr->dwBufferLength = length;
	memcpy(wavhdr->lpData, pBuffer, length);

	// Prepare & write newest header
	waveOutPrepareHeader(m_hwo, wavhdr, m_hdr_size);
	waveOutWrite(m_hwo, wavhdr, m_hdr_size);
	
	cerr << "Wrote: " << length << " bytes" << endl;
    return length;
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

int32 
SoundCardPMO::
Write()
{
	int32 result = 0;

	// Actually write only when m_buffer is actually full.
	if ((m_buffer_count & BIT_SELECT) == 0) 
	{
		m_buffer_count = 0;

		// Wait for 2 completed headers
		if (m_fillup > 1) 
		{
			// Prepare & write newest header
			waveOutPrepareHeader(m_hwo, m_wavehdr_array[2], m_hdr_size);
			waveOutWrite(m_hwo, m_wavehdr_array[2], m_hdr_size);

			// Header has now been sent
			m_wavehdr_array[2]->dwUser = 1;

			wave_swap();

			// Unprepare oldest header
			if (m_wavehdr_array[2]->dwUser) 
			{
			   WaitForSingleObject(MCISemaphore, 10000);
			}

		} 
		else if (++m_fillup == 2) 
		{
			// Write the previously calculated 2 headers
			waveOutPrepareHeader(m_hwo, m_wavehdr_array[0], m_hdr_size);
			waveOutWrite(m_hwo, m_wavehdr_array[0], m_hdr_size);

			// Header has now been sent
			m_wavehdr_array[0]->dwUser = 1;

			wave_swap();

			waveOutPrepareHeader(m_hwo, m_wavehdr_array[0], m_hdr_size);
			waveOutWrite(m_hwo, m_wavehdr_array[0], m_hdr_size);

			// Header has now been sent
			m_wavehdr_array[0]->dwUser = 1;

		} 
		else 
		{
			wave_swap();
		}

		for(uint32 i=0; i<m_channels; i++)
		{
			m_buffer[i] = i * m_channels;
		}
	}

	return result;
}
void 
SoundCardPMO::
Clear()
{
	waveOutReset(m_hwo);

	if(m_initialized)
	{
		uint32 i;

		for(i = 0; i < 3; i++) 
		{
			LPWAVEHDR temp = m_wavehdr_array[i];

			if ((temp->dwUser) && (i < 2))
			{
				WaitForSingleObject(MCISemaphore, 10000);
			}

			temp->dwUser = 0;

			for(uint32 j = 0; j < m_data_size; j++)
			{
				temp->lpData[j] = (char) 0;
			}
		}

		// Reset m_buffer pointers
		for(i = 0; i < m_channels; i++)
		{
			m_buffer[i] = i * m_channels;
		}

		// Force the m_buffers to m_fillup before playing.
		m_fillup = m_buffer_count = 0;
	}
}

void 
SoundCardPMO::
wave_swap()
{
	LPWAVEHDR temp   = m_wavehdr_array[2];
	m_wavehdr_array[2] = m_wavehdr_array[1];
	m_wavehdr_array[1] = m_wavehdr_array[0];
	m_wavehdr_array[0] = temp;
}


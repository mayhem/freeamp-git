
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
	
	$Id: dsoundcardpmo.h,v 1. 1998/11/11 00:00:00 sr
____________________________________________________________________________*/

/* system headers */
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

/* project headers */
#include "config.h"
#include "win32thread.h"
#include "semaphore.h"
#include "DSoundCardPMO.h"

#define BUFFER_DURATION				2000		/*   2 secs   */
#define POLLTIME					100			/* 100 msecs  */               
#define SLEEPTIME					50			/*  50 msecs  */               
#define MSG_EXIT_THREAD				WM_USER

char DebugStr[255];
#define	OUTPUTDEBUGVALUE(str, value)			\
	sprintf(DebugStr, "%s %d\n", str, value);	\
	OutputDebugString((LPCTSTR) DebugStr);
#define	OUTPUTDEBUG(str)						\
	OutputDebugString((LPCTSTR) DebugStr);


extern "C" {

PhysicalMediaOutput* Initialize() 
{
    return new DSoundCardPMO();
}

} 
 
static 
bool 
CALLBACK
EnumThreadWndProc(	HWND hwnd, 
					LPARAM lParam )
{
	DSoundCardPMO	*pDSoundCardPmo = (DSoundCardPMO *)lParam;
 	pDSoundCardPmo->m_hMainWndHandle = hwnd;
	return TRUE;
}

static 
bool 
CALLBACK
DSEnumCallback(	LPGUID	lpGuid,
				LPSTR	strDescription,
				LPSTR	strModule,
				LPVOID	lpContext)
{
	DSDevice	*pDSDevice;
	HRESULT		hResult;
	size_t		size = 0;

	DSoundCardPMO	*pDSoundCardPmo = (DSoundCardPMO *)lpContext;

	if (!pDSoundCardPmo)
		return FALSE;

	if (pDSoundCardPmo->m_pDSDevices)
		size = _msize( pDSoundCardPmo->m_pDSDevices );

	pDSDevice = (DSDevice *)realloc(pDSoundCardPmo->m_pDSDevices, size + sizeof(DSDevice));

	if (pDSDevice == NULL)
	{
		OUTPUTDEBUG("error reallocating DSDevices memory...");
		return FALSE;
	}

	pDSoundCardPmo->m_pDSDevices = pDSDevice;
	pDSDevice = &pDSoundCardPmo->m_pDSDevices[pDSoundCardPmo->m_nNbDSDevices];

	if (lpGuid)
	{
		pDSDevice->pGuid = (GUID *)malloc(sizeof(GUID));
		memcpy(pDSDevice->pGuid, lpGuid, sizeof(GUID));
	}
	else
		pDSDevice->pGuid = NULL;

	pDSDevice->szName = strdup(strModule);
	pDSDevice->szDescription = strdup(strDescription);

	hResult = DirectSoundCreate(pDSDevice->pGuid, &pDSDevice->pDSObject, NULL);
	if (FAILED(hResult))
	{
		OUTPUTDEBUG("error creating DirectSound Object...");
		if (lpGuid)
			free((void *) pDSDevice->pGuid);

		free((void *) pDSDevice->szName);
		free((void *) pDSDevice->szDescription);

		pDSDevice = (DSDevice *)realloc(pDSoundCardPmo->m_pDSDevices, size);

		if (pDSDevice != NULL)
			pDSoundCardPmo->m_pDSDevices = pDSDevice;

		return FALSE;
	}

    pDSDevice->lCreated = 0;

	pDSoundCardPmo->m_nNbDSDevices++;

    return TRUE;
}

DSoundCardPMO::
DSoundCardPMO()
{
	HRESULT hResult;

	m_wfex				= NULL;
	m_user_stop			= false;
	m_bDSInitialized	= false;
	m_bLMCHasPaused		= FALSE;
	m_nNbDSDevices		= 0;
	m_pDSDevices		= NULL;
	m_nCurrentDevice	= -1;
	m_bDSEnumFailed		= FALSE;
	m_hMainWndHandle	= NULL;

	m_DSBufferMonitor.pDSBMThread		= NULL;
	m_DSBufferMonitor.pBufferSemaphore	= NULL;
	m_DSBufferMonitor.pWriteSemaphore	= NULL;

	// Get a list of windows handle
	BOOL breturn = EnumWindows((WNDENUMPROC) EnumThreadWndProc, (LPARAM) this); 
	// get a list of devices
	hResult = DirectSoundEnumerate((LPDSENUMCALLBACK) DSEnumCallback, this);

	if (FAILED(hResult))
	{
		m_bDSEnumFailed	= TRUE;
	}
}

#define TYPICAL_DELETE(x) if (x) { delete x; x = NULL; }

DSoundCardPMO::
~DSoundCardPMO()
{
	int	i;

    if(m_bDSInitialized)
	{
		delete m_wfex;

        // ask the BufferMonitor thread to exit
        m_DSBufferMonitor.pDSBMThread->PostThreadMessage(MSG_EXIT_THREAD, 0, 0);

		// wait for the BufferMonitor thread to exit
		m_DSBufferMonitor.pDSBMThread->Join();
		TYPICAL_DELETE(m_DSBufferMonitor.pDSBMThread);

        //release the synchronisation Init object
        CloseHandle(m_DSBufferMonitor.hInitOKEvent);
		TYPICAL_DELETE(m_DSBufferMonitor.pBufferSemaphore);
		TYPICAL_DELETE(m_DSBufferMonitor.pWriteSemaphore);


        // delete the direct sound secondary buffer
        if (m_DSBufferManager.pDSSecondaryBuffer)
		{
		    IDirectSoundBuffer_Stop(m_DSBufferManager.pDSSecondaryBuffer);
		    IDirectSoundBuffer_Release(m_DSBufferManager.pDSSecondaryBuffer);
        }

		// Do I need to release primary as well ?
	}

	if (m_pDSDevices != NULL)
	{
		for (i=0;i<m_nNbDSDevices;i++)
		{
			if (&m_pDSDevices[i] != NULL)
			{
				// Should I destroy the DirectSound object as well ?

				if ((&m_pDSDevices[i])->pGuid)
				{
					free((void *) (&m_pDSDevices[i])->pGuid);
					(&m_pDSDevices[i])->pGuid = NULL;
				}

				free((void *) (&m_pDSDevices[i])->szName);
				(&m_pDSDevices[i])->szName = NULL;
				free((void *) (&m_pDSDevices[i])->szDescription);
				(&m_pDSDevices[i])->szDescription = NULL;
			}
		}
		if (m_pDSDevices)
		{
			free((void *) (m_pDSDevices));
			m_pDSDevices = NULL;
		}
	}
}


Error 
DSoundCardPMO::
Init(OutputInfo* info)
{
    HRESULT			hResult;
	DSBUFFERDESC	DSBufferInfo;
	Error			result	= kError_UnknownErr;

	if ((m_bDSEnumFailed) || (m_nNbDSDevices == 0) || (m_pDSDevices == NULL))
		return result;

	m_DSBufferManager.state				= UNDERFLOW;
	// Use Primary Sound Driver
	m_nCurrentDevice					= 0;
	m_DSBufferManager.pDSDevice			= &m_pDSDevices[m_nCurrentDevice];
	m_DSBufferManager.pDSSecondaryBuffer= NULL;
	
	// What if the Callback hasn't finished yet here ?
	// Do it only if it didn't happen already
	if (InterlockedExchange(&m_DSBufferManager.pDSDevice->lCreated,1) == 0)
	{
		// the device was not initialized
   		// create primary buffer

		// Should set cooperative level here to set primary buffer format
		// but we need a handle to a window ... need to talk to Mark about that
		hResult = IDirectSound_SetCooperativeLevel(	m_DSBufferManager.pDSDevice->pDSObject,
													m_hMainWndHandle,
    												DSSCL_PRIORITY);
		if (FAILED(hResult))
		{
			OUTPUTDEBUG("error cannot set DSSCL_PRIORITY Cooperative Level...");
			return result;
		}

		memset(&DSBufferInfo, 0, sizeof(DSBUFFERDESC));
		DSBufferInfo.dwSize		= sizeof(DSBUFFERDESC);
		DSBufferInfo.dwFlags	= DSBCAPS_PRIMARYBUFFER;

		hResult = IDirectSound_CreateSoundBuffer(	m_DSBufferManager.pDSDevice->pDSObject,
													&DSBufferInfo,
													&m_DSBufferManager.pDSPrimaryBuffer,
													NULL);
		if (FAILED(hResult))
		{
			OUTPUTDEBUG("error Creating Primary Buffer...");
			return result;
		}

		// try to set the primary buffer to 44.1 stereo 16 bits
		if (SUCCEEDED(hResult))
		{
			WAVEFORMATEX format;

			format.wFormatTag		= WAVE_FORMAT_PCM;
			format.nChannels	    = 2; 
			format.nSamplesPerSec	= 44100;
			format.nAvgBytesPerSec  = 2*2*44100;
			format.nBlockAlign		= 2*2; 
			format.wBitsPerSample	= 16;
			format.cbSize           = 0;

			hResult = IDirectSoundBuffer_SetFormat(m_DSBufferManager.pDSPrimaryBuffer, &format);
			if (FAILED(hResult)) 
			{
				OUTPUTDEBUG("Cannot Set Primary Buffer Format...");
				return result;
			}
		}

	}


	// create synchronization objects
    m_DSBufferMonitor.hInitOKEvent		= CreateEvent(NULL, FALSE, FALSE, NULL);
    m_DSBufferMonitor.pWriteSemaphore	= new Semaphore(1);
    m_DSBufferMonitor.pBufferSemaphore	= new Semaphore(0);
    m_DSBufferManager.lStateLock		= FALSE;

	// create a BufferMonitor thread for the directsound circular buffer
    m_DSBufferMonitor.pDSBMThread = new win32Thread();
    if (!m_DSBufferMonitor.pDSBMThread->Create(DSoundCardPMO::DSBufferMonitorThreadProc,this))
	{
        return result;
    }
	else 
	{
		DWORD dwStatus = m_DSBufferMonitor.pDSBMThread->Join(m_DSBufferMonitor.hInitOKEvent, INFINITE);
        if (dwStatus == WAIT_FAILED) 
			return result;
    }

    // give thread high priority
    m_DSBufferMonitor.pDSBMThread->SetPriority(Critical);

	memset(&DSBufferInfo, 0, sizeof(DSBUFFERDESC));
	m_wfex							= new WAVEFORMATEX;

    // compute the levels and buffer size 
	// For now, this is fixed to 2 secondes but maybe should use info->max_buffer_size
	m_DSBufferManager.dwBufferSize	= (((info->bits_per_sample >> 3) * info->number_of_channels * 
										info->samples_per_second) * BUFFER_DURATION)/1000;
	m_DSBufferManager.dwOverflow	= 7 * (m_DSBufferManager.dwBufferSize >> 3);	/* 3/4 of max   */
	m_DSBufferManager.dwUnderflow	= m_DSBufferManager.dwBufferSize >> 3;			/* 1/8 of max   */
    m_DSBufferManager.dwTrigger		= m_DSBufferManager.dwBufferSize >> 1;			/* 1/2 of max   */
    m_DSBufferManager.dwZerofill	= m_DSBufferManager.dwBufferSize >> 1;			/* 1/2 of max   */

	m_wfex->wBitsPerSample			= info->bits_per_sample;
	m_wfex->wFormatTag				= WAVE_FORMAT_PCM;
	m_wfex->nChannels				= (WORD) info->number_of_channels;
	m_wfex->nSamplesPerSec			= info->samples_per_second;
	m_wfex->nAvgBytesPerSec			= info->number_of_channels * info->samples_per_second * 
										(info->bits_per_sample >> 3);
	m_wfex->nBlockAlign				= (WORD) (info->number_of_channels * (info->bits_per_sample >> 3));
	m_wfex->cbSize					= 0;

	DSBufferInfo.dwSize				= sizeof(DSBUFFERDESC);
	DSBufferInfo.dwFlags			= 	DSBCAPS_CTRLFREQUENCY       |
										DSBCAPS_CTRLPAN             |
										DSBCAPS_CTRLVOLUME          |
										DSBCAPS_GETCURRENTPOSITION2 |
										DSBCAPS_GLOBALFOCUS;
	DSBufferInfo.dwBufferBytes		= m_DSBufferManager.dwBufferSize;
	DSBufferInfo.lpwfxFormat		= m_wfex;

	// Create Secondary Buffer
	hResult = IDirectSound_CreateSoundBuffer(m_DSBufferManager.pDSDevice->pDSObject,
		                                    &DSBufferInfo,
		                                    &m_DSBufferManager.pDSSecondaryBuffer,
											NULL);
	if (FAILED(hResult))
	{
		OUTPUTDEBUG("error Creating Secondary Buffer...");
		m_DSBufferManager.pDSSecondaryBuffer = NULL;
		return result;
	}

	m_bDSInitialized = true; 

    // Clear the new buffer
    DSClear();

	// Release Semaphore so that Thread can start polling buffer state
	m_DSBufferMonitor.pBufferSemaphore->Signal();

	return kError_NoErr;
}

void 
DSoundCardPMO::
DSBufferMonitorThreadProc(void *lpParameter)
{
    DSoundCardPMO	*pDSoundCardPmo = (DSoundCardPMO *)lpParameter;
	bool			bTimeToDie = FALSE;

    // force the creation of a message queue
    {
        MSG message;
        PeekMessage(&message, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    }
    
    // signal the init event to say we're ready
    SetEvent(pDSoundCardPmo->m_DSBufferMonitor.hInitOKEvent);

	do 
	{
		MSG message;

		// process messages in the queue if any
		while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			switch(message.message)
			{
				case MSG_EXIT_THREAD:
				bTimeToDie = TRUE;
				break;
			}
		}

		// look at the circular buffer and change state if necessary
		pDSoundCardPmo->m_DSBufferMonitor.pBufferSemaphore->Wait();
		pDSoundCardPmo->DSMonitorBufferState();
		pDSoundCardPmo->m_DSBufferMonitor.pBufferSemaphore->Signal();

        // sleep between loops
        Sleep(POLLTIME);
	} while(!bTimeToDie);

    //return 0;
}

Error 
DSoundCardPMO::
DSWriteToSecBuffer(int32& wrote, void *pBuffer, int32 length) 
{
	HRESULT		hResult;
	LPVOID		ptr1;
	DWORD		dwBytes1;
	LPVOID		ptr2;
	DWORD		dwBytes2;
	Error		result	= kError_UnknownErr;

	//OUTPUTDEBUG("DSWriteToSecBuffer : ", length);

    // grab the write semaphore
	m_DSBufferMonitor.pWriteSemaphore->Wait();

	// get a lock to a memory region to write to
	hResult = IDirectSoundBuffer_Lock(	m_DSBufferManager.pDSSecondaryBuffer,
										m_DSBufferManager.dwWritePtr, 
										length,
										&ptr1, 
										&dwBytes1,
										&ptr2, 
										&dwBytes2,
										0);
	if (hResult == DSERR_BUFFERLOST)
	{
		hResult = IDirectSoundBuffer_Restore(m_DSBufferManager.pDSSecondaryBuffer);
		if (FAILED(hResult)) 
		{
			wrote = 0;
			return result;
		}
		hResult = IDirectSoundBuffer_Lock(	m_DSBufferManager.pDSSecondaryBuffer,
											m_DSBufferManager.dwWritePtr, 
											length,
											&ptr1, 
											&dwBytes1,
											&ptr2, 
											&dwBytes2,
											0);
	}

	if (FAILED(hResult))
	{
		wrote = 0;
		return result;
	}

	// copy the samples
	if (pBuffer)
		memcpy(ptr1, pBuffer, dwBytes1);
	else
		memset(ptr1, 0, dwBytes1);

	m_DSBufferManager.dwWritePtr += dwBytes1;
	if (m_DSBufferManager.dwWritePtr >= m_DSBufferManager.dwBufferSize)
	{
		m_DSBufferManager.dwWritePtr = 0;
	}
	if (ptr2)
	{
        if (pBuffer)
			memcpy(ptr2, (unsigned char *)pBuffer + dwBytes1, dwBytes2);
		else
			memset(ptr2, 0, dwBytes2);
		m_DSBufferManager.dwWritePtr = dwBytes2;
	}

	// unlock the memory region
	IDirectSoundBuffer_Unlock(m_DSBufferManager.pDSSecondaryBuffer, 
		                      ptr1, 
							  dwBytes1, 
							  ptr2, 
							  dwBytes2);

    // release the write semaphore
	m_DSBufferMonitor.pWriteSemaphore->Signal();
	
	//OUTPUTDEBUG("Wrote : ", length);
	wrote = length;
    return kError_NoErr;
}

Error 
DSoundCardPMO::
Write(int32& wrote, void *pBuffer, int32 length) 
{
    int	nState;

    // wait until we are in a normal state
    nState = DSMonitorBufferState();
    while(nState == -1 || nState == OVERFLOW || nState == STOPPING)
	{
		// Don't sleep if we paused, just return
		// (Hoping the LMC will repost the data...)
		if (m_bLMCHasPaused)
		{
			wrote = length;
			//maybe wrote = 0;
			return kError_NoErr;
		}

        Sleep(SLEEPTIME);
        nState = DSMonitorBufferState();
    }

    return DSWriteToSecBuffer(wrote, pBuffer, length);

}

int32
DSoundCardPMO::
DSGetNumSamplesBuffered()
{
	DWORD dwReadPos;
	DWORD dwWritePos;

	if (m_DSBufferManager.pDSSecondaryBuffer == NULL)
		return 0;

	IDirectSoundBuffer_GetCurrentPosition(m_DSBufferManager.pDSSecondaryBuffer,
		                                  &dwReadPos,
										  &dwWritePos);

	if (m_DSBufferManager.dwWritePtr >= dwReadPos)
	{
		return m_DSBufferManager.dwWritePtr - dwReadPos;
	}
	else
	{
		return (m_DSBufferManager.dwBufferSize - dwReadPos) +
			    m_DSBufferManager.dwWritePtr;
	}
}

int32 
DSoundCardPMO::
DSMonitorBufferState()
{
    DWORD		dwBuffered;
    DSState		new_state;
	int32		wrote;

    if (InterlockedExchange(&m_DSBufferManager.lStateLock, TRUE) == TRUE)
	{
        // this section of the code is locked
        return -1;
    }

    new_state = m_DSBufferManager.state;

    dwBuffered = DSGetNumSamplesBuffered();
	//OUTPUTDEBUG("Buffered : ", dwBuffered);

    switch(m_DSBufferManager.state)
	{
		case UNDERFLOW:
			OUTPUTDEBUGVALUE("UNDERFLOW    : ", dwBuffered*100/m_DSBufferManager.dwBufferSize);
			if (dwBuffered > m_DSBufferManager.dwTrigger)
			{
				if (dwBuffered >= m_DSBufferManager.dwBufferSize)
				{
					new_state = OVERFLOW;
				}
				else
				{
					new_state = NORMAL;
				}

				// restart the buffer
				IDirectSoundBuffer_Play(m_DSBufferManager.pDSSecondaryBuffer, 
										0, 
										0, 
										DSBPLAY_LOOPING);
				m_bLMCHasPaused = FALSE;
			}
		break;

		case NORMAL:
			OUTPUTDEBUGVALUE("NORMAL       : ", dwBuffered*100/m_DSBufferManager.dwBufferSize);
			if (dwBuffered < m_DSBufferManager.dwUnderflow)
			{
				new_state = STOPPING;
				DSWriteToSecBuffer(wrote, NULL, m_DSBufferManager.dwZerofill);
			}
			else if (dwBuffered >= m_DSBufferManager.dwOverflow)
			{
				new_state = OVERFLOW;
			}
			else
			{
				new_state = NORMAL;
			}
		break;

		case OVERFLOW:
			OUTPUTDEBUGVALUE("OVERFLOW     : ", dwBuffered*100/m_DSBufferManager.dwBufferSize);
			if (dwBuffered < m_DSBufferManager.dwUnderflow)
			{
				new_state = STOPPING;
				DSWriteToSecBuffer(	wrote, NULL, m_DSBufferManager.dwZerofill);
			}
			else if (dwBuffered < m_DSBufferManager.dwOverflow)
			{
				new_state = NORMAL;
			}
        break;

		case STOPPING:
			OUTPUTDEBUGVALUE("STOPPING     : ", dwBuffered*100/m_DSBufferManager.dwBufferSize);
			if (dwBuffered < m_DSBufferManager.dwZerofill)
			{
				DSReset();
				DSClear();
				new_state = UNDERFLOW;
			}
        break;
    }

    m_DSBufferManager.state = new_state;

    InterlockedExchange(&m_DSBufferManager.lStateLock, FALSE);
    
	//OUTPUTDEBUG("New State : ", new_state);
    return new_state;
}

Error 
DSoundCardPMO::
Pause()
{
    IDirectSoundBuffer_Stop(m_DSBufferManager.pDSSecondaryBuffer);
	m_bLMCHasPaused = TRUE;
	return kError_NoErr;
}

Error 
DSoundCardPMO::
Resume()
{
	Error result = kError_NoErr;
    IDirectSoundBuffer_Play(m_DSBufferManager.pDSSecondaryBuffer,
                            0, 
							0, 
							DSBPLAY_LOOPING);
	m_bLMCHasPaused = FALSE;

    return result;
}

Error 
DSoundCardPMO::
Reset(bool user_stop)
{
	Error result = kError_NoErr;

	if(user_stop)
	{
		m_user_stop = user_stop;
		m_DSBufferMonitor.pBufferSemaphore->Wait();
		DSReset();
		DSClear();
		m_DSBufferMonitor.pBufferSemaphore->Signal();
	}

	return result;
}

void 
DSoundCardPMO::
DSReset()
{
	if (m_DSBufferManager.pDSSecondaryBuffer)
	{
		IDirectSoundBuffer_Stop(m_DSBufferManager.pDSSecondaryBuffer);
	}
}

void 
DSoundCardPMO::
DSClear()
{
	HRESULT	hResult;
	LPVOID	ptr;
	DWORD	dwBytes;

	// grab the write semaphore
	m_DSBufferMonitor.pWriteSemaphore->Wait();

	// init the fields
	m_DSBufferManager.dwWritePtr= 0;
	m_DSBufferManager.state		= UNDERFLOW;

	// reset the play cursor
	IDirectSoundBuffer_SetCurrentPosition(m_DSBufferManager.pDSSecondaryBuffer, 0);

	// write zeros into secondary buffer
	hResult = IDirectSoundBuffer_Lock(m_DSBufferManager.pDSSecondaryBuffer,
									 0, 
									 m_DSBufferManager.dwBufferSize,
			     					 &ptr, 
									 &dwBytes,
				    				 NULL, 
									 NULL, 
									 0);
	if (SUCCEEDED(hResult))
	{
		memset(ptr, 0, m_DSBufferManager.dwBufferSize);
		IDirectSoundBuffer_Unlock(m_DSBufferManager.pDSSecondaryBuffer,
			     				  ptr, 
								  dwBytes,
				    			  NULL, 
								  0);
	}
	// release the write semaphore
	m_DSBufferMonitor.pWriteSemaphore->Signal();
}


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


#ifndef _DSOUNDCARDPMO_H_
#define _DSOUNDCARDPMO_H_

/* system headers */
#include <stdlib.h>
#include <windows.h>
#include <dsound.h>

/* project headers */
#include "config.h"
#include "win32thread.h"
#include "semaphore.h"
#include "pmo.h"

typedef enum {
    UNDERFLOW,
    NORMAL,
    OVERFLOW,
    STOPPING,
} DSState;

typedef struct {
	GUID				*pGuid;
	char				*szName;
	char				*szDescription;
	LPDIRECTSOUND		pDSObject;
    LONG				lCreated;
} DSDevice;

typedef struct {
    win32Thread*		pDSBMThread;
    HANDLE				hInitOKEvent;
	Semaphore*          pBufferSemaphore;	// Semaphore to access Sec Buffer for setting purpose
	Semaphore*          pWriteSemaphore;	// Semaphore to access Sec Buffer for writing purpose
} DSBufferMonitor;

typedef struct {
    long				lStateLock;
    DSState				state;
    DWORD				dwBufferSize;
	DWORD				dwWritePtr;
    DWORD				dwOverflow;
    DWORD				dwUnderflow;
    DWORD				dwTrigger;
    DWORD				dwZerofill;
	DSDevice			*pDSDevice;
	LPDIRECTSOUNDBUFFER	pDSPrimaryBuffer;
	LPDIRECTSOUNDBUFFER	pDSSecondaryBuffer;
} DSBufferManager;


class DSoundCardPMO : public PhysicalMediaOutput{

public:
    DSoundCardPMO();
    virtual ~DSoundCardPMO();
    
    virtual Error	Init(OutputInfo* info);
    virtual Error	Reset(bool user_stop);
    virtual Error	Write(int32&, void*, int32);
    virtual Error	Pause();
    virtual Error	Resume();

    static  void	DSBufferMonitorThreadProc(void *);
	virtual void	DSClear();
	virtual void	DSReset();
	virtual int32	DSMonitorBufferState();
	virtual int32	DSGetNumSamplesBuffered();
	virtual	Error	DSWriteToSecBuffer(int32&, void*, int32);

private:
	WAVEFORMATEX*	m_wfex;
	bool			m_user_stop;

	bool			m_bDSInitialized;
	bool			m_bLMCHasPaused;

public:
	HWND			m_hMainWndHandle;

	/* DirectSound objects */
	bool			m_bDSEnumFailed;
	int				m_nNbDSDevices;
	DSDevice		*m_pDSDevices;
	int				m_nCurrentDevice;

	DSBufferMonitor	m_DSBufferMonitor;
	DSBufferManager	m_DSBufferManager;
};

#endif /* _SOUNDCARDPMO_H_ */





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
#include <windows.h>
#include <dsound.h>
#include <stdlib.h>

/* project headers */
#include "config.h"
#include "pmo.h"
#include "thread.h"
#include "mutex.h"
#include "properties.h"
#include "eventbuffer.h"
#include "Semaphore.h"

typedef enum {
  UNDERFLOW,
  NORMAL,
  OVERFLOW,
  STOPPING,
} DSState;

typedef struct {
  GUID*         pGuid;
  char*         szName;
  char*         szDescription;
  LPDIRECTSOUND pDSObject;
  LONG          lCreated;
} DSDevice;

typedef struct {
  DSState       state;
  DWORD         dwBufferSize;
  DWORD         dwWritePtr;
  DWORD         dwOldReadPos;
  DWORD         dwOverflow;
  DWORD         dwUnderflow;
  DWORD         dwTrigger;
  DWORD         dwZerofill;
  DSDevice     *pDSDevice;

  LPDIRECTSOUNDBUFFER pDSPrimaryBuffer;
  LPDIRECTSOUNDBUFFER pDSSecondaryBuffer;
} DSBufferManager;


class DSoundCardPMO : public PhysicalMediaOutput, public EventBuffer
{

public:
  DSoundCardPMO();
  virtual ~DSoundCardPMO();

  virtual Error   Init(OutputInfo* info);
  virtual Error   Pause();
  virtual Error   Resume();
  virtual Error   Break();
  virtual void    WaitToQuit();
  virtual Error   Clear();

  virtual Error   SetPropManager(Properties *p);
  virtual void    SetVolume(int32);
  virtual int32   GetVolume(void);

  static  void    StartWorkerThread(void *);
  virtual Error   BeginWrite(void *&pBuffer, size_t &iBytesToWrite);
  virtual Error   EndWrite  (size_t iNumBytesWritten);
  virtual Error   AcceptEvent(Event *);
  virtual int     GetBufferPercentage();


private:
  void            WorkerThread(void);
  virtual Error   Reset(bool user_stop);
  void            HandleTimeInfoEvent(PMOTimeInfoEvent *pEvent);
  Error           GetData(void *&pBuffer);
  Error           Write(void *pBuffer);

  void            DSClear();
  void            DSReset();
  int32           DSMonitorBufferState();
  Error           DSWriteToSecBuffer(int32&, void*, int32);

private:
  Properties*     m_propManager;
  WAVEFORMATEX*   m_wfex;

  uint32          m_samples_per_second;
  uint32          m_data_size;
  bool            m_initialized;

  Thread*         m_pBufferThread;
  bool            m_bIsBufferEmptyNow;
  bool            m_bLMCsaidToPlay;
  int             m_iOutputBufferSize, m_iTotalBytesWritten, m_iBytesPerSample;
  int             m_iLastFrame;

  /* DirectSound objects */
  Semaphore*      m_pDSBufferSem; // Semaphore to access Sec Buffer for setting purpose
  Semaphore*      m_pDSWriteSem;  // Semaphore to access Sec Buffer for writing purpose
  bool            m_bDSEnumFailed;
  int             m_nCurrentDevice;

public:
  DSBufferManager m_DSBufferManager;
  HWND            m_hMainWndHandle;
  int             m_nNbDSDevices;
  DSDevice        *m_pDSDevices;
};

#endif /* _DSOUNDCARDPMO_H_ */




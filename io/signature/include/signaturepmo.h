/*____________________________________________________________________________

  FreeAmp - The Free MP3 Player

  Portions Copyright (C) 2000 Relatable

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


#ifndef INCLUDED_SIGNATUREPMO_H_
#define INCLUDED_SIGNATUREPMO_H_

/* project headers */
#ifdef WIN32
#include <windows.h>
#endif

#include "config.h"
#include "pmo.h"
#include "thread.h"
#include "mutex.h"
#include "properties.h"
#include "eventbuffer.h"
#include "semaphore.h"
#include "facontext.h"
#include "preferences.h"

#include "sigfft.h"

const int iFFTPoints = 32;
const int iMaxFFTs   = 9000;

class SignaturePMO : public PhysicalMediaOutput
{

public:
  SignaturePMO(FAContext *context);
  virtual ~SignaturePMO();

  virtual Error   Init(OutputInfo* info);

  void            Pause(void);
  void            Resume(void);
  void            Quit(void);

  virtual void    GetVolume(int32 &, int32 &);
  virtual void    SetVolume(int32, int32);

  static  void    StartWorkerThread(void *);

 private:

  void            WorkerThread(void);
  virtual Error   Reset(bool user_stop);
  Error           Write(void *pBuffer);

  void            Clear(void);

  uint32          m_samples_per_second, m_samples_per_frame;
  uint32          m_data_size;
  uint32          m_downmix_size;
  uint32          m_loopcount;
  uint32          m_maxloop;
  string          m_url;
  unsigned char  *m_downmixBuffer;

  Thread*         m_pBufferThread;
  bool            m_initialized;
  int             m_iTotalBytesWritten, m_iBytesPerSample;
  int             m_bits_per_sample;
  int             m_number_of_channels;
  int             m_iFinishedFFTs;

  int             m_iZeroCrossings;
  double          m_dEnergySum;
  bool            m_bLastNeg; 
  bool            m_bSetLastNeg;
  int             m_iSpectrum[iFFTPoints];
  FFT            *m_pFFT;
};

#endif /* INCLUDED_SIGNATUREPMOO_H_ */

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

#ifndef _FILEBUFFER_H_
#define _FILEBUFFER_H_

/* system headers */
#include <stdlib.h>

/* project headers */
#include "errors.h"
#include "thread.h"
#include "semaphore.h"
#include "pullbuffer.h"

const int32 iMaxFileNameLen = 255;
const int32 iID3TagSize = 128;

class FAContext;

class FileBuffer : public PullBuffer
{
    public:

               FileBuffer(size_t iBufferSize, size_t iOverFlowSize,
                          size_t iWriteTriggerSize, char *szFile,
			  FAContext *context);
      virtual ~FileBuffer(void);

      Error    Open(void);
      Error    Run(void);

      Error    Seek(int32 iRet, int32 iPos, int32 iFrom);
      Error    GetLength(size_t &iSize)
               {
                  iSize = m_iFileSize;
                  return kError_NoErr;
               };
      void     Clear(void);

      Error    GetID3v1Tag(unsigned char *pTag);

      static   void     StartWorkerThread(void *);

    private:

      FILE           *m_fpFile;
      char            m_szFile[iMaxFileNameLen];
      Thread         *m_pBufferThread;
      bool            m_bLoop, m_bExit;
      size_t          m_iFileSize;
      unsigned char  *m_pID3Tag;

    public:

      void            WorkerThread(void);
};

#endif

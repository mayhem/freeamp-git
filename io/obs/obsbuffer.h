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

#ifndef _OBSBUFFER_H_
#define _OBSBUFFER_H_

/* system headers */
#include <stdlib.h>

/* project headers */
#include "errors.h"
#include "thread.h"
#include "semaphore.h"
#include "streambuffer.h"

const int iMaxUrlLen = 1024;
const int iMaxErrorLen = 1024;
const int iMAX_PACKET_SIZE = 8192;
const int iSTREAM_EOS = 0x02;
const int iMAX_RETRYS = 10;

struct ID3Tag
{
   char szTag[3];
   char szTitle[30];
   char szArtist[30];
   char szAlbum[30];
   char szYear[4];
   char szComment[30];
   char cGenre;
}; 

struct RTPHeader
{
    int32 iFlags;
    int32 iTimestamp;
    int32 iSyncSourceId;
};

enum
{
   obsError_MinimumError = 0x00010000,
   obsError_BadUrl,
   obsError_CannotSetSocketOpts,
   obsError_CannotCreateSocket,
   obsError_CannotBind,
   obsError_SocketRead,
   obsError_MaximumError
};

class ObsBuffer : public StreamBuffer
{
    public:

               ObsBuffer(size_t iBufferSize, size_t iOverFlowSize,
                         size_t iWriteTriggerSize, char *szFile);
      virtual ~ObsBuffer(void);

      Error    Open(void);
      Error    ReadTest(void);
      Error    Run(void);

      Error    GetID3v1Tag(unsigned char *pTag);

      const char *GetErrorString(int32 error);
      static   void     StartWorkerThread(void *);

    private:


      int                 m_hHandle;
      char                m_szUrl[iMaxUrlLen], *m_szError;
      Thread             *m_pBufferThread;
      bool                m_bLoop;
      ID3Tag             *m_pID3Tag;
      struct sockaddr_in *m_pSin;
 

    public:

      void            WorkerThread(void);
};

#endif

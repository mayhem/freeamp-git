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

/* system headers */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <iostream.h>
#include <errno.h>
#include <assert.h>

#include <config.h>

#if HAVE_UNISTD_H
#include <unistd.h>
#elif HAVE_IO_H
#include <io.h>
#else
#error Must have unistd.h or io.h!
#endif // HAVE_UNISTD_H

/* project headers */
#include "httpinput.h"

const int iBufferSize = 8192;
const int iOverflowSize = 1536;
const int iTriggerSize = 1024;
const char *szDefaultStreamTitle = "SHOUTcast Stream";

extern    "C"
{
   PhysicalMediaInput *Initialize(FAContext *context)
   {
      return new HttpInput(context);
   }
}
HttpInput::HttpInput(FAContext *context):
           PhysicalMediaInput()
{
    m_context = context;
    m_path = NULL;
    m_pPullBuffer = NULL;
}

HttpInput::HttpInput(char *path):
           PhysicalMediaInput()
{
   if (path)
   {
      assert(0);
   }
   else
   {
      m_path = NULL;
      m_pPullBuffer = NULL;
   }
}

HttpInput::~HttpInput()
{
   if (m_path)
   {
      delete[]m_path;
      m_path = NULL;
   }
   if (m_pPullBuffer >= 0)
   {
      delete m_pPullBuffer;
      m_pPullBuffer = NULL;
   }
}

bool HttpInput::CanHandle(char *szUrl, char *szTitle)
{
   bool bRet;

   bRet = strncmp(szUrl, "http://", 7) == 0;

   if (szTitle && bRet)
      strcpy(szTitle, szDefaultStreamTitle);

   return bRet;
}

Error HttpInput::SetTo(char *url, bool bStartThread)
{
   Error     result = kError_NoErr;

   if (m_pPullBuffer)
   {
      delete m_pPullBuffer;
      m_pPullBuffer = NULL;
   }

   if (m_path)
   {
      delete[]m_path;
      m_path = NULL;
   }

   if (url)
   {
      int32     len = strlen(url) + 1;
      m_path = new char[len];

      if (m_path)
      {
         memcpy(m_path, url, len);
      }
      else
      {
         result = kError_OutOfMemory;
      }

      if (IsntError(result))
      {
         m_pPullBuffer = new HttpBuffer(iBufferSize, iOverflowSize, 
                                         iTriggerSize, url, this, m_context);
         assert(m_pPullBuffer);

         result = m_pPullBuffer->Open();
         if (result == kError_NoErr && bStartThread)
             result = m_pPullBuffer->Run();
      }
   }
   else
   {
      result = kError_InvalidParam;
   }

   return result;
}

Error HttpInput::
SetBufferSize(size_t iNewSize)
{
    return m_pPullBuffer->Resize(iNewSize, iNewSize / 6, iNewSize / 8);
}

Error HttpInput::
GetLength(size_t &iSize)
{
    iSize = 0;

    return kError_FileSeekNotSupported;
}

Error HttpInput::
GetID3v1Tag(unsigned char *pTag)
{
   assert(m_pPullBuffer);
    return m_pPullBuffer->GetID3v1Tag(pTag);
}

Error HttpInput::
BeginRead(void *&buf, size_t &bytesneeded)
{
   assert(m_pPullBuffer);
   return m_pPullBuffer->BeginRead(buf, bytesneeded);
}

Error HttpInput::
EndRead(size_t bytesused)
{
   assert(m_pPullBuffer);
   return m_pPullBuffer->EndRead(bytesused);
}

int32 HttpInput::GetBufferPercentage()
{
   assert(m_pPullBuffer);
   return m_pPullBuffer->GetBufferPercentage();
}

int32 HttpInput::GetNumBytesInBuffer()
{
   assert(m_pPullBuffer);
   return m_pPullBuffer->GetNumBytesInBuffer();
}

void HttpInput::Pause()
{
   if (m_pPullBuffer)
   {
      m_pPullBuffer->DidDiscardBytes();
      m_pPullBuffer->Pause();
   }
}

void HttpInput::Break()
{
   if (m_pPullBuffer)
       m_pPullBuffer->BreakBlocks();
}

bool HttpInput::Resume()
{
   bool bRet;

   if (!m_pPullBuffer)
      return false;

   bRet = m_pPullBuffer->DidDiscardBytes();

   m_pPullBuffer->Resume();

   return bRet;
}

Error     HttpInput::
Seek(int32 & rtn, int32 offset, int32 origin)
{
   return kError_FileSeekNotSupported;
}

Error     HttpInput::
Close(void)
{
   delete m_pPullBuffer;
   m_pPullBuffer = NULL;

   return kError_NoErr;
}

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
#include "obsinput.h"

const int iBufferSize = 8192;
const int iOverflowSize = 1024;
const int iTriggerSize = 1024;

extern    "C"
{
   PhysicalMediaInput *Initialize()
   {
      return new ObsInput();
   }
}
ObsInput::
ObsInput():
PhysicalMediaInput()
{
   m_path = NULL;
   m_pPullBuffer = NULL;
}

ObsInput::
ObsInput(char *path):
PhysicalMediaInput()
{
   if (path)
   {
      int32     len = strlen(path) + 1;
      m_path = new char[len];

      if (m_path)
      {
         memcpy(m_path, path, len);
      }

      m_pPullBuffer = new ObsBuffer(iBufferSize, iOverflowSize, 
                                      iTriggerSize, path);
      assert(m_pPullBuffer);

      // Hmmm. Error gets lost here. I don't want to mess with
      // the current infrastructure at the moment, and it doesn't
      // seem to be used...
      m_pPullBuffer->Open();
   }
   else
   {
      m_path = NULL;
      m_pPullBuffer = NULL;
   }
}

ObsInput::
~ObsInput()
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

bool ObsInput::
CanHandle(char *szUrl)
{
   return strncmp(szUrl, "rtp://", 6) == 0;
}

Error     ObsInput::
SetTo(char *url)
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
         m_pPullBuffer = new ObsBuffer(iBufferSize, iOverflowSize, 
                                         iTriggerSize, url);
         assert(m_pPullBuffer);

         result = m_pPullBuffer->Open();
         if (result == kError_NoErr)
            result = m_pPullBuffer->Run();
      }
   }
   else
   {
      result = kError_InvalidParam;
   }

   return result;
}

Error ObsInput::
SetBufferSize(size_t iNewSize)
{
    return m_pPullBuffer->Resize(iNewSize, iNewSize / 8, iNewSize / 8);
}

Error ObsInput::
GetLength(size_t &iSize)
{
    iSize = 0;

    return kError_FileSeekNotSupported;
}

Error ObsInput::
GetID3v1Tag(unsigned char *pTag)
{
    return m_pPullBuffer->GetID3v1Tag(pTag);
}

Error ObsInput::
BeginRead(void *&buf, size_t &bytesneeded)
{
   return m_pPullBuffer->BeginRead(buf, bytesneeded);
}

Error ObsInput::
EndRead(size_t bytesused)
{
   return m_pPullBuffer->EndRead(bytesused);
}

int32 ObsInput::
GetBufferPercentage()
{
   return m_pPullBuffer->GetBufferPercentage();
}

void ObsInput::
Pause()
{
   return m_pPullBuffer->Pause();
}

void ObsInput::
Resume()
{
   return m_pPullBuffer->Resume();
}

Error     ObsInput::
Seek(int32 & rtn, int32 offset, int32 origin)
{
   return kError_FileSeekNotSupported;
}

Error     ObsInput::
Close(void)
{
   delete m_pPullBuffer;
   m_pPullBuffer = NULL;

   return kError_NoErr;
}

const char *ObsInput::
GetErrorString(int32 error)
{
   if (m_pPullBuffer == NULL)
      return NULL;

   return m_pPullBuffer->GetErrorString(error);
}


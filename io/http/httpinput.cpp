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

const int iBufferSize = 65536;
const int iOverflowSize = 8192;
const int iTriggerSize = 8192;

extern    "C"
{
   PhysicalMediaInput *Initialize()
   {
      return new HttpInput();
   }
}
HttpInput::
HttpInput():
PhysicalMediaInput()
{
   m_path = NULL;
   m_pPullBuffer = NULL;
}

HttpInput::
HttpInput(char *path):
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

      m_pPullBuffer = new HttpBuffer(iBufferSize, iOverflowSize, 
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

HttpInput::
~HttpInput()
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


Error     HttpInput::
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
         m_pPullBuffer = new HttpBuffer(iBufferSize, iOverflowSize, 
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

Error HttpInput::
GetLength(size_t &iSize)
{
    iSize = 0;

    return kError_FileSeekNotSupported;
}

Error HttpInput::
GetID3v1Tag(unsigned char *pTag)
{
    if (m_pPullBuffer->GetID3v1Tag(pTag)) {
	return kError_NoErr;
    } else {
	return kError_UnknownErr;
    }
}

Error HttpInput::
BeginRead(void *&buf, size_t &bytesneeded)
{
   return m_pPullBuffer->BeginRead(buf, bytesneeded);
}

Error HttpInput::
EndRead(size_t bytesused)
{
   return m_pPullBuffer->EndRead(bytesused);
}

Error     HttpInput::
Seek(int32 & rtn, int32 offset, int32 origin)
{
    rtn = 0;

    // RAK: HACK HACK HACK HACK HACK HACK HACK!!!!!!
    if (offset == 0)
    {
        rtn = 0;
        return kError_NoErr;
    }

    return kError_FileSeekNotSupported;
}

Error     HttpInput::
Close(void)
{
   delete m_pPullBuffer;
   m_pPullBuffer = NULL;

   return kError_NoErr;
}

const char *HttpInput::
GetErrorString(int32 error)
{
   if (m_pPullBuffer == NULL)
      return NULL;

   return m_pPullBuffer->GetErrorString(error);
}


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
#include "localfileinput.h"
#include "log.h"

LogFile *g_Log;

const int32 iBufferSize = 65536;
const int32 iOverflowSize = 8192;
const int32 iTriggerSize = 8192;

extern    "C"
{
   PhysicalMediaInput *Initialize(LogFile *pLog)
   {
	  g_Log = pLog;
      return new LocalFileInput();
   }
}
LocalFileInput::LocalFileInput():
                PhysicalMediaInput()
{
   m_path = NULL;
   m_pPullBuffer = NULL;
}

LocalFileInput::LocalFileInput(char *path):
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

LocalFileInput::~LocalFileInput()
{
   if (m_path)
   {
      delete[]m_path;
      m_path = NULL;
   }
   if (m_pPullBuffer)
   {
      delete m_pPullBuffer;
      m_pPullBuffer = NULL;
   }
}

bool LocalFileInput::CanHandle(char *szUrl, char *szTitle)
{
   bool bRet;

   bRet = strncmp(szUrl, "file://", 7) == 0;
   if (szTitle && bRet)
      *szTitle = 0;

   return bRet;
}

Error     LocalFileInput::SetTo(char *url)
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
		   if (strncmp(url, "file://", 7) == 0)
             memcpy(m_path, url + 7, len);
         else
             memcpy(m_path, url, len);
      }
      else
      {
         result = kError_OutOfMemory;
      }

      if (IsntError(result))
      {
         m_pPullBuffer = new FileBuffer(iBufferSize, iOverflowSize, 
                                         iTriggerSize, m_path);
         assert(m_pPullBuffer);

         result = m_pPullBuffer->Open();
         if (!IsError(result))
         {
             result = m_pPullBuffer->Run();
             if (IsError(result))
                 ReportError("Could run the input plugin.");
         }
         else
             ReportError("Could not open the specified file.");


      }
   }
   else
   {
      result = kError_InvalidParam;
   }

   return result;
}

Error LocalFileInput::GetLength(size_t &iSize)
{
    return m_pPullBuffer->GetLength(iSize);
}

Error LocalFileInput::GetID3v1Tag(unsigned char *pTag)
{
	return m_pPullBuffer->GetID3v1Tag(pTag);
}

Error LocalFileInput::BeginRead(void *&buf, size_t &bytesneeded)
{
   return m_pPullBuffer->BeginRead(buf, bytesneeded);
}

Error LocalFileInput::EndRead(size_t bytesused)
{
   return m_pPullBuffer->EndRead(bytesused);
}

Error LocalFileInput::Seek(int32 & rtn, int32 offset, int32 origin)
{
   return m_pPullBuffer->Seek(rtn, offset, origin);
}

Error LocalFileInput::Close(void)
{
   delete m_pPullBuffer;
   m_pPullBuffer = NULL;

   return kError_NoErr;
}


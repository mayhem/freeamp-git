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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef WIN32
#include <errno.h>
#endif

#include "filebuffer.h"
#include "log.h"
//#include "debug.hpp"

extern LogFile *g_Log;

#define DB //printf("%s:%d\n", __FILE__, __LINE__);

FileBuffer::FileBuffer(size_t iBufferSize, size_t iOverFlowSize, 
                       size_t iWriteTriggerSize, char *szFile) :
            PullBuffer(iBufferSize, iOverFlowSize, iWriteTriggerSize)
{
    m_fpFile = NULL;
    m_bLoop = false;
    m_bExit = false;
    m_pBufferThread = NULL;
    m_pID3Tag = NULL;

    strcpy(m_szFile, szFile);
}

FileBuffer::~FileBuffer(void)
{
    if (m_fpFile)
       fclose(m_fpFile); 

    m_bExit = true;
    m_pWriteSem->Signal();

    if (m_pBufferThread)
    {
       m_pBufferThread->Join();
       delete m_pBufferThread;
    }

    if (m_pID3Tag)
       delete m_pID3Tag;
}

Error FileBuffer::Seek(int32 iRet, int32 iPos, int32 iFrom) 
{ 
    Error eRet = kError_NoErr;

    iRet = fseek(m_fpFile, iPos, iFrom);
    if (iRet < 0)
       eRet = kError_SeekFailed;
    else
       iRet = ftell(m_fpFile);

    PullBuffer::Clear();

    return eRet;
}

void FileBuffer::Clear(void) 
{ 
    fseek(m_fpFile, SEEK_SET, 0); 
    PullBuffer::Clear();
}

Error FileBuffer::Open(void)
{
    if (strcmp(m_szFile, "-") == 0)
    {
       m_fpFile = stdin;
    }
    else
       m_fpFile = fopen(m_szFile, "rb");

    if (m_fpFile == NULL)
    {
        switch (errno)
        {
            case EACCES:
               g_Log->Error("Access to the file was denied.\n");
               return kError_FileNoAccess;
               break;

            case EINVAL:
               g_Log->Error("Internal error: The file could not be opened.\n");
               return kError_FileInvalidArg;
               break;

            case EMFILE:
               g_Log->Error("Internal error: The file could not be opened.\n");
               return kError_FileNoHandles;
               break;

            case ENOENT:
               g_Log->Error("File not found.\n");
               return kError_FileNotFound;
               break;

            default:
               return kError_UnknownErr;
               break;
        }
    }

    fseek(m_fpFile, 0, SEEK_END);
    m_iFileSize = ftell(m_fpFile);

    fseek(m_fpFile, -iID3TagSize, SEEK_CUR);

    if (m_pID3Tag == NULL)
        m_pID3Tag = new unsigned char[iID3TagSize];

    int iRet = fread(m_pID3Tag, sizeof(char), iID3TagSize, m_fpFile);
    if (iRet == iID3TagSize)
    {
        if (strncmp((char *)m_pID3Tag, "TAG", 3))
        {
            delete m_pID3Tag;
            m_pID3Tag = NULL;
        }
        else
            m_iFileSize -= iID3TagSize;
    }
    else
    {
        delete m_pID3Tag;
        m_pID3Tag = NULL;
    }

    fseek(m_fpFile, 0, SEEK_SET);

    return kError_NoErr;
}

Error FileBuffer::GetID3v1Tag(unsigned char *pTag)
{
    if (m_pID3Tag)
    { 
        memcpy(pTag, m_pID3Tag, iID3TagSize);
        return kError_NoErr;
    }

    return kError_NoDataAvail;
}

Error FileBuffer::Run(void)
{
    if (!m_pBufferThread)
    {
       m_pBufferThread = Thread::CreateThread();
       if (!m_pBufferThread)
       {
           g_Log->Error("Could not create filebuffer thread.");
           return (Error)kError_CreateThreadFailed;
       }
       m_pBufferThread->Create(FileBuffer::StartWorkerThread, this);
    }

    return kError_NoErr;
}

void FileBuffer::StartWorkerThread(void *pVoidBuffer)
{
   ((FileBuffer*)pVoidBuffer)->WorkerThread();
}

void FileBuffer::WorkerThread(void)
{
   size_t  iToCopy, iRead; 
   void   *pBuffer;
   Error   eError;

   for(; !m_bExit;)
   {
      if (IsEndOfStream())
      {
          m_pWriteSem->Wait();
          continue;
      }
          
      eError = BeginWrite(pBuffer, iToCopy);
      if (eError == kError_NoErr)
      {
          iRead = fread((unsigned char *)pBuffer, 1, iToCopy, m_fpFile);
          eError = EndWrite(iRead);
          if (IsError(eError))
          {
              g_Log->Error("local: EndWrite returned: %d\n", eError);
              break;
          }

          if (iRead < iToCopy)
             SetEndOfStream(true);
      }
      if (eError == kError_BufferTooSmall)
      {
          m_pWriteSem->Wait();
      }
   }
   g_Log->Log(LogInput, "PMI: filebuffer thread exit\n");
}

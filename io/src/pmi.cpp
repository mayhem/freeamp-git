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
#include "pmi.h"
#include "pullbuffer.h"
#include "facontext.h"
#include "log.h"

#define DB printf("%s:%d\n", __FILE__, __LINE__);

PhysicalMediaInput::PhysicalMediaInput(FAContext *context):
                PipelineUnit(context)
{
    m_pID3Tag = NULL;
    m_path = NULL;
}

PhysicalMediaInput::~PhysicalMediaInput()
{
    m_bExit = true;
    m_bPause = false;
    m_pSleepSem->Signal();
    m_pPauseSem->Signal();

    if (m_path)
    {
       delete m_path;
       m_path = NULL;
    }

    if (m_pID3Tag)
    {
       delete m_pID3Tag;
       m_pID3Tag = NULL;
    }
}

Error PhysicalMediaInput::SetTo(char *url)
{
    delete m_path;

    m_path = new char[strlen(url) + 1];
    strcpy(m_path, url);

    return kError_NoErr;
}

Error PhysicalMediaInput::GetID3v1Tag(Id3TagInfo &sTag)
{
    Error eRet = kError_NoErr;

    if (m_pOutputBuffer == NULL)
    {
        eRet = Open();
        if (!IsError(eRet))
        {
            if (m_pID3Tag)
            {
               memcpy(&sTag, m_pID3Tag, iID3TagSize);
            }
            else
               eRet = kError_NoDataAvail;
        }
       
        Close();
 
        return eRet;
    }
    else
    {
       if (m_pID3Tag)
          memcpy(&sTag, m_pID3Tag, iID3TagSize);
       else
          eRet = kError_NoDataAvail;

       return eRet;
    }
}

Error PhysicalMediaInput::Close(void)
{
    if (m_pOutputBuffer)
       m_pOutputBuffer->Clear();

    return kError_NoErr;
}


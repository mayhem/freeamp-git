
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

extern "C" {
PhysicalMediaInput *Initialize() {
    return new LocalFileInput();
}
	   }
LocalFileInput::
LocalFileInput(): 
PhysicalMediaInput()
{
	m_path = NULL;
	m_fd = -1;
}

LocalFileInput::
LocalFileInput(char* path): 
PhysicalMediaInput()
{
    if (path) {
	int32 len = strlen(path) + 1;
	m_path = new char[len];
	
	if(m_path) {
	    memcpy(m_path,path,len);
	}
	
	m_fd = open(path, RD_BNRY_FLAGS);
    } else {
	m_path = NULL;
	m_fd = -1;
    }
}

LocalFileInput::
~LocalFileInput()
{
    //cout << "Deleting LocalFileInput" << endl;
    if(m_path) {
	delete [] m_path;
	m_path = NULL;
    }
    if (m_fd >= 0) {
	close(m_fd);
	m_fd = -1;
    }
    //cout << "Done deleting LocalFileInput" << endl;
}

Error LocalFileInput::
SetTo(char* url)
{
    Error result = kError_NoErr;

    if(m_fd >= 0) {
	    close(m_fd);
	    m_fd = -1;
    }

    if(m_path) {
	    delete [] m_path;
	    m_path = NULL;
    }

    if (url) {
	    int32 len = strlen(url) + 1;
	    m_path = new char[len];
	
	    if(m_path) {
	        memcpy(m_path,url,len);
	    } else {
	        result = kError_OutOfMemory;
	    }
	
        if(IsntError(result))
        {
	        m_fd = open(m_path, RD_BNRY_FLAGS);
	
	        if( m_fd < 0 ) {

                switch(errno) {
                    case EACCES:
                        result =  kError_FileNoAccess;
                        break;

                    case EEXIST:
                        result =  kError_FileExists;
                        break;

                    case EINVAL:
                        result =  kError_FileInvalidArg;
                        break;

                    case EMFILE:
                        result =  kError_FileNoHandles;
                        break;

                    case ENOENT:
                        result =  kError_FileNotFound;
                        break;

                    default:
                        result =  kError_UnknownErr;
                        break;
                }
	            
            }
        }
    } else {
	    result = kError_InvalidParam;
    }

    return result;
}

int32 LocalFileInput::
Read(void* buf, size_t numbytes)
{
	return read(m_fd, (char*)buf, numbytes);
}

int32 LocalFileInput::
Seek(int32 offset, int32 origin)
{
	return lseek(m_fd, offset, origin);
}

Error LocalFileInput::
Close(void)
{
	close(m_fd);
	m_fd = -1;
	return kError_NoErr;
}








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

#ifndef _LOCALFILEINPUT_H_
#define _LOCALFILEINPUT_H_

/* system headers */
#include <stdlib.h>

/* project headers */
#include "config.h"
#include "pmi.h"
#include "httpbuffer.h"

class     HttpInput:public PhysicalMediaInput
{
   public:

   HttpInput();
   HttpInput(char *path);
   virtual ~ HttpInput(void);

   virtual Error BeginRead(void *&buf, size_t &bytesneeded);
   virtual Error EndRead(size_t bytesused);

   virtual Error Seek(int32 & rtn, int32 offset, int32 origin);
   virtual Error GetLength(size_t &iSize); 
   virtual bool  GetID3v1Tag(unsigned char *pTag);

   virtual Error SetTo(char *url);
   virtual Error Close(void);
   virtual const char *Url(void) const
   {
      return m_path;
   }
   virtual const char *GetErrorString(int32);

   private:

   HttpBuffer *m_pPullBuffer;
   char       *m_path;
};

#endif /* _LOCALFILEINPUT_H_ */

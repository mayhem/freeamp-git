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
#include "thread.h"
#include "config.h"
#include "pmi.h"

const int32 iMaxFileNameLen = 255;
const int32 iID3TagSize = 128;
class FAContext;

class LocalFileInput:public PhysicalMediaInput
{
   public:

   LocalFileInput(FAContext *context);
   LocalFileInput(char *path);
   virtual ~ LocalFileInput(void);

   virtual Error Run(void);
   virtual Error Seek(int32 & rtn, int32 offset, int32 origin);
   virtual Error GetLength(size_t &iSize); 
   virtual Error GetID3v1Tag(unsigned char *pTag);

	virtual bool  CanHandle(char *szUrl, char *szTitle);
	virtual bool  IsStreaming(void)
	              { return false; };

   virtual Error Prepare(PullBuffer *&pBuffer, bool bStartThread = true);

   virtual Error SetTo(char *url);
   virtual Error Close(void);
   virtual void  Clear(void);
   virtual const char *Url(void) const
   {
      return m_path;
   }
   

   void           WorkerThread(void);

 private:

   static   void  StartWorkerThread(void *);
   virtual  Error Open(void);

   char           *m_path;
   FILE           *m_fpFile;
   Thread         *m_pBufferThread;
   bool            m_bLoop;
   size_t          m_iFileSize;
   unsigned char  *m_pID3Tag;

};

#endif /* _LOCALFILEINPUT_H_ */

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

#ifndef _HTTPINPUT_H_
#define _HTTPINPUT_H_

/* system headers */
#include <stdlib.h>

/* project headers */
#include "config.h"
#include "pmi.h"
#include "httpbuffer.h"

class FAContext;

class HttpInput:public PhysicalMediaInput
{
   public:

   HttpInput(FAContext *context);
   HttpInput(char *path);
   virtual ~ HttpInput(void);

   virtual Error BeginRead(void *&buf, size_t &bytesneeded);
   virtual Error EndRead(size_t bytesused);

   virtual Error Seek(int32 & rtn, int32 offset, int32 origin);
   virtual Error GetLength(size_t &iSize); 
   virtual Error GetID3v1Tag(unsigned char *pTag);

   virtual bool  CanHandle(char *szUrl, char *szTitle);
   virtual bool  IsStreaming(void)
                 { return true; };
	virtual int32 GetBufferPercentage();
	virtual int32 GetNumBytesInBuffer();
   virtual Error SetBufferSize(size_t iNewSize);
	virtual bool  CachePMI()
	              { return true; };

   virtual void  Pause();
   virtual bool  Resume();
   virtual void  Break();

   virtual Error SetTo(char *url, bool bStartThread = true);
   virtual Error Close(void);
   virtual const char *Url(void) const
   {
      return m_path;
   }

   virtual Error SetPropManager(Properties *p) 
	{ 
	   m_propManager = p; 
		if (p) 
		    return kError_NoErr; 
	   else 
		    return kError_UnknownErr; 
	};

 protected:
   FAContext  *m_context;

 private:
   Properties *m_propManager;

   HttpBuffer *m_pPullBuffer;
   char       *m_path;
};

#endif /* _HTTPFILEINPUT_H_ */

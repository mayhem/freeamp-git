/*_________________________________________________________________________
         
        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 2000 Relatable

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program; if not, Write to the Free Software
        Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
        
        $Id$
____________________________________________________________________________*/


/***************************************************************************
                          automutex.h  -  description
                             -------------------
    begin                : Tue Apr 4 2000
    copyright            : (C) 2000 by Relatable, LLC
    programed by         : Sean Ward
    email                : sward@relatable.com
 ***************************************************************************/


#ifndef AUTOMUTEX_H
#define AUTOMUTEX_H

#include "mutex.h"

/**Acquires its associated mutex on construction, and releases it on destruction. useful to automate mutex locking.
  *@author Sean Ward
  */

class AutoMutex {
public: 
    AutoMutex(Mutex* pMutex)
    {
        if (pMutex != NULL) 
            pMutex->Acquire();
        m_pMutex = pMutex;
    }
   ~AutoMutex() { if (m_pMutex != NULL) m_pMutex->Release(); }

private:
    Mutex* m_pMutex;	
};

#endif

/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1999 EMusic.com

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

#ifndef INCLUDED_WIN32_UPDATE_MANAGER_H_
#define INCLUDED_WIN32_UPDATE_MANAGER_H_

#include <assert.h>
#include <string>
#include <vector>
#include <deque>
#include <functional>

using namespace std;

#include "config.h"
#include "facontext.h"

#include "errors.h"
#include "updatemanager.h"


class Win32UpdateManager : public UpdateManager {

 public:
    Win32UpdateManager(FAContext* context);
    virtual ~Win32UpdateManager();
    
    virtual Error DetermineLocalVersions();
        
 protected:
  
    Error GetFileVersions(const char* path);

 private:

    FAContext* m_context;

    Mutex m_mutex;

    // paths i need to ignore
    char m_musicPath[MAX_PATH];
    char m_updatePath[MAX_PATH];

};

#endif // INCLUDED_WIN32_UPDATE_MANAGER_H_











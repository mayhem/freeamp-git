
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


#ifndef _PREFERENCES_H
#define _PREFERENCES_H

#include "config.h"
#include "errors.h"
#include "list.h"
#include "win32impl.h"

class LibDirFindHandle {
 public:
    List <char *> *m_pLibDirs;
    int32 m_current;
};


class Preferences {

 public:
	Preferences();
	~Preferences();

    Error GetInstallDirectory(char* path, uint32* len);
    Error SetInstallDirectory(char* path);

    Error GetDefaultUI(char* name, uint32* len);
    Error SetDefaultUI(char* name);

    Error GetDefaultPMO(char* name, uint32* len);
    Error SetDefaultPMO(char* name);

    Error GetOpenSaveDirectory(char* path, uint32* len);
    Error SetOpenSaveDirectory(char* path);

    HANDLE GetFirstLibDir(char *path, uint32 *len);
    Error GetNextLibDir(HANDLE hLibDirFind,char *path, uint32 *len);
    Error GetLibDirClose(HANDLE hLibDirFind);

    Error  GetLogMain(bool *bValue)
          { *bValue = false; return kError_NoErr; };
    Error  GetLogDecode(bool *bValue)
          { *bValue = false; return kError_NoErr; };
    Error  GetLogInput(bool *bValue)
          { *bValue = false; return kError_NoErr; };
    Error  GetLogOutput(bool *bValue)
          { *bValue = false; return kError_NoErr; };
    Error  GetLogPerf(bool *bValue)
          { *bValue = false; return kError_NoErr; };
    Error  GetUseDebugLog(bool *bValue)
          { *bValue = false; return kError_NoErr; };
    Error  GetInputBufferSize(int32 *iValue)
          { *iValue = 64; return kError_NoErr; };
    Error  GetOutputBufferSize(int32 *iValue)
          { *iValue = 512 * 1024; return kError_NoErr; };
    Error  GetStreamBufferInterval(int32 *iValue)
          { *iValue = 2; return kError_NoErr; };
    Error  GetDecoderThreadPriority(int32 *iValue)
          { *iValue = 0; return kError_NoErr; };

    static const char *GetLibDirs();

 protected:
    Error GetPrefString(const char* pref, char* buf, uint32* len);
    Error SetPrefString(const char* pref, char* buf);

 private:
//     HKEY   m_prefsKey;
    static char *m_libDirs;
};

#endif /* _PREFERENCES_H */






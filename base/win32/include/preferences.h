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

#include <windows.h>
#include "config.h"
#include "errors.h"

// preferences
extern const char* kInstallDirPref;
extern const char* kUIPref;
extern const char* kPMOPref;
extern const char* kOpenSaveDirPref;
extern const char* kStayOnTopPref;
extern const char* kLiveInTrayPref;
extern const char* kInputBufferSizePref;
extern const char* kOutputBufferSizePref;
extern const char* kStreamBufferIntervalPref;
extern const char* kDecoderThreadPriorityPref;
extern const char* kUseDebugLogPref;
extern const char* kLogMainPref;
extern const char* kLogDecodePref;
extern const char* kLogInputPref;
extern const char* kLogOutputPref;
extern const char* kLogPerformancePref;


class Preferences {

 public:
	Preferences();
    Preferences(const char* componentName);
	~Preferences();

    Error Initialize();

    // foundation preference functions
    Error GetPrefString(const char* pref, char* buf, uint32* len);
    Error SetPrefString(const char* pref, const char* buf);

    Error GetPrefBoolean(const char* pref, bool* value);
    Error SetPrefBoolean(const char* pref, bool value);

    Error GetPrefInt32(const char* pref, int32* value);
    Error SetPrefInt32(const char* pref, int32 value);

    // convenience functions
    Error GetInstallDirectory(char* path, uint32* len);
    Error SetInstallDirectory(char* path);

    Error GetDefaultUI(char* name, uint32* len);
    Error SetDefaultUI(char* name);

    Error GetDefaultPMO(char* name, uint32* len);
    Error SetDefaultPMO(char* name);

    Error GetOpenSaveDirectory(char* path, uint32* len);
    Error SetOpenSaveDirectory(char* path);

    Error GetStayOnTop(bool* value);
    Error SetStayOnTop(bool value);

    Error GetLiveInTray(bool* value);
    Error SetLiveInTray(bool value);

    Error GetInputBufferSize(int32* value);
    Error SetInputBufferSize(int32 value);

    Error GetOutputBufferSize(int32* value);
    Error SetOutputBufferSize(int32 value);

    Error GetStreamBufferInterval(int32* value);
    Error SetStreamBufferInterval(int32 value);

    Error GetDecoderThreadPriority(int32* value);
    Error SetDecoderThreadPriority(int32 value);

    Error GetUseDebugLog(bool* value);
    Error SetUseDebugLog(bool value);

    Error GetLogMain(bool* value);
    Error SetLogMain(bool value);

    Error GetLogDecode(bool* value);
    Error SetLogDecode(bool value);

    Error GetLogInput(bool* value);
    Error SetLogInput(bool value);

    Error GetLogOutput(bool* value);
    Error SetLogOutput(bool value);

    Error GetLogPerformance(bool* value);
    Error SetLogPerformance(bool value);

 protected:
    

 private:
     HKEY   m_prefsKey;
	
};

#endif /* _PREFERENCES_H */

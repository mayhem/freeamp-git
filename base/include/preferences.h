/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998-1999 GoodNoise

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

// preferences
extern const char* kInstallDirPref;
extern const char* kLibraryPathPref;
extern const char* kUIPref;
extern const char* kTextUIPref;
extern const char* kPMOPref;
extern const char* kALSADevicePref;
extern const char* kOpenSaveDirPref;
extern const char* kStayOnTopPref;
extern const char* kLiveInTrayPref;
extern const char* kInputBufferSizePref;
extern const char* kOutputBufferSizePref;
extern const char* kStreamBufferIntervalPref;
extern const char* kDecoderThreadPriorityPref;
extern const char* kWindowPositionLeftPref;
extern const char* kWindowPositionTopPref;
extern const char* kWindowPositionWidthPref;
extern const char* kWindowPositionHeightPref;
extern const char* kUseDebugLogPref;
extern const char* kLogMainPref;
extern const char* kLogDecodePref;
extern const char* kLogInputPref;
extern const char* kLogOutputPref;
extern const char* kLogPerformancePref;
extern const char* kSaveStreamsPref;
extern const char* kSaveStreamsDirPref;
extern const char* kUseProxyPref;
extern const char* kProxyHostPref;
extern const char* kPreBufferPref;
extern const char* kUseNIC;
extern const char* kNICAddress;
extern const char* kUseTitleStreaming;  

class LibDirFindHandle;

class Preferences {
  public:
    virtual ~Preferences() { }

    virtual Error SetDefaults();
    virtual Error Save() = 0;

    virtual Preferences *ComponentPrefs(const char *componentName) = 0;

    // foundation preference functions
    virtual Error GetPrefString(const char* pref, char* buf, uint32* len) = 0;
    virtual Error SetPrefString(const char* pref, const char* buf) = 0;

    virtual Error GetPrefBoolean(const char* pref, bool* value);
    virtual Error SetPrefBoolean(const char* pref, bool value);

    virtual Error GetPrefInt32(const char* pref, int32* value);
    virtual Error SetPrefInt32(const char* pref, int32 value);

    // Iterators for the freeamp library path
    virtual LibDirFindHandle *GetFirstLibDir(char *path, uint32 *len);
    virtual Error GetNextLibDir(LibDirFindHandle *hLibDirFind,
				char *path, uint32 *len);
    virtual Error GetLibDirClose(LibDirFindHandle *hLibDirFind);

    // XXX: This is a non-portable hack, and should only be used for user
    //      messages.  Normally, you should use the iterators above instead.
    virtual const char *GetLibDirs() = 0;


    // convenience functions
    Error GetInstallDirectory(char* path, uint32* len);
    Error SetInstallDirectory(char* path);

    Error GetDefaultUI(char* name, uint32* len);
    Error SetDefaultUI(char* name);

    Error GetDefaultTextUI(char* name, uint32* len);
    Error SetDefaultTextUI(char* name);

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

    Error GetWindowPosition(int32* left,
                            int32* top,
                            int32* width,
                            int32* height);

    Error SetWindowPosition(int32 left,
                            int32 top,
                            int32 width,
                            int32 height);

    Error GetSaveStreams(bool* value);
    Error SetSaveStreams(bool value);

    Error GetSaveStreamsDirectory(char* path, uint32* len);
    Error SetSaveStreamsDirectory(char* path);

    Error GetUseProxyServer(bool* value);
    Error SetUseProxyServer(bool value);

    Error GetProxyServerAddress(char* host, uint32* len);
    Error SetProxyServerAddress(char* host);

    Error GetPrebufferLength(int32* value);
    Error SetPrebufferLength(int32 value);

};

#endif /* _PREFERENCES_H */

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

#include <string.h>
#include <stdlib.h>
#include "preferences.h"


// preferences
const char* kInstallDirPref = "InstallDirectory";
const char* kLibraryPathPref = "LibraryPath";
const char* kUIPref = "UI";
const char* kTextUIPref = "TextUI";
const char* kPMOPref = "PMO";
const char* kALSADevicePref = "ALSADevice";
const char* kOpenSaveDirPref = "OpenSaveDirectory";
const char* kStayOnTopPref = "StayOnTop";
const char* kLiveInTrayPref = "LiveInTray";
const char* kInputBufferSizePref = "InputBufferSize";
const char* kOutputBufferSizePref = "OutputBufferSize";
const char* kStreamBufferIntervalPref = "StreamBufferInterval";
const char* kDecoderThreadPriorityPref = "DecoderThreadPriority";
const char* kWindowPositionLeftPref = "WindowPositionLeft";
const char* kWindowPositionTopPref = "WindowPositionTop";
const char* kWindowPositionWidthPref = "WindowPositionWidth";
const char* kWindowPositionHeightPref = "WindowPositionHeight";
const char* kSaveStreamPref = "SaveStream";
const char* kSaveStreamDirPref = "SaveStreamDirectory";

//logging
const char* kUseDebugLogPref = "UseDebugLog";
const char* kLogMainPref = "LogMain";
const char* kLogDecodePref = "LogDecode";
const char* kLogInputPref = "LogInput";
const char* kLogOutputPref = "LogOutput";
const char* kLogPerformancePref = "LogPerformance";

// default values
const int32 kDefaultInputBufferSize = 64;
const int32 kDefaultOutputBufferSize = 512;
const int32 kDefaultStreamBufferInterval = 3;
const int32 kDefaultDecoderThreadPriority = 1;
const bool  kDefaultLogging = false;
const bool  kDefaultSaveStream = false;
const char* kDefaultSaveStreamDir = ".";

Error
Preferences::
SetDefaults()
{
    int32 dummyInt;
    bool  dummyBool;
    char  dummyString[255];
    unsigned size = 255;

    // set default for input buffer size
    if (GetPrefInt32(kInputBufferSizePref, &dummyInt) == kError_NoPrefValue)
        SetPrefInt32(kInputBufferSizePref, kDefaultInputBufferSize);

    // set default for output buffer size
    if (GetPrefInt32(kOutputBufferSizePref, &dummyInt) == kError_NoPrefValue)
        SetPrefInt32(kOutputBufferSizePref, kDefaultOutputBufferSize);

    // set default for streaming buffer interval
    if (GetPrefInt32(kStreamBufferIntervalPref, &dummyInt)
        == kError_NoPrefValue)
        SetPrefInt32(kStreamBufferIntervalPref, 
                     kDefaultStreamBufferInterval);

    // set default for decoder thread priority
    if (GetPrefInt32(kDecoderThreadPriorityPref, &dummyInt)
        == kError_NoPrefValue)
        SetPrefInt32(kDecoderThreadPriorityPref, 
                     kDefaultDecoderThreadPriority);

    // set defaults for logging
    if (GetPrefBoolean(kUseDebugLogPref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kUseDebugLogPref, kDefaultLogging);
    if (GetPrefBoolean(kLogMainPref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kLogMainPref, kDefaultLogging);
    if (GetPrefBoolean(kLogDecodePref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kLogDecodePref, kDefaultLogging);
    if (GetPrefBoolean(kLogInputPref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kLogInputPref, kDefaultLogging);
    if (GetPrefBoolean(kLogOutputPref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kLogOutputPref, kDefaultLogging);
    if (GetPrefBoolean(kLogPerformancePref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kLogPerformancePref, kDefaultLogging);

    if (GetPrefBoolean(kSaveStreamPref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kSaveStreamPref, kDefaultSaveStream);

    if (GetPrefString(kSaveStreamDirPref, dummyString, &size) == kError_NoPrefValue)
        SetPrefString(kSaveStreamDirPref, kDefaultSaveStreamDir);

    return kError_NoErr;
}

Error
Preferences::
GetPrefBoolean(const char* pref, bool* value)
{
    Error error;
    char temp[32];
    uint32 size = sizeof(temp);

    *value = false;
    error = GetPrefString(pref, temp, &size);
    if (IsntError(error))
    {
        if (0 == strcasecmp(temp, "true"))
            *value = true;
        else if (0 != strcasecmp(temp, "false"))
        {
            // In the case of a syntax error, pretend the preference doesn't
            // exist, so that it'll be replaced by a default value.
            error = kError_NoPrefValue;
        }
    }
    return error;

}

Error
Preferences::
SetPrefBoolean(const char* pref, bool value)
{
    return SetPrefString(pref, value ? "true" : "false");
}

Error
Preferences::
GetPrefInt32(const char* pref, int32* value)
{
    Error error;
    char temp[32];
    char* endp;
    uint32 size = sizeof(temp);

    *value = -1;
    error = GetPrefString(pref, temp, &size);
    if (IsntError(error))
    {
        *value = strtol(temp, &endp, 10);
        // In the case of a syntax error, pretend the preference doesn't
        // exist, so that it'll be replaced by a default value.
        if (*endp != '\0')
            error = kError_NoPrefValue;
    }
    return error;
}

Error
Preferences::
SetPrefInt32(const char* pref, int32 value)
{
    Error error;
    bool isNeg = 0;
    char temp[32];
    char* p = temp + sizeof(temp);

    if (value < 0)
    {
        value = -value;
        isNeg = 1;
    }

    *--p = '\0';

    do
    {
        *--p = '0' + (value % 10);
        value /= 10;
    } while (value > 0);

    if (isNeg)
        *--p = '-';

    error = SetPrefString(pref, p);

    return error;
}


Error 
Preferences::
GetInstallDirectory(char* path, uint32* len)
{
    return GetPrefString(kInstallDirPref, path, len);
}

Error 
Preferences::
SetInstallDirectory(char* path)
{
    return SetPrefString(kInstallDirPref, path);
}

Error 
Preferences::
GetDefaultUI(char* name, uint32* len)
{
    return GetPrefString(kUIPref, name, len);
}

Error 
Preferences::
SetDefaultUI(char* name)
{
    return SetPrefString(kUIPref, name);
}

Error 
Preferences::
GetDefaultTextUI(char* name, uint32* len)
{
    return GetPrefString(kTextUIPref, name, len);
}

Error 
Preferences::
SetDefaultTextUI(char* name)
{
    return SetPrefString(kTextUIPref, name);
}

Error 
Preferences::
GetDefaultPMO(char* name, uint32* len)
{
    return GetPrefString(kPMOPref, name, len);
}

Error 
Preferences::
SetDefaultPMO(char* name)
{
    return SetPrefString(kPMOPref, name);
}

Error 
Preferences::
GetOpenSaveDirectory(char* path, uint32* len)
{
    return GetPrefString(kOpenSaveDirPref, path, len);
}

Error 
Preferences::
SetOpenSaveDirectory(char* path)
{
    return SetPrefString(kOpenSaveDirPref, path);
}

Error 
Preferences::
GetStayOnTop(bool* value)
{
    return GetPrefBoolean(kStayOnTopPref, value);
}

Error 
Preferences::
SetStayOnTop(bool value)
{
    return SetPrefBoolean(kStayOnTopPref, value);
}

Error 
Preferences::
GetLiveInTray(bool* value)
{
    return GetPrefBoolean(kLiveInTrayPref, value);
}

Error 
Preferences::
SetLiveInTray(bool value)
{
    return SetPrefBoolean(kLiveInTrayPref, value);
}

Error 
Preferences::
GetInputBufferSize(int32* value)
{
    return GetPrefInt32(kInputBufferSizePref, value);
}

Error 
Preferences::
SetInputBufferSize(int32 value)
{
    return SetPrefInt32(kInputBufferSizePref, value);
}

Error 
Preferences::
GetOutputBufferSize(int32* value)
{
    return GetPrefInt32(kOutputBufferSizePref, value);
}

Error 
Preferences::
SetOutputBufferSize(int32 value)
{
    return SetPrefInt32(kOutputBufferSizePref, value);
}

Error 
Preferences::
GetStreamBufferInterval(int32* value)
{
    return GetPrefInt32(kStreamBufferIntervalPref, value);
}

Error 
Preferences::
SetStreamBufferInterval(int32 value)
{
    return SetPrefInt32(kStreamBufferIntervalPref, value);
}

Error 
Preferences::
GetDecoderThreadPriority(int32* value)
{
    return GetPrefInt32(kDecoderThreadPriorityPref, value);
}

Error 
Preferences::
SetDecoderThreadPriority(int32 value)
{
    return SetPrefInt32(kDecoderThreadPriorityPref, value);
}

Error 
Preferences::
SetUseDebugLog(bool value)
{
    return SetPrefBoolean(kUseDebugLogPref, value);
}

Error 
Preferences::
GetUseDebugLog(bool* value)
{
    return GetPrefBoolean(kUseDebugLogPref, value);
}

Error 
Preferences::
SetLogMain(bool value)
{
    return SetPrefBoolean(kLogMainPref, value);
}

Error 
Preferences::
GetLogMain(bool* value)
{
    return GetPrefBoolean(kLogMainPref, value);
}

Error 
Preferences::
SetLogDecode(bool value)
{
    return SetPrefBoolean(kLogDecodePref, value);
}

Error 
Preferences::
GetLogDecode(bool* value)
{
    return GetPrefBoolean(kLogDecodePref, value);
}

Error 
Preferences::
SetLogInput(bool value)
{
    return SetPrefBoolean(kLogInputPref, value);
}

Error 
Preferences::
GetLogInput(bool* value)
{
    return GetPrefBoolean(kLogInputPref, value);
}

Error 
Preferences::
SetLogOutput(bool value)
{
    return SetPrefBoolean(kLogOutputPref, value);
}

Error 
Preferences::
GetLogOutput(bool* value)
{
    return GetPrefBoolean(kLogOutputPref, value);
}

Error 
Preferences::
SetLogPerformance(bool value)
{
    return SetPrefBoolean(kLogPerformancePref, value);
}

Error 
Preferences::
GetLogPerformance(bool* value)
{
    return GetPrefBoolean(kLogPerformancePref, value);
}

Error 
Preferences::
GetWindowPosition(  int32* left,
                    int32* top,
                    int32* width,
                    int32* height)
{
    Error result;

    result = GetPrefInt32(kWindowPositionLeftPref, left);

    if(IsntError(result))
        result = GetPrefInt32(kWindowPositionTopPref, top);

    if(IsntError(result))
        result = GetPrefInt32(kWindowPositionWidthPref, width);

    if(IsntError(result))
        result = GetPrefInt32(kWindowPositionHeightPref, height);

    return result;
}

Error 
Preferences::
SetWindowPosition(  int32 left,
                    int32 top,
                    int32 width,
                    int32 height)
{
    Error result;

    result = SetPrefInt32(kWindowPositionLeftPref, left);

    if(IsntError(result))
        result = SetPrefInt32(kWindowPositionTopPref, top);

    if(IsntError(result))
        result = SetPrefInt32(kWindowPositionWidthPref, width);

    if(IsntError(result))
        result = SetPrefInt32(kWindowPositionHeightPref, height);

    return result;
}


Error 
Preferences::
GetSaveStream(bool* value)
{
    return GetPrefBoolean(kSaveStreamPref, value);
}

Error 
Preferences::
SetSaveStream(bool value)
{
    return SetPrefBoolean(kSaveStreamPref, value);
}

Error 
Preferences::
GetStreamSaveDirectory(char* path, uint32* len)
{
    return GetPrefString(kSaveStreamDirPref, path, len);
}

Error 
Preferences::
SetStreamSaveDirectory(char* path)
{
    return SetPrefString(kSaveStreamDirPref, path);
}

LibDirFindHandle *
Preferences::
GetFirstLibDir(char *path, uint32 *len)
{
    Error error;
    
    error = GetInstallDirectory(path, len);
    return IsError(error) ? 0 : (LibDirFindHandle *) 1;
}

Error
Preferences::
GetNextLibDir(LibDirFindHandle *hLibDirFind, char *path, uint32 *len)
{
    path[0] = '\0';
    return kError_NoMoreLibDirs;
}

Error
Preferences::
GetLibDirClose(LibDirFindHandle *hLibDirFind)
{
    return kError_NoErr;
}

const char *
Preferences::
GetLibDirs()
{
    return NULL;
}


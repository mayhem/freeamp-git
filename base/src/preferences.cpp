/*____________________________________________________________________________
        
        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 1998-1999 EMusic.com

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
const char* kMusicBrowserUIPref = "MusicBrowserUI";
const char* kDownloadManagerUIPref = "DownloadManagerUI";
const char* kToolbarUIPref = "ToolbarUI";
const char* kPMOPref = "PMO";
const char* kALSADevicePref = "ALSADevice";
const char* kESOUNDHostPref = "EsounDHost";
const char* kOpenSaveDirPref = "OpenSaveDirectory";
const char* kStayOnTopPref = "StayOnTop";
const char* kLiveInTrayPref = "LiveInTray";
const char* kInputBufferSizePref = "InputBufferSize";
const char* kOutputBufferSizePref = "OutputBufferSize";
const char* kStreamBufferIntervalPref = "StreamBufferInterval";
const char* kDecoderThreadPriorityPref = "DecoderThreadPriority";
const char* kSaveStreamsPref = "SaveStreams";
const char* kSaveStreamsDirPref = "SaveStreamsDirectory";
const char* kUseProxyPref = "UseProxy";
const char* kProxyHostPref = "ProxyHost";  
const char* kPreBufferPref = "PreBuffer";  
const char* kUseAlternateNICPref = "UseAlternateNIC";  
const char* kAlternateNICAddressPref = "AlternateNICAddress";  
const char* kUseTitleStreamingPref = "UseTitleStreaming";  
const char* kThemePathPref = "ThemePath";  
const char* kMainWindowPosPref = "MainWindowPos";  
const char* kThemeDefaultFontPref = "ThemeDefaultFont";
const char* kSaveMusicDirPref = "SaveMusicDirectory";
const char* kDatabaseDirPref = "DatabaseDirectory";
const char* kCheckForUpdatesPref = "CheckForUpdates";
const char* kPlaylistRepeatPref = "PlaylistRepeat";
const char* kPlaylistShufflePref = "PlaylistShuffle";
const char* kTimeDisplayPref = "TimeDisplay";
const char* kVolumePref = "Volume";
const char* kUserNamePref = "UserName";
const char* kReclaimFiletypesPref = "ReclaimFiletypes";
const char* kAskToReclaimFiletypesPref = "AskToReclaimFiletypes";
const char* kUsersPortablePlayersPref = "UsersPortablePlayers";
const char* kShowToolbarTextLabelsPref = "ShowToolbarTextLabels";
const char* kShowToolbarImagesPref = "ShowToolbarImages";
const char* kSaveCurrentPlaylistOnExitPref = "SaveCurrentPlaylistOnExit";
const char* kViewMusicBrowserPref = "ViewMusicBrowser";
const char* kWindowModePref = "WindowMode";
const char* kWelcomePref = "Welcome";
const char* kPlayImmediatelyPref = "PlayImmediately";
const char* kNumberOfURLsToRememberPref = "NumberOfURLsToRemember";
const char* kCDDevicePathPref = "CDDevice";
const char* kCDDBServerPref = "CDDBServer";

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
const int32 kDefaultDecoderThreadPriority = 5;
const bool  kDefaultLogging = false;
const bool  kDefaultSaveStreams = false;
const char *kDefaultSaveStreamsDir = ".";
const int32 kDefaultPreBuffer = 0;
const char *kDefaultProxyHost = "http://proxy.yourdomain.com:8080";
const bool  kDefaultUseProxy = false;
const bool  kDefaultUseNIC = false;
const char *kDefaultNICAddress = "192.168.1.1";
const bool  kDefaultUseTitleStreaming = true;
const char *kDefaultThemePath = "";  
const char *kDefaultMainWindowPos = "-1,-1";  
const char *kDefaultWindowMode = "MainWindow";
#ifdef WIN32
const char *kDefaultThemeDefaultFont = "Arial";
const char *kDefaultCDDevicePath = "";
#else
const char *kDefaultThemeDefaultFont = "Helvetica";
const char *kDefaultCDDevicePath = "/dev/cdrom";
#endif
const char *kDefaultDownloadManagerUI = "download.ui";
const char *kDefaultMusicBrowserUI = "musicbrowser.ui";
const char *kDefaultToolbarUI = "toolbar.ui";
const bool kDefaultCheckForUpdates = false;
const int32 kDefaultPlaylistRepeat = 0;
const bool kDefaultPlaylistShuffle = false;
const int32 kDefaultTimeDisplay = 0;
const int32 kDefaultVolume = 85;
const char* kDefaultUserName = "";
const bool kDefaultReclaimFiletypes = true;
const bool kDefaultAskToReclaimFiletypesPref = true;
const char* kDefaultUsersPortablePlayers = "";
const bool kDefaultShowToolbarTextLabels = true;
const bool kDefaultShowToolbarImages = true;
const bool kDefaultSaveCurrentPlaylistOnExit = false;
const bool kDefaultViewMusicBrowser = true;
const bool kDefaultWelcome = true;
const bool kDefaultPlayImmediately = true;
const int32 kDefaultNumberOfURLsToRemember = 10;
const char* kDefaultCDDBServer = "http://www2.freedb.org/cgi-bin/cddb.cgi";

Error
Preferences::
SetDefaults()
{
    int32 dummyInt;
    bool  dummyBool;
    char  dummyString[255];
    uint32 size = 255;

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

    if (GetPrefBoolean(kSaveStreamsPref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kSaveStreamsPref, kDefaultSaveStreams);

    if (GetPrefString(kSaveStreamsDirPref, dummyString, &size) == kError_NoPrefValue)
        SetPrefString(kSaveStreamsDirPref, kDefaultSaveStreamsDir);

    if (GetPrefInt32(kPreBufferPref, &dummyInt) == kError_NoPrefValue)
        SetPrefInt32(kPreBufferPref, kDefaultPreBuffer);

    if (GetPrefBoolean(kUseProxyPref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kUseProxyPref, kDefaultUseProxy);

    dummyInt = 255;
    if (GetPrefString(kProxyHostPref, dummyString, 
        (uint32 *)&dummyInt) == kError_NoPrefValue)
        SetPrefString(kProxyHostPref, kDefaultProxyHost);

    dummyInt = 255;
    if (GetPrefString(kCDDBServerPref, dummyString, (uint32 *)&dummyInt) == 
        kError_NoPrefValue)
        SetPrefString(kCDDBServerPref, kDefaultCDDBServer);

    if (GetPrefBoolean(kUseAlternateNICPref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kUseAlternateNICPref, kDefaultUseNIC);

    dummyInt = 255;
    if (GetPrefString(kAlternateNICAddressPref, dummyString, 
        (uint32 *)&dummyInt) == kError_NoPrefValue)
        SetPrefString(kAlternateNICAddressPref, kDefaultNICAddress);

    if (GetPrefBoolean(kUseTitleStreamingPref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kUseTitleStreamingPref, kDefaultUseTitleStreaming);

    dummyInt = 255;
    if (GetPrefString(kThemePathPref, dummyString, 
        (uint32 *)&dummyInt) == kError_NoPrefValue)
        SetPrefString(kThemePathPref, kDefaultThemePath);

    dummyInt = 255;
    if (GetPrefString(kMainWindowPosPref, dummyString, 
        (uint32 *)&dummyInt) == kError_NoPrefValue)
        SetPrefString(kMainWindowPosPref, kDefaultMainWindowPos);

    dummyInt = 255;
    if (GetPrefString(kWindowModePref, dummyString, 
        (uint32 *)&dummyInt) == kError_NoPrefValue)
        SetPrefString(kWindowModePref, kDefaultWindowMode);

    dummyInt = 255;
    if (GetPrefString(kThemeDefaultFontPref, dummyString, 
        (uint32 *)&dummyInt) == kError_NoPrefValue)
        SetPrefString(kThemeDefaultFontPref, kDefaultThemeDefaultFont);

    dummyInt = 255;
    if (GetPrefString(kMusicBrowserUIPref, dummyString,
        (uint32 *)&dummyInt) == kError_NoPrefValue)
        SetPrefString(kMusicBrowserUIPref, kDefaultMusicBrowserUI);

    dummyInt = 255;
    if (GetPrefString(kDownloadManagerUIPref, dummyString,
        (uint32 *)&dummyInt) == kError_NoPrefValue)
        SetPrefString(kDownloadManagerUIPref, kDefaultDownloadManagerUI);

    dummyInt = 255;
    if (GetPrefString(kToolbarUIPref, dummyString,
        (uint32 *)&dummyInt) == kError_NoPrefValue)
        SetPrefString(kToolbarUIPref, kDefaultToolbarUI);

    if (GetPrefBoolean(kCheckForUpdatesPref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kCheckForUpdatesPref, kDefaultCheckForUpdates);

    if (GetPrefInt32(kPlaylistRepeatPref, &dummyInt) == kError_NoPrefValue)
        SetPrefInt32(kPlaylistRepeatPref, kDefaultPlaylistRepeat);

    if (GetPrefBoolean(kPlaylistShufflePref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kPlaylistShufflePref, kDefaultPlaylistShuffle);

    if (GetPrefInt32(kTimeDisplayPref, &dummyInt) == kError_NoPrefValue)
        SetPrefInt32(kTimeDisplayPref, kDefaultTimeDisplay);

    if (GetPrefInt32(kVolumePref, &dummyInt) == kError_NoPrefValue)
        SetPrefInt32(kVolumePref, kDefaultVolume);

    dummyInt = 255;
    if (GetPrefString(kUserNamePref, dummyString, 
        (uint32 *)&dummyInt) == kError_NoPrefValue)
        SetPrefString(kUserNamePref, kDefaultUserName);

    dummyInt = 255;
    if (GetPrefString(kCDDevicePathPref, dummyString,
        (uint32 *)&dummyInt) == kError_NoPrefValue)
        SetPrefString(kCDDevicePathPref, kDefaultCDDevicePath);

    if (GetPrefBoolean(kReclaimFiletypesPref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kReclaimFiletypesPref, kDefaultReclaimFiletypes);

    if (GetPrefBoolean(kAskToReclaimFiletypesPref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kAskToReclaimFiletypesPref, kDefaultAskToReclaimFiletypesPref);

    dummyInt = 255;
    if (GetPrefString(kUsersPortablePlayersPref, dummyString, 
        (uint32 *)&dummyInt) == kError_NoPrefValue)
        SetPrefString(kUsersPortablePlayersPref, kDefaultUsersPortablePlayers);

    if (GetPrefBoolean(kShowToolbarTextLabelsPref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kShowToolbarTextLabelsPref, kDefaultShowToolbarTextLabels);

    if (GetPrefBoolean(kShowToolbarImagesPref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kShowToolbarImagesPref, kDefaultShowToolbarImages);

    if (GetPrefBoolean(kSaveCurrentPlaylistOnExitPref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kSaveCurrentPlaylistOnExitPref, kDefaultSaveCurrentPlaylistOnExit);

    if (GetPrefBoolean(kViewMusicBrowserPref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kViewMusicBrowserPref, kDefaultViewMusicBrowser);

    if (GetPrefBoolean(kWelcomePref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kWelcomePref, kDefaultWelcome);

    if (GetPrefBoolean(kPlayImmediatelyPref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kPlayImmediatelyPref, kDefaultPlayImmediately);

    if (GetPrefInt32(kNumberOfURLsToRememberPref, &dummyInt) == kError_NoPrefValue)
        SetPrefInt32(kNumberOfURLsToRememberPref, kDefaultNumberOfURLsToRemember);

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
SetInstallDirectory(const char* path)
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
SetDefaultUI(const char* name)
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
SetDefaultTextUI(const char* name)
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
SetDefaultPMO(const char* name)
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
SetOpenSaveDirectory(const char* path)
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
GetSaveStreams(bool* value)
{
    return GetPrefBoolean(kSaveStreamsPref, value);
}

Error 
Preferences::
SetSaveStreams(bool value)
{
    return SetPrefBoolean(kSaveStreamsPref, value);
}

Error 
Preferences::
GetSaveStreamsDirectory(char* path, uint32* len)
{
    return GetPrefString(kSaveStreamsDirPref, path, len);
}

Error 
Preferences::
SetSaveStreamsDirectory(const char* path)
{
    return SetPrefString(kSaveStreamsDirPref, path);
}

Error
Preferences::
GetUseProxyServer(bool* value)
{
    return GetPrefBoolean(kUseProxyPref, value);
}

Error
Preferences::
SetUseProxyServer(bool value)
{
    return SetPrefBoolean(kUseProxyPref, value);
}

Error
Preferences::
GetProxyServerAddress(char* host, uint32* len)
{
    return GetPrefString(kProxyHostPref, host, len);
}

Error
Preferences::
SetProxyServerAddress(const char* host)
{
    return SetPrefString(kProxyHostPref, host);
}

Error
Preferences::
GetCDDBServerAddress(char* host, uint32* len)
{
    return GetPrefString(kCDDBServerPref, host, len);
}

Error
Preferences::
SetCDDBServerAddress(const char* host)
{
    return SetPrefString(kCDDBServerPref, host);
}

Error
Preferences::
GetPrebufferLength(int32* value)
{
    return GetPrefInt32(kPreBufferPref, value);
}

Error
Preferences::
SetPrebufferLength(int32 value)
{
    return SetPrefInt32(kPreBufferPref, value);
}

Error
Preferences::
GetUseAlternateNIC(bool* value)
{
    return GetPrefBoolean(kUseAlternateNICPref, value);
}

Error
Preferences::
SetUseAlternateNIC(bool value)
{
    return SetPrefBoolean(kUseAlternateNICPref, value);
}

Error
Preferences::
GetAlternateNICAddress(char* address, uint32* len)
{
    return GetPrefString(kAlternateNICAddressPref, address, len);
}

Error
Preferences::
SetAlternateNICAddress(const char* address)
{
    return SetPrefString(kAlternateNICAddressPref, address);
}

Error
Preferences::
GetThemeDefaultFont(char* font, uint32* len)
{
    return GetPrefString(kThemeDefaultFontPref, font, len);
}

Error
Preferences::
SetThemeDefaultFont(const char* font)
{
    return SetPrefString(kThemeDefaultFontPref, font);
}

Error
Preferences::
GetThemePath(char* path, uint32* len)
{
    return GetPrefString(kThemePathPref, path, len);
}

Error
Preferences::
SetThemePath(const char* path)
{
    return SetPrefString(kThemePathPref, path);
}

Error
Preferences::
GetCheckForUpdates(bool* value)
{
    return GetPrefBoolean(kCheckForUpdatesPref, value);
}

Error
Preferences::
SetCheckForUpdates(bool value)
{
    return SetPrefBoolean(kCheckForUpdatesPref, value);
}

Error
Preferences::
GetPlaylistRepeat(int32* value)
{
    return GetPrefInt32(kPlaylistRepeatPref, value);
}

Error
Preferences::
SetPlaylistRepeat(int32 value)
{
    return SetPrefInt32(kPlaylistRepeatPref, value);
}


Error
Preferences::
GetPlaylistShuffle(bool* value)
{
    return GetPrefBoolean(kPlaylistShufflePref, value);
}

Error
Preferences::
SetPlaylistShuffle(bool value)
{
    return SetPrefBoolean(kPlaylistShufflePref, value);
}

Error
Preferences::
GetTimeDisplay(int32* value)
{
    return GetPrefInt32(kTimeDisplayPref, value);
}

Error
Preferences::
SetTimeDisplay(int32 value)
{
    return SetPrefInt32(kTimeDisplayPref, value);
}

Error
Preferences::
GetVolume(int32* value)
{
    return GetPrefInt32(kVolumePref, value);
}

Error
Preferences::
SetVolume(int32 value)
{
    return SetPrefInt32(kVolumePref, value);
}

Error
Preferences::
GetUsersName(char* name, uint32* len)
{
    return GetPrefString(kUserNamePref, name, len);
}

Error
Preferences::
SetUsersName(const char* name)
{
    return SetPrefString(kUserNamePref, name);
}

Error 
Preferences::
GetSaveMusicDirectory(char* path, uint32* len)
{
    return GetPrefString(kSaveMusicDirPref, path, len);
}

Error 
Preferences::
SetSaveMusicDirectory(const char* path)
{
    return SetPrefString(kSaveMusicDirPref, path);
}

Error
Preferences::
GetCDDevicePath(char* path, uint32* len)
{
    return GetPrefString(kCDDevicePathPref, path, len);
}

Error
Preferences::
SetCDDevicePath(const char* path)
{
    return SetPrefString(kCDDevicePathPref, path);
}

Error
Preferences::
GetReclaimFiletypes(bool* value)
{
    return GetPrefBoolean(kReclaimFiletypesPref, value);
}

Error
Preferences::
SetReclaimFiletypes(bool value)
{
    return SetPrefBoolean(kReclaimFiletypesPref, value);
}

Error
Preferences::
GetAskToReclaimFiletypes(bool* value)
{
    return GetPrefBoolean(kAskToReclaimFiletypesPref, value);
}

Error
Preferences::
SetAskToReclaimFiletypes(bool value)
{
    return SetPrefBoolean(kAskToReclaimFiletypesPref, value);
}

Error 
Preferences::
GetUsersPortablePlayers(char* list, uint32* len)
{
    return GetPrefString(kUsersPortablePlayersPref, list, len);
}

Error 
Preferences::
SetUsersPortablePlayers(const char* list)
{
    return SetPrefString(kUsersPortablePlayersPref, list);
}

Error
Preferences::
GetShowToolbarTextLabels(bool* value)
{
    return GetPrefBoolean(kShowToolbarTextLabelsPref, value);
}

Error
Preferences::
SetShowToolbarTextLabels(bool value)
{
    return SetPrefBoolean(kShowToolbarTextLabelsPref, value);
}

Error
Preferences::
GetShowToolbarImages(bool* value)
{
    return GetPrefBoolean(kShowToolbarImagesPref, value);
}

Error
Preferences::
SetShowToolbarImages(bool value)
{
    return SetPrefBoolean(kShowToolbarImagesPref, value);
}

Error
Preferences::
GetSaveCurrentPlaylistOnExit(bool* value)
{
    return GetPrefBoolean(kSaveCurrentPlaylistOnExitPref, value);
}

Error
Preferences::
SetSaveCurrentPlaylistOnExit(bool value)
{
    return SetPrefBoolean(kSaveCurrentPlaylistOnExitPref, value);
}

Error
Preferences::
GetViewMusicBrowser(bool* value)
{
    return GetPrefBoolean(kViewMusicBrowserPref, value);
}

Error
Preferences::
SetViewMusicBrowser(bool value)
{
    return SetPrefBoolean(kViewMusicBrowserPref, value);
}


Error
Preferences::
GetPlayImmediately(bool* value)
{
    return GetPrefBoolean(kPlayImmediatelyPref, value);
}

Error
Preferences::
SetPlayImmediately(bool value)
{
    return SetPrefBoolean(kPlayImmediatelyPref, value);
}

Error
Preferences::
GetNumberOfURLsToRemember(int32* value)
{
    return GetPrefInt32(kNumberOfURLsToRememberPref, value);
}

Error
Preferences::
SetNumberOfURLsToRemember(int32 value)
{
    return SetPrefInt32(kNumberOfURLsToRememberPref, value);
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


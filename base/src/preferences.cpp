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
const char* kSecondaryUIPref = "SecondaryUI";
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
const char* kConvertUnderscoresToSpacesPref = "ConvertUnderscoresToSpaces";
const char* kAllowMultipleInstancesPref = "AllowMultipleInstances";
const char* kWAVOutputPathPref = "WAVOutputPath";
const char* kAudioCDLengthPref = "AudioCDLength";
const char* kWatchThisDirectoryPref = "WatchThisDirectory";
const char* kWatchThisDirTimeoutPref = "WatchThisDirectoryTimeout";
const char* kWriteID3v1Pref = "WriteID3v1Tags";
const char* kWriteID3v2Pref = "WriteID3v2Tags";
const char* kEqualizerSettingsPref = "EqualizerSettings";
const char* kSavedPlaylistPositionPref = "SavedPlaylistPosition";
const char* kMusicBrowserPositionPref = "MusicBrowserPosition";
const char* kMusicBrowserHeaderWidthsPref = "MusicBrowserHeaderWidths";
const char* kCloseDLMOnCompletePref = "CloseDLMOnComplete";

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
const char *kDefaultCDDevicePath = "cdaudio";
#else
const char *kDefaultThemeDefaultFont = "Helvetica";
const char *kDefaultCDDevicePath = "/dev/cdrom";
#endif
const char *kDefaultDownloadManagerUI = "download.ui";
const char *kDefaultMusicBrowserUI = "musicbrowser.ui";
const char *kDefaultToolbarUI = "toolbar.ui";
const char *kDefaultSecondaryUI = "";
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
const bool kDefaultConvertUnderscoresToSpaces = true;
const bool kDefaultAllowMultipleInstances = false;
const char* kDefaultWAVOutPath = ".";
const int32 kDefaultAudioCDLength = 4440; // 74 minutes
const int32 kDefaultWatchThisDirTimeout = 600000;
const bool  kDefaultWriteID3v1 = true;
const bool  kDefaultWriteID3v2 = true;
const char* kDefaultEqualizerSettings = "1,50,50,50,50,50,50,50,50,50,50";
const uint32 kDefaultSavedPlaylistPosition = 0;
const char* kDefaultMusicBrowserPosition = "-1,-1,-1,-1,-1";
const char* kDefaultMusicBrowserHeaderWidths = "-1,-1,-1.-1";
const bool  kDefaultCloseDLMOnComplete = false;

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

    dummyInt = 255;
    if (GetPrefString(kSecondaryUIPref, dummyString,
        (uint32 *)&dummyInt) == kError_NoPrefValue)
        SetPrefString(kSecondaryUIPref, kDefaultSecondaryUI);

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

    if (GetPrefBoolean(kConvertUnderscoresToSpacesPref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kConvertUnderscoresToSpacesPref, kDefaultConvertUnderscoresToSpaces);

    if (GetPrefBoolean(kAllowMultipleInstancesPref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kAllowMultipleInstancesPref, kDefaultAllowMultipleInstances);

	dummyInt = 255;
    if (GetPrefString(kWAVOutputPathPref, dummyString, 
        (uint32 *)&dummyInt) == kError_NoPrefValue)
        SetPrefString(kWAVOutputPathPref, kDefaultWAVOutPath);

    if (GetPrefInt32(kAudioCDLengthPref, &dummyInt) == kError_NoPrefValue)
        SetPrefInt32(kAudioCDLengthPref, kDefaultAudioCDLength);

    if (GetPrefInt32(kWatchThisDirTimeoutPref, &dummyInt) == kError_NoPrefValue)
        SetPrefInt32(kWatchThisDirTimeoutPref, kDefaultWatchThisDirTimeout);

    if (GetPrefBoolean(kWriteID3v1Pref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kWriteID3v1Pref, kDefaultWriteID3v1);

    if (GetPrefBoolean(kWriteID3v2Pref, &dummyBool) == kError_NoPrefValue)
        SetPrefBoolean(kWriteID3v2Pref, kDefaultWriteID3v2);

    if (GetPrefBoolean(kCloseDLMOnCompletePref, &dummyBool) == 
        kError_NoPrefValue)
        SetPrefBoolean(kCloseDLMOnCompletePref, kDefaultCloseDLMOnComplete);

	dummyInt = 255;
    if (GetPrefString(kEqualizerSettingsPref, dummyString, 
        (uint32 *)&dummyInt) == kError_NoPrefValue)
        SetPrefString(kEqualizerSettingsPref, kDefaultEqualizerSettings);

    if (GetPrefInt32(kSavedPlaylistPositionPref, &dummyInt) == kError_NoPrefValue)
        SetPrefInt32(kSavedPlaylistPositionPref, kDefaultSavedPlaylistPosition);

    dummyInt = 255;
    if (GetPrefString(kMusicBrowserPositionPref, dummyString, 
        (uint32 *)&dummyInt) == kError_NoPrefValue)
        SetPrefString(kMusicBrowserPositionPref, kDefaultMusicBrowserPosition);

    dummyInt = 255;
    if (GetPrefString(kMusicBrowserHeaderWidthsPref, dummyString, 
        (uint32 *)&dummyInt) == kError_NoPrefValue)
        SetPrefString(kMusicBrowserHeaderWidthsPref, kDefaultMusicBrowserHeaderWidths);

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
GetPrefString(kInstallDirPref, char* path, uint32* len)
{
    return GetPrefString(kInstallDirPref, path, len);
}

Error 
Preferences::
SetPrefString(kInstallDirPref, const char* path)
{
    return SetPrefString(kInstallDirPref, path);
}

Error 
Preferences::
GetPrefString(kUIPref, char* name, uint32* len)
{
    return GetPrefString(kUIPref, name, len);
}

Error 
Preferences::
SetPrefString(kUIPref, const char* name)
{
    return SetPrefString(kUIPref, name);
}

Error 
Preferences::
GetPrefString(kTextUIPref, char* name, uint32* len)
{
    return GetPrefString(kTextUIPref, name, len);
}

Error 
Preferences::
SetPrefString(kTextUIPref, const char* name)
{
    return SetPrefString(kTextUIPref, name);
}

Error 
Preferences::
GetPrefString(kSecondaryUIPref, char* name, uint32* len)
{
    return GetPrefString(kSecondaryUIPref, name, len);
}

Error 
Preferences::
SetPrefString(kSecondaryUIPref, const char* name)
{
    return SetPrefString(kSecondaryUIPref, name);
}

Error 
Preferences::
GetPrefString(kPMOPref, char* name, uint32* len)
{
    return GetPrefString(kPMOPref, name, len);
}

Error 
Preferences::
SetPrefString(kPMOPref, const char* name)
{
    return SetPrefString(kPMOPref, name);
}

Error 
Preferences::
GetPrefString(kOpenSaveDirPref, char* path, uint32* len)
{
    return GetPrefString(kOpenSaveDirPref, path, len);
}

Error 
Preferences::
SetPrefString(kOpenSaveDirPref, const char* path)
{
    return SetPrefString(kOpenSaveDirPref, path);
}

Error 
Preferences::
GetPrefBoolean(kStayOnTopPref, bool* value)
{
    return GetPrefBoolean(kStayOnTopPref, value);
}

Error 
Preferences::
SetPrefBoolean(kStayOnTopPref, bool value)
{
    return SetPrefBoolean(kStayOnTopPref, value);
}

Error 
Preferences::
GetPrefBoolean(kLiveInTrayPref, bool* value)
{
    return GetPrefBoolean(kLiveInTrayPref, value);
}

Error 
Preferences::
SetPrefBoolean(kLiveInTrayPref, bool value)
{
    return SetPrefBoolean(kLiveInTrayPref, value);
}

Error 
Preferences::
GetPrefInt32(kInputBufferSizePref, int32* value)
{
    return GetPrefInt32(kInputBufferSizePref, value);
}

Error 
Preferences::
SetPrefInt32(kInputBufferSizePref, int32 value)
{
    return SetPrefInt32(kInputBufferSizePref, value);
}

Error 
Preferences::
GetPrefInt32(kOutputBufferSizePref, int32* value)
{
    return GetPrefInt32(kOutputBufferSizePref, value);
}

Error 
Preferences::
SetPrefInt32(kOutputBufferSizePref, int32 value)
{
    return SetPrefInt32(kOutputBufferSizePref, value);
}

Error 
Preferences::
GetPrefInt32(kStreamBufferIntervalPref, int32* value)
{
    return GetPrefInt32(kStreamBufferIntervalPref, value);
}

Error 
Preferences::
SetPrefInt32(kStreamBufferIntervalPref, int32 value)
{
    return SetPrefInt32(kStreamBufferIntervalPref, value);
}

Error 
Preferences::
GetPrefInt32(kDecoderThreadPriorityPref, int32* value)
{
    return GetPrefInt32(kDecoderThreadPriorityPref, value);
}

Error 
Preferences::
SetPrefInt32(kDecoderThreadPriorityPref, int32 value)
{
    return SetPrefInt32(kDecoderThreadPriorityPref, value);
}

Error 
Preferences::
SetPrefBoolean(kUseDebugLogPref, bool value)
{
    return SetPrefBoolean(kUseDebugLogPref, value);
}

Error 
Preferences::
GetPrefBoolean(kUseDebugLogPref, bool* value)
{
    return GetPrefBoolean(kUseDebugLogPref, value);
}

Error 
Preferences::
SetPrefBoolean(kLogMainPref, bool value)
{
    return SetPrefBoolean(kLogMainPref, value);
}

Error 
Preferences::
GetPrefBoolean(kLogMainPref, bool* value)
{
    return GetPrefBoolean(kLogMainPref, value);
}

Error 
Preferences::
SetPrefBoolean(kLogDecodePref, bool value)
{
    return SetPrefBoolean(kLogDecodePref, value);
}

Error 
Preferences::
GetPrefBoolean(kLogDecodePref, bool* value)
{
    return GetPrefBoolean(kLogDecodePref, value);
}

Error 
Preferences::
SetPrefBoolean(kLogInputPref, bool value)
{
    return SetPrefBoolean(kLogInputPref, value);
}

Error 
Preferences::
GetPrefBoolean(kLogInputPref, bool* value)
{
    return GetPrefBoolean(kLogInputPref, value);
}

Error 
Preferences::
SetPrefBoolean(kLogOutputPref, bool value)
{
    return SetPrefBoolean(kLogOutputPref, value);
}

Error 
Preferences::
GetPrefBoolean(kLogOutputPref, bool* value)
{
    return GetPrefBoolean(kLogOutputPref, value);
}

Error 
Preferences::
SetPrefBoolean(kLogPerformancePref, bool value)
{
    return SetPrefBoolean(kLogPerformancePref, value);
}

Error 
Preferences::
GetPrefBoolean(kLogPerformancePref, bool* value)
{
    return GetPrefBoolean(kLogPerformancePref, value);
}

Error 
Preferences::
GetPrefBoolean(kSaveStreamsPref, bool* value)
{
    return GetPrefBoolean(kSaveStreamsPref, value);
}

Error 
Preferences::
SetPrefBoolean(kSaveStreamsPref, bool value)
{
    return SetPrefBoolean(kSaveStreamsPref, value);
}

Error 
Preferences::
GetPrefString(kSaveStreamsDirPref, char* path, uint32* len)
{
    return GetPrefString(kSaveStreamsDirPref, path, len);
}

Error 
Preferences::
SetPrefString(kSaveStreamsDirPref, const char* path)
{
    return SetPrefString(kSaveStreamsDirPref, path);
}

Error
Preferences::
GetPrefBoolean(kUseProxyPref, bool* value)
{
    return GetPrefBoolean(kUseProxyPref, value);
}

Error
Preferences::
SetPrefBoolean(kUseProxyPref, bool value)
{
    return SetPrefBoolean(kUseProxyPref, value);
}

Error
Preferences::
GetPrefString(kProxyHostPref, char* host, uint32* len)
{
    return GetPrefString(kProxyHostPref, host, len);
}

Error
Preferences::
SetPrefString(kProxyHostPref, const char* host)
{
    return SetPrefString(kProxyHostPref, host);
}

Error
Preferences::
GetPrefString(kCDDBServerPref, char* host, uint32* len)
{
    return GetPrefString(kCDDBServerPref, host, len);
}

Error
Preferences::
SetPrefString(kCDDBServerPref, const char* host)
{
    return SetPrefString(kCDDBServerPref, host);
}

Error
Preferences::
GetPrefInt32(kPreBufferPref, int32* value)
{
    return GetPrefInt32(kPreBufferPref, value);
}

Error
Preferences::
SetPrefInt32(kPreBufferPref, int32 value)
{
    return SetPrefInt32(kPreBufferPref, value);
}

Error
Preferences::
GetPrefBoolean(kUseAlternateNICPref, bool* value)
{
    return GetPrefBoolean(kUseAlternateNICPref, value);
}

Error
Preferences::
SetPrefBoolean(kUseAlternateNICPref, bool value)
{
    return SetPrefBoolean(kUseAlternateNICPref, value);
}

Error
Preferences::
GetPrefString(kAlternateNICAddressPref, char* address, uint32* len)
{
    return GetPrefString(kAlternateNICAddressPref, address, len);
}

Error
Preferences::
SetPrefString(kAlternateNICAddressPref, const char* address)
{
    return SetPrefString(kAlternateNICAddressPref, address);
}

Error
Preferences::
GetPrefString(kThemeDefaultFontPref, char* font, uint32* len)
{
    return GetPrefString(kThemeDefaultFontPref, font, len);
}

Error
Preferences::
SetPrefString(kThemeDefaultFontPref, const char* font)
{
    return SetPrefString(kThemeDefaultFontPref, font);
}

Error
Preferences::
GetPrefString(kThemePathPref, char* path, uint32* len)
{
    return GetPrefString(kThemePathPref, path, len);
}

Error
Preferences::
SetPrefString(kThemePathPref, const char* path)
{
    return SetPrefString(kThemePathPref, path);
}

Error
Preferences::
GetPrefBoolean(kCheckForUpdatesPref, bool* value)
{
    return GetPrefBoolean(kCheckForUpdatesPref, value);
}

Error
Preferences::
SetPrefBoolean(kCheckForUpdatesPref, bool value)
{
    return SetPrefBoolean(kCheckForUpdatesPref, value);
}

Error
Preferences::
GetPrefInt32(kPlaylistRepeatPref, int32* value)
{
    return GetPrefInt32(kPlaylistRepeatPref, value);
}

Error
Preferences::
SetPrefInt32(kPlaylistRepeatPref, int32 value)
{
    return SetPrefInt32(kPlaylistRepeatPref, value);
}


Error
Preferences::
GetPrefBoolean(kPlaylistShufflePref, bool* value)
{
    return GetPrefBoolean(kPlaylistShufflePref, value);
}

Error
Preferences::
SetPrefBoolean(kPlaylistShufflePref, bool value)
{
    return SetPrefBoolean(kPlaylistShufflePref, value);
}

Error
Preferences::
GetPrefInt32(kTimeDisplayPref, int32* value)
{
    return GetPrefInt32(kTimeDisplayPref, value);
}

Error
Preferences::
SetPrefInt32(kTimeDisplayPref, int32 value)
{
    return SetPrefInt32(kTimeDisplayPref, value);
}

Error
Preferences::
GetPrefInt32(kVolumePref, int32* value)
{
    return GetPrefInt32(kVolumePref, value);
}

Error
Preferences::
SetPrefInt32(kVolumePref, int32 value)
{
    return SetPrefInt32(kVolumePref, value);
}

Error
Preferences::
GetPrefString(kUserNamePref, char* name, uint32* len)
{
    return GetPrefString(kUserNamePref, name, len);
}

Error
Preferences::
SetPrefString(kUserNamePref, const char* name)
{
    return SetPrefString(kUserNamePref, name);
}

Error 
Preferences::
GetPrefString(kSaveMusicDirPref, char* path, uint32* len)
{
    return GetPrefString(kSaveMusicDirPref, path, len);
}

Error 
Preferences::
SetPrefString(kSaveMusicDirPref, const char* path)
{
    return SetPrefString(kSaveMusicDirPref, path);
}

Error
Preferences::
GetPrefString(kCDDevicePathPref, char* path, uint32* len)
{
    return GetPrefString(kCDDevicePathPref, path, len);
}

Error
Preferences::
SetPrefString(kCDDevicePathPref, const char* path)
{
    return SetPrefString(kCDDevicePathPref, path);
}

Error
Preferences::
GetPrefBoolean(kReclaimFiletypesPref, bool* value)
{
    return GetPrefBoolean(kReclaimFiletypesPref, value);
}

Error
Preferences::
SetPrefBoolean(kReclaimFiletypesPref, bool value)
{
    return SetPrefBoolean(kReclaimFiletypesPref, value);
}

Error
Preferences::
GetPrefBoolean(kAskToReclaimFiletypesPref, bool* value)
{
    return GetPrefBoolean(kAskToReclaimFiletypesPref, value);
}

Error
Preferences::
SetPrefBoolean(kAskToReclaimFiletypesPref, bool value)
{
    return SetPrefBoolean(kAskToReclaimFiletypesPref, value);
}

Error 
Preferences::
GetPrefString(kUsersPortablePlayersPref, char* list, uint32* len)
{
    return GetPrefString(kUsersPortablePlayersPref, list, len);
}

Error 
Preferences::
SetPrefString(kUsersPortablePlayersPref, const char* list)
{
    return SetPrefString(kUsersPortablePlayersPref, list);
}

Error
Preferences::
GetPrefBoolean(kShowToolbarTextLabelsPref, bool* value)
{
    return GetPrefBoolean(kShowToolbarTextLabelsPref, value);
}

Error
Preferences::
SetPrefBoolean(kShowToolbarTextLabelsPref, bool value)
{
    return SetPrefBoolean(kShowToolbarTextLabelsPref, value);
}

Error
Preferences::
GetPrefBoolean(kShowToolbarImagesPref, bool* value)
{
    return GetPrefBoolean(kShowToolbarImagesPref, value);
}

Error
Preferences::
SetPrefBoolean(kShowToolbarImagesPref, bool value)
{
    return SetPrefBoolean(kShowToolbarImagesPref, value);
}

Error
Preferences::
GetPrefBoolean(kSaveCurrentPlaylistOnExitPref, bool* value)
{
    return GetPrefBoolean(kSaveCurrentPlaylistOnExitPref, value);
}

Error
Preferences::
SetPrefBoolean(kSaveCurrentPlaylistOnExitPref, bool value)
{
    return SetPrefBoolean(kSaveCurrentPlaylistOnExitPref, value);
}

Error
Preferences::
GetPrefBoolean(kViewMusicBrowserPref, bool* value)
{
    return GetPrefBoolean(kViewMusicBrowserPref, value);
}

Error
Preferences::
SetPrefBoolean(kViewMusicBrowserPref, bool value)
{
    return SetPrefBoolean(kViewMusicBrowserPref, value);
}


Error
Preferences::
GetPrefBoolean(kPlayImmediatelyPref, bool* value)
{
    return GetPrefBoolean(kPlayImmediatelyPref, value);
}

Error
Preferences::
SetPrefBoolean(kPlayImmediatelyPref, bool value)
{
    return SetPrefBoolean(kPlayImmediatelyPref, value);
}

Error
Preferences::
GetPrefInt32(kNumberOfURLsToRememberPref, int32* value)
{
    return GetPrefInt32(kNumberOfURLsToRememberPref, value);
}

Error
Preferences::
SetPrefInt32(kNumberOfURLsToRememberPref, int32 value)
{
    return SetPrefInt32(kNumberOfURLsToRememberPref, value);
}

Error
Preferences::
GetPrefBoolean(kConvertUnderscoresToSpacesPref, bool* value)
{
    return GetPrefBoolean(kConvertUnderscoresToSpacesPref, value);
}

Error
Preferences::
SetPrefBoolean(kConvertUnderscoresToSpacesPref, bool value)
{
    return SetPrefBoolean(kConvertUnderscoresToSpacesPref, value);
}

Error
Preferences::
GetPrefBoolean(kAllowMultipleInstancesPref, bool* value)
{
    return GetPrefBoolean(kAllowMultipleInstancesPref, value);
}

Error
Preferences::
SetPrefBoolean(kAllowMultipleInstancesPref, bool value)
{
    return SetPrefBoolean(kAllowMultipleInstancesPref, value);
}

Error 
Preferences::
GetPrefString(kWAVOutputPathPref, char* path, uint32* len)
{
    return GetPrefString(kWAVOutputPathPref, path, len);
}

Error 
Preferences::
SetPrefString(kWAVOutputPathPref, const char* path)
{
    return SetPrefString(kWAVOutputPathPref, path);
}

Error
Preferences::
GetPrefInt32(kAudioCDLengthPref, int32* value)
{
    return GetPrefInt32(kAudioCDLengthPref, value);
}

Error
Preferences::
SetPrefInt32(kAudioCDLengthPref, int32 value)
{
    return SetPrefInt32(kAudioCDLengthPref, value);
}

Error
Preferences::
GetPrefString(kWatchThisDirectoryPref, char* path, uint32* len)
{
    return GetPrefString(kWatchThisDirectoryPref, path, len);
}

Error
Preferences::
SetPrefString(kWatchThisDirectoryPref, const char* path)
{
    return SetPrefString(kWatchThisDirectoryPref, path);
}

Error
Preferences::
GetPrefInt32(kWatchThisDirTimeoutPref, int32* value)
{
    return GetPrefInt32(kWatchThisDirTimeoutPref, value);
}

Error
Preferences::
SetPrefInt32(kWatchThisDirTimeoutPref, int32 value)
{
    return SetPrefInt32(kWatchThisDirTimeoutPref, value);
}

Error
Preferences::
GetPrefBoolean(kWriteID3v1Pref, bool* value)
{
    return GetPrefBoolean(kWriteID3v1Pref, value);
}

Error
Preferences::
SetPrefBoolean(kWriteID3v1Pref, bool value)
{
    return SetPrefBoolean(kWriteID3v1Pref, value);
}

Error
Preferences::
GetPrefBoolean(kWriteID3v2Pref, bool* value)
{
    return GetPrefBoolean(kWriteID3v2Pref, value);
}

Error
Preferences::
SetPrefBoolean(kWriteID3v2Pref, bool value)
{
    return SetPrefBoolean(kWriteID3v2Pref, value);
}

Error
Preferences::
GetPrefString(kEqualizerSettingsPref, char* settings, uint32* len)
{
    return GetPrefString(kEqualizerSettingsPref, settings, len);
}

Error
Preferences::
SetPrefString(kEqualizerSettingsPref, const char* settings)
{
    return SetPrefString(kEqualizerSettingsPref, settings);
}

Error
Preferences::
GetPrefInt32(kSavedPlaylistPositionPref, uint32* value)
{
    return GetPrefInt32(kSavedPlaylistPositionPref, (int32*)value);
}

Error
Preferences::
SetPrefInt32(kSavedPlaylistPositionPref, uint32 value)
{
    return SetPrefInt32(kSavedPlaylistPositionPref, (int32)value);
}

Error
Preferences::
GetPrefBoolean(kCloseDLMOnCompletePref, bool* value)
{
    return GetPrefBoolean(kCloseDLMOnCompletePref, value);
}

Error
Preferences::
SetPrefBoolean(kCloseDLMOnCompletePref, bool value)
{
    return SetPrefBoolean(kCloseDLMOnCompletePref, value);
}

LibDirFindHandle *
Preferences::
GetFirstLibDir(char *path, uint32 *len)
{
    Error error;
    
    error = GetPrefString(kInstallDirPref, path, len);
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


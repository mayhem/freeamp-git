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

#ifndef INCLUDED_PREFERENCES_H
#define INCLUDED_PREFERENCES_H

#include "config.h"
#include "errors.h"

// preferences
extern const char* kInstallDirPref;
extern const char* kLibraryPathPref;
extern const char* kUIPref;
extern const char* kTextUIPref;
extern const char* kMusicBrowserUIPref;
extern const char* kDownloadManagerUIPref;
extern const char* kToolbarUIPref;
extern const char* kPMOPref;
extern const char* kALSADevicePref;
extern const char* kESOUNDHostPref;
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
extern const char* kSaveStreamsPref;
extern const char* kSaveStreamsDirPref;
extern const char* kUseProxyPref;
extern const char* kProxyHostPref;
extern const char* kPreBufferPref;
extern const char* kUseAlternateNICPref;
extern const char* kAlternateNICAddressPref;
extern const char* kUseTitleStreamingPref;  
extern const char* kThemePathPref;
extern const char* kMainWindowPosPref;
extern const char* kWindowModePref;
extern const char* kThemeDefaultFontPref;
extern const char* kSaveMusicDirPref;
extern const char* kDatabaseDirPref;
extern const char* kCheckForUpdatesPref;
extern const char* kPlaylistRepeatPref;
extern const char* kPlaylistShufflePref;
extern const char* kTimeDisplayPref;
extern const char* kVolumePref;
extern const char* kUserNamePref;
extern const char* kReclaimFiletypesPref;
extern const char* kAskToReclaimFiletypesPref;
extern const char* kUsersPortablePlayersPref;
extern const char* kShowToolbarTextLabelsPref;
extern const char* kShowToolbarImagesPref;
extern const char* kSaveCurrentPlaylistOnExitPref;
extern const char* kViewMusicBrowserPref;
extern const char* kWelcomePref;
extern const char* kPlayImmediatelyPref;
extern const char* kNumberOfURLsToRememberPref;
extern const char* kCDDevicePathPref;
extern const char* kCDDBServerPref;
extern const char* kConvertUnderscoresToSpacesPref;
extern const char* kAllowMultipleInstancesPref;
extern const char* kAudioCDLengthPref;

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
    Error SetInstallDirectory(const char* path);

    Error GetDefaultUI(char* name, uint32* len);
    Error SetDefaultUI(const char* name);

    Error GetDefaultTextUI(char* name, uint32* len);
    Error SetDefaultTextUI(const char* name);

    Error GetDefaultPMO(char* name, uint32* len);
    Error SetDefaultPMO(const char* name);

    Error GetOpenSaveDirectory(char* path, uint32* len);
    Error SetOpenSaveDirectory(const char* path);

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
    Error SetSaveStreamsDirectory(const char* path);

    Error GetUseProxyServer(bool* value);
    Error SetUseProxyServer(bool value);

    Error GetProxyServerAddress(char* host, uint32* len);
    Error SetProxyServerAddress(const char* host);

    Error GetCDDBServerAddress(char* host, uint32* len);
    Error SetCDDBServerAddress(const char* host);

    Error GetPrebufferLength(int32* value);
    Error SetPrebufferLength(int32 value);

    Error GetUseAlternateNIC(bool* value);
    Error SetUseAlternateNIC(bool value);

    Error GetAlternateNICAddress(char* address, uint32* len);
    Error SetAlternateNICAddress(const char* address);

    Error GetThemeDefaultFont(char* font, uint32* len);
    Error SetThemeDefaultFont(const char* font);

    Error GetThemePath(char* path, uint32* len);
    Error SetThemePath(const char* path);

    Error GetCheckForUpdates(bool* value);
    Error SetCheckForUpdates(bool value);

    Error GetPlaylistRepeat(int32* value);
    Error SetPlaylistRepeat(int32 value);

    Error GetPlaylistShuffle(bool* value);
    Error SetPlaylistShuffle(bool value);

    Error GetTimeDisplay(int32* value);
    Error SetTimeDisplay(int32 value);

    Error GetVolume(int32* value);
    Error SetVolume(int32 value);  

    Error GetUsersName(char* name, uint32* len);
    Error SetUsersName(const char* name);

    Error GetSaveMusicDirectory(char* path, uint32* len);
    Error SetSaveMusicDirectory(const char* path);

    Error GetReclaimFiletypes(bool* value);
    Error SetReclaimFiletypes(bool value);

    Error GetAskToReclaimFiletypes(bool* value);
    Error SetAskToReclaimFiletypes(bool value);

    Error GetUsersPortablePlayers(char* list, uint32* len);
    Error SetUsersPortablePlayers(const char* list);

    Error GetShowToolbarTextLabels(bool* value);
    Error SetShowToolbarTextLabels(bool value);

    Error GetShowToolbarImages(bool* value);
    Error SetShowToolbarImages(bool value);

    Error GetSaveCurrentPlaylistOnExit(bool* value);
    Error SetSaveCurrentPlaylistOnExit(bool value);

    Error GetViewMusicBrowser(bool* value);
    Error SetViewMusicBrowser(bool value);

    Error GetPlayImmediately(bool* value);
    Error SetPlayImmediately(bool value);

    Error GetNumberOfURLsToRemember(int32* value);
    Error SetNumberOfURLsToRemember(int32 value);

    Error GetCDDevicePath(char* path, uint32* len);
    Error SetCDDevicePath(const char* path);

    Error GetConvertUnderscoresToSpaces(bool* value);
    Error SetConvertUnderscoresToSpaces(bool value);

    Error GetAllowMultipleInstances(bool* value);
    Error SetAllowMultipleInstances(bool  value);  

    Error GetWAVOutDirectory(char* path, uint32* len);
    Error SetWAVOutDirectory(const char* path);

    Error GetAudioCDLength(int32* value);
    Error SetAudioCDLength(int32  value);
};

#endif /* _PREFERENCES_H */

/*____________________________________________________________________________

   FreeAmp - The Free MP3 Player

   Copyright (C) 1999 EMusic

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

#ifndef INCLUDED_WIN32PREFERENCEWINDOW_H__
#define INCLUDED_WIN32PREFERENCEWINDOW_H__

// The debugger can't handle symbols more than 255 characters long.
// STL often creates symbols longer than that.
// When symbols are longer than 255 characters, the warning is disabled.
#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <map>
#include <set>

using namespace std;

#include "config.h"
#include "PreferenceWindow.h"
#include "preferences.h"
#include "log.h"
#include "registrar.h"
#include "resource.h"

typedef set<string> PortableSet;

typedef struct PrefsStruct 
{
    // page 1
    string defaultUI;
    string defaultPMO;
    int32 inputBufferSize;
    int32 outputBufferSize;
    int32 preBufferLength;
    int32 decoderThreadPriority;
    bool stayOnTop;
    bool liveInTray;

    // page 2
    int32 streamInterval;
    bool saveStreams;
    string saveStreamsDirectory;
    bool useProxyServer;
    string proxyServer;
    bool useAlternateIP;
    string alternateIP;
    
    // page 3
    bool enableLogging;
    bool logMain;
    bool logInput;
    bool logOutput;
    bool logDecoder;
    bool logPerformance;

	// page 5
    string defaultFont;
    string currentTheme;

    // page 6
    bool checkForUpdates;

    bool reclaimFiletypes;
    bool askReclaimFiletypes;
    string saveMusicDirectory;

    PortableSet portablePlayers;

    
    bool operator == (const struct PrefsStruct& pref)
    {
        return (
            defaultUI == pref.defaultUI &&
            defaultPMO == pref.defaultPMO &&
            inputBufferSize == pref.inputBufferSize &&
            outputBufferSize == pref.outputBufferSize &&
            preBufferLength == pref.preBufferLength &&
            decoderThreadPriority == pref.decoderThreadPriority &&
            stayOnTop == pref.stayOnTop &&
            liveInTray == pref.liveInTray &&
            streamInterval == pref.streamInterval &&
            saveStreams == pref.saveStreams &&
            saveStreamsDirectory == pref.saveStreamsDirectory &&
            useProxyServer == pref.useProxyServer &&
            proxyServer == pref.proxyServer &&
            useAlternateIP == pref.useAlternateIP &&
            alternateIP == pref.alternateIP &&
            enableLogging == pref.enableLogging &&
            logMain == pref.logMain &&
            logInput == pref.logInput &&
            logOutput == pref.logOutput &&
            logDecoder == pref.logDecoder &&
            logPerformance == pref.logPerformance &&

            defaultFont == pref.defaultFont &&
            currentTheme == pref.currentTheme &&

            checkForUpdates == pref.checkForUpdates &&

            reclaimFiletypes == pref.reclaimFiletypes &&
            askReclaimFiletypes == pref.askReclaimFiletypes &&
            saveMusicDirectory == pref.saveMusicDirectory &&

            portablePlayers == pref.portablePlayers &&

            true
        );
    }

    bool operator != (const struct PrefsStruct& pref)
    {
        return ! (*this == pref);
    }

} PrefsStruct;

class Win32PreferenceWindow : public PreferenceWindow
{
    public:

               Win32PreferenceWindow(FAContext *context,
                                     ThemeManager *pThemeMan);
      virtual ~Win32PreferenceWindow(void); 
      
      virtual  bool Show(Window *pParent);

               void GetPrefsValues(Preferences* prefs, 
                                   PrefsStruct* values);

               void SavePrefsValues(Preferences* prefs, 
                                    PrefsStruct* values);

               bool PrefGeneralProc(HWND hwnd, 
                                  UINT msg, 
                                  WPARAM wParam, 
                                  LPARAM lParam);
               bool PrefThemeProc(HWND hwnd, 
                                  UINT msg, 
                                  WPARAM wParam, 
                                  LPARAM lParam);
               bool PrefStreamingProc(HWND hwnd, 
                                  UINT msg, 
                                  WPARAM wParam, 
                                  LPARAM lParam);
               bool PrefPluginsProc(HWND hwnd, 
                                  UINT msg, 
                                  WPARAM wParam, 
                                  LPARAM lParam);
               bool PrefUpdateProc(HWND hwnd, 
                                  UINT msg, 
                                  WPARAM wParam, 
                                  LPARAM lParam);
               bool PrefAdvancedProc(HWND hwnd, 
                                  UINT msg, 
                                  WPARAM wParam, 
                                  LPARAM lParam);
               bool PrefAboutProc(HWND hwnd, 
                                  UINT msg, 
                                  WPARAM wParam, 
                                  LPARAM lParam);
               bool PrefDebugProc(HWND hwnd, 
                                  UINT msg, 
                                  WPARAM wParam, 
                                  LPARAM lParam);

	protected:
    
               bool DisplayPreferences(HWND hwndParent, 
                                       Preferences* prefs);
               void LoadThemeListBox  (HWND hwnd);
    
    
      PrefsStruct  m_originalValues;
      PrefsStruct  m_currentValues;
      PrefsStruct  m_proposedValues;
    
      map<string, string> m_oThemeList;
};

#endif

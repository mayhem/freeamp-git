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

using namespace std;

#include "config.h"
#include "PreferenceWindow.h"
#include "preferences.h"
#include "log.h"
#include "registrar.h"
#include "resource.h"

typedef struct PrefsStruct 
{
    Preferences* prefs;

    // page 1
    char defaultUI[256];
    char defaultPMO[256];
    int32 inputBufferSize;
    int32 outputBufferSize;
    int32 preBufferLength;
    int32 decoderThreadPriority;
    bool stayOnTop;
    bool liveInTray;

    // page 2
    int32 streamInterval;
    bool saveStreams;
    char saveStreamsDirectory[MAX_PATH + 1];
    bool useProxyServer;
    char proxyServer[256]; // is there a domain name length limit???
    bool useAlternateIP;
    char alternateIP[16];
    
    // page 3
    bool enableLogging;
    bool logMain;
    bool logInput;
    bool logOutput;
    bool logDecoder;
    bool logPerformance;

	// page 5
    char   defaultFont[64];
    bool   fontChanged;
    string currentTheme;
    int    listboxIndex;

    PrefsStruct()
    {
        memset(defaultUI, 0x00, sizeof(defaultUI));
        memset(defaultPMO, 0x00, sizeof(defaultPMO));
        memset(saveStreamsDirectory, 0x00, sizeof(saveStreamsDirectory));
        memset(proxyServer, 0x00, sizeof(proxyServer));
        memset(alternateIP, 0x00, sizeof(alternateIP));
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

               bool PrefPage1Proc(HWND hwnd, 
                                  UINT msg, 
                                  WPARAM wParam, 
                                  LPARAM lParam);
               bool PrefPage2Proc(HWND hwnd, 
                                  UINT msg, 
                                  WPARAM wParam, 
                                  LPARAM lParam);
               bool PrefPage3Proc(HWND hwnd, 
                                  UINT msg, 
                                  WPARAM wParam, 
                                  LPARAM lParam);
               bool PrefPage4Proc(HWND hwnd, 
                                  UINT msg, 
                                  WPARAM wParam, 
                                  LPARAM lParam);
               bool PrefPage5Proc(HWND hwnd, 
                                  UINT msg, 
                                  WPARAM wParam, 
                                  LPARAM lParam);
               bool PrefPage6Proc(HWND hwnd, 
                                  UINT msg, 
                                  WPARAM wParam, 
                                  LPARAM lParam);

	protected:
    
               bool DisplayPreferences(HWND hwndParent, 
                                       Preferences* prefs);
               void LoadThemeListBox  (HWND hwnd);
    
    
      PrefsStruct  originalValues;
      PrefsStruct  currentValues;
    
      map<string, string> m_oThemeList;
};

#endif

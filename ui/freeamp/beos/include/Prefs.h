/*____________________________________________________________________________

   FreeAmp - The Free MP3 Player

   Copyright (C) 1999 EMusic
   Portions Copyright (C) 1999 Hiromasa Kato (hiro@eng.iastate.edu)

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

#ifndef INCLUDED_PREFS_H__
#define INCLUDED_PREFS_H__

#include "config.h"
#include <string>
#include <set>
using namespace std;

class Preferences;
class ThemeManager;

typedef set<string> PortableSet;

struct Prefs
{
    ThemeManager* themeManager;

    // "Advanced" pane
    string defaultUI;
    string defaultPMO;
    int32 inputBufferSize;
    int32 outputBufferSize;
    int32 preBufferLength;
    int32 decoderThreadPriority;
    bool stayOnTop;
    bool liveInTray;

    // "Streaming" pane
    int32 streamInterval;
    bool saveStreams;
    string saveStreamsDirectory;
    bool useProxyServer;
    string proxyServer;
    bool useAlternateIP;
    string alternateIP;
    
    // "Debug" pane
    bool enableLogging;
    bool logMain;
    bool logInput;
    bool logOutput;
    bool logDecoder;
    bool logPerformance;

	// "Themes" pane
    string defaultFont;
    string currentTheme;

    // "General" pane
    bool checkForUpdates;
    bool reclaimFiletypes;
    bool askReclaimFiletypes;
    string saveMusicDirectory;
    bool useTextLabels;
    bool useImages;
    bool savePlaylistOnExit;
    bool playImmediately;

    PortableSet portablePlayers;
    
    bool operator == (const struct Prefs& pref);

    bool operator != (const struct Prefs& pref);

    void GetPrefsValues( Preferences* prefs, ThemeManager* themeMan );
    void SavePrefsValues( Preferences* prefs, ThemeManager* themeMan );
};

#endif // INCLUDED_PREFS_VAL_H__

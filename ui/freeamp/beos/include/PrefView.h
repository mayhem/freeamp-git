/*____________________________________________________________________________

   FreeAmp - The Free MP3 Player

   Copyright (C) 1999 EMusic
   Copyright (C) 1999 Hiromasa Kato

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

#ifndef INCLUDED_PREF_VIEW_H__
#define INCLUDED_PREF_VIEW_H__

#include <be/interface/View.h>
#include <be/interface/TabView.h>
#include <string>
#include <set>
#include <map>
#include <vector>

using namespace std;

class FAContext;
class Preferences;
class PrefPane;
class ThemeManager;
class BFilePanel;
class BTextControl;

typedef set<string> PortableSet;

typedef struct PrefsStruct 
{
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

            useTextLabels == pref.useTextLabels &&
            useImages == pref.useImages &&

            savePlaylistOnExit == pref.savePlaylistOnExit &&
            playImmediately == pref.playImmediately &&

            true
        );
    }

    bool operator != (const struct PrefsStruct& pref)
    {
        return ! (*this == pref);
    }

} PrefsStruct;

class PrefView : public BView
{
public:
                        PrefView( FAContext* context, ThemeManager* themeMan,
                                  BRect frame, const char* name );
    virtual             ~PrefView();
    virtual void        AttachedToWindow( void );
    virtual void        MessageReceived( BMessage* message );
    void                Apply( void );

protected:
    void                GetPrefsValues( Preferences* prefs,
                                        PrefsStruct* values );
    void                SavePrefsValues( Preferences* prefs,
                                         PrefsStruct* values );
    void                CreateAboutPane( void );
    void                CreateGeneralPane( void );
    void                CreateThemesPane( void );
    void                CreateAdvancedPane( void );

private:
    Preferences*        m_prefs;
    FAContext*          m_context;
    ThemeManager*       m_themeMan;
    map<string, string> m_themeList;
    PrefsStruct         m_originalValues;
    PrefsStruct         m_currentValues;
    PrefsStruct         m_proposedValues;
    PrefPane*           m_general;
    PrefPane*           m_themes;
    PrefPane*           m_advanced;
    PrefPane*           m_about;
    BFilePanel*         m_filePanel;

    BTextControl*       m_saveMusicFolder;

    vector<BControl*>   m_controls; // temp place holder for BControls that need
                                    // target set at the AttachedToWindow() time.
};

class PrefTabView : public BTabView
{
public:
                        PrefTabView( BRect r, const char* name );
    virtual             ~PrefTabView();
    virtual void        Select( int32 tab );
};

class PrefPane : public BView
{
public:
                        PrefPane( BRect r, const char* name );
    virtual             ~PrefPane();
    virtual void        AttachedToWindow( void );
};

#endif // INCLUDED_PREF_VIEW_H__

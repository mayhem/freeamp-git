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

#include "Prefs.h"
#include "ThemeManager.h"
#include "preferences.h"

bool
Prefs::operator == ( const struct Prefs& pref )
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

bool
Prefs::operator != ( const struct Prefs& pref )
{
    return ! (*this == pref);
}

void
Prefs::GetPrefsValues( Preferences* prefs, ThemeManager* themeMan )
{
    themeManager = themeMan;

    uint32 bufferSize = 1;
    uint32 size;

    // get the string prefs
    char* buffer = (char*)malloc(bufferSize);

    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetPrefString(kPMOPref, buffer, &size))
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetPrefString( kPMOPref, buffer, &size );
    }

    defaultPMO = buffer;
    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetPrefString(kUIPref, buffer, &size))
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetPrefString( kUIPref, buffer, &size );
    }
    
    defaultUI = buffer;
    size = bufferSize;

    if ( kError_BufferTooSmall ==
         prefs->GetPrefString( kProxyHostPref, buffer, &size ) )
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetPrefString( kProxyHostPref, buffer, &size );
    }

    proxyServer = buffer;
    size = bufferSize;

    if ( kError_BufferTooSmall ==
         prefs->GetPrefString(kSaveStreamsDirPref, buffer, &size))
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetPrefString( kSaveStreamsDirPref, buffer, &size );
    }

    saveStreamsDirectory = buffer;
    size = bufferSize;

    if ( kError_BufferTooSmall ==
         prefs->GetPrefString( kAlternateNICAddressPref, buffer, &size ) )
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetPrefString( kAlternateNICAddressPref, buffer, &size );
    }

    alternateIP = buffer;
    size = bufferSize;

    if ( kError_BufferTooSmall ==
         prefs->GetPrefString( kThemeDefaultFontPref, buffer, &size ) )
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetPrefString( kThemeDefaultFontPref, buffer, &size );
    }

    defaultFont = buffer;
    size = bufferSize;

    if ( kError_BufferTooSmall ==
         prefs->GetPrefString( kSaveMusicDirPref, buffer, &size ) )
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetPrefString( kSaveMusicDirPref, buffer, &size );
    }

    saveMusicDirectory = buffer;
    size = bufferSize;

    if ( kError_BufferTooSmall ==
         prefs->GetPrefString( kUsersPortablePlayersPref, buffer, &size ) )
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetPrefString( kUsersPortablePlayersPref, buffer, &size );
    }

    char* cp = buffer;
    char* name = cp;

    while((cp = strchr(cp, ';')))
    {
        *cp = 0x00;
        portablePlayers.insert(string(name));
        cp++;
        name = cp;
    }

    if(*name)
    {
        portablePlayers.insert(string(name));
    }

    //portablePlayers = buffer;
    size = bufferSize;

    themeMan->GetCurrentTheme( currentTheme );

    // get the other prefs
    prefs->GetPrefInt32(kDecoderThreadPriorityPref, &decoderThreadPriority);
    prefs->GetPrefInt32(kInputBufferSizePref, &inputBufferSize);
    prefs->GetPrefInt32(kOutputBufferSizePref, &outputBufferSize);
    prefs->GetPrefInt32(kPreBufferPref, &preBufferLength);
    prefs->GetPrefBoolean(kStayOnTopPref, &stayOnTop);
    prefs->GetPrefBoolean(kLiveInTrayPref, &liveInTray);
    prefs->GetPrefBoolean(kSaveStreamsPref, &saveStreams);
    prefs->GetPrefBoolean(kUseProxyPref, &useProxyServer);
    prefs->GetPrefBoolean(kUseAlternateNICPref, &useAlternateIP);
    prefs->GetPrefBoolean(kUseDebugLogPref, &enableLogging);
    prefs->GetPrefBoolean(kLogMainPref, &logMain);
    prefs->GetPrefBoolean(kLogDecodePref, &logDecoder);
    prefs->GetPrefBoolean(kLogInputPref, &logInput);
    prefs->GetPrefBoolean(kLogOutputPref, &logOutput);
    prefs->GetPrefBoolean(kLogPerformancePref, &logPerformance);
    prefs->GetPrefBoolean(kCheckForUpdatesPref, &checkForUpdates);
    prefs->GetPrefBoolean(kAskToReclaimFiletypesPref, &askReclaimFiletypes);
    prefs->GetPrefBoolean(kReclaimFiletypesPref, &reclaimFiletypes);
    prefs->GetPrefBoolean(kShowToolbarTextLabelsPref, &useTextLabels);
    prefs->GetPrefBoolean(kShowToolbarImagesPref, &useImages);
    prefs->GetPrefBoolean(kSaveCurrentPlaylistOnExitPref, &savePlaylistOnExit);
    prefs->GetPrefBoolean(kPlayImmediatelyPref, &playImmediately);
//    prefs->GetPrefBoolean(kConvertUnderscoresToSpacesPref, &convertUnderscores);

    free(buffer);
}

void
Prefs::SavePrefsValues( Preferences* prefs, ThemeManager* themeMan )
{
    prefs->SetPrefString(kPMOPref, defaultPMO.c_str());
    prefs->SetPrefString(kUIPref, defaultUI.c_str());
    prefs->SetPrefInt32(kDecoderThreadPriorityPref, decoderThreadPriority);
    prefs->SetPrefInt32(kInputBufferSizePref, inputBufferSize);
    prefs->SetPrefInt32(kOutputBufferSizePref, outputBufferSize);
    prefs->SetPrefInt32(kPreBufferPref, preBufferLength);
    prefs->SetPrefBoolean(kStayOnTopPref, stayOnTop);
    prefs->SetPrefBoolean(kLiveInTrayPref, liveInTray);

    prefs->SetPrefBoolean(kSaveStreamsPref, saveStreams);
    prefs->SetPrefString(kSaveStreamsDirPref, saveStreamsDirectory.c_str());
    prefs->SetPrefString(kProxyHostPref, proxyServer.c_str());
    prefs->SetPrefBoolean(kUseProxyPref, useProxyServer);
    prefs->SetPrefString(kAlternateNICAddressPref, alternateIP.c_str());
    prefs->SetPrefBoolean(kUseAlternateNICPref, useAlternateIP);

    prefs->SetPrefBoolean(kUseDebugLogPref, enableLogging);
    prefs->SetPrefBoolean(kLogMainPref, logMain);
    prefs->SetPrefBoolean(kLogDecodePref, logDecoder);
    prefs->SetPrefBoolean(kLogInputPref, logInput);
    prefs->SetPrefBoolean(kLogOutputPref, logOutput);
    prefs->SetPrefBoolean(kLogPerformancePref, logPerformance);

    prefs->SetPrefString(kThemeDefaultFontPref, defaultFont.c_str());
    map<string, string> themeList;
    themeManager->GetThemeList( themeList );
    if ( themeList[currentTheme].length() > 0 )
       themeManager->UseTheme( themeList[currentTheme] );

    prefs->SetPrefBoolean(kCheckForUpdatesPref, checkForUpdates);
    prefs->SetPrefString(kSaveMusicDirPref, saveMusicDirectory.c_str());
    prefs->SetPrefBoolean(kAskToReclaimFiletypesPref, askReclaimFiletypes);
    prefs->SetPrefBoolean(kReclaimFiletypesPref, reclaimFiletypes);

    PortableSet::const_iterator i;
    string portableList;

    for(i = portablePlayers.begin(); i != portablePlayers.end(); i++)
    {
        portableList += *i;
        portableList += ";";
    }

    prefs->SetPrefString(kUsersPortablePlayersPref, portableList.c_str());
    prefs->SetPrefBoolean(kShowToolbarTextLabelsPref, useTextLabels);
    prefs->SetPrefBoolean(kShowToolbarImagesPref, useImages);

    prefs->SetPrefBoolean(kSaveCurrentPlaylistOnExitPref, savePlaylistOnExit);
    prefs->SetPrefBoolean(kPlayImmediatelyPref, playImmediately);

#if 0
    // this gets called by each page unfortunately
    // so save some effort by only doing it once
    if(*values != m_currentValues) 
    {
        PRINT(( "PrefsChanged:\n" ));
        m_context->target->AcceptEvent(new Event(INFO_PrefsChanged));
        m_currentValues = m_proposedValues = *values;
    }
#endif
}

// vi: set ts=4:

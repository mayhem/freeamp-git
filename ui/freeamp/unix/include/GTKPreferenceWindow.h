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

#ifndef INCLUDED_GTKPREFERENCEWINDOW_H__
#define INCLUDED_GTKPREFERENCEWINDOW_H__

#include <map>
#include <set>
using namespace std;

#include <gtk/gtk.h>

#include "config.h"
#include "PreferenceWindow.h"
#include "ThemeManager.h"
#include "preferences.h"
#include "log.h"
#include "registrar.h"

typedef set<string> PortableSet;

typedef struct PrefsStruct 
{
    Preferences* prefs;

    // page 1
    string defaultPMO;
    int32 outputIndex;
    int32 inputBufferSize;
    int32 outputBufferSize;
    int32 preBufferLength;

    // page 2
    int32 streamInterval;
    bool saveStreams;
    string saveStreamsDirectory;
    bool useProxyServer;
    string proxyServer; // is there a domain name length limit???
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
    int    listboxIndex;
   
    string saveMusicDirectory;
    PortableSet portablePlayers;

    bool operator == (const struct PrefsStruct& pref)
    {
        return (
            defaultPMO == pref.defaultPMO &&
            inputBufferSize == pref.inputBufferSize &&
            outputBufferSize == pref.outputBufferSize &&
            preBufferLength == pref.preBufferLength &&
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
            saveMusicDirectory == pref.saveMusicDirectory &&
            portablePlayers == pref.portablePlayers &&
            listboxIndex == pref.listboxIndex &&
            outputIndex == pref.outputIndex && 
            true
        );
    }

    bool operator != (const struct PrefsStruct& pref)
    {
        return ! (*this == pref);
    }
} PrefsStruct;

class GTKPreferenceWindow : public PreferenceWindow
{
  public:

               GTKPreferenceWindow(FAContext *context,
                                   ThemeManager *pThemeMan,
                                   uint32 defaultPage,
                                   bool inEventLoop);
      virtual ~GTKPreferenceWindow(void); 
      
      virtual  bool Show(Window *pParent);

               void GetPrefsValues(Preferences* prefs, 
                                   PrefsStruct* values);

               void SavePrefsValues(Preferences* prefs, 
                                    PrefsStruct* values);
  protected:
    
      PrefsStruct  currentValues;
      PrefsStruct  originalValues;
      PrefsStruct  proposedValues;

      map<string, string> m_oThemeList;

  private:
      bool   eventLoop;
      uint32 startPage;

      GtkWidget *CreatePage1(void);
      GtkWidget *CreatePage2(void);
      GtkWidget *CreatePage3(void);
      GtkWidget *CreateAbout(void);
      GtkWidget *CreatePage5(void);
      GtkWidget *CreatePage6(void);

      GtkWidget *applyButton;

      GtkWidget *saveStreamLabel;
      GtkWidget *saveStreamBox;
      GtkWidget *saveBrowseBox;

      GtkWidget *proxyAddyLabel;
      GtkWidget *proxyAddyBox;
      GtkWidget *proxyColon;
      GtkWidget *proxyPortLabel;
      GtkWidget *proxyPortBox;

      GtkWidget *ipLabel;
      GtkWidget *ipOneBox;
      GtkWidget *ipPeriod1;
      GtkWidget *ipTwoBox;
      GtkWidget *ipPeriod2;
      GtkWidget *ipThreeBox;
      GtkWidget *ipPeriod3;
      GtkWidget *ipFourBox;

      GtkWidget *pmoMenu;

      GtkWidget *themeList;

      GtkWidget *fontDialog;

  public:
      GtkWidget *mainWindow;
      GtkWidget *pmoOptionMenu;

      int32 numPMOs;
      bool  done;

      void UpdateThemeList(void);
      void AddThemeEvent(const char *newpath);
      void DeleteThemeEvent(void);
      void ChooseFont(void);
      void SetFont(void);

      void ApplyInfo(void);
      void CancelInfo(void);
      
      void SetInputBufferSize(int newvalue);
      void SetOutputBufferSize(int newvalue);
      void SetPreBufferLength(int newvalue);

      void SetStreamInterval(int newvalue);
      void SaveLocalToggle(int active);
      void SaveLocalSet(char *newpath);
      void ProxyAddySet(void);
      void ProxyToggle(int active);
      void AltIPSet(void);
      void AltIPToggle(int active);

      void SelectTheme(int number);

      void SetPMO(int newsel);

      void ShowHelp(void);
};

#endif

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
#include <gtk/gtk.h>

using namespace std;

#include "config.h"
#include "PreferenceWindow.h"
#include "ThemeManager.h"
#include "preferences.h"
#include "log.h"
#include "registrar.h"

typedef struct PrefsStruct 
{
    Preferences* prefs;

    // page 1
    char defaultUI[256];
    char defaultPMO[256];
    int32 inputBufferSize;
    int32 outputBufferSize;
    int32 preBufferLength;

    // page 2
    int32 streamInterval;
    bool saveStreams;
    char saveStreamsDirectory[_MAX_PATH + 1];
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
    char   defaultFont[256];
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
        memset(defaultFont, 0x00, sizeof(defaultFont));
        currentTheme = "";
    }

} PrefsStruct;

class GTKPreferenceWindow : public PreferenceWindow
{
  public:

               GTKPreferenceWindow(FAContext *context,
                                   ThemeManager *pThemeMan);
      virtual ~GTKPreferenceWindow(void); 
      
      virtual  bool Show(Window *pParent);

               void GetPrefsValues(Preferences* prefs, 
                                   PrefsStruct* values);

               void SavePrefsValues(Preferences* prefs, 
                                    PrefsStruct* values);

  protected:
    
      PrefsStruct  currentValues;

      map<string, string> m_oThemeList;

  private:
      GtkWidget *CreatePage1(void);
      GtkWidget *CreatePage2(void);
      GtkWidget *CreatePage3(void);
      GtkWidget *CreateAbout(void);
      GtkWidget *CreatePage5(void);

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

      GtkWidget *logGeneral;
      GtkWidget *logPMI;
      GtkWidget *logPMO;
      GtkWidget *logLMC;
      GtkWidget *logPerf;

  public:
      GtkWidget *mainWindow;

      void ApplyInfo(void);
      
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

      void LogToggle(int active);
      void GeneralToggle(int active);
      void PMIToggle(int active);
      void PMOToggle(int active);
      void LMCToggle(int active);
      void PerfToggle(int active);

      void SelectTheme(int number);
};

#endif

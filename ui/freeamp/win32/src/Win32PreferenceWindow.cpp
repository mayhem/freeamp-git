/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1999 EMusic.com

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

/* system headers */
#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <shlobj.h>
#include <commctrl.h>
#include <stdlib.h>
#include <assert.h>

// The debugger can't handle symbols more than 255 characters long.
// STL often creates symbols longer than that.
// When symbols are longer than 255 characters, the warning is disabled.
#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <sstream>
#include <set>

using namespace std;

#include "eventdata.h"
#include "thread.h"
#include "win32updatemanager.h"
#include "Win32PreferenceWindow.h"
#include "Win32Window.h"
#include "help.h"
#include "Debug.h"

#define DB Debug_v("%s:%d\n", __FILE__, __LINE__);

static Win32PreferenceWindow *g_pCurrentPrefWindow = NULL;
const char* kThemeFileFilter =
            BRANDING" Themes (.fat)\0"
            "*.fat\0"
            "All Files (*.*)\0"
            "*.*\0";

uint32 CalcStringEllipsis(HDC hdc, string& displayString, int32 columnWidth);

static BOOL CALLBACK 
PrefGeneralCallback(HWND hwnd, 
                    UINT msg, 
                    WPARAM wParam, 
                    LPARAM lParam)
{
	return g_pCurrentPrefWindow->PrefGeneralProc(hwnd, msg, wParam, lParam);
}          

static BOOL CALLBACK 
PrefThemeCallback(HWND hwnd, 
                  UINT msg, 
                  WPARAM wParam, 
                  LPARAM lParam)
{
	return g_pCurrentPrefWindow->PrefThemeProc(hwnd, msg, wParam, lParam);
}          

static BOOL CALLBACK 
PrefStreamingCallback(HWND hwnd, 
                      UINT msg, 
                      WPARAM wParam, 
                      LPARAM lParam)
{
	return g_pCurrentPrefWindow->PrefStreamingProc(hwnd, msg, wParam, lParam);
}          

static BOOL CALLBACK 
PrefPluginsCallback(HWND hwnd, 
                    UINT msg, 
                    WPARAM wParam, 
                    LPARAM lParam)
{
	return g_pCurrentPrefWindow->PrefPluginsProc(hwnd, msg, wParam, lParam);
}          

static BOOL CALLBACK 
PrefUpdateCallback(HWND hwnd, 
                   UINT msg, 
                   WPARAM wParam, 
                   LPARAM lParam)
{
	return g_pCurrentPrefWindow->PrefUpdateProc(hwnd, msg, wParam, lParam);
}      

static BOOL CALLBACK 
PrefAdvancedCallback(HWND hwnd, 
                     UINT msg, 
                     WPARAM wParam, 
                     LPARAM lParam)
{
	return g_pCurrentPrefWindow->PrefAdvancedProc(hwnd, msg, wParam, lParam);
}    

static BOOL CALLBACK 
PrefAboutCallback(HWND hwnd, 
                  UINT msg, 
                  WPARAM wParam, 
                  LPARAM lParam)
{
	return g_pCurrentPrefWindow->PrefAboutProc(hwnd, msg, wParam, lParam);
}      

static BOOL CALLBACK 
PrefDebugCallback(HWND hwnd, 
                  UINT msg, 
                  WPARAM wParam, 
                  LPARAM lParam)
{
	return g_pCurrentPrefWindow->PrefDebugProc(hwnd, msg, wParam, lParam);
}            

Win32PreferenceWindow::Win32PreferenceWindow(FAContext *context,
                                             ThemeManager *pThemeMan,
                                             uint32 defaultPage) :
     PreferenceWindow(context, pThemeMan)
{     
	g_pCurrentPrefWindow = this;
    m_defaultPage = defaultPage;
}

Win32PreferenceWindow::~Win32PreferenceWindow(void)
{
	g_pCurrentPrefWindow = NULL;
} 

bool Win32PreferenceWindow::Show(Window *pWindow)
{
	HWND hWnd;
    
    hWnd = ((Win32Window *)pWindow)->GetWindowHandle(); 
    return DisplayPreferences(hWnd, m_pContext->prefs);
}

bool Win32PreferenceWindow::DisplayPreferences(HWND hwndParent, Preferences* prefs)
{
    bool result = false;
    PROPSHEETPAGE psp[7];
    PROPSHEETHEADER psh;
	
    HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwndParent, GWL_HINSTANCE);

    psp[0].dwSize = sizeof(PROPSHEETPAGE);
    psp[0].dwFlags = PSP_HASHELP;
    psp[0].hInstance = hinst;
    psp[0].pszTemplate = MAKEINTRESOURCE(IDD_PREF_GENERAL);
    psp[0].pszIcon = NULL;
    psp[0].pfnDlgProc = PrefGeneralCallback;
    psp[0].pszTitle = NULL;
    psp[0].lParam = (LPARAM)prefs;

    psp[1].dwSize = sizeof(PROPSHEETPAGE);
    psp[1].dwFlags = PSP_HASHELP;
    psp[1].hInstance = hinst;
    psp[1].pszTemplate = MAKEINTRESOURCE(IDD_PREF_THEME);
    psp[1].pszIcon = NULL;
    psp[1].pfnDlgProc = PrefThemeCallback;
    psp[1].pszTitle = NULL;
    psp[1].lParam = (LPARAM)prefs;

    psp[2].dwSize = sizeof(PROPSHEETPAGE);
    psp[2].dwFlags = PSP_HASHELP;
    psp[2].hInstance = hinst;
    psp[2].pszTemplate = MAKEINTRESOURCE(IDD_PREF_STREAMING);
    psp[2].pszIcon = NULL;
    psp[2].pfnDlgProc = PrefStreamingCallback;
    psp[2].pszTitle = NULL;
    psp[2].lParam = (LPARAM)prefs;

    psp[3].dwSize = sizeof(PROPSHEETPAGE);
    psp[3].dwFlags = PSP_HASHELP;
    psp[3].hInstance = hinst;
    psp[3].pszTemplate = MAKEINTRESOURCE(IDD_PREF_PLUGINS);
    psp[3].pszIcon = NULL;
    psp[3].pfnDlgProc = PrefPluginsCallback;
    psp[3].pszTitle = NULL;
    psp[3].lParam = (LPARAM)prefs;

    UpdateManager* updateManager = NULL;

    updateManager = new Win32UpdateManager(m_pContext);
    updateManager->DetermineLocalVersions();
    updateManager->SetPlatform(string("WIN32"));

    psp[4].dwSize = sizeof(PROPSHEETPAGE);
    psp[4].dwFlags = PSP_HASHELP;
    psp[4].hInstance = hinst;
    psp[4].pszTemplate = MAKEINTRESOURCE(IDD_PREF_UPDATE);
    psp[4].pszIcon = NULL;
    psp[4].pfnDlgProc = PrefUpdateCallback;
    psp[4].pszTitle = NULL;
    psp[4].lParam = (LPARAM)updateManager;

    psp[5].dwSize = sizeof(PROPSHEETPAGE);
    psp[5].dwFlags = PSP_HASHELP;
    psp[5].hInstance = hinst;
    psp[5].pszTemplate = MAKEINTRESOURCE(IDD_PREF_ADVANCED);
    psp[5].pszIcon = NULL;
    psp[5].pfnDlgProc = PrefAdvancedCallback;
    psp[5].pszTitle = NULL;
    psp[5].lParam = (LPARAM)prefs;

    psp[6].dwSize = sizeof(PROPSHEETPAGE);
    psp[6].dwFlags = PSP_HASHELP;
    psp[6].hInstance = hinst;
    psp[6].pszTemplate = MAKEINTRESOURCE(IDD_PREF_ABOUT);
    psp[6].pszIcon = NULL;
    psp[6].pfnDlgProc = PrefAboutCallback;
    psp[6].pszTitle = NULL;
    psp[6].lParam = (LPARAM)prefs;

   
    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_PROPSHEETPAGE | PSH_HASHELP;
    psh.hwndParent = hwndParent;
    psh.hInstance = hinst;
    psh.pszIcon = NULL;
    psh.pszCaption = BRANDING" Preferences";
    psh.nPages = sizeof(psp)/sizeof(PROPSHEETPAGE);
    psh.nStartPage = m_defaultPage;
    psh.ppsp = psp;
    psh.pfnCallback = NULL;

    GetPrefsValues(prefs, &m_originalValues);

    m_proposedValues = m_currentValues = m_originalValues;
    
    result = (PropertySheet(&psh) > 0);

    delete updateManager;

    return result;
}

void Win32PreferenceWindow::GetPrefsValues(Preferences* prefs, 
                                           PrefsStruct* values)
{
    uint32 bufferSize = 1;
    uint32 size;

    // get the string prefs
    char* buffer = (char*)malloc(bufferSize);

    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetDefaultPMO(buffer, &size))
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetDefaultPMO(buffer, &size);
    }

    values->defaultPMO = buffer;
    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetDefaultUI(buffer, &size))
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetDefaultUI(buffer, &size);
    }
    
    values->defaultUI = buffer;
    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetProxyServerAddress(buffer, &size))
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetProxyServerAddress(buffer, &size);
    }

    values->proxyServer = buffer;
    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetSaveStreamsDirectory(buffer, &size))
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetSaveStreamsDirectory(buffer, &size);
    }

    values->saveStreamsDirectory = buffer;
    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetAlternateNICAddress(buffer, &size))
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetAlternateNICAddress(buffer, &size);
    }

    values->alternateIP = buffer;
    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetThemeDefaultFont(buffer, &size))
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetThemeDefaultFont(buffer, &size);
    }

    values->defaultFont = buffer;
    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetSaveMusicDirectory(buffer, &size))
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetSaveMusicDirectory(buffer, &size);
    }

    values->saveMusicDirectory = buffer;
    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetUsersPortablePlayers(buffer, &size))
    {
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetUsersPortablePlayers(buffer, &size);
    }

    char* cp = buffer;
    char* name = cp;

    while(cp = strchr(cp, ';'))
    {
        *cp = 0x00;
        values->portablePlayers.insert(string(name));
        //MessageBox(NULL, name, "name", MB_OK);

        cp++;
        name = cp;
    }

    if(*name)
    {
        values->portablePlayers.insert(string(name));
        //MessageBox(NULL, name, "name", MB_OK);
    }

    //values->portablePlayers = buffer;
    size = bufferSize;

    m_pThemeMan->GetCurrentTheme(values->currentTheme);

    // get the other prefs
    prefs->GetDecoderThreadPriority(&values->decoderThreadPriority);
    prefs->GetInputBufferSize(&values->inputBufferSize);
    prefs->GetOutputBufferSize(&values->outputBufferSize);
    prefs->GetPrebufferLength(&values->preBufferLength);
    prefs->GetStayOnTop(&values->stayOnTop);
    prefs->GetLiveInTray(&values->liveInTray);
    prefs->GetStreamBufferInterval(&values->streamInterval);
    prefs->GetSaveStreams(&values->saveStreams);
    prefs->GetUseProxyServer(&values->useProxyServer);
    prefs->GetUseAlternateNIC(&values->useAlternateIP);
    prefs->GetUseDebugLog(&values->enableLogging);
    prefs->GetLogMain(&values->logMain);
    prefs->GetLogDecode(&values->logDecoder);
    prefs->GetLogInput(&values->logInput);
    prefs->GetLogOutput(&values->logOutput);
    prefs->GetLogPerformance(&values->logPerformance);
    prefs->GetCheckForUpdates(&values->checkForUpdates);
    prefs->GetAskToReclaimFiletypes(&values->askReclaimFiletypes);
    prefs->GetReclaimFiletypes(&values->reclaimFiletypes);

    free(buffer);
}

void Win32PreferenceWindow::SavePrefsValues(Preferences* prefs, 
                                            PrefsStruct* values)
{
    prefs->SetDefaultPMO(values->defaultPMO.c_str());
    prefs->SetDefaultUI(values->defaultUI.c_str());
    prefs->SetDecoderThreadPriority(values->decoderThreadPriority);
    prefs->SetInputBufferSize(values->inputBufferSize);
    prefs->SetOutputBufferSize(values->outputBufferSize);
    prefs->SetPrebufferLength(values->preBufferLength);
    prefs->SetStayOnTop(values->stayOnTop);
    prefs->SetLiveInTray(values->liveInTray);

    prefs->SetStreamBufferInterval(values->streamInterval);
    prefs->SetSaveStreams(values->saveStreams);
    prefs->SetSaveStreamsDirectory(values->saveStreamsDirectory.c_str());
    prefs->SetProxyServerAddress(values->proxyServer.c_str());
    prefs->SetUseProxyServer(values->useProxyServer);
    prefs->SetAlternateNICAddress(values->alternateIP.c_str());
    prefs->SetUseAlternateNIC(values->useAlternateIP);

    prefs->SetUseDebugLog(values->enableLogging);
    prefs->SetLogMain(values->logMain);
    prefs->SetLogDecode(values->logDecoder);
    prefs->SetLogInput(values->logInput);
    prefs->SetLogOutput(values->logOutput);
    prefs->SetLogPerformance(values->logPerformance);

    prefs->SetThemeDefaultFont(values->defaultFont.c_str());
    m_pThemeMan->UseTheme(m_oThemeList[values->currentTheme]);

    prefs->SetCheckForUpdates(values->checkForUpdates);
    prefs->SetSaveMusicDirectory(values->saveMusicDirectory.c_str());
    prefs->SetAskToReclaimFiletypes(values->askReclaimFiletypes);
    prefs->SetReclaimFiletypes(values->reclaimFiletypes);

    PortableSet::const_iterator i;
    string portableList;

    for(i = values->portablePlayers.begin(); i != values->portablePlayers.end(); i++)
    {
        portableList += *i;
        portableList += ";";
    }

    prefs->SetUsersPortablePlayers(portableList.c_str());

    // this gets called by each page unfortunately
    // so save some effort by only doing it once
    if(*values != m_currentValues) 
    {
        m_pContext->target->AcceptEvent(new Event(INFO_PrefsChanged));
        m_currentValues = m_proposedValues = *values;
    }
}

void Win32PreferenceWindow::LaunchHelp(HWND hwnd, uint32 topic)
{
    string            oHelpFile;
    char              dir[MAX_PATH];
    uint32            len = sizeof(dir);

    m_pContext->prefs->GetInstallDirectory(dir, &len);
    oHelpFile = string(dir);
    oHelpFile += string("\\");    
    oHelpFile += string(HELP_FILE);    

    WinHelp(hwnd, oHelpFile.c_str(), HELP_CONTEXT, topic);
}                

bool Win32PreferenceWindow::PrefGeneralProc(HWND hwnd, 
                                            UINT msg, 
                                            WPARAM wParam, 
                                            LPARAM lParam)      
{
    bool result = false;
    static PROPSHEETPAGE* psp = NULL;
    static Preferences* prefs = NULL;
    static HWND hwndAskReclaimFiletypes = NULL;
    static HWND hwndReclaimFiletypes = NULL;
    static HWND hwndSaveMusicDir = NULL;
    static HWND hwndStayOnTop = NULL;
    static HWND hwndLiveInTray = NULL;
    

    switch(msg)
    {
        case WM_INITDIALOG:
        {
            // remember these for later...
            psp = (PROPSHEETPAGE*)lParam;
            prefs = (Preferences*)psp->lParam;

            // get the handles to all our controls
            
            hwndStayOnTop = GetDlgItem(hwnd, IDC_STAYONTOP);
            hwndLiveInTray = GetDlgItem(hwnd, IDC_TRAY);
            hwndAskReclaimFiletypes = GetDlgItem(hwnd, IDC_ASKRECLAIM);
            hwndReclaimFiletypes = GetDlgItem(hwnd, IDC_RECLAIMFILETYPES);
            hwndSaveMusicDir = GetDlgItem(hwnd, IDC_SAVEMUSICDIR);


            Button_SetCheck(hwndStayOnTop, m_originalValues.stayOnTop);
            Button_SetCheck(hwndLiveInTray, m_originalValues.liveInTray);

            Button_SetCheck(hwndReclaimFiletypes, m_originalValues.reclaimFiletypes);
            Button_SetCheck(hwndAskReclaimFiletypes, m_originalValues.askReclaimFiletypes);
           

            Edit_SetText(hwndSaveMusicDir, 
                         m_originalValues.saveMusicDirectory.c_str());
            
            break;
        }

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDC_STAYONTOP:
                {
                    if(Button_GetCheck(hwndStayOnTop) == BST_CHECKED)
                    {
                        m_proposedValues.stayOnTop = true;
                    }
                    else
                    {
                        m_proposedValues.stayOnTop = false;
                    }

                    if(m_proposedValues != m_currentValues)
                    {
                        PropSheet_Changed(GetParent(hwnd), hwnd);
                    }
                    else
                    {
                        PropSheet_UnChanged(GetParent(hwnd), hwnd);
                    }

                    break;
                }

                case IDC_TRAY:
                {
                    if(Button_GetCheck(hwndLiveInTray) == BST_CHECKED)
                    {
                        m_proposedValues.liveInTray = true;
                    }
                    else
                    {
                        m_proposedValues.liveInTray = false;
                    }

                    if(m_proposedValues != m_currentValues)
                    {
                        PropSheet_Changed(GetParent(hwnd), hwnd);
                    }
                    else
                    {
                        PropSheet_UnChanged(GetParent(hwnd), hwnd);
                    }

                    break;
                }

                case IDC_RECLAIMFILETYPES:
                {
                    if(Button_GetCheck(hwndReclaimFiletypes) == BST_CHECKED)
                    {
                        m_proposedValues.reclaimFiletypes = true;
                    }
                    else
                    {
                        m_proposedValues.reclaimFiletypes = false;
                    }

                    if(m_proposedValues != m_currentValues)
                    {
                        PropSheet_Changed(GetParent(hwnd), hwnd);
                    }
                    else
                    {
                        PropSheet_UnChanged(GetParent(hwnd), hwnd);
                    }

                    break;
                }

                case IDC_ASKRECLAIM:
                {
                    if(Button_GetCheck(hwndAskReclaimFiletypes) == BST_CHECKED)
                    {
                        m_proposedValues.askReclaimFiletypes = true;
                    }
                    else
                    {
                        m_proposedValues.askReclaimFiletypes = false;
                    }

                    if(m_proposedValues != m_currentValues)
                    {
                        PropSheet_Changed(GetParent(hwnd), hwnd);
                    }
                    else
                    {
                        PropSheet_UnChanged(GetParent(hwnd), hwnd);
                    }

                    break;
                }

                case IDC_SAVEMUSICDIR:
                {
                    if(HIWORD(wParam) == EN_CHANGE)
                    {
                        char temp[MAX_PATH];

                        Edit_GetText(   hwndSaveMusicDir, 
                                        temp,
                                        MAX_PATH);

                        m_proposedValues.saveMusicDirectory = temp;

                        if(m_proposedValues != m_currentValues)
                        {
                            PropSheet_Changed(GetParent(hwnd), hwnd);
                        }
                        else
                        {
                            PropSheet_UnChanged(GetParent(hwnd), hwnd);
                        }
                    }

                    break;
                }

                case IDC_BROWSE:
                {
                    LPMALLOC pMalloc;

                    if(SUCCEEDED(SHGetMalloc(&pMalloc)))
                    {
                        BROWSEINFO bi; 
                        LPITEMIDLIST browseId;
                        char displayName[MAX_PATH + 1];

                        bi.hwndOwner = hwnd;
                        bi.pidlRoot = NULL;
                        bi.pszDisplayName = displayName;
                        bi.lpszTitle = "Please select the folder to which you want to save music.";
                        bi.ulFlags = BIF_RETURNONLYFSDIRS;
                        bi.lpfn = NULL;

                        browseId = SHBrowseForFolder(&bi);
            
                        if(browseId)
                        {
                            char temp[MAX_PATH];

                            SHGetPathFromIDList(browseId, temp);
                            
                            m_proposedValues.saveMusicDirectory = temp;

                            Edit_SetText(hwndSaveMusicDir, temp);

                            pMalloc->Free(browseId);
                        }
                    }
                
                    break;
                } 
            }

            break;
        }

        case WM_NOTIFY:
        {
            NMHDR* notify = (NMHDR*)lParam;

            switch(notify->code)
            {
                case PSN_HELP:
                {
                    LaunchHelp(hwnd, Preferences_General);
                    break;
                }
            
                case PSN_SETACTIVE:
                {
                    break;
                }

                case PSN_APPLY:
                {
                    SavePrefsValues(prefs, &m_proposedValues);
                    break;
                }

                case PSN_KILLACTIVE:
                {
                    break;
                }

                case PSN_RESET:
                {
                    SavePrefsValues(prefs, &m_originalValues);
                    break;
                }
            }

            break;
        }
    }

    return result;
}

bool Win32PreferenceWindow::PrefStreamingProc(HWND hwnd, 
                                              UINT msg, 
                                              WPARAM wParam, 
                                              LPARAM lParam)      
{
    bool result = false;
    static PROPSHEETPAGE* psp = NULL;
    static Preferences* prefs = NULL;
    static HWND hwndStreamInterval = NULL;
    static HWND hwndSaveStreams = NULL;
    static HWND hwndSaveStreamsDirectory = NULL;
    static HWND hwndBrowse = NULL;
    static HWND hwndSaveLocationText = NULL;
    static HWND hwndUseProxyServer = NULL;
    static HWND hwndProxyServerAddress = NULL;
    static HWND hwndProxyServerPort = NULL;
    static HWND hwndProxyServerAddressText = NULL;
    static HWND hwndProxyServerPortText = NULL;
    static HWND hwndColon = NULL;

    static HWND hwndUseAlternateIP = NULL;
    static HWND hwndUseAlternateIPText = NULL;
    static HWND hwndAlternateIPAddress1 = NULL;
    static HWND hwndAlternateIPAddress2 = NULL;
    static HWND hwndAlternateIPAddress3 = NULL;
    static HWND hwndAlternateIPAddress4 = NULL;
    static HWND hwndPeriod1 = NULL;
    static HWND hwndPeriod2 = NULL;
    static HWND hwndPeriod3 = NULL;
   
    switch(msg)
    {
        case WM_INITDIALOG:
        {
            // remember these for later...
            psp = (PROPSHEETPAGE*)lParam;
            prefs = (Preferences*)psp->lParam;

            // get the handles to all our controls
            hwndStreamInterval = GetDlgItem(hwnd, IDC_STREAM_INTERVAL);

            hwndSaveStreams = GetDlgItem(hwnd, IDC_SAVESTREAMS);
            hwndSaveStreamsDirectory = GetDlgItem(hwnd, IDC_STREAMSAVEDIR);
            hwndBrowse = GetDlgItem(hwnd, IDC_BROWSE);
            hwndSaveLocationText = GetDlgItem(hwnd, IDC_SAVELOCATION_TEXT);

            hwndUseProxyServer = GetDlgItem(hwnd, IDC_USEPROXY);
            hwndProxyServerAddress = GetDlgItem(hwnd, IDC_PROXYADDRESS);
            hwndProxyServerAddressText = GetDlgItem(hwnd, IDC_PROXYADDRESS_TEXT);
            hwndProxyServerPort = GetDlgItem(hwnd, IDC_PORT);
            hwndProxyServerPortText = GetDlgItem(hwnd, IDC_PORT_TEXT);
            hwndColon = GetDlgItem(hwnd, IDC_COLON_TEXT);

            hwndUseAlternateIP = GetDlgItem(hwnd, IDC_USETHISIP);
            hwndUseAlternateIPText = GetDlgItem(hwnd, IDC_IPADDRESS_TEXT);
            hwndAlternateIPAddress1 = GetDlgItem(hwnd, IDC_IPADDRESS1);
            hwndAlternateIPAddress2 = GetDlgItem(hwnd, IDC_IPADDRESS2);
            hwndAlternateIPAddress3 = GetDlgItem(hwnd, IDC_IPADDRESS3);
            hwndAlternateIPAddress4 = GetDlgItem(hwnd, IDC_IPADDRESS4);
            hwndPeriod1 = GetDlgItem(hwnd, IDC_PERIOD1);
            hwndPeriod2 = GetDlgItem(hwnd, IDC_PERIOD2);
            hwndPeriod3 = GetDlgItem(hwnd, IDC_PERIOD3);
   

            // initialize our controls
            int32 value;
            char temp[256];

            value = m_originalValues.streamInterval;
            sprintf(temp, "%d", value);
            Edit_LimitText(hwndStreamInterval, 2);
            Edit_SetText(hwndStreamInterval, temp);
            
            Button_SetCheck(hwndSaveStreams, m_originalValues.saveStreams);


            Edit_SetText(   hwndSaveStreamsDirectory, 
                            m_originalValues.saveStreamsDirectory.c_str());

            Button_Enable(  hwndSaveStreamsDirectory, 
                            m_originalValues.saveStreams);

            Button_Enable(  hwndBrowse, 
                            m_originalValues.saveStreams);

            Button_Enable(  hwndSaveLocationText,
                            m_originalValues.saveStreams);

            char* port = NULL;

            strcpy(temp, m_originalValues.proxyServer.c_str());
            port = strrchr(temp, ':');

            if(port)
            {
                *port = 0x00;
                port++;

                Edit_SetText(hwndProxyServerPort, 
                             port);

            }

            Edit_SetText(   hwndProxyServerAddress, 
                            temp);

            Edit_LimitText(hwndProxyServerPort, 5);


            Button_SetCheck(hwndUseProxyServer, m_originalValues.useProxyServer);


            Button_Enable(  hwndProxyServerAddress, 
                            m_originalValues.useProxyServer);

            Button_Enable(  hwndProxyServerAddressText, 
                            m_originalValues.useProxyServer);

            Button_Enable(  hwndProxyServerPort,
                            m_originalValues.useProxyServer);

            Button_Enable(  hwndProxyServerPortText, 
                            m_originalValues.useProxyServer);

            Button_Enable(  hwndColon, 
                            m_originalValues.useProxyServer);

            char* dot = NULL;
            char* ip[4];
            int32 i = 1;

            strcpy(temp, m_originalValues.alternateIP.c_str());
            ip[0] = temp;
            dot = temp;

            while(dot = strchr(dot, '.'))
            {
                *dot = 0x00;
                ip[i++] = ++dot;
            }

            Edit_SetText(hwndAlternateIPAddress1, ip[0]);
            Edit_SetText(hwndAlternateIPAddress2, ip[1]);
            Edit_SetText(hwndAlternateIPAddress3, ip[2]);
            Edit_SetText(hwndAlternateIPAddress4, ip[3]);

            Edit_LimitText(hwndAlternateIPAddress1, 3);
            Edit_LimitText(hwndAlternateIPAddress2, 3);
            Edit_LimitText(hwndAlternateIPAddress3, 3);
            Edit_LimitText(hwndAlternateIPAddress4, 3);

            Button_SetCheck(hwndUseAlternateIP, m_originalValues.useAlternateIP);


            Button_Enable(  hwndUseAlternateIPText, 
                            m_originalValues.useAlternateIP);

            Button_Enable(  hwndAlternateIPAddress1,
                            m_originalValues.useAlternateIP);

            Button_Enable(  hwndPeriod1, 
                            m_originalValues.useAlternateIP);

            Button_Enable(  hwndAlternateIPAddress2, 
                            m_originalValues.useAlternateIP);

            Button_Enable(  hwndPeriod2, 
                            m_originalValues.useAlternateIP);

            Button_Enable(  hwndAlternateIPAddress3, 
                            m_originalValues.useAlternateIP);

            Button_Enable(  hwndPeriod3, 
                            m_originalValues.useAlternateIP);

            Button_Enable(  hwndAlternateIPAddress4, 
                            m_originalValues.useAlternateIP);

            
            break;
        }

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDC_STREAM_INTERVAL:
                {
                    if(HIWORD(wParam) == EN_CHANGE)
                    {
                        char text[128];

                        Edit_GetText(hwndStreamInterval, text, sizeof(text));

                        m_proposedValues.streamInterval = atoi(text);

                        if(m_proposedValues != m_currentValues)
                        {
                            PropSheet_Changed(GetParent(hwnd), hwnd);
                        }
                        else
                        {
                            PropSheet_UnChanged(GetParent(hwnd), hwnd);
                        }
                    }

                    break;
                }

                case IDC_SAVESTREAMS:
                {
                    BOOL enabled;

                    if(Button_GetCheck(hwndSaveStreams) == BST_CHECKED)
                    {
                        m_proposedValues.saveStreams = true;
                    }
                    else
                    {
                        m_proposedValues.saveStreams = false;
                    }

                    enabled = (m_proposedValues.saveStreams ? TRUE : FALSE);

                    Button_Enable(hwndSaveStreamsDirectory, enabled); 
                    Button_Enable(hwndBrowse, enabled);
                    Button_Enable(  hwndSaveLocationText,enabled);


                    if(m_proposedValues != m_currentValues)
                    {
                        PropSheet_Changed(GetParent(hwnd), hwnd);
                    }
                    else
                    {
                        PropSheet_UnChanged(GetParent(hwnd), hwnd);
                    }

                    break;
                }

                case IDC_STREAMSAVEDIR:
                {
                    if(HIWORD(wParam) == EN_CHANGE)
                    {
                        char temp[MAX_PATH];

                        Edit_GetText(   hwndSaveStreamsDirectory, 
                                        temp,
                                        MAX_PATH);

                        m_proposedValues.saveStreamsDirectory = temp;

                        if(m_proposedValues != m_currentValues)
                        {
                            PropSheet_Changed(GetParent(hwnd), hwnd);
                        }
                        else
                        {
                            PropSheet_UnChanged(GetParent(hwnd), hwnd);
                        }
                    }

                    break;
                }

                case IDC_BROWSE:
                {
                    LPMALLOC pMalloc;

                    if(SUCCEEDED(SHGetMalloc(&pMalloc)))
                    {
                        BROWSEINFO bi; 
                        LPITEMIDLIST browseId;
                        char displayName[MAX_PATH + 1];

                        bi.hwndOwner = hwnd;
                        bi.pidlRoot = NULL;
                        bi.pszDisplayName = displayName;
                        bi.lpszTitle = "Please select the folder to which you want to save streams.";
                        bi.ulFlags = BIF_RETURNONLYFSDIRS;
                        bi.lpfn = NULL;

                        browseId = SHBrowseForFolder(&bi);
            
                        if(browseId)
                        {
                            char temp[MAX_PATH];

                            SHGetPathFromIDList(browseId, temp);
                            
                            m_proposedValues.saveStreamsDirectory = temp;

                            Edit_SetText(hwndSaveStreamsDirectory, temp);

                            pMalloc->Free(browseId);
                        }
                    }
                
                    break;
                } 

                case IDC_USEPROXY:
                {
                    BOOL enabled;

                    if(Button_GetCheck(hwndUseProxyServer) == BST_CHECKED)
                    {
                        m_proposedValues.useProxyServer = true;
                    }
                    else
                    {
                        m_proposedValues.useProxyServer = false;
                    }

                    enabled = (m_proposedValues.useProxyServer ? TRUE : FALSE);

                    Button_Enable(hwndProxyServerAddress, enabled);

                    Button_Enable(hwndProxyServerAddressText, enabled);

                    Button_Enable(hwndProxyServerPort, enabled);

                    Button_Enable(hwndProxyServerPortText, enabled);

                    Button_Enable(hwndColon, enabled);


                    if(m_proposedValues != m_currentValues)
                    {
                        PropSheet_Changed(GetParent(hwnd), hwnd);
                    }
                    else
                    {
                        PropSheet_UnChanged(GetParent(hwnd), hwnd);
                    }

                    break;
                }

                case IDC_PROXYADDRESS:
                case IDC_PORT:
                {
                    if(HIWORD(wParam) == EN_CHANGE)
                    {
                        char temp[MAX_PATH];
                        char port[6];

                        Edit_GetText( hwndProxyServerAddress, 
                                      temp,
                                      MAX_PATH);

                        m_proposedValues.proxyServer = temp;


                        Edit_GetText( hwndProxyServerPort, 
                                      port,
                                      sizeof(port));

                        if(*port)
                        {
                            m_proposedValues.proxyServer += ":";
                            m_proposedValues.proxyServer += port;
                        }

                        if(m_proposedValues != m_currentValues)
                        {
                            PropSheet_Changed(GetParent(hwnd), hwnd);
                        }
                        else
                        {
                            PropSheet_UnChanged(GetParent(hwnd), hwnd);
                        }
                    }

                    break;
                }

                case IDC_USETHISIP:
                {
                    BOOL enabled;

                    if(Button_GetCheck(hwndUseAlternateIP) == BST_CHECKED)
                    {
                        m_proposedValues.useAlternateIP = true;
                    }
                    else
                    {
                        m_proposedValues.useAlternateIP = false;
                    }

                    enabled = (m_proposedValues.useAlternateIP ? TRUE : FALSE);

                    Button_Enable(hwndUseAlternateIPText, enabled);
                    Button_Enable(hwndAlternateIPAddress1, enabled);
                    Button_Enable(hwndPeriod1, enabled);
                    Button_Enable(hwndAlternateIPAddress2, enabled);
                    Button_Enable(hwndPeriod2, enabled);
                    Button_Enable(hwndAlternateIPAddress3, enabled);
                    Button_Enable(hwndPeriod3, enabled);
                    Button_Enable(hwndAlternateIPAddress4, enabled);
                    


                    if(m_proposedValues != m_currentValues)
                    {
                        PropSheet_Changed(GetParent(hwnd), hwnd);
                    }
                    else
                    {
                        PropSheet_UnChanged(GetParent(hwnd), hwnd);
                    }

                    break;
                }

                case IDC_IPADDRESS1:
                case IDC_IPADDRESS2:
                case IDC_IPADDRESS3:
                case IDC_IPADDRESS4:
                {
                    if(HIWORD(wParam) == EN_CHANGE)
                    {
                        char ip[4];

                        Edit_GetText(hwndAlternateIPAddress1, ip, 4);

                        if(*ip)
                        {
                            m_proposedValues.alternateIP += ip;
                        }
                        else
                        {
                            m_proposedValues.alternateIP += "0";
                        }

                        m_proposedValues.alternateIP += ".";

                        Edit_GetText(hwndAlternateIPAddress2, ip, 4);

                        if(*ip)
                        {
                            m_proposedValues.alternateIP += ip;
                        }
                        else
                        {
                            m_proposedValues.alternateIP += "0";
                        }

                        m_proposedValues.alternateIP += ".";

                        Edit_GetText(hwndAlternateIPAddress3, ip, 4);

                        if(*ip)
                        {
                            m_proposedValues.alternateIP += ip;
                        }
                        else
                        {
                            m_proposedValues.alternateIP += "0";
                        }

                        m_proposedValues.alternateIP += ".";

                        Edit_GetText(hwndAlternateIPAddress4, ip, 4);

                        if(*ip)
                        {
                            m_proposedValues.alternateIP += ip;
                        }
                        else
                        {
                           m_proposedValues.alternateIP += "0";
                        }

                        if(m_proposedValues != m_currentValues)
                        {
                            PropSheet_Changed(GetParent(hwnd), hwnd);
                        }
                        else
                        {
                            PropSheet_UnChanged(GetParent(hwnd), hwnd);
                        }
                    }

                    break;
                }

            }

            break;
        }


        case WM_NOTIFY:
        {
            NMHDR* notify = (NMHDR*)lParam;

            switch(notify->code)
            {
                case PSN_HELP:
                {
                    LaunchHelp(hwnd, Preferences_Streaming);
                    break;
                }
                case PSN_SETACTIVE:
                {
                    
                    break;
                }

                case PSN_APPLY:
                {
                    SavePrefsValues(prefs, &m_proposedValues);
                    break;
                }

                case PSN_KILLACTIVE:
                {
                    
                    break;
                }

                case PSN_RESET:
                {
                    SavePrefsValues(prefs, &m_originalValues);
                    break;
                }
            }

            break;
        }
    }

    return result;
}

bool Win32PreferenceWindow::PrefDebugProc(HWND hwnd, 
                                          UINT msg, 
                                          WPARAM wParam, 
                                          LPARAM lParam)      
{
    bool result = false;
    static PROPSHEETPAGE* psp = NULL;
    static Preferences* prefs = NULL;
    static HWND hwndLog = NULL;
    static HWND hwndLogDecoder = NULL;
    static HWND hwndLogInput = NULL;
    static HWND hwndLogOutput = NULL;
    static HWND hwndLogMain = NULL;
    static HWND hwndLogPerformance = NULL;
    
    switch(msg)
    {
        case WM_INITDIALOG:
        {
            // remember these for later...
            psp = (PROPSHEETPAGE*)lParam;
            prefs = (Preferences*)psp->lParam;

            // get the handles to all our controls
            hwndLog = GetDlgItem(hwnd, IDC_LOG);
            hwndLogDecoder = GetDlgItem(hwnd, IDC_LOGDECODER);
            hwndLogInput = GetDlgItem(hwnd, IDC_LOGINPUT);
            hwndLogOutput = GetDlgItem(hwnd, IDC_LOGOUTPUT);
            hwndLogMain = GetDlgItem(hwnd, IDC_LOGMAIN);
            hwndLogPerformance = GetDlgItem(hwnd, IDC_LOGPERFORMANCE);

            // initialize our controls
            bool value;

            value = m_originalValues.enableLogging;

            Button_SetCheck(hwndLog, value); 
            Button_Enable(hwndLogDecoder, value); 
            Button_Enable(hwndLogInput, value);
            Button_Enable(hwndLogOutput, value);
            Button_Enable(hwndLogMain, value);
            Button_Enable(hwndLogPerformance, value);

            Button_SetCheck(hwndLogMain, m_originalValues.logMain); 
            Button_SetCheck(hwndLogDecoder, m_originalValues.logDecoder); 
            Button_SetCheck(hwndLogInput, m_originalValues.logInput); 
            Button_SetCheck(hwndLogOutput, m_originalValues.logOutput); 
            Button_SetCheck(hwndLogPerformance, m_originalValues.logPerformance); 
                     
            break;
        }

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDC_LOG:
                {
                    BOOL enabled;

                    if(Button_GetCheck(hwndLog) == BST_CHECKED)
                    {
                        m_proposedValues.enableLogging = true;
                       
                    }
                    else
                    {
                        m_proposedValues.enableLogging = false;
                    }

                    enabled = (m_proposedValues.enableLogging ? TRUE : FALSE);

                    Button_Enable(hwndLogDecoder, enabled); 
                    Button_Enable(hwndLogInput, enabled);
                    Button_Enable(hwndLogOutput, enabled);
                    Button_Enable(hwndLogMain, enabled);
                    Button_Enable(hwndLogPerformance, enabled);

                    if(m_proposedValues != m_currentValues)
                    {
                        PropSheet_Changed(GetParent(hwnd), hwnd);
                    }
                    else
                    {
                        PropSheet_UnChanged(GetParent(hwnd), hwnd);
                    }

                    break;
                }

                case IDC_LOGDECODER:
                {
                    if(Button_GetCheck(hwndLogDecoder) == BST_CHECKED)
                    {
                        m_proposedValues.logDecoder = true;
                    }
                    else
                    {
                        m_proposedValues.logDecoder = false;
                    }

                    if(m_proposedValues != m_currentValues)
                    {
                        PropSheet_Changed(GetParent(hwnd), hwnd);
                    }
                    else
                    {
                        PropSheet_UnChanged(GetParent(hwnd), hwnd);
                    }

                    break;
                }

                case IDC_LOGINPUT:
                {
                    if(Button_GetCheck(hwndLogInput) == BST_CHECKED)
                    {
                        m_proposedValues.logInput = true;
                    }
                    else
                    {
                        m_proposedValues.logInput = false;
                    }

                    if(m_proposedValues != m_currentValues)
                    {
                        PropSheet_Changed(GetParent(hwnd), hwnd);
                    }
                    else
                    {
                        PropSheet_UnChanged(GetParent(hwnd), hwnd);
                    }

                    break;
                }

                case IDC_LOGOUTPUT:
                {
                    if(Button_GetCheck(hwndLogOutput) == BST_CHECKED)
                    {
                        m_proposedValues.logOutput = true;
                    }
                    else
                    {
                        m_proposedValues.logOutput = false;
                    }

                    if(m_proposedValues != m_currentValues)
                    {
                        PropSheet_Changed(GetParent(hwnd), hwnd);
                    }
                    else
                    {
                        PropSheet_UnChanged(GetParent(hwnd), hwnd);
                    }

                    break;
                }

                case IDC_LOGPERFORMANCE:
                {
                    if(Button_GetCheck(hwndLogPerformance) == BST_CHECKED)
                    {
                        m_proposedValues.logPerformance = true;
                    }
                    else
                    {
                        m_proposedValues.logPerformance = false;
                    }

                    if(m_proposedValues != m_currentValues)
                    {
                        PropSheet_Changed(GetParent(hwnd), hwnd);
                    }
                    else
                    {
                        PropSheet_UnChanged(GetParent(hwnd), hwnd);
                    }

                    break;
                }

                case IDC_LOGMAIN:
                {
                    if(Button_GetCheck(hwndLogMain) == BST_CHECKED)
                    {
                        m_proposedValues.logMain = true;
                    }
                    else
                    {
                        m_proposedValues.logMain = false;
                    }

                    if(m_proposedValues != m_currentValues)
                    {
                        PropSheet_Changed(GetParent(hwnd), hwnd);
                    }
                    else
                    {
                        PropSheet_UnChanged(GetParent(hwnd), hwnd);
                    }

                    break;
                }
            }

            break;
        }

        case WM_NOTIFY:
        {
            NMHDR* notify = (NMHDR*)lParam;

            switch(notify->code)
            {
                case PSN_SETACTIVE:
                {
                    
                    break;
                }

                case PSN_APPLY:
                {
                    SavePrefsValues(prefs, &m_proposedValues);
                    break;
                }

                case PSN_KILLACTIVE:
                {
                    
                    break;
                }

                case PSN_RESET:
                {
                    SavePrefsValues(prefs, &m_originalValues);
                    break;
                }
            }

            break;
        }
    }

    return result;
}

bool Win32PreferenceWindow::PrefAboutProc(HWND hwnd, 
                                          UINT msg, 
                                          WPARAM wParam, 
                                          LPARAM lParam)      
{
    bool result = false;
    static PROPSHEETPAGE* psp = NULL;
    static Preferences* prefs = NULL;
    
    switch(msg)
    {
        case WM_INITDIALOG:
        {
            // remember these for later...
            psp = (PROPSHEETPAGE*)lParam;
            prefs = (Preferences*)psp->lParam;
            
            break;
        }

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDC_GOTOFREEAMP:
                {
                    ShellExecute(   hwnd, 
                                    "open", 
                                    "http://www.freeamp.org/", 
                                    NULL, 
                                    NULL, 
                                    SW_SHOWNORMAL);
                    break;
                }

                case IDC_GOTOEMUSIC:
                {
                    ShellExecute(   hwnd, 
                                    "open", 
                                    "http://www.emusic.com/", 
                                    NULL, 
                                    NULL, 
                                    SW_SHOWNORMAL);
                    break;
                }
            }

            break;
        }

        case WM_NOTIFY:
        {
            NMHDR* notify = (NMHDR*)lParam;

            switch(notify->code)
            {
                case PSN_HELP:
                {
                    LaunchHelp(hwnd, Preferences_About);
                    break;
                }
                case PSN_SETACTIVE:
                {
                    
                    break;
                }

                case PSN_APPLY:
                {
                    SavePrefsValues(prefs, &m_proposedValues);
                    break;
                }

                case PSN_KILLACTIVE:
                {
                    
                    break;
                }

                case PSN_RESET:
                {
                    SavePrefsValues(prefs, &m_originalValues);
                    break;
                }
            }

            break;
        }
    }

    return result;
}

void Win32PreferenceWindow::LoadThemeListBox(HWND hwnd)
{
    map<string, string>::iterator i;

	//m_pThemeMan->GetCurrentTheme(m_proposedValues.currentTheme);

    m_oThemeList.clear();    

    SendDlgItemMessage(hwnd, IDC_THEMELISTBOX, LB_RESETCONTENT, 0, 0);

    m_pThemeMan->GetThemeList(m_oThemeList);

    for(i = m_oThemeList.begin(); i != m_oThemeList.end(); i++)
    {
        int index;

    	index = SendDlgItemMessage(hwnd, IDC_THEMELISTBOX, LB_ADDSTRING,
                           0, (LPARAM)(*i).first.c_str());

        if ((*i).first == m_proposedValues.currentTheme)
            SendDlgItemMessage(hwnd, IDC_THEMELISTBOX, LB_SETCURSEL, index, 0);
    }                      
     
    EnableWindow(GetDlgItem(hwnd, IDC_DELETETHEME), 0);
}

bool Win32PreferenceWindow::PrefThemeProc(HWND hwnd, 
                                          UINT msg, 
                                          WPARAM wParam, 
                                          LPARAM lParam)      
{
    bool result = false;
    static PROPSHEETPAGE* psp = NULL;
    static Preferences* prefs = NULL;
    
    switch(msg)
    {
        case WM_INITDIALOG:
        {
            // remember these for later...
            psp = (PROPSHEETPAGE*)lParam;
            prefs = (Preferences*)psp->lParam;

            LoadThemeListBox(hwnd);

            SetFocus(GetDlgItem(hwnd, IDC_THEMELISTBOX));
            result = false;
            break;
        }

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
				case IDC_THEMELISTBOX:
                {
                    switch (HIWORD(wParam)) 
                    { 
                        case LBN_SELCHANGE: 
                        {
                	        int    iIndex;
                            char   szCurSel[256];
                    
                            iIndex = SendDlgItemMessage(hwnd, IDC_THEMELISTBOX, 
                                                            LB_GETCURSEL, 0, 0);

                            if(iIndex >= 0)
                            {
                                SendDlgItemMessage(hwnd, IDC_THEMELISTBOX, 
                                                   LB_GETTEXT, iIndex, 
                                                   (LPARAM)szCurSel);
                                if (strcasecmp(szCurSel, "freeamp"))                   
                                   EnableWindow(GetDlgItem(hwnd, IDC_DELETETHEME), 1);
                                else   
                                   EnableWindow(GetDlgItem(hwnd, IDC_DELETETHEME), 0);

                                m_proposedValues.currentTheme = string(szCurSel);

                                if(m_proposedValues != m_currentValues)
                                {
                                    PropSheet_Changed(GetParent(hwnd), hwnd);
                                }
                                else
                                {
                                    PropSheet_UnChanged(GetParent(hwnd), hwnd);
                                }
                            }  
                            
                            break;
                        }
                    }
                
                	break;
                }
                case IDC_ADDTHEME:
                {
                	OPENFILENAME sOpen;
                    char szFile[256];
                    
                    szFile[0] = 0;
                    sOpen.lStructSize = sizeof(OPENFILENAME);
                    sOpen.hwndOwner = hwnd;
                    sOpen.hInstance = NULL;
                    sOpen.lpstrFilter = kThemeFileFilter;
                    sOpen.lpstrCustomFilter = NULL;
                    sOpen.nMaxCustFilter = 0;
                    sOpen.nFilterIndex = 1;
                    sOpen.lpstrFile = szFile;
                    sOpen.nMaxFile = 256;
                    sOpen.lpstrFileTitle = NULL;
                    sOpen.lpstrInitialDir = NULL;
                    sOpen.lpstrTitle = "Open Theme";
                    sOpen.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
                    sOpen.lpstrDefExt = "fat";
                      
                	if (GetOpenFileName(&sOpen))
                    {
                        string       oThemeName(sOpen.lpstrFile);
                        Error        eRet;
                        
                        eRet = m_pThemeMan->AddTheme(oThemeName);
                        if (IsError(eRet))
                            MessageBox(hwnd, "Could not add theme.", "Add Theme", MB_OK);
                        else    
                            LoadThemeListBox(hwnd);
                    }
                    
                    break;
                }

                case IDC_DELETETHEME:
                {
                    int          iIndex;
                    string       oMesg, oThemeName;
                    Error        eRet;
                    char         szCurSel[256];
                    
                    
                    iIndex = SendDlgItemMessage(hwnd, IDC_THEMELISTBOX, LB_GETCURSEL, 0, 0);
                    SendDlgItemMessage(hwnd, IDC_THEMELISTBOX, 
                                       LB_GETTEXT, iIndex, 
                                       (LPARAM)szCurSel);
                    
                    oThemeName = szCurSel;
                    oMesg = string("Are you sure you want to delete theme ") +
                            oThemeName + string("?");
                    if(MessageBox(hwnd, oMesg.c_str(), "Confirm Delete", MB_YESNO) == IDYES)        
                    {
                        eRet = m_pThemeMan->DeleteTheme(m_oThemeList[oThemeName]);
                        if(IsError(eRet))
                            MessageBox(hwnd, "Could not delete theme.", "Delete Theme", MB_OK);
                        LoadThemeListBox(hwnd);
                    }    
                    
                    break;
                }

                case IDC_CHOOSEFONT:
                {
                	CHOOSEFONT sFont;
                    LOGFONT    sLogFont;
                    
                    memset(&sLogFont, 0, sizeof(sLogFont));
                    
                    strcpy(sLogFont.lfFaceName, m_proposedValues.defaultFont.c_str());
                    sLogFont.lfWeight = FW_NORMAL;
                    sLogFont.lfHeight = -24;
                    
                    sFont.lStructSize = sizeof(sFont);
                    sFont.hwndOwner = hwnd;
                    sFont.hDC = NULL;
                    sFont.lpLogFont = &sLogFont;
                    sFont.iPointSize = 18;
                    sFont.Flags = CF_TTONLY | CF_FORCEFONTEXIST |
                                  CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT |
                                  CF_LIMITSIZE;
                    sFont.nSizeMin = 18;              
                    sFont.nSizeMax = 18;              
                    
                    if(ChooseFont(&sFont))
                    {
                    	m_proposedValues.defaultFont= sLogFont.lfFaceName;

                        if(m_proposedValues != m_currentValues)
                        {
                            PropSheet_Changed(GetParent(hwnd), hwnd);
                        }
                        else
                        {
                            PropSheet_UnChanged(GetParent(hwnd), hwnd);
                        }
                    }    
                    
                    break;
                }
            }

            break;
        }

        case WM_NOTIFY:
        {
            NMHDR* notify = (NMHDR*)lParam;

            switch(notify->code)
            {
                case PSN_HELP:
                {
                    LaunchHelp(hwnd, Preferences_Themes);
                    break;
                }
                case PSN_SETACTIVE:
                {
                    
                    break;
                }

                case PSN_APPLY:
                {
                    SavePrefsValues(prefs, &m_proposedValues);
                    LoadThemeListBox(hwnd);
                    break;
                }

                case PSN_KILLACTIVE:
                {
                    
                    break;
                }

                case PSN_RESET:
                {
                    SavePrefsValues(prefs, &m_originalValues);                         
                    break;
                }
            }

            break;
        }
    }

    return result;
}

typedef struct ThreadStruct {
    Thread* thread;
    FAContext* context;
    UpdateManager* um;
    HWND hwndList;
    bool cancel;
}ThreadStruct;

static bool callback_function(UMEvent* event, void* userData)
{
    ThreadStruct* ts = (ThreadStruct*)userData;
    HWND hwndParent = GetParent(ts->hwndList);
    HWND hwndStatus = GetDlgItem(hwndParent, IDC_STATUS);

    switch(event->type)
    {
        case kUMEvent_Status:
        {
            ostringstream ost;

            ost << " Status: " << event->eventString << endl;

            SetWindowText(hwndStatus, ost.str().c_str());
            break;
        }

        case kUMEvent_Progress:
        {
            UMEventProgressData data = event->data.progressData;

            ostringstream ost;

            ost << " Downloading "; 

            
            if(data.item)
            {
                ost << data.item->GetLocalFileName() << " -  Progress: ";      
            }

            float total;
            float recvd;
            uint32 percent;
            
            ost.precision(2);
            ost.flags(ios_base::fixed);

            total = data.total;
            recvd = data.position;
            percent= (uint32)recvd/total*100;

            if(total >= 1048576)
            {
                total /= 1048576;
                recvd /= 1048576;
                ost  << percent << "% (" << recvd << " of "<< total << " MB) ";
            }
            else if(total >= 1024)
            {
                total /= 1024;
                recvd /= 1024;
                ost << percent << "% ("<< recvd << " of "<< total << " MB)";
            }
            else
            {
                ost << percent << "% (" << data.position << " of " << 
                    data.total << " Bytes)";
            }

            SetWindowText(hwndStatus, ost.str().c_str());
            break;
        }

        case kUMEvent_Done:
        {
            ostringstream ost;

            ost << " Status: " << event->eventString << endl;

            SetWindowText(hwndStatus, ost.str().c_str());
            break;
        }

        case kUMEvent_Error:
        {
            UMEventErrorData data = event->data.errorData;

            ostringstream ost;

            ost << " Error: An error of type " <<  data.errorCode << " occurred.";

            SetWindowText(hwndStatus, ost.str().c_str());

            break;
        }
    }

    return (!ts->cancel);
}

static void check_function(void* arg)
{
    ThreadStruct* ts = (ThreadStruct*)arg;
    HWND hwnd = GetParent(ts->hwndList);
    HWND hwndPref = GetParent(hwnd);
    HWND hwndUpdate = GetDlgItem(hwnd, IDC_UPDATE);
    HWND hwndCheck = GetDlgItem(hwnd, IDC_CHECK);
    HWND hwndCancel = GetDlgItem(hwnd, IDC_CANCELCHECK);
    HWND hwndStatus = GetDlgItem(hwnd, IDC_STATUS);
    HWND hwndPrefOK = GetDlgItem(hwndPref, IDOK);
    HWND hwndPrefCancel = GetDlgItem(hwndPref, IDCANCEL);

    //SetWindowText(hwndPrefOK, "Cool!");
    //SetWindowText(hwndPrefCancel, "Bogus!");
    EnableWindow(hwndPrefOK, FALSE);
    EnableWindow(hwndPrefCancel, FALSE);
    
    EnableWindow(hwndUpdate, FALSE);
    EnableWindow(hwndCancel, TRUE);
    ShowWindow(hwndCheck, SW_HIDE);
    ShowWindow(hwndCancel, SW_SHOW);
    
    
    Error result;
    result = ts->um->RetrieveLatestVersionInfo(callback_function, ts);

    if(IsntError(result))
    {
        // Add items
        LV_ITEM lv_item;
        UpdateItem* item = NULL;
        uint32 i = 0;
        uint32 count = ListView_GetItemCount(ts->hwndList);

        while(item = ts->um->ItemAt(i++))
        {
            bool newItem = true;

            for(uint32 j = 0; j < count; j++)
            {
                lv_item.mask = LVIF_PARAM;
                lv_item.iItem = j;

                if(ListView_GetItem(ts->hwndList, &lv_item))
                {
                    if((UpdateItem*)lv_item.lParam == item)
                    {
                        newItem = false;
                        break;
                    }
                }
            }

            if(newItem)
            {
                lv_item.mask = LVIF_PARAM | LVIF_STATE;
                lv_item.state = 0;
                lv_item.stateMask = 0;
                lv_item.iItem = ListView_GetItemCount(ts->hwndList);
                lv_item.iSubItem = 0;
                lv_item.lParam = (LPARAM)item;

                ListView_InsertItem(ts->hwndList, &lv_item);
            }
        }

        ListView_RedrawItems(ts->hwndList, 0, ListView_GetItemCount(ts->hwndList) - 1);
    }
    else if(result == kError_UserCancel)
    {
        SetWindowText(hwndStatus, " Status: Update cancelled by user."); 
    }

    EnableWindow(hwndUpdate, TRUE);
    EnableWindow(hwndCancel, FALSE);
    ShowWindow(hwndCheck, SW_SHOW);
    ShowWindow(hwndCancel, SW_HIDE);

    EnableWindow(hwndPrefOK, TRUE);
    EnableWindow(hwndPrefCancel, TRUE);

    delete ts->thread;
}

static void update_function(void* arg)
{
    ThreadStruct* ts = (ThreadStruct*)arg;
    Error result;
    HWND hwnd = GetParent(ts->hwndList);
    HWND hwndPref = GetParent(hwnd);
    HWND hwndUpdate = GetDlgItem(hwnd, IDC_UPDATE);
    HWND hwndCheck = GetDlgItem(hwnd, IDC_CHECK);
    HWND hwndCancel = GetDlgItem(hwnd, IDC_CANCELUPDATE);
    HWND hwndStatus = GetDlgItem(hwnd, IDC_STATUS);
    HWND hwndPrefOK = GetDlgItem(hwndPref, IDOK);
    HWND hwndPrefCancel = GetDlgItem(hwndPref, IDCANCEL);
    
    EnableWindow(hwndPrefOK, FALSE);
    EnableWindow(hwndPrefCancel, FALSE);

    EnableWindow(hwndCheck, FALSE);
    EnableWindow(hwndCancel, TRUE);
    ShowWindow(hwndUpdate, SW_HIDE);
    ShowWindow(hwndCancel, SW_SHOW);

    ts->um->RetrieveLatestVersionInfo();

    // Add items
    LV_ITEM lv_item;
    UpdateItem* item = NULL;
    uint32 i = 0;
    uint32 count = ListView_GetItemCount(ts->hwndList);

    while(item = ts->um->ItemAt(i++))
    {
        bool newItem = true;

        for(uint32 j = 0; j < count; j++)
        {
            lv_item.mask = LVIF_PARAM;
            lv_item.iItem = j;

            if(ListView_GetItem(ts->hwndList, &lv_item))
            {
                if((UpdateItem*)lv_item.lParam == item)
                {
                    newItem = false;
                    break;
                }
            }
        }

        if(newItem)
        {
            lv_item.mask = LVIF_PARAM | LVIF_STATE;
            lv_item.state = 0;
            lv_item.stateMask = 0;
            lv_item.iItem = ListView_GetItemCount(ts->hwndList);
            lv_item.iSubItem = 0;
            lv_item.lParam = (LPARAM)item;

            ListView_InsertItem(ts->hwndList, &lv_item);
        }
    }

    ListView_RedrawItems(ts->hwndList, 0, ListView_GetItemCount(ts->hwndList) - 1);

    result = ts->um->UpdateComponents(callback_function, ts);

    if(IsntError(result))
    {
        int32 response;
        HWND hwnd = GetParent(GetParent(ts->hwndList));

        response = MessageBox(hwnd, 
                            "FreeAmp needs to close down and restart in order to replace components\r\n"
                            "which are being used. If you do not wish to quit the application you\r\n"
                            "can choose \"Cancel\" and update again at a later time.",
                            "Restart FreeAmp?", 
                            MB_OKCANCEL|MB_ICONQUESTION);

        if(response == IDOK)
        {
            char appPath[MAX_PATH];
            uint32 length = sizeof(appPath);
            ts->context->prefs->GetPrefString(kInstallDirPref, appPath, &length);
            
            strcat(appPath, "\\update.exe");

            ts->context->target->AcceptEvent(new Event(CMD_QuitPlayer));

            WinExec(appPath, SW_NORMAL);
        }
    }
    else if(result == kError_UserCancel)
    {
        SetWindowText(hwndStatus, " Status: Update cancelled by user."); 
    }

    EnableWindow(hwndCheck, TRUE);
    EnableWindow(hwndCancel, FALSE);
    ShowWindow(hwndUpdate, SW_SHOW);
    ShowWindow(hwndCancel, SW_HIDE);

    EnableWindow(hwndPrefOK, TRUE);
    EnableWindow(hwndPrefCancel, TRUE);
    delete ts->thread;
}

bool Win32PreferenceWindow::PrefUpdateProc(HWND hwnd, 
                                           UINT msg, 
                                           WPARAM wParam, 
                                           LPARAM lParam)      
{
    bool result = false;
    static PROPSHEETPAGE* psp = NULL;
    static UpdateManager* um = NULL;
    static Preferences* prefs = NULL;
    static HWND hwndList = NULL;
    static HWND hwndDescription = NULL;
    static HWND hwndUpdate = NULL;
    static HWND hwndCheck = NULL;
    static HWND hwndAutoCheck = NULL;
    static ThreadStruct ts;
    const char* kNoSelection = "No component is selected.";

    
    switch(msg)
    {
        case WM_INITDIALOG:
        {
            // remember these for later...
            psp = (PROPSHEETPAGE*)lParam;
            um = (UpdateManager*)psp->lParam;
            prefs = m_pContext->prefs;


            hwndList = GetDlgItem(hwnd, IDC_LIST);
            hwndDescription = GetDlgItem(hwnd, IDC_DESCRIPTION);
            hwndUpdate = GetDlgItem(hwnd, IDC_UPDATE);
            hwndCheck = GetDlgItem(hwnd, IDC_CHECK);
            hwndAutoCheck = GetDlgItem(hwnd, IDC_AUTOCHECK);

            // Init our controls

            // First let's add our columns
            RECT rect;
            GetClientRect(hwndList, &rect);

            LV_COLUMN lvc;

            lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
            lvc.fmt = LVCFMT_LEFT; // left align column
            lvc.cx = (rect.right-rect.left)/2; // width of column in pixels
            lvc.pszText = "Component Name";
            lvc.iSubItem = 0;

            lvc.cx -= GetSystemMetrics(SM_CXVSCROLL); // width of the scrollbar

            if((rect.right-rect.left)%2) // squeeze out every last pixel :)
                lvc.cx += 1;

            ListView_InsertColumn(hwndList, 0, &lvc);

            lvc.pszText = "Your Version";
            lvc.cx = (rect.right-rect.left)/4; // width of column in pixels
            lvc.iSubItem = 1;

            ListView_InsertColumn(hwndList, 1, &lvc);

            lvc.pszText = "Latest Version";
            lvc.iSubItem = 2;

            ListView_InsertColumn(hwndList, 2, &lvc);

            // Add Items that are currently in the update manager
            LV_ITEM lv_item;
            UpdateItem* item = NULL;
            uint32 i = 0;

            while(item = um->ItemAt(i++))
            {
                lv_item.mask = LVIF_PARAM | LVIF_STATE;
                lv_item.state = 0;
                lv_item.stateMask = 0;
                lv_item.iItem = ListView_GetItemCount(hwndList);
                lv_item.iSubItem = 0;
                lv_item.lParam = (LPARAM)item;

                ListView_InsertItem(hwndList, &lv_item);
            }

            // Don't want this to be blank on startup
            SetWindowText(hwndDescription, kNoSelection);

            Button_SetCheck(hwndAutoCheck, m_originalValues.checkForUpdates);

            break;
        }

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDC_UPDATE:
                {
                    Thread* thread = Thread::CreateThread();

                    if(thread)
                    {
                        ts.thread = thread;
                        ts.um = um;
                        ts.hwndList = hwndList;
                        ts.context = m_pContext;
                        ts.cancel = false;

                        thread->Create(update_function, &ts);
                    }
                	break;
                }

                case IDC_CHECK:
                {
                    Thread* thread = Thread::CreateThread();

                    if(thread)
                    {
                        ts.thread = thread;
                        ts.um = um;
                        ts.hwndList = hwndList;
                        ts.context = m_pContext;
                        ts.cancel = false;

                        thread->Create(check_function, &ts);
                    }
                    break;
                }         
                
                case IDC_CANCELCHECK:
                case IDC_CANCELUPDATE:
                {
                    ts.cancel = true;
                    break;
                }

                case IDC_AUTOCHECK:
                {
                    if(Button_GetCheck(hwndAutoCheck) == BST_CHECKED)
                    {
                        m_proposedValues.checkForUpdates = true;
                    }
                    else
                    {
                        m_proposedValues.checkForUpdates = false;
                    }

                    if(m_proposedValues != m_currentValues)
                    {
                        PropSheet_Changed(GetParent(hwnd), hwnd);
                    }
                    else
                    {
                        PropSheet_UnChanged(GetParent(hwnd), hwnd);
                    }

                    break;
                }

            }

            break;
        }

        case WM_DRAWITEM:
        {
            DRAWITEMSTRUCT* dis = (DRAWITEMSTRUCT*) lParam;
            int32 controlId = (UINT) wParam;
            UpdateItem* item = (UpdateItem*)dis->itemData;

            uint32 localMajorVersion, currentMajorVersion;
            uint32 localMinorVersion, currentMinorVersion;
            uint32 localRevisionVersion, currentRevisionVersion;
            int32 numFields;
            bool currentVersionMoreRecent = false;
            
            numFields = sscanf(item->GetLocalFileVersion().c_str(),
                   "%lu.%lu.%lu", 
                   &localMajorVersion,&localMinorVersion,&localRevisionVersion);

            if(numFields < 3)
                localRevisionVersion = 0;

            if(numFields < 2)
                localMinorVersion = 0;

            if(numFields < 1)
                localMajorVersion = 0;
            
            numFields = sscanf(item->GetCurrentFileVersion().c_str(),
                   "%d.%d.%d", 
                   &currentMajorVersion,&currentMinorVersion,&currentRevisionVersion);

            if(numFields < 3)
                currentRevisionVersion = 0;

            if(numFields < 2)
                currentMinorVersion = 0;

            if(numFields < 1)
                currentMajorVersion = 0;

            // is the version on the server more recent?
            if( (currentMajorVersion > localMajorVersion) ||
                (currentMajorVersion == localMajorVersion && 
                 currentMinorVersion > localMinorVersion) ||
                (currentMajorVersion == localMajorVersion && 
                 currentMinorVersion == localMinorVersion &&
                 currentRevisionVersion > localRevisionVersion))
            {
                currentVersionMoreRecent = true;
            }


            uint32 uiFlags = ILD_TRANSPARENT;
            RECT rcClip;

            // Check to see if this item is selected
            if(dis->itemState & ODS_SELECTED)
            {
                // Set the text background and foreground colors
                SetTextColor(dis->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
                SetBkColor(dis->hDC, GetSysColor(COLOR_HIGHLIGHT));
            }
            else
            {
                // Set the text background and foreground colors 
                // to the standard window colors
                SetTextColor(dis->hDC, GetSysColor(COLOR_WINDOWTEXT));
                SetBkColor(dis->hDC, GetSysColor(COLOR_WINDOW));
            }

            rcClip = dis->rcItem;            

            // Check to see if the string fits in the clip rect.  If not, truncate
            // the string and add "...".
            string displayString = item->GetLocalFileName();

            CalcStringEllipsis(dis->hDC, 
                               displayString, 
                               ListView_GetColumnWidth(hwndList, 0));

            ExtTextOut( dis->hDC, 
                        rcClip.left + 3, rcClip.top + 1, 
                        ETO_CLIPPED | ETO_OPAQUE,
                        &rcClip, 
                        displayString.c_str(),
                        displayString.size(),
                        NULL);

            UINT oldAlign = SetTextAlign(dis->hDC, TA_CENTER);    

            rcClip.left += ListView_GetColumnWidth(hwndList, 0);

            displayString = item->GetLocalFileVersion();

            CalcStringEllipsis(dis->hDC, 
                               displayString, 
                               ListView_GetColumnWidth(hwndList, 1));

            if(currentVersionMoreRecent && !(dis->itemState & ODS_SELECTED))
                SetTextColor(dis->hDC, RGB(192, 0, 0));

            ExtTextOut( dis->hDC, 
                        rcClip.left + (ListView_GetColumnWidth(hwndList, 1))/2, 
                        rcClip.top + 1, 
                        ETO_CLIPPED | ETO_OPAQUE,
                        &rcClip, 
                        displayString.c_str(),
                        displayString.size(),
                        NULL);
                   
        
            rcClip.left += ListView_GetColumnWidth(hwndList, 1);

            displayString = item->GetCurrentFileVersion();

            CalcStringEllipsis(dis->hDC, 
                               displayString, 
                               ListView_GetColumnWidth(hwndList, 2));

            if(currentVersionMoreRecent && !(dis->itemState & ODS_SELECTED))
                SetTextColor(dis->hDC, RGB(0, 127, 0));

            ExtTextOut( dis->hDC, 
                        rcClip.left + (ListView_GetColumnWidth(hwndList, 1))/2,
                        rcClip.top + 1, 
                        ETO_CLIPPED | ETO_OPAQUE,
                        &rcClip, 
                        displayString.c_str(),
                        displayString.size(),
                        NULL);

            SetTextAlign(dis->hDC, oldAlign);   


            // If we changed the colors for the selected item, undo it
            if(dis->itemState & ODS_SELECTED)
            {
                // Set the text background and foreground colors
                SetTextColor(dis->hDC, GetSysColor(COLOR_WINDOWTEXT));
                SetBkColor(dis->hDC, GetSysColor(COLOR_WINDOW));
            }

            // If the item is focused, now draw a focus rect around the entire row
            if(dis->itemState & ODS_FOCUS)
            {
                // Draw the focus rect
                DrawFocusRect(dis->hDC, &dis->rcItem);
            }

            result = true;
            break;
        }

        case WM_NOTIFY:
        {
            NMHDR* notify = (NMHDR*)lParam;

            if(notify->hwndFrom == hwndList)
            {
                NM_LISTVIEW* nmlv = (NM_LISTVIEW*)notify;

                if(notify->code == LVN_ITEMCHANGED)
                {
                    UpdateItem* item = (UpdateItem*)nmlv->lParam;

                    SetWindowText(hwndDescription, item->GetFileDescription().c_str());
                }
            }
            else
            {
                switch(notify->code)
                {
                    case PSN_HELP:
                    {
                        LaunchHelp(hwnd, Preferences_Update);
                        break;
                    }
                    case PSN_SETACTIVE:
                    {
                    
                        break;
                    }

                    case PSN_APPLY:
                    {
                        SavePrefsValues(prefs, &m_proposedValues);
                        break;
                    }

                    case PSN_KILLACTIVE:
                    {
                    
                        break;
                    }

                    case PSN_RESET:
                    {
                        SavePrefsValues(prefs, &m_originalValues);
                        break;
                    }
                }
            }

            break;
        }
    }

    return result;
}

uint32 CalcStringEllipsis(HDC hdc, string& displayString, int32 columnWidth)
{
    const TCHAR szEllipsis[] = TEXT("...");
    SIZE   sizeString;
    SIZE   sizeEllipsis;
    string temp;
    
    // Adjust the column width to take into account the edges
    //columnWidth -= 4;

    temp = displayString;        

    GetTextExtentPoint32(hdc, temp.c_str(), temp.size(), &sizeString);       

    // If the width of the string is greater than the column width shave
    // the string and add the ellipsis
    if(sizeString.cx > columnWidth)
    {
        GetTextExtentPoint32(hdc, szEllipsis, strlen(szEllipsis), &sizeEllipsis);
       
        while(temp.size() > 1)
        {
            temp.erase(temp.size() - 1, 1);

            GetTextExtentPoint32(hdc, temp.c_str(), temp.size(), &sizeString);
            
            if ((uint32)(sizeString.cx + sizeEllipsis.cx) <= columnWidth)
            {
                // The string with the ellipsis finally fits                
                // Concatenate the two strings and break out of the loop
                temp += szEllipsis;
                displayString = temp;
                break;
            }
            else if(temp.size() == 1)
            {
                temp += szEllipsis;
                displayString = temp;
                break;
            }
        }
    }

    GetTextExtentPoint32(hdc, displayString.c_str(), displayString.size(), &sizeString);

    return sizeString.cx;
}

bool Win32PreferenceWindow::PrefAdvancedProc(HWND hwnd, 
                                             UINT msg, 
                                             WPARAM wParam, 
                                             LPARAM lParam)      
{
    bool result = false;
    static PROPSHEETPAGE* psp = NULL;
    static Preferences* prefs = NULL;
    static HWND hwndPMO = NULL;
    static HWND hwndPriority = NULL;
    static HWND hwndInput = NULL;
    static HWND hwndOutput = NULL;
    static HWND hwndPrebuffer = NULL;
    static HWND hwndStayOnTop = NULL;
    static HWND hwndLiveInTray = NULL;
    

    switch(msg)
    {
        case WM_INITDIALOG:
        {
            // remember these for later...
            psp = (PROPSHEETPAGE*)lParam;
            prefs = (Preferences*)psp->lParam;

            // get the handles to all our controls
            hwndPMO = GetDlgItem(hwnd, IDC_PMO);
            hwndPriority = GetDlgItem(hwnd, IDC_PRIORITY);
            hwndInput = GetDlgItem(hwnd, IDC_INPUT);
            hwndOutput = GetDlgItem(hwnd, IDC_OUTPUT);
            hwndPrebuffer = GetDlgItem(hwnd, IDC_PREBUFFER);

            // get registries
            Registrar registrar;
            Registry pmo;
            Registry ui;

            registrar.SetSubDir("plugins");
            registrar.SetSearchString("*.pmo");
            registrar.InitializeRegistry(&pmo, prefs);
            registrar.SetSearchString("*.ui");
            registrar.InitializeRegistry(&ui, prefs);

            // initialize our controls

            int32 i = 0;
            int32 pos = 0;
            RegistryItem *item;

            while(item = pmo.GetItem(i++))
            {
                if(*item->Description())
                    pos = ComboBox_AddString(hwndPMO, item->Description());
                else
                    pos = ComboBox_AddString(hwndPMO, item->Name());

                string* name = new string(item->Name());

                ComboBox_SetItemData(hwndPMO, pos, (DWORD)name);

                if(m_originalValues.defaultPMO == item->Name())
                {
                    ComboBox_SetCurSel(hwndPMO, pos);
                }
            }

            SendMessage(hwndPriority, 
                        TBM_SETRANGE, 
                        (WPARAM) TRUE,                   
                        (LPARAM) MAKELONG(0, 6));

            SendMessage(hwndPriority, 
                        TBM_SETPOS, 
                        (WPARAM) TRUE,                   
                        (LPARAM) m_originalValues.decoderThreadPriority);

            int32 value;
            char temp[256];

            value = m_originalValues.inputBufferSize;
            sprintf(temp, "%d", value);
            Edit_LimitText(hwndInput, 4);
            Edit_SetText(hwndInput, temp);

            value = m_originalValues.outputBufferSize;
            sprintf(temp, "%d", value);
            Edit_LimitText(hwndOutput, 4);
            Edit_SetText(hwndOutput, temp);

            value = m_originalValues.preBufferLength;
            sprintf(temp, "%d", value);
            Edit_LimitText(hwndPrebuffer, 2);
            Edit_SetText(hwndPrebuffer, temp);
            
            break;
        }

        case WM_DESTROY:
        {
            int32 count = ComboBox_GetCount(hwndPMO);

            if(count != CB_ERR)
            {
                for(int32 i = 0; i < count;i++)
                {
                    string* s;
                    s = (string*)ComboBox_GetItemData(hwndPMO, i);

                    if(s)
                        delete s;
                }
            }
            break;
        }

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDC_PMO:
                {
                    if(HIWORD(wParam) == CBN_CLOSEUP)
                    {

                        int32 sel = ComboBox_GetCurSel(hwndPMO);

                        if(sel != CB_ERR)
                        {
                            string* s;
                            s = (string*)ComboBox_GetItemData(hwndPMO, sel);
                            
                            m_proposedValues.defaultPMO = *s;
                        }

                        /*ComboBox_GetText( hwndPMO, 
                                            m_proposedValues.defaultPMO, 
                                            256);*/

                        if(m_proposedValues != m_currentValues)
                        {
                            PropSheet_Changed(GetParent(hwnd), hwnd);
                        }
                        else
                        {
                            PropSheet_UnChanged(GetParent(hwnd), hwnd);
                        }
                    }

                    break;
                }                

                case IDC_INPUT:
                {
                    if(HIWORD(wParam) == EN_CHANGE)
                    {
                        char text[128];

                        Edit_GetText(hwndInput, text, sizeof(text));

                        m_proposedValues.inputBufferSize = atoi(text);

                        if(m_proposedValues != m_currentValues)
                        {
                            PropSheet_Changed(GetParent(hwnd), hwnd);
                        }
                        else
                        {
                            PropSheet_UnChanged(GetParent(hwnd), hwnd);
                        }
                    }

                    break;
                }

                case IDC_OUTPUT:
                {
                    if(HIWORD(wParam) == EN_CHANGE)
                    {
                        char text[128];

                        Edit_GetText(hwndOutput, text, sizeof(text));

                        m_proposedValues.outputBufferSize = atoi(text);

                        if(m_proposedValues != m_currentValues)
                        {
                            PropSheet_Changed(GetParent(hwnd), hwnd);
                        }
                        else
                        {
                            PropSheet_UnChanged(GetParent(hwnd), hwnd);
                        }
                    }

                    break;
                }

                case IDC_PREBUFFER:
                {
                    if(HIWORD(wParam) == EN_CHANGE)
                    {
                        char text[128];

                        Edit_GetText(hwndPrebuffer, text, sizeof(text));

                        m_proposedValues.preBufferLength = atoi(text);

                        if(m_proposedValues != m_currentValues)
                        {
                            PropSheet_Changed(GetParent(hwnd), hwnd);
                        }
                        else
                        {
                            PropSheet_UnChanged(GetParent(hwnd), hwnd);
                        }
                    }

                    break;
                }
            }

            break;
        }

        case WM_HSCROLL:
		{
			LRESULT position;
            HWND hwndScroll = (HWND) lParam;

			switch (LOWORD(wParam)) 
			{
				case TB_LINEUP:
				case TB_LINEDOWN:
				case TB_PAGEDOWN:
				case TB_PAGEUP:
				case TB_THUMBPOSITION:
                case TB_THUMBTRACK:
				{
                    if(hwndScroll == hwndPriority)
                    {
		  			    position = SendMessage(	hwndScroll, 
											    TBM_GETPOS, 
											    0, 
											    0);

                        m_proposedValues.decoderThreadPriority = position;

                        if(m_proposedValues != m_currentValues)
                        {
                            PropSheet_Changed(GetParent(hwnd), hwnd);
                        }
                        else
                        {
                            PropSheet_UnChanged(GetParent(hwnd), hwnd);
                        }
                    }
                }
			}

			break;
        }

        case WM_NOTIFY:
        {
            NMHDR* notify = (NMHDR*)lParam;

            switch(notify->code)
            {
                case PSN_HELP:
                {
                    LaunchHelp(hwnd, Preferences_Advanced);
                    break;
                }
                case PSN_SETACTIVE:
                {
                    
                    break;
                }

                case PSN_APPLY:
                {
                    SavePrefsValues(prefs, &m_proposedValues);
                    break;
                }

                case PSN_KILLACTIVE:
                {
                    
                    break;
                }

                case PSN_RESET:
                {
                    SavePrefsValues(prefs, &m_originalValues);
                    break;
                }
            }

            break;
        }
    }

    return result;
}

typedef struct PortablePlayer {
    string name;
    string plugin;
    bool checked;
} PortablePlayer;


LRESULT WINAPI 
ListViewWndProc(HWND hwnd, 
                UINT msg, 
                WPARAM wParam, 
                LPARAM lParam);

bool Win32PreferenceWindow::PrefPluginsProc(HWND hwnd, 
                                            UINT msg, 
                                            WPARAM wParam, 
                                            LPARAM lParam)      
{
    bool result = false;
    static PROPSHEETPAGE* psp = NULL;
    static Preferences* prefs = NULL;
    static HWND hwndList = NULL;
    static HWND hwndPMO = NULL;
    static HIMAGELIST imageList = NULL;
    static PortableSet originalSet;
    static PortableSet currentSet;

    
    switch(msg)
    {
        case WM_INITDIALOG:
        {
            // remember these for later...
            psp = (PROPSHEETPAGE*)lParam;
            prefs = (Preferences*)psp->lParam;

            // get the handles to all our controls
            hwndList = GetDlgItem(hwnd, IDC_PORTABLELIST);
            hwndPMO = GetDlgItem(hwnd, IDC_PMO);

            // Init our controls

            // get registries
            Registrar registrar;
            Registry pmo;
            Registry ppp;

            registrar.SetSubDir("plugins");
            registrar.SetSearchString("*.pmo");
            registrar.InitializeRegistry(&pmo, prefs);

            registrar.SetSearchString("*.ppp");
            registrar.InitializeRegistry(&ppp, prefs);

            uint32 i = 0;
            int32 pos = 0;
            RegistryItem *item;

            while(item = pmo.GetItem(i++))
            {
                if(*item->Description())
                    pos = ComboBox_AddString(hwndPMO, item->Description());
                else
                    pos = ComboBox_AddString(hwndPMO, item->Name());

                string* name = new string(item->Name());

                ComboBox_SetItemData(hwndPMO, pos, (DWORD)name);

                if(m_originalValues.defaultPMO == item->Name())
                {
                    ComboBox_SetCurSel(hwndPMO, pos);
                }
            }
            
            HINSTANCE hinst = (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE);

            // get our checkbox images
            imageList = ImageList_Create(16, 16, ILC_COLOR24|ILC_MASK, 2, 0);

            HBITMAP bmp;
            bmp = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_UNCHECKED));
            ImageList_AddMasked(imageList, bmp, RGB(255,0,0));
            DeleteObject(bmp);
            bmp = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_CHECKED));
            ImageList_AddMasked(imageList, bmp, RGB(255,0,0));
            DeleteObject(bmp);

            ListView_SetImageList(hwndList, imageList, LVSIL_SMALL); 

            // add items to portable plugin list
            LV_ITEM lv_item;
            i = 0;

            while(item = ppp.GetItem(i++))
            {
                lv_item.mask = LVIF_PARAM | LVIF_STATE;
                lv_item.state = 0;
                lv_item.stateMask = 0;
                lv_item.iItem = ListView_GetItemCount(hwndList);
                lv_item.iSubItem = 0;

                PortablePlayer* p = new PortablePlayer;

                lv_item.lParam = (LPARAM)p;

                ListView_InsertItem(hwndList, &lv_item);

                if(*item->Description())
                    p->name = item->Description();
                else
                    p->name = item->Name();

                p->plugin = item->Name();

                PortableSet::iterator i;

                i = m_originalValues.portablePlayers.find(string(item->Name()));

                p->checked = i != m_originalValues.portablePlayers.end();

            }

            // add our columns
            RECT rect;
            GetClientRect(hwndList, &rect);

            LV_COLUMN lvc;

            lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
            lvc.fmt = LVCFMT_LEFT; // left align column
            lvc.cx = (rect.right-rect.left); // width of column in pixels
            lvc.pszText = "Component Name";
            lvc.iSubItem = 0;

            lvc.cx -= GetSystemMetrics(SM_CXVSCROLL); // width of scrollbar

            ListView_InsertColumn(hwndList, 0, &lvc);

            break;
        }

        case WM_DESTROY:
        {
            int32 count = ComboBox_GetCount(hwndPMO);

            if(count != CB_ERR)
            {
                for(int32 i = 0; i < count;i++)
                {
                    string* s;
                    s = (string*)ComboBox_GetItemData(hwndPMO, i);

                    if(s)
                        delete s;
                }
            }


            count = ListView_GetItemCount(hwndList);

            if(count)
            {
                LV_ITEM item;

                for(int32 i = 0; i < count; i++)
                {
                    item.mask = LVIF_PARAM;
                    item.iItem = i;
                    item.lParam = 0;

                    if(ListView_GetItem(hwndList, &item))
                    {
                        if(item.lParam)
                        {
                            delete (PortablePlayer*)item.lParam;
                            break;
                        }
                    }
                }
            }

            break;
        }

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDC_PMO:
                {
                    if(HIWORD(wParam) == CBN_CLOSEUP)
                    {

                        int32 sel = ComboBox_GetCurSel(hwndPMO);

                        if(sel != CB_ERR)
                        {
                            string* s;
                            s = (string*)ComboBox_GetItemData(hwndPMO, sel);
                            
                            m_proposedValues.defaultPMO = *s;
                        }

                        /*ComboBox_GetText( hwndPMO, 
                                            m_proposedValues.defaultPMO, 
                                            256);*/

                        if(m_proposedValues != m_currentValues)
                        {
                            PropSheet_Changed(GetParent(hwnd), hwnd);
                        }
                        else
                        {
                            PropSheet_UnChanged(GetParent(hwnd), hwnd);
                        }
                    }

                    break;
                }
            }

            break;
        }

        case WM_MEASUREITEM:
        {
            MEASUREITEMSTRUCT* mis = (MEASUREITEMSTRUCT*) lParam;
            TEXTMETRIC tm;
            HDC hdc;
	        HFONT hFont, oldFont;
	        HWND hwndLV;


            // Make sure the control is the listview control
            if (mis->CtlType != ODT_LISTVIEW || mis->CtlID != IDC_PORTABLELIST)
                return FALSE;

	        // Get the handle of the ListView control we're using
	        hwndLV = GetDlgItem(hwnd, mis->CtlID);

	        // Get the font the control is currently using
	        hFont = (HFONT)(DWORD) SendMessage(hwndLV, WM_GETFONT, 0, 0L);

	        // Set the font of the DC to the same font the control is using
            hdc = GetDC(hwndLV);
	        oldFont = (HFONT)SelectObject(hdc, hFont);

            // Get the height of the font used by the control
            if (!GetTextMetrics(hdc, &tm))
                return FALSE;

            // Add a little extra space between items
            mis->itemHeight = tm.tmHeight + 1;

            // Make sure there is enough room for the images which are CY_SMICON high
            if (mis->itemHeight < 17)
    	        mis->itemHeight = 17;

            // width ?
            RECT rect;
            GetClientRect(hwndLV, &rect);

            mis->itemWidth = (rect.right-rect.left); // width of column in pixels

	        // Clean up
            SelectObject(hdc, oldFont);
	        ReleaseDC(hwndLV, hdc);

            return TRUE;

            break;
        }

        case WM_DRAWITEM:
        {
            DRAWITEMSTRUCT* dis = (DRAWITEMSTRUCT*) lParam;
            int32 controlId = (UINT) wParam;
            HIMAGELIST himl;
            int32 cxImage = 0, cyImage = 0;
            PortablePlayer* item = (PortablePlayer*)dis->itemData;

            // Get Image List
            himl = ListView_GetImageList(dis->hwndItem, LVSIL_SMALL);
            ImageList_GetIconSize(himl, &cxImage, &cyImage);

            uint32 uiFlags = ILD_TRANSPARENT;
            RECT rcClip;

            // Check to see if this item is selected
            /*if(dis->itemState & ODS_SELECTED)
            {
                // Set the text background and foreground colors
                SetTextColor(dis->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
                SetBkColor(dis->hDC, GetSysColor(COLOR_HIGHLIGHT));
            }
            else*/
            {
                // Set the text background and foreground colors 
                // to the standard window colors
                SetTextColor(dis->hDC, GetSysColor(COLOR_WINDOWTEXT));
                SetBkColor(dis->hDC, GetSysColor(COLOR_WINDOW));
            }

            rcClip = dis->rcItem;            

            // Check to see if the string fits in the clip rect.  If not, truncate
            // the string and add "...".
            string displayString = item->name;

            /*CalcStringEllipsis(dis->hDC, 
                               displayString, 
                               ListView_GetColumnWidth(hwndList, 0));*/

            // Get the height of the font used by the control
            TEXTMETRIC tm;
            GetTextMetrics(dis->hDC, &tm);
            
            uint32 top = rcClip.top + ((rcClip.bottom - rcClip.top) - tm.tmHeight)/2;

            ExtTextOut( dis->hDC, 
                        rcClip.left + cxImage + 3, top, 
                        ETO_CLIPPED | ETO_OPAQUE,
                        &rcClip, 
                        displayString.c_str(),
                        displayString.size(),
                        NULL);

            // Draw the icon. Drawing it after the first item allows us
            // to let ExtTextOut draw the correct background 
            if(himl)
            {
                top = rcClip.top + ((rcClip.bottom - rcClip.top) - cyImage)/2;
                int32 index = item->checked ? 1 : 0;
                ImageList_Draw( himl, index, dis->hDC, 
                                rcClip.left, top,
                                uiFlags);
            }

            // If we changed the colors for the selected item, undo it
            if(dis->itemState & ODS_SELECTED)
            {
                // Set the text background and foreground colors
                SetTextColor(dis->hDC, GetSysColor(COLOR_WINDOWTEXT));
                SetBkColor(dis->hDC, GetSysColor(COLOR_WINDOW));
            }

            // If the item is focused, now draw a focus rect around the entire row
            /*if(dis->itemState & ODS_FOCUS)
            {
                // Draw the focus rect
                DrawFocusRect(dis->hDC, &dis->rcItem);
            }*/

            

            result = true;
            break;
        }

        case WM_NOTIFY:
        {
            NMHDR* notify = (NMHDR*)lParam;

            if(notify->hwndFrom == hwndList)
            {
                NM_LISTVIEW* nmlv = (NM_LISTVIEW*)notify;

                if(notify->code == NM_CLICK)
                {
                    LV_HITTESTINFO hti;
                    POINT pt;

                    GetCursorPos(&pt);
                    ScreenToClient(hwndList, &pt);
            
                    hti.pt.x = pt.x;
                    hti.pt.y = pt.y;

                    int32 index = ListView_HitTest(hwndList, &hti);

                    if(index >= 0)
                    {
                        LV_ITEM lv_item;

                        lv_item.mask = LVIF_PARAM;
                        lv_item.iItem = index;
                        lv_item.iSubItem = 0;

                        if(ListView_GetItem(hwndList, &lv_item ))
                        {
                            PortablePlayer* p = (PortablePlayer*)lv_item.lParam;

                            p->checked = !p->checked;

                            if(p->checked)
                                m_proposedValues.portablePlayers.insert(p->plugin);
                            else
                                m_proposedValues.portablePlayers.erase(p->plugin);
                          
                            if(m_proposedValues != m_currentValues)
                            {
                                PropSheet_Changed(GetParent(hwnd), hwnd);
                            }
                            else
                            {
                                PropSheet_UnChanged(GetParent(hwnd), hwnd);
                            }

                            ListView_RedrawItems(hwndList, index, index);
                        }
                    }
                }
            }
            else
            {
                switch(notify->code)
                {
                    case PSN_HELP:
                    {
                        LaunchHelp(hwnd, Preferences_Plugins);
                        break;
                    }
                    case PSN_SETACTIVE:
                    {
                        break;
                    }

                    case PSN_APPLY:
                    {
                        SavePrefsValues(prefs, &m_proposedValues);
                        break;
                    }

                    case PSN_KILLACTIVE:
                    {
                        break;
                    }

                    case PSN_RESET:
                    {
                        SavePrefsValues(prefs, &m_originalValues);
                        break;
                    }
                }
            }

            break;
        }
    }

    return result;
}
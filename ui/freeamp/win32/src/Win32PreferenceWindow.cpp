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

#include <sstream>

using namespace std;

#include "eventdata.h"
#include "thread.h"
#include "win32updatemanager.h"
#include "Win32PreferenceWindow.h"
#include "Win32Window.h"
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
PrefPage1Callback(HWND hwnd, 
                  UINT msg, 
                  WPARAM wParam, 
                  LPARAM lParam)
{
	return g_pCurrentPrefWindow->PrefPage1Proc(hwnd, msg, wParam, lParam);
}          

static BOOL CALLBACK 
PrefPage2Callback(HWND hwnd, 
                  UINT msg, 
                  WPARAM wParam, 
                  LPARAM lParam)
{
	return g_pCurrentPrefWindow->PrefPage2Proc(hwnd, msg, wParam, lParam);
}          

static BOOL CALLBACK 
PrefPage3Callback(HWND hwnd, 
                  UINT msg, 
                  WPARAM wParam, 
                  LPARAM lParam)
{
	return g_pCurrentPrefWindow->PrefPage3Proc(hwnd, msg, wParam, lParam);
}          

static BOOL CALLBACK 
PrefPage4Callback(HWND hwnd, 
                  UINT msg, 
                  WPARAM wParam, 
                  LPARAM lParam)
{
	return g_pCurrentPrefWindow->PrefPage4Proc(hwnd, msg, wParam, lParam);
}          

static BOOL CALLBACK 
PrefPage5Callback(HWND hwnd, 
                  UINT msg, 
                  WPARAM wParam, 
                  LPARAM lParam)
{
	return g_pCurrentPrefWindow->PrefPage5Proc(hwnd, msg, wParam, lParam);
}      

static BOOL CALLBACK 
PrefPage6Callback(HWND hwnd, 
                  UINT msg, 
                  WPARAM wParam, 
                  LPARAM lParam)
{
	return g_pCurrentPrefWindow->PrefPage6Proc(hwnd, msg, wParam, lParam);
}             

Win32PreferenceWindow::Win32PreferenceWindow(FAContext *context,
                                             ThemeManager *pThemeMan) :
     PreferenceWindow(context, pThemeMan)
{     
	g_pCurrentPrefWindow = this;
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
    PROPSHEETPAGE psp[6];
    PROPSHEETHEADER psh;
	
    HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwndParent, GWL_HINSTANCE);

    psp[0].dwSize = sizeof(PROPSHEETPAGE);
    psp[0].dwFlags = 0;
    psp[0].hInstance = hinst;
    psp[0].pszTemplate = MAKEINTRESOURCE(IDD_PREF1);
    psp[0].pszIcon = NULL;
    psp[0].pfnDlgProc = PrefPage1Callback;
    psp[0].pszTitle = NULL;
    psp[0].lParam = (LPARAM)prefs;

    psp[1].dwSize = sizeof(PROPSHEETPAGE);
    psp[1].dwFlags = 0;
    psp[1].hInstance = hinst;
    psp[1].pszTemplate = MAKEINTRESOURCE(IDD_PREF5);
    psp[1].pszIcon = NULL;
    psp[1].pfnDlgProc = PrefPage5Callback;
    psp[1].pszTitle = NULL;
    psp[1].lParam = (LPARAM)prefs;

    psp[2].dwSize = sizeof(PROPSHEETPAGE);
    psp[2].dwFlags = 0;
    psp[2].hInstance = hinst;
    psp[2].pszTemplate = MAKEINTRESOURCE(IDD_PREF2);
    psp[2].pszIcon = NULL;
    psp[2].pfnDlgProc = PrefPage2Callback;
    psp[2].pszTitle = NULL;
    psp[2].lParam = (LPARAM)prefs;

    UpdateManager* updateManager = NULL;

    updateManager = new Win32UpdateManager(m_pContext);
    updateManager->DetermineLocalVersions();
    updateManager->SetPlatform(string("WIN32"));

    psp[3].dwSize = sizeof(PROPSHEETPAGE);
    psp[3].dwFlags = 0;
    psp[3].hInstance = hinst;
    psp[3].pszTemplate = MAKEINTRESOURCE(IDD_PREF6);
    psp[3].pszIcon = NULL;
    psp[3].pfnDlgProc = PrefPage6Callback;
    psp[3].pszTitle = NULL;
    psp[3].lParam = (LPARAM)updateManager;

    psp[4].dwSize = sizeof(PROPSHEETPAGE);
    psp[4].dwFlags = 0;
    psp[4].hInstance = hinst;
    psp[4].pszTemplate = MAKEINTRESOURCE(IDD_PREF3);
    psp[4].pszIcon = NULL;
    psp[4].pfnDlgProc = PrefPage3Callback;
    psp[4].pszTitle = NULL;
    psp[4].lParam = (LPARAM)prefs;

    psp[5].dwSize = sizeof(PROPSHEETPAGE);
    psp[5].dwFlags = 0;
    psp[5].hInstance = hinst;
    psp[5].pszTemplate = MAKEINTRESOURCE(IDD_PREF4);
    psp[5].pszIcon = NULL;
    psp[5].pfnDlgProc = PrefPage4Callback;
    psp[5].pszTitle = NULL;
    psp[5].lParam = (LPARAM)prefs;

    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_PROPSHEETPAGE;
    psh.hwndParent = hwndParent;
    psh.hInstance = hinst;
    psh.pszIcon = NULL;
    psh.pszCaption = BRANDING" Preferences";
    psh.nPages = sizeof(psp)/sizeof(PROPSHEETPAGE);
    psh.nStartPage = 0;
    psh.ppsp = psp;
    psh.pfnCallback = NULL;

    GetPrefsValues(prefs, &originalValues);

    currentValues = originalValues;

    result = (PropertySheet(&psh) > 0);

    delete updateManager;

    return result;
}

void Win32PreferenceWindow::GetPrefsValues(Preferences* prefs, 
                                           PrefsStruct* values)
{
    uint32 size = 256;

    prefs->GetDefaultPMO(values->defaultPMO, &size);
    size = 256;
    prefs->GetDefaultUI(values->defaultUI, &size);
    prefs->GetDecoderThreadPriority(&values->decoderThreadPriority);
    prefs->GetInputBufferSize(&values->inputBufferSize);
    prefs->GetOutputBufferSize(&values->outputBufferSize);
    prefs->GetPrebufferLength(&values->preBufferLength);
    prefs->GetStayOnTop(&values->stayOnTop);
    prefs->GetLiveInTray(&values->liveInTray);

    prefs->GetStreamBufferInterval(&values->streamInterval);
    prefs->GetSaveStreams(&values->saveStreams);
    size = 256;
    prefs->GetProxyServerAddress(values->proxyServer, &size);
    prefs->GetUseProxyServer(&values->useProxyServer);
    size = MAX_PATH;
    prefs->GetSaveStreamsDirectory(values->saveStreamsDirectory, &size);
    size = 16;
    prefs->GetAlternateNICAddress(values->alternateIP, &size);
    prefs->GetUseAlternateNIC(&values->useAlternateIP);

    prefs->GetUseDebugLog(&values->enableLogging);
    prefs->GetLogMain(&values->logMain);
    prefs->GetLogDecode(&values->logDecoder);
    prefs->GetLogInput(&values->logInput);
    prefs->GetLogOutput(&values->logOutput);
    prefs->GetLogPerformance(&values->logPerformance);
    
    size = 64;
    prefs->GetThemeDefaultFont(values->defaultFont, &size);
    m_pThemeMan->GetCurrentTheme(values->currentTheme);

    prefs->GetCheckForUpdates(&values->checkForUpdates);
}

void Win32PreferenceWindow::SavePrefsValues(Preferences* prefs, 
                                            PrefsStruct* values)
{
    prefs->SetDefaultPMO(values->defaultPMO);
    prefs->SetDefaultUI(values->defaultUI);
    prefs->SetDecoderThreadPriority(values->decoderThreadPriority);
    prefs->SetInputBufferSize(values->inputBufferSize);
    prefs->SetOutputBufferSize(values->outputBufferSize);
    prefs->SetPrebufferLength(values->preBufferLength);
    prefs->SetStayOnTop(values->stayOnTop);
    prefs->SetLiveInTray(values->liveInTray);

    prefs->SetStreamBufferInterval(values->streamInterval);
    prefs->SetSaveStreams(values->saveStreams);
    prefs->SetSaveStreamsDirectory(values->saveStreamsDirectory);
    prefs->SetProxyServerAddress(values->proxyServer);
    prefs->SetUseProxyServer(values->useProxyServer);
    prefs->SetAlternateNICAddress(values->alternateIP);
    prefs->SetUseAlternateNIC(values->useAlternateIP);

    prefs->SetUseDebugLog(values->enableLogging);
    prefs->SetLogMain(values->logMain);
    prefs->SetLogDecode(values->logDecoder);
    prefs->SetLogInput(values->logInput);
    prefs->SetLogOutput(values->logOutput);
    prefs->SetLogPerformance(values->logPerformance);

    prefs->SetThemeDefaultFont(values->defaultFont);
    m_pThemeMan->UseTheme(m_oThemeList[currentValues.currentTheme]);

    prefs->SetCheckForUpdates(values->checkForUpdates);
}

bool Win32PreferenceWindow::PrefPage1Proc(HWND hwnd, 
                                          UINT msg, 
                                          WPARAM wParam, 
                                          LPARAM lParam)      
{
    bool result = false;
    static PROPSHEETPAGE* psp = NULL;
    static Preferences* prefs = NULL;
    static HWND hwndUI = NULL;
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
            hwndUI = GetDlgItem(hwnd, IDC_UI);
            hwndPMO = GetDlgItem(hwnd, IDC_PMO);
            hwndPriority = GetDlgItem(hwnd, IDC_PRIORITY);
            hwndInput = GetDlgItem(hwnd, IDC_INPUT);
            hwndOutput = GetDlgItem(hwnd, IDC_OUTPUT);
            hwndPrebuffer = GetDlgItem(hwnd, IDC_PREBUFFER);
            hwndStayOnTop = GetDlgItem(hwnd, IDC_STAYONTOP);
            hwndLiveInTray = GetDlgItem(hwnd, IDC_TRAY);

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
                pos = ComboBox_AddString(hwndPMO, item->Name());

                if(!strcmp(originalValues.defaultPMO, item->Name()))
                {
                    ComboBox_SetCurSel(hwndPMO, pos);
                }
            }
            
            i = 0;

            while(item = ui.GetItem(i++))
            {
                pos = ComboBox_AddString(hwndUI, item->Name());

                if(!strcmp(originalValues.defaultUI, item->Name()))
                {
                    ComboBox_SetCurSel(hwndUI, pos);
                }
            }

            SendMessage(hwndPriority, 
                        TBM_SETRANGE, 
                        (WPARAM) TRUE,                   
                        (LPARAM) MAKELONG(0, 6));

            SendMessage(hwndPriority, 
                        TBM_SETPOS, 
                        (WPARAM) TRUE,                   
                        (LPARAM) originalValues.decoderThreadPriority);

            int32 value;
            char temp[256];

            value = originalValues.inputBufferSize;
            sprintf(temp, "%d", value);
            Edit_LimitText(hwndInput, 4);
            Edit_SetText(hwndInput, temp);

            value = originalValues.outputBufferSize;
            sprintf(temp, "%d", value);
            Edit_LimitText(hwndOutput, 4);
            Edit_SetText(hwndOutput, temp);

            value = originalValues.preBufferLength;
            sprintf(temp, "%d", value);
            Edit_LimitText(hwndPrebuffer, 2);
            Edit_SetText(hwndPrebuffer, temp);

            Button_SetCheck(hwndStayOnTop, originalValues.stayOnTop);

            Button_SetCheck(hwndLiveInTray, originalValues.liveInTray);
            
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
                        memset(currentValues.defaultPMO, 0x00, 256);
                        ComboBox_GetText(   hwndPMO, 
                                            currentValues.defaultPMO, 
                                            256);

                        if(memcmp(  &originalValues, 
                                    &currentValues, 
                                    sizeof(PrefsStruct)))
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

                case IDC_UI:
                {
                    if(HIWORD(wParam) == CBN_CLOSEUP)
                    {
                        memset(currentValues.defaultUI, 0x00, 256);
                        ComboBox_GetText(   hwndUI, 
                                            currentValues.defaultUI, 
                                            256);

                        if(memcmp(  &originalValues, 
                                    &currentValues, 
                                    sizeof(PrefsStruct)))
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

                        currentValues.inputBufferSize = atoi(text);

                        if(memcmp(  &originalValues, 
                                    &currentValues, 
                                    sizeof(PrefsStruct)))
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

                        currentValues.outputBufferSize = atoi(text);

                        if(memcmp(  &originalValues, 
                                    &currentValues, 
                                    sizeof(PrefsStruct)))
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

                        currentValues.preBufferLength = atoi(text);

                        if(memcmp(  &originalValues, 
                                    &currentValues, 
                                    sizeof(PrefsStruct)))
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

                case IDC_STAYONTOP:
                {
                    if(Button_GetCheck(hwndStayOnTop) == BST_CHECKED)
                    {
                        currentValues.stayOnTop = true;
                    }
                    else
                    {
                        currentValues.stayOnTop = false;
                    }

                    if(memcmp(  &originalValues, 
                                &currentValues, 
                                sizeof(PrefsStruct)))
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
                        currentValues.liveInTray = true;
                    }
                    else
                    {
                        currentValues.liveInTray = false;
                    }

                    if(memcmp(  &originalValues, 
                                &currentValues, 
                                sizeof(PrefsStruct)))
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

                        currentValues.decoderThreadPriority = position;

                        if(memcmp(  &originalValues, 
                                    &currentValues, 
                                    sizeof(PrefsStruct)))
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
                case PSN_SETACTIVE:
                {
                    
                    break;
                }

                case PSN_APPLY:
                {
                    SavePrefsValues(prefs, &currentValues);
                    break;
                }

                case PSN_KILLACTIVE:
                {
                    
                    break;
                }

                case PSN_RESET:
                {
                    SavePrefsValues(prefs, &originalValues);
                    break;
                }
            }

            break;
        }
    }

    return result;
}

bool Win32PreferenceWindow::PrefPage2Proc(HWND hwnd, 
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

            value = originalValues.streamInterval;
            sprintf(temp, "%d", value);
            Edit_LimitText(hwndStreamInterval, 2);
            Edit_SetText(hwndStreamInterval, temp);
            
            Button_SetCheck(hwndSaveStreams, originalValues.saveStreams);


            Edit_SetText(   hwndSaveStreamsDirectory, 
                            originalValues.saveStreamsDirectory);

            Button_Enable(  hwndSaveStreamsDirectory, 
                            originalValues.saveStreams);

            Button_Enable(  hwndBrowse, 
                            originalValues.saveStreams);

            Button_Enable(  hwndSaveLocationText,
                            originalValues.saveStreams);

            char* port = NULL;

            strcpy(temp, originalValues.proxyServer);
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


            Button_SetCheck(hwndUseProxyServer, originalValues.useProxyServer);


            Button_Enable(  hwndProxyServerAddress, 
                            originalValues.useProxyServer);

            Button_Enable(  hwndProxyServerAddressText, 
                            originalValues.useProxyServer);

            Button_Enable(  hwndProxyServerPort,
                            originalValues.useProxyServer);

            Button_Enable(  hwndProxyServerPortText, 
                            originalValues.useProxyServer);

            Button_Enable(  hwndColon, 
                            originalValues.useProxyServer);

            char* dot = NULL;
            char* ip[4];
            int32 i = 1;

            strcpy(temp, originalValues.alternateIP);
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

            Button_SetCheck(hwndUseAlternateIP, originalValues.useAlternateIP);


            Button_Enable(  hwndUseAlternateIPText, 
                            originalValues.useAlternateIP);

            Button_Enable(  hwndAlternateIPAddress1,
                            originalValues.useAlternateIP);

            Button_Enable(  hwndPeriod1, 
                            originalValues.useAlternateIP);

            Button_Enable(  hwndAlternateIPAddress2, 
                            originalValues.useAlternateIP);

            Button_Enable(  hwndPeriod2, 
                            originalValues.useAlternateIP);

            Button_Enable(  hwndAlternateIPAddress3, 
                            originalValues.useAlternateIP);

            Button_Enable(  hwndPeriod3, 
                            originalValues.useAlternateIP);

            Button_Enable(  hwndAlternateIPAddress4, 
                            originalValues.useAlternateIP);

            
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

                        currentValues.streamInterval = atoi(text);

                        if(memcmp(  &originalValues, 
                                    &currentValues, 
                                    sizeof(PrefsStruct)))
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
                        currentValues.saveStreams = true;
                    }
                    else
                    {
                        currentValues.saveStreams = false;
                    }

                    enabled = (currentValues.saveStreams ? TRUE : FALSE);

                    Button_Enable(hwndSaveStreamsDirectory, enabled); 
                    Button_Enable(hwndBrowse, enabled);
                    Button_Enable(  hwndSaveLocationText,enabled);


                    if(memcmp(  &originalValues, 
                                &currentValues, 
                                sizeof(PrefsStruct)))
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
                        memset(currentValues.saveStreamsDirectory, 0x00, MAX_PATH);
                        Edit_GetText(   hwndSaveStreamsDirectory, 
                                        currentValues.saveStreamsDirectory,
                                        MAX_PATH);

                        if(memcmp(  &originalValues, 
                                    &currentValues, 
                                    sizeof(PrefsStruct)))
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
                            memset( currentValues.saveStreamsDirectory, 
                                    0x00, 
                                    MAX_PATH);

                            SHGetPathFromIDList(browseId,
                                                currentValues.saveStreamsDirectory);
                            
                            Edit_SetText(   hwndSaveStreamsDirectory, 
                                            currentValues.saveStreamsDirectory);

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
                        currentValues.useProxyServer = true;
                    }
                    else
                    {
                        currentValues.useProxyServer = false;
                    }

                    enabled = (currentValues.useProxyServer ? TRUE : FALSE);

                    Button_Enable(hwndProxyServerAddress, enabled);

                    Button_Enable(hwndProxyServerAddressText, enabled);

                    Button_Enable(hwndProxyServerPort, enabled);

                    Button_Enable(hwndProxyServerPortText, enabled);

                    Button_Enable(hwndColon, enabled);


                    if(memcmp(  &originalValues, 
                                &currentValues, 
                                sizeof(PrefsStruct)))
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
                        char port[6];

                        memset(currentValues.proxyServer, 0x00, MAX_PATH);
                        Edit_GetText(   hwndProxyServerAddress, 
                                        currentValues.proxyServer,
                                        MAX_PATH);

                        Edit_GetText(   hwndProxyServerPort, 
                                        port,
                                        sizeof(port));

                        if(*port)
                        {
                            strcat(currentValues.proxyServer, ":");
                            strcat(currentValues.proxyServer, port);
                        }

                        if(memcmp(  &originalValues, 
                                    &currentValues, 
                                    sizeof(PrefsStruct)))
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
                        currentValues.useAlternateIP = true;
                    }
                    else
                    {
                        currentValues.useAlternateIP = false;
                    }

                    enabled = (currentValues.useAlternateIP ? TRUE : FALSE);

                    Button_Enable(hwndUseAlternateIPText, enabled);
                    Button_Enable(hwndAlternateIPAddress1, enabled);
                    Button_Enable(hwndPeriod1, enabled);
                    Button_Enable(hwndAlternateIPAddress2, enabled);
                    Button_Enable(hwndPeriod2, enabled);
                    Button_Enable(hwndAlternateIPAddress3, enabled);
                    Button_Enable(hwndPeriod3, enabled);
                    Button_Enable(hwndAlternateIPAddress4, enabled);
                    


                    if(memcmp(  &originalValues, 
                                &currentValues, 
                                sizeof(PrefsStruct)))
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

                        memset(currentValues.alternateIP, 0x00, 16);
                        Edit_GetText(hwndAlternateIPAddress1, ip, 4);

                        if(*ip)
                        {
                            strcat(currentValues.alternateIP, ip);
                        }
                        else
                        {
                            strcat(currentValues.alternateIP, "0");
                        }

                        strcat(currentValues.alternateIP, ".");


                        Edit_GetText(hwndAlternateIPAddress2, ip, 4);

                        if(*ip)
                        {
                            strcat(currentValues.alternateIP, ip);
                        }
                        else
                        {
                            strcat(currentValues.alternateIP, "0");
                        }

                        strcat(currentValues.alternateIP, ".");

                        Edit_GetText(hwndAlternateIPAddress3, ip, 4);

                        if(*ip)
                        {
                            strcat(currentValues.alternateIP, ip);
                        }
                        else
                        {
                            strcat(currentValues.alternateIP, "0");
                        }

                        strcat(currentValues.alternateIP, ".");

                        Edit_GetText(hwndAlternateIPAddress4, ip, 4);

                        if(*ip)
                        {
                            strcat(currentValues.alternateIP, ip);
                        }
                        else
                        {
                            strcat(currentValues.alternateIP, "0");
                        }

                        if(memcmp(  &originalValues, 
                                    &currentValues, 
                                    sizeof(PrefsStruct)))
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
                case PSN_SETACTIVE:
                {
                    
                    break;
                }

                case PSN_APPLY:
                {
                    SavePrefsValues(prefs, &currentValues);
                    break;
                }

                case PSN_KILLACTIVE:
                {
                    
                    break;
                }

                case PSN_RESET:
                {
                    SavePrefsValues(prefs, &originalValues);
                    break;
                }
            }

            break;
        }
    }

    return result;
}

bool Win32PreferenceWindow::PrefPage3Proc(HWND hwnd, 
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

            value = originalValues.enableLogging;

            Button_SetCheck(hwndLog, value); 
            Button_Enable(hwndLogDecoder, value); 
            Button_Enable(hwndLogInput, value);
            Button_Enable(hwndLogOutput, value);
            Button_Enable(hwndLogMain, value);
            Button_Enable(hwndLogPerformance, value);

            Button_SetCheck(hwndLogMain, originalValues.logMain); 
            Button_SetCheck(hwndLogDecoder, originalValues.logDecoder); 
            Button_SetCheck(hwndLogInput, originalValues.logInput); 
            Button_SetCheck(hwndLogOutput, originalValues.logOutput); 
            Button_SetCheck(hwndLogPerformance, originalValues.logPerformance); 
                     
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
                        currentValues.enableLogging = true;
                       
                    }
                    else
                    {
                        currentValues.enableLogging = false;
                    }

                    enabled = (currentValues.enableLogging ? TRUE : FALSE);

                    Button_Enable(hwndLogDecoder, enabled); 
                    Button_Enable(hwndLogInput, enabled);
                    Button_Enable(hwndLogOutput, enabled);
                    Button_Enable(hwndLogMain, enabled);
                    Button_Enable(hwndLogPerformance, enabled);

                    if(memcmp(  &originalValues, 
                                &currentValues, 
                                sizeof(PrefsStruct)))
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
                        currentValues.logDecoder = true;
                    }
                    else
                    {
                        currentValues.logDecoder = false;
                    }

                    if(memcmp(  &originalValues, 
                                &currentValues, 
                                sizeof(PrefsStruct)))
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
                        currentValues.logInput = true;
                    }
                    else
                    {
                        currentValues.logInput = false;
                    }

                    if(memcmp(  &originalValues, 
                                &currentValues, 
                                sizeof(PrefsStruct)))
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
                        currentValues.logOutput = true;
                    }
                    else
                    {
                        currentValues.logOutput = false;
                    }

                    if(memcmp(  &originalValues, 
                                &currentValues, 
                                sizeof(PrefsStruct)))
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
                        currentValues.logPerformance = true;
                    }
                    else
                    {
                        currentValues.logPerformance = false;
                    }

                    if(memcmp(  &originalValues, 
                                &currentValues, 
                                sizeof(PrefsStruct)))
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
                        currentValues.logMain = true;
                    }
                    else
                    {
                        currentValues.logMain = false;
                    }

                    if(memcmp(  &originalValues, 
                                &currentValues, 
                                sizeof(PrefsStruct)))
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
                    SavePrefsValues(prefs, &currentValues);
                    break;
                }

                case PSN_KILLACTIVE:
                {
                    
                    break;
                }

                case PSN_RESET:
                {
                    SavePrefsValues(prefs, &originalValues);
                    break;
                }
            }

            break;
        }
    }

    return result;
}

bool Win32PreferenceWindow::PrefPage4Proc(HWND hwnd, 
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
                case PSN_SETACTIVE:
                {
                    
                    break;
                }

                case PSN_APPLY:
                {
                    SavePrefsValues(prefs, &currentValues);
                    break;
                }

                case PSN_KILLACTIVE:
                {
                    
                    break;
                }

                case PSN_RESET:
                {
                    SavePrefsValues(prefs, &originalValues);
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
	int                           iLoop;
    map<string, string>::iterator i;

	currentValues.listboxIndex = -1;
	m_pThemeMan->GetCurrentTheme(currentValues.currentTheme);

    m_oThemeList.clear();    
    SendDlgItemMessage(hwnd, IDC_THEMELISTBOX, LB_RESETCONTENT, 0, 0);

    m_pThemeMan->GetThemeList(m_oThemeList);
    for(iLoop = 0, i = m_oThemeList.begin(); 
        i != m_oThemeList.end(); i++, iLoop++)
    {
    	SendDlgItemMessage(hwnd, IDC_THEMELISTBOX, LB_ADDSTRING,
                           0, (LPARAM)(*i).first.c_str());
        if ((*i).first == currentValues.currentTheme)
            currentValues.listboxIndex = iLoop;
    }                      
    
    if (currentValues.listboxIndex >= 0)
    	SendDlgItemMessage(hwnd, IDC_THEMELISTBOX, LB_SETCURSEL, 
                           currentValues.listboxIndex, 0);
     
    EnableWindow(GetDlgItem(hwnd, IDC_DELETETHEME), 0);
    currentValues.fontChanged = false;
}

bool Win32PreferenceWindow::PrefPage5Proc(HWND hwnd, 
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
            break;
        }

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
				case IDC_THEMELISTBOX:
                {
                	int    iIndex;
                    char   szCurSel[256];
                    
                    iIndex = SendDlgItemMessage(hwnd, IDC_THEMELISTBOX, LB_GETCURSEL, 0, 0);
                    if (iIndex >= 0)
                    {
                        SendDlgItemMessage(hwnd, IDC_THEMELISTBOX, 
                                           LB_GETTEXT, iIndex, 
                                           (LPARAM)szCurSel);
                        if (strcasecmp(szCurSel, "freeamp"))                   
                           EnableWindow(GetDlgItem(hwnd, IDC_DELETETHEME), 1);
                        else   
                           EnableWindow(GetDlgItem(hwnd, IDC_DELETETHEME), 0);
                    }    
                    currentValues.currentTheme = string(szCurSel);
                    if (iIndex != currentValues.listboxIndex || 
                        currentValues.fontChanged)
                        PropSheet_Changed(GetParent(hwnd), hwnd);
                    else
                        PropSheet_UnChanged(GetParent(hwnd), hwnd);
                
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
                    if (MessageBox(hwnd, oMesg.c_str(), "Confirm Delete", MB_YESNO) == IDYES)        
                    {
                        eRet = m_pThemeMan->DeleteTheme(m_oThemeList[oThemeName]);
                        if (IsError(eRet))
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
                    
                    strcpy(sLogFont.lfFaceName, currentValues.defaultFont);
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
                    
                    if (ChooseFont(&sFont))
                    {
                        memset(currentValues.defaultFont, 0x00, 64);
                    	strcpy(currentValues.defaultFont, 
                               sLogFont.lfFaceName);
                        currentValues.fontChanged = true;
                        PropSheet_Changed(GetParent(hwnd), hwnd);
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
                    SavePrefsValues(prefs, &currentValues);
                    LoadThemeListBox(hwnd);
                    m_pContext->target->AcceptEvent(
                         new Event(INFO_PrefsChanged));
                    
                    break;
                }

                case PSN_KILLACTIVE:
                {
                    
                    break;
                }

                case PSN_RESET:
                {
                    SavePrefsValues(prefs, &originalValues);
                    m_pContext->target->AcceptEvent(
                         new Event(INFO_PrefsChanged));
                         
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
    UpdateManager* um;
    HWND hwndList;
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

    return true;
}

static void check_function(void* arg)
{
    ThreadStruct* ts = (ThreadStruct*)arg;

    ts->um->RetrieveLatestVersionInfo(callback_function, ts);

    ListView_RedrawItems(ts->hwndList, 0, ListView_GetItemCount(ts->hwndList) - 1);

    delete ts->thread;
    delete ts;
}

static void update_function(void* arg)
{
    ThreadStruct* ts = (ThreadStruct*)arg;

    ts->um->RetrieveLatestVersionInfo();

    ListView_RedrawItems(ts->hwndList, 0, ListView_GetItemCount(ts->hwndList) - 1);

    ts->um->UpdateComponents(callback_function, ts);

    delete ts->thread;
    delete ts;
}

bool Win32PreferenceWindow::PrefPage6Proc(HWND hwnd, 
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
                        ThreadStruct* ts = new ThreadStruct;

                        ts->thread = thread;
                        ts->um = um;
                        ts->hwndList = hwndList;

                        thread->Create(update_function, ts);
                    }
                	break;
                }

                case IDC_CHECK:
                {
                    Thread* thread = Thread::CreateThread();

                    if(thread)
                    {
                        ThreadStruct* ts = new ThreadStruct;

                        ts->thread = thread;
                        ts->um = um;
                        ts->hwndList = hwndList;

                        thread->Create(check_function, ts);
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
                    case PSN_SETACTIVE:
                    {
                    
                        break;
                    }

                    case PSN_APPLY:
                    {
                        SavePrefsValues(prefs, &currentValues);
                        break;
                    }

                    case PSN_KILLACTIVE:
                    {
                    
                        break;
                    }

                    case PSN_RESET:
                    {
                        SavePrefsValues(prefs, &originalValues);
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

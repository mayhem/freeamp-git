/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998 GoodNoise

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
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdlib.h>
#include <assert.h>

/* project headers */
#include "config.h"
#include "prefdialog.h"
#include "preferences.h"
#include "registrar.h"
#include "resource.h"

typedef struct Page1Values {
    char defaultUI[256];
    char defaultPMO[256];
}Page1Values;

static
BOOL 
CALLBACK 
PrefPage1Proc(  HWND hwnd, 
                UINT msg, 
                WPARAM wParam, 
                LPARAM lParam)
{
    UINT result = 0;
    static PROPSHEETPAGE* psp = NULL;
    static Preferences* prefs = NULL;
    static HWND hwndUI = NULL;
    static HWND hwndPMO = NULL;
    static HWND hwndPriority = NULL;
    static HWND hwndInput = NULL;
    static HWND hwndOutput = NULL;
    static Page1Values originalValues;
    static Page1Values currentValues;

    switch(msg)
    {
        case WM_INITDIALOG:
        {
            memset(&originalValues, 0x00, sizeof(Page1Values));
            memset(&currentValues, 0x00, sizeof(Page1Values));

            // remember these for later...
            psp = (PROPSHEETPAGE*)lParam;
            prefs = (Preferences*)psp->lParam;

            // get the handles to all our controls
            hwndUI = GetDlgItem(hwnd, IDC_UI);
            hwndPMO = GetDlgItem(hwnd, IDC_PMO);
            hwndPriority = GetDlgItem(hwnd, IDC_PRIORITY);
            hwndInput = GetDlgItem(hwnd, IDC_INPUT);
            hwndOutput = GetDlgItem(hwnd, IDC_OUTPUT);

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
            RegistryItem *item;

            char defaultItem[256];
            uint32 size = sizeof(defaultItem);

            prefs->GetDefaultPMO(defaultItem, &size);
            strcpy(originalValues.defaultPMO, defaultItem);

            while(item = pmo.GetItem(i++))
            {
                ComboBox_AddString(hwndPMO, item->Name());

                if(!strcmp(defaultItem, item->Name()))
                {
                    ComboBox_SetCurSel(hwndPMO, i-1);
                }
            }
            
            i = 0;
            prefs->GetDefaultUI(defaultItem, &size);
            strcpy(originalValues.defaultUI, defaultItem);

            while(item = ui.GetItem(i++))
            {
                ComboBox_AddString(hwndUI, item->Name());

                if(!strcmp(defaultItem, item->Name()))
                {
                    ComboBox_SetCurSel(hwndUI, i-1);
                }
            }
        
            currentValues = originalValues;
            
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
                        ComboBox_GetText(hwndPMO, currentValues.defaultPMO, 256);

                        if(memcmp(&originalValues, &currentValues, sizeof(Page1Values)))
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
                        ComboBox_GetText(hwndUI, currentValues.defaultUI, 256);

                        if(memcmp(&originalValues, &currentValues, sizeof(Page1Values)))
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
                    
                    break;
                }

                case PSN_KILLACTIVE:
                {
                    
                    break;
                }

                case PSN_RESET:
                {
                    
                    break;
                }
            }

            break;
        }
    }

    return result;
}

bool DisplayPreferences(HWND hwndParent)
{
    bool result = false;
    PROPSHEETPAGE psp[1];
    PROPSHEETHEADER psh;
    HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwndParent, GWL_HINSTANCE);
    Preferences prefs;

    psp[0].dwSize = sizeof(PROPSHEETPAGE);
    psp[0].dwFlags = 0;
    psp[0].hInstance = hinst;
    psp[0].pszTemplate = MAKEINTRESOURCE(IDD_PREF1);
    psp[0].pszIcon = NULL;
    psp[0].pfnDlgProc = PrefPage1Proc;
    psp[0].pszTitle = "";
    psp[0].lParam = (LPARAM)&prefs;

    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_PROPSHEETPAGE;
    psh.hwndParent = hwndParent;
    psh.hInstance = hinst;
    psh.pszIcon = NULL;
    psh.pszCaption = "FreeAmp Preferences";
    psh.nPages = sizeof(psp)/sizeof(PROPSHEETPAGE);
    psh.ppsp = psp;

    result = (PropertySheet(&psh) > 0);

    return result;
}
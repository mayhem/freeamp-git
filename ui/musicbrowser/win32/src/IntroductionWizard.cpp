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

// system includes
#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <shlobj.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>

// project includes
#include "config.h"
#include "resource.h"
#include "Win32MusicBrowser.h"


static BOOL CALLBACK IntroWizardHello(HWND hwnd, 
                                      UINT msg, 
                                      WPARAM wParam, 
                                      LPARAM lParam)
{
    BOOL result = FALSE;

    switch(msg)
    {
        case WM_INITDIALOG:
        {
            break;
        }

        case WM_DRAWITEM:
        {
            DRAWITEMSTRUCT* dis = (DRAWITEMSTRUCT*)lParam;
            UINT ctrlId = wParam;
            const char* kCaption = "Welcome to "BRANDING"!";
            const char* kText = "This wizard will walk you though a quick "
                                "tutorial to help you begin organizing your "
                                "music collection and get you started playing "
                                "your music.\r\n"
                                "\r\n"
                                "Topics covered:\r\n\r\n"
                                "* How does "BRANDING" help you to organize your "
                                "music?\r\n\r\n"
                                "* How do you choose what music you want to "
                                "listen to?\r\n\r\n"
                                "* Where can you go to find more music?\r\n\r\n"
                                "* How does "BRANDING" help you download music?"
                                "\r\n"
                                "\r\n"
                                "Finally, the wizard will allow you to search "
                                "your computer for music you already own "
                                "and organize it for you within "BRANDING".";


            switch(ctrlId)
            {
                case IDC_CAPTION:
                {
                    HFONT font, oldFont;

                    LOGFONT lf;

                    GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);

                    lf.lfWeight = FW_BOLD;

                    font = CreateFontIndirect(&lf);

                    oldFont = (HFONT)SelectObject(dis->hDC, font);

                    RECT clientRect;
                    GetClientRect(dis->hwndItem, &clientRect);

                    DrawText(dis->hDC, 
                             kCaption,
                             strlen(kCaption),
                             &clientRect,
                             DT_LEFT|DT_WORDBREAK);

                    SelectObject(dis->hDC, oldFont);

                    DeleteObject(font);

                    break;
                } 

                case IDC_TEXT:
                {
                    HFONT font, oldFont;

                    font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

                    oldFont = (HFONT)SelectObject(dis->hDC, font);

                    RECT clientRect;
                    GetClientRect(dis->hwndItem, &clientRect);

                    DrawText(dis->hDC, 
                             kText,
                             strlen(kText),
                             &clientRect,
                             DT_LEFT|DT_WORDBREAK);

                    SelectObject(dis->hDC, oldFont);

                    DeleteObject(font);
                    break;
                }
            }

            break;
        }

        case WM_NOTIFY:
        {
            switch(((NMHDR*)lParam)->code)
            {
                case PSN_KILLACTIVE:
                {
                    SetWindowLong(hwnd,	DWL_MSGRESULT, FALSE);
                    result = TRUE;
                    break;
                }

                case PSN_RESET:
                {
                    SetWindowLong(hwnd,	DWL_MSGRESULT, FALSE);
                    break;
                }

                case PSN_SETACTIVE:
                {
                    PropSheet_SetWizButtons(GetParent(hwnd), PSWIZB_NEXT);
                    break;
                }

                case PSN_WIZNEXT:
                {
                    break;
                }

                case PSN_WIZBACK:
                {
                    break;
                }
            }

            break;
        }

    }

	return result;
}   

static BOOL CALLBACK IntroWizardBrowser(HWND hwnd, 
                                        UINT msg, 
                                        WPARAM wParam, 
                                        LPARAM lParam)
{
	BOOL result = FALSE;

    switch(msg)
    {
        case WM_INITDIALOG:
        {
            break;
        }

        case WM_DRAWITEM:
        {
            DRAWITEMSTRUCT* dis = (DRAWITEMSTRUCT*)lParam;
            UINT ctrlId = wParam;
            const char* kCaption = "What is My Music?";
            const char* kText = "My Music is a window that includes two separate "
                                "panes. The pane on the left is titled My Music "
                                "Collection.";

            switch(ctrlId)
            {
                case IDC_CAPTION:
                {
                    HFONT font, oldFont;

                    LOGFONT lf;

                    GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);

                    lf.lfWeight = FW_BOLD;

                    font = CreateFontIndirect(&lf);

                    oldFont = (HFONT)SelectObject(dis->hDC, font);

                    RECT clientRect;
                    GetClientRect(dis->hwndItem, &clientRect);

                    DrawText(dis->hDC, 
                             kCaption,
                             strlen(kCaption),
                             &clientRect,
                             DT_LEFT|DT_WORDBREAK);

                    SelectObject(dis->hDC, oldFont);

                    DeleteObject(font);

                    break;
                } 

                case IDC_TEXT:
                {
                    HFONT font, oldFont;

                    font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

                    oldFont = (HFONT)SelectObject(dis->hDC, font);

                    RECT clientRect;
                    GetClientRect(dis->hwndItem, &clientRect);

                    DrawText(dis->hDC, 
                             kText,
                             strlen(kText),
                             &clientRect,
                             DT_LEFT|DT_WORDBREAK);

                    SelectObject(dis->hDC, oldFont);

                    DeleteObject(font);
                    break;
                }
            }

            break;
        }

        case WM_NOTIFY:
        {
            switch(((NMHDR*)lParam)->code)
            {
                case PSN_KILLACTIVE:
                {
                    SetWindowLong(hwnd,	DWL_MSGRESULT, FALSE);
                    result = TRUE;
                    break;
                }

                case PSN_RESET:
                {
                    SetWindowLong(hwnd,	DWL_MSGRESULT, FALSE);
                    break;
                }

                case PSN_SETACTIVE:
                {
                    PropSheet_SetWizButtons(GetParent(hwnd), PSWIZB_BACK | PSWIZB_NEXT);
                    break;
                }

                case PSN_WIZNEXT:
                {
                    break;
                }

                case PSN_WIZBACK:
                {
                    break;
                }
            }

            break;
        }

    }

	return result;
}   

static BOOL CALLBACK IntroWizardPlaylist(HWND hwnd, 
                                         UINT msg, 
                                         WPARAM wParam, 
                                         LPARAM lParam)
{
	BOOL result = FALSE;

    switch(msg)
    {
        case WM_INITDIALOG:
        {
            break;
        }

        case WM_NOTIFY:
        {
            switch(((NMHDR*)lParam)->code)
            {
                case PSN_KILLACTIVE:
                {
                    SetWindowLong(hwnd,	DWL_MSGRESULT, FALSE);
                    result = TRUE;
                    break;
                }

                case PSN_RESET:
                {
                    SetWindowLong(hwnd,	DWL_MSGRESULT, FALSE);
                    break;
                }

                case PSN_SETACTIVE:
                {
                    PropSheet_SetWizButtons(GetParent(hwnd), PSWIZB_BACK | PSWIZB_NEXT);
                    break;
                }

                case PSN_WIZNEXT:
                {
                    break;
                }

                case PSN_WIZBACK:
                {
                    break;
                }
            }

            break;
        }

    }

	return result;
}   

static BOOL CALLBACK IntroWizardSearch(HWND hwnd, 
                                       UINT msg, 
                                       WPARAM wParam, 
                                       LPARAM lParam)
{
	BOOL result = FALSE;

    switch(msg)
    {
        case WM_INITDIALOG:
        {
            break;
        }

        case WM_NOTIFY:
        {
            switch(((NMHDR*)lParam)->code)
            {
                case PSN_KILLACTIVE:
                {
                    SetWindowLong(hwnd,	DWL_MSGRESULT, FALSE);
                    result = TRUE;
                    break;
                }

                case PSN_RESET:
                {
                    SetWindowLong(hwnd,	DWL_MSGRESULT, FALSE);
                    break;
                }

                case PSN_SETACTIVE:
                {
                    PropSheet_SetWizButtons(GetParent(hwnd), PSWIZB_BACK | PSWIZB_FINISH);
                    break;
                }

                case PSN_WIZNEXT:
                {
                    break;
                }

                case PSN_WIZBACK:
                {
                    break;
                }
            }

            break;
        }

    }

	return result;
}      

bool MusicBrowserUI::IntroductionWizard()
{
    PROPSHEETPAGE psp[4];
    PROPSHEETHEADER psh;

    HINSTANCE hinst = (HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE);

    psp[0].dwSize = sizeof(PROPSHEETPAGE);
    psp[0].dwFlags = 0;
    psp[0].hInstance = hinst;
    psp[0].pszTemplate = MAKEINTRESOURCE(IDD_INTROWIZARD_HELLO);
    psp[0].pszIcon = NULL;
    psp[0].pfnDlgProc = IntroWizardHello;
    psp[0].pszTitle = NULL;
    psp[0].lParam = (LPARAM)0;

    psp[1].dwSize = sizeof(PROPSHEETPAGE);
    psp[1].dwFlags = 0;
    psp[1].hInstance = hinst;
    psp[1].pszTemplate = MAKEINTRESOURCE(IDD_INTROWIZARD_BROWSER);
    psp[1].pszIcon = NULL;
    psp[1].pfnDlgProc = IntroWizardBrowser;
    psp[1].pszTitle = NULL;
    psp[1].lParam = (LPARAM)0;

    psp[2].dwSize = sizeof(PROPSHEETPAGE);
    psp[2].dwFlags = 0;
    psp[2].hInstance = hinst;
    psp[2].pszTemplate = MAKEINTRESOURCE(IDD_INTROWIZARD_PLAYLIST);
    psp[2].pszIcon = NULL;
    psp[2].pfnDlgProc = IntroWizardPlaylist;
    psp[2].pszTitle = NULL;
    psp[2].lParam = (LPARAM)0;

    psp[3].dwSize = sizeof(PROPSHEETPAGE);
    psp[3].dwFlags = 0;
    psp[3].hInstance = hinst;
    psp[3].pszTemplate = MAKEINTRESOURCE(IDD_INTROWIZARD_SEARCH);
    psp[3].pszIcon = NULL;
    psp[3].pfnDlgProc = IntroWizardSearch;
    psp[3].pszTitle = NULL;
    psp[3].lParam = (LPARAM)0;

    /*psp[4].dwSize = sizeof(PROPSHEETPAGE);
    psp[4].dwFlags = 0;
    psp[4].hInstance = hinst;
    psp[4].pszTemplate = MAKEINTRESOURCE(IDD_INTROWIZARD_DONE);
    psp[4].pszIcon = NULL;
    psp[4].pfnDlgProc = IntroWizardSearch;
    psp[4].pszTitle = NULL;
    psp[4].lParam = (LPARAM)0;*/

    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_PROPSHEETPAGE | PSH_WIZARD | PSH_NOAPPLYNOW;
    psh.hwndParent = m_hWnd;
    psh.hInstance = hinst;
    psh.pszIcon = NULL;
    psh.pszCaption = "Welcome to "BRANDING;
    psh.nPages = sizeof(psp)/sizeof(PROPSHEETPAGE);
    psh.nStartPage = 0;
    psh.ppsp = psp;
    psh.pfnCallback = NULL;

    return (PropertySheet(&psh) > 0);
}
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
#include "utility.h"
#include "resource.h"
#include "Win32MusicBrowser.h"
#include "help.h"

BOOL CALLBACK MusicSearchDlgProc(HWND hwnd, 
                                 UINT msg, 
                                 WPARAM wParam, 
                                 LPARAM lParam )
{
    MusicBrowserUI* ui = (MusicBrowserUI*)GetWindowLong(hwnd, GWL_USERDATA);

    switch (msg)
    {
        case WM_INITDIALOG:
        {
            ui = (MusicBrowserUI*)lParam;
            assert(ui != NULL);
            SetWindowLong(hwnd, GWL_USERDATA, (LONG)ui);
            break;
        }            
    }

    return ui->MusicSearchDlgProc(hwnd, msg, wParam, lParam);
}        

static int CALLBACK BrowseCallbackProc(HWND hwnd, 
                                       UINT uMsg, 
                                       LPARAM lParam,
                                       LPARAM lpData)
{
        // Called just after the dialog is initialized
        // Select the dir passed in BROWSEINFO.lParam
        // TAKE CARE THAT IF THE DIR STRING ENDS WITH \ IT WILL NOT WORK ???
        if (uMsg == BFFM_INITIALIZED)
                ::SendMessage(hwnd, BFFM_SETSELECTION, FALSE, lpData);

        return 0;
}

const char* kAllDrives = "All Drives";
const char* kAllFolders = "All Folders";

BOOL MusicBrowserUI::MusicSearchDlgProc(HWND hwnd, 
                                        UINT msg, 
                                        WPARAM wParam, 
                                        LPARAM lParam )
{
    BOOL result = FALSE;

    switch (msg)
    {
        case WM_INITDIALOG:
        {
            HWND hwndDrives = GetDlgItem(hwnd, IDC_DRIVES);
            HWND hwndDirectory = GetDlgItem(hwnd, IDC_DIRECTORY);
            DWORD  dwDrives;
            char   *szDrive = "X:";
            int32  i, ret;

            i = ComboBox_AddString(hwndDrives, kAllDrives);
            ComboBox_SetCurSel(hwndDrives, i);

            dwDrives = GetLogicalDrives();
            for(i = 0; i < sizeof(DWORD) * 8; i++)
            {
               if (dwDrives & (1 << i))
               {
                  szDrive[0] = 'A' + i;
                  ret = GetDriveType(szDrive);
                  if (ret != DRIVE_CDROM && ret != DRIVE_REMOVABLE)
                  {
                      ComboBox_AddString(hwndDrives, szDrive);
                  }
               }   
            }

            Edit_SetText(hwndDirectory, kAllFolders);

            break;
        }      

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDHELP:
                {
                    string            oHelpFile;
                    char              dir[MAX_PATH];
                    uint32            len = sizeof(dir);

                    m_context->prefs->GetInstallDirectory(dir, &len);
                    oHelpFile = string(dir);
                    oHelpFile += string("\\");
                    oHelpFile += string(HELP_FILE);

                    //WinHelp(m_hWnd, oHelpFile.c_str(), HELP_FINDER, 0);
                    WinHelp(m_hWnd, oHelpFile.c_str(), HELP_CONTEXT, Music_Browser_Drive_Selection);
                    break;
                }

                case IDCANCEL:
                    EndDialog(hwnd, FALSE);
                    break;

                case IDOK:
                {
                    HWND hwndDrives = GetDlgItem(hwnd, IDC_DRIVES);
                    HWND hwndDirectory = GetDlgItem(hwnd, IDC_DIRECTORY);
                    char temp[MAX_PATH];

                    ComboBox_GetText(hwndDrives, 
                                     temp, 
                                     MAX_PATH);

                    BOOL allDrives = !strcmp(temp, kAllDrives);

                    if(allDrives)
                    {
                        DWORD  dwDrives;
                        char   *szDrive = "X:\\";
                        int32  i, ret;

                        dwDrives = GetLogicalDrives();
                        for(i = 0; i < sizeof(DWORD) * 8; i++)
                        {
                           if (dwDrives & (1 << i))
                           {
                              szDrive[0] = 'A' + i;
                              ret = GetDriveType(szDrive);
                              if (ret != DRIVE_CDROM && ret != DRIVE_REMOVABLE)
                              {
                                  m_searchPathList.push_back(szDrive);
                              }
                           }   
                        }
                    }
                    else
                    {
                        Edit_GetText(hwndDirectory, 
                                     temp,
                                     MAX_PATH);

                        m_searchPathList.push_back(temp);
                    }

                    EndDialog(hwnd, TRUE);
                    break;
                }

                case IDC_DRIVES:
                {
                    if(HIWORD(wParam) == CBN_CLOSEUP)
                    {
                        HWND hwndCombo = (HWND) lParam;
                        char temp[MAX_PATH];
                        int32 sel = ComboBox_GetCurSel(hwndCombo);

                        ComboBox_GetText( hwndCombo, 
                                          temp, 
                                          MAX_PATH);

                        HWND hwndDirectory = GetDlgItem(hwnd, IDC_DIRECTORY);
                        HWND hwndBrowse = GetDlgItem(hwnd, IDC_BROWSE);
                        
                        BOOL enable = strcmp(temp, kAllDrives);

                        if(!enable)
                            strcpy(temp, kAllFolders);
                        else
                            sprintf(temp, "%s\\", temp);
                            
                        Edit_SetText(hwndDirectory, temp);

                        //EnableWindow(hwndText, enable);
                        //EnableWindow(hwndDirectory, enable);
                        EnableWindow(hwndBrowse, enable);
                    }

                    break;
                }          

                case IDC_DIRECTORY:
                {
                    /*if(HIWORD(wParam) == EN_CHANGE)
                    {
                        char temp[MAX_PATH];
                        HWND hwndEdit = (HWND) lParam;

                        Edit_GetText( hwndEdit, 
                                      temp,
                                      MAX_PATH);
                    }*/

                    break;
                }

                case IDC_BROWSE:
                {
                    LPMALLOC pMalloc;

                    if(SUCCEEDED(SHGetMalloc(&pMalloc)))
                    {
                        HWND hwndDrives = GetDlgItem(hwnd, IDC_DRIVES);
                        HWND hwndDirectory = GetDlgItem(hwnd, IDC_DIRECTORY);
                        char temp[MAX_PATH];

                        LPITEMIDLIST pidlDrive;

                        // get the PIDL for this dir and set the root
                        LPSHELLFOLDER desktop;

                        if(SUCCEEDED(SHGetDesktopFolder(&desktop)))
                        {
                            USHORT drive[MAX_PATH];
                            USHORT path[MAX_PATH];
                            ULONG eaten;
                            LPITEMIDLIST pidlPath;

                            ComboBox_GetText(hwndDrives, 
                                             temp, 
                                             MAX_PATH);

                            strcat(temp, "\\");

                            MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, temp,
                                                strlen(temp), drive, sizeof(drive));

                            HRESULT res;

                            res = desktop->ParseDisplayName(hwnd, NULL, drive, &eaten, &pidlDrive, NULL);

                            LPVOID lpMessageBuffer;

		                    FormatMessage(
		                      FORMAT_MESSAGE_ALLOCATE_BUFFER |
		                      FORMAT_MESSAGE_FROM_SYSTEM,
		                      NULL,
		                      res,
		                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		                      (LPTSTR) &lpMessageBuffer,
		                      0,
		                      NULL );

		                    // now display this string
 		                    //MessageBox(NULL, (char*)lpMessageBuffer, 0, MB_OK);

		                    // Free the buffer allocated by the system
		                    LocalFree( lpMessageBuffer );

                            Edit_GetText(hwndDirectory, 
                                         temp,
                                         MAX_PATH);

                            MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, temp, 
                                                strlen(temp), path, sizeof(path));

                            desktop->ParseDisplayName(hwnd, NULL, path, &eaten, &pidlPath, NULL);

                            BROWSEINFO bi; 
                            LPITEMIDLIST browseId;
                            char displayName[MAX_PATH + 1];

                            bi.hwndOwner = hwnd;
                            bi.pidlRoot = pidlDrive;
                            bi.pszDisplayName = displayName;
                            bi.lpszTitle = "Please select the folder in which you want to search.";
                            bi.ulFlags = BIF_RETURNONLYFSDIRS;
                            bi.lpfn = BrowseCallbackProc;
                            bi.lParam = (LPARAM)pidlPath;

                            browseId = SHBrowseForFolder(&bi);
        
                            if(browseId)
                            {
                                SHGetPathFromIDList(browseId, temp);
                        
                                Edit_SetText(hwndDirectory, temp);

                                pMalloc->Free(browseId);
                            }
                        }
                    }
                
                    break;
                } 
            }

            break;

        }
    }

    return result;


}
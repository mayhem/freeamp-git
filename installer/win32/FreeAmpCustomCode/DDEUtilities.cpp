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

// The debugger can't handle symbols more than 255 characters long.
// STL often creates symbols longer than that.
// When symbols are longer than 255 characters, the warning is disabled.
#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <DDEML.H>

#include <vector>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

#include "DDEUtilities.h"
#include "resource.h"

DWORD idInst;

HINSTANCE g_hinst = NULL;

HDDEDATA CALLBACK DdeCallback(UINT uType, 
                              UINT uFmt,
                              HCONV hconv,
                              HSZ hsz1,
                              HSZ hsz2,
                              HDDEDATA hdata,
                              DWORD dwData1,
                              DWORD dwData2);

INT 
WINAPI 
DllMain (   HINSTANCE hInstance,
            ULONG ul_reason_being_called,
	        LPVOID lpReserved)
{
	switch (ul_reason_being_called)
	{
		case DLL_PROCESS_ATTACH:
			g_hinst = hInstance;
	    	break;

		case DLL_THREAD_ATTACH:
		    break;

		case DLL_THREAD_DETACH:
		    break;

		case DLL_PROCESS_DETACH:
		    break;
	}

    return 1;                 
}

bool InitDDE()
{
	DdeInitialize(  &idInst,
                    (PFNCALLBACK) DdeCallback,
                    CBF_FAIL_EXECUTES |CBF_SKIP_ALLNOTIFICATIONS,
                    0);
    return true;
}

bool UninitDDE()
{
	DdeUninitialize(idInst);

    return true;
}

HDDEDATA CALLBACK DdeCallback(UINT uType,
                              UINT uFmt,
                              HCONV hconv,
                              HSZ hsz1,
                              HSZ hsz2,
                              HDDEDATA hdata,
                              DWORD dwData1,
                              DWORD dwData2)
{
	 switch (uType) 
     {
		  case XTYP_REGISTER:
		  case XTYP_UNREGISTER:
				return (HDDEDATA) NULL;

		  case XTYP_ADVDATA:
				return (HDDEDATA) DDE_FACK;

		  case XTYP_XACT_COMPLETE:
				return (HDDEDATA) NULL;

		  case XTYP_DISCONNECT:
				return (HDDEDATA) NULL;

		  default:
				return (HDDEDATA) NULL;
	 }
}

static
BOOL CALLBACK ProgressDlgProc(HWND hwnd,
                              UINT msg,
                              WPARAM wParam,
                              LPARAM lParam)
{
    BOOL result = FALSE;
    static HWND hwndProgress;

	switch(msg)
	{
		case WM_INITDIALOG:
		{
            hwndProgress = GetDlgItem(hwnd, IDC_PROGRESS);

            SendMessage(hwndProgress,
                        PBM_SETRANGE,
                        0,
                        MAKELPARAM(0,10));

            SendMessage(hwndProgress,
                        PBM_SETPOS,
                        0,
                        0);

            SendMessage(hwndProgress,
                        PBM_SETSTEP,
                        1,
                        0);

            SetTimer(hwnd, 0, 500, NULL);
            result = FALSE;
			break;
		}   
        
        case WM_TIMER:
        {
            static int count = 0;

            SendMessage(hwndProgress,
                        PBM_STEPIT,
                        0,
                        0);

            if(++ count == 10)
            {
                KillTimer(hwnd, 0);
                EndDialog(hwnd, TRUE);
            }

            break;
        }

        case WM_DESTROY:
        {
            
            break;
        }

 
	}

	return result;
}

static void ShowProgressDialog()
{
    InitCommonControls();

    // stupid MS compiler... why is it finding the wrong function?
    DialogBox(g_hinst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (int (__stdcall *)(void))ProgressDlgProc);

    /*CreateDialog(g_hinst, 
              MAKEINTRESOURCE(IDD_DIALOG1),
              NULL,
              ProgressDlgProc);*/
}

bool RestartNetscape()
{
	HSZ hszServName;
	HSZ hszTopic;
	HSZ hszItem;
    HSZ hszInfoTopic;
	HSZ hszInfoItem;
	HCONV hConv;
    HCONV hInfoConv;
    HDDEDATA hdded = NULL;
	HDDEDATA hWindowIdData = NULL;
    HDDEDATA hWindowInfoData = NULL;
	char* endStuff = ",,0x0,0x1,,,";
	char* szMsg;
    vector<string> url_array;

	szMsg = new char [4096 + strlen(endStuff) + 1];

    hszServName = DdeCreateStringHandle(idInst,"NETSCAPE",CP_WINANSI);
    //hszServName = DdeCreateStringHandle(idInst,"IEXPLORE",CP_WINANSI);

    hszTopic = DdeCreateStringHandle(idInst,"WWW_ListWindows",CP_WINANSI);
    hszItem =  DdeCreateStringHandle(idInst,"WWW_ListWindows",CP_WINANSI);

    hConv = DdeConnect(idInst,hszServName,hszTopic,(PCONVCONTEXT) NULL);

	hWindowIdData = DdeClientTransaction(NULL,0,hConv,hszItem, CF_TEXT, XTYP_REQUEST, 60000UL, NULL);
	
    if(hWindowIdData)
	{
        DWORD* windowArray;
        DWORD size;

	    windowArray = (DWORD*)DdeAccessData(hWindowIdData, &size);	

        if(windowArray)
        {
            char id[11];
            char* url = new char[4096];
            int i = 0;

            hszInfoTopic = DdeCreateStringHandle(idInst,"WWW_GetWindowInfo",CP_WINANSI);
            hInfoConv = DdeConnect(idInst,hszServName,hszInfoTopic,(PCONVCONTEXT) NULL);

            while(windowArray[i])
            {
                sprintf(id, "%#lx",windowArray[i]);

                hszInfoItem =  DdeCreateStringHandle(idInst,id,CP_WINANSI);
                
	            hWindowInfoData = DdeClientTransaction(NULL,0,hInfoConv,hszInfoItem, CF_TEXT, XTYP_REQUEST, 60000UL, NULL);

                if(hWindowInfoData)
	            {
                    BYTE* data;
                    DWORD size;

	                data = DdeAccessData(hWindowInfoData, &size);	

                    if(data)
                    {
                        char title[256] = {0x00};
                        char name[256]= {0x00};
                        int fields = 0;

                        memcpy(szMsg, data, size);
                        szMsg[size] = 0x00;

                        fields = sscanf(szMsg, "\"%[^\"] \",\"%[^\"] \",\"%[^\"] \"", url, title, name);

                        if(!*name)
                        {
                            url_array.push_back(string(url));
                        }

                        DdeUnaccessData(hWindowInfoData);
                    }
	            }


                DdeFreeStringHandle(idInst, hszInfoItem);

                i++;
            }

            delete [] url;

            DdeDisconnect(hInfoConv);

            DdeFreeStringHandle(idInst, hszInfoTopic);

            DdeUnaccessData(hWindowIdData);
        }
	}

    DdeDisconnect(hConv); 

    DdeFreeStringHandle(idInst, hszTopic);
    DdeFreeStringHandle(idInst, hszItem);


    hszTopic = DdeCreateStringHandle(idInst,"WWW_Exit",CP_WINANSI);
    hszItem =  DdeCreateStringHandle(idInst,"WWW_Exit",CP_WINANSI);

    hConv = DdeConnect(idInst,hszServName,hszTopic,(PCONVCONTEXT) NULL);
    hdded = DdeClientTransaction(NULL,0,hConv,hszItem, CF_TEXT, XTYP_POKE, 60000UL, NULL);
    DdeDisconnect(hConv);

    DdeFreeStringHandle(idInst, hszTopic);
    DdeFreeStringHandle(idInst, hszItem);

    // let it quit
    ShowProgressDialog();

    hszTopic = DdeCreateStringHandle(idInst,"WWW_OpenURL",CP_WINANSI);

    hConv = DdeConnect(idInst,hszServName,hszTopic,(PCONVCONTEXT) NULL);

    vector<string>::iterator i;
    
    for(i = url_array.begin(); i != url_array.end(); i++)
    {
        //if(i == url_array.begin())
        //{
            wsprintf(szMsg,"c:\\progra~1\\netscape\\communicator\\program\\netscape.exe %s",(*i).c_str()); 
            WinExec(szMsg, SW_SHOWNORMAL);
            Sleep(1000);
        //}
        //else
        //{
        //    wsprintf(szMsg,"%s%s",(*i).c_str() , endStuff);
	    //    hszItem = DdeCreateStringHandle(idInst,szMsg,CP_WINANSI);

         //   hdded = DdeClientTransaction(NULL,0,hConv,hszItem, CF_TEXT, XTYP_REQUEST, 60000UL, NULL);

        //    DdeFreeStringHandle(idInst, hszItem);
        //}
    }

    DdeDisconnect(hConv);

    
    // clean up
    DdeFreeStringHandle(idInst, hszServName);
    DdeFreeStringHandle(idInst, hszTopic);
    DdeFreeStringHandle(idInst, hszItem);

	delete [] szMsg;

	return (url_array.size() != 0);
}


URLList* RetrieveNetscapeURLs()
{
	HSZ hszServName;
	HSZ hszTopic;
	HSZ hszItem;
    HSZ hszInfoTopic;
	HSZ hszInfoItem;
	HCONV hConv;
    HCONV hInfoConv;
    HDDEDATA hdded = NULL;
	HDDEDATA hWindowIdData = NULL;
    HDDEDATA hWindowInfoData = NULL;
	char* endStuff = ",,0x0,0x1,,,";
	char* szMsg;
    vector<string>* url_array = new vector<string>;

	szMsg = new char[4096 + strlen(endStuff) + 1];

    hszServName = DdeCreateStringHandle(idInst,"NETSCAPE",CP_WINANSI);
    //hszServName = DdeCreateStringHandle(idInst,"IEXPLORE",CP_WINANSI);

    hszTopic = DdeCreateStringHandle(idInst,"WWW_ListWindows",CP_WINANSI);
    hszItem =  DdeCreateStringHandle(idInst,"WWW_ListWindows",CP_WINANSI);

    cout << "Requesting Window List" << endl;

    hConv = DdeConnect(idInst,hszServName,hszTopic,(PCONVCONTEXT) NULL);

	hWindowIdData = DdeClientTransaction(NULL,0,hConv,hszItem, CF_TEXT, XTYP_REQUEST, 60000UL, NULL);
	
    if(hWindowIdData)
	{
        DWORD* windowArray;
        DWORD size;

        cout << "Received response to request" << endl;

	    windowArray = (DWORD*)DdeAccessData(hWindowIdData, &size);	

        if(windowArray)
        {
            char id[11];
            char* url = new char[4096];
            int i = 0;

            hszInfoTopic = DdeCreateStringHandle(idInst,"WWW_GetWindowInfo",CP_WINANSI);
            hInfoConv = DdeConnect(idInst,hszServName,hszInfoTopic,(PCONVCONTEXT) NULL);

            while(windowArray[i])
            {
                sprintf(id, "%#lx",windowArray[i]);

                cout << "Window Id: " << id << endl;

                hszInfoItem =  DdeCreateStringHandle(idInst,id,CP_WINANSI);
                
	            hWindowInfoData = DdeClientTransaction(NULL,0,hInfoConv,hszInfoItem, CF_TEXT, XTYP_REQUEST, 60000UL, NULL);

                if(hWindowInfoData)
	            {
                    BYTE* data;
                    DWORD size;

	                data = DdeAccessData(hWindowInfoData, &size);	

                    //char buf[] = "\"foo\",\"bar\",\"baz\"" ;

                    if(data)
                    {
                        char title[256] = {0x00};
                        char name[256]= {0x00};
                        int fields = 0;

                        memcpy(szMsg, data, size);
                        szMsg[size] = 0x00;

                        //sscanf(buf, "\"%s[^\"],\"%s[^\"],\"%s[^\"]", url, title, name);
                        fields = sscanf(szMsg, "\"%[^\"] \",\"%[^\"] \",\"%[^\"] \"", url, title, name);

                        //cout << szMsg << endl << url << endl << title << endl<< name << endl;
                        cout << fields << endl;

                        if(!*name)
                        {
                            url_array->push_back(string(url));
                        }

                        DdeUnaccessData(hWindowInfoData);
                    }
	            }


                DdeFreeStringHandle(idInst, hszInfoItem);

                i++;
            }

            delete [] url;

            DdeDisconnect(hInfoConv);

            DdeFreeStringHandle(idInst, hszInfoTopic);

            DdeUnaccessData(hWindowIdData);
        }
	}

    DdeDisconnect(hConv); 

  
    // clean up
    DdeFreeStringHandle(idInst, hszServName);
    DdeFreeStringHandle(idInst, hszTopic);
    DdeFreeStringHandle(idInst, hszItem);

	delete [] szMsg;

	return url_array;
}

bool ExitNetscape()
{
    HSZ hszServName;
	HSZ hszTopic;
	HSZ hszItem;
	HCONV hConv;
    HDDEDATA hdded = NULL;

    hszServName = DdeCreateStringHandle(idInst,"NETSCAPE",CP_WINANSI);
    hszTopic = DdeCreateStringHandle(idInst,"WWW_Exit",CP_WINANSI);
    hszItem =  DdeCreateStringHandle(idInst,"WWW_Exit",CP_WINANSI);

    hConv = DdeConnect(idInst,hszServName,hszTopic,(PCONVCONTEXT) NULL);
    hdded = DdeClientTransaction(NULL,0,hConv,hszItem, CF_TEXT, XTYP_POKE, 60000UL, NULL);
    DdeDisconnect(hConv);

    DdeFreeStringHandle(idInst, hszTopic);
    DdeFreeStringHandle(idInst, hszItem);
    DdeFreeStringHandle(idInst, hszServName);

    return true;
}

bool OpenNetscapeURLs(URLList* list)
{
    HSZ hszServName;
	HSZ hszTopic;
	HSZ hszItem;
	HCONV hConv;
    HDDEDATA hdded = NULL;
    char* szMsg;

	szMsg = new char[4096 + 1];

    hszServName = DdeCreateStringHandle(idInst,"NETSCAPE",CP_WINANSI);
    hszTopic = DdeCreateStringHandle(idInst,"WWW_OpenURL",CP_WINANSI);

    hConv = DdeConnect(idInst,hszServName,hszTopic,(PCONVCONTEXT) NULL);

    vector<string>::iterator i;
    
    for(i = list->begin(); i != list->end(); i++)
    {
        //if(i == url_array.begin())
        //{
            wsprintf(szMsg,"c:\\progra~1\\netscape\\communicator\\program\\netscape.exe %s",(*i).c_str()); 
            WinExec(szMsg, SW_SHOWNORMAL);
            Sleep(1000);
        //}
        //else
        //{
        //    wsprintf(szMsg,"%s%s",(*i).c_str() , endStuff);
	    //    hszItem = DdeCreateStringHandle(idInst,szMsg,CP_WINANSI);

         //   hdded = DdeClientTransaction(NULL,0,hConv,hszItem, CF_TEXT, XTYP_REQUEST, 60000UL, NULL);

        //    DdeFreeStringHandle(idInst, hszItem);
        //}
    }

    DdeDisconnect(hConv);


    DdeFreeStringHandle(idInst, hszTopic);
    DdeFreeStringHandle(idInst, hszItem);
    DdeFreeStringHandle(idInst, hszServName);
    delete [] szMsg;
    delete list;

    return true;
}

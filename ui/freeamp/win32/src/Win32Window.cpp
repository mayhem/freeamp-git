/*____________________________________________________________________________

   FreeAmp - The Free MP3 Player

   Copyright (C) 1999 EMusic
   Portions Copyright (C) 1999 Valters Vingolds

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

#include <stdio.h>
#include "Theme.h"
#include "Win32Window.h"
#include "Win32Canvas.h"
#include "debug.h"

#define DB Debug_v("%s:%d\n", __FILE__, __LINE__);

const static char szAppName[] = BRANDING;
HINSTANCE g_hinst = NULL;

#define IDI_EXE_ICON 101


INT WINAPI DllMain (HINSTANCE hInstance,
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

static LRESULT WINAPI MainWndProc(HWND hwnd, UINT msg, 
                                  WPARAM wParam, LPARAM lParam )
{
    LRESULT result = 0;
    Win32Window* ui = (Win32Window*)GetWindowLong(hwnd, GWL_USERDATA);

    switch (msg)
    {
        case WM_CREATE:
        {
            // When we create the window we pass in a pointer to our
            // UserInterface object...
            // Tuck away the pointer in a safe place
            
            ui = (Win32Window*)((LPCREATESTRUCT)lParam)->lpCreateParams;

            assert(ui != NULL);
          
            result = SetWindowLong(hwnd, GWL_USERDATA, (LONG)ui);
            ui->Init();
            
            SetTimer(hwnd, 0, 250, NULL);
            // We want people to be able to drop files on the player
            DragAcceptFiles(hwnd, TRUE);
            
            break;
        }

		case WM_CLOSE:
        {
            Rect oRect;
            Pos  oPos;

            KillTimer(hwnd, 0);
 
            ui->GetWindowPosition(oRect);
            oPos.x = oRect.x1;
            oPos.y = oRect.y1;
            ui->SaveWindowPos(oPos);
            PostQuitMessage(0);
            break;
        }    

		case WM_TIMER:
            ui->TimerEvent();
            break;

        case WM_DESTROY:
            break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC         hDc;
            Rect        oRect;
            
        	hDc = BeginPaint(hwnd, &ps);
            oRect.x1 = ps.rcPaint.left;
            oRect.y1 = ps.rcPaint.top;
            oRect.x2 = ps.rcPaint.right;
            oRect.y2 = ps.rcPaint.bottom;
            
            ((Win32Canvas *)(ui->GetCanvas()))->Paint(hDc, oRect);
            EndPaint(hwnd, &ps);
            
            break;
        }    

        case WM_MOUSEMOVE:
        {
            POINT pt;
        	Pos oPos;

            pt.x = (int16)LOWORD(lParam);
            pt.y = (int16)HIWORD(lParam);

            ClientToScreen(hwnd, &pt);
            oPos.x = pt.x;
            oPos.y = pt.y;
            
            ui->HandleMouseMove(oPos);
            break;
        }


        case WM_NCMOUSEMOVE:
        {
        	Pos oPos;
            
        	oPos.x = (int16)LOWORD(lParam);
            oPos.y = (int16)HIWORD(lParam);  
            ui->HandleMouseMove(oPos);

            break;
        }		

        case WM_LBUTTONDOWN:
        {
            POINT pt;
        	Pos oPos;

            pt.x = (int16)LOWORD(lParam);
            pt.y = (int16)HIWORD(lParam);

            ClientToScreen(hwnd, &pt);
            oPos.x = pt.x;
            oPos.y = pt.y;
            
            ui->HandleMouseLButtonDown(oPos);
            
            break;
        }

        case WM_LBUTTONUP:
        {
            POINT pt;
        	Pos oPos;

            pt.x = (int16)LOWORD(lParam);
            pt.y = (int16)HIWORD(lParam);

            ClientToScreen(hwnd, &pt);
            oPos.x = pt.x;
            oPos.y = pt.y;
            
            ui->HandleMouseLButtonUp(oPos);
            
            break;
        }

        case WM_KEYDOWN:
        {
            ui->Keystroke((unsigned char)wParam);
            break;
        }
        case WM_DROPFILES:
            ui->DropFiles((HDROP) wParam);
            break;

        default:
            result = DefWindowProc( hwnd, msg, wParam, lParam );
            break;

    }

    return result;
}


Win32Window::Win32Window(Theme *pTheme, string &oName)
            :Window(pTheme, oName)
{
    m_pCanvas = new Win32Canvas(this);
	m_hWnd = NULL;
}

Win32Window::~Win32Window(void)
{
}

Error Win32Window::Run(Pos &oPos)
{
    WNDCLASS wc;
    MSG      msg;
    Rect     oRect;
    HRGN     hRgn;
    HDC      hDc;
    int      iMaxX, iMaxY;

	m_oWindowPos = oPos;

    memset(&wc, 0x00, sizeof(WNDCLASS));

    wc.style = CS_OWNDC|CS_DBLCLKS ;
    wc.lpfnWndProc = MainWndProc;
    wc.hInstance = g_hinst;
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hIcon = LoadIcon( g_hinst, MAKEINTRESOURCE(IDI_EXE_ICON) );
    wc.hbrBackground = NULL;
    wc.lpszClassName = szAppName;

    RegisterClass(&wc);
         
    hDc = GetDC(NULL);
	iMaxX = GetDeviceCaps(hDc, HORZRES);
	iMaxY = GetDeviceCaps(hDc, VERTRES);
    ReleaseDC(NULL, hDc);
    
    GetCanvas()->GetBackgroundRect(oRect);
    
    if (m_oWindowPos.x > iMaxX || m_oWindowPos.x + oRect.Width() < 0)
       m_oWindowPos.x = 0;
    if (m_oWindowPos.y > iMaxY || m_oWindowPos.y + oRect.Height() < 0)
       m_oWindowPos.y = 0;

    if (m_bLiveInToolbar)
        m_hWnd = CreateWindowEx(WS_EX_TOOLWINDOW,
                              szAppName, 
                              szAppName,
                              WS_POPUP | WS_VISIBLE | WS_SYSMENU, 
                              m_oWindowPos.x, 
                              m_oWindowPos.y, 
                              oRect.Width(), 
                              oRect.Height(),
                              NULL, 
                              NULL, 
                              g_hinst, 
                              this);
    else                          
        m_hWnd = CreateWindow(szAppName, 
                              szAppName,
                              WS_POPUP | WS_VISIBLE | WS_SYSMENU, 
                              m_oWindowPos.x, 
                              m_oWindowPos.y, 
                              oRect.Width(), 
                              oRect.Height(),
                              NULL, 
                              NULL, 
                              g_hinst, 
                              this);

    if( m_hWnd )
    {
        hRgn = ((Win32Canvas *)m_pCanvas)->GetMaskRgn(); 
        if (hRgn)
            SetWindowRgn(m_hWnd, hRgn, false);

        ShowWindow( m_hWnd, SW_NORMAL);
        if (m_bStayOnTop)
            SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
        
        UpdateWindow( m_hWnd );

        while( GetMessage( &msg, NULL, 0, 0 ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
    }
    m_hWnd = NULL;

	oPos = m_oWindowPos;

    return kError_NoErr;
}			

void Win32Window::Init(void)
{
	Window::Init();
    ((Win32Canvas *)m_pCanvas)->SetParent(this);
}

Error Win32Window::VulcanMindMeld(Window *pOther)
{
    HRGN     hRgn;
    Error    eRet;
    Rect     oRect;
    RECT     sRect;

	oRect.x1 = oRect.x2 = oRect.y1 = oRect.y2 = 0;
	GetWindowPosition(oRect);
    sRect.left = oRect.x1;
    sRect.right = oRect.x2;
    sRect.top = oRect.y1;
    sRect.right = oRect.y2;

	// Invalidate the complete old window to ensure a redraw
    if (oRect.Width() > 0 && oRect.Height() > 0)
       InvalidateRect(NULL, &sRect, true);
    
    eRet = Window::VulcanMindMeld(pOther);
    if (IsError(eRet))
       return eRet;

    if (m_hWnd)
    {   
        m_pCanvas->GetBackgroundRect(oRect);   
        SetWindowPos(m_hWnd, NULL, 0, 0, oRect.Width(), oRect.Height(),
                     SWP_NOZORDER|SWP_NOMOVE);
    
        hRgn = ((Win32Canvas *)m_pCanvas)->GetMaskRgn(); 
        if (hRgn)
           SetWindowRgn(m_hWnd, hRgn, false);

	    InvalidateRect(m_hWnd, NULL, false);
        UpdateWindow(m_hWnd);
    }    

    return kError_NoErr;
}

void Win32Window::SaveWindowPos(Pos &oPos)
{
    m_oWindowPos = oPos;
}									

Error Win32Window::Close(void)
{
	if (!m_hWnd)
       return kError_YouScrewedUp;
       
	SendMessage(m_hWnd, WM_CLOSE, 0, 0);

    return kError_NoErr;
}

Error Win32Window::Enable(void)
{
	if (!m_hWnd)
       return kError_YouScrewedUp;
       
	return EnableWindow(m_hWnd, false) ? kError_NoErr : kError_InvalidParam;
}

Error Win32Window::Disable(void)
{
	if (!m_hWnd)
       return kError_YouScrewedUp;
	return EnableWindow(m_hWnd, true) ? kError_NoErr : kError_InvalidParam;
}

Error Win32Window::Show(void)
{
	if (!m_hWnd)
       return kError_YouScrewedUp;
	return ShowWindow(m_hWnd, SW_SHOWNORMAL) ? kError_NoErr : kError_InvalidParam;
}

Error Win32Window::Hide(void)
{
	if (!m_hWnd)
       return kError_YouScrewedUp;
	return ShowWindow(m_hWnd, SW_HIDE) ? kError_NoErr : kError_InvalidParam;
}

Error Win32Window::SetTitle(string &oTitle)
{
	if (!m_hWnd)
       return kError_YouScrewedUp;
       
	return SetWindowText(m_hWnd, oTitle.c_str()) ? kError_NoErr : kError_InvalidParam;
}

Error Win32Window::CaptureMouse(bool bCapture)
{
	if (!m_hWnd)
       return kError_YouScrewedUp;
       
	if (bCapture)
    {
        SetCapture(m_hWnd);
        return kError_NoErr;
    }
    else    
        return ReleaseCapture() ? kError_NoErr : kError_InvalidParam;
}

Error Win32Window::HideMouse(bool bHide)
{
	return ShowCursor(!bHide) ? kError_NoErr : kError_InvalidParam;
}

Error Win32Window::SetMousePos(Pos &oPos)
{
	return SetCursorPos(oPos.x, oPos.y) ? kError_NoErr : kError_InvalidParam;
}

Error Win32Window::GetMousePos(Pos &oPos)
{
	POINT oPoint;
    BOOL  bRet;
    
    bRet = GetCursorPos(&oPoint);
    oPos.x = oPoint.x;
    oPos.y = oPoint.y;
    
	return bRet ? kError_NoErr : kError_InvalidParam;
}

HWND Win32Window::GetWindowHandle(void)
{
	return m_hWnd;
}

Error Win32Window::SetWindowPosition(Rect &oWindowRect)
{
	if (!m_hWnd)
       return kError_YouScrewedUp;
       
    MoveWindow(m_hWnd, oWindowRect.x1, oWindowRect.y1,
                       oWindowRect.Width(), oWindowRect.Height(),
                       true);
    return kError_NoErr;
}

Error Win32Window::GetWindowPosition(Rect &oWindowRect)
{
	RECT sRect;
    
	if (!m_hWnd)
       return kError_YouScrewedUp;
       
    GetWindowRect(m_hWnd, &sRect);
    oWindowRect.x1 = sRect.left;
    oWindowRect.x2 = sRect.right;
    oWindowRect.y1 = sRect.top;
    oWindowRect.y2 = sRect.bottom;

    return kError_NoErr;
}

Error Win32Window::Minimize(void)
{
	if (!m_hWnd)
       return kError_YouScrewedUp;
       
	ShowWindow(m_hWnd, SW_MINIMIZE);

    return kError_NoErr;
}

Error Win32Window::Restore(void)
{
	if (!m_hWnd)
       return kError_YouScrewedUp;
       
	ShowWindow(m_hWnd, SW_RESTORE);

    return kError_NoErr;
}


void Win32Window::DropFiles(HDROP dropHandle)
{
    int32 count;
    char  file[MAX_PATH + 1];
    vector<string> oFileList;

    count = DragQueryFile(  dropHandle,
                            -1L,
                            file,
                            sizeof(file));

    for(int32 i = 0; i < count; i++)
    {
        DragQueryFile(  dropHandle,
                        i,
                        file,
                        sizeof(file));
        oFileList.push_back(string(file));
    }
    m_pTheme->DropFiles(&oFileList);
}

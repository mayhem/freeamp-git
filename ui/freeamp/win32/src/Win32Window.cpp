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

// The debugger can't handle symbols more than 255 characters long.
// STL often creates symbols longer than that.
// When symbols are longer than 255 characters, the warning is disabled.
#ifdef WIN32
#pragma warning(disable:4786)
#endif

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
        case WM_NCCREATE:  
        case WM_NCDESTROY: 
        case WM_NCCALCSIZE:
            return 1;    
        
        default:
            return ui->WindowProc(hwnd, msg, wParam, lParam);
    }
    return result;
}        
        
LRESULT Win32Window::WindowProc(HWND hwnd, UINT msg, 
                                 WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    switch (msg)
    {
		case WM_CLOSE:
        {
            Rect oRect;
            Pos  oPos;

            KillTimer(hwnd, 0);
 
            GetWindowPosition(oRect);
            oPos.x = oRect.x1;
            oPos.y = oRect.y1;
            SaveWindowPos(oPos);
            PostQuitMessage(0);
            
            m_pTheme->HandleControlMessage(string("Quit"), CM_Pressed);
            
            break;
        }   
        
		case WM_TIMER:
            MouseLeaveCheck();
            m_pMindMeldMutex->Acquire();
            TimerEvent();
            m_pMindMeldMutex->Release();
            break;

        case WM_DESTROY:
            break;

        case WM_PAINT:
            m_pMindMeldMutex->Acquire();
            Paint();
            m_pMindMeldMutex->Release();
            break;

        case WM_MOUSEMOVE:
        {
            POINT pt;
        	Pos oPos;

            pt.x = (int16)LOWORD(lParam);
            pt.y = (int16)HIWORD(lParam);

            ClientToScreen(hwnd, &pt);
            oPos.x = pt.x;
            oPos.y = pt.y;
            
            m_pMindMeldMutex->Acquire();
            HandleMouseMove(oPos);
            m_pMindMeldMutex->Release();
            break;
        }

        case WM_NCMOUSEMOVE:
        {
        	Pos oPos;
            
        	oPos.x = (int16)LOWORD(lParam);
            oPos.y = (int16)HIWORD(lParam);  
            m_pMindMeldMutex->Acquire();
            HandleMouseMove(oPos);
            m_pMindMeldMutex->Release();

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
            
            m_pMindMeldMutex->Acquire();
            HandleMouseLButtonDown(oPos);
            m_pMindMeldMutex->Release();
            
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

            m_pMindMeldMutex->Acquire();
            HandleMouseLButtonUp(oPos);
            m_pMindMeldMutex->Release();
            
            break;
        }

        case WM_KEYDOWN:
        {
            if (wParam == VK_F1)
               wParam = 'h';
               
            Keystroke((unsigned char)wParam);
            break;
        }
        
        case WM_NOTIFY:
            Notify(wParam, (LPNMHDR)lParam);
            break;
            
        case WM_DROPFILES:
            DropFiles((HDROP) wParam);
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
    m_pMindMeldMutex = new Mutex();
    m_bMouseInWindow = false;
}

Win32Window::~Win32Window(void)
{
    delete m_pMindMeldMutex;
}

Error Win32Window::Run(Pos &oPos)
{
    WNDCLASS wc;
    MSG      msg;
    Rect     oRect;
    HRGN     hRgn;
    HDC      hDc;
    int      iMaxX, iMaxY;
	Canvas  *pCanvas;

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
    
	
    pCanvas = GetCanvas();
	if (pCanvas)
    {
		pCanvas->GetBackgroundRect(oRect);
    
        if (m_oWindowPos.x > iMaxX || m_oWindowPos.x + oRect.Width() < 0)
            m_oWindowPos.x = 0;
        if (m_oWindowPos.y > iMaxY || m_oWindowPos.y + oRect.Height() < 0)
            m_oWindowPos.y = 0;

        if(m_oWindowPos.x < 0 || m_oWindowPos.y < 0)
		{    
            m_oWindowPos.x = (iMaxX - oRect.Width())/2;
            m_oWindowPos.y = (iMaxY - oRect.Height())/2;
        }
    }

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
        
        CreateTooltips();
        UpdateWindow( m_hWnd );

        m_pTheme->PostWindowCreate();

        SetForegroundWindow(m_hWnd);

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

    m_pMindMeldMutex->Acquire();

	oRect.x1 = oRect.x2 = oRect.y1 = oRect.y2 = 0;
	GetWindowPosition(oRect);
    sRect.left = oRect.x1;
    sRect.right = oRect.x2;
    sRect.top = oRect.y1;
    sRect.right = oRect.y2;

    InvalidateRect(NULL, &sRect, true);
    
    eRet = Window::VulcanMindMeld(pOther);
    m_pMindMeldMutex->Release();

    if (IsError(eRet))
       return eRet;

    if (m_hWnd)
    {   
        Rect oNewRect, oSize;
 
        m_pCanvas->GetBackgroundRect(oSize);   
        GetReloadWindowPos(oRect, oSize.Width(), oSize.Height(), oNewRect);
        SetWindowPos(m_hWnd, NULL, oNewRect.x1, oNewRect.y1, 
                     oNewRect.Width(), oNewRect.Height(),
                     SWP_NOZORDER);
    
        hRgn = ((Win32Canvas *)m_pCanvas)->GetMaskRgn(); 
        if (hRgn)
           SetWindowRgn(m_hWnd, hRgn, false);

        CreateTooltips();

	    InvalidateRect(m_hWnd, NULL, false);
        UpdateWindow(m_hWnd);
    }    

    return kError_NoErr;
}

void Win32Window::Paint(void)
{
    PAINTSTRUCT ps;
    HDC         hDc;
    Rect        oRect;
    
	hDc = BeginPaint(m_hWnd, &ps);
    oRect.x1 = ps.rcPaint.left;
    oRect.y1 = ps.rcPaint.top;
    oRect.x2 = ps.rcPaint.right;
    oRect.y2 = ps.rcPaint.bottom;
    
    ((Win32Canvas *)(GetCanvas()))->Paint(hDc, oRect);
    EndPaint(m_hWnd, &ps);
}    

void Win32Window::TimerEvent(void)
{
    Window::TimerEvent();
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

void 
Win32Window::
Notify(int32 command, LPNMHDR notifyMsgHdr)
{
    if(notifyMsgHdr->code == TTN_NEEDTEXT)
    {
        int32 idCtrl = notifyMsgHdr->idFrom;
        LPTOOLTIPTEXT lpttt = (LPTOOLTIPTEXT) notifyMsgHdr; 

        string strTip;

        //
        // and feed it's tip
        //

        m_oControls[idCtrl]->GetTip(strTip);
        if(strTip.length())
		{
			if(strTip.length()>79)
			{
                // to avoid buffer overruns
                lpttt->szText[79]=0;
                strncpy(lpttt->szText,strTip.c_str(),78);
			}
			else
			    strcpy(lpttt->szText,strTip.c_str()); // if tip is there
		}
    }
}

void 
Win32Window::
CreateTooltips()
{
    // tooltip support
    static HWND hwndTooltip = NULL;
        static uint32 uTooltipCount = 0;
    HINSTANCE hinst = (HINSTANCE)GetWindowLong( m_hWnd, 
                                                GWL_HINSTANCE);
    TOOLINFO ti;
    vector<Control *>::iterator i;
    uint32 uCtr;

    //
    // check if we have been here
    //
    if(hwndTooltip)
    {
        // remove old tooltips, then
        for(uCtr=0; uCtr<uTooltipCount; uCtr++)
        {

            ti.cbSize = sizeof(TOOLINFO); 
            ti.hwnd = m_hWnd; 
            ti.uId = uCtr; 
 
            SendMessage(hwndTooltip, 
                    TTM_DELTOOL, 
                    0, 
                    (LPARAM) &ti);
        }
    }

    if(!hwndTooltip)
    {
        // for some reason if mindmeld, destroying and
        // re-creating window does not work. we can
        // create it only once.

        hwndTooltip = CreateWindowEx(WS_EX_TOPMOST,
            TOOLTIPS_CLASS, 
            NULL, 
            0, 
            CW_USEDEFAULT, 
            CW_USEDEFAULT, 
            CW_USEDEFAULT, 
            CW_USEDEFAULT, 
            m_hWnd, 
            (HMENU) NULL, 
            hinst, 
            NULL);
    }

    //
    // now go adding tooltip regions
    //
    uCtr=0;
    for(i = m_oControls.begin(); i != m_oControls.end(); i++)
    {
        Rect rect;
        string strTip;

        (*i)->GetTip(strTip);
        (*i)->GetRect(rect);

        // add a tool tip

        ti.cbSize = sizeof(TOOLINFO); 
        ti.uFlags =  TTF_SUBCLASS; 
        ti.hwnd = m_hWnd; 
        ti.hinst = hinst; 
        ti.uId = uCtr; 
        ti.lpszText = (LPSTR) LPSTR_TEXTCALLBACK; 
        ti.rect.left = rect.x1; 
        ti.rect.top = rect.y1; 
        ti.rect.right = rect.x2; 
        ti.rect.bottom = rect.y2; 

        SendMessage(hwndTooltip, 
            TTM_ADDTOOL, 
            0, 
            (LPARAM) &ti);

        uCtr++;
    }
        
    uTooltipCount = uCtr; // save value for next mindmeld
}

void Win32Window::SetStayOnTop(bool bStay)
{
    Window::SetStayOnTop(bStay);

    if (m_hWnd == NULL)
       return;

    if (m_bStayOnTop)
       SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
    else   
       SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
}    

void Win32Window::SetLiveInToolbar(bool bLive)
{
    int iExtStyle, iStyle;
    
    Window::SetLiveInToolbar(bLive);

    if (m_hWnd == NULL)
       return;

    ShowWindow(m_hWnd, FALSE);

    iStyle = GetWindowLong(m_hWnd, GWL_STYLE);
    iExtStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
    if (m_bLiveInToolbar)
        iExtStyle |= WS_EX_TOOLWINDOW;
    else
        iExtStyle &= ~WS_EX_TOOLWINDOW;

    SetWindowLong(m_hWnd, GWL_STYLE, WS_VISIBLE);
    SetWindowLong(m_hWnd, GWL_STYLE, iStyle);
    SetWindowLong(m_hWnd, GWL_EXSTYLE, iExtStyle);
    ShowWindow(m_hWnd, TRUE);
}

bool Win32Window::LButtonDown(void)
{
    return GetAsyncKeyState(VK_LBUTTON) < 0;
}

void Win32Window::MouseLeaveCheck(void)
{
    Rect  oRect;
    POINT sPos;
    Pos   oPos;
    HDC   hDc;
    
    GetWindowPosition(oRect);
    GetCursorPos(&sPos);
    oPos.x = sPos.x;
    oPos.y = sPos.y;
    ScreenToClient(m_hWnd, &sPos);
    
    hDc = GetDC(m_hWnd);
    if (!oRect.IsPosInRect(oPos) || !PtVisible(hDc, sPos.x, sPos.y))
    {
        if (m_bMouseInWindow)
            MouseHasLeftWindow();
        m_bMouseInWindow = false;
    }
    else
        m_bMouseInWindow = true;
        
    ReleaseDC(m_hWnd, hDc);
}

Error Win32Window::GetDesktopSize(int32 &iX, int32 &iY)
{
    RECT sRect;
    
    SystemParametersInfo(SPI_GETWORKAREA, 0, &sRect, 0);
    iX = sRect.right;
    iY = sRect.bottom;

    return kError_NoErr;
}

void Win32Window::BringWindowToFront(void)
{
    ShowWindow(m_hWnd, SW_RESTORE);
    SetForegroundWindow(m_hWnd);
}
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

#ifndef _TEXTVIEW_H_
#define _TEXTVIEW_H_

#include "config.h"
#include "view.h"
#include "thread.h"
#include "semaphore.h"



#define TextType_Normal         0       
#define TextType_Wiggle         2
#define TextType_MouseWiggle    4

class TextView : public View {
 public:
    TextView(   HWND hwnd, 
                View* parent, 
                HRGN viewRegion, 
                DIB* fontBitmap,
                int32 fontHeight,
                int32* fontWidths,
                int32 command = -1,
                int32 flags = 0);

    TextView(   HWND hwnd, 
                View* parent, 
                RECT* viewRect, 
                DIB* fontBitmap,
                int32 fontHeight,
                int32* fontWidths,
                int32 command = -1,
                int32 flags = 0);

    virtual ~TextView();

    // Tell the view to render itself into a bitmap
    virtual void Draw(DIB* canvas, RECT* invalidRect);
    virtual void MouseEntered(void);
    virtual void MouseLeft(void);
    virtual void MouseMove(int32 x, int32 y, int32 modifiers);
    virtual void LeftButtonDown(int32 x, int32 y, int32 modifiers);
    virtual void LeftButtonUp(int32 x, int32 y, int32 modifiers);

    virtual void Invoke(void);

    void SetText(const char* text);
    char* Text(void) const { return m_text; }


 protected:
    static void thread_function(void*);
    void ThreadFunction();

 private:
    DIB*                m_fontBitmap;
    DIB*                m_textBitmap;
    int32*              m_fontWidths;
    int32               m_fontHeight;
    int32               m_offset;
    int32               m_command;
    int32               m_flags;
    bool                m_needToScroll;
    bool                m_wiggle;
    bool                m_viewDying;
    bool                m_pressed;

    char*               m_text;
    int32               m_textLength;
    int32               m_bitmapLength;

    Thread*             m_thread;
    CRITICAL_SECTION    m_criticalSection;
    Semaphore*          m_semaphore;
};

#endif /* _TEXTVIEW_H_ */
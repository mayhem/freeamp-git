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

#ifndef _TIME_VIEW_H_
#define _TIME_VIEW_H_

#include "config.h"
#include "view.h"

#define TimeType_TimeOnRight    2
#define TimeType_TimeOnLeft     4 


class TimeView : public View {
 public:
    TimeView(   HWND hwnd, 
                View* parent, 
                HRGN viewRegion, 
                //DIB* backgroundBitmap,
                DIB* timeFontBitmap,
                int32 timeFontHeight,
                int32* timeFontWidths,
                DIB* labelFontBitmap,
                int32 labelFontHeight,
                int32* labelFontWidths,
                int32 command = -1,
                int32 flags = 0);

    TimeView(   HWND hwnd, 
                View* parent, 
                RECT* viewRect, 
                //DIB* backgroundBitmap,
                DIB* timeFontBitmap,
                int32 timeFontHeight,
                int32* timeFontWidths,
                DIB* labelFontBitmap,
                int32 labelFontHeight,
                int32* labelFontWidths,
                int32 command = -1,
                int32 flags = 0);

    virtual ~TimeView();

    // Tell the view to render itself into a bitmap
    virtual void Draw(DIB* canvas, RECT* invalidRect);
    virtual void LeftButtonDown(int32 x, int32 y, int32 modifiers);
    virtual void LeftButtonUp(int32 x, int32 y, int32 modifiers);

    virtual void Invoke(void);

    void SetLabel(char* label);
    char* label(void) const { return m_label; }

    void SetTime(   int32 hours, 
                    int32 minutes, 
                    int32 seconds);

    void Time(  int32* hours, 
                int32* minutes, 
                int32* seconds);

    int32 Hours(void) const { return m_hours; }
    int32 Minutes(void) const { return m_minutes; }
    int32 Seconds(void) const { return m_seconds; }

    DIB* TimeFont() const { return m_timeFontBitmap; }
    int32 TimeFontWidth(char c) { return m_timeFontWidths[c - 32];}
    int32 TimeFontHeight(void) const { return m_timeFontHeight;}

    DIB* LabelFont() const { return m_labelFontBitmap; }
    int32 LabelFontWidth(char c) { return m_labelFontWidths[c - 32];}
    int32 LabelFontHeight(void) const { return m_labelFontHeight;}

    int32 Flags(void) const { return m_flags; }
    void SetFlags(int32 flags) { m_flags = flags; }

 protected:
    
 private:
    //DIB*            m_backgroundBitmap;
    //DIB*            m_viewBitmap;

    DIB*            m_timeFontBitmap;
    int32*          m_timeFontWidths;
    int32           m_timeFontHeight;

    DIB*            m_labelFontBitmap;
    int32*          m_labelFontWidths;
    int32           m_labelFontHeight;

    int32           m_timeLength;

    int32           m_offset;
    bool            m_pressed;
    int32           m_command;
    int32           m_flags;
    int32           m_hours;
    int32           m_minutes;
    int32           m_seconds;

    char*           m_label;
    char            m_time[12]; // i should probably do this dynamically
                                // but geez (hhhhh:mm:ss) is 100,000 hours!
};

#endif /* _TIME_VIEW_H_ */
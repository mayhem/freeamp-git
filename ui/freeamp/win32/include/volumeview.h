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

#ifndef _VOLUME_VIEW_H_
#define _VOLUME_VIEW_H_

#include "config.h"
#include "view.h"

class VolumeView : public View {
 public:
    VolumeView( HWND hwnd, 
                View* parent, 
                HRGN viewRegion, 
                DIB* backgroundBitmap,
                DIB* timeFontBitmap,
                int32 timeFontHeight,
                int32* timeFontWidths,
                DIB* labelFontBitmap,
                int32 labelFontHeight,
                int32* labelFontWidths,
                int32 command = -1);

    VolumeView( HWND hwnd, 
                View* parent, 
                RECT* viewRect, 
                DIB* backgroundBitmap,
                DIB* timeFontBitmap,
                int32 timeFontHeight,
                int32* timeFontWidths,
                DIB* labelFontBitmap,
                int32 labelFontHeight,
                int32* labelFontWidths,
                int32 command = -1);

    virtual ~VolumeView();

    // Tell the view to render itself into a bitmap
    virtual void Draw(DIB* canvas, RECT* invalidRect);
    virtual void LeftButtonDown(int32 x, int32 y, int32 modifiers);
    virtual void LeftButtonUp(int32 x, int32 y, int32 modifiers);

    virtual void Invoke(void);

    void SetVolume( int32 volume );

    int32 Volume(void) const { return m_volume;};

 protected:
    
 private:
    DIB*            m_backgroundBitmap;
    DIB*            m_viewBitmap;

    DIB*            m_volFontBitmap;
    int32*          m_volFontWidths;
    int32           m_volFontHeight;

    DIB*            m_labelFontBitmap;
    int32*          m_labelFontWidths;
    int32           m_labelFontHeight;

    int32           m_volLength;

    int32           m_offset;
    bool            m_pressed;
    int32           m_command;
    int32           m_volume;

    char            m_label[7];
};

#endif /* _VOLUME_VIEW_H_ */
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

#ifndef _STATUS_VIEW_H_
#define _STATUS_VIEW_H_

#include "config.h"
#include "view.h"


class StatusView : public View {
 public:
    StatusView( HWND hwnd, 
                View* parent, 
                HRGN viewRegion, 
                DIB* viewBitmap);

    StatusView( HWND hwnd, 
                View* parent, 
                RECT* viewRect, 
                DIB* viewBitmap);

    virtual ~StatusView();

    // Tell the view to render itself into a bitmap
    virtual void Draw(DIB* canvas, RECT* invalidRect);

    virtual void On(void);
    virtual void On(bool on);
    virtual void Off(void);
    virtual bool Status(void) const { return m_on;}


 protected:
    
 private:
    DIB*            m_viewBitmap;
    int32           m_offset;
    bool            m_on;
    
};

#endif /* _STATUS_VIEW_H_ */
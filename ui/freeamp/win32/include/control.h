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

#ifndef _CONTROL_H_
#define _CONTROL_H_

class Control {
 public:
    Control(HWND hwndParent,
            HRGN controlRegion,
            HBITMAP controlBitmap);

    ~Control();

    virtual void MouseMove(int32 x, int32 y, int32 modifiers){}
    virtual void LeftButtonDown(int32 x, int32 y, int32 modifiers){}
    virtual void LeftButtonUp(int32 x, int32 y, int32 modifiers){}


    bool PointInControl(int32 x, int32 y);
    void SetEnabled(bool enabled) { m_enabled = enabled; }
    bool Enabled() const { return m_enabled; }

 protected:
    
 private:
    HWND            m_hwndParent;
    HRGN            m_controlRegion;
    RECT            m_boundingRect;
    HBITMAP         m_controlBitmap;   
    
    bool            m_enabled;
};

#endif /* _CONTROL_H_ */
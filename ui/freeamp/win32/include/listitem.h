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

#ifndef _LIST_ITEM_H
#define _LIST_ITEM_H

#include "config.h"
#include "dib.h"

class ListItem {

 public:
                    ListItem()
                    {
                        m_height = 0;
	                    m_width = 0;
	                    m_selected = false;
	                    m_enabled = true;
                        m_userValue = NULL;
                    }

    virtual        ~ListItem(){}

    int32   Height() const { return m_height;}
    int32   Width() const { return m_width;}

    void	SetHeight(int32 height) { m_height = height;}
	void	SetWidth(int32 width) { m_width = width;}

    bool	IsSelected() const { return m_selected;}
    void	Select(bool selected = true) { m_selected = selected;}
    void	Deselect() { m_selected = false;}

    bool	IsEnabled() const { return m_enabled;}
    void    Enable(bool enable = true) { m_enabled = enable;}
    void    Disable() { m_enabled = false;}

    void*   UserValue() const { return m_userValue;}
    void    SetUserValue(void* value) { m_userValue = value;}

		    
    virtual	void    DrawItem(DIB* canvas, RECT* bounds) = 0;


 private:

    int32		m_width;
    int32		m_height;
    bool		m_selected;
    bool		m_enabled;
    void*       m_userValue;

};

#endif /* _LIST_ITEM_H */

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

#ifndef _STRING_ITEM_H
#define _STRING_ITEM_H

#include "config.h"
#include "listitem.h"
#include "dib.h"

class StringItem : public ListItem {

 public:
     StringItem(char* text,
                DIB* fontBitmap,
                int32 fontHeight,
                int32* fontWidths);

    virtual ~StringItem();

    virtual void Select(bool selected = true);
    virtual void Deselect();

    virtual	void DrawItem(DIB* canvas, RECT* bounds);

 private:
    DIB*                m_fontBitmap;
    DIB*                m_textBitmap;
    int32*              m_fontWidths;
    int32               m_fontHeight;
    char*               m_text;
    int32               m_textLength;

     
};

#endif /* _STRING_ITEM_H */

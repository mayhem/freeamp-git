/*____________________________________________________________________________

   FreeAmp - The Free MP3 Player

   Copyright (C) 1999 EMusic

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

#ifndef INCLUDED_WIN32BITMAP_H__
#define INCLUDED_WIN32BITMAP_H__

#include <string>

#include <windows.h>
#include "Bitmap.h"
#include "Types.h"
#include "errors.h"

class Win32Bitmap : public Bitmap
{
    public:

              Win32Bitmap(string &oName);
			  Win32Bitmap(int iWidth, int iHeight, string &oName);
     virtual ~Win32Bitmap(void);

     virtual Error LoadBitmapFromDisk(string &oFile);
     virtual Error BlitRect(Bitmap *pSrcBitmap, Rect &oSrcRect, 
                            Rect &oDestRect);
     virtual Error MaskBlitRect(Bitmap *pSrcBitmap, Rect &oSrcRect, 
                                Rect &oDestRect);
     virtual bool  IsPosVisible(Pos &oPos);

     HBITMAP       GetBitmapHandle(void);
     HBITMAP       GetMaskBitmapHandle(void);
     HPALETTE      GetPaletteFromBackground(HDC hDc);

    protected:

     void          CreateMaskBitmap(void);
    
     HBITMAP  m_hBitmap;
     HBITMAP  m_hMaskBitmap;
	 void    *m_pBitmapData;
};

#endif

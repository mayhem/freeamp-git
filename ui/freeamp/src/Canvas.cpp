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

#include "Canvas.h"
#include "debug.h"

Canvas::Canvas(void)
{
	m_pBGBitmap = NULL;
	m_pMaskBitmap = NULL;
}

Canvas::~Canvas(void)
{
}

void Canvas::SetBackgroundRect(Rect &oRect)
{
    m_oBGRect = oRect;
}

void Canvas::SetBackgroundBitmap(Bitmap *pBitmap)
{
	delete m_pBGBitmap;
    m_pBGBitmap = pBitmap;
}

void Canvas::SetMaskBitmap(Bitmap *pBitmap)
{
    delete m_pMaskBitmap;
    m_pMaskBitmap = pBitmap;
}

void Canvas::GetBackgroundRect(Rect &oRect)
{
    oRect = m_oBGRect;
}

Bitmap *Canvas::GetBackgroundBitmap(void)
{
    return m_pBGBitmap;
}


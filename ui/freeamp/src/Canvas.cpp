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
#include "Window.h"
#include "debug.h"

Canvas::Canvas(void)
{
    m_pBGBitmap = NULL;
    m_pCompleteBGBitmap = NULL;
    m_pMaskBitmap = NULL;
}

Canvas::~Canvas(void)
{
	// The bitmap that was loaded by the theme will be destroyed by the theme
	// However, if we created any clones, we need to kill those...
	if (m_pCompleteBGBitmap)
		delete m_pBGBitmap;
}

void Canvas::SetBackgroundRect(Rect &oRect)
{
    m_oBGRect = oRect;
}

void Canvas::SetBackgroundBitmap(Bitmap *pBitmap)
{
    delete m_pBGBitmap;
    delete m_pCompleteBGBitmap;
    m_pCompleteBGBitmap = pBitmap;
    m_pBGBitmap = NULL;
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

void Canvas::InitBackgrounds(vector<Panel *> *pPanels)
{
    vector<Panel *>::iterator i;
    Rect                      oDestRect;

    if (m_pCompleteBGBitmap == NULL)
       return;

    if (pPanels->size() == 0)
    {
        m_pBGBitmap = m_pCompleteBGBitmap;
        m_pCompleteBGBitmap = NULL;
        return;
    }

	if (m_pBGBitmap)
	   delete m_pBGBitmap;

    m_pBGBitmap = m_pCompleteBGBitmap->Clone();
    for(i = pPanels->begin(); i != pPanels->end(); i++)
    {
        if (!(*i)->m_bIsOpen)
        {
            m_pBGBitmap->MakeTransparent((*i)->m_oOpenRect);
            if ((*i)->m_pClosedBitmap)
            {
                oDestRect.x1 = (*i)->m_oOffset.x;
                oDestRect.y1 = (*i)->m_oOffset.y;
                oDestRect.x2 = oDestRect.x1 + (*i)->m_oClosedRect.Width();
                oDestRect.y2 = oDestRect.y1 + (*i)->m_oClosedRect.Height();
                m_pBGBitmap->BlitRect((*i)->m_pClosedBitmap, 
                                      (*i)->m_oClosedRect,
                                      oDestRect);
                m_pBGBitmap->BlitRectMaskBitmap((*i)->m_pClosedBitmap, 
                                                (*i)->m_oClosedRect,
                                                oDestRect);
            }
        }
    }
}


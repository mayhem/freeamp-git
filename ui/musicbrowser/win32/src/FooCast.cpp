/*____________________________________________________________________________

        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 1999 EMusic.com

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

#define STRICT
#include <windows.h>
#include <windowsx.h>

#include <string>

using namespace std;

#include "config.h"
#include "FooCast.h"

char* kBusy = "Searching for Stations...";

FooCast::FooCast(HWND hwndTree, HTREEITEM parentItem):
    m_hwndTree(hwndTree), m_parentItem(parentItem)
{
    TV_INSERTSTRUCT insert;

    insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                       TVIF_SELECTEDIMAGE | TVIF_PARAM; 

    insert.item.pszText = kBusy;
    insert.item.cchTextMax = lstrlen(insert.item.pszText);
    insert.item.iImage = 12;
    insert.item.iSelectedImage = 12;
    insert.item.cChildren= 0;
    insert.item.lParam = NULL;
    insert.hInsertAfter = TVI_LAST;
    insert.hParent = m_parentItem;
    m_busyItem = TreeView_InsertItem(m_hwndTree, &insert);
}

FooCast::~FooCast()
{

}
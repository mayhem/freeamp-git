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

#include <windows.h>
#include <windowsx.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>

#include "config.h"
#include "utility.h"
#include "resource.h"
#include "Win32MusicBrowser.h"
#include "debug.h"


TreeDataIndex::TreeDataIndex(void)
{
}

TreeDataIndex::~TreeDataIndex(void)
{
}

bool TreeDataIndex::IsValidParam(int32 lParam)
{
    return lParam >= 0 && lParam < m_oTreeData.size();
}

bool TreeDataIndex::IsPlaylist(int32 lParam)
{
    if (!IsValidParam(lParam))
       return false;
    
    return m_oTreeData[lParam].m_oPlaylistName.length() > 0;
}

bool TreeDataIndex::IsArtist(int32 lParam)
{
    bool result = false;

    if (!IsValidParam(lParam))
       return false;

    result = m_oTreeData[lParam].m_pArtist != NULL &&
             m_oTreeData[lParam].m_pAlbum == NULL &&
             m_oTreeData[lParam].m_pTrack == NULL;
   
    return result;
}

bool TreeDataIndex::IsAlbum(int32 lParam)
{
    bool result = false;

    if (!IsValidParam(lParam))
       return false;

    result = m_oTreeData[lParam].m_pAlbum != NULL &&
             m_oTreeData[lParam].m_pArtist != NULL &&
             m_oTreeData[lParam].m_pTrack == NULL;

    return result;
}

bool TreeDataIndex::IsTrack(int32 lParam)
{
   bool result = false;

    if (!IsValidParam(lParam))
       return false;

    result = m_oTreeData[lParam].m_pAlbum != NULL &&
             m_oTreeData[lParam].m_pArtist != NULL &&
             m_oTreeData[lParam].m_pTrack != NULL;
    
    return result;
}

bool TreeDataIndex::IsUncatagorized(int32 lParam)
{
   bool result = false;

    if (!IsValidParam(lParam))
       return false;

    result = m_oTreeData[lParam].m_pAlbum == NULL &&
             m_oTreeData[lParam].m_pArtist == NULL &&
             m_oTreeData[lParam].m_pTrack != NULL;

    return result;
}

bool TreeDataIndex::IsPortable(int32 lParam)
{
   bool result = false;

    if (!IsValidParam(lParam))
       return false;

    result = m_oTreeData[lParam].m_pPortable != NULL;

    return result;
}

bool TreeDataIndex::IsLeaf(int32 lParam)
{
    if (!IsValidParam(lParam))
       return false;
    
    return ( IsTrack(lParam) || IsPlaylist(lParam) || IsUncatagorized(lParam));
}

int32 TreeDataIndex::GetLevel(int32 lParam)
{
    if (!IsValidParam(lParam))
       return -1;
       
    return m_oTreeData[lParam].m_iLevel;
}

const TreeData &TreeDataIndex::Data(int32 lParam)
{
    assert(IsValidParam(lParam));

    return m_oTreeData[lParam];
}

int32 TreeDataIndex::Add(TreeData &oData)
{
    m_oTreeData.push_back(oData);  
    return m_oTreeData.size() - 1;
}

void TreeDataIndex::Clear(void)
{
    m_oTreeData.clear();
}


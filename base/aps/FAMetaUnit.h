/*_______________________________________________________________________
         
        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 2000 Relatable

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program; if not, Write to the Free Software
        Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
        
        $Id$
____________________________________________________________________________*/
//
// FAMetaUnit.h: interface for the FAMetaUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FAMETAUNIT_H__A6B5FCC1_6544_4B6D_9E18_968349DB003C__INCLUDED_)
#define AFX_FAMETAUNIT_H__A6B5FCC1_6544_4B6D_9E18_968349DB003C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "metadata.h"
#include "aps.h"
#include "playlist.h"
#include "apsmetadata.h"

class FAMetaUnit : public APSMetaData  
{
public:
    // Fills the MetaData with this FAMetaUnits data
    uint32 GetMetaData(MetaData* pMetaData); 
    FAMetaUnit();
    FAMetaUnit(MetaData* pMetaData, const char* pczURL);
    FAMetaUnit(PlaylistItem* pPlayItem);

    // Fill the FAMetaUnit from the MetaData object
    uint32 FillData(MetaData* pMetaData, const char* pczURL);
    // Fill the FAMetaUnit from the PlaylistItem
    uint32 FillData(PlaylistItem* pPlayItem); 
    virtual ~FAMetaUnit();
};

#endif // !defined(AFX_FAMETAUNIT_H__A6B5FCC1_6544_4B6D_9E18_968349DB003C__INCLUDED_)

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

#ifndef INCLUDED_DATAINDEX_H_
#define INCLUDED_DATAINDEX_H_

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include "playlist.h"
#include "musicbrowser.h"

class TreeData
{
    public:

               TreeData(void)
                    {
                        m_iLevel = -1;
                        m_pArtist = NULL;
                        m_pAlbum = NULL;
                        m_pTrack = NULL;
                    };
               TreeData(const TreeData &oOther)
                    {
                        m_iLevel = oOther.m_iLevel; 
                        m_pArtist= oOther.m_pArtist;
                        m_pAlbum = oOther.m_pAlbum; 
                        m_pTrack = oOther.m_pTrack; 
                        m_oPlaylistName = oOther.m_oPlaylistName;
                        m_oPlaylistPath = oOther.m_oPlaylistPath;
                    };
      virtual ~TreeData(void) { };

      int32         m_iLevel;
      ArtistList   *m_pArtist;
      AlbumList    *m_pAlbum;
      PlaylistItem *m_pTrack;
      string        m_oPlaylistName, m_oPlaylistPath;
};

class TreeDataIndex
{
    public:

               TreeDataIndex(void);
      virtual ~TreeDataIndex(void);

      bool  IsValidParam    (int32 lParam);
      bool  IsPlaylist      (int32 lParam);
      bool  IsArtist        (int32 lParam);
      bool  IsAlbum         (int32 lParam);
      bool  IsTrack         (int32 lParam);
      bool  IsLeaf          (int32 lParam);
      bool  IsUncatagorized (int32 lParam);
      int32 GetLevel        (int32 lParam);
      const TreeData &Data  (int32 lParam); 
      int32 Add             (TreeData &pData);
      void  Clear           (void);
      
    private:
    
      vector<TreeData> m_oTreeData;  
};



#endif
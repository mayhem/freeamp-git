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

#ifndef INCLUDED_INFOEDITOR_H_
#define INCLUDED_INFOEDITOR_H_

#include "config.h"

#include <gtk/gtk.h>

#include "facontext.h"
#include "playlist.h"

class infoeditorUI {
 public:
    infoeditorUI(FAContext *, PlaylistManager *plm,
                 vector<PlaylistItem *> *itemlist);
   ~infoeditorUI();
  
    void DisplayInfo();
 
    GtkWidget *m_okButton;
    GtkWidget *m_applyButton;
  
    void DoApplyInfoEdit();
    void CheckWidget(GtkWidget *widget);

    bool changed;
 protected:
     FAContext *m_context;

 private:
     GtkWidget *m_window;
     GtkWidget *m_titleEntry;
     GtkWidget *m_artistEntry;
     GtkWidget *m_albumEntry;
     GtkWidget *m_yearEntry;
     GtkWidget *m_commentEntry;
     GtkWidget *m_genreEntry;
     GtkWidget *m_trackEntry;

     vector<PlaylistItem *> *m_itemlist;

     bool   m_artists;
     bool   m_albums;
     bool   m_years;
     bool   m_genres;
     uint32 m_listsize;

     bool title_change;
     bool artist_change;
     bool album_change;
     bool genre_change;
     bool year_change;
     bool comment_change;
     bool track_change;

     PlaylistManager *m_plm;
};

#endif

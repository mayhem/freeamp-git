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

#ifndef INCLUDED_MISSINGFILE_H_
#define INCLUDED_MISSINGFILE_H_

#include "config.h"

#include <gtk/gtk.h>

#include "facontext.h"
#include "playlist.h"

class MissingFileUI {
 public:
    MissingFileUI(FAContext *context, PlaylistItem *missing);
   ~MissingFileUI();
 
    void Run();
 
 protected:
    FAContext *m_context;

 private:
    
    PlaylistItem   *m_missing;
};
   

#endif

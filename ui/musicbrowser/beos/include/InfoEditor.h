/*____________________________________________________________________________

        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 1999, 2000 EMusic.com

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

#ifndef INCLUDED_INFO_EDITOR_H__
#define INCLUDED_INFO_EDITOR_H__

#include <be/interface/Window.h>

class PlaylistItem;
class BTextControl;

class InfoEditor : public BWindow
{
public:
                        InfoEditor( BRect frame, const char* title,
                                    PlaylistItem* item,
                                    window_look look = B_DOCUMENT_WINDOW_LOOK,
                                    window_feel feel = B_NORMAL_WINDOW_FEEL,
                                    uint32 flags = 0 );
    virtual             ~InfoEditor();
    virtual void        MessageReceived( BMessage* message );
    virtual bool        QuitRequested( void );

protected:

private:
    PlaylistItem*       m_item;
};

#endif // INCLUDED_INFO_EDITOR_H__

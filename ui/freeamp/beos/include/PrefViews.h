/*____________________________________________________________________________

   FreeAmp - The Free MP3 Player

   Copyright (C) 1999 EMusic
   Copyright (C) 1999 Hiromasa Kato

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

#ifndef INCLUDED_PREF_VIEWS_H__
#define INCLUDED_PREF_VIEWS_H__

#include <be/interface/View.h>
#include <be/interface/Box.h>

class FAContext;
class ThemeManager;
class Prefs;

class PrefViews
{
public:
                        PrefViews();
    virtual             ~PrefViews();
    virtual BView*      BuildGeneralPane( BRect frame, const char* name );
    virtual BView*      BuildThemesPane( BRect frame, const char* name );

    void                UsePrefs( Prefs* prefs ) { m_prefs = prefs; }

protected:

private:
    Prefs*              m_prefs;
};

class BoxView : public BBox
{
public:
                        BoxView( BRect frame, const char* name, BView* view,
                                 uint32 resizingMode = B_FOLLOW_LEFT_RIGHT |
                                                       B_FOLLOW_TOP );
    virtual             ~BoxView();
    void                SetContentView( BView* view );
    float               ContentWidth( void ) const;

    static const float  MARGIN;
};

#endif // INCLUDED_PREF_VIEWS_H__


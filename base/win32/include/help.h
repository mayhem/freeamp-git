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

#ifndef INCLUDED_HELP_H
#define INCLUDED_HELP_H

#define HELP_FILE BRANDING_HELP_FILE

//Main Dialog by default in Help_Finder
#define FreeAmp_Main_Window	0x0	

//Music Browser
#define Music_Browser 1
//Actual name of dialog for drive selection unknown
//Temporary name used to allocate ID number.
#define Music_Browser_Drive_Selection 10

//Download Manager
#define Download_Manager 2

//Preferences Dialog
#define Preferences_General 3
#define Preferences_Themes 4
#define Preferences_Streaming 5
#define Preferences_Update 6
#define Preferences_Plugins 7
#define Preferences_Advanced 8
#define Preferences_About 9
#define Preferences_Directory 12

#define Edit_Info 11


#endif
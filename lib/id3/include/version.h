// $Id$

// id3lib: a C++ library for creating and manipulating id3v1/v2 tags
// Copyright 1999, 2000  Scott Thomas Haug

// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; either version 2 of the License, or (at your
// option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
// License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

// The id3lib authors encourage improvements and optimisations to be sent to
// the id3lib coordinator.  Please see the README file for details on where to
// send such submissions.  See the AUTHORS file for a list of people who have
// contributed to id3lib.  See the ChangeLog file for a list of changes to
// id3lib.  These files are distributed with id3lib at
// http://download.sourceforge.net/id3lib/

#ifndef ID3LIB_VERSION_H
#define ID3LIB_VERSION_H

/** id3lib defines some functions for making known the libraries version info.
 **    
 ** They are used to identify the version and date of the id3lib that is
 ** currently being used.
 **/
/// Returns the name of the library (i.e., "id3lib")
const char* ID3_Name(void);
/// Returns the library release (i.e., "3.0.6")
const char* ID3_Version(void);
/// Returns the full name of the library (i.e., "id3lib-3.0.6")
const char* ID3_FullName(void);
/// Returns the library version (i.e., 3)
unsigned short ID3_Major(void);
/// Returns the library revision (i.e., 0)
unsigned short ID3_Minor(void);
/// Returns the library patch (i.e., 6)
unsigned short ID3_Patch(void);

#endif

// $Log$
// Revision 1.1  2000/04/17 18:48:14  robert
// I've added the header files for the id3v2 lib and then I will also
// be checking in the .DLL for this. For unix we will simply depend on
// id3v2 lib being installed.
//
// Revision 1.5  2000/04/11 03:38:15  eldamitri
// Updated comments for use with doxygen.
//
// Revision 1.4  2000/04/07 04:40:32  eldamitri
// Changed the version functions to reflect the name change of release
// and revision to major and minor.
//
// Revision 1.3  2000/04/05 05:20:52  eldamitri
// Updated initial comment information to reflect license, copyright
// change.
//
// Revision 1.2  1999/12/17 16:05:02  scott
// Updated opening comment block.
//
// Revision 1.1  1999/12/01 17:16:12  scott
// moved from src/id3 to include/id3
//
// Revision 1.8  1999/11/30 20:16:35  scott
// Changed from preprocessor macros to functions.  Now changes to top
// level configure.in with regards to version info will be reflected in
// these functions without having to change this file.
//
// Revision 1.7  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.6  1999/11/29 19:13:51  scott
// Updated documentation to work better with doc++ (probably doesn't work
// well with kdoc anymore).  Updated the constants.
//
// Revision 1.5  1999/11/25 19:25:46  scott
// Added doc++/kdoc/javadoc-like documentation.  Replaced #define's with
// const's.
//
// Revision 1.4  1999/11/04 04:15:55  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//

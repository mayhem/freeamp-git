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

#ifndef ID3LIB_HEADER_TAG_H
#define ID3LIB_HEADER_TAG_H

#include "types.h"
#include "header.h"

#define ID3_TAGID               "ID3"
#define ID3_TAGIDSIZE           (3)
#define ID3_TAGHEADERSIZE       (10)

#define ID3HF_UNSYNC            (1 << 7)
#define ID3HF_EXTENDEDHEADER    (1 << 6)
#define ID3HF_EXPERIMENTAL      (1 << 5)

class ID3_TagHeader : public ID3_Header
{
public:
  virtual size_t Size(void);
  virtual size_t Render(uchar *buffer);
  ID3_TagHeader& operator=(const ID3_TagHeader&);
};

lsint ID3_IsTagHeader(uchar header[ID3_TAGHEADERSIZE]);

#endif

// $Log$
// Revision 1.1  2000/04/17 18:48:14  robert
// I've added the header files for the id3v2 lib and then I will also
// be checking in the .DLL for this. For unix we will simply depend on
// id3v2 lib being installed.
//
// Revision 1.6  2000/04/10 16:57:05  eldamitri
// (class ID3_TagHeader): Added decl for operator=.
//
// Revision 1.5  2000/04/05 05:20:52  eldamitri
// Updated initial comment information to reflect license, copyright
// change.
//
// Revision 1.4  1999/12/27 06:06:54  scott
// (ID3_TAGIDSIZE): Added constant.
// (class ID3_TagHeader): Minor return type changes for Size and Render.
//
// Revision 1.3  1999/12/17 16:05:02  scott
// Updated opening comment block.
//
// Revision 1.2  1999/12/02 22:45:28  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places.
//
// Revision 1.1  1999/12/01 17:16:06  scott
// moved from src/id3 to include/id3
//
// Revision 1.5  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.4  1999/11/04 04:15:55  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//

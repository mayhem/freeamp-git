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

#ifndef ID3LIB_HEADER_FRAME_H
#define ID3LIB_HEADER_FRAME_H

#include "types.h"
#include "header.h"
#include "header_tag.h"
#include "field.h"

#define ID3FL_TAGALTER    (1 << 15)
#define ID3FL_FILEALTER   (1 << 14)
#define ID3FL_READONLY    (1 << 13)
#define ID3FL_COMPRESSION (1 <<  7)
#define ID3FL_ENCRYPTION  (1 <<  6)
#define ID3FL_GROUPING    (1 <<  5)

class ID3_FrameHeader : public ID3_Header
{
public:
  ID3_FrameHeader();
  virtual ~ID3_FrameHeader();
  
  virtual size_t Size(void);
  virtual size_t Parse(uchar *buffer);
  virtual size_t Render(uchar *buffer);
  virtual void  SetFrameID(ID3_FrameID id);
  virtual ID3_FrameID GetFrameID() const;
  virtual const char *GetTextID(void) const;
  virtual const ID3_FrameDef *GetFrameDef() const;
  virtual void Clear();
  ID3_FrameHeader& operator=(const ID3_FrameHeader&);
 
protected:
  virtual void  SetUnknownFrame(const char*);

  ID3_FrameDef *__pFrameDef;
  bool __bDynFrameDef;
}
;

#endif

// $Log$
// Revision 1.1  2000/04/17 18:48:14  robert
// I've added the header files for the id3v2 lib and then I will also
// be checking in the .DLL for this. For unix we will simply depend on
// id3v2 lib being installed.
//
// Revision 1.8  2000/04/10 16:56:56  eldamitri
// (class ID3_FrameHeader): Added decl for operator=, removed decl of Copy
//
// Revision 1.7  2000/04/09 22:36:45  eldamitri
// (class ID3_FrameHeader): Added destructor and new method
// SetUnknownFrame(const char*) for creating a frame with an unknown
// frame id.  Added __bDynFrameDef member, set when __pFrameDef has been
// dynamically created.
//
// Revision 1.6  2000/04/05 05:20:52  eldamitri
// Updated initial comment information to reflect license, copyright
// change.
//
// Revision 1.5  1999/12/27 06:09:26  scott
// (ID3_FrameAttr): Removed.
// (class ID3_FrameHeader): Added declarations for default constructor and
// Parse, GetFrameID, GetTextID, GetFrameDef, and Clear methods.  Changed
// return type for Size and Render.  Replaced __eFrameID data member with
// __pFrameDef, which encapsulates more information about the frame
// header.
//
// Revision 1.4  1999/12/26 16:40:18  scott
// (ID3FL_READONLY): Renamed from ID3FL_SIGNED.
// (class ID3_FrameHeader): Minor cleanup to interface.
//
// Revision 1.3  1999/12/17 16:05:02  scott
// Updated opening comment block.
//
// Revision 1.2  1999/12/02 22:45:28  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places.
//
// Revision 1.1  1999/12/01 17:16:05  scott
// moved from src/id3 to include/id3
//
// Revision 1.6  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.5  1999/11/15 20:19:25  scott
// Made variable names more descriptive.
//
// Revision 1.4  1999/11/04 04:15:55  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//

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

#ifndef ID3LIB_HEADER_H
#define ID3LIB_HEADER_H

#include "types.h"

#define ID3v2_VERSION  (3)
#define ID3v2_REVISION (0)

struct ID3_HeaderInfo
{
  uchar ucVersion;
  uchar ucRevision;
  uchar ucFrameIDBytes;
  uchar ucFrameSizeBytes;
  uchar ucFrameFlagsBytes;
  bool  bHasExtHeader;
  luint ulExtHeaderBytes;
  bool  bSetExpBit;
};

extern ID3_HeaderInfo ID3_VersionInfo[];

class ID3_Header
{
public:
  ID3_Header(void);
  
  virtual void   SetVersion(uchar ver, uchar rev);
  virtual uchar  GetVersion() const;
  virtual uchar  GetRevision() const;
  virtual void   SetDataSize(size_t newSize);
  virtual size_t GetDataSize() const;
  virtual void   SetFlags(uint16 newFlags);
  virtual void   AddFlags(uint16 newFlags);
  virtual void   RemoveFlags(uint16 newFlags);
  virtual uint16 GetFlags() const;
  virtual void   Clear();
  virtual size_t Size(void) = 0;
  virtual size_t Render(uchar *buffer) = 0;

  ID3_Header &operator=( const ID3_Header & );

protected:
  uchar  __ucVersion;        // which version?
  uchar  __ucRevision;       // which revision?
  size_t __ulDataSize;       // how big is the data?
  uint16 __ulFlags;          // header flags
  ID3_HeaderInfo *__pInfo;   // the info about this version of the headers
}
;

ID3_HeaderInfo *ID3_LookupHeaderInfo(uchar ver, uchar rev);

#endif

// $Log$
// Revision 1.1  2000/04/17 18:48:14  robert
// I've added the header files for the id3v2 lib and then I will also
// be checking in the .DLL for this. For unix we will simply depend on
// id3v2 lib being installed.
//
// Revision 1.7  2000/04/10 16:56:48  eldamitri
// (class ID3_Header): Removed decl of Copy.
//
// Revision 1.6  2000/04/05 05:20:52  eldamitri
// Updated initial comment information to reflect license, copyright
// change.
//
// Revision 1.5  1999/12/27 06:11:33  scott
// (ID3_VERSION, ID3_REVISION): Renamed to ID3v2_*
// (class ID3_Header): Added declarations for GetVersion, GetRevision,
// AddFlags, SetFlags, Clear, Copy, and operator= methods.  Changed
// return type for Size and Render from luint to size_t.
//
// Revision 1.4  1999/12/26 16:40:13  scott
// (class ID3_Header): Minor cleanup to interface.
//
// Revision 1.3  1999/12/17 16:05:02  scott
// Updated opening comment block.
//
// Revision 1.2  1999/12/02 22:45:28  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places.
//
// Revision 1.1  1999/12/01 17:16:04  scott
// moved from src/id3 to include/id3
//
// Revision 1.6  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.5  1999/11/15 20:18:59  scott
// Made variable names more descriptive.
//
// Revision 1.4  1999/11/04 04:15:55  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//

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

#ifndef __ID3LIB_HEADER_FRAME_H__
#define __ID3LIB_HEADER_FRAME_H__

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

#endif /* __ID3LIB_HEADER_FRAME__ */

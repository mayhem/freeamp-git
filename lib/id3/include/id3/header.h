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

#ifndef __ID3LIB_HEADER_H__
#define __ID3LIB_HEADER_H__

#include "spec.h"

struct ID3_HeaderInfo
{
  ID3_V2Spec eSpec;
  uchar      ucFrameIDBytes;
  uchar      ucFrameSizeBytes;
  uchar      ucFrameFlagsBytes;
  bool       bHasExtHeader;
  luint      ulExtHeaderBytes;
  bool       bSetExpBit;
};

// extern ID3_HeaderInfo ID3_SpecInfo[];

class ID3_Header : public ID3_Speccable
{
public:
  ID3_Header(void);
  
  virtual void       SetSpec(const ID3_V2Spec);
  virtual ID3_V2Spec GetSpec() const;
  virtual void       SetDataSize(size_t newSize);
  virtual size_t     GetDataSize() const;
  virtual void       SetFlags(uint16 newFlags);
  virtual void       AddFlags(uint16 newFlags);
  virtual void       RemoveFlags(uint16 newFlags);
  virtual uint16     GetFlags() const;
  virtual void       Clear();
  virtual size_t     Size(void) = 0;
  virtual size_t     Render(uchar *buffer) = 0;

  ID3_Header &operator=( const ID3_Header & );

protected:
  ID3_V2Spec      __spec;             // which version of the spec 
  size_t          __ulDataSize;       // how big is the data?
  uint16          __ulFlags;          // header flags
  ID3_HeaderInfo* __pInfo;            // the info about this version of the headers
}
;

ID3_HeaderInfo* ID3_LookupHeaderInfo(ID3_V2Spec);

/* Deprecated */
ID3_HeaderInfo* ID3_LookupHeaderInfo(uchar ver, uchar rev);

#endif /* __ID3LIB_HEADER_H */

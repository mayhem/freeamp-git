// $Id$

// id3lib: a software library for creating and manipulating id3v1/v2 tags
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

#ifndef __ID3LIB_TAG_H__
#define __ID3LIB_TAG_H__

#include <stdio.h>
#include "header_tag.h"
#include "frame.h"
#include "spec.h"

struct ID3_Elem
{
  ID3_Elem  *pNext;
  ID3_Frame *pFrame;
  uchar     *acBinary;
  bool       bTagOwns;
};

/** String used for the description field of a comment tag converted from an
 ** id3v1 tag to an id3v2 tag
 **
 ** \sa #ID3V1_Tag
 **/
//const char STR_V1_COMMENT_DESC[] = "ID3v1_Comment";
const char STR_V1_COMMENT_DESC[] = "";

class ID3_Tag : public ID3_Speccable
{
public:
  ID3_Tag(char *name = NULL);
  ID3_Tag(const ID3_Tag &tag);
  virtual ~ID3_Tag();
  
  void       Clear();
  bool       HasChanged() const;
  void       SetUnsync(bool bSync);
  void       SetExtendedHeader(bool bExt);
  void       SetPadding(bool bPad);
  void       AddFrame(const ID3_Frame&);
  void       AddFrame(const ID3_Frame*);
  void       AttachFrame(ID3_Frame*);
  void       RemoveFrame(const ID3_Frame *);
  luint      Render(uchar*);
  luint      RenderV1(uchar*);
  luint      Size() const;
  size_t     Parse(const uchar header[ID3_TAGHEADERSIZE], const uchar *buffer);
  luint      Link(const char *fileInfo, luint tt = (luint) ID3TT_ALL);
  luint      Update(luint tt = (luint) ID3TT_ID3V2);
  luint      Strip(luint tt = (luint) ID3TT_ALL);

  //@{
  /// Finds frame with given frame id
  ID3_Frame* Find(ID3_FrameID id) const;

  /// Finds frame with given frame id, fld id, and integer data
  ID3_Frame* Find(ID3_FrameID id, ID3_FieldID fld, luint data) const;

  /// Finds frame with given frame id, fld id, and ascii data
  ID3_Frame* Find(ID3_FrameID id, ID3_FieldID fld, const char *) const;

  /// Finds frame with given frame id, fld id, and unicode data
  ID3_Frame* Find(ID3_FrameID id, ID3_FieldID fld, const unicode_t *) const;
  //@}

  luint      NumFrames() const;
  ID3_Frame* GetFrameNum(luint) const;
  ID3_Frame* operator[](luint) const;
  ID3_Tag&   operator=( const ID3_Tag & );

  /** Indicates whether the an V1 tag is present.
   **  
   ** This method was inserted for use by the ID3Com .dll.
   **
   ** @return Whether or not a V1 tag is present.
   **/
  bool HasV1Tag() const {return __has_v1_tag;};

  /** Indicates whether the an V2 tag is present.
   **
   ** This method was inserted for use by the ID3Com .dll.
   **
   ** @return Whether or not a V2 tag is present.
   **/
  bool HasV2Tag() const {return (__orig_tag_size > 0);};
     
  /** Indicates whether there are Lyrics present.
   **
   ** This method was inserted for use by the ID3Com .dll.
   **
   ** @return Whether or not Lyrics are present.
   **/
  bool HasLyrics() const {return __has_v1_tag;};

  static size_t IsV2Tag(const uchar*);

  /* Deprecated! */
  void       AddNewFrame(ID3_Frame* f) { this->AttachFrame(f); }
  luint      Link(const char *fileInfo, bool parseID3v1, bool parseLyrics3);
  void       SetCompression(bool) { ; }
  void       AddFrames(const ID3_Frame *, luint);

protected:
  size_t     ParseFrames(const uchar*, size_t);
  void       AddFrame(ID3_Frame , bool);
  void       AddFrames(ID3_Frame *, luint, bool);
  void       SetupTag(char *);
  bool       SetSpec(ID3_V2Spec);
  ID3_V2Spec GetSpec() const;
  void       ClearList(ID3_Elem *);
  void       DeleteElem(ID3_Elem *);
  void       RemoveFromList(ID3_Elem *, ID3_Elem **);
  ID3_Elem*  Find(const ID3_Frame *) const;
  luint      PaddingSize(luint) const;
  void       RenderExtHeader(uchar *);
  ID3_Err    OpenFileForWriting();
  ID3_Err    OpenFileForReading();
  void       CreateFile();
  bool       CloseFile();
  void       RenderV1ToHandle();
  void       RenderV2ToHandle();
  luint      ParseFromHandle();
  void       ParseID3v1();
  void       ParseLyrics3();
  luint      GetUnSyncSize(uchar *, luint);
  void       UnSync(uchar *, luint, uchar *, luint);
  luint      ReSync(uchar *, luint);

private:
  ID3_V2Spec __spec;            // what version of the spec are we using
  ID3_Elem*  __frames;          // the list of known frames currently attached to this tag
  ID3_Elem*  __binaries;        // the list of yet-to-be-parsed frames currently attached to this tag
  mutable ID3_Elem*  __cursor;  // on which element in the frameList are we currently positioned?
  bool       __is_unsync;       // should we unsync this tag when rendering?
  bool       __is_padded;       // add padding to tags?
  bool       __is_extended;     // create an extended header when rendering?
  bool       __changed;         // has the tag changed since the last parse or render?
  bool       __is_file_writable;// is the associated file (via Link) writable?
  FILE*      __file_handle;     // a handle to the file we are linked to
  luint      __file_size;       // the size of the file (without any tag)
  luint      __orig_tag_size;   // the size of the old tag (if any)
  luint      __extra_bytes;     // extra bytes to strip from end of file (ID3v1 and Lyrics3 tags)
  bool       __has_v1_tag;      // does the file have an ID3v1 tag attached?
  luint      __tags_to_parse;   // which tag types should be parsed
  char*      __file_name;       // name of the file we are linked to
  ID3_TagHeader __hdr;
  static luint __instances;     // how many ID3_Tag objects are floating around in this app?
};

//@{
/// Copies
ID3_Tag& operator<<(ID3_Tag&, const ID3_Frame &);
/// Attaches a pointer to a frame
ID3_Tag& operator<<(ID3_Tag&, const ID3_Frame *);
//@}

// deprecated!
lsint ID3_IsTagHeader(const uchar header[ID3_TAGHEADERSIZE]);

#endif /* __ID3LIB_TAG_H__ */

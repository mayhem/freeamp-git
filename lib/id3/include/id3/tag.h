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
#include "frame.h"
#include "header_frame.h"
#include "header_tag.h"
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
 ** @see #ID3V1_Tag
 **/
const char STR_V1_COMMENT_DESC[] = "ID3v1_Comment";

/** The representative class of an id3 tag.
 ** 
 ** This is the 'container' class for everything else.  It is through an
 ** ID3_Tag that most of the productive stuff happens.  Let's look at what's
 ** required to start using ID3v2 tags.
 ** 
 ** \code
 **   #include <id3/tag.h>
 ** \endcode
 ** 
 ** This simple \c #include does it all.  In order to read an
 ** existing tag, do the following:
 **
 ** \code
 **   ID3_Tag myTag;
 **   myTag.Link("something.mp3");
 ** \endcode
 ** 
 ** That is all there is to it.  Now all you have to do is use the 
 ** <a href="#Find">Find</a> method to locate the frames you are interested in
 ** is the following:
 ** 
 ** \code
 **   ID3_Frame *myFrame;
 **   if (myTag.Find(ID3FID_TITLE) == myFrame)
 **   {
 **     char title[1024];
 **     myFrame->Field(ID3FN_TEXT).Get(title, 1024);
 **     cout << "Title: " << title << endl;
 **   }
 ** \endcode
 ** 
 ** This code snippet locates the TITLE frame and copies the contents of the
 ** text field into a buffer and displays the buffer.  Not difficult, eh?
 **
 ** When using the <a href="#Link">Link</a> method of an ID3_Tag object, you
 ** automatically gain access to any ID3v1/1.1, ID3v2, and Lyrics3 v2.0 tags
 ** present in the file.  The class will automaticaly parse and convert any of
 ** these foreign tag formats into ID3v2 tags.  Also, id3lib will correctly
 ** parse any correctly formatted 'CDM' frames from the unreleased 2.01 draft
 ** that id3lib 2.16 supports.
 **
 ** @author Dirk Mahoney
 ** @version $Id$
 ** @see ID3_Frame
 ** @see ID3_Field
 ** @see ID3_Err
 **/
class ID3_Tag : public ID3_Speccable
{
public:
  ID3_Tag(char *name = NULL);
  ID3_Tag(const ID3_Tag &tag);
  ~ID3_Tag(void);
  
  void       Clear(void);
  bool       HasChanged(void) const;
  void       SetUnsync(bool bSync);
  void       SetExtendedHeader(bool bExt);
  void       SetCompression(bool bComp);
  void       SetPadding(bool bPad);
  void       AddFrame(const ID3_Frame&);
  void       AddFrame(const ID3_Frame*);
  void       AttachFrame(ID3_Frame*);
  void       AddFrames(const ID3_Frame *frames, luint num_frames);
  void       RemoveFrame(ID3_Frame *pOldFrame);
  luint      Render(uchar *buffer);
  luint      RenderV1(char *buffer);
  luint      Size(void) const;
  void       Parse(uchar header[ID3_TAGHEADERSIZE], uchar *buffer);
  luint      Link(char *fileInfo, const luint tt = (luint) ID3TT_ALL);
  luint      Update(const luint tt = (luint) ID3TT_ID3V2);
  luint      Strip(const luint tt = (luint) ID3TT_ALL);

  //@{
  /// Finds frame with given frame id
  ID3_Frame* Find(ID3_FrameID id);

  /// Finds frame with given frame id, fld id, and integer data
  ID3_Frame* Find(ID3_FrameID id, ID3_FieldID fld, luint data);

  /// Finds frame with given frame id, fld id, and ascii data
  ID3_Frame* Find(ID3_FrameID id, ID3_FieldID fld, char *data);

  /// Finds frame with given frame id, fld id, and unicode data
  ID3_Frame* Find(ID3_FrameID id, ID3_FieldID fld, unicode_t *data);
  //@}

  luint      NumFrames(void) const;
  ID3_Frame* GetFrameNum(luint nIndex) const;
  ID3_Frame* operator[](luint nIndex) const;
  ID3_Tag&   operator=( const ID3_Tag &rTag );

  /** Indicates whether the an V1 tag is present.
   **  
   ** This method was inserted for use by the ID3Com .dll.
   **
   ** @return Whether or not a V1 tag is present.
   **/
  bool HasV1Tag() {return __bHasV1Tag;};

  /** Indicates whether the an V2 tag is present.
   **
   ** This method was inserted for use by the ID3Com .dll.
   **
   ** @return Whether or not a V2 tag is present.
   **/
  bool HasV2Tag() {return (__ulOldTagSize > 0);};
     
  /** Indicates whether there are Lyrics present.
   **
   ** This method was inserted for use by the ID3Com .dll.
   **
   ** @return Whether or not Lyrics are present.
   **/
  bool HasLyrics() {return __bHasV1Tag;};

  /* Deprecated! */
  void       AddNewFrame(ID3_Frame* f) { this->AttachFrame(f); }
  luint      Link(char *fileInfo, bool parseID3v1, bool parseLyrics3);

protected:
  void       AddFrame(ID3_Frame *pNewFrame, bool bFreeWhenDone);
  void       AddFrames(ID3_Frame *newFrames, luint nFrames, bool freeWhenDone);
  void       SetupTag(char *fileInfo);
  void       SetSpec(const ID3_V2Spec);
  ID3_V2Spec GetSpec() const;
  void       ClearList(ID3_Elem *list);
  void       DeleteElem(ID3_Elem *cur);
  void       AddBinary(uchar *buffer, luint size);
  void       ExpandBinaries(uchar *buffer, luint size);
  void       ProcessBinaries(ID3_FrameID whichFrame = ID3FID_NOFRAME, bool attach = true);  // this default means all frames
  void       RemoveFromList(ID3_Elem *which, ID3_Elem **list);
  ID3_Elem*  GetLastElem(ID3_Elem *list);
  ID3_Elem*  Find(ID3_Frame *frame) const;
  luint      PaddingSize(luint curSize) const;
  void       RenderExtHeader(uchar *buffer);
  ID3_Err    OpenFileForWriting(void);
  ID3_Err    OpenFileForReading(void);
  void       CreateFile(void);
  bool       CloseFile(void);
  void       RenderV1ToHandle(void);
  void       RenderV2ToHandle(void);
  luint      ParseFromHandle(void);
  void       ParseID3v1(void);
  void       ParseLyrics3(void);
  luint      GetUnSyncSize(uchar *buffer, luint size);
  void       UnSync(uchar *destData, luint destSize, uchar *sourceData, luint sourceSize);
  luint      ReSync(uchar *binarySourceData, luint sourceSize);

private:
  ID3_V2Spec __spec;            // what version of the spec are we using
  ID3_Elem*  __pFrameList;      // the list of known frames currently attached to this tag
  ID3_Elem*  __pBinaryList;     // the list of yet-to-be-parsed frames currently attached to this tag
  ID3_Elem*  __pFindCursor;     // on which element in the frameList are we currently positioned?
  bool       __bSyncOn;         // should we unsync this tag when rendering?
  bool       __bCompression;    // should we compress frames when rendering?
  bool       __bPadding;        // add padding to tags?
  bool       __bExtendedHeader; // create an extended header when rendering?
  bool       __bHasChanged;     // has the tag changed since the last parse or render?
  bool       __bFileWritable;   // is the associated file (via Link) writable?
  FILE*      __fFileHandle;     // a handle to the file we are linked to
  luint      __ulFileSize;      // the size of the file (without any tag)
  luint      __ulOldTagSize;    // the size of the old tag (if any)
  luint      __ulExtraBytes;    // extra bytes to strip from end of file (ID3v1 and Lyrics3 tags)
  bool       __bHasV1Tag;       // does the file have an ID3v1 tag attached?
  luint      __ulTagsToParse;   // which tag types should be parsed
  char*      __sFileName;       // name of the file we are linked to
  static luint s_ulInstances;   // how many ID3_Tag objects are floating around in this app?
};

//@{
/// Copies
ID3_Tag& operator<<(ID3_Tag&, const ID3_Frame &);
/// Attaches a pointer to a frame
ID3_Tag& operator<<(ID3_Tag&, const ID3_Frame *);
//@}

#endif

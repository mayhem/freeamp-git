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

#ifndef ID3LIB_TAG_H
#define ID3LIB_TAG_H

#include <stdio.h>
#include "types.h"
#include "frame.h"
#include "header_frame.h"
#include "header_tag.h"
#include "version.h"

#if defined WIN32
#define MAXPATHLEN 1024
#else
#include <sys/param.h>
#endif

struct ID3_Elem
{
  ID3_Elem  *pNext;
  ID3_Frame *pFrame;
  uchar     *acBinary;
  bool       bTagOwns;
};

/** The length of an id3v1 tag
 **    
 ** @see ID3V1_Tag
 **/
const luint LEN_V1         = 128;
/** The length of the id field in an id3v1 tag
 ** 
 ** @see ID3V1_Tag#sID
 **/
const luint LEN_V1_ID      =   3;
/** The length of the title field in an id3v1 tag
 ** 
 ** @see ID3V1_Tag#sTitle
 **/
const luint LEN_V1_TITLE   =  30;
/** The length of the artist field in an id3v1 tag
 **
 ** @see ID3V1_Tag#sArtist
 **/
const luint LEN_V1_ARTIST  =  30;
/** The length of the album field in an id3v1 tag
 **
 ** @see ID3V1_Tag#sAlbum
 **/
const luint LEN_V1_ALBUM   =  30;
/** The length of the year field in an id3v1 tag 
 ** 
 ** @see ID3V1_Tag#sYear
 **/
const luint LEN_V1_YEAR    =   4;
/** The length of the comment field in an id3v1 tag
 **
 ** @see ID3V1_Tag#sComment
 **/
const luint LEN_V1_COMMENT =  30;
/** The length of the genre field in an id3v1 tag
 ** 
 ** @see ID3V1_Tag#ucGenre
 **/
const luint LEN_V1_GENRE   =   1;

/** A structure for storing the contents of an id3v1 tag.
 **
 ** @author Dirk Mahoney (dirk@id3.org)
 ** @author Scott Thomas Haug (sth2@cs.wustl.edu)
 ** @version $Id$
 ** @see ID3_Tag
 **/
struct ID3V1_Tag
{
  /// String for storing the "TAG" identifier.
  char sID     [1 + LEN_V1_ID];
  /// String for storing the title
  char sTitle  [1 + LEN_V1_TITLE];
  /// String for storing the artist
  char sArtist [1 + LEN_V1_ARTIST];
  /// String for storing the album
  char sAlbum  [1 + LEN_V1_ALBUM];
  /// String for storing the year
  char sYear   [1 + LEN_V1_YEAR];
  /// String for storing the comment
  char sComment[1 + LEN_V1_COMMENT];
  /// Byte for storing the genre
  uchar ucGenre;
};

/** String used for the description field of a comment tag converted from an
 ** id3v1 tag to an id3v2 tag
 **
 ** @see #ID3V1_Tag
 **/
const char STR_V1_COMMENT_DESC[] = "ID3v1_Comment";

/** Flag used to represent the id3v1 tag
 **
 ** @see ID3_Tag#Update
 ** @see ID3_Tag#Strip
 **/
enum ID3_TagType
{
  /// Represents an empty or non-existant tag
  NO_TAG        =      0,
  /// Represents an id3v1 or id3v1.1 tag
  V1_TAG        = 1 << 0,
  /// Represents an id3v2 tag
  V2_TAG        = 1 << 1,
  /// Represents a Lyrics tag
  LYRICS_TAG    = 1 << 2
};

/** Represents both id3 tags: id3v1 and id3v2
 ** 
 ** @see ID3_TagType#V1_TAG
 ** @see ID3_TagType#V2_TAG
 **/
const luint BOTH_ID3_TAGS = V1_TAG | V2_TAG;

/** Represents all possible types of tags
 ** 
 ** @see ID3_TagType
 **/
const luint ALL_TAG_TYPES = BOTH_ID3_TAGS | LYRICS_TAG;

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
class ID3_Tag
{
public:
  /** Default constructor; it can accept an optional filename as a parameter.
   **
   ** If this file exists, it will be opened and all id3lib-supported tags will
   ** be parsed and converted to id3v2 if necessary.  After the conversion, the
   ** file will remain unchanged, and will continue to do so until you use the
   ** <a href="#Update"><code>Update</code></a> method on the tag (if you
   ** choose to <code>Update</code> at all).
   **
   ** @param name The filename of the mp3 file to link to
   **/
  ID3_Tag(char *name = NULL);
  /** Standard copy constructor.
   **
   ** @param tag What is copied into this tag
   **/
  ID3_Tag(const ID3_Tag &tag);
  ~ID3_Tag(void);
  
  /** Clears the object and disassociates it from any files.
   **
   ** It frees any resources for which the object is responsible, and the
   ** object is now free to be used again for any new or existing tag.
   **/
  void       Clear(void);
  
  /** Indicates whether the tag has been altered since the last parse, render,
   ** or update.
   **
   ** If you have a tag linked to a file, you do not need this method since the
   ** <a href="#Update">Update</a> method will check for changes before writing
   ** the tag.
   ** 
   ** This method is primarily intended as a status indicator for applications
   ** and for applications that use the <a href="#Parse">Parse</a> and <a
   ** href="#Render">Render</a> methods.
   **
   ** Setting a field, changed the ID of an attached frame, setting or grouping
   ** or encryption IDs, and clearing a frame or field all constitute a change
   ** to the tag, as do calls to the <a href="#SetUnsync">SetUnsync</a>, <a
   ** href="#SetExtendedHeader">SetExtendedHeader</a>, <a
   ** href="#SetPadding">SetPadding</a>, and <a
   ** href="#SetCompression">SetCompression</a> methods.
   
   ** \code
   **   if (myTag.HasChanged())
   **   {
   **     // render and output the tag
   **   }
   ** \endcode
   
   ** @return Whether or not the tag has been altered.
   **/
  bool       HasChanged(void) const;

  /** Turns unsynchronization on or off, dependant on the value of the boolean
   ** parameter.
   ** 
   ** If you call this method with 'false' as the parameter, the
   ** binary tag will not be unsync'ed, regardless of whether the tag should
   ** be.  This option is useful when the file is only going to be used by
   ** ID3v2-compliant software.  See the id3v2 standard document for futher
   ** details on unsync.
   **
   ** Be default, tags are created without unsync.
   ** 
   ** \code
   **   myTag.SetUnsync(false);
   ** \endcode
   ** 
   ** @param bSync Whether the tag should be unsynchronized
   **/
  void       SetUnsync(bool bSync);

  /** Turns extended header rendering on or off, dependant on the value of the
   ** boolean parameter.
   ** 
   ** This option is currently ignored as id3lib doesn't yet create extended
   ** headers.  This option only applies when rendering tags for id3v2 versions
   ** that support extended headers.
   **
   ** By default, id3lib will generate extended headers for all tags in which
   ** extended headers are supported.
   ** 
   ** \code
   **   myTag.SetExtendedHeader(true);
   ** \endcode
   ** 
   ** @param bExt Whether to render an extended header
   **/
  void       SetExtendedHeader(bool bExt);

  /** Turns compression on or off, dependant on the value of the boolean
   ** parameter.
   ** 
   ** Under id3v2 reversion 3, there is support for the compression of data in
   ** tags.  The current standard defines compression on a frame-by-frame basis
   ** using the zlib data compression standard.  Using this call, you tell the
   ** object whether to attempt to compress frames or not.  If a frame doesn't
   ** compress well, it will not be compressed, regardless of this setting.
   ** This method takes a single boolean parameter.  By default, id3lib will
   ** attempt to compress all the frames in a tag.
   ** 
   ** \code
   **   myTag.SetCompression(true);
   ** \endcode
   ** 
   ** When creating tags for a version of id3v2 in which compression isn't
   ** defined, the tags are simply rendered without compression to ensure
   ** compliance to the relevant version of the standard.
   ** 
   ** @param bComp Whether to render the tag's frames in a compressed format.
   **/
  void       SetCompression(bool bComp);

  /**Turns padding on or off, dependant on the value of the boolean
   ** parameter.
   ** 
   ** When using id3v2 tags in association with files, id3lib can optionally
   ** add padding to the tags to ensure minmal file write times when updating
   ** the tag in the future.
   ** 
   ** When the padding option is switched on, id3lib automatically creates
   ** padding according to the 'ID3v2 Programming Guidelines'.  Specifically,
   ** enough padding will be added to round out the entire file (song plus
   ** tag) to an even multiple of 2K.  Padding will only be created when the
   ** tag is attached to a file and that file is not empty (aside from a
   ** pre-existing tag).
   ** 
   ** id3lib's addition to the guidelines for padding, is that if frames are
   ** removed from a pre-existing tag (or the tag simply shrinks because of
   ** other reasons), the new tag will continue to stay the same size as the
   ** old tag (with padding making the difference of course) until such time as
   ** the padding is greater than 4K.  When this happens, the padding will be
   ** reduced and the new tag will be smaller than the old.
   ** 
   ** By default, padding is switched on.
   ** 
   ** \code
   **   myTag.SetPadding(false);
   ** \endcode
   ** 
   ** @param bPad Whether or not render the tag with padding.
   **/
  void       SetPadding(bool bPad);

  /** Attaches a frame to the tag; the tag doesn't take responsibility for
   ** releasing the frame's memory when tag goes out of scope.
   ** 
   ** Optionally, <a href="operator<<">operator<<</a> can also be used to
   ** attach a frame to a tag.  To use, simply supply its parameter a pointer
   ** to the ID3_Frame object you wish to attach.
   ** 
   ** \code
   **   ID3_Frame myFrame;
   **   myTag.AddFrame(&myFrame);
   ** \endcode 
   ** 
   ** As stated, this method attaches the frames to the tag---the tag does
   ** not create its own copy of the frame.  Frames created by an application
   ** must exist until the frame is removed or the tag is finished with it.
   ** 
   ** @param pFrame A pointer to the frame that is being added to the tag.
   ** @see ID3_Frame
   **/
  void       AddFrame(ID3_Frame *pFrame);

  /** Attaches a frame to the tag; the tag takes responsibility for
   ** releasing the frame's memory when tag goes out of scope.
   ** 
   ** Unlike <a href="#AddFrame">AddFrame</a>, this method accepts
   ** responsibility for the added frame, and will delete the frame and its
   ** contents when the tag goes out of scope or is deleted.
   ** 
   ** \code
   **   ID3_Frame *myFrame = new ID3_Frame;
   **   myTag.AddFrame(myFrame);
   ** \endcode
   ** 
   ** @param pNewFrame A pointer to the frame that is being added to the tag.
   **/
  void       AddNewFrame(ID3_Frame *pNewFrame);

  /** Attaches a frame to the tag; the tag doesn't take responsibility for
   ** releasing the frame's memory when tag goes out of scope.
   ** 
   ** Operator<< supports the addition of a pointer to a frame object, or
   ** the frame object itself.
   **
   ** \code
   **   ID3_Frame *framePoint, frameObj;
   **   myTag << framePoint;
   **   myTag << frameObj;
   ** \endcode
   **
   ** Both these methods attach the given frame to the tag---the tag does not
   ** create its own copy of the frame.  Frames created by an application must
   ** exist until the frame is removed or the tag is finished with it.
   ** 
   ** @name operator<<
   ** @param frame The frame to be added to the tag.
   **/
  //@{
  /// Attaches a reference to a frame
  ID3_Tag& operator<<(ID3_Frame &frame);
  /// Attaches a pointer to a frame
  ID3_Tag& operator<<(ID3_Frame *frame);
  //@}

  /** Attaches an array of frames to the tag; the tag doesn't take
   **responsibility for releasing the frames' memory when the tag goes out of
   ** scope.
   ** 
   ** This method attaches each frame in an array to the tag.  As in 
   ** <a href="#AddFrame">AddFrame</a>, the tag doesn't take responsiblity
   ** for freeing the frames' memory when the tag goes out of scope.
   ** 
   ** \code
   **   ID3_Frame myFrames[10];
   **   myTag.AddFrames(myFrames, 10);
   ** \endcode
   ** 
   ** @see ID3_Frame
   ** @see ID3_Frame#AddFrame
   ** @param pNewFrames A pointer to an array of frames to be added to the tag.
   ** @param nFrames The number of frames in the array pNewFrames.
   **/
  void AddFrames(ID3_Frame *pNewFrames, luint nFrames);

  /**Removes a frame from the tag.
   ** 
   ** If you already own the frame object in question, then you should already
   ** have a pointer to the frame you want to delete.  If not, or if you wish
   ** to delete a pre-existing frame (from a tag you have parsed, for example),
   ** the use one of the <a href="#Find">Find</a> methods to obtain a frame
   ** pointer to pass to this method.
   ** 
   ** \code
   **   ID3_Frame *someFrame;
   **   if (someFrame = myTag.Find(ID3FID_TITLE))
   **   {
   **     myTag.RemoveFrame(someFrame);
   **   }
   ** \endcode
   **   
   ** @see ID3_Tag#Find
   ** @param pOldFrame A pointer to the frame that is to be removed from the
   **                  tag
   **/
  void RemoveFrame(ID3_Frame *pOldFrame);

  /** Renders a binary image of the tag into the supplied buffer.
   ** 
   ** See <a href="#Size">Size</a> for an example.  This method returns the
   ** actual number of the bytes of the buffer used to store the tag.  This
   ** will be no more than the size of the buffer itself, because
   ** <a href="#Size">Size</a> over estimates the required buffer size when
   ** padding is enabled.
   ** 
   ** Before calling this method, it is advisable to call <a
   ** href="#HasChanged">HasChanged</a> first as this will let you know
   ** whether you should bother rendering the tag.
   ** 
   ** @see    ID3_IsTagHeader
   ** @see    ID3_Tag#HasChanged
   ** @return The actual number of the bytes of the buffer used to store the
   **         tag
   ** @param  buffer The buffer that will contain the rendered tag.
   **/
  luint Render(uchar *buffer);

  /** Renders an id3v1.1 version of the tag into the supplied buffer.
   ** 
   ** @return The actual number of the bytes of the buffer used to store the
   **         tag (should always be 128)
   ** @param  buffer The buffer that will contain the id3v1.1 tag.
   **/
  luint RenderV1(char *buffer);

  /** Returns an over estimate of the number of bytes required to store a
   ** binary version of a tag. 
   ** 
   ** When using <a href="#Render">Render</a> to render a binary tag to a
   ** memory buffer, first use the result of this call to allocate a buffer of
   ** unsigned chars.
   ** 
   ** \code
   **   luint tagSize;
   **   uchar *buffer;
   **   if (myTag.HasChanged())
   **   {
   **     if ((tagSize= myTag.Size()) > 0)
   **     {
   **       if (buffer = new uchar[tagSize])
   **       {
   **         luint actualSize = myTag.Render(buffer);
   **         // do something useful with the first
   **         // 'actualSize' bytes of the buffer,
   **         // like push it down a socket
   **         delete [] buffer;
   **       }
   **     }
   **   }
   ** \endcode
   **
   ** @see #Render
   ** @return The (overestimated) number of bytes required to store a binary
   **         version of a tag
   **/
  luint      Size(void) const;

  /** Turns a binary tag into a series of ID3_Frame objects attached to the
   ** tag.
   ** 
   ** \code
   **   ID3_Tag myTag;
   **   uchar header[ID3_TAGHEADERSIZE];
   **   uchar *buffer;
   **   luint tagSize;
   ** 
   **   // get ID3_TAGHEADERSIZE from a socket or somewhere
   **   // ...
   ** 
   **   if ((tagSize = ID3_IsTagHeader(ourSourceBuffer)) > -1)
   **   {
   **     // read a further 'tagSize' bytes in
   **     // from our data source
   **     // ...
   **   
   **     if (buffer = new uchar[tagSize])
   **     {
   **       // now we will call ID3_Tag::Parse()
   **       // with these values (explained later)
   **       myTag.Parse(header, buffer);
   **       
   **       // do something with the objects,
   **       // like look for titles, artists, etc.
   **       // ...
   **       
   **       // free the buffer
   **       delete [] buffer;
   **     }
   **   }
   ** \endcode
   ** 
   ** @see ID3_Frame
   ** @param header The byte header read in from the data source.
   ** @param buffer The remainder of the tag (not including the data source) 
   **               read in from the data source.
   **/
  void       Parse(uchar header[ID3_TAGHEADERSIZE], uchar *buffer);

  /** Attaches a file to the tag, parses the file, and adds any tag information
   ** found in the file to the tag.
   ** 
   ** Use this method if you created your ID3_Tag object without supplying a
   ** parameter to the constructor (maybe you created an array of ID3_Tag
   ** pointers).  This is the preferred method of interacting with files, since
   ** id3lib can automatically do things like parse foreign tag formats and
   ** handle padding when linked to a file.  When a tag is linked to a file,
   ** you do not need to use the <a href="#Size">Size</a>, <a
   ** href="#Render">Render</a>, or <a href="#Parse">Parse</a> methods or the
   ** <code>ID3_IsTagHeader</code> function---id3lib will take care of those
   ** details for you.  The single parameter is a pointer to a file name.
   ** 
   ** Link returns a 'luint' which is the byte position within the file that
   ** the audio starts (i.e., where the id3v2 tag ends).
   ** 
   ** \code
   **   ID3_Tag *myTag;
   **   if (myTag = new ID3_Tag)
   **   {
   **     myTag->Link("mysong.mp3");
   **     
   **     // do whatever we want with the tag
   **     // ...
   **   
   **     // setup all our rendering parameters
   **     myTag->SetUnsync(false);
   **     myTag->SetExtendedHeader(true);
   **     myTag->SetCompression(true);
   **     myTag->SetPadding(true);
   **     
   **     // write any changes to the file
   **     myTag->Update()
   **     
   **     // free the tag
   **     delete myTag;
   **   }
   ** \endcode
   ** 
   ** @see ID3_IsTagHeader
   ** @param fileInfo The filename of the file to link to.
   **/

  /** Attaches a file to the tag, parses the file, and adds any tag informatio
   ** found in the file to the tag.  Also decides if to parse the id3v1 tag, 
   ** or the Lyrics3 tag 
   **/
  luint      Link(char *fileInfo, bool parseID3v1 = true, 
                  bool parseLyrics3 = true);

  /** Renders the tag and writes it to the attached file; the type of tag 
   ** rendered can be specified as a parameter.
   ** 
   ** Make sure the rendering parameters areset before calling the method.
   ** See <a href="#Link">Link</a> for an example of this method in use.
   ** 
   ** @param ulTagFlag Flag which indicates which tag to update.  Possibilities
   **                  are V1_TAG, V2_TAG, and BOTH_ID3_TAGS.
   **/
  luint      Update(const luint ulTagFlag = V2_TAG);

  /** Strips the tag(s) from the attached file; the type of tag stripped
   ** (either V1_TAG, V2_TAG, or BOTH_ID3_TAGS) can be specified as a
   ** parameter.
   ** 
   ** @param ulTagFlag Flag which indicates which tag to strip.  Possibilities
   **                  are V1_TAG, V2_TAG, and BOTH_ID3_TAGS.
   ** @see V1_TAG
   ** @see V2_TAG
   ** @see BOTH_ID3_TAGS
   **/
  luint      Strip(const luint ulTagFlag = BOTH_ID3_TAGS);

  /** Returns a pointer to the next ID3_Frame with the given ID3_FrameID;
   ** returns NULL if no such frame found.
   ** 
   ** If there are multiple frames in the tag with the same ID (which, for some
   ** frames, is allowed), then subsequent calls to <a href="#Find">Find</a>
   ** will return subsequent frame pointers, wrapping if necessary.
   ** 
   ** \code
   **   ID3_Frame *myFrame;
   **   if (myFrame = myTag.Find(ID3FID_TITLE))
   **   {
   **     // do something with the frame, like copy
   **     // the contents into a buffer, display the
   **     // contents in a window, etc.
   **     // ...
   **   }
   ** \endcode
   ** 
   ** You may optionally supply to more parameters ot this method, being an
   ** ID3_FieldID and a value of some sort.  Depending on the field name/ID you
   ** supply, you may supply an integer, a char* or a unicode_t* as the third
   ** parameter.  If you supply an ID3_FrameID, you must also supply a data
   ** value to compare against.
   ** 
   ** This method will then return the first frame that has a matching frame
   ** ID, and which has a field with the same name as that which you supplied
   ** in the second parameter, whose calue matches that which you supplied as
   ** the third parameter.  For example:
   ** 
   ** \code
   **   ID3_Frame *myFrame;
   **   if (myFrame = myTag.Find(ID3FID_TITLE, ID3FN_TEXT, "Nirvana"))
   **   {
   **     // found it, do something with it.
   **     // ...
   **   }
   ** \endcode
   **     
   ** This example will return the first TITLE frame and whose TEXT field is
   ** 'Nirvana'.  Currently there is no provision for things like 'contains',
   ** 'greater than', or 'less than'.  If there happens to be more than one of
   ** these frames, subsequent calls to the <a href="#Find">Find</a> method
   ** will return subsequent frames and will wrap around to the beginning.
   ** 
   ** Another example...
   ** 
   ** \code
   **   ID3_Frame *myFrame;
   **   if (myFrame = myTag.Find(ID3FID_COMMENT, ID3FN_TEXTENC, ID3TE_UNICODE))
   **   {
   **     // found it, do something with it.
   **     // ...
   **   }
   ** \endcode
   ** 
   ** This returns the first COMMENT frame that uses Unicode as its text
   ** encdoing.
   **  
   ** @name   Find
   ** @param  id The ID of the frame that is to be located
   ** @return A pointer to the first frame found that has the given frame id,
   **         or NULL if no such frame.
   **/
  //@{
  /// Finds frame with given frame id
  ID3_Frame *Find(ID3_FrameID id);

  /// Finds frame with given frame id, fld id, and integer data
  ID3_Frame *Find(ID3_FrameID id, ID3_FieldID fld, luint data);

  /// Finds frame with given frame id, fld id, and ascii data
  ID3_Frame *Find(ID3_FrameID id, ID3_FieldID fld, char *data);

  /// Finds frame with given frame id, fld id, and unicode data
  ID3_Frame *Find(ID3_FrameID id, ID3_FieldID fld, unicode_t *data);
  //@}

  /** Returns the number of frames present in the tag object.
   ** 
   ** This includes only those frames that id3lib recognises.  This is used as
   ** the upper bound on calls to the <a href="#GetFrame">GetFrame</a> and <a
   ** href="#operator[]">operator[]</a> methods.
   ** 
   ** @return The number of frames present in the tag object.
   **/
  luint      NumFrames(void) const;

  /** Returns a pointer to the frame with the given index; returns NULL if
   ** there is no such frame at that index.
   ** 
   ** Optionally, <a href="#operator[]">operator[]</a> can be used as an
   ** alternative to this method.  Indexing is 0-based (that is, the first
   ** frame is number 0, and the last frame in a tag that holds n frames is
   ** n-1).
   ** 
   ** If you wish to have a more comlex searching facility, then at least for
   ** now you will have to devise it yourself and implement it useing these
   ** methods.
   ** 
   ** @param nIndex The index of the frame that is to be retrieved
   ** @return A pointer to the requested frame, or NULL if no such frame.
   **/
  ID3_Frame *GetFrameNum(luint nIndex) const;

  /** Returns a pointer to the frame with the given index; returns NULL if
   ** there is no such frame at that index.
   ** 
   ** @name operator[]
   ** @param nIndex The index of the frame that is to be retrieved
   ** @return A pointer to the requested frame, or NULL if no such frame. 
   ** @see #GetFrameNum
   **/
  ID3_Frame *operator[](luint nIndex) const;
  
  ID3_Tag &operator=( const ID3_Tag &rTag );

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


protected:
  void      AddFrame(ID3_Frame *pNewFrame, bool bFreeWhenDone);
  void      AddFrames(ID3_Frame *newFrames, luint nFrames, bool freeWhenDone);
  void      SetupTag(char *fileInfo);
  void      SetVersion(uchar ver, uchar rev);
  void      ClearList(ID3_Elem *list);
  void      DeleteElem(ID3_Elem *cur);
  void      AddBinary(uchar *buffer, luint size);
  void      ExpandBinaries(uchar *buffer, luint size);
  void      ProcessBinaries(ID3_FrameID whichFrame = ID3FID_NOFRAME, bool attach = true);  // this default means all frames
  void      RemoveFromList(ID3_Elem *which, ID3_Elem **list);
  ID3_Elem *GetLastElem(ID3_Elem *list);
  ID3_Elem *Find(ID3_Frame *frame) const;
  luint     PaddingSize(luint curSize) const;
  void      RenderExtHeader(uchar *buffer);
  void      OpenFileForWriting(void);
  void      OpenFileForReading(void);
  void      CreateFile(void);
  bool      CloseFile(void);
  void      RenderV1ToHandle(void);
  void      RenderV2ToHandle(void);
  luint     ParseFromHandle(void);
  void      ParseID3v1(void);
  void      ParseLyrics3(void);
  luint     GetUnSyncSize(uchar *buffer, luint size);
  void      UnSync(uchar *destData, luint destSize, uchar *sourceData, luint sourceSize);
  luint     ReSync(uchar *binarySourceData, luint sourceSize);

private:
  uchar     __ucVersion;       // what version tag?
  uchar     __ucRevision;      // what revision tag?
  ID3_Elem *__pFrameList;      // the list of known frames currently attached to this tag
  ID3_Elem *__pBinaryList;     // the list of yet-to-be-parsed frames currently attached to this tag
  ID3_Elem *__pFindCursor;     // on which element in the frameList are we currently positioned?
  bool      __bSyncOn;         // should we unsync this tag when rendering?
  bool      __bCompression;    // should we compress frames when rendering?
  bool      __bPadding;        // add padding to tags?
  bool      __bExtendedHeader; // create an extended header when rendering?
  bool      __bHasChanged;     // has the tag changed since the last parse or render?
  bool      __bFileWritable;   // is the associated file (via Link) writable?
  FILE     *__fFileHandle;     // a handle to the file we are linked to
  luint     __ulFileSize;      // the size of the file (without any tag)
  luint     __ulOldTagSize;    // the size of the old tag (if any)
  luint     __ulExtraBytes;    // extra bytes to strip from end of file (ID3v1 and Lyrics3 tags)
  bool      __bHasV1Tag;       // does the file have an ID3v1 tag attached?
  bool      __bParseID3v1;     // do we parse the ID3v1 tag?
  bool      __bParseLyrics3;   // do we parse the Lyrics3 tag?
  char      __sFileName[MAXPATHLEN + 1]; // name of the file we are linked to
  static luint s_ulInstances;  // how many ID3_Tag objects are floating around in this app?
}
;

#endif

// $Log$
// Revision 1.1  2000/04/17 18:48:14  robert
// I've added the header files for the id3v2 lib and then I will also
// be checking in the .DLL for this. For unix we will simply depend on
// id3v2 lib being installed.
//
// Revision 1.13  2000/04/11 03:38:15  eldamitri
// Updated comments for use with doxygen.
//
// Revision 1.12  2000/04/10 03:40:56  eldamitri
// Started updating comments for doxygen.
//
// Revision 1.11  2000/04/08 04:32:36  eldamitri
// Changed new ANSI-standard C++ include headers to old-style headers.
//
// Revision 1.10  2000/04/07 19:35:14  eldamitri
// Added HasV1Tag, HasV2Tag, and HasLyrics (thanks John Adcock).
//
// Revision 1.9  2000/04/07 04:45:31  eldamitri
// Minor cleanup for log comments.
//
// Revision 1.8  2000/04/07 04:29:58  eldamitri
// Added optional parameters to Link to make parsing of id3v1/lyrics3
// tags optional.
//
// Revision 1.7  2000/04/05 05:20:52  eldamitri
// Updated initial comment information to reflect license, copyright
// change.
//
// Revision 1.6  2000/01/04 15:42:22  eldamitri
// For compilation with gcc 2.95.2 and better compatibility with ANSI/ISO
// standard C++, updated, rearranged, and removed (where necessary)
// #include directives.
//
// Revision 1.5  1999/12/17 16:05:02  scott
// Updated opening comment block.
//
// Revision 1.4  1999/12/13 04:23:25  scott
// (): Include sys/param.h (if available) to define MAXPATHLEN.
// (class ID3_Tag): Made private methods protected.  Changed __sFileName
// from a char * to a char array of size MAXPATHLEN+1.
//
// Revision 1.3  1999/12/09 02:45:59  scott
// (class ID3_Tag): Added copy constructor and operator= method
// declarations.
//
// Revision 1.2  1999/12/02 22:45:28  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places.
//
// Revision 1.1  1999/12/01 17:16:10  scott
// moved from src/id3 to include/id3
//
// Revision 1.10  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.9  1999/11/29 19:09:31  scott
// Updated documentation to work better with doc++ (probably doesn't work
// well with kdoc anymore).
//
// Revision 1.8  1999/11/25 19:22:18  scott
// (): Added doc++/kdoc/javadoc-like documentation, mostly transcribed
// from Dirk's ID3Lib Documentation MSWord doc.
// (ID3_TagTypes): Added.  The different types of tags that can be parsed
// and/or rendered by id3lib.
// (class ID3_Tag): Changed the interface to AddFrame slightly.  Now there
// is two AddTag-like methods: AddTag and AddNewTag.  The former merely
// attaches the frame to the tag without taking repsonsibility for
// deallocating the frame when the tag goes out of scope or is deleted.
// The latter does take this responsibility.  Granted, there could be a
// better interface for this, but it works for now...  Update and Strip
// now return flags which indicate what tags were actually updated or
// stripped.  Removed OpenLinkedFile method; replaced with
// OpenFileForReading, OpenFileForWriting, and CreateFile.
//
// Revision 1.7  1999/11/19 19:07:13  scott
// Added new constants: STR_V1_COMMENT_DESC (to be added to
// the description of comments converted from id3v1 tags); V1_TAG,
// V2_TAG, and BOTH_TAGS (used for methods Strip and Update to
// determine which tag to act on); and LEN_V1, LEN_V1_ID,
// LEN_V1_TITLE, LEN_V1_ARTIST, LEN_V1_ALBUM, LEN_V1_YEAR,
// LEN_V1_COMMENT, LEN_V1_GENRE (the lengths of the id3v1 tag and its
// fields). Generalized ID3V1_Tag struct using newly defined
// constants.  Added 1 to each char array size for null terminator.
// Added const qualifier to appropriate methods and parameters.  Added
// declaration of RenderV1ToHandle method.  Renamed "RenderToHandle" to
// "RenderV2ToHandle".
//
// Revision 1.6  1999/11/15 20:20:30  scott
// Made variable names more descriptive.  Added const qualifier to
// appropriate methods.
//
// Revision 1.5  1999/11/04 04:32:11  scott
// Initial revision
//
// Revision 1.4  1999/11/04 04:15:55  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//

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

#ifndef ID3LIB_FIELD_H
#define ID3LIB_FIELD_H

#include <stdlib.h>
#include "types.h"
#include "error.h"

// field flags
enum ID3_FieldFlags 
{
  ID3FF_NONE       =      0,
  ID3FF_NULL       = 1 << 0,
  ID3FF_NULLDIVIDE = 1 << 1,
  ID3FF_ADJUSTENC  = 1 << 2,
  ID3FF_ADJUSTEDBY = 1 << 3
};

// Enumeration of the types of field types
enum ID3_FieldType
{
  ID3FTY_INTEGER        = 0,
  ID3FTY_BITFIELD,
  ID3FTY_BINARY,
  ID3FTY_ASCIISTRING,
  ID3FTY_UNICODESTRING
};

// Used for version control
enum ID3_VerCtl
{
  ID3VC_HIGHER  = 0,
  ID3VC_LOWER
};

struct ID3_FieldDef
{
  ID3_FieldID   eID;
  ID3_FieldType eType;
  lsint         lFixedLength;
  uchar         ucVersion;
  uchar         ucRevision;
  ID3_VerCtl    eControl;
  luint         ulFlags;
  ID3_FieldID   eLinkedField;
  static const ID3_FieldDef* DEFAULT;
};

class ID3_Frame;
class ID3_Tag;

// Structure used for defining how frames are defined internally.
struct ID3_FrameDef
{
  ID3_FrameID   eID;
  char          sShortTextID[3 + 1];
  char          sLongTextID[4 + 1];
  lsint         lPriority;    // currently unused
  bool          bTagDiscard;
  bool          bFileDiscard;
  bool        (*parseHandler) (ID3_Frame *frame);
  ID3_FieldDef *aeFieldDefs;
};

/** The representative class of an id3v2 field.
 ** 
 ** As a general rule, you need never create an object of this type.  id3lib
 ** uses them internally as part of the id3_frame class.  You must know how to
 ** interact with these objects, though, and that's what this section is about.
 ** 
 ** The ID3_Field contains many overloaded methods to provide these facilities
 ** for four different data types: integers, ASCII strings, Unicode strings,
 ** and binary data.
 ** 
 ** An integer field supports the Get(), Set(luint), and operator=(luint)
 ** methods.
 ** 
 ** Both types of strings support the GetNumTextItems() method.
 ** 
 ** An ASCII string field supports the Get(char*, const luint, const luint)), 
 ** Set(const char*), Add(const char*), and operator=(const char*) methods.
 ** 
 ** A Unicode field also supports Get(unicode_t*, const luint, const luint),
 ** Set(const unicode_t*), Add(const unicode_t*), and 
 ** operator=(const unicode_t*).  Without elaborating, the Unicode
 ** methods behave exactly the same as their ASCII counterparts, taking
 ** \c unicode_t pointers in place of \c char pointers.
 ** 
 ** All strings in id3lib are handled internally as Unicode.  This means that
 ** when you set a field with an ASCII source type, it will be converted and
 ** stored internally as a Unicode string.  id3lib will handle all necessary
 ** conversions when parsing, rendering, and retrieving.  If you set a field as
 ** an ASCII string, then try to read the string into a \c unicode_t buffer,
 ** id3lib will automatically convert the string into Unicode so this will
 ** function as expected.  The same holds true in reverse.  Of course, when
 ** converting from Unicode to ASCII, you will experience problems when the
 ** Unicode string contains characters that don't map to ISO-8859-1.
 ** 
 ** A binary field supports the Get(uchar *, const luint), Set(const uchar*, 
 ** const luint), FromFile(const char*), and ToFile(const char*) methods.
 ** The binary field holds miscellaneous data that can't easily be described
 ** any other way, such as a JPEG image.
 ** 
 ** As a general implementation note, you should be prepared to support all
 ** fields in an id3lib frame, even if all fields in the id3lib version of the
 ** frame aren't present in the id3v2 version.  This is because of frames like
 ** the picture frame, which changed slightly from one version of the id3v2
 ** standard to the next (the IMAGEFORMAT format in 2.0 changed to a MIMETYPE
 ** in 3.0).  If you support all id3lib fields in a given frame, id3lib can
 ** generate the correct id3v2 frame for the id3v2 version you wish to support.
 ** Alternatively, just support the fields you know will be used in, say, 3.0
 ** if you only plan to generate 3.0 tags.
 ** 
 ** @author Dirk Mahoney
 ** @version $Id$
 ** \sa ID3_Tag
 ** \sa ID3_Frame
 ** \sa ID3_Err 
 **/
class ID3_Field
{
  friend ID3_Frame;
  friend ID3_Tag;
public:
  ~ID3_Field(void);
  
  /** Clears any data and frees any memory associated with the field
   ** 
   ** \sa ID3_Tag::Clear()
   ** \sa ID3_Frame::Clear()
   **/
  void Clear();

  /** Returns the size of a field.
   ** 
   ** The value returned is dependent on the type of the field.  For ASCII
   ** strings, this returns the number of characters in the field, no including
   ** any NULL-terminator.  The same holds true for Unicode---it returns the
   ** number of characters in the field, not bytes, and this does not include
   ** the Unicode BOM, which isn't put in a Unicode string obtained by the
   ** Get(unicode_t*, const luint, const luint) method anyway.  For binary and
   ** integer fields, this returns the number of bytes in the field.
   ** 
   ** \code
   **   luint howBig = myFrame.Field(ID3FN_DATA).Size();
   ** \endcode
   ** 
   ** \return The size of the field, either in bytes (for binary or integer
   **         fields) or characters (for strings).
   **/
  luint Size();

  /** Returns the number of items in a text list.
   ** 
   ** \code
   **   luint numItems = myFrame.Field(ID3FN_TEXT).GetNumItems();
   ** \endcode
   ** 
   ** \return The number of items in a text list.
   **/
  luint GetNumTextItems();

  // integer field functions
  /** A shortcut for the Set method.
   **
   ** \code
   **   myFrame.Field(ID3FN_PICTURETYPE) = 0x0B;
   ** \endcode
   ** 
   ** \param data The data to assign to this field
   ** \sa Set
   **/
  ID3_Field& operator= (const luint);

  /** Sets the value of the field to the specified integer.
   ** 
   ** \param data The data to assign to this field
   **/
  void Set(const luint);

  /** Returns the value of the integer field.
   ** 
   ** \code
   **   luint picType = myFrame.Field(ID3FN_PICTURETYPE).Get();
   ** \endcode
   **
   ** \return The value of the integer field
   **/
  luint Get() const;

  // ASCII string field functions
  /** Shortcut for the Set operator.
   ** 
   ** \param data The string to assign to this field
   ** \sa Set(const luint)
   **/
  ID3_Field& operator= (const char*);

  /** Copies the supplied string to the field.
   ** 
   ** You may dispose of the source string after a call to this method.
   ** 
   ** \code
   **   myFrame.Field(ID3FN_TEXT).Set("ID3Lib is very cool!");
   ** \endcode
   **/
  void          Set(const char*);

  /** Copies the contents of the field into the supplied buffer, up to the
   ** number of characters specified; for fields with multiple entries, the
   ** optional third parameter indicates which of the fields to retrieve.
   ** 
   ** The third parameter is useful when using text lists (see ID3_Field::Add
   ** for more details).  The default value for this third parameter is 1,
   ** which returns the entire string if the field contains only one item.
   ** 
   ** It returns the number of characters (not bytes necessarily, and not
   ** including any NULL terminator) of the supplied buffer that are now used.
   ** 
   ** \code
   **   char myBuffer[1024];
   **   luint charsUsed = myFrame.Field(ID3FN_TEXT).Get(buffer, 1024);
   ** \endcode
   ** 
   ** It fills the buffer with as much data from the field as is present in the
   ** field, or as large as the buffer, whichever is smaller.
   ** 
   ** \code
   **   char myBuffer[1024];
   **   luint charsUsed = myFrame.Field(ID3FN_TEXT).Get(buffer, 1024, 3);
   ** \endcode
   ** 
   ** This fills the buffer with up to the first 1024 characters from the third
   ** element of the text list.
   ** 
   ** \sa ID3_Field::Add
   **/
  luint       Get(char *buffer,   ///< Where to copy the data
                  const luint,    ///< Maximum number of characters to copy
                  const luint = 1 ///< The item number to retrieve
                  );

  /** For fields which support this feature, adds a string to the list of
   ** strings currently in the field.
   ** 
   ** This is useful for using id3v2 frames such as the involved people list,
   ** composer, and part of set.  You can use the GetNumItems() method to find
   ** out how many such items are in a list.
   ** 
   ** \code
   **   myFrame.Field(ID3FN_TEXT).Add("this is a test");
   ** \endcode
   ** 
   ** \param string The string to add to the field
   **/
  void          Add(const char*);

  // Unicode string field functions
  /** Shortcut for the Set operator.
   ** 
   ** Peforms similarly as the ASCII assignment operator, taking a unicode_t
   ** string as a parameter rather than an ascii string.
   ** 
   ** \sa Add(const char*)
   ** \param string The string to assign to the field
   **/
  ID3_Field    &operator= (const unicode_t*);

  /** Copies the supplied unicode string to the field.
   ** 
   ** Peforms similarly as the ASCII <a href="#Set">Set</a> method, taking a
   ** unicode_t string as a parameter rather than an ascii string.
   ** 
   ** \param string The unicode string to set this field to.
   ** \sa #Add
   **/
  void          Set(const unicode_t *string);

  /** Copies the contents of the field into the supplied buffer, up to the
   ** number of characters specified; for fields with multiple entries, the
   ** optional third parameter indicates which of the fields to retrieve.
   ** 
   ** Peforms similarly as the ASCII <a href="#Get">Get</a> method, taking a
   ** unicode_t string as a parameter rather than an ascii string.  The
   ** maxChars parameter still represents the maximum number of characters, not
   ** bytes.
   **   
   ** \code
   **   unicode_t myBuffer[1024];
   **   luint charsUsed = myFrame.Field(ID3FN_TEXT).Get(buffer, 1024);
   ** \endcode 
   **   
   ** \param buffer   Where the field's data is copied to
   ** \param maxChars The maximum number of characters to copy to the buffer.
   ** \param itemNum  For fields with multiple items (such as the involved
   **                 people frame, the item number to retrieve.
   ** \sa #Get
   **/
  luint         Get(unicode_t *buffer, const luint maxChars, const luint itemNum = 1);
  /** For fields which support this feature, adds a string to the list of
   ** strings currently in the field.
   ** 
   ** Peforms similarly as the ASCII <a href="#Add">Add</a> method, taking a
   ** unicode_t string as a parameter rather than an ascii string.
   **/
  void          Add(const unicode_t *string);
  // binary field functions
  /** Copies the supplied unicode string to the field.
   ** 
   ** Again, like the string types, the binary <a href="#Set">Set</a> function
   ** copies the data so you may dispose of the source data after a call to
   ** this method.
   ** 
   ** \param newData The data to assign to this field.
   ** \param newSize The number of bytes to be copied from the data array.
   **/
  void          Set(const uchar *newData, const luint newSize);
  /** Copies the field's internal string to the buffer.
   ** 
   ** It copies the data in the field into the buffer, for as many bytes as the
   ** field contains, or the size of buffer, whichever is smaller.
   ** 
   ** \code
   **   uchar buffer[1024];
   **   myFrame.Field(ID3FN_DATA).Get(buffer, sizeof(buffer));
   ** \endcode
   ** 
   ** \param buffer Where to copy the contents of the field.
   ** \param length The number of bytes in the buffer
   **/
  void          Get(uchar *buffer, const luint length);
  /** Copies binary data from the file specified to the field.
   ** 
   ** \code
   **   myFrame.Field(ID3FN_DATA).FromFile("mypic.jpg");
   ** \endcode
   ** 
   ** \param info The name of the file to read the data from.
   **/
  void          FromFile(const char *info);
  /** Copies binary data from the field to the specified file.
   ** 
   ** \code
   **   myFrame.Field(ID3FN_DATA).ToFile("output.bin");
   ** \endcode
   ** 
   ** \param info The name of the file to write the data to.
   **/
  void          ToFile(const char *sInfo);
  
  ID3_Field    &operator=( const ID3_Field & );

private:
  luint         BinSize(const bool withExtras = true);
  bool          HasChanged(void);
  void          SetVersion(const uchar ver, const uchar rev);
  luint         Render(uchar *buffer);
  luint         Parse(const uchar *buffer, const luint posn, const luint buffSize);

private:
  // To prevent public instantiation, the constructor is made private
  ID3_Field(void);

  ID3_FieldID   __eName;           // the ID of this field
  ID3_FieldType __eType;           // what type is this field or should be
  lsint         __lFixedLength;    // length of field (fixed if positive)
  uchar         __ucIOVersion;     // specific version
  uchar         __ucIORevision;    // specific revision
  ID3_VerCtl    __eControl;        // render if ver/rev is higher, or lower than frame::version, frame::revision?
  luint         __ulFlags;         // special field flags
  uchar         __ucVersion;       // the version being rendered/parsed
  uchar         __ucRevision;      // the revision being rendered/parsed
  bool          __bHasChanged;     // field changed since last parse/render?
  uchar        *__sData;
  luint         __ulSize;
protected:
  luint RenderInteger(uchar *buffer);
  luint RenderASCIIString(uchar *buffer);
  luint RenderUnicodeString(uchar *buffer);
  luint RenderBinary(uchar *buffer);
  
  size_t ParseInteger(const uchar *buffer, luint posn, size_t nSize);
  size_t ParseASCIIString(const uchar *buffer, luint posn, size_t nSize);
  size_t ParseUnicodeString(const uchar *buffer, luint posn, size_t nSize);
  size_t ParseBinary(const uchar *buffer, luint posn, size_t nSize);
  
};

ID3_FrameDef *ID3_FindFrameDef(const ID3_FrameID id);
ID3_FrameID   ID3_FindFrameID(const char *id);

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
// Revision 1.12  2000/04/10 20:17:35  eldamitri
// Updated comments for use with doxygen.
//
// Revision 1.11  2000/04/09 22:34:57  eldamitri
// (struct ID3_FieldDef): Added DEFAULT.
// (struct ID3_FrameDef): Made text id's fixed length char arrays rather
// than char ptrs.
//
// Revision 1.10  2000/04/08 04:32:36  eldamitri
// Changed new ANSI-standard C++ include headers to old-style headers.
//
// Revision 1.9  2000/04/07 04:45:31  eldamitri
// Minor cleanup for log comments.
//
// Revision 1.8  2000/04/05 05:20:52  eldamitri
// Updated initial comment information to reflect license, copyright
// change.
//
// Revision 1.7  2000/01/04 15:42:22  eldamitri
// For compilation with gcc 2.95.2 and better compatibility with ANSI/ISO
// standard C++, updated, rearranged, and removed (where necessary)
// #include directives.
//
// Revision 1.6  1999/12/26 16:40:04  scott
// (class ID3_Field): Cleaned up interface to Parse* methods.
//
// Revision 1.5  1999/12/17 16:05:02  scott
// Updated opening comment block.
//
// Revision 1.4  1999/12/15 06:37:39  scott
// Reformatted some comments.
//
// Revision 1.3  1999/12/09 02:45:41  scott
// (class ID3_Field): Added const to Get method.  Added operator= method
// declaration.
//
// Revision 1.2  1999/12/02 22:45:28  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places.
//
// Revision 1.1  1999/12/01 17:16:01  scott
// moved from src/id3 to include/id3
//
// Revision 1.10  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.9  1999/11/29 19:05:41  scott
// Updated documentation to work better with doc++ (probably doesn't work
// well with kdoc anymore).
//
// Revision 1.8  1999/11/25 19:11:40  scott
// Added doc++/kdoc/javadoc-like documentation, mostly transcribed
// from Dirk's ID3Lib Documentation MSWord doc.
// (ID3_FieldFlags ): Created enumeration to replace generic constants.
// (ID3_FieldID): Removed.  Replaced in globals.h.
// (ID3_FrameID): Removed.  Replaced in globals.h.
// (class ID3_Field): Made the constructor private.
//
// Revision 1.7  1999/11/19 17:19:58  scott
// Updated method interfaces to make appropriate parameters const.
//
// Revision 1.6  1999/11/15 20:15:36  scott
// Made ID3_Frame and ID3_Tag friends of the ID3_Field class to allow
// access to the private members in ID3_Field.  Made ID3FF field
// flags const luints, rather than #define's.  Made member variable
// names of classes and structs more descriptive.
//
// Revision 1.5  1999/11/04 04:32:11  scott
// Initial revision
//
// Revision 1.4  1999/11/04 04:15:54  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//

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

#include "types.h"
#include "field.h"
#include "header_frame.h"

class ID3_Tag;

/** The representative class of an id3v2 frame.
 ** 
 ** id3lib defines frames in a funny way.  Using some nice c++ conventions,
 ** ID3_Frame class objects appear to be quite polymorphic; that is, they can
 ** take on many forms.  The same ID3_Frame class provides the facilities for
 ** the implementation of a complex APIC frame and for a simple text frame.
 ** 
 ** @author Dirk Mahoney
 ** @version $Id$
 ** @see ID3_Tag
 ** @see ID3_Field
 ** @see ID3_Err
 **/
class ID3_Frame
{
  friend ID3_Tag;
public:
  /** Default constructor; accepts as a default parameter the type of frame
   ** to create.
   ** 
   ** The parameter which will internally set the frame's structure.  See
   ** <a href="#SetID">SetID</a> for more details.
   **     
   ** @param id The type of frame to create
   ** @see ID3_FrameID
   ** @see SetID
   **/
  ID3_Frame(ID3_FrameID id = ID3FID_NOFRAME);
  ID3_Frame(const ID3_FrameHeader &);

  /// Destructor.
  ~ID3_Frame(void);
  
  /** Clears the frame of alld ata and resets the frame such that it can take
   ** on the form of any id3v2 frame that id3lib supports.
   ** 
   ** @see ID3_Tag::Clear
   **/
  void        Clear(void);

  /** Establishes the internal structure of an ID3_Frame object so
   ** that it represents the id3v2 frame indicated by the parameter
   ** 
   ** Given an ID3_FrameID (a list of which is found in &lt;id3/field.h&gt;),
   ** <a href="#SetID">SetID</a> will structure the object according to the
   ** frame you wish to implement.
   ** 
   ** Either using this call or via the constructor, this must be the first
   ** command performed on an ID3_Frame object.  
   ** 
   ** \code
   **   myFrame.SetID(ID3FID_TITLE);
   ** \endcode
   ** 
   ** @param id The type of frame this frame should be set to
   ** @see ID3_FrameID
   **/
  void        SetID(ID3_FrameID id);

  /** Returns the type of frame that the object represents.
   ** 
   ** Useful in conjunction with ID3_Tag's Find method
   ** 
   ** @returns The type, or id, of the frame
   ** @see ID3_Tag::Find
   **/
  ID3_FrameID GetID(void) const;
  
  /** Returns a reference to the frame's internal field indicated by the 
   ** parameter.
   ** 
   ** A list of fields that are in given frames appears in
   ** &lt;id3/field.cpp&gt;.  This method returns a reference to the field in
   ** question so that the result can be used as though it were a field
   ** itself.
   **
   ** \code
   **   ID3_TextEnc enc;
   **   enc = (ID3_TextEnc) myFrame.Field(ID3FN_TEXTENC).Get();
   ** \endcode
   ** 
   ** @param name The name of the field to be retrieved
   ** @returns A reference to the desired field
   **/
  ID3_Field  &Field(ID3_FieldID name) const;
  
  ID3_Frame  &operator=( const ID3_Frame &rFrame );

protected:
  void        InitFields(const ID3_FrameDef *);
  void        InitFieldBits(void);
  bool        HasChanged(void) const;
  void        SetVersion(uchar ver, uchar rev);
  void        Parse(uchar *buffer, luint size);
  void        UpdateStringTypes(void);
  void        UpdateFieldDeps(void);
  luint       Size(void);
  luint       Render(uchar *buffer);
  lsint       FindField(ID3_FieldID name) const;

private:
  char        __sEncryptionID[256]; // encryption method used with this frame
  char        __sGroupingID[256];   // the group to which this frame belongs
  bool        __bHasChanged;        // frame changed since last parse/render?
  bitset      __auiFieldBits;       // which fields are present?
  luint       __ulNumFields;        // how many fields are in this frame?
  ID3_Field **__apFields;           // an array of field object pointers
  ID3_FrameHeader __FrmHdr;         // 
}
;

// $Log$
// Revision 1.1  2000/04/17 18:48:14  robert
// I've added the header files for the id3v2 lib and then I will also
// be checking in the .DLL for this. For unix we will simply depend on
// id3v2 lib being installed.
//
// Revision 1.9  2000/04/11 03:38:15  eldamitri
// Updated comments for use with doxygen.
//
// Revision 1.8  2000/04/10 20:17:35  eldamitri
// Updated comments for use with doxygen.
//
// Revision 1.7  2000/04/05 05:20:52  eldamitri
// Updated initial comment information to reflect license, copyright
// change.
//
// Revision 1.6  1999/12/27 06:14:22  scott
// (class ID3_Frame): Added declaration for constructor which accepts a
// frame header to copy.  Removed redundant data members that are now
// encapsulated within new frame header member.  Added declaration for
// new methods InitFields and InitFieldBits.
//
// Revision 1.5  1999/12/26 16:40:08  scott
// (class ID3_Frame): Added bReadOnly property.
//
// Revision 1.4  1999/12/17 16:05:02  scott
// Updated opening comment block.
//
// Revision 1.3  1999/12/09 02:45:46  scott
// (class ID3_Frame): Added operator= method declaration.
//
// Revision 1.2  1999/12/02 22:45:28  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places.
//
// Revision 1.1  1999/12/01 17:16:02  scott
// moved from src/id3 to include/id3
//
// Revision 1.9  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.8  1999/11/29 19:06:50  scott
// Updated documentation to work better with doc++ (probably doesn't work
// well with kdoc anymore).
//
// Revision 1.7  1999/11/25 19:15:15  scott
// Added doc++/kdoc/javadoc-like documentation, mostly transcribed
// from Dirk's ID3Lib Documentation MSWord doc.
//
// Revision 1.6  1999/11/15 20:18:11  scott
// Made ID3_Tag a friend of ID3_Frame to allow access to the private
// members in ID3_Frame.  Made variable names more descriptive.
// Added const qualifier to appropriate methods.
//
// Revision 1.5  1999/11/04 04:32:11  scott
// Initial revision
//
// Revision 1.4  1999/11/04 04:15:54  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//

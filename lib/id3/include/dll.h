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

#ifndef ID3LIB_DLLHEADERS_H
#define ID3LIB_DLLHEADERS_H

#include "globals.h"

class ID3_Field;
class ID3_Frame;
class ID3_Tag;

struct ID3_VerInfo
{
  char name[30];
  luint version;
  luint revision;
};

extern "C"
{

void        ID3_GetVersion(ID3_VerInfo *info);
// tag wrappers
ID3_Tag    *ID3Tag_New(void);
void        ID3Tag_Delete(ID3_Tag *tag);
void        ID3Tag_Clear(ID3_Tag *tag);
bool        ID3Tag_HasChanged(ID3_Tag *tag);
void        ID3Tag_SetUnsync(ID3_Tag *tag, bool unsync);
void        ID3Tag_SetExtendedHeader(ID3_Tag *tag, bool ext);
void        ID3Tag_SetCompression(ID3_Tag *tag, bool comp);
void        ID3Tag_SetPadding(ID3_Tag *tag, bool pad);
void        ID3Tag_AddFrame(ID3_Tag *tag, ID3_Frame *frame);
void        ID3Tag_AddFrames(ID3_Tag *tag, ID3_Frame *frames, luint num);
void        ID3Tag_RemoveFrame(ID3_Tag *tag, ID3_Frame *frame);
void        ID3Tag_Parse(ID3_Tag *tag, uchar header[ID3_TAGHEADERSIZE], uchar *buffer);
luint       ID3Tag_Link(ID3_Tag *tag, char *fileName);
void        ID3Tag_Update(ID3_Tag *tag);
void        ID3Tag_Strip(ID3_Tag *tag, bool v1Also);
ID3_Frame  *ID3Tag_FindFrameWithID(ID3_Tag *tag, ID3_FrameID id);
ID3_Frame  *ID3Tag_FindFrameWithINT(ID3_Tag *tag, ID3_FrameID id, ID3_FieldID fld, luint data);
ID3_Frame  *ID3Tag_FindFrameWithASCII(ID3_Tag *tag, ID3_FrameID id, ID3_FieldID fld, char *data);
ID3_Frame  *ID3Tag_FindFrameWithUNICODE(ID3_Tag *tag, ID3_FrameID id, ID3_FieldID fld, unicode_t *data);
ID3_Frame  *ID3Tag_GetFrameNum(ID3_Tag *tag, luint num);
luint       ID3Tag_NumFrames(ID3_Tag *tag);
// frame wrappers
void        ID3Frame_Clear(ID3_Frame *frame);
void        ID3Frame_SetID(ID3_Frame *frame, ID3_FrameID id);
ID3_FrameID ID3Frame_GetID(ID3_Frame *frame);
ID3_Field  *ID3Frame_GetField(ID3_Frame *frame, ID3_FieldID name);
// field wrappers
void        ID3Field_Clear(ID3_Field *field);
luint       ID3Field_Size(ID3_Field *field);
luint       ID3Field_GetNumTextItems(ID3_Field *field);
void        ID3Field_SetINT(ID3_Field *field, luint data);
luint       ID3Field_GetINT(ID3_Field *field);
void        ID3Field_SetUNICODE(ID3_Field *field, unicode_t *string);
luint       ID3Field_GetUNICODE(ID3_Field *field, unicode_t *buffer, luint maxChars, luint itemNum);
void        ID3Field_AddUNICODE(ID3_Field *field, unicode_t *string);
void        ID3Field_SetASCII(ID3_Field *field, char *string);
luint       ID3Field_GetASCII(ID3_Field *field, char *buffer, luint maxChars, luint itemNum);
void        ID3Field_AddASCII(ID3_Field *field, char *string);
void        ID3Field_SetBINARY(ID3_Field *field, uchar *data, luint size);
void        ID3Field_GetBINARY(ID3_Field *field, uchar *buffer, luint buffLength);
void        ID3Field_FromFile(ID3_Field *field, char *fileName);
void        ID3Field_ToFile(ID3_Field *field, char *fileName);

}

#endif

// $Log$
// Revision 1.1  2000/04/17 18:48:14  robert
// I've added the header files for the id3v2 lib and then I will also
// be checking in the .DLL for this. For unix we will simply depend on
// id3v2 lib being installed.
//
// Revision 1.5  2000/04/07 19:07:37  eldamitri
// Added extern "C"
//
// Revision 1.4  2000/04/05 05:20:52  eldamitri
// Updated initial comment information to reflect license, copyright
// change.
//
// Revision 1.3  1999/12/17 16:05:02  scott
// Updated opening comment block.
//
// Revision 1.2  1999/12/02 22:45:28  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places.
//
// Revision 1.1  1999/12/01 17:14:06  scott
// moved from src/id3 to include/id3
//
// Revision 1.7  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.6  1999/11/29 15:11:49  scott
// Updated unicode function declarations to now pass unicode_t instead of
// wchar_t.  unicode_t now defined in globals.h.
//
// Revision 1.5  1999/11/25 18:15:06  scott
// Removed many of the enums and typedefs to globals.h to reduce redundancy.
//
// Revision 1.4  1999/11/04 04:15:54  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//

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

#ifndef ID3LIB_MISC_SUPPORT_H
#define ID3LIB_MISC_SUPPORT_H

#include "types.h"
#include "tag.h"

#define MASK(bits) ((1 << (bits)) - 1)
#define MASK1 MASK(1)
#define MASK2 MASK(2)
#define MASK3 MASK(3)
#define MASK4 MASK(4)
#define MASK5 MASK(5)
#define MASK6 MASK(6)
#define MASK7 MASK(7)
#define MASK8 MASK(8)

uint32 ParseNumber(const uchar *buffer, size_t size = sizeof(uint32));
void   RenderNumber(uchar *buffer, uint32 val, size_t size = sizeof(uint32));

// defined in 'id3_misc_support.cpp'
// these are 'convenience functions,' to make using the library easier for the 
// most common of operations
char  *ID3_GetArtist(ID3_Tag *tag);
bool   ID3_AddArtist(ID3_Tag *tag, const char *text, bool bReplace = false);
size_t ID3_RemoveArtists(ID3_Tag *tag);

char  *ID3_GetAlbum(ID3_Tag *tag);
bool   ID3_AddAlbum(ID3_Tag *tag, const char *text, bool bReplace = false);
size_t ID3_RemoveAlbums(ID3_Tag *tag);

char  *ID3_GetTitle(ID3_Tag *tag);
bool   ID3_AddTitle(ID3_Tag *tag, const char *text, bool bReplace = false);
size_t ID3_RemoveTitles(ID3_Tag *tag);

char  *ID3_GetYear(ID3_Tag *tag);
bool   ID3_AddYear(ID3_Tag *tag, const char *text, bool bReplace = false);
size_t ID3_RemoveYears(ID3_Tag *tag);

char  *ID3_GetComment(ID3_Tag *tag);
bool   ID3_AddComment(ID3_Tag *tag, const char *text,
                      const char *sDescription = "", bool bReplace = false);
size_t ID3_RemoveComments(ID3_Tag *tag, const char *sDescription = NULL);

char  *ID3_GetTrack(ID3_Tag *tag);
luint  ID3_GetTrackNum(ID3_Tag *tag);
bool   ID3_AddTrack(ID3_Tag *tag, uchar ucTrack, uchar ucTotal = 0,
                    bool bReplace = false);
bool   ID3_AddTrack(ID3_Tag *tag, uchar ucTrack, bool bReplace);
size_t ID3_RemoveTracks(ID3_Tag *tag);

char  *ID3_GetGenre(ID3_Tag *tag);
luint  ID3_GetGenreNum(ID3_Tag *tag);
bool   ID3_AddGenre(ID3_Tag *tag, luint ucGenre, bool bReplace = false);
size_t ID3_RemoveGenres(ID3_Tag *tag);

char  *ID3_GetLyrics(ID3_Tag *tag);
bool   ID3_AddLyrics(ID3_Tag *tag, const char *text, bool bReplace = false);
size_t ID3_RemoveLyrics(ID3_Tag *tag);

void   mbstoucs(unicode_t *unicode, const char *ascii, const luint len);
void   ucstombs(char *ascii, const unicode_t *unicode, const luint len);
size_t ucslen(const unicode_t *unicode);
void   ucscpy(unicode_t *dest, const unicode_t *src);
void   ucsncpy(unicode_t *dest, const unicode_t *src, size_t len);
int    ucscmp(const unicode_t *s1, const unicode_t *s2);
int    ucsncmp(const unicode_t *s1, const unicode_t *s2, size_t len);

char *ID3_GetString(const ID3_Frame *frame, const ID3_FieldID fldName,
                    const size_t nItems = 1);
// in 'id3_tag_parse_v1.cpp'
void ID3_RemoveTrailingSpaces(char *buffer, luint length);

#endif

// $Log$
// Revision 1.1  2000/04/17 18:48:14  robert
// I've added the header files for the id3v2 lib and then I will also
// be checking in the .DLL for this. For unix we will simply depend on
// id3v2 lib being installed.
//
// Revision 1.10  2000/04/07 04:45:31  eldamitri
// Minor cleanup for log comments.
//
// Revision 1.9  2000/04/05 05:20:52  eldamitri
// Updated initial comment information to reflect license, copyright
// change.
//
// Revision 1.8  2000/01/04 15:42:22  eldamitri
// For compilation with gcc 2.95.2 and better compatibility with ANSI/ISO
// standard C++, updated, rearranged, and removed (where necessary)
// #include directives.
//
// Revision 1.7  1999/12/26 16:40:26  scott
// (MASK): Defined macro for masking bits.
// (): Added declarations for ParseNumber and RenderNumber, for converting
// between character arrays and unsigned ints.
//
// Revision 1.6  1999/12/17 16:05:02  scott
// Updated opening comment block.
//
// Revision 1.5  1999/12/09 02:45:52  scott
// (ID3_GetString): Added nItems parameter for retrieving a string from a
// list (for example, the involved people list).
//
// Revision 1.4  1999/12/05 05:34:41  scott
// Updated the Add functions so that they accept an option boolean
// parameter which indicates if the function should replace the current
// frame, if any.  Made any char * parameters const, if appropriate.
// Added a Remove function for each of the frame types handled in this
// file.  Also generalized AddComment so that it now accepts a Description
// string, which defaults to "".
//
// Revision 1.3  1999/12/02 22:45:28  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places.
//
// Revision 1.2  1999/12/01 22:10:02  scott
// Minor declaraction problem (thanks, elrod).
//
// Revision 1.1  1999/12/01 17:16:08  scott
// moved from src/id3 to include/id3
//
// Revision 1.8  1999/11/30 20:13:09  scott
// Removed include for wchar.h
//
// Revision 1.7  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.6  1999/11/29 19:17:33  scott
// Renamed ID3_ASCIItoUnicode to mbstoucs.  Renamed ID3_UnicodeToASCII to
// ucstombs.  Added declarations for ucslen, ucscpy, ucsncpy, ucscmp, and
// ucsncmp.
//
// Revision 1.5  1999/11/19 18:59:09  scott
// Added the function declarations for ID3_GetString,
// ID3_GetArtist, ID3_GetAlbum, ID3_GetTitle, ID3_GetYear, ID3_AddYear,
// ID3_GetComment, ID3_AddComment, ID3_GetTrack, ID3_GetTrackNum,
// ID3_AddTrack, ID3_GetGenre, ID3_GetGenreNum, and ID3_GetLyrics.
//
// Revision 1.4  1999/11/04 04:15:55  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//

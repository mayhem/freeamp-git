/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998 GoodNoise

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
	
	$Id$
____________________________________________________________________________*/

#ifndef _ID3V1_H_
#define _ID3V1_H_

#include <iostream.h>
#include <string.h>

class Id3TagInfo {
 public:
    bool m_containsInfo;

    char m_songName[31];
    char m_artist[31];
    char m_album[31];
    char m_year[5];
    char m_comment[31];
    char m_genre;

    Id3TagInfo() {
	    m_containsInfo = false;
	    nullinit();
    }

    Id3TagInfo(char *buffer) {
	    if (buffer && (!strncmp(buffer,"TAG",3))) {
	        memset(this,0,sizeof(Id3TagInfo));
	        m_containsInfo = true;
	        strncpy(m_songName, buffer + 3, 30);
	        strncpy(m_artist, buffer + 33, 30);
	        strncpy(m_album, buffer + 63, 30);
	        strncpy(m_year, buffer + 93, 4);
	        strncpy(m_comment, buffer + 97, 30);
	        m_genre = buffer[127];
	    } else {
	        m_containsInfo = false;
	        nullinit();
	    }
    }

 private:
    void nullinit() {
	    *m_songName = '\0';
	    *m_artist = '\0';
	    *m_album = '\0';
	    *m_year = '\0';
	    *m_comment = '\0';
	    m_genre = '\0';
    }
};


#endif /* _ID3V1_H_ */




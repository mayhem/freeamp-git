/*_______________________________________________________________________        
        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 2000 Relatable

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program; if not, Write to the Free Software
        Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
        
        $Id$
____________________________________________________________________________*/

#ifndef APSCONVERT_H_
#define APSCONVERT_H_

#include <iostream>
#include <string>
#include <vector>
#include "apsplaylist.h"

using namespace std;

typedef vector<pair<string, string> > StreamList;

struct EventRecord
{
    string strGUID;
    int    nEventType;
    long   lTimeStamp;
};

typedef vector<EventRecord> EventLog;

int BufSize(StreamList& TheVec);
int BufSize(EventLog& TheLog);
int BufSize(APSPlaylist& TheList);

void ToBuffer(StreamList& TheVec, char* pBuffer, int& nBuffUsed);
void ToBuffer(EventLog& TheLog, char* pBuffer, int& nBuffUsed);
void ToBuffer(APSPlaylist& TheList, char* pBuffer, int& nBuffUsed);

void FromBuffer(StreamList& TheVec, char* pBuffer, int& nBuffUsed);
void FromBuffer(EventLog& TheLog, char* pBuffer, int& nBuffUsed);
void FromBuffer(APSPlaylist& TheList, char* pBuffer, int& nBuffUsed);

#endif

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

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include "config.h"
#include "apsconvert.h"

int BufSize(StreamList& TheVec)
{
       int nBufSize = TheVec.size() * (sizeof(char)*2);
       nBufSize += sizeof(int);

       StreamList::iterator i;
       for (i = TheVec.begin(); i != TheVec.end(); i++)
       {
               nBufSize += (*i).first.size() + (*i).second.size();
       }
       return nBufSize;
}

int BufSize(EventLog& TheLog)
{
       int nBufSize = sizeof(int);
       EventLog::iterator i;

       for (i = TheLog.begin(); i != TheLog.end(); i++)
       {
               nBufSize += (*i).strGUID.size() + sizeof(char);
               nBufSize += sizeof(int) + sizeof(long);
       }
       return nBufSize;
}

int BufSize(APSPlaylist& TheList)
{
       return TheList.GetBufLen();
}

void ToBuffer(StreamList& TheVec, char* pBuffer, int& nBuffUsed)
{
       int nNumEntries = TheVec.size();
       int nOffset = 0;
       int nTemp = 0;
       memcpy(pBuffer, &nNumEntries, sizeof(int));
       StreamList::iterator i;
       nOffset = sizeof(int);

       for (i = TheVec.begin(); i != TheVec.end(); i++)
       {
               nTemp = (*i).first.size();
               memcpy(&pBuffer[nOffset], (*i).first.c_str(), nTemp);
               nOffset += nTemp;
               pBuffer[nOffset] = '\0';
               nOffset += sizeof(char);

               nTemp = (*i).second.size();
               memcpy(&pBuffer[nOffset], (*i).second.c_str(), nTemp);
               nOffset += nTemp;
               pBuffer[nOffset] = '\0';
               nOffset += sizeof(char);
       }
       nBuffUsed = nOffset;
}

void ToBuffer(EventLog& TheLog, char* pBuffer, int& nBuffUsed)
{
       int nNumEntries = TheLog.size();
       int nOffset = sizeof(int);
       memcpy(pBuffer, &nNumEntries, sizeof(int));
       EventLog::iterator i;
       int nTemp = 0;

       for (i = TheLog.begin(); i != TheLog.end(); i++)
       {
               nTemp = (*i).strGUID.size();
               memcpy(&pBuffer[nOffset], (*i).strGUID.c_str(), nTemp);
               nOffset += nTemp;
               pBuffer[nOffset] = '\0';
               nOffset += sizeof(char);
               memcpy(&pBuffer[nOffset], &((*i).nEventType), sizeof(int));
               nOffset += sizeof(int);
               memcpy(&pBuffer[nOffset], &((*i).lTimeStamp), sizeof(long));
               nOffset += sizeof(long);
       }
       
       nBuffUsed = nOffset;
}

void ToBuffer(APSPlaylist& TheList, char* pBuffer, int& nBuffUsed)
{
       TheList.ToBuffer(pBuffer, nBuffUsed);
}

void FromBuffer(StreamList& TheVec, char* pBuffer, int& nBuffUsed)
{
       int nOffset = 0;
       int nNumEntries = 0;
       memcpy(&nNumEntries, pBuffer, sizeof(int));
       int i = 0;
       string strOne, strTwo;
       nOffset = sizeof(int);
       for (i = 0; i < nNumEntries; i++)
       {
               strOne = &pBuffer[nOffset];
               nOffset += strOne.size() + sizeof(char);
               strTwo = &pBuffer[nOffset];
               nOffset += strTwo.size() + sizeof(char);
               TheVec.push_back(pair<string, string>(strOne, strTwo));
       }
       nBuffUsed = nOffset;
}

void FromBuffer(EventLog& TheLog, char* pBuffer, int& nBuffUsed)
{
       int nOffset = sizeof(int);
       int nNumEntries = 0;
       memcpy(&nNumEntries, pBuffer, sizeof(int));
       int i = 0;
       //int nTemp;
       EventRecord     TheRecord;
       for (i = 0; i < nNumEntries; i++)
       {
               TheRecord.strGUID = &pBuffer[nOffset];
               nOffset += TheRecord.strGUID.size() + sizeof(char);
               memcpy(&(TheRecord.nEventType), &pBuffer[nOffset], sizeof(int));
               nOffset += sizeof(int);
               memcpy(&(TheRecord.lTimeStamp), &pBuffer[nOffset], sizeof(long));
               nOffset += sizeof(long);
               TheLog.push_back(TheRecord);
       }
       nBuffUsed = nOffset;
}

void FromBuffer(APSPlaylist& TheList, char* pBuffer, int& nBuffUsed)
{
       TheList.FromBuffer(pBuffer, nBuffUsed);
}

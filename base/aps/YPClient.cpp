/*________________________________________________________________________        
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

#include "YPClient.h"

#ifdef WIN32
#include "wincomsocket.h"
#else
#include "comsocket.h"
#endif

#include "mutex.h"
#include "automutex.h"
#include <string.h>
// Any other includes go here

namespace YP
{
    static const int  nTimeout = 15;
    static const char GeneratePlaylist = 'A';
    static const char SubmitPlaylist   = 'B';
    static const char GetGUID          = 'C';
    static const char GetStreams       = 'D';
    static const char DeleteProfile    = 'E';
    static const char SyncLog          = 'F'; 
    static const char SoundsLike       = 'G';
    static const char StatusSuccess    = 'X';
    static const char StatusFailure    = 'Y';
    static const char Disconnect       = 'Z';
}

using namespace std;

YPClient::YPClient()
{
    m_pSocket = new COMSocket;
    m_pMutex = new Mutex;
}

YPClient::~YPClient()
{
    if (m_pSocket->IsConnected()) 
        this->Disconnect();
    if (m_pSocket != NULL) 
        delete m_pSocket;
    if (m_pMutex != NULL)
        delete m_pMutex;
}

int YPClient::Connect(string& strIP, int nPort)
{
    int nErr = m_pSocket->Connect(strIP.c_str(), nPort, SOCK_STREAM);
    if (nErr == -1)
    {
        return -1;
    }
    return 0;
}

int YPClient::Disconnect()
{
    if (m_pSocket->IsConnected())
    {
        char cBuffer[sizeof(char) + sizeof(int)];
        memset(cBuffer, 0x00, sizeof(char) + sizeof(int));
        cBuffer[0] = YP::Disconnect;
        int nBytes = 0;
        m_pSocket->Write(cBuffer, sizeof(char) + sizeof(int), &nBytes);
        m_pSocket->Disconnect();
    }
    return 0;
}

int YPClient::GeneratePlayList(APSPlaylist& ResultList, APSPlaylist& SeedList,
                               int nMaxItems, int bLocalOnly, string& strUID,
                               string& CollectionID)
{
    AutoMutex AM(m_pMutex);
    int nConRes = this->Connect(m_strIP, m_nPort);
    if (nConRes != 0) 
        return -1;

    int nBufLen, nOffset, nBytes, nTemp, nRes;
    nBufLen = sizeof(char) + sizeof(int);
    nOffset = nBufLen;
    nBufLen += BufSize(SeedList);
    nBufLen += sizeof(int);
    nBufLen += sizeof(int);
    nBufLen += strUID.size() + sizeof(char);
    nBufLen += CollectionID.size() + sizeof(char);

    char* pBuffer = new char[nBufLen];
    memset(pBuffer, 0x00, nBufLen);
    pBuffer[0] = YP::GeneratePlaylist;
    nBufLen -= nOffset;
    memcpy(&pBuffer[1], &nBufLen, sizeof(int));
    nBufLen += nOffset;

    ToBuffer(SeedList, &pBuffer[nOffset], nTemp);
    nOffset += nTemp;
    memcpy(&pBuffer[nOffset], &nMaxItems, sizeof(int));
    nOffset += sizeof(int);
    memcpy(&pBuffer[nOffset], &bLocalOnly, sizeof(int));
    nOffset += sizeof(int);
    nTemp = strUID.size();
    memcpy(&pBuffer[nOffset], strUID.c_str(), nTemp);
    nOffset += nTemp;
    pBuffer[nOffset] = '\0';
    nOffset += sizeof(char);
    nTemp = CollectionID.size();
    memcpy(&pBuffer[nOffset], CollectionID.c_str(), nTemp);
    nOffset += nTemp;
    pBuffer[nOffset] = '\0';
    nOffset += sizeof(char);

    nBytes = 0;
    nRes = m_pSocket->Write(pBuffer, nBufLen, &nBytes);
    nBufLen = 0;

    nRes = m_pSocket->NBRead(pBuffer, sizeof(char) + sizeof(int), 
                             &nBytes, YP::nTimeout);
    if ((nRes != -1) && (nBytes == (sizeof(char) + sizeof(int))) && 
        (pBuffer[0] == YP::StatusSuccess))
    {
        memcpy(&nBufLen, &pBuffer[sizeof(char)], sizeof(int));
        if (nBufLen != 0)
        {
            delete [] pBuffer;
            pBuffer = new char[nBufLen+1];
            pBuffer[nBufLen] = '\0';
            nOffset = 0;
            while ((nOffset < nBufLen) && (nRes != -1))
            {
                 nRes = m_pSocket->Read(&pBuffer[nOffset], nBufLen - nOffset, 
                                        &nBytes);
                 nOffset += nBytes;
            }
            nBytes = nOffset;
            nOffset = 0;
            
            FromBuffer(ResultList, &pBuffer[nOffset], nTemp);
            nOffset += nTemp;
            nRes = 0;
        }
    }
    else
    {
        nRes = -1;
    }
    if (pBuffer != NULL) 
        delete [] pBuffer;
    this->Disconnect();
    return nRes;
}

int YPClient::SubmitPlaylist(APSPlaylist& SeedList, string& strUID)
{
    AutoMutex AM(m_pMutex);
    int nConRes = this->Connect(m_strIP, m_nPort);
    if (nConRes != 0) 
        return -1;

    int nBufLen, nOffset, nBytes, nTemp, nRes;
    nBufLen = sizeof(char) + sizeof(int);
    nOffset = nBufLen;
    nBufLen += BufSize(SeedList);
    nBufLen += strUID.size() + sizeof(char);

    char* pBuffer = new char[nBufLen];
    memset(pBuffer, 0x00, nBufLen);
    pBuffer[0] = YP::SubmitPlaylist;
    nBufLen -= nOffset;
    memcpy(&pBuffer[1], &nBufLen, sizeof(int));
    nBufLen += nOffset;

    ToBuffer(SeedList, &pBuffer[nOffset], nTemp);
    nOffset += nTemp;
    nTemp = strUID.size();
    memcpy(&pBuffer[nOffset], strUID.c_str(), nTemp);
    nOffset += nTemp;
    pBuffer[nOffset] = '\0';
    nOffset += sizeof(char);

    nBytes = 0;
    nRes = m_pSocket->Write(pBuffer, nBufLen, &nBytes);

    delete [] pBuffer;
    pBuffer = NULL;

    char cRetVal = '\0';
    nRes = m_pSocket->NBRead(&cRetVal, sizeof(char), &nBytes, YP::nTimeout);
    if ((nRes != -1) && (cRetVal == YP::StatusSuccess)) 
        nRes = 0;
    else
        nRes = -1;
    this->Disconnect();
    return nRes;
}

int YPClient::GetGUID(string& strGUID, int nSeed)
{
    AutoMutex AM(m_pMutex);
    int nConRes = this->Connect(m_strIP, m_nPort);
    if (nConRes != 0) 
        return -1;

    int nBufLen, nOffset, nBytes, nRes;
    nBufLen = sizeof(char) + sizeof(int);
    nOffset = nBufLen;
    nBufLen += sizeof(int);

    char* pBuffer = new char[nBufLen];
    memset(pBuffer, 0x00, nBufLen);
    pBuffer[0] = YP::GetGUID;
    nBufLen -= nOffset;
    memcpy(&pBuffer[1], &nBufLen, sizeof(int));
    nBufLen += nOffset;

    memcpy(&pBuffer[nOffset], &nSeed, sizeof(int));
    nOffset += sizeof(int);

    nBytes = 0;
    nRes = m_pSocket->Write(pBuffer, nBufLen, &nBytes);
    nBufLen = 0;

    nRes = m_pSocket->NBRead(pBuffer, sizeof(char) + sizeof(int), &nBytes, 
                             YP::nTimeout);
    if ((nRes != -1) && (nBytes == (sizeof(char) + sizeof(int))) && 
        (pBuffer[0] == YP::StatusSuccess))
    {
        memcpy(&nBufLen, &pBuffer[sizeof(char)], sizeof(int));
        if (nBufLen != 0)
        {
            delete [] pBuffer;
            pBuffer = new char[nBufLen+1];
            pBuffer[nBufLen] = '\0';
            nOffset = 0;
            while ((nOffset < nBufLen) && (nRes != -1))
            {
                nRes = m_pSocket->Read(&pBuffer[nOffset], nBufLen - nOffset, 
                                       &nBytes);
                nOffset += nBytes;
            }
            nBytes = nOffset;
            nOffset = 0;

            strGUID = &pBuffer[nOffset];
            nOffset += strGUID.size() + sizeof(char);
            nRes = 0;
        }
    }
    else
    {
        nRes = -1;
    }

    if (pBuffer != NULL) 
        delete [] pBuffer;
    this->Disconnect();
    return nRes;
}

int YPClient::GetStreams(StreamList& ResultList, string& strUID, string&
strCollectionID)
{
    AutoMutex AM(m_pMutex);
    int nConRes = this->Connect(m_strIP, m_nPort);
    if (nConRes != 0) 
        return -1;

    int nBufLen, nOffset, nBytes, nTemp, nRes;
    nBufLen = sizeof(char) + sizeof(int);
    nOffset = nBufLen;
    nBufLen += strUID.size() + sizeof(char);
    nBufLen += strCollectionID.size() + sizeof(char);


    char* pBuffer = new char[nBufLen];
    memset(pBuffer, 0x00, nBufLen);
    pBuffer[0] = YP::GetStreams;
    nBufLen -= nOffset;
    memcpy(&pBuffer[1], &nBufLen, sizeof(int));
    nBufLen += nOffset;

    nTemp = strUID.size();
    memcpy(&pBuffer[nOffset], strUID.c_str(), nTemp);
    nOffset += nTemp;
    pBuffer[nOffset] = '\0';
    nOffset += sizeof(char);
    nTemp = strCollectionID.size();
    memcpy(&pBuffer[nOffset], strCollectionID.c_str(), nTemp);
    nOffset += nTemp;
    pBuffer[nOffset] = '\0';
    nOffset += sizeof(char);

    nBytes = 0;
    nRes = m_pSocket->Write(pBuffer, nBufLen, &nBytes);
    nBufLen = 0;

    nRes = m_pSocket->NBRead(pBuffer, sizeof(char) + sizeof(int), &nBytes, 
                             YP::nTimeout);
    if ((nRes != -1) && (nBytes == (sizeof(char) + sizeof(int))) && 
        (pBuffer[0] == YP::StatusSuccess))
    {
        memcpy(&nBufLen, &pBuffer[sizeof(char)], sizeof(int));
        if (nBufLen != 0)
        {
            delete [] pBuffer;
            pBuffer = new char[nBufLen+1];
            pBuffer[nBufLen] = '\0';
            nOffset = 0;
            while ((nOffset < nBufLen) && (nRes != -1))
            {
                nRes = m_pSocket->Read(&pBuffer[nOffset], nBufLen - nOffset, 
                                       &nBytes);
                nOffset += nBytes;
            }
            nBytes = nOffset;
            nOffset = 0;

            FromBuffer(ResultList, &pBuffer[nOffset], nTemp);
            nOffset += nTemp;
            nRes = 0;
        }
    }
    else
    {
        nRes = -1;
    }
    if (pBuffer != NULL) 
        delete [] pBuffer;
    this->Disconnect();
    return nRes;
}

int YPClient::DeleteProfile(string& strUID)
{
    AutoMutex AM(m_pMutex);
    int nConRes = this->Connect(m_strIP, m_nPort);
    if (nConRes != 0) 
        return -1;

    int nBufLen, nOffset, nBytes, nTemp, nRes;
    nBufLen = sizeof(char) + sizeof(int);
    nOffset = nBufLen;
    nBufLen += strUID.size() + sizeof(char);

    char* pBuffer = new char[nBufLen];
    memset(pBuffer, 0x00, nBufLen);
    pBuffer[0] = YP::DeleteProfile;
    nBufLen -= nOffset;
    memcpy(&pBuffer[1], &nBufLen, sizeof(int));
    nBufLen += nOffset;

    nTemp = strUID.size();
    memcpy(&pBuffer[nOffset], strUID.c_str(), nTemp);
    nOffset += nTemp;
    pBuffer[nOffset] = '\0';
    nOffset += sizeof(char);

    nBytes = 0;
    nRes = m_pSocket->Write(pBuffer, nBufLen, &nBytes);
    delete [] pBuffer;
    pBuffer = NULL;

    char cRetVal = '\0';
    nRes = m_pSocket->NBRead(&cRetVal, sizeof(char), &nBytes, YP::nTimeout);
    if ((nRes != -1) && (cRetVal == YP::StatusSuccess)) 
        nRes = 0;
    else
        nRes = -1;
    this->Disconnect();
    return nRes;
}

int YPClient::SyncLog(EventLog& TheLog, string& strUID)
{
    AutoMutex AM(m_pMutex);
    int nConRes = this->Connect(m_strIP, m_nPort);
    if (nConRes != 0) 
        return -1;

    int nBufLen, nOffset, nBytes, nTemp, nRes;
    nBufLen = sizeof(char) + sizeof(int);
    nOffset = nBufLen;
    nBufLen += BufSize(TheLog);
    nBufLen += strUID.size() + sizeof(char);

    char* pBuffer = new char[nBufLen];
    memset(pBuffer, 0x00, nBufLen);
    pBuffer[0] = YP::SyncLog;
    nBufLen -= nOffset;
    memcpy(&pBuffer[1], &nBufLen, sizeof(int));
    nBufLen += nOffset;

    ToBuffer(TheLog, &pBuffer[nOffset], nTemp);
    nOffset += nTemp;
    nTemp = strUID.size();
    memcpy(&pBuffer[nOffset], strUID.c_str(), nTemp);
    nOffset += nTemp;
    pBuffer[nOffset] = '\0';
    nOffset += sizeof(char);

    nBytes = 0;
    nRes = m_pSocket->Write(pBuffer, nBufLen, &nBytes);
    delete [] pBuffer;
    pBuffer = NULL;

    char cRetVal = '\0';
    nRes = m_pSocket->NBRead(&cRetVal, sizeof(char), &nBytes, YP::nTimeout);
    if ((nRes != -1) && (cRetVal == YP::StatusSuccess)) 
        nRes = 0;
    else
        nRes = -1;
    this->Disconnect();
    return nRes;
}

int YPClient::SoundsLike(APSPlaylist& ResultList, APSPlaylist& SeedList,
string& strCollectionID)
{
        AutoMutex AM(m_pMutex);
        int nConRes = this->Connect(m_strIP, m_nPort);
        if (nConRes != 0) return -1;

        int nBufLen, nOffset, nBytes, nTemp, nRes;
        nBufLen = sizeof(char) + sizeof(int);
        nOffset = nBufLen;
        nBufLen += BufSize(SeedList);
        nBufLen += strCollectionID.size() + sizeof(char);

        char* pBuffer = new char[nBufLen];
        memset(pBuffer, 0x00, nBufLen);
        pBuffer[0] = YP::SoundsLike;
        nBufLen -= nOffset;
        memcpy(&pBuffer[1], &nBufLen, sizeof(int));
        nBufLen += nOffset;

        ToBuffer(SeedList, &pBuffer[nOffset], nTemp);
        nOffset += nTemp;
        nTemp = strCollectionID.size();
        memcpy(&pBuffer[nOffset], strCollectionID.c_str(), nTemp);
        nOffset += nTemp;
        pBuffer[nOffset] = '\0';
        nOffset += sizeof(char);

        nBytes = 0;
        nRes = m_pSocket->Write(pBuffer, nBufLen, &nBytes);
        nBufLen = 0;

        nRes = m_pSocket->NBRead(pBuffer, sizeof(char) + sizeof(int), &nBytes, YP::nTimeout);
        if ((nRes != -1) && (nBytes == (sizeof(char) + sizeof(int))) && (pBuffer[0] == YP::StatusSuccess))
        {
                memcpy(&nBufLen, &pBuffer[sizeof(char)], sizeof(int));
                if (nBufLen != 0)
                {
                        delete [] pBuffer;
                        pBuffer = new char[nBufLen+1];
                        pBuffer[nBufLen] = '\0';
                        nOffset = 0;
                        while ((nOffset < nBufLen) && (nRes != -1))
                        {
                                nRes = m_pSocket->Read(&pBuffer[nOffset], nBufLen - nOffset, &nBytes);
                                nOffset += nBytes;
                        }
                        nBytes = nOffset;
						nOffset = 0;
        FromBuffer(ResultList, &pBuffer[nOffset], nTemp);
        nOffset += nTemp;
                        nRes = 0;
                }
        }
        else
        {
                nRes = -1;
        }
        if (pBuffer != NULL) delete [] pBuffer;
        this->Disconnect();
        return nRes;
}


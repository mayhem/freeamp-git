/***************************************************************************
                          slclient.cpp  -  description
                             -------------------
    begin                : Wed Sep 6 2000
    copyright            : (C) 2000 by Relatable
    programmed by        : Isaac Richards
    email                : ijr@relatable.com
 ***************************************************************************/

#include "slclient.h"
#include "comsocket.h"
#include "mutex.h"
#include "automutex.h"
#include <string.h>
// Any other includes go here

namespace SLSet
{
	static const int nTimeout = 15;
	static const char SoundsLike    = 'S';
        static const char SoundsLikeD   = 'T';
	static const char StatusSuccess = 'X';
	static const char StatusFailure = 'Y';
	static const char Disconnect    = 'E';
}

using namespace std;

SoundsLikeClient::SoundsLikeClient()
{
	m_pSocket = new COMSocket;
}

SoundsLikeClient::~SoundsLikeClient()
{
	if (m_pSocket->IsConnected()) this->Disconnect();
	if (m_pSocket != NULL) delete m_pSocket;
}

int SoundsLikeClient::Connect(string& strIP, int nPort)
{
	int nErr = m_pSocket->Connect(strIP.c_str(), nPort, SOCK_STREAM);
	if (nErr == -1)
	{
		return -1;
	}
	return 0;
}

int SoundsLikeClient::Disconnect()
{
	if (m_pSocket->IsConnected())
	{
		char cBuffer[sizeof(char) + sizeof(int)];
		memset(cBuffer, 0x00, sizeof(char) + sizeof(int));
		cBuffer[0] = SLSet::Disconnect;
		int nBytes = 0;
		int nRes = m_pSocket->Write(cBuffer, sizeof(char) + sizeof(int), &nBytes);
		m_pSocket->Disconnect();
	}
	return 0;
}

vector<pair<string, float> > *
SoundsLikeClient::SoundsLike(vector<string> *pSeedGUIDs, 
                             vector<string> *pCollGUIDs, int nNumClosest,
                             float fMaxDistance)
{
    vector<pair<string, float> > *retList = new vector<pair<string, float> >;

    int nConRes = this->Connect(m_strIP, m_nPort);
    if (nConRes != 0) 
        return retList;

    int nBufLen, nOffset, nBytes, nTemp, nRes;
    vector<string>::iterator i;
    nBufLen = sizeof(char) + sizeof(int);
    nOffset = nBufLen;
    nBufLen += sizeof(int) * 3;

    if (fMaxDistance > 0)
        nBufLen += sizeof(float);

    for (i = pSeedGUIDs->begin(); i != pSeedGUIDs->end(); i++)
        nBufLen += (*i).size() + sizeof(char);
    for (i = pCollGUIDs->begin(); i != pCollGUIDs->end(); i++)
        nBufLen += (*i).size() + sizeof(char);

    char* pBuffer = new char[nBufLen];
    memset(pBuffer, 0x00, nBufLen);
    if (fMaxDistance > 0)
        pBuffer[0] = SLSet::SoundsLikeD;
    else
        pBuffer[0] = SLSet::SoundsLike;

    nBufLen -= nOffset;
    memcpy(&pBuffer[1], &nBufLen, sizeof(int));
    nBufLen += nOffset;

    memcpy(&pBuffer[nOffset], &nNumClosest, sizeof(int));
    nOffset += sizeof(int);
    nTemp = pSeedGUIDs->size();
    memcpy(&pBuffer[nOffset], &nTemp, sizeof(int));
    nOffset += sizeof(int);
    nTemp = pCollGUIDs->size();
    memcpy(&pBuffer[nOffset], &nTemp, sizeof(int));
    nOffset += sizeof(int);

    if (fMaxDistance > 0) {
        memcpy(&pBuffer[nOffset], &fMaxDistance, sizeof(float));
        nOffset += sizeof(float);
    }

    for (i = pSeedGUIDs->begin(); i != pSeedGUIDs->end(); i++)
    {
        nTemp = (*i).size();
        memcpy(&pBuffer[nOffset], (*i).c_str(), nTemp);
        nOffset += nTemp;
        pBuffer[nOffset] = '\0';
        nOffset += sizeof(char);
    }
    for (i = pCollGUIDs->begin(); i != pCollGUIDs->end(); i++)
    {
        nTemp = (*i).size();
        memcpy(&pBuffer[nOffset], (*i).c_str(), nTemp);
        nOffset += nTemp;
        pBuffer[nOffset] = '\0';
        nOffset += sizeof(char);
    }

    nBytes = 0;
    nRes = m_pSocket->Write(pBuffer, nBufLen, &nBytes);
    nBufLen = 0;

    nRes = m_pSocket->NBRead(pBuffer, sizeof(char) + sizeof(int), 
                             &nBytes, SLSet::nTimeout);
    if ((nRes != -1) && (pBuffer[0] == SLSet::StatusSuccess) &&
        (nBytes == (sizeof(char) + sizeof(int))))
    {
        memcpy(&nBufLen, &pBuffer[sizeof(char)], sizeof(int));
        if (nBufLen != 0)
        {
            delete [] pBuffer;
            pBuffer = new char[nBufLen + 1];
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

            int numRet;
            memcpy(&numRet, &pBuffer[nOffset], sizeof(int));
            nOffset += sizeof(int);

            int j;
            string strGUID;
            float distance;
            for (j = 0; j < numRet; j++)
            {
                strGUID = &pBuffer[nOffset];
                nOffset += strGUID.size() + sizeof(char);
                memcpy(&distance, &pBuffer[nOffset], sizeof(float));
                nOffset += sizeof(float);

                retList->push_back(vector<pair<string, float> >::
                                    value_type(strGUID, distance));
            }
        }
    }

    this->Disconnect();
    return retList;
}

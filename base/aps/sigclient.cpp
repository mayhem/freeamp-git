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


/***************************************************************************
ypclient.cpp  -  description
-------------------
begin                : Sat Jun 3 2000
copyright            : (C) 2000 by Relatable 
written by           : Sean Ward
email                : sward@relatable.com
***************************************************************************/

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include "sigclient.h"
#ifdef WIN32
#include "wincomsocket.h"
#else
#include "comsocket.h"
#endif

#include "mutex.h"
#include "apsplaylist.h"
#include "automutex.h"
#include "sigxdr.h"

namespace SigClientVars
{
    static const char cGetGUID = 'G';
    static const char cDisconnect = 'E';

    static const int nGUIDSize = 16;
    static const int nTimeout = 15;
    static const int nHeaderSize = sizeof(char) + sizeof(int);
}

using namespace std;

SigClient::SigClient()
{
    m_pSocket = new COMSocket;
    m_pMutex = new Mutex;
    m_nNumFailures = 0;
}

SigClient::~SigClient()
{
    if (m_pSocket->IsConnected()) 
        this->Disconnect();
    if (m_pSocket != NULL) 
        delete m_pSocket;
    if (m_pMutex != NULL) 
        delete m_pMutex;
}

int SigClient::GetSignature(AudioSig *sig, string &strGUID)
{
    AutoMutex AM(m_pMutex);
    int nConRes = this->Connect(m_strIP, m_nPort);
    if (nConRes != 0) 
        return -1;

    SigXDR converter;

    int nOffSet = sizeof(char) + sizeof(int);
    int iSigEncodeSize = 35 * sizeof(int32);
    int nTotalSize = nOffSet + iSigEncodeSize;

    char* pBuffer = new char[nTotalSize];
    memset(pBuffer, 0, nTotalSize);
    memcpy(&pBuffer[0], &SigClientVars::cGetGUID, sizeof(char));
    memcpy(&pBuffer[1], &iSigEncodeSize, sizeof(int));

    char *sigencode = converter.FromSig(sig);
    memcpy(&pBuffer[nOffSet], sigencode, iSigEncodeSize);

    int nBytes = 0;
    int ret = m_pSocket->Write(pBuffer, nTotalSize, &nBytes);

    memset(pBuffer, 0, nTotalSize);
    int iGUIDSize = 16 * sizeof(int32);

    ret = m_pSocket->NBRead(pBuffer, iGUIDSize, &nBytes, 
                            SigClientVars::nTimeout);

    
    if ((ret != -1) && (nBytes == iGUIDSize)) {
        ret = 0;
        strGUID = converter.ToStrGUID(pBuffer, nBytes);
    }
    else {
	ret = -1;
        strGUID = "";
    }
    this->Disconnect();

    delete [] pBuffer;

    return ret;
}

int SigClient::Connect(string& strIP, int nPort)
{
    if (m_nNumFailures > 5) 
        return -1;  // server probably down. 
    int nErr = m_pSocket->Connect(strIP.c_str(), nPort, SOCK_STREAM);
    if (nErr == -1)
    {
        m_nNumFailures++;
        return -1;
    }
    m_nNumFailures = 0;
    return 0;
}

int SigClient::Disconnect()
{
    if (m_pSocket->IsConnected())
    {
        char cBuffer[sizeof(char) + sizeof(int)];
        memset(cBuffer, 0, sizeof(char) + sizeof(int));
        cBuffer[0] = SigClientVars::cDisconnect;

        int nBytes = 0;
        m_pSocket->Write(cBuffer, sizeof(char) + sizeof(int), 
                         &nBytes);
        m_pSocket->Disconnect();	
    }
	
    return 0;
}

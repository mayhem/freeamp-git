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


#include "apsutility.h"

const char cFILLMETADATA = '1';

vector<APSMetaData*>* NetToHost(const char* pBuffer, string& strCollectionGUID)
{
    const int nGUIDSize = 16;
    typedef unsigned short int uint16_t;
    //typedef word uint16_t;
    cout << "in nettohost" << endl;

    vector<APSMetaData*>* pAPSList = new vector<APSMetaData*>;
    APSMetaData m_APSMetaData;
    uint16_t* pnFieldSize = new uint16_t[m_APSMetaData.NumFields()];
    uint16_t* pnNetFields = new uint16_t[m_APSMetaData.NumFields()];
    int nOffset = sizeof(char) * 2 + sizeof(uint16_t) + sizeof(uint16_t);
    int nAPSSize = 0;
    int nTotalSize = 0,
        nCRC = 0,
        i = 0;

    cout << "Version: " << pBuffer[0] << endl;
    cout << "Command: " << pBuffer[sizeof(char)] << endl;
    memcpy(&nTotalSize, &pBuffer[sizeof(char)+sizeof(uint16_t)], sizeof(uint16_t));
    memcpy(&nAPSSize, &pBuffer[sizeof(char)*2+sizeof(uint16_t)], sizeof(uint16_t));
    //nTotalSize = ntohs(nTotalSize);
    nAPSSize = ntohs(nAPSSize);
    cout << "Total size: " << nTotalSize << endl;
    cout << "APSSize: " << nAPSSize << endl;

    cout << GenCRC(pBuffer, nTotalSize- sizeof(uint16_t)) << endl;
    memcpy(&nCRC, &pBuffer[nTotalSize-sizeof(uint16_t)], sizeof(uint16_t));
    nCRC = ntohs(nCRC);
    cout << "crc2: " << nCRC << endl;

    // Check for both first byte and second byte being undefined.  
    // If undefined, then drop the packet.
    if (pBuffer[0] != '2')
    {
      cout << "Version undefined.  Packet dropped." << endl;
      // drop packet.
    }
    if (pBuffer[1] != cFILLMETADATA)
    {
      cout << "Command undefined.  Packet dropped." << endl;
      // drop packet.
    }

    for (int j=0; j<nAPSSize; j++)
    {
        APSMetaData* pAPSMetaData = new APSMetaData;
        const int nNumFields = 11;
        char* cFields[nNumFields];
    
        for (i = 0; i < m_APSMetaData.NumFields(); i++)
        {
            memcpy(&pnNetFields[i], &pBuffer[nOffset], sizeof(uint16_t));
            pnFieldSize[i] = ntohs(pnNetFields[i]);
            nOffset += sizeof(uint16_t);
            cFields[i] = new char[pnFieldSize[i]+1];
            memcpy(cFields[i], &pBuffer[nOffset], pnFieldSize[i]+1);
            memset(&(cFields[i][pnFieldSize[i]]), 0, sizeof(char));
            string tempField = cFields[i];
            pAPSMetaData->SetField(i, tempField);
            nOffset += pnFieldSize[i];
            delete cFields[i];
        }
        pAPSList->push_back(pAPSMetaData);
    }

    cout << "offset: " << nOffset << endl;
    // Store CollectionGUID.
    char *pCollectionGUID = new char[nGUIDSize + 1];
    memset(pCollectionGUID, 0, nGUIDSize+1);
    memcpy(&pCollectionGUID[0], &pBuffer[nOffset], nGUIDSize);
    strCollectionGUID = pCollectionGUID;
    delete [] pCollectionGUID;
 
    cout << "CollectionGUID: " << strCollectionGUID << endl;

    if (pnFieldSize != NULL)
    {
        delete [] pnFieldSize;
        pnFieldSize = NULL;
    }
    if (pnNetFields != NULL)
    {
        delete [] pnNetFields;
        pnNetFields = NULL;
    }
    return pAPSList;
}

/*________________________________________________________________________
        
        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 1998-2000 Relatable

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

const char* HostToNet(vector<APSMetaData*>* pAPSList, string& strCollectionGUID,
                      int& nBufferLen)
{
    const int nGUIDSize = 16;
    typedef unsigned short int uint16_t;
    char* pBuffer = NULL;
  
    if (pAPSList->size() > 0)
    {
        APSMetaData m_APSMetaData;
        cout << "pAPSList->size(): " << pAPSList->size() << endl;
 
        vector<uint16_t*>* pnFieldList = new vector<uint16_t*>;
        vector<uint16_t*>* pnNetList = new vector<uint16_t*>;
        int nTotalSize = sizeof(char) * 2 + sizeof(uint16_t) + 
                         sizeof(uint16_t) + (sizeof(uint16_t) * 
                         ((*pAPSList)[0]->NumFields()) * pAPSList->size()) + 
                         nGUIDSize + sizeof(uint16_t);
        uint16_t nNetTotalSize = 0;
        uint16_t nAPSSize = 0;
        int nOffset = sizeof(char) * 2 + sizeof(uint16_t) + sizeof(uint16_t);
        int i = 0;

        cout << "total size: " << nTotalSize << endl;
        for (unsigned int j = 0; j < pAPSList->size(); j++)
        {
            uint16_t* pnFieldSize = new uint16_t[m_APSMetaData.NumFields()];
            uint16_t* pnNetFields = new uint16_t[m_APSMetaData.NumFields()];
            for (i = 0; i < (*pAPSList)[j]->NumFields(); i++)
            {
                pnFieldSize[i] = (*pAPSList)[j]->GetField(i).size();
                nTotalSize += pnFieldSize[i];
            }
            pnFieldList->push_back(pnFieldSize);
    
            for (i = 0; i < m_APSMetaData.NumFields(); i++)
            {
                pnNetFields[i] = htons(pnFieldSize[i]);
            }
            pnNetList->push_back(pnNetFields);
        }

        pBuffer = new char[nTotalSize+1];
        memset(pBuffer, 0, nTotalSize+1);

        memset(&pBuffer[0], '2', sizeof(char));
        memset(&pBuffer[sizeof(char)], cFILLMETADATA, sizeof(char));
        nNetTotalSize = htons(nTotalSize);
        nAPSSize = htons(pAPSList->size());
                    
        memcpy(&pBuffer[sizeof(char) * 2], &nNetTotalSize, sizeof(uint16_t));
        memcpy(&pBuffer[sizeof(char) * 2 + sizeof(uint16_t)], 
               &nAPSSize, sizeof(uint16_t));


        for (unsigned int k=0; k < pnNetList->size(); k++)
        {
            for (i = 0; i < (*pAPSList)[k]->NumFields(); i++)
            {
                memcpy(&pBuffer[nOffset], &(*pnNetList)[k][i], sizeof(uint16_t));
                memcpy(&pBuffer[nOffset+sizeof(uint16_t)], 
                       (*pAPSList)[k]->GetField(i).c_str(), 
                       (*pnFieldList)[k][i]);
                nOffset += sizeof(uint16_t)+(*pnFieldList)[k][i];
            }
        }

        cout << "offset: " << nOffset << endl;
        // Add collection GUID to buffer.
        memcpy(&pBuffer[nOffset], strCollectionGUID.c_str(), nGUIDSize);
        nOffset += nGUIDSize;

        cout << "TotalSize " << nTotalSize << " Offset: " << nOffset << endl;

        uint16_t nCRC = GenCRC(pBuffer, nTotalSize-sizeof(uint16_t));
        cout << "CRC1: " << nCRC << endl;
        nCRC = htons(nCRC);
        memcpy(&pBuffer[nTotalSize - sizeof(uint16_t)], &nCRC, sizeof(uint16_t));

        nBufferLen = nTotalSize;
        if (pnFieldList != NULL)
        {
            for (vector<uint16_t*>::iterator i = pnFieldList->begin(); 
                 i != pnFieldList->end(); i++)
                delete *i;
            delete pnFieldList;
            pnFieldList = NULL;
        }
        if (pnNetList != NULL)
        {
            for (vector<uint16_t*>::iterator i = pnNetList->begin(); 
                 i != pnNetList->end(); i++)
                delete *i;
            delete pnNetList;
            pnNetList = NULL;
        }

        return pBuffer;
    }
    else
    {
        cout << "ERROR: APSList is empty.  Program aborted." << endl;
        exit(1);
    }
}

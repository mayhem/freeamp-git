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

///////////////////////////////////////////////////////////////////
// Copyright 1999 Relatable, LLC. All Rights Reserved
// Programmed By: Sean Ward
// Description: Interface declaration file for Relatable APS system
// Date: 12/13/1999
// Modification History:
// 12/13/1999 : First Created
// Sometime: Stuff
// 07/25/2000 : Lots of stuff/cruft/hallucinations cleaned up
///////////////////////////////////////////////////////////////////

#include "aps.h"
#include "apsplaylist.h"
#include "apsmetadata.h"
#include <strstream>

#ifdef WIN32
#include "wincomsocket.h"
#else
#include "comsocket.h"
#endif

#include "mutex.h"
#include "semaphore.h"
#include "YPClient.h"
#include "apsutility.h"
#include "apsconvert.h"
#include "sigclient.h"
#include <math.h>

#ifndef WIN32
#define ios_base ios
#endif

const int nAPSYPPort  = 4444;
const int nAPSSigPort = 4445;

APSInterface::APSInterface(const char* pIP, const char *pSigIP)
{
    //srand((unsigned)time(NULL)); // comment out if already inited elsewhere
    m_strIP = pIP;
    m_sigIP = pSigIP;
    m_pMutex = new Mutex();
    m_pSema = new Semaphore(MAX_METADATAQUERIES);
    m_pProfileMap = new map<string, string>;
    m_pActiveProfiles = new vector<string>;

    m_pLogFile = NULL;

    LoadProfileMap("profiles.txt"); // could be made into a configurable option

    m_pYpClient = new YPClient;
    m_pYpClient->SetAddress(m_strIP.c_str(), nAPSYPPort);
    m_nMetaFailures = 0;

    m_pSigClient = new SigClient;
    m_pSigClient->SetAddress(m_sigIP.c_str(), nAPSSigPort);

    if (!m_strCurrentProfile.empty()) 
        ChangeProfile(m_strCurrentProfile.c_str());
}

APSInterface::~APSInterface()
{
    WriteProfileMap("profiles.txt");

    if (m_pYpClient != NULL) 
    {
        delete m_pYpClient;
        m_pYpClient = NULL;
    }
    if (m_pSigClient != NULL)
    {
        delete m_pSigClient;
        m_pSigClient = NULL;
    }
    if (m_pMutex != NULL) 
    {
        delete m_pMutex;
        m_pMutex = NULL;
    }
    if (m_pSema != NULL) 
    {
        delete m_pSema;
        m_pSema = NULL;
    }
    if (m_pLogFile != NULL)
    {
        m_pLogFile->close();
        delete m_pLogFile;
        m_pLogFile = NULL;
    }
    if (m_pProfileMap != NULL)
    {
        delete m_pProfileMap;
        m_pProfileMap = NULL;
    }
    if (m_pActiveProfiles != NULL)
    {
        delete m_pActiveProfiles;
        m_pActiveProfiles = NULL;
    }
}

int APSInterface::APSFillMetaData(APSMetaData* pmetaData, bool bUseCollection)
{
    if (pmetaData == NULL) 
        return APS_PARAMERROR;
    string strCollectionID = "NOT_OPTED_IN    ";
    if (m_strCollectionID.size() == 16)
        strCollectionID = m_strCollectionID;

    m_pSema->Wait();

    // Client socket will send pBuffer to server.
    COMSocket TheSock;
    int nErr = SOCKET_ERROR;
    int nNumRetries = 0;
    while ( (nErr == SOCKET_ERROR) && (nNumRetries < 2) && 
            (m_nMetaFailures < 10))  // If error re-try two times with a 3 
                                     // second sleep time.
    {
        nErr = TheSock.Connect("209.249.187.199", 4321, SOCK_STREAM);
        if (nErr == SOCKET_ERROR)
        {
            nNumRetries++;
            m_nMetaFailures++;
            if (m_nMetaFailures == 10)
            {
                //MessageBox(NULL, "MetaData Server Temporarily down. Disabling MetaData Lookups until next restart., "Network Error", MB_OK | MB_SYSTEMMODAL);
                break;
            }
            Sleep(3000);
        }
        else
        {
            m_nMetaFailures = 0;
            vector<APSMetaData*> APSList;
            APSList.push_back(pmetaData);

            int nBufferLen = 0;
            const char* pBuffer = NULL;
            pBuffer = HostToNet(&APSList, strCollectionID, nBufferLen);

            if (nBufferLen == 4096)
            {
                return 1;
            }

            int nBytes = 0;

            nErr = TheSock.Write(pBuffer, nBufferLen, &nBytes);

            char* pBuffer2;
            pBuffer2 = new char[4096];
            memset(pBuffer2, 0, 4096);
            //int nBytes2 = 0;

            vector<APSMetaData*>* pNewAPSList = NULL;

            int nErr = TheSock.NBRead(pBuffer2, 4095, &nBytes, 90);

            if (nErr == 0)
            {
                fstream fin("test.txt", 
                            ios_base::in | ios_base::out | ios_base::app);
                fin << "NBRead timed out " << endl;

                nErr = SOCKET_ERROR;
            }
            else
            {
                // this needs decrufting
                pNewAPSList = NetToHost(pBuffer2, strCollectionID);

                if (pNewAPSList->size() == 1)
		{
                    *pmetaData = *(*pNewAPSList)[0];
                }
                else if (pNewAPSList->size() > 1)
                {
                    // Let user pick from multiple result set.
                    // For now, the first item from the vector will be chosen.
                    // TODO: Create a dialog box and let the user choose from 
                    // result set.
	            *pmetaData = *(*pNewAPSList)[0];
		}
		
                // clean up
                for (vector<APSMetaData*>::iterator i = pNewAPSList->begin(); 
                     i != pNewAPSList->end(); i++)
	            delete *i;

                pNewAPSList->erase(pNewAPSList->begin(), pNewAPSList->end());
                if (pNewAPSList != NULL)
                {
                    delete pNewAPSList;
                    pNewAPSList = NULL;
                }

                break;
            }

            delete [] pBuffer2;
        }
    }
	
    m_pSema->Signal();

    // Debug logging code
    fstream fin("test.txt", ios_base::in | ios_base::out | ios_base::app);
    if (pmetaData->GUID().size() != 16)
    {
        fin << "wrong GUID len for " << pmetaData->Filename() << " guid of : " 
            << pmetaData->GUID() << endl;
    }
    else
    {
        fstream fout("success.txt", 
                     ios_base::in | ios_base::out | ios_base::app);
        fout << pmetaData->Filename() << endl;
        fout << pmetaData->GUID() << endl;
    }
    return APS_NOERROR;
}

int APSInterface::APSLookupSignature(AudioSig *sig, string &strGUID)
{
    if (sig == NULL)
        return APS_PARAMERROR;

    m_pMutex->Acquire();
    int nRes = m_pSigClient->GetSignature(sig, strGUID);
    m_pMutex->Release();

    return nRes;
}

int APSInterface::APSGetPlaylist(APSPlaylist* pPlayList, 
                                 APSPlaylist* pResultList, int nMaxItems, 
                                 bool bLocalOnly)
{
    if ((pPlayList == NULL) || (pResultList == NULL)) 
        return APS_PARAMERROR;

    m_pMutex->Acquire();

    string strUID = "";
    if (!m_strCurrentProfile.empty())
        strUID = (*m_pProfileMap)[m_strCurrentProfile];
    if (strUID.empty())
        strUID = "NOT_OPTED_IN    "; // 16 char aggregate query id

    int nRes = m_pYpClient->GeneratePlayList(*pResultList, *pPlayList, 
                                             nMaxItems, bLocalOnly, strUID,
                                             m_strCollectionID);
    m_pMutex->Release();

    return nRes;
}

int APSInterface::APSSubmitPlaylist(APSPlaylist* pPlayList)
{
    if (pPlayList == NULL)  
        return APS_PARAMERROR;

    m_pMutex->Acquire();
    string strUID = "";

    if (!m_strCurrentProfile.empty())
        strUID = (*m_pProfileMap)[m_strCurrentProfile];

    if (strUID.empty())
        strUID = "NOT_OPTED_IN    ";        // 16 char aggregate query id
    int nRes = m_pYpClient->SubmitPlaylist(*pPlayList, strUID);
    m_pMutex->Release();

    return nRes;
}

void APSInterface::WriteToLog(const string& strGUID, int nEventType = SongDone)
{
    if (m_strCurrentProfile.empty())
        return;  // if no active profile, don't log anything

    m_pMutex->Acquire();
    if (m_pLogFile == NULL)
    {
        // name the logfile after the profile alias
        m_pLogFile = new fstream(m_strCurrentProfile.c_str(), 
                                 ios_base::out | ios_base::app);
    }

    time_t currenttime;
    time(&currenttime);

    *m_pLogFile << currenttime << " " << strGUID << " " << nEventType << endl;

    m_pMutex->Release();
}

int APSInterface::ChangeProfile(const char *pczUserName)
{
    if (pczUserName == NULL) 
        return APS_PARAMERROR;
    if (m_pProfileMap->empty()) 
        return APS_PARAMERROR;

    map<string, string>::iterator i;
    i = m_pProfileMap->find(pczUserName);
    if (i != m_pProfileMap->end())
    {
        if (m_pLogFile != NULL)
        {
            if (m_pLogFile->is_open()) 
                m_pLogFile->close();
            delete m_pLogFile;
            m_pLogFile = NULL;
        }
		
        m_strCurrentProfile = pczUserName;

        CombineProfile(pczUserName);

        m_pLogFile = new fstream(pczUserName, ios_base::out | ios_base::app);
        return APS_NOERROR;
    }
    else
        return APS_PARAMERROR;
}

int APSInterface::APSGetStreams(vector<pair<string, string> >* pResultList)
{
    if (pResultList == NULL) 
        return APS_PARAMERROR;

    m_pMutex->Acquire();
    
    string strUID = "";
       
    if (!m_strCurrentProfile.empty())
        strUID = (*m_pProfileMap)[m_strCurrentProfile];
       
    if (strUID.empty()) 
        strUID = "NOT_OPTED_IN    ";        // 16 char aggregate query id
       
    int nRes = m_pYpClient->GetStreams(*pResultList, strUID);
       
    m_pMutex->Release();

    return nRes;
}

int APSInterface::CreateProfile(const char *pczNewName)
{
    if (pczNewName == NULL) 
        return APS_PARAMERROR;

    m_pMutex->Acquire();

    if ((m_pProfileMap->empty()) || 
        (m_pProfileMap->end() == m_pProfileMap->find(pczNewName)))
    {
        string strGUID = "";
        int nRes = m_pYpClient->GetGUID(strGUID, 0);
        if (nRes == 0) 
        {
            (*m_pProfileMap)[pczNewName] = strGUID;
        }
        if (m_strCollectionID.empty())
        {
            nRes = m_pYpClient->GetGUID(strGUID, 0);
            if (nRes == 0)
            {
                m_strCollectionID = strGUID;
            }
        }

        m_pMutex->Release();

        return ChangeProfile(pczNewName);
    }

    m_pMutex->Release();

    return APS_PARAMERROR;
}

int APSInterface::DeleteProfile(const char *pczNewName, bool bServerToo = false)
{
    // TODO: add server side deletion as well
    if (pczNewName == NULL) 
        return APS_PARAMERROR;

#ifdef WIN32
    DeleteFile((*m_pProfileMap)[pczNewName].c_str());
#else
    unlink((*m_pProfileMap)[pczNewName].c_str());
#endif

    m_pProfileMap->erase(pczNewName);
    if (m_strCurrentProfile == pczNewName)
    {
        m_strCurrentProfile = ""; // clear current profile
        if (m_pLogFile != NULL)
        {
            m_pLogFile->close();
            delete m_pLogFile;
            m_pLogFile = NULL;
        }
    }

    return APS_NOERROR;
}

int APSInterface::LoadProfileMap(const char *pczFile)
{
    if (pczFile == NULL) 
        return APS_PARAMERROR;
    int nNumProfiles = 0;
    fstream fTemp(pczFile, ios_base::in);
    string strCurrentUser;
    string strCollectionID;
    string strUserName;
    string strFilename;

    fTemp >> nNumProfiles >> strCurrentUser >> strCollectionID;
    for ( int i = 0; ((i < nNumProfiles) && !(fTemp.eof())); i++)
    {
        fTemp >> strUserName;
        fTemp >> strFilename;
        if ((!strUserName.empty()) && (!strFilename.empty()))
            (*m_pProfileMap)[strUserName] = strFilename;
    }

    if (!strCurrentUser.empty())
        return ChangeProfile(strCurrentUser.c_str());
    else
        return -1;  // no active profile
}

int APSInterface::WriteProfileMap(const char *pczFile)
{
    if (pczFile == NULL) 
        return APS_PARAMERROR;
    int nNumProfiles = m_pProfileMap->size();

    fstream fTemp(pczFile, ios_base::out | ios_base::trunc);

    map<string, string>::iterator i;
    string strCurrentProfile = m_strCurrentProfile;
    if (strCurrentProfile.empty()) 
        strCurrentProfile = "NULL";

    // to store current profile name (or none)
    fTemp << nNumProfiles << " " << strCurrentProfile << endl;
    fTemp << m_strCollectionID << endl; // to store music collection id

    for (i = m_pProfileMap->begin(); i != m_pProfileMap->end(); i++)
    {
        fTemp << (*i).first << " ";   // store the list of known profiles
        fTemp << (*i).second << endl; // and their GUIDs
    }

    return APS_NOERROR;
}

int APSInterface::CombineProfile(const char *ProfileName)
{
    if (ProfileName == NULL) 
        return APS_PARAMERROR;

    bool bInList = false;

    vector<string>::iterator i;

    for ( i = m_pActiveProfiles->begin(); i != m_pActiveProfiles->end(); i++)
    {
        if (*i == ProfileName) 
            bInList = true;
    }

    if (!bInList)
    {
        map<string, string>::iterator i;
        i = m_pProfileMap->find(ProfileName);
        if (i != m_pProfileMap->end())
        {
            m_pActiveProfiles->insert(m_pActiveProfiles->end(), ProfileName);
        }
    }

    return APS_NOERROR;
}

vector<string>* APSInterface::GetKnownProfiles()
{
    if (m_pProfileMap->empty()) 
        return NULL;

    vector<string>* pReturn = new vector<string>;
    map<string, string>::iterator i = m_pProfileMap->begin();

    if (m_pProfileMap->size() == 1)
    {
        pReturn->insert(pReturn->end(), (*i).first);
    }
    else {
        for (i = m_pProfileMap->begin(); i != m_pProfileMap->end(); i++)
        {
            pReturn->insert(pReturn->end(), (*i).first);
        }
    }

    if (pReturn->empty())
    {
        delete pReturn;
        return NULL;
    }

    return pReturn;
}

vector<string>* APSInterface::GetActiveProfiles()
{
    if (m_pActiveProfiles->empty()) 
        return NULL;

    vector<string>* pReturn = new vector<string>(*m_pActiveProfiles);

    return pReturn;
}

void APSInterface::ClearActiveProfiles()
{
    m_pActiveProfiles->clear();

    if (!m_strCurrentProfile.empty()) 
        CombineProfile(m_strCurrentProfile.c_str());
}

int APSInterface::SyncLog()
{
    if (m_strCurrentProfile.empty()) 
        return APS_PARAMERROR;

    EventRecord TempRecord;
    EventLog TheLog;
    m_pMutex->Acquire();
       
    if (m_pLogFile != NULL) // close the current log file
    {
        if (m_pLogFile->is_open()) 
            m_pLogFile->close();
        delete m_pLogFile;
        m_pLogFile = NULL;
    }
    fstream fIn(m_strCurrentProfile.c_str(), ios_base::in);
       
    TempRecord.strGUID = "blah";
    // Does this deal with the spaces between fields or the binary bits in the 
    // GUID?
    while (!fIn.eof() && !TempRecord.strGUID.empty())
    {
        TempRecord.strGUID.erase(TempRecord.strGUID.begin(), 
                                 TempRecord.strGUID.end());
        fIn >> TempRecord.lTimeStamp >> TempRecord.strGUID 
            >> TempRecord.nEventType;
        if (!TempRecord.strGUID.empty())
        {
            TheLog.push_back(TempRecord);
        }
    }
       
    fIn.close();
    int nRes = m_pYpClient->SyncLog(TheLog, 
                                    (*m_pProfileMap)[m_strCurrentProfile]);
       
    if (nRes == 0)
    {
       // empty the log file
       fIn.open(m_strCurrentProfile.c_str(), ios_base::out | ios_base::trunc);
       fIn.close();
    }
                      
    m_pMutex->Release();
       
    this->ChangeProfile(m_strCurrentProfile.c_str()); // reopen logfile.
    return nRes;
}


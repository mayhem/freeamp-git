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
// Programed By: Sean Ward
// Description: Interface declaration file for Relatable APS system
// Date: 12/13/1999
// Modification History:
// 12/13/1999 : First Created
// Sometime: Stuff
// 07/25/2000 : Lots of stuff/cruft/hallucinations cleaned up
///////////////////////////////////////////////////////////////////
#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include "aps.h"
#include "apsplaylist.h"
#include "apsmetadata.h"
#include "uuid.h"
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
#include "utility.h"

#include <musicbrainz/musicbrainz.h>

#include "slclient.h" // FIXME: remove before b9

#ifndef WIN32
#define ios_base ios
#endif

const int nAPSYPPort  = 4444;
const int nAPSSigPort = 4445;

APSInterface::APSInterface(char *profilePath, const char* pIP, 
                           const char *pSigIP)
{
    //srand((unsigned)time(NULL)); // comment out if already inited elsewhere
    m_strIP = pIP;
    m_sigIP = pSigIP;
    m_pMutex = new Mutex();
    m_pSema = new Semaphore(MAX_METADATAQUERIES);
    m_pProfileMap = new map<string, string>;
    m_pActiveProfiles = new vector<string>;

    m_bRelatableOn = true;

    m_pLogFile = NULL;
    m_profilePath = string(profilePath);
    string savedProfiles = m_profilePath + string(DIR_MARKER_STR) +
                           string("profiles.txt");

    LoadProfileMap(savedProfiles.c_str()); // could be made into a configurable option

    m_pYpClient = new YPClient;
    m_pYpClient->SetAddress(m_strIP.c_str(), nAPSYPPort);
    m_nMetaFailures = 0;

    m_pSigClient = new SigClient; 
    m_pSigClient->SetAddress(m_sigIP.c_str(), nAPSSigPort);

    m_pSLClient = new SoundsLikeClient;
    m_pSLClient->SetAddress(m_sigIP.c_str(), nAPSSigPort);

    if (!m_strCurrentProfile.empty()) {
        ChangeProfile(m_strCurrentProfile.c_str());
        SyncLog();
    }
}

APSInterface::~APSInterface()
{
    string savedProfiles = m_profilePath + string(DIR_MARKER_STR) +
                           string("profiles.txt");
    WriteProfileMap(savedProfiles.c_str());

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

int APSInterface::APSFillMetaData(APSMetaData* pmetaData)
{
    if (pmetaData == NULL) 
        return APS_PARAMERROR;

    m_pSema->Wait();

    MusicBrainz o;
    string  error, data;
    bool    ret;
    vector<string> args;
    char    temp[10], guid[40];

    o.SetServer(string("www.musicbrainz.org"), 80);

    uuid_ascii((unsigned char*)pmetaData->GUID().c_str(), guid);

    args.push_back(pmetaData->Title());
    args.push_back(string(guid));
    args.push_back(pmetaData->Filename());
    args.push_back(pmetaData->Artist());
    args.push_back(pmetaData->Album());
    sprintf(temp, "%d", pmetaData->Track());
    args.push_back(string(temp));
    sprintf(temp, "%d", pmetaData->Length());
    args.push_back(string(temp));
    sprintf(temp, "%d", pmetaData->Year());
    args.push_back(string(temp));
    args.push_back(pmetaData->Genre());
    args.push_back(pmetaData->Comment());

    ret = o.Query(string(MB_ExchangeMetadata), &args);
    if (!ret)
    {
        o.GetQueryError(error);
	m_pSema->Signal();
        return APS_NETWORKERROR;
    }

    // This query should always return one item
    if (o.GetNumItems() == 0)
    {
	m_pSema->Signal();
	return APS_GENERALERROR;
    }

    // Now start the data extraction process.
    // Select the first item in the list of returned items
    o.Select(MB_SelectExchangedData);

    pmetaData->SetArtist(o.Data(MB_GetArtistName).c_str());
    pmetaData->SetTitle(o.Data(MB_GetTrackName).c_str());
    pmetaData->SetAlbum(o.Data(MB_GetAlbumName).c_str());
    pmetaData->SetGenre(o.Data(MB_GetGenre).c_str());
    pmetaData->SetComment(o.Data(MB_GetDescription).c_str());
    pmetaData->SetYear(o.DataInt(MB_GetYear));
    pmetaData->SetTrack(o.DataInt(MB_GetTrackNum));
    pmetaData->SetLength(o.DataInt(MB_GetDuration));

    m_pSema->Signal();

    return APS_NOERROR;
}

int APSInterface::APSLookupSignature(AudioSig *sig, string &strGUID, 
                                     bool bUseCollection)
{
    if (sig == NULL)
        return APS_PARAMERROR;

    m_pMutex->Acquire();

    string strCollectionID = "EMPTY_COLLECTION";
    if (bUseCollection && m_bRelatableOn && !m_strCollectionID.empty())
        strCollectionID = m_strCollectionID;

    int nRes = m_pSigClient->GetSignature(sig, strGUID, strCollectionID);

    fstream fout("sigs.txt", ios_base::out | ios_base::app);
    fout << "GetSignature returned: " << nRes << " and filled in " <<
            strGUID.size() << " : " << strGUID << endl;
    fout.close();

    m_pMutex->Release();

    return nRes;
}

int APSInterface::APSGetSoundsLike(vector<string> *seedGUIDs,
                                   vector<string> *collectionGUIDs,
                                   vector<string> *returnGUIDs,
                                   int items, float fMax)
{
    if ((seedGUIDs == NULL) || (collectionGUIDs == NULL) || 
        (returnGUIDs == NULL))
        return APS_PARAMERROR;

    m_pMutex->Acquire();

    vector<pair<string, float> > *blah;
    blah = m_pSLClient->SoundsLike(seedGUIDs, collectionGUIDs, items, fMax);

    vector<pair<string, float> >::iterator i = blah->begin();
    for (; i != blah->end(); i++)
        returnGUIDs->push_back((*i).first);

    m_pMutex->Release();

    return APS_NOERROR;
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
        strUID = "NOT_OPTED_IN1111"; // 16 char aggregate query id

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
        strUID = "NOT_OPTED_IN1111";        // 16 char aggregate query id
    int nRes = m_pYpClient->SubmitPlaylist(*pPlayList, strUID);
    m_pMutex->Release();

    return nRes;
}

void APSInterface::WriteToLog(const string& strGUID, int nEventType)
{
    if (m_strCurrentProfile.empty())
        return;  // if no active profile, don't log anything

    m_pMutex->Acquire();
    if (m_pLogFile == NULL)
    {
        // name the logfile after the profile alias
        string logfilename = m_profilePath + string(DIR_MARKER_STR) +
                             m_strCurrentProfile;

        m_pLogFile = new fstream(logfilename.c_str(), 
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

        string logfilename = m_profilePath + string(DIR_MARKER_STR) + 
                             string(pczUserName);
        m_pLogFile = new fstream(logfilename.c_str(), 
                                 ios_base::out | ios_base::app);

        string savedProfiles = m_profilePath + string(DIR_MARKER_STR) +
                               string("profiles.txt");
        WriteProfileMap(savedProfiles.c_str());

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
        strUID = "NOT_OPTED_IN1111";        // 16 char aggregate query id
       
    int nRes = m_pYpClient->GetStreams(*pResultList, strUID);
       
    fstream fout("test.txt", ios_base::in | ios_base::out | ios_base::app);
    fout << "GetStream returned: " << nRes << " and filled in " <<
            pResultList->size() << " items:" << endl;
    vector<pair<string, string> >::iterator i;
    for (i = pResultList->begin(); i != pResultList->end(); i++)
    {
        fout << (*i).first << " : " << (*i).second << endl;
    }
    fout.close();

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

int APSInterface::DeleteProfile(const char *pczNewName, bool bServerToo)
{
    // TODO: add server side deletion as well
    if (pczNewName == NULL) 
        return APS_PARAMERROR;

    string logfilename = m_profilePath + string(DIR_MARKER_STR) + 
                         string(pczNewName);
#ifdef WIN32
    DeleteFile(logfilename.c_str());
#else
    unlink(logfilename.c_str());
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

    string savedProfiles = m_profilePath + string(DIR_MARKER_STR) +
                           string("profiles.txt");
    WriteProfileMap(savedProfiles.c_str());

    return APS_NOERROR;
}

int APSInterface::LoadProfileMap(const char *pczFile)
{
    if (pczFile == NULL) 
        return APS_PARAMERROR;

	FILE* pfIn = fopen(pczFile, "r");
	if (pfIn == NULL) return APS_PARAMERROR;

    int nNumProfiles = 0;
    string strCurrentProfileName;
    string strCollectionID;
	bool bIsOn = false;
	int nBufLen = 2048;
	int nStrLen = 0;
	char* pBuffer = new char[nBufLen];
	memset(pBuffer, 0x00, nBufLen);
	
	int nRes = fread(&nNumProfiles, sizeof(int), 1, pfIn);	// Number of profiles
	if (nRes == 0)
	{
		delete [] pBuffer;
		fclose(pfIn);
		return APS_PARAMERROR;
	}
	
	nRes = fread(&bIsOn, sizeof(bool), 1, pfIn); // is on flag
	if (nRes == 0)
	{
		delete [] pBuffer;
		fclose(pfIn);
		return APS_PARAMERROR;
	}
	
	nRes = fread(&nStrLen, sizeof(int), 1, pfIn); // len of current profile
	if ((nRes == 0) && (nStrLen < nBufLen))
	{
		delete [] pBuffer;
		fclose(pfIn);
		return APS_PARAMERROR;
	}
	
	nRes = fread(pBuffer, nStrLen, 1, pfIn); // current profile
	if (nRes == 0)
	{
		delete [] pBuffer;
		fclose(pfIn);
		return APS_PARAMERROR;
	}
	
	strCurrentProfileName = pBuffer;
	memset(pBuffer, 0x00, nStrLen);

	nRes = fread(&nStrLen, sizeof(int), 1, pfIn); // len of collection ID
	if ((nRes == 0) && (nStrLen < nBufLen))
	{
		delete [] pBuffer;
		fclose(pfIn);
		return APS_PARAMERROR;
	}
	
	nRes = fread(pBuffer, nStrLen, 1, pfIn);  // collection id
	if (nRes == 0)
	{
		delete [] pBuffer;
		fclose(pfIn);
		return APS_PARAMERROR;
	}

	strCollectionID = pBuffer;
	memset(pBuffer, 0x00, nStrLen);

	string* pProfNames = new string[nNumProfiles];
	string* pProfGUID = new string[nNumProfiles];
	
	int i = 0;
	for (i = 0; i < nNumProfiles; i++)
	{
		nRes = fread(&nStrLen, sizeof(int), 1, pfIn); // profile name len
		if ((nRes == 0) && (nStrLen < nBufLen))
		{
			delete [] pProfNames;
			delete [] pProfGUID;
			delete [] pBuffer;
			fclose(pfIn);
			return APS_PARAMERROR;
		}
		
		nRes = fread(pBuffer, nStrLen, 1, pfIn); // profile name
		if (nRes == 0)
		{
			delete [] pProfNames;
			delete [] pProfGUID;
			delete [] pBuffer;
			fclose(pfIn);
			return APS_PARAMERROR;
		}
		
		pProfNames[i] = pBuffer;
		memset(pBuffer, 0x00, nStrLen);

		nRes = fread(&nStrLen, sizeof(int), 1, pfIn); // GUID len
		if ((nRes == 0) && (nStrLen < nBufLen))
		{
			delete [] pProfNames;
			delete [] pProfGUID;
			delete [] pBuffer;
			fclose(pfIn);
			return APS_PARAMERROR;
		}
		
		nRes = fread(pBuffer, nStrLen, 1, pfIn); // GUID
		if (nRes == 0)
		{
			delete [] pProfNames;
			delete [] pProfGUID;
			delete [] pBuffer;
			fclose(pfIn);
			return APS_PARAMERROR;
		}
		
		pProfGUID[i] = pBuffer;
		memset(pBuffer, 0x00, nStrLen);
	}
	
	// having successfully read in the entire profile log file, apply it
    fclose(pfIn);

    if (strCollectionID == "NULL")
        m_strCollectionID = "";
    else
        m_strCollectionID = strCollectionID;

	m_bRelatableOn = bIsOn;
    for (i = 0; i < nNumProfiles; i++)
    {
        if ((!pProfNames[i].empty()) && (!pProfGUID[i].empty()))
            (*m_pProfileMap)[pProfNames[i]] = pProfGUID[i];
    }
	
	delete [] pProfNames;
	delete [] pProfGUID;
	delete [] pBuffer;

    if (!strCurrentProfileName.empty())
        return ChangeProfile(strCurrentProfileName.c_str());
    else
        return -1;  // no active profile
}

int APSInterface::WriteProfileMap(const char *pczFile)
{
    if (pczFile == NULL) 
        return APS_PARAMERROR;

    int nNumProfiles = m_pProfileMap->size();
    int nStrLen = 0;
    FILE* pfOut = fopen(pczFile, "w+b");
	
    if (pfOut == NULL)
	return APS_PARAMERROR;

    map<string, string>::iterator i;
    string strCurrentProfile = m_strCurrentProfile;
    if (strCurrentProfile.empty()) 
        strCurrentProfile = "NULL";
    
    string strCollectionID = m_strCollectionID;
    if (strCollectionID.empty())
        strCollectionID = "NULL";
	
    // file format is : numprofiles, isenabled flag, current profile name len, current profile name, 
    //	current collection id len, current collection id, 
    // for each profile entry, name len, name, guid len, guid
    int nRes = fwrite(&nNumProfiles, sizeof(int), 1, pfOut);
    if (nRes == 0) 
    {
	fclose(pfOut);
	return APS_PARAMERROR;
    }
	
	nRes = fwrite(&m_bRelatableOn, sizeof(bool), 1, pfOut);
	if (nRes == 0) 
	{
		fclose(pfOut);
		return APS_PARAMERROR;
	}

	nStrLen = strCurrentProfile.size();
	nRes = fwrite(&nStrLen, sizeof(int), 1, pfOut);
	if (nRes == 0) 
	{
		fclose(pfOut);
		return APS_PARAMERROR;
	}
	
	nRes = fwrite(strCurrentProfile.c_str(), nStrLen, 1, pfOut);
	if (nRes == 0) 
	{
		fclose(pfOut);
		return APS_PARAMERROR;
	}
	
	nStrLen = strCollectionID.size();
	nRes = fwrite(&nStrLen, sizeof(int), 1, pfOut);
	if (nRes == 0) 
	{
		fclose(pfOut);
		return APS_PARAMERROR;
	}

	nRes = fwrite(strCollectionID.c_str(), nStrLen, 1, pfOut);
	if (nRes == 0) 
	{
		fclose(pfOut);
		return APS_PARAMERROR;
	}
	
    for (i = m_pProfileMap->begin(); i != m_pProfileMap->end(); i++)
    {
		nStrLen = (*i).first.size();
		nRes = fwrite(&nStrLen, sizeof(int), 1, pfOut);
		if (nRes == 0) 
		{
			fclose(pfOut);
			return APS_PARAMERROR;
		}

		nRes = fwrite((*i).first.c_str(), nStrLen, 1, pfOut);
		if (nRes == 0) 
		{
			fclose(pfOut);
			return APS_PARAMERROR;
		}
		
		nStrLen = (*i).second.size();
		nRes = fwrite(&nStrLen, sizeof(int), 1, pfOut);
		if (nRes == 0) 
		{
			fclose(pfOut);
			return APS_PARAMERROR;
		}
		
		nRes = fwrite((*i).second.c_str(), nStrLen, 1, pfOut);
		if (nRes == 0) 
		{
			fclose(pfOut);
			return APS_PARAMERROR;
		}
    }

    fclose(pfOut);
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
    if (!IsTurnedOn()) 
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
    if (!IsTurnedOn())
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
    string logfilename = m_profilePath + string(DIR_MARKER_STR) +
                         m_strCurrentProfile;

    fstream fIn(logfilename.c_str(), ios_base::in);
       
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
       fIn.open(logfilename.c_str(), ios_base::out | ios_base::trunc);
       fIn.close();
    }
                      
    m_pMutex->Release();
       
    this->ChangeProfile(m_strCurrentProfile.c_str()); // reopen logfile.
    return nRes;
}

bool APSInterface::IsTurnedOn()
{
    if (m_bRelatableOn && m_strCurrentProfile != "")
        return true;
    return false;
}

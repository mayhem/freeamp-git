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

///////////////////////////////////////////////////////////////////
// Copyright 1999 Relatable, LLC. All Rights Reserved
// Programmed By: Sean Ward
// Description: Interface declaration file for Relatable APS system
// Date: 12/2/1999
// Modification History:
// 12/2/1999 : First Created
// 02/14/2000 : linux code synced
// 07/25/2000 : major decrufting
///////////////////////////////////////////////////////////////////

#ifndef APSINTERFACE_H
#define APSINTERFACE_H

#ifdef WIN32
#pragma warning (disable :4786)
#endif

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>

using namespace std;
#define MAX_METADATAQUERIES 10

class APSPlaylist;
class APSHistory;
class APSMetaData;
class Mutex;
class Semaphore;
class YPClient;
class AudioSig;
class SoundsLikeClient;

#define APS_NOERROR       0
#define APS_EMPTYLIST     1
#define APS_ITEMNOTINLIST 2
#define APS_PARAMERROR    3
#define APS_ENDLIST       4
#define APS_MEMERROR      6
#define APS_NETWORKERROR  7
#define APS_GENERALERROR  8

typedef vector<pair<string, string> > StreamList;

// Primary APSInterface Class. Create only one of this object. Used for all communications to the APS Server.
class APSInterface
{
public:
    /** Defines profile log events */
    enum
    {
        SongDone = 0,
        SongSkip = 1,
        SongRateLike = 3,
        SongRateHate = 4
    } APSEvent;
       
    /** IP of APS Servers can be overwridden in constructor. */
    APSInterface(char *profilePath, const char* pYpIP  = "209.249.187.199", 
                 const char* pSigIP = "209.249.187.199");
    virtual ~APSInterface(); // to be or not to be virtual....
       
    /** Attempts to finish filling in the metadata Record, 
        by comparing the record/filenames against a master music 
        database. */ 
    int APSFillMetaData(APSMetaData* pmetaData);
      
    // Playlist Submission/Sorting/Generation Functions
    /** TEMPORARY.  Used to generate a playlist based on the SoundsLike algo.
        To be removed before next beta */
    int APSGetSoundsLike(vector<string> *seedGUIDs,
                         vector<string> *collectionGUIDs,
                         vector<string> *returnGUIDs,
                         int items, float fMax = -1.0);

    /** Used to generate a playlist. pPlayList is an optional
        list of seed items to focus the recommendation from, otherwise
        a profile query based on a psuedorandom gittering of the current
        active profile's most popular songs is used. The resulting playlist
        is stored in pResultList, containing up to nMaxItems, or 0 for the
        everything the server has decided to toss back ;). Finally, the
        optional bLocalOnly flag tells the server to only recommend music 
        available in your music collection, instead of including shoutcast
        streamable music as well. */
    int APSGetPlaylist(APSPlaylist* pPlayList, APSPlaylist* pResultList, 
                       int nMaxItems = 0, bool bLocalOnly = false); 
  
    /** Explicitly submits a playlist as liked. Used mostly for
        manually training music relationships, or to submit any manually
        created playlists */
    int APSSubmitPlaylist(APSPlaylist* pPlayList);  
 
    /** Returns the list of recommended shoutcast streams, based on
        a comparison of the current active profiles to the currently
        available shoutcast/icecast streams. The return list is contained
        in best match order, with the first string containing the 
        stream name, and the second the URL for the stream */
    int APSGetStreams(StreamList* pResultList);
      
    // User Management Functions
    /** Returns the names (aliases actually) of the current set of
        active profiles. */
    vector<string>* GetActiveProfiles();
    
    /** Clears the ActiveProfiles list, returning to using only one 
        profile for queries */
    void ClearActiveProfiles();
   
    /** Returns the list of profile aliases known on this machine */
    vector<string>* GetKnownProfiles();
  
    /** Used to add another profile alias to the active set. This 
        is done to allow the system to combine the preferences of more than
        one person, such as when you want to generate playlists which would
        be of interest to you and some friends. Mathematically, it tells the 
        system to focus on the intersection of one or more profiles when it 
        generates recommendations (although some people are unreconcilable ;)*/
    int CombineProfile(const char* ProfileName);
 
    /** Returns the name (alias) of the current active profile
        Note: only the active profile has implicit event logging active, 
        ie, learning what you like from listen patterns. If the return
        string is empty, no profiles are active and all recommendation/
        profile functions are not opted in. */
    string GetCurrentProfileName() { return m_strCurrentProfile; }

    /** Adds an action event to the current active profile log.
        nEventType is one of the actions from the APSEvent enum. 
        If no profile is current, the event is discarded */
    void WriteToLog(const string& strGUID, int nEventType = SongDone);

    /** Syncs the server side profile log with recent listen events */
    int SyncLog();

    /** Deletes a profile alias from this computer. If bServerToo
        is set, it also sends a delete message to the APSServer */
    int DeleteProfile(const char* pczNewName, bool bServerToo = false);

    /** Change the current active profile */
    int ChangeProfile(const char *pczUserName);

    /** Add a new profile, registering with the YP Server */
    int CreateProfile(const char *pczUserName);

    /** Convenience function to wrap the active status */
    bool IsTurnedOn(void);

    /** Convenience functions to quickly enable + disable all these features */
    void TurnOff(void) { m_bRelatableOn = false; }
    void TurnOn(void) { m_bRelatableOn = true; }
    
    /** Convenience function to just get the status of the OnOff flag */
    bool GetTurnedOnFlag(void) { return m_bRelatableOn; }

    string GetCollectionId(void) { return m_strCollectionID; }

protected:
    /** Helper function to save the profile alias map */
    int WriteProfileMap(const char* pczFile);
    /** Helper function to load the profile alias map */
    int LoadProfileMap(const char* pczFile);

private:
    vector<string>* m_pActiveProfiles;// list of multiple profiles for 
                                      // combination recommendations
    string m_strCurrentProfile;      // name of current active profile.
                                     // empty for non opt in clients.
    string m_strCollectionID;        // collection id of current music 
                                     // collection, so that you aren't
                                     // recommended stuff that isn't on your
                                     // HD or streamable ;)
    map<string, string>* m_pProfileMap; // map of profile names-profile GUIDs
    Mutex *m_pMutex;                 // Mutex for thread saftey
    string m_strIP;                  // ip of YPServer
    int m_nPort;                     // port of YPServer
    string m_sigIP;                  // ip of SigServer
    int m_nSigPort;                  // port of SigServer
    fstream* m_pLogFile;             // open log file of current action log
    Semaphore* m_pSema;              // Semaphore to enforce max# simulataneous
                                     // metadata lookups
    YPClient* m_pYpClient;           // Wraps the connection logic for talking
                                     // to the YourPlay Servers
    int m_nMetaFailures;             // track metadata timeouts to stop lookups
                                     // if the server is down ;)
    string m_profilePath;            // store the path to the profile
    bool m_bRelatableOn;             // is this thing on?

    SoundsLikeClient *m_pSLClient;   // FIXME: remove before b9
};

#endif

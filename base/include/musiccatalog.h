/*____________________________________________________________________________
 
        FreeAmp - The Free MP3 Player
 
        Portions Copyright (C) 1999 EMusic.com

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program; if not, write to the Free Software
        Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

        $Id$
 ____________________________________________________________________________*/

#ifndef INCLUDED_MUSICBROWSER_H_
#define INCLUDED_MUSICBROWSER_H_

#include <vector>
#include <string>
using namespace std;

#include "event.h"
#include "database.h"
#include "metadata.h"
#include "playlist.h"

typedef enum {
   kTypeTrack = 0,
   kTypeStream
} MetadataStorageType;

class FAContext;
class Player;

class AlbumList {
 public:
     AlbumList() { m_trackList = new vector<PlaylistItem *>; }
    ~AlbumList() {
                      while (m_trackList->size() > 0) {
                          delete (*m_trackList)[0];
                          m_trackList->erase(m_trackList->begin());
                      }
                      delete m_trackList;
                      m_trackList = NULL;
                  }

    vector<PlaylistItem *> *m_trackList;
    string name;
};

class ArtistList {
 public:
    ArtistList() { m_albumList = new vector<AlbumList *>; }
   ~ArtistList() {
                     while (m_albumList->size() > 0) {
                         delete (*m_albumList)[0];
                         m_albumList->erase(m_albumList->begin());
                     }
                     delete m_albumList;
                     m_albumList = NULL;
		 }
    
    vector<AlbumList *> *m_albumList;
    string name;
};

class MusicCatalog : public EventQueue
{
 public:
    MusicCatalog(FAContext *context, char *databasepath = NULL);
    virtual ~MusicCatalog();

    void SetDatabase(const char *path);

    void SearchMusic(vector<string> &pathList);
    void StopSearchMusic(void);

    void WriteMetaDataToDatabase(const char *url, const MetaData metadata,
                                 MetadataStorageType type = kTypeTrack);
    MetaData *ReadMetaDataFromDatabase(const char *url);

    Error AddPlaylist(const char *url);
    Error AddSong(const char *url);
    Error AddStream(const char *url);
    Error Add(const char *url);    

    Error UpdateSong(PlaylistItem *item);

    Error RePopulateFromDatabase();

    Error RemovePlaylist(const char *url);
    Error RemoveSong(const char *url);
    Error RemoveStream(const char *url);
    Error Remove(const char *url);

    void  ClearCatalog(void);

    void  Sort(void);

    const vector<ArtistList *> *GetMusicList(void) { return m_artistList; }
    const vector<PlaylistItem *> *GetUnsortedMusic(void) { return m_unsorted; }
    const vector<string> *GetPlaylists(void) { return m_playlists; }
    const vector<PlaylistItem *> *GetStreams(void) { return m_streams; }

    void  GetCatalogLock(void) { m_catMutex->Acquire(); }
    void  ReleaseCatalogLock(void) { m_catMutex->Release(); }

    virtual Error AcceptEvent(Event *e);

 protected:
    static void musicsearch_thread_function(void *arg);
    static void prune_thread_function(void *arg);

    void PruneThread(bool sendmessages);

    void DoSearchMusic(char *path);
    void DoSearchPaths(vector<string> &pathList);
    void PruneDatabase(bool sendmessages = false, bool spawn = false);

    bool m_exit;
    Mutex *m_mutex;

    FAContext *m_context;

 private:
    bool CaseCompare(string s1, string s2);

    Mutex *m_catMutex;

    vector<ArtistList *> *m_artistList;
    vector<PlaylistItem *> *m_unsorted;
    vector<string> *m_playlists;
    vector<PlaylistItem *> *m_streams;

    Database *m_database;
    PlaylistManager *m_plm;

    bool   m_inUpdateSong;
    bool   m_acceptItemChanged;
    uint32 m_itemWaitCount;

    PlaylistItem *m_newItem;
    PlaylistItem *m_oldItem;
    ArtistList *m_newArtist;
    ArtistList *m_oldArtist;
    AlbumList *m_newAlbum;
    AlbumList *m_oldAlbum;
};
#endif

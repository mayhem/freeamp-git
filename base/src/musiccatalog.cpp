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

// The debugger can't handle symbols more than 255 characters long.
// STL often creates symbols longer than that.
// When symbols are longer than 255 characters, the warning is disabled.
#ifdef WIN32
#pragma warning(disable:4786) 
#endif

#include <sys/stat.h>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#include "win32impl.h"
#endif

#include <string>
#include <algorithm>
#ifdef unix
#include <strstream>
typedef ostrstream ostringstream;
#else
#include <sstream>
#endif

using namespace std;

#include "config.h"
#include "musiccatalog.h"
#include "player.h"
#include "utility.h"
#include "debug.h"

#define METADATABASE_VERSION 1

MusicCatalog::MusicCatalog(FAContext *context, char *databasepath)
{
    m_database = NULL;
    m_context = context;
    m_plm = context->plm;
    m_mutex = new Mutex();
    m_inUpdateSong = false;
    m_acceptItemChanged = false;
    m_artistList = new vector<ArtistList *>;
    m_unsorted = new vector<PlaylistItem *>;
    m_playlists = new vector<string>;
   
    if (databasepath)
        SetDatabase(databasepath);
}

MusicCatalog::~MusicCatalog()
{
    if (m_database)
        delete m_database;
    delete m_artistList;
    delete m_unsorted;
    delete m_playlists;
    delete m_mutex;
}

class comp_catalog {
  public:
      bool operator()(PlaylistItem *a, PlaylistItem *b)
      {
          if (a->GetMetaData().Track() == b->GetMetaData().Track())
             // sort alphabetically...
             return (a->GetMetaData().Title() < b->GetMetaData().Title());
          return (a->GetMetaData().Track() < b->GetMetaData().Track());
      }
      bool operator()(AlbumList *a, AlbumList *b)
      {
          return (a->name < b->name);
      }
      bool operator()(ArtistList *a, ArtistList *b)
      {
          return (a->name < b->name);
      }
};
          
void MusicCatalog::Sort(void)
{
    // Sort the playlists...
    sort(m_playlists->begin(), m_playlists->end());

    // Sort the uncategorized tracks
    sort(m_unsorted->begin(), m_unsorted->end(), comp_catalog());

    // Sort the rest o the junk
    vector<ArtistList *>::iterator i = m_artistList->begin();
    for (; i != m_artistList->end(); i++) {
        vector<AlbumList *>::iterator j = (*i)->m_albumList->begin();
        for (; j != (*i)->m_albumList->end(); j++) {
            sort((*j)->m_trackList->begin(), (*j)->m_trackList->end(), 
                 comp_catalog());
        }
        sort((*i)->m_albumList->begin(), (*i)->m_albumList->end(), 
             comp_catalog());
    }
    sort(m_artistList->begin(), m_artistList->end(), comp_catalog());
}

Error MusicCatalog::RemovePlaylist(const char *url)
{
    assert(url);

    if (!m_database->Working())
        return kError_DatabaseNotWorking;

    char *data = m_database->Value(url);

    if (!data)
        return kError_ItemNotFound;

    delete [] data;
    m_database->Remove(url);

    vector<string>::iterator i = m_playlists->begin();
    for (; i != m_playlists->end(); i++)
         if ((*i) == url) {
             string tempstr = url;
             if (tempstr.find("currentlist.m3u") >= tempstr.length()) {
                 string sUrl = url; 
                 m_context->target->AcceptEvent(new MusicCatalogPlaylistRemovedEvent(sUrl));
                 m_playlists->erase(i);
             }
             return kError_NoErr;
         }   

    return kError_ItemNotFound;
}


Error MusicCatalog::AddPlaylist(const char *url)
{
    assert(url);

    if (!m_database->Working())
        return kError_DatabaseNotWorking;
    
    char *data = m_database->Value(url);
    if (!data) 
        m_database->Insert(url, "P");
    else 
        delete [] data;

    bool found = false;
    vector<string>::iterator i = m_playlists->begin();
    for (; i != m_playlists->end(); i++)
         if ((*i) == url)
             found = true;
    
    if (!found) {
        string tempstr = url;
        if (tempstr.find("currentlist.m3u") < tempstr.length()) 
            m_playlists->insert(m_playlists->begin(), url);
        else
            m_playlists->push_back(url);

        string sUrl = url;
        m_context->target->AcceptEvent(new MusicCatalogPlaylistAddedEvent(sUrl));
        return kError_NoErr;
    }

    return kError_DuplicateItem;
}

Error MusicCatalog::RemoveSong(const char *url)
{
    assert(url);

    if (!m_database->Working())
        return kError_DatabaseNotWorking;

    MetaData *meta = ReadMetaDataFromDatabase(url);
    if (!meta)
        return kError_ItemNotFound;

    m_database->Remove(url);

    if ((meta->Artist().size() == 0) || (meta->Artist() == " ")) {
        vector<PlaylistItem *>::iterator i = m_unsorted->begin();
        for (; i != m_unsorted->end(); i++)
            if (url == (*i)->URL())
            {
                AcceptEvent(new MusicCatalogTrackRemovedEvent(*i, NULL, NULL));     
                m_unsorted->erase(i);
                break;
            }
    }
    else 
    {
        vector<ArtistList *>::iterator    i;
        vector<AlbumList *>              *alList;
        vector<AlbumList *>::iterator     j;
        vector<PlaylistItem *>           *trList;
        vector<PlaylistItem *>::iterator  k;
        bool                              found = false;
        
        i = m_artistList->begin();
        for (; i != m_artistList->end() && !found; i++) 
        {
            if (meta->Artist() == (*i)->name) 
            {
                alList = (*i)->m_albumList;
                j = alList->begin();
                for (; j != alList->end() && !found; j++) 
                {
                    if (meta->Album() == (*j)->name) 
                    {
                        trList = (*j)->m_trackList;
                        k = trList->begin();
                        for (; k != trList->end() && !found; k++)
                            if (url == (*k)->URL())
                            {
                                AcceptEvent(new MusicCatalogTrackRemovedEvent(*k, *i, *j));
                                trList->erase(k);
                                found = true;
                                break;
                            }    

                        if (trList->size() == 0)
                            alList->erase(j);
                     }
                }

                if (alList->size() == 0)
                    m_artistList->erase(i);
            }
        }
    }

    delete meta;

    return kError_NoErr;
}

Error MusicCatalog::AddSong(const char *url)
{
    assert(url);

    if (!m_database->Working())
        return kError_DatabaseNotWorking;

    PlaylistItem *newtrack;

    MetaData *meta = ReadMetaDataFromDatabase(url);

    if (!meta) {
        newtrack = new PlaylistItem(url);
        m_context->plm->RetrieveMetaData(newtrack);

        while (newtrack->GetState() != kPlaylistItemState_Normal)
            usleep(5);

        MetaData tempdata = (MetaData)(newtrack->GetMetaData());
        WriteMetaDataToDatabase(url, tempdata);
        meta = ReadMetaDataFromDatabase(url);
    }
    else
        newtrack = new PlaylistItem(url, meta);

    if ((meta->Artist().size() == 0) || (meta->Artist() == " ")) {
        m_unsorted->push_back(newtrack);
        AcceptEvent(new MusicCatalogTrackAddedEvent(newtrack, NULL, NULL));
    }
    else {
        bool changed = false;
        if (meta->Album() == " " || meta->Album().size() == 0) {
            meta->SetAlbum("Unknown");
            changed = true;
        }
        if (meta->Title() == " " || meta->Title().size() == 0) {
            meta->SetTitle("Unknown");
            changed = true;
        }
  
        if (changed)
            WriteMetaDataToDatabase(url, *meta);

        bool found_artist = false;
        vector<ArtistList *>::iterator i = m_artistList->begin();
        for (; i != m_artistList->end(); i++) {
            if (meta->Artist() == (*i)->name) {
                bool found_album = false;
                found_artist = true;
                vector<AlbumList *> *alList = (*i)->m_albumList;
                vector<AlbumList *>::iterator j = alList->begin();
                for (; j != alList->end(); j++) {
                    if (meta->Album() == (*j)->name) {
                        found_album = true;
                       
                        vector<PlaylistItem *> *trList = (*j)->m_trackList;
                        vector<PlaylistItem *>::iterator k = trList->begin();
                        for (; k != trList->end(); k++) {
                            if (url == (*k)->URL()) {
                                delete newtrack;                                
                                return kError_DuplicateItem;
                            }
                        }
                        (*j)->m_trackList->push_back(newtrack);
                        AcceptEvent(new MusicCatalogTrackAddedEvent(newtrack, *i, *j));
                        break;
                     }
                }
                if (!found_album) {
                    AlbumList *newalbum = new AlbumList;
                    newalbum->name = meta->Album();
                    newalbum->m_trackList->push_back(newtrack);
                    alList->push_back(newalbum);
                    AcceptEvent(new MusicCatalogTrackAddedEvent(newtrack, (*i), newalbum));
                    break;
                }
            }
        }
        if (!found_artist) {
            ArtistList *newartist = new ArtistList;
            newartist->name = meta->Artist();
            AlbumList *newalbum = new AlbumList;
            newalbum->name = meta->Album();
            newalbum->m_trackList->push_back(newtrack);
            newartist->m_albumList->push_back(newalbum);
            m_artistList->push_back(newartist);
            AcceptEvent(new MusicCatalogTrackAddedEvent(newtrack, newartist, newalbum));
        }
    }
    return kError_NoErr;
}

Error MusicCatalog::UpdateSong(PlaylistItem *item)
{
    assert(item);

    if (!m_database->Working())
        return kError_DatabaseNotWorking;

    m_inUpdateSong = true;

    Error err = RemoveSong(item->URL().c_str());
    
    if (IsError(err))
        return err;
   
    WriteMetaDataToDatabase(item->URL().c_str(), item->GetMetaData());

    m_database->Sync();

    err = AddSong(item->URL().c_str());

    if (IsError(err))
        return err;

    m_inUpdateSong = false;

    m_context->target->AcceptEvent(new MusicCatalogTrackChangedEvent(m_oldItem, m_newItem, m_oldArtist, m_newArtist, m_oldAlbum, m_newAlbum));

    return kError_NoErr;
}

Error MusicCatalog::Remove(const char *url)
{
    assert(url);

    if (!m_database->Working())
        return kError_DatabaseNotWorking;

    char *data = m_database->Value(url);
    if (!data)
        return kError_ItemNotFound;

    Error retvalue = kError_YouScrewedUp;

    if (!strncmp("P", data, 1))
        retvalue = RemovePlaylist(url);
    else if (!strncmp("M", data, 1))
        retvalue = RemoveSong(url);

    delete [] data;

    return retvalue;
}

Error MusicCatalog::Add(const char *url)
{
    assert(url);

    if (!m_database->Working())
        return kError_DatabaseNotWorking;

    char *data = m_database->Value(url);
    if (!data)
        return kError_ItemNotFound;

    Error retvalue = kError_YouScrewedUp;

    if (!strncmp("P", data, 1))
        retvalue = AddPlaylist(url);
    else if (!strncmp("M", data, 1)) 
        retvalue = AddSong(url);

    delete [] data;

    return retvalue;
}

void MusicCatalog::ClearCatalog()
{
    delete m_artistList;
    delete m_unsorted;
    delete m_playlists;
    m_artistList = new vector<ArtistList *>;
    m_unsorted = new vector<PlaylistItem *>;
    m_playlists = new vector<string>;
}

Error MusicCatalog::RePopulateFromDatabase()
{
    if (!m_database->Working())
        return kError_DatabaseNotWorking;

    ClearCatalog();
 
    char *key = m_database->NextKey(NULL);
    Error err = kError_NoErr;

    while (key) {
        err = Add(key);

        if (IsError(err)) {
             m_context->target->AcceptEvent(new ErrorMessageEvent("There was an internal error during generation of the Music Catalog"));
             return kError_YouScrewedUp;
        }
            
        key = m_database->NextKey(key);
    }
    return kError_NoErr;
}

void MusicCatalog::SetDatabase(const char *path)
{
    if (m_database)
        delete m_database;

    m_database = new Database(path, METADATABASE_VERSION);

    if (!m_database->Working()) {
        delete m_database;
        m_database = NULL;
    }

    PruneDatabase();

    if (m_database) {
        RePopulateFromDatabase();
        Sort();
    }
}

void MusicCatalog::PruneDatabase(void)
{
    char *key = m_database->NextKey(NULL);
    struct stat st;

    while (key) {
        if (!strncmp("file://", key, 7)) {
            uint32 length = strlen(key) + 1;
            char *filename = new char[length];

            if (IsntError(URLToFilePath(key, filename, &length)))
                if (-1 == stat(filename, &st)) {
                    m_database->Remove(key);
                    key = NULL;
                }

            delete [] filename;
        }
        key = m_database->NextKey(key);
    }
}

typedef struct MusicSearchThreadStruct {
    MusicCatalog *mc;
    vector<string> pathList;
    Thread *thread;
} MusicSearchThreadStruct;

void MusicCatalog::SearchMusic(vector<string> &pathList)
{
    if (!m_database->Working())
        return;

    Thread *thread = Thread::CreateThread();

    if (thread) {
        MusicSearchThreadStruct *mst = new MusicSearchThreadStruct;

        mst->mc = this;
        mst->pathList = pathList;
        mst->thread = thread;

        thread->Create(musicsearch_thread_function, mst);
    }
}

void MusicCatalog::StopSearchMusic(void)
{
    m_exit = true;
}

void MusicCatalog::musicsearch_thread_function(void *arg)
{
    MusicSearchThreadStruct *mst = (MusicSearchThreadStruct *)arg;

    mst->mc->m_mutex->Acquire();

    mst->mc->m_exit = false;
    mst->mc->DoSearchPaths(mst->pathList);

    mst->mc->AcceptEvent(new Event(INFO_SearchMusicDone));

    mst->mc->m_mutex->Release();

    delete mst->thread;
    delete mst;
}

void MusicCatalog::DoSearchPaths(vector<string> &pathList)
{
    vector<string>::iterator i;
    
    m_acceptItemChanged = true;
    m_itemWaitCount = 0;

    for(i = pathList.begin(); i != pathList.end(); i++)
        DoSearchMusic((char *)(*i).c_str());

    while (m_itemWaitCount > 0)
        usleep(5);
    m_acceptItemChanged = false;
}

void MusicCatalog::DoSearchMusic(char *path)
{
    vector<PlaylistItem *> *metalist = new vector<PlaylistItem *>;

    WIN32_FIND_DATA find;
    HANDLE handle;
    string search = path;

#ifndef WIN32
    if (!strcmp(path, "/dev") || !strcmp(path, "/proc"))
        return;
#endif
 
    string *info = new string("Searching: ");
    (*info) += path;
    m_context->player->AcceptEvent(new BrowserMessageEvent(info->c_str()));
    delete info;
   
    if (search[search.size() - 1] != DIR_MARKER)
       search.append(DIR_MARKER_STR);

    search.append("*");

    handle = FindFirstFile((char *)search.c_str(), &find);

    if (handle != INVALID_HANDLE_VALUE)
    {
        do
        {
            char *fileExt;
#ifndef WIN32
            if (find.dwFileAttributes == FILE_ATTRIBUTE_SYMLINK)
                continue;
#endif
            if (find.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
            {
                if (!(!strcmp("." , find.cFileName) || !strcmp("..", find.cFileName)))
                {
                    string newDir = path; 
                    if (path[strlen(path) - 1] != DIR_MARKER)
                        newDir.append(DIR_MARKER_STR);
                    newDir.append(find.cFileName);

                    DoSearchMusic((char *)newDir.c_str());
                }
            }
            else if ((fileExt = m_context->player->GetExtension(find.cFileName)))
            {
                if (m_plm->IsSupportedPlaylistFormat(fileExt) && 
                    strcmp("currentlist.m3u", find.cFileName))
                {
                    string file = path;
                    file.append(DIR_MARKER_STR);
                    file.append(find.cFileName);

                    uint32 urlLength = strlen(file.c_str()) + 10;
                    char *url = new char[urlLength];

                    if (IsntError(FilePathToURL(file.c_str(), url, &urlLength)))
                        m_database->Insert(url, "P");

                    delete [] url;
                }
                else if (m_context->player->IsSupportedExtension(fileExt))
                {
                    string file = path;
                    file.append(DIR_MARKER_STR);
                    file.append(find.cFileName);

                    char *tempurl = new char[file.length() + 10];
                    uint32 length = file.length() + 10;

                    if (IsError(FilePathToURL(file.c_str(), tempurl, &length)))
                    {
                        continue;
                    }
                       
                    PlaylistItem *plist = new PlaylistItem(tempurl);
                    metalist->push_back(plist);
                    m_itemWaitCount++;

                    delete [] tempurl;
                }
                delete fileExt;
            }
        }
        while (FindNextFile(handle, &find) && !m_exit);
        FindClose(handle);

        if (metalist->size() > 0)
            m_plm->RetrieveMetaData(metalist);
        else 
            delete metalist;
    }
}

void MusicCatalog::WriteMetaDataToDatabase(const char *url, 
                                           const MetaData metadata)
{
    if (!m_database->Working())
        return;

    ostringstream ost;
    char num[256];
    const char *kDatabaseDelimiter = " ";

    ost << "M" << kDatabaseDelimiter;

    ost << 9 << kDatabaseDelimiter;

    ost << metadata.Artist().size() << kDatabaseDelimiter;
    ost << metadata.Album().size() << kDatabaseDelimiter;
    ost << metadata.Title().size() << kDatabaseDelimiter;
    ost << metadata.Comment().size() << kDatabaseDelimiter;
    ost << metadata.Genre().size() << kDatabaseDelimiter;

    sprintf(num, "%d", metadata.Year());
    ost << strlen(num) << kDatabaseDelimiter;
    sprintf(num, "%d", metadata.Track());
    ost << strlen(num) << kDatabaseDelimiter;
    sprintf(num, "%d", metadata.Time());
    ost << strlen(num) << kDatabaseDelimiter;
    sprintf(num, "%d", metadata.Size());
    ost << strlen(num) << kDatabaseDelimiter;

    ost << metadata.Artist();
    ost << metadata.Album();
    ost << metadata.Title();
    ost << metadata.Comment();
    ost << metadata.Genre();
    ost << metadata.Year();
    ost << metadata.Track();
    ost << metadata.Time();
    ost << metadata.Size();
    ost << '\0';

#ifdef WIN32
    m_database->Insert(url, (char *)ost.str().c_str());
#else
    m_database->Insert(url, (char *)ost.str());
#endif
}

MetaData *MusicCatalog::ReadMetaDataFromDatabase(const char *url)
{
    if (!m_database->Working())
        return NULL;

    char *dbasedata = m_database->Value(url);

    if (!dbasedata)
        return NULL;

    MetaData *metadata = new MetaData();
    char *value = dbasedata + 2;

    uint32 numFields, offset = 0;

    sscanf(value, "%d%n", &numFields, &offset);
    uint32* fieldLength =  new uint32[numFields];

    for(uint32 i = 0; i < numFields; i++)
    {
        uint32 temp;

        sscanf(value + offset, " %d %n", &fieldLength[i], &temp);

        if (i == numFields - 1) {
            char intholder[10];
            sprintf(intholder, "%d", fieldLength[i]);
            offset += strlen(intholder) + 1;
        }
        else
            offset += temp;
    }

    string data = value;
    data.erase(0, offset);

    uint32 count = 0;

    for(uint32 j = 0; j < numFields; j++)
    {
        if (fieldLength[j] == 0) 
            continue;

        switch(j)
        {
            case 0:
                metadata->SetArtist(data.substr(count, fieldLength[j]).c_str());
                break;
            case 1:
                metadata->SetAlbum(data.substr(count, fieldLength[j]).c_str());
                break;
            case 2:
                metadata->SetTitle(data.substr(count, fieldLength[j]).c_str());
                break;
            case 3:
                metadata->SetComment(data.substr(count, fieldLength[j]).c_str());
                break;
            case 4:
                metadata->SetGenre(data.substr(count, fieldLength[j]).c_str());
                break;
            case 5:
                metadata->SetYear(atoi(data.substr(count, fieldLength[j]).c_str()));
                break;
            case 6:
                metadata->SetTrack(atoi(data.substr(count, fieldLength[j]).c_str()));
                break;
            case 7:
                metadata->SetTime(atoi(data.substr(count, fieldLength[j]).c_str()));
                break;
            case 8:
                metadata->SetSize(atoi(data.substr(count, fieldLength[j]).c_str()));
                break;
            default:
                break;

        }

        count += fieldLength[j];
    }

    delete [] fieldLength;

    delete [] dbasedata;

    return metadata;
}

int32 MusicCatalog::AcceptEvent(Event *e)
{
    switch (e->Type()) {
        case INFO_MusicCatalogTrackRemoved: {
            if (m_inUpdateSong) {
                MusicCatalogTrackRemovedEvent *mctr = 
                                             (MusicCatalogTrackRemovedEvent *)e;
                m_oldItem = (PlaylistItem *)mctr->Item();
                m_oldArtist = (ArtistList *)mctr->Artist();
                m_oldAlbum = (AlbumList *)mctr->Album();
     
                delete e;
            }
            else 
                m_context->target->AcceptEvent(e);
            break; }
        case INFO_MusicCatalogTrackAdded: {
            if (m_inUpdateSong) {
                MusicCatalogTrackAddedEvent *mcta =
                                             (MusicCatalogTrackAddedEvent *)e;
                m_newItem = (PlaylistItem *)mcta->Item();
                m_newArtist = (ArtistList *)mcta->Artist();
                m_newAlbum = (AlbumList *)mcta->Album();

                delete e;
            }
            else 
                m_context->target->AcceptEvent(e);
            break; }
        case INFO_PlaylistItemUpdated: {
            if (m_acceptItemChanged) {
                PlaylistItemUpdatedEvent *piu = (PlaylistItemUpdatedEvent *)e;
                WriteMetaDataToDatabase(piu->Item()->URL().c_str(), 
                                        (MetaData)piu->Item()->GetMetaData());

                delete piu->Item();
                m_itemWaitCount--;
            }
            break; }
        case INFO_SearchMusicDone: {
            m_database->Sync();
            string info = "Pruning the Music Catalog Database...";
            m_context->target->AcceptEvent(new BrowserMessageEvent(info.c_str()));
            PruneDatabase();
            info = "Regenerating the Music Catalog Database...";
            m_context->target->AcceptEvent(new BrowserMessageEvent(info.c_str()));
            RePopulateFromDatabase();
            info = "Sorting the Music Catalog Database...";
            m_context->target->AcceptEvent(new BrowserMessageEvent(info.c_str()));
            Sort();
            m_context->target->AcceptEvent(new Event(INFO_SearchMusicDone));
            delete e;
            break;
        } 
    }
    return 0; 
}

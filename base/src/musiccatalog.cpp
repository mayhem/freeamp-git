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

using namespace std;

#include "config.h"
#include "musicbrowser.h"
#include "player.h"
#include "utility.h"
#include "debug.h"

#define DBASEDELIM ":"

MusicCatalog::MusicCatalog(FAContext *context)
{
    m_context = context;
    m_artistList = new vector<ArtistList *>;
    m_unsorted = new vector<PlaylistItem *>;
    m_playlists = new vector<string>;
}

MusicCatalog::~MusicCatalog()
{
    delete m_artistList;
    delete m_unsorted;
    delete m_playlists;
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

    // sort the rest o the junk
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
    Database *dbase = m_context->browser->GetDatabase();
    assert(dbase);
    assert(url);

    if (!dbase->Working())
        return kError_DatabaseNotWorking;

    char *data = dbase->Value(url);

    if (!data)
        return kError_ItemNotFound;

    delete [] data;
    dbase->Remove(url);

    vector<string>::iterator i = m_playlists->begin();
    for (; i != m_playlists->end(); i++)
         if ((*i) == url) {
             string tempstr = url;
             if (tempstr.find("currentlist.m3u") >= tempstr.length())
                 m_playlists->erase(i);
             return kError_NoErr;
         }   

    return kError_ItemNotFound;
}


Error MusicCatalog::AddPlaylist(const char *url)
{
    Database *dbase = m_context->browser->GetDatabase();
    assert(dbase);
    assert(url);

    if (!dbase->Working())
        return kError_DatabaseNotWorking;
    
    char *data = dbase->Value(url);
    if (!data) 
        dbase->Insert(url, "P");
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
        return kError_NoErr;
    }

    return kError_DuplicateItem;
}

Error MusicCatalog::RemoveSong(const char *url)
{
    Database *dbase = m_context->browser->GetDatabase();

    assert(dbase);
    assert(url);

    if (!dbase->Working())
        return kError_DatabaseNotWorking;

    MetaData *meta = m_context->browser->ReadMetaDataFromDatabase(url);
    if (!meta)
        return kError_ItemNotFound;

    dbase->Remove(url);

    if (meta->Artist() == " ") {
        vector<PlaylistItem *>::iterator i = m_unsorted->begin();
        for (; i != m_unsorted->end(); i++)
            if (url == (*i)->URL())
            {
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
    Database *dbase = m_context->browser->GetDatabase();
    assert(dbase);
    assert(url);

    if (!dbase->Working())
        return kError_DatabaseNotWorking;

    PlaylistItem *newtrack;

    MetaData *meta = m_context->browser->ReadMetaDataFromDatabase(url);

    if (!meta) {
        PlaylistItem *newtrack = new PlaylistItem(url);
        m_context->plm->RetrieveMetaData(newtrack);

        while (newtrack->GetState() != kPlaylistItemState_Normal)
            usleep(5);

        MetaData tempdata = (MetaData)(newtrack->GetMetaData());
        m_context->browser->WriteMetaDataToDatabase(url, tempdata);
        meta = m_context->browser->ReadMetaDataFromDatabase(url);
    }
    else
        newtrack = new PlaylistItem(url, meta);

    if (meta->Artist() == " ") {
        m_unsorted->push_back(newtrack);
    }
    else {
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
                     }
                }
                if (!found_album) {
                    AlbumList *newalbum = new AlbumList;
                    newalbum->name = meta->Album();
                    newalbum->m_trackList->push_back(newtrack);
                    alList->push_back(newalbum);
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
        }
    }
    return kError_NoErr;
}

Error MusicCatalog::Remove(const char *url)
{
    Database *dbase = m_context->browser->GetDatabase();
    assert(dbase);
    assert(url);

    if (!dbase->Working())
        return kError_DatabaseNotWorking;

    char *data = dbase->Value(url);
    if (!data)
        return kError_ItemNotFound;

    if (!strncmp("P", data, 1))
        return RemovePlaylist(url);
    else if (!strncmp("M", data, 1))
        return RemoveSong(url);

    delete [] data;

    return kError_YouScrewedUp;
}

Error MusicCatalog::Add(const char *url)
{
    Database *dbase = m_context->browser->GetDatabase();
    assert(dbase);
    assert(url);

    if (!dbase->Working())
        return kError_DatabaseNotWorking;

    char *data = dbase->Value(url);
    if (!data)
        return kError_ItemNotFound;

    if (!strncmp("P", data, 1))
        return AddPlaylist(url);
    else if (!strncmp("M", data, 1)) 
        return AddSong(url);

    delete [] data;
    return kError_YouScrewedUp;
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
    Database *dbase = m_context->browser->GetDatabase();
    assert(dbase);

    if (!dbase->Working())
        return kError_DatabaseNotWorking;

    ClearCatalog();
 
    char *key = dbase->NextKey(NULL);
    Error err = kError_NoErr;

    while (key) {
        err = Add(key);

        if (IsError(err)) {
             m_context->target->AcceptEvent(new ErrorMessageEvent("There was an internal error during generation of the Music Catalog"));
             return kError_YouScrewedUp;
        }
            
        key = dbase->NextKey(key);
    }
    return kError_NoErr;
}

MusicBrowser::MusicBrowser(FAContext *context, char *path)
{
    m_database = NULL;
    m_context = context;
    m_plm = context->plm;
    m_catalog = new MusicCatalog(context);
    m_mutex = new Mutex();   
 
    if (path)
        SetDatabase(path);
}

MusicBrowser::~MusicBrowser()
{
    if (m_database)
        delete m_database;
    delete m_catalog;
    delete m_mutex;
}

void MusicBrowser::SetDatabase(const char *path)
{
    if (m_database)
        delete m_database;

    m_database = new Database(path);

    if (!m_database->Working()) {
        delete m_database;
        m_database = NULL;
    }

    PruneDatabase();

    if (m_database)
        m_catalog->RePopulateFromDatabase();
}

void MusicBrowser::PruneDatabase(void)
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
    MusicBrowser *mb;
    vector<string> pathList;
    Thread *thread;
} MusicSearchThreadStruct;

void MusicBrowser::SearchMusic(vector<string> &pathList)
{
    if (!m_database->Working())
        return;

    Thread *thread = Thread::CreateThread();

    if (thread) {
        MusicSearchThreadStruct *mst = new MusicSearchThreadStruct;

        mst->mb = this;
        mst->pathList = pathList;
        mst->thread = thread;

        thread->Create(musicsearch_thread_function, mst);
    }
}

void MusicBrowser::StopSearchMusic(void)
{
    m_exit = true;
}

void MusicBrowser::musicsearch_thread_function(void *arg)
{
    MusicSearchThreadStruct *mst = (MusicSearchThreadStruct *)arg;

    mst->mb->m_mutex->Acquire();

    mst->mb->m_exit = false;
    mst->mb->DoSearchPaths(mst->pathList);

    mst->mb->AcceptEvent(new Event(INFO_SearchMusicDone));

    mst->mb->m_mutex->Release();

    delete mst->thread;
    delete mst;
}

void MusicBrowser::DoSearchPaths(vector<string> &pathList)
{
    vector<string>::iterator i;
    
    for(i = pathList.begin(); i != pathList.end(); i++)
        DoSearchMusic((char *)(*i).c_str());
}

void MusicBrowser::DoSearchMusic(char *path)
{
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
                if (!strncmp("M3U", fileExt, 3) && 
                    strcmp("currentlist.m3u", find.cFileName))
                {
                    string file = path;
                    file.append(DIR_MARKER_STR);
                    file.append(find.cFileName);

                    uint32 urlLength = strlen(file.c_str()) + 10;
                    char *url = new char[urlLength];

                    if (IsntError(FilePathToURL(file.c_str(), url, &urlLength)))
                        m_database->Insert(file.c_str(), "P");

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
                    m_plm->RetrieveMetaData(plist);

                    while (plist->GetState() != kPlaylistItemState_Normal)
                       usleep(5);

                    WriteMetaDataToDatabase(tempurl, plist->GetMetaData());
 
                    delete [] tempurl;
                    delete plist;
                }
                delete fileExt;
            }
        }
        while (FindNextFile(handle, &find) && !m_exit);
        FindClose(handle);
    }
}

void MusicBrowser::WriteMetaDataToDatabase(const char *url, 
                                           const MetaData information)
{
    if (!m_database->Working())
        return;

    string data;
    char tempstr[11];

    data = "M";
    if (information.Artist() != "")
        data += information.Artist();
    else
        data += " ";
    data += DBASEDELIM;
    if (information.Album() != "")
        data += information.Album();
    else
        data += " ";
    data += DBASEDELIM;
    if (information.Title() != "")
        data += information.Title();
    else
        data += " ";
    data += DBASEDELIM;
    if (information.Comment() != "")
        data += information.Comment();
    else
        data += " ";
    data += DBASEDELIM;
    if (information.Genre() != "")
        data += information.Genre();
    else
        data += " ";
    data += DBASEDELIM;

    sprintf(tempstr, "%u", information.Year());
    data += tempstr;
    data += DBASEDELIM;
    sprintf(tempstr, "%u", information.Track());
    data += tempstr;
    data += DBASEDELIM;
    sprintf(tempstr, "%u", information.Time());
    data += tempstr;
    data += DBASEDELIM;
    sprintf(tempstr, "%u", information.Size());
    data += tempstr;

    m_database->Insert(url, (char *)data.c_str());
}

MetaData *MusicBrowser::ReadMetaDataFromDatabase(const char *url)
{
    if (!m_database->Working())
        return NULL;

    char *data = m_database->Value(url);

    if (!data)
        return NULL;

    MetaData *new_metadata = new MetaData();
    char *temp = data + 1;
    char *info; 

    info = strtok(temp, DBASEDELIM);
    new_metadata->SetArtist(info);
    info = strtok(NULL, DBASEDELIM);
    new_metadata->SetAlbum(info);
    info = strtok(NULL, DBASEDELIM);
    new_metadata->SetTitle(info);
    info = strtok(NULL, DBASEDELIM);
    new_metadata->SetComment(info);
    info = strtok(NULL, DBASEDELIM);
    new_metadata->SetGenre(info);
    info = strtok(NULL, DBASEDELIM);
    new_metadata->SetYear(atoi(info));
    info = strtok(NULL, DBASEDELIM);
    new_metadata->SetTrack(atoi(info));
    info = strtok(NULL, DBASEDELIM);
    new_metadata->SetTime(atoi(info));
    info = strtok(NULL, DBASEDELIM);
    new_metadata->SetSize(atoi(info));

    delete [] data;

    return new_metadata;
}

int32 MusicBrowser::AcceptEvent(Event *e)
{
    switch (e->Type()) {
        case INFO_SearchMusicDone: {
            m_database->Sync();
            string info = "Pruning the Music Catalog Database...";
            m_context->target->AcceptEvent(new BrowserMessageEvent(info.c_str()));
            PruneDatabase();
            info = "Regenerating the Music Catalog Database...";
            m_context->target->AcceptEvent(new BrowserMessageEvent(info.c_str()));
            m_catalog->RePopulateFromDatabase();
            info = "Sorting the Music Catalog Database...";
            m_context->target->AcceptEvent(new BrowserMessageEvent(info.c_str()));
            m_catalog->Sort();
            m_context->target->AcceptEvent(new Event(INFO_SearchMusicDone));
            break;
        } 
    }
    delete e;

    return 0; 
}

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

void MusicCatalog::AddPlaylist(const char *path)
{
    Database *dbase = m_context->browser->GetDatabase();
    assert(dbase);
    assert(path);

    if (!dbase->Working())
        return;
    
    char *data = dbase->Value(path);
    if (!data) 
        dbase->Insert(path, "P");

    bool found = false;
    vector<string>::iterator i = m_playlists->begin();
    for (; i != m_playlists->end(); i++)
         if ((*i) == path)
             found = true;
    
    if (!found) {
        string tempstr = path;
        if (tempstr.find("currentlist.m3u") < tempstr.length()) 
            m_playlists->insert(m_playlists->begin(), path);
        else
            m_playlists->push_back(path);
    }
}

void MusicCatalog::AddSong(const char *path)
{
    Database *dbase = m_context->browser->GetDatabase();
    assert(dbase);
    assert(path);
    if (!dbase->Working())
        return;

    PlaylistItem *newtrack;
    MetaData *meta = m_context->browser->ReadMetaDataFromDatabase((char *)path);
    if (!meta) {
        PlaylistItem *newtrack = new PlaylistItem(path);
        m_context->plm->RetrieveMetaData(newtrack);
        while (newtrack->GetState() != kPlaylistItemState_Normal)
            usleep(5);
        MetaData tempdata = (MetaData)(newtrack->GetMetaData());
        m_context->browser->WriteMetaDataToDatabase((char *)path, tempdata);
        meta = m_context->browser->ReadMetaDataFromDatabase((char *)path);
    }
    else
        newtrack = new PlaylistItem(path, meta);

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
}

void MusicCatalog::AddOneFromDatabase(char *key)
{
    Database *dbase = m_context->browser->GetDatabase();
    assert(dbase);
    assert(key);

    if (!dbase->Working())
        return;

    char *data = dbase->Value(key);
    if (!data)
        return;

    if (!strncmp("P", data, 1)) {
        string tempstr = key;
        if (tempstr.find("currentlist.m3u") < tempstr.length())
            m_playlists->insert(m_playlists->begin(), key);
        else  
            m_playlists->push_back(key);
    }
    else if (!strncmp("M", data, 1)) 
        AddSong(key);
}

void MusicCatalog::PopulateFromDatabase()
{
    Database *dbase = m_context->browser->GetDatabase();
    assert(dbase);

    if (!dbase->Working())
        return;

    delete m_artistList;
    delete m_unsorted;
    delete m_playlists;

    m_artistList = new vector<ArtistList *>;
    m_unsorted = new vector<PlaylistItem *>;
    m_playlists = new vector<string>;
 
    char *key = dbase->NextKey(NULL);
    while (key) {
        AddOneFromDatabase(key);
        key = dbase->NextKey(key);
    }
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

    if (m_database)
        m_catalog->PopulateFromDatabase();
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
        m_catalog->PopulateFromDatabase();
}

void MusicBrowser::PruneDatabase(void)
{
    char *key = m_database->NextKey(NULL);
    struct stat st;

    while (key) {
        if (-1 == stat(key, &st))
            m_database->Remove(key);
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
                if (!strncmp("M3U", fileExt, 3))
                {
                    string file = path;
                    file.append(DIR_MARKER_STR);
                    file.append(find.cFileName);
                    m_database->Insert(file.c_str(), "P");

                }
                else if (m_context->player->IsSupportedExtension(fileExt))
                {
                    string file = path;
                    file.append(DIR_MARKER_STR);
                    file.append(find.cFileName);

                    PlaylistItem *plist = new PlaylistItem(file.c_str());
                    m_plm->RetrieveMetaData(plist);

                    while (plist->GetState() != kPlaylistItemState_Normal)
                       usleep(5);

                    WriteMetaDataToDatabase((char *)file.c_str(), (MetaData)(plist->GetMetaData()));
 
                    delete plist;
                }
                delete fileExt;
            }
        }
        while (FindNextFile(handle, &find) && !m_exit);
        FindClose(handle);
    }
}

void MusicBrowser::WriteMetaDataToDatabase(char *path, MetaData information)
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

    m_database->Insert(path, (char *)data.c_str());
}

MetaData *MusicBrowser::ReadMetaDataFromDatabase(char *path)
{
    if (!m_database->Working())
        return NULL;

    char *data = m_database->Value(path);

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

    delete data;
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
            m_catalog->PopulateFromDatabase();
            m_context->target->AcceptEvent(new Event(INFO_SearchMusicDone));
            break;
        } 
    }
    delete e;

    return 0; 
}

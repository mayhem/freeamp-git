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

// system includes
#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <shlobj.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>

// project includes
#include "config.h"
#include "utility.h"
#include "resource.h"
#include "Win32MusicBrowser.h"


BOOL CALLBACK EditTrackInfoDlgProc(HWND hwnd,
                                   UINT msg,
                                   WPARAM wParam,
                                   LPARAM lParam)
{
    MusicBrowserUI* ui = (MusicBrowserUI*)GetWindowLong(hwnd, GWL_USERDATA);

    switch (msg)
    {
        case WM_INITDIALOG:
        {
            ui = (MusicBrowserUI*)lParam;
            assert(ui != NULL);
            SetWindowLong(hwnd, GWL_USERDATA, (LONG)ui);
            break;
        }            
    }

    return ui->EditTrackInfoDlgProc(hwnd, msg, wParam, lParam);
}        

BOOL MusicBrowserUI::EditTrackInfoDlgProc(HWND hwnd, 
                                          UINT msg, 
                                          WPARAM wParam, 
                                          LPARAM lParam )
{
    BOOL result = FALSE;

    switch (msg)
    {
        case WM_INITDIALOG:
        {
            vector<string> artists;
            vector<string> albums;
            set<string> genres;
            char number[64];
            
            CreateEditInfoLists(artists, albums, genres);

            HWND hwndArtist = GetDlgItem(hwnd, IDC_ARTIST);
            HWND hwndAlbum = GetDlgItem(hwnd, IDC_ALBUM);
            HWND hwndGenre = GetDlgItem(hwnd, IDC_GENRE);
            HWND hwndTitle = GetDlgItem(hwnd, IDC_TITLE);
            HWND hwndTrack = GetDlgItem(hwnd, IDC_TRACK);
            HWND hwndSpinTrack = GetDlgItem(hwnd, IDC_SPINTRACK);
            HWND hwndYear = GetDlgItem(hwnd, IDC_YEAR);
            HWND hwndSpinYear = GetDlgItem(hwnd, IDC_SPINYEAR);
            HWND hwndComment = GetDlgItem(hwnd, IDC_COMMENT);
            HWND hwndTitleText =  FindWindowEx(hwnd, NULL, NULL, "Title:"); //GetDlgItem(hwnd, IDC_TITLETEXT);
            HWND hwndTrackText =  FindWindowEx(hwnd, NULL, NULL, "Track #:"); //GetDlgItem(hwnd, IDC_TRACKTEXT);
            
            SYSTEMTIME sysTime;

            GetSystemTime(&sysTime);

            SendMessage(hwndSpinTrack, UDM_SETRANGE, 0, MAKELPARAM(1000, 0));
            SendMessage(hwndSpinYear, UDM_SETRANGE, 0, MAKELPARAM(sysTime.wYear, 0));

            vector<string>::iterator vi;
            set<string>::iterator si;

            // track name
            Edit_SetText(hwndTitle, m_editTrackMetaData.Title().c_str());

            // track number
            if(m_editTrackMetaData.Track() == -1)
            {
                Edit_SetText(hwndTrack, "<Multiple>");
                EnableWindow(hwndTrack, FALSE);
                EnableWindow(hwndTrackText, FALSE);

                EnableWindow(hwndTitle, FALSE);
                EnableWindow(hwndTitleText, FALSE);
            }
            else
            {
                sprintf(number, "%d", m_editTrackMetaData.Track());
                Edit_SetText(hwndTrack, number);
            }

            // track year
            if(m_editTrackMetaData.Year() == -1)
            {
                Edit_SetText(hwndYear, "<Multiple>");
            }
            else
            {
                sprintf(number, "%d", m_editTrackMetaData.Year());
                Edit_SetText(hwndYear, number);
            }

            // track comment
            Edit_SetText(hwndComment, m_editTrackMetaData.Comment().c_str());


            // add artists
            for(vi = artists.begin(); vi != artists.end(); vi++)
            {
                uint32 length = (*vi).size();

                if(length)
                {
                    bool notJustWhiteSpace = false;

                    for(uint32 i = 0; i < length; i++)
                    {
                        if(!isspace((*vi)[i]))
                        {
                            notJustWhiteSpace = true;
                            break;
                        }
                    }

                    if(notJustWhiteSpace)
                        ComboBox_AddString(hwndArtist, (*vi).c_str());
                }
            }

            ComboBox_SetText(hwndArtist, m_editTrackMetaData.Artist().c_str());

            // add albums
            for(vi = albums.begin(); vi != albums.end(); vi++)
            {
                uint32 length = (*vi).size();

                if(length)
                {
                    bool notJustWhiteSpace = false;

                    for(uint32 i = 0; i < length; i++)
                    {
                        if(!isspace((*vi)[i]))
                        {
                            notJustWhiteSpace = true;
                            break;
                        }
                    }

                    if(notJustWhiteSpace)
                        ComboBox_AddString(hwndAlbum, (*vi).c_str());
                }
            }

            ComboBox_SetText(hwndAlbum, m_editTrackMetaData.Album().c_str());

            // add genres
            for(si = genres.begin(); si != genres.end(); si++)
            {
                uint32 length = (*si).size();

                if(length)
                {
                    bool notJustWhiteSpace = false;

                    for(uint32 i = 0; i < length; i++)
                    {
                        if(!isspace((*si)[i]))
                        {
                            notJustWhiteSpace = true;
                            break;
                        }
                    }

                    if(notJustWhiteSpace)
                        ComboBox_AddString(hwndGenre, (*si).c_str());
                }
            }

            ComboBox_SetText(hwndGenre, m_editTrackMetaData.Genre().c_str());
  
            break;
        }      

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDCANCEL:
                    EndDialog(hwnd, FALSE);
                    break;

                case IDOK:
                {
                    HWND hwndArtist = GetDlgItem(hwnd, IDC_ARTIST);
                    HWND hwndAlbum = GetDlgItem(hwnd, IDC_ALBUM);
                    HWND hwndGenre = GetDlgItem(hwnd, IDC_GENRE);
                    HWND hwndTitle = GetDlgItem(hwnd, IDC_TITLE);
                    HWND hwndTrack = GetDlgItem(hwnd, IDC_TRACK);
                    HWND hwndYear = GetDlgItem(hwnd, IDC_YEAR);
                    HWND hwndComment = GetDlgItem(hwnd, IDC_COMMENT);
                    uint32 length;
                    char* data;
                    char number[64];

                    length = ComboBox_GetTextLength(hwndArtist) + 1;
                    data = new char[length];
                    ComboBox_GetText(hwndArtist, data, length);
                    m_editTrackMetaData.SetArtist(data);
                    delete [] data;

                    length = ComboBox_GetTextLength(hwndAlbum) + 1;
                    data = new char[length];
                    ComboBox_GetText(hwndAlbum, data, length);
                    m_editTrackMetaData.SetAlbum(data);
                    delete [] data;

                    length = ComboBox_GetTextLength(hwndGenre) + 1;
                    data = new char[length];
                    ComboBox_GetText(hwndGenre, data, length);
                    m_editTrackMetaData.SetGenre(data);
                    delete [] data;

                    length = Edit_GetTextLength(hwndTitle) + 1;
                    data = new char[length];
                    Edit_GetText(hwndTitle, data, length);
                    m_editTrackMetaData.SetTitle(data);
                    delete [] data;

                    length = Edit_GetTextLength(hwndComment) + 1;
                    data = new char[length];
                    Edit_GetText(hwndComment, data, length);
                    m_editTrackMetaData.SetComment(data);
                    delete [] data;

                    Edit_GetText(hwndTrack, number, sizeof(number));

                    if(!strcmp(number, "Unknown"))
                        strcpy(number, "0");
                    else if(!strcmp(number, "<Multiple>"))
                        strcpy(number, "-1");

                    m_editTrackMetaData.SetTrack(atoi(number));

                    Edit_GetText(hwndYear, number, sizeof(number));

                    if(!strcmp(number, "Unknown"))
                        strcpy(number, "0");
                    else if(!strcmp(number, "<Multiple>"))
                        strcpy(number, "-1");

                    m_editTrackMetaData.SetYear(atoi(number));
                    
                    EndDialog(hwnd, TRUE);
                    break;
                }

                case IDC_TRACK:
                case IDC_YEAR:
                {
                    if(HIWORD(wParam) == EN_CHANGE)
                    {
                        char track[6];

                        Edit_GetText( (HWND)lParam, 
                                      track,
                                      sizeof(track));

                        if(!strcmp(track, "0"))
                        {
                            Edit_SetText((HWND)lParam, "Unknown");
                        }
                    }
                    break;
                }
            }
  
            break;
        }
    }

    return result;
}

void MusicBrowserUI::CreateEditInfoLists(vector<string>& artists,
                                         vector<string>& albums,
                                         set<string>& genres)
{
    vector<ArtistList*>*            artistList;
    vector<ArtistList*>::iterator   artist;    

    artistList = (vector<ArtistList*>*)
            m_context->catalog->GetMusicList();

    for(artist = artistList->begin(); 
        artist != artistList->end(); 
        artist++)
    {
        vector<AlbumList*>::iterator album;

        artists.push_back((*artist)->name);

        for(album = (*artist)->m_albumList->begin();
            album != (*artist)->m_albumList->end();
            album++)
        {
            vector<PlaylistItem*>::iterator track;

            albums.push_back((*album)->name);

            for(track = (*album)->m_trackList->begin();
                track != (*album)->m_trackList->end();
                track++)
            {
                MetaData metadata = (*track)->GetMetaData();

                genres.insert(metadata.Genre());
            }
        }
    }
}

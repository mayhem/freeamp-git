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

#if !defined(WIN32)
#include <strstream>
typedef ostrstream ostringstream;
#else
#include <sstream>
#endif

using namespace std;

#include "config.h"
#include "missingfile.h"
#include "utility.h"


MissingFile::MissingFile(FAContext *context)
{
    m_context = context;
}

MissingFile::~MissingFile()
{
}

Error MissingFile::FindMissingFile(const string &url, 
                                   const string &searchPath, 
                                   string &newUrl)
{
    string::size_type pos;
    string  file;

    pos = url.rfind(DIR_MARKER, url.length() - 1);
    if (pos != string::npos)
        file = url.substr(pos + 1, url.size());
    else
        return kError_FileNotFound;

    return RecurseFindMissingFile(file, searchPath, newUrl);
}

Error MissingFile::RecurseFindMissingFile(const string &file, 
                                          const string &searchPath, 
                                          string &newUrl)
{
    WIN32_FIND_DATA find;
    HANDLE handle;
    string search = searchPath;
    Error ret;

#ifndef WIN32
    if (!strcmp(search.c_str(), "/dev") || !strcmp(search.c_str(), "/proc"))
        return kError_FileNotFound;
#endif
 
    if (search[search.size() - 1] != DIR_MARKER)
       search.append(DIR_MARKER_STR);

    search.append("*");
#ifdef WIN32
    search.append(".*");
#endif

    handle = FindFirstFile((char *)search.c_str(), &find);
    if (handle != INVALID_HANDLE_VALUE)
    {
        do
        {
#ifndef WIN32
            if (find.dwFileAttributes & FILE_ATTRIBUTE_SYMLINK)
                continue;
#endif
            if (find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (!(!strcmp("." , find.cFileName) || !strcmp("..", find.cFileName)))
                {
                    string newDir = searchPath; 
                    if (searchPath[searchPath.length() - 1] != DIR_MARKER)
                        newDir.append(DIR_MARKER_STR);
                    newDir.append(find.cFileName);

                    ret = RecurseFindMissingFile(file, newDir, newUrl);
                    if (ret == kError_NoErr)
                    {
                        FindClose(handle);
                        return ret;
                    }
                }
            }
            else 
            {
#ifdef WIN32
                string dirtest = searchPath;
                if (searchPath[searchPath.length() - 1] != DIR_MARKER)
                    dirtest.append(DIR_MARKER_STR);
                dirtest.append(find.cFileName);

                struct stat stdir;

                stat(dirtest.c_str(), &stdir);
                if (stdir.st_mode & _S_IFDIR) 
                {
                    ret = RecurseFindMissingFile(file, dirtest, newUrl);
                    if (ret == kError_NoErr)
                    {
                        FindClose(handle);
                        return ret;
                    }
                    continue;
                }
#endif           
#ifdef WIN32
                if (strcasecmp(find.cFileName, file.c_str()) == 0)
#else
                if (strcmp(find.cFileName, file.c_str()) == 0)
#endif
                {
                    newUrl = searchPath + string(DIR_MARKER_STR) + string(file);
                    FindClose(handle);

                    return kError_NoErr;
                }
            }
        }
        while (FindNextFile(handle, &find));
        FindClose(handle);
    }

    return kError_FileNotFound;
}

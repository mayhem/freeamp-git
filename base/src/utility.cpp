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

#include <assert.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdio.h>

#ifdef WIN32
#include <direct.h>
#define MKDIR(z) mkdir(z)
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#define MKDIR(z) mkdir(z, 0755)
#endif

#include "config.h"
#include "utility.h"

void CreateDirectoryPath(const char* path)
{
    char* temp = new char[strlen(path) + 1];

    // make a copy we can destroy
    strcpy(temp, path);

    char* cp = temp;
    char* end;

    do
    {
        end = strchr(cp, DIR_MARKER);

        if(end)
            *end = 0x00;

        MKDIR(temp);

        if(end)
        {
            *end = DIR_MARKER;
            cp = end + 1;
        }

    }while(end);

    delete [] temp;
}

static bool IsRelative(const char* path)
{
    bool result = false;

    assert(path);

    if(path)
    {
        result = (  path[0] != DIR_MARKER );

#ifdef WIN32
       if(result && strlen(path) > 2)
       {
            result = !(path[1] == ':' && path[2] == '\\');
       }
#endif
    }

    return result;
}

void ResolvePath(char** path)
{
    assert(path);
    assert(*path);

    if(IsRelative(*path))
    {
        // need to get cwd and resolve this path in
        // relation to it.
        char* cwd = new char[_MAX_PATH];

        getcwd(cwd, _MAX_PATH);

        char* fullPath = new char[strlen(cwd) + strlen(*path) + 2];
        
        strcpy(fullPath, cwd);
        strcat(fullPath, "\\");
        strcat(fullPath, *path);

        delete [] *path;

        *path = fullPath;

        delete [] cwd;
    }

    char* cp = *path;

    // remove multiple separators between elements
    // and make sure end does not contain separator
    // unless it is the root directory and remove
    // relative indicators in the *path

    
    bool separator = false;

    char dotslash[3] = ".\\";
    char dotdotslash[4] = "..\\";

    dotslash[1] = DIR_MARKER;
    dotdotslash[2] = DIR_MARKER;

    while(*cp)
    {
        if(cp[0] == DIR_MARKER)
        {
            if(separator)
            {
                //memcpy(cp - 1, cp, strlen(cp) + 1);
                strcpy(cp - 1, cp);
                continue;
            }

            separator = true;
        }
        else
        {
            separator = false;

            if(cp[0] == '.')
            {
               
                if(!strncmp(cp, dotslash, 2))
                {
                    strcpy(cp, cp + 2);
                    continue;
                }
                else if(!strncmp(cp, dotdotslash, 3) && cp != *path)
                {
                    char* lastDir;
                    bool found = false;

                    lastDir = cp - 2;

                    while(lastDir > *path)
                    {
                        if(*lastDir == DIR_MARKER)
                        {
                            lastDir++;
                            found = true;
                            break;
                        }

                        lastDir--;
                    }

                    if(found || lastDir == *path)
                    {
                        strcpy(lastDir, cp + 3);

                        cp = lastDir;
                    }

                    continue;
                }
            }
        }

        cp++;
    }

    // check the end for stragglers
    if(*(cp - 1) == '.')
    {

        if( cp - 1 != *path && 
            *(cp - 2) == '.' && 
            cp - 2 != *path) 
        {
            *(cp - 2) = 0x00;
            cp = cp - 2;
        }
        else if(cp - 1 != *path && 
                *(cp - 2) != '.') 
        {
            *(cp - 1) = 0x00;
            cp = cp - 1;
        }
    }

#ifdef WIN32
    if(*(cp - 1) == DIR_MARKER && 
        cp - 1 != *path &&
        *(cp - 2) != ':') 
    {
        *(cp - 1) = 0x00;

    }

#else
    if(*(cp - 1) == DIR_MARKER && 
        cp - 1 != *path) 
    {
        *(cp - 1) = 0x00;
    }
#endif
    
}

void RFC822GMTTimeString(struct tm* time, char buf[32])
{
    const char* day[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    const char* month[] = {"Jan", "Feb",  "Mar", "Apr", "May", "Jun",  
                           "Jul", "Aug", "Sep", "Oct",  "Nov", "Dec"};

    // Example: Sun, 06 Nov 1994 08:49:37 GMT
    const char* k822Format = "%s, %2d %s %4d %2d:%2d:%2d GMT";

    sprintf(buf, k822Format, day[time->tm_wday],
                             time->tm_mday,
                             month[time->tm_mon],
                             time->tm_hour,
                             time->tm_min,
                             time->tm_sec);
}

char *strdup_new(const char *str)
{
    char *n;

    n = new char[strlen(str) + 1];
    strcpy(n, str);
 
    return n;
}

#ifdef WIN32

char *FreeampDir(Preferences *pref)
{
    char path[_MAX_PATH] = {0x00};
    uint32 len = sizeof(path);
    char *s;

    pref->GetInstallDirectory(path, &len);

    s = new char[strlen(path) + 1];
    strcpy(s, path);

    return s;
}

#elif defined(__BEOS__)

#include <be/storage/FindDirectory.h>
#include <be/storage/Path.h>

char *FreeampDir(Preferences *pref)
{
    BPath prefPath;
    assert(find_directory(B_USER_SETTINGS_DIRECTORY, &prefPath) == B_NO_ERROR);
    prefPath.Append( "freeamp.org" );

    char *path = prefPath.Path();
    char *s = new char[strlen(path) + 1];
    strcpy(s, path);

    return s;
}

#else

#include <stdlib.h>

char *FreeampDir(Preferences *prefs)
{
    char *homeDir = getenv("HOME");
    const char *fadir = "/.freeamp";
    char *s;

    if (!homeDir) {
        s = new char[2];
        strcpy(s, "/");
        return s;
    }

    s = new char[strlen(homeDir) + strlen(fadir) + 1];
    strcpy(s, homeDir);
    strcat(s, fadir);

    return s;
}
#endif

const char* protocol = "file://";

Error FilePathToURL(const char* path, char* url, uint32* length)
{
    Error result = kError_InvalidParam;

    assert(path);
    assert(url);
    assert(length);

    if(path && url && length)
    {
        result = kError_BufferTooSmall;
        int32 extra = 0;

#ifdef WIN32
         if(path[0] == '\\')
         {
             extra = 2;
         }
#endif

        if(*length >= strlen(path) + strlen(protocol) + 1 + extra)
        {

            strcpy(url, protocol);

#ifdef WIN32
            if(extra)
            {
                int32 drive = _getdrive();
           
                url[7] = drive + 'A' - 1;
                url[8] = '|';
                url[9] = 0x00;
            }
#endif

            strcat(url, path);

#ifdef WIN32
            if(strlen(path) > 1 && path[1] == ':')
            {
                url[8] = '|';
            }

            for(int32 index = strlen(url) - 1; index >=0; index--)
            {
                if(url[index] == '\\')
                    url[index] = '/';
            }
#endif  
            result = kError_NoErr;
        }

        *length = strlen(path) + strlen(protocol) + 1 + extra;
    }

    return result;
}

Error URLToFilePath(const char* url, char* path, uint32* length)
{
    Error result = kError_InvalidParam;

    assert(path);
    assert(url);
    assert(length);

    if(path && url && length && !strncasecmp(url, protocol, strlen(protocol)))
    {
        result = kError_BufferTooSmall;

        if(*length >= strlen(url) - strlen(protocol) + 1)
        {
            strcpy(path, url + strlen(protocol));
#ifdef WIN32
            if(strlen(path) > 1 && path[1] == '|')
            {
                path[1] = ':';
            }

            for(int32 index = strlen(path) - 1; index >=0; index--)
            {
                if(path[index] == '/')
                    path[index] = '\\';
            }
#endif 
            result = kError_NoErr; 
        }

        *length = strlen(url) - strlen(protocol) + 1;
    }

    return result;
}

void ToUpper(char *s)
{
    char *p;
    
    for(p = s; *p != '\0'; p++)
       *p = toupper(*p);
}       

void ToLower(char *s)
{
    char *p;
    
    for(p = s; *p != '\0'; p++)
       *p = tolower(*p);
}      

#ifndef WIN32
void LaunchBrowser(char* url)
{
   char         url2[URL_SIZE];
   char         lockfile[255];
   int          lockfile_fd;
   struct stat  sb;
   char        *home, *browser;

   browser = "netscape";
   sprintf(url2, "openURL(%s)", url);

   if (strcmp(browser, "netscape") == 0)
   {
      home=getenv("HOME");
      if(!home)
         home="/";

      sprintf(lockfile,"%.200s/.netscape/lock",home);
      if(fork()>0)
         _exit(0);

      if((lockfile_fd=lstat(lockfile, &sb))!=-1)
      {
         execlp("netscape", "netscape", "-remote", url2, NULL);
      } else
      {
         execlp("netscape", "netscape", url, NULL);
      }
      perror("Could not launch netscape");
   }
   else
   {
      char command[256];
      sprintf(command, "%s \"%s\"", browser, url);

      system(command);
   }
}
#endif
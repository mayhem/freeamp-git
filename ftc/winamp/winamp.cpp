/*____________________________________________________________________________
        
   FreeAmp - The Free MP3 Player

   Copyright (C) 2000 EMusic.com

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

#include <string>
using namespace std;

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>

#include "config.h"
#include "facontext.h"
#include "errors.h"
#include "utility.h"

#include "winamp.h"

extern "C"
{
   ThemeFormat *Initialize(FAContext* context)
   {
      return new Winamp(context);
   }
}


Winamp::Winamp(FAContext *context):ThemeFormat(context)
{
    m_context = context;
}

Winamp::~Winamp()
{
}
 
bool Winamp::IsSupportedFormat(string &oDir)
{
    string testpath;
    struct stat st;

    testpath = oDir + string(DIR_MARKER_STR) + string("cbuttons.bmp");

    if (-1 == stat(testpath.c_str(), &st)) 
        return false;

    testpath = oDir + string(DIR_MARKER_STR) + string("main.bmp");

    if (-1 == stat(testpath.c_str(), &st)) 
        return false;

    return true;
}

Error Winamp::ConvertToNative(string &oDir)
{
    string sourcePath;
    string destPath;

    char *dir = new char[_MAX_PATH];
    uint32 len = _MAX_PATH;

    m_context->prefs->GetInstallDirectory(dir, &len);
    sourcePath = string(dir);
#if defined(unix)
    sourcePath += string(BRANDING_SHARE_PATH);
#endif
    sourcePath += string(DIR_MARKER_STR) + string("winamp_theme.xml");

    destPath = oDir + string(DIR_MARKER_STR) + string("theme.xml");

    Error eErr;

    eErr = CopyFile(sourcePath.c_str(), destPath.c_str(), false) ?
           kError_NoErr : kError_CopyFailed;

    return eErr;
}

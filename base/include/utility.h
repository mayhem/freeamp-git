/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998-1999 EMusic.com

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

#ifndef INCLUDED_UTILITY_H
#define INCLUDED_UTILITY_H

// The debugger can't handle symbols more than 255 characters long.
// STL often creates symbols longer than that.
// When symbols are longer than 255 characters, the warning is disabled.
#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <string>
#include <vector>

using namespace std;

#include "config.h"
#include "facontext.h"
#include "errors.h"
#include "preferences.h"

void CreateDirectoryPath(const char* path);
void ResolvePath(char** path);
void RFC822GMTTimeString(struct tm* time, char buf[32]);
char *strdup_new(const char *str);
char *FreeampDir(Preferences *prefs);
Error FilePathToURL(const char* path, char* url, uint32* length);
Error URLToFilePath(const char* url, char* path, uint32* length);
void ToUpper(char *s);
void ToLower(char *s);

#ifndef WIN32
void LaunchBrowser(const char* url);
bool CopyFile(const char *pExistingFileName, 
              const char *pNewFileName,      // name of new file
              bool bFailIfExists);     // operation if file exists
#endif

void FindMusicFiles(const char* rootPath, 
                    vector<string>& urls, 
                    vector<string>& queries); 
#ifdef WIN32
bool ResolveLink(string& path);
#endif

/* func to find the real filename of a path, irregardless of case */
string FindFile(string oPath);

void ShowHelp(FAContext *context, const char *helptopic);

#endif /* INCLUDED_UTILITY_H */

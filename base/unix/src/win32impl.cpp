#include <dlfcn.h>
#include <dirent.h>
#include <string.h>
#include <iostream.h>

#include "win32impl.h"

class FindData {
 public:
    DIR *pDir;
    char *dir; // points to directory name of search
    char *rest; // points to remainder of the filename, which we should match against
    ~FindData();
    FindData();
};
FindData::FindData() {
    dir = NULL;
    rest = NULL;
    pDir = NULL;
}
FindData::~FindData() {
    if (dir) delete dir;
    if (rest) delete rest;
}

bool Match(char *,char *);
void FillWin32FindData(char *,WIN32_FIND_DATA *);

HANDLE FindFirstFile(char *lpFileName, WIN32_FIND_DATA *lpFindFileData) {
    //cout << "FindFirstFile: begin" << endl;
    if (!lpFileName || !lpFindFileData) return INVALID_HANDLE_VALUE;

    FindData *pFD = new FindData();
    // 1) find earliest wildcard, chop up to directory marker
    char *ps = strchr(lpFileName,'*');
    char *pQ = strchr(lpFileName,'?');
    char *chopChar = '\0';
    if (!ps && !pQ) {
	// no wildcards... fill in the win32_find_data structure
	FillWin32FindData(lpFileName,lpFindFileData);
	// no more files...
	pFD->pDir = NULL;
	pFD->dir = NULL;
	pFD->rest = NULL;
	return (HANDLE)pFD;
    } else if (!ps && pQ) {
	chopChar = pQ;
    } else if (ps && !pQ) {
	chopChar = ps;
    } else {
	chopChar = (ps > pQ) ? pQ : ps;
    }

    char holdChar = *chopChar;
    *chopChar = '\0';
    char *lastSlash = strrchr(lpFileName,'/');
    *chopChar = holdChar;
    char *pDir, *pRest;
    if (lastSlash) {
	pDir = lpFileName;
	pRest = lastSlash + sizeof(char);
	*lastSlash = '\0';
	pFD->dir = new char[strlen(pDir)+1];
	pFD->rest = new char[strlen(pRest)+1];
	strcpy(pFD->dir,pDir);
	strcpy(pFD->rest,pRest);
	*lastSlash = '/';
    } else {
	pDir = NULL;
	pRest = lpFileName;
	pFD->rest = new char[strlen(pRest)+1];
	strcpy(pFD->rest,pRest);
    }

    if (pFD->dir) {
	pFD->pDir = opendir(pFD->dir);
	if (pFD->pDir) {
	    struct dirent *pdirent = readdir(pFD->pDir);
	    while (pdirent) {
		//cout << "FindFirstFile: matching " << pFD->rest << " to " << pdirent->d_name << endl;
		if (Match(pFD->rest,pdirent->d_name)) {
		    FillWin32FindData(pdirent->d_name,lpFindFileData);
		    return (HANDLE)pFD;
		}
		pdirent = readdir(pFD->pDir);
	    }
	    // no matching directory entries...
	    return INVALID_HANDLE_VALUE;
	} else {
	    // dir open failed, so no matches
	    return INVALID_HANDLE_VALUE;
	}
    } else {
	// no directory, just see if file exists...
    }

    return INVALID_HANDLE_VALUE;
}

bool Match(char *pattern,char *string) {
    //cout << "Comparing " << pattern << " to " << string << endl;
    if (!pattern || !string) return false;
    char *ps1 = pattern;
    char *ps2 = string;

    while ((*ps1 != '\0') && (ps2 != '\0')) {
	switch (*ps1) {
	    case '?':
		ps1++;
		ps2++;
		break;
	    case '*': {
	        // find existance of next block
		ps1++;
		char *pS = strchr(ps1,'*');
		char *pQ = strchr(ps1,'?');
		if (pS) {*pS = '\0';}  if (pQ) {*pQ = '\0';}
		char *pStr = strstr(ps2,ps1);
		if (pS) {*pS = '*';} if (pQ) {*pQ = '?';}
		if (pStr) {
		    ps2 = pStr;
		    return Match(ps1,ps2);
		}
		return false;
		break; }
	    default:
		if (*ps1 != *ps2) return false;
		ps1++;
		ps2++;
		break;
	}
    }
    if ((*ps1 == '\0') && (*ps2 == '\0'))
	return true;
    else
	return false;
}


void FillWin32FindData(char *pF,WIN32_FIND_DATA *wfd) {
//    cout << "FillWin32FindData: Entering: " << pF << endl;
    strcpy(wfd->cFileName,pF);
}


bool FindNextFile(HANDLE hFindHandle, WIN32_FIND_DATA *lpFindFileData) {
//    cout << "FindNext: begin" << endl;
    if (!hFindHandle || !lpFindFileData) return false;
    FindData *pFD = (FindData *)hFindHandle;
    if (!pFD->pDir) {
	// no dir, that means only one match, which we already did
	return false;
    }
    struct dirent *pdirent = readdir(pFD->pDir);
    while (pdirent) {
	if (Match(pFD->rest,pdirent->d_name)) {
	    FillWin32FindData(pdirent->d_name,lpFindFileData);
	    return true;
	}
	pdirent = readdir(pFD->pDir);
    }
    // no matches, or dir empty...
    return false;
}


bool FindClose(HANDLE hFindFile) {
    if (!hFindFile) return false;
    FindData *pFD = (FindData *)hFindFile;
    closedir(pFD->pDir);
    return true;
}



HINSTANCE LoadLibrary(char *lpLibFileName) {
    HINSTANCE hInst = dlopen(lpLibFileName, RTLD_NOW);
    if (!hInst) cout << dlerror() << endl;
    return hInst;
}



bool FreeLibrary(HMODULE hLibModule) {
    return dlclose(hLibModule) ? true : true;
}


FARPROC GetProcAddress(HMODULE hModule, char *lpProcName) {
    return dlsym(hModule,lpProcName);
}


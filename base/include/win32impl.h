#ifndef _WIN32IMPL_H_
#define _WIN32IMPL_H_

#include "config.h"

#define MAX_PATH 1024

class FILETIME {
 public:
    int32 dwLowDateTime;
    int32 dwHighDateTime;
};

class WIN32_FIND_DATA {
 public:
    int32 dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    int32 nFileSizeHigh;
    int32 nFileSizeLow;
    int32 dwReserved0;
    int32 dwReserved1;
    char cFileName[ MAX_PATH ];
    char cAlternateFileName[ 14 ];
};

typedef int HANDLE;
#define INVALID_HANDLE_VALUE ((HANDLE)-1)
typedef void *HMODULE;
#define HINSTANCE HMODULE

typedef void *FARPROC;

HANDLE FindFirstFile(char *lpFileName, WIN32_FIND_DATA *lpFindFileData);
bool FindNextFile(HANDLE hFindFile, WIN32_FIND_DATA *lpFindFileData);
bool FindClose(HANDLE hFindFile);
HINSTANCE LoadLibrary(char *lpLibFileName);
bool FreeLibrary(HMODULE hLibModule);
FARPROC GetProcAddress(HMODULE hModule, char *lpProcName);





#endif

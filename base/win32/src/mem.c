#include <windows.h>

#include "conerror.h"

HINSTANCE hInstance;

#if defined(__BORLANDC__)
#define DllMain DllEntryPoint
#endif

INT WINAPI DllMain
(
    HANDLE hInst,
    ULONG ul_reason_being_called,
	LPVOID lpReserved
)
{

    switch (ul_reason_being_called)
    {
		case DLL_PROCESS_ATTACH:
				hInstance = hInst;
            break;

        case DLL_THREAD_ATTACH:

            break;

        case DLL_THREAD_DETACH:

            break;

		  case DLL_PROCESS_DETACH:
				break;
    }

    return 1;                 
}


__declspec(dllexport) void* __malloc(size_t size)
{
	void* result = NULL;

    result = malloc(size);

    return result;
}


__declspec(dllexport) void __free(void* p)
{
    if(p)
    {
        free(p);
    }
}

/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player
	Portions copyright (C) 1998 GoodNoise

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

#ifndef ERROR_H
#define ERROR_H

#include <config.h>


typedef enum Error
{
    kError_NoErr		    = 0,
    kError_UnknownErr	    = 1,
    kError_InvalidParam     = 2,
    kError_LoadLibFailed    = 3,
    kError_FindFuncFailed   = 4,
    kError_NoFiles          = 5,
    kError_NoPrefs          = 6,
    kError_BufferTooSmall   = 7,
    kError_OutOfMemory      = 8,
    kError_FileNoAccess     = 9,
    kError_FileExists       = 10,
    kError_FileInvalidArg   = 11,
    kError_FileNotFound     = 12,
    kError_FileNoHandles    = 13,
    kError_NoMoreLibDirs    = 14,
    kError_NullValueInvalid = 15,
    kError_InvalidError     = 16,
    kError_PluginNotInitialized = 17,
    kError_GotDefaultMethod = 18,
    kError_InputUnsuccessful = 19,
    kError_FileSeekNotSupported = 20,
    kError_OutputUnsuccessful = 21,
    kError_InitFailed = 22,
    kError_InitFailedSafely = 23,
    kError_NoDataAvail = 24,
    kError_SeekFailed = 25,
    kError_CreateThreadFailed = 26,
    kError_BufferingUp = 27,
    kError_Interrupt   = 28,
    kError_EventPending = 29,
    kError_YouScrewedUp = 30
}Error;


#define IsError( err )		( (err) != kError_NoErr )
#define IsntError( err )	( (err) == kError_NoErr )



#endif /* ERROR_H */

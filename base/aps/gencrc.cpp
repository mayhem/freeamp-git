/*__________________________________________________________________________        
        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 2000 Relatable

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program; if not, Write to the Free Software
        Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
        
        $Id$
____________________________________________________________________________*/


#include "apsutility.h"

unsigned short int GenCRC(const char* pBuffer, int nLen)
{
    unsigned short int nReturn = 0;
    int j = 0;
    for (int i = 0; i < nLen; i++)
    {
        nReturn = nReturn ^ (pBuffer[i] << 8);
        for (j = 0; j < 8; j++ )
        {
            if (nReturn & 0x8000)
            {
                nReturn = ((nReturn << 1) ^ 0x1021);
            }
            else
                nReturn <<= 1;
        }
    }

    // Bleh. This is VERY ugly. asm swap below is MUCH better.
    unsigned char a;
    a = ((unsigned char*)(void*)&nReturn)[0];
    ((unsigned char*)(void*)&nReturn)[0] = ((unsigned char*)(void*)&nReturn)[1];
    ((unsigned char*)(void*)&nReturn)[1] = a;
	
    /* How do you inline asm in G++? this is MUCH faster than cast city above
    asm {
        mov ax,nReturn
        xchg ah,al
        mov nReturn,ax
    }
    */
    return nReturn;
}

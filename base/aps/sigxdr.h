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

#ifndef _SIGXDR_H_
#define _SIGXDR_H_

#include "config.h"
#include "audiosig.h"

#include <string>
using namespace std;

#define FIELDSIZE 4

class SigXDR
{
public:
    SigXDR();
   ~SigXDR();

    char  *FromSig(AudioSig *sig);
    string ToStrGUID(char *buffer, long size);
 
private:
    void PutInt32(int32 *data);
    void GetInt32(int32 *data);

    void PutFloat(float *data);
    void GetFloat(float *data);

    char *m_buffer;
    char *m_position;
    long  m_size;
};

#endif

/*_______________________________________________________________________        
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

/***************************************************************************
                          ypclient.h  -  description
                             -------------------
    begin                : Sat Jun 3 2000
    copyright            : (C) 2000 by Relatable
    written by           : Sean Ward
    email                : sward@relatable.com
 ***************************************************************************/

#ifndef SIGCLIENT_H
#define SIGCLIENT_H

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "audiosig.h"

using namespace std;

class COMSocket;
class Mutex;

class SigClient
{
public:
    SigClient();
   ~SigClient();
    int GetSignature(AudioSig *sig, string &strGUID);
    void SetAddress(string strIP, int nPort) 
    { m_strIP = strIP; m_nPort = nPort; }

protected:
    int Connect(string& strIP, int nPort);
    int Disconnect();

private:
    COMSocket* m_pSocket;
    Mutex* m_pMutex;
    string m_strIP;
    int m_nPort;
    int m_nNumFailures;
};

#endif


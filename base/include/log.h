/*____________________________________________________________________________
        
        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 1998 GoodNoise

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
#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

enum 
{
    LogMain    = 0,
    LogDecode  = 1,
    LogInput   = 2,
    LogOutput  = 3,

    LogLast    = 4,
    LogMax     = 32
};

class LogFile
{
    public:

                 LogFile(char *szLogFileArg);
        virtual ~LogFile(void);

        bool     Open(void);
        void     ClearLogLevel(void) 
                 { m_iLogLevelFlags = 0; };
        void     AddLogLevel(int iLogLevelFlags);

        void     Error(char *Format, ...);
        void     Log(int Section, char *Format, ...);

    private:

        FILE *m_fpLog;
        char *m_szLogFile;
        int   m_iLogLevelFlags;
};

extern LogFile *g_Log;

#endif
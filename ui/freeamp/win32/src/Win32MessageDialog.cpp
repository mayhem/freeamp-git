/*____________________________________________________________________________

   FreeAmp - The Free MP3 Player

   Copyright (C) 1999 EMusic

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

#include <windows.h>
#include "MessageDialog.h"


MessageDialog::MessageDialog(void)
{
}

MessageDialog::~MessageDialog(void)
{

}

MessageDialogReturnEnum MessageDialog::
                        Show(const char        *szMessage, 
                             const char        *szTitle, 
                             MessageDialogEnum  eType)
{
    string oMessage(szMessage), oTitle(szTitle);
    
    return Show(oMessage, oTitle, eType);
}

MessageDialogReturnEnum MessageDialog::
                        Show(const string      &oMessage, 
                             const string      &oTitle, 
                             MessageDialogEnum  eType)
{
	int                     iRet, iType;
    
    switch(eType)
    {
        case kMessageOk:
           iType = MB_OK;
           break;
        case kMessageYesNo:
           iType = MB_YESNO;
           break;
        case kMessageOkCancel:
           iType = MB_OKCANCEL;
           break;
        case kMessageMonicaSucks:
           iType = MB_RETRYCANCEL;
           break;
    }
    
    iRet = MessageBox(NULL, oMessage.c_str(), oTitle.c_str(), iType);
    switch(iRet)
    {
    	case IDOK:
           return kMessageReturnOk;
    	case IDYES:
           return kMessageReturnYes;
    	case IDNO:
           return kMessageReturnNo;
    	case IDCANCEL:
           return kMessageReturnCancel;
    	case IDRETRY:
           return kMessageReturnMonicaDoesSuck;
        default:   
           return kMessageReturnUnknown;
    }
    
}


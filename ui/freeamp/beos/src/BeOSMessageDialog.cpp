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

#include <Alert.h>
#include "MessageDialog.h"
#include <stdio.h>

MessageDialog::MessageDialog(FAContext *context)
{
}

MessageDialog::~MessageDialog()
{
}

MessageDialogReturnEnum
MessageDialog::Show( const char* szMessage,
                     const char* szTitle,
                     MessageDialogEnum eType,
                     bool InEventLoop )
{
	MessageDialogReturnEnum result = kMessageReturnUnknown;
	int32 button_index = 0;
	BAlert *alert = NULL;
	
	
    printf( "MessageDialog: %s\n", szMessage );
    
    switch(eType)
    {	
    	case kMessageOk:
    		alert = new BAlert(szTitle, szMessage, "OK");
    		break;
    		
    	case kMessageYesNo:
    		alert = new BAlert(szTitle, szMessage, "Yes", "No");
    		break;
    		
    	case kMessageOkCancel:
    	default:
    		alert = new BAlert(szTitle, szMessage, "OK", "Cancel");
    		break;
	}
    
	button_index = alert->Go(); 
	
	switch(eType)
	{
		case kMessageOk:
    		result = kMessageReturnOk;
    		break;
    		
    	case kMessageYesNo:
    		if(button_index == 0)
    			result = kMessageReturnYes;
    		else
    			result = kMessageReturnNo;
    		break;
    	
    	case kMessageOkCancel:
    	default:
    		if(button_index == 0)
    			result = kMessageReturnOk;
    		else
    			result = kMessageReturnCancel;
    		break;
    	
    	
	}

   	return result;
}

MessageDialogReturnEnum
MessageDialog::Show( const string& oMessage,
                     const string& oTitle,
                     MessageDialogEnum eType,
                     bool InEventLoop )
{
    return Show(oMessage.c_str(), oTitle.c_str(), eType, InEventLoop );
}

/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998-1999 EMusic.com

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

#include <Appearance.h>

#include <LGrowZone.h>
#include <UDrawingState.h>
#include <UEnvironment.h>
#include <UMemoryMgr.h>

//#include "player.h"
//#include "event.h"
//#include "registrar.h"
//#include "log.h"
#include "facontext.h"
//#include "macprefs.h"
#include "CFreeAmp.h"




	int
main()
{
	SetDebugThrow_(debugAction_Debugger);
	SetDebugSignal_(debugAction_Debugger);
	InitializeHeap(3);
	UQDGlobals::InitializeToolbox(&qd);
	new LGrowZone(20000);
	
	CFreeAmp	theApp;

	theApp.Run();
	
	return 0;
}



CFreeAmp::CFreeAmp()
{
	::RegisterAppearanceClient();

	// Register classes
}



CFreeAmp::~CFreeAmp()
{
}



	void
CFreeAmp::StartUp()
{
}



	Boolean
CFreeAmp::ObeyCommand(
	CommandT	inCommand,
	void*		ioParam)
{
	Boolean		cmdHandled = true;

	switch (inCommand) {
		default:
		{
			cmdHandled = LApplication::ObeyCommand(inCommand, ioParam);
		}
		break;
	}
	
	return cmdHandled;
}



	void
CFreeAmp::FindCommandStatus(
	CommandT	inCommand,
	Boolean&	outEnabled,
	Boolean&	outUsesMark,
	UInt16&		outMark,
	Str255		outName)
{
	switch (inCommand) {
		default:
		{
			LApplication::FindCommandStatus(inCommand, outEnabled,
										outUsesMark, outMark, outName);
		}
		break;
	}
}

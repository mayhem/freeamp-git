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
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
	
	$Id$
____________________________________________________________________________*/

#include <iostream.h>
#include <stdio.h>

#include "CommandLineCOO.h"
#include "event.h"
#include "player.h"

#include "eventdata.h"

CommandLineCOO::CommandLineCOO() {}

CommandLineCOO::~CommandLineCOO() {
    //cout << "CommandLineCOO: begin deleted..." << endl;
}

void CommandLineCOO::setArgs(int a, char **b) {}


int32 CommandLineCOO::acceptCOOEvent(Event *e) {
    if (e) {
	//cout << "CommandLineCOO: processing event " << e->getEvent() << endl;
	switch (e->getEvent()) {
	    case INFO_MediaVitalStats: {
		MediaVitalInfo *pmvi = (MediaVitalInfo *)e->getArgument();
		if (pmvi) {
		    cout << "Playing: " << pmvi->songTitle << endl;
#if 0
		    //cout << "CMDLineCOO: got pmvi" << endl;
		    char foo[64];
		    sprintf(foo,"%.2d:%.2d:%.2d.%.2d",pmvi->hours,pmvi->minutes,pmvi->seconds,pmvi->milliseconds);
		    cout << "Track Title: " << pmvi->title << endl;
		    //cout << "Track Length: " << pmvi->hours << ":" << pmvi->minutes << ":" << pmvi->seconds << "." << pmvi->milliseconds << endl;
		    cout << "Track Length: " << foo << endl;
		    cout << "Track Range: " << pmvi->range << endl;
		    cout << "Track bps: " << pmvi->bps << endl;
		    cout << "Track frequency: " << pmvi->frequency << endl;
		    cout << endl;
#endif
		}
		break; }
	    case CMD_Cleanup: {
		Event *e = new Event(INFO_ReadyToDieCOO,this);
		Player::getPlayer()->acceptEvent(*e);
		delete e;
		break; }
	    default:
		break;
	}
    }
    //cout << "Done cmdlinecoo: accpet event" << endl;
    return 0;
}







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
#include <unistd.h>

#include <curses.h>
#include <termios.h>


#include "CommandLineCIO.h"
#include "event.h"
#include "player.h"
#include "playlist.h"
#include "thread.h"

#define stdinfd 0

static struct termios normalTTY;
static struct termios rawTTY;
int getKey() {
    fd_set rdfs;

    FD_ZERO(&rdfs);
    FD_SET(stdinfd, &rdfs);
    if (select (stdinfd+1,&rdfs,NULL,NULL,NULL) == 1) {
	if (FD_ISSET(stdinfd, &rdfs)) return 1;
    }
    return 0;
}
CommandLineCIO::CommandLineCIO() {

    mypl = NULL;

    tcgetattr(stdinfd, &::normalTTY);
    ::rawTTY = ::normalTTY;
    ::rawTTY.c_lflag &= ~ICANON;
    ::rawTTY.c_lflag &= ~ECHO;
    tcsetattr(stdinfd, TCSANOW, &rawTTY);


    keyboardListenThread = Thread::CreateThread();
    keyboardListenThread->Create(CommandLineCIO::keyboardServiceFunction,this);
    
    cout << endl << "Command Line Interface" << endl << endl;
    cout << " * q    Quit" << endl;
    cout << " * +/=  Next Song" << endl;
    cout << " * -    Prev Song" << endl;
    cout << " * p    Pause / UnPause" << endl;
    cout << " * s    Shuffle" << endl << endl;
}


CommandLineCIO::~CommandLineCIO() {
    tcsetattr(stdinfd, TCSANOW, &normalTTY);
    //cout << "CommandLineCIO: begin deleted..." << endl;
    if (keyboardListenThread) {
	keyboardListenThread->Destroy();
	delete keyboardListenThread;
	keyboardListenThread = NULL;
    }
}

void CommandLineCIO::keyboardServiceFunction(void *pclcio) {
    CommandLineCIO *pMe = (CommandLineCIO *)pclcio;
    char *pkey = new char[1];
    char chr;
    size_t rtn;
    int fn = STDIN_FILENO;
    for (;;) {
	::getKey();
	read(stdinfd,&chr,1);
	switch (chr) {
	    case 'p':
	    case 'P': {
		Event *e = new Event(CMD_TogglePause);
		Player::GetPlayer()->AcceptEvent(e);
		break;
	    }
	    case '-': {
		Event *e = new Event(CMD_PrevMediaPiece);
		Player::GetPlayer()->AcceptEvent(e);
		e = new Event(CMD_Play);
		Player::GetPlayer()->AcceptEvent(e);
		break;
	    }
	    case '=':
	    case '+':
	    case 'n':
	    case 'N': {
		Event *e = new Event(CMD_NextMediaPiece);
		Player::GetPlayer()->AcceptEvent(e);
		e = new Event(CMD_Play);
		Player::GetPlayer()->AcceptEvent(e);
		break; }
	    case 'q':
	    case 'Q': {
		Event *e = new Event(CMD_QuitPlayer);
		Player::GetPlayer()->AcceptEvent(e);
		break; }
	    case 's':
	    case 'S': {
		if (pMe->mypl) {
		    pMe->mypl->Shuffle();
		    pMe->mypl->SetFirst();
		}
		Event *e = new Event(CMD_Stop);
		Player::GetPlayer()->AcceptEvent(e);
		e = new Event(CMD_Play);
		Player::GetPlayer()->AcceptEvent(e);
	    }
	    default:
		break;
	}
    }

}

int32 CommandLineCIO::acceptCIOEvent(Event *e) {
    if (e) {
	//cout << "CommandLineCIO: processing event " << e->getEvent() << endl;
	switch (e->getEvent()) {
	    case INFO_PlayListDonePlay: {
		Event *e = new Event(CMD_QuitPlayer);
		Player::GetPlayer()->AcceptEvent(e);
		break; }
	    case CMD_Cleanup: {
		Event *e = new Event(INFO_ReadyToDieCIO,this);
		Player::GetPlayer()->AcceptEvent(e);
		break; }
	    default:
		break;
	}
    }
    return 0;
}

void CommandLineCIO::setArgs(int argc, char **argv) {
    mypl = new PlayList();
    char *pc = NULL;
    for(int i=1;i<argc;i++) {
	//cout << "Adding arg " << i << ": " << argv[i] << endl;
	pc = argv[i];
	if (pc[0] == '-') {
	    processSwitch(&(pc[0]));
	} else {
	    mypl->Add(pc,0);
	}
    }
    mypl->SetFirst();
    Event *e = new Event(CMD_SetPlaylist,mypl);
    Player::GetPlayer()->AcceptEvent(e);
    e = new Event(CMD_Play);
    Player::GetPlayer()->AcceptEvent(e);
}

void CommandLineCIO::processSwitch(char *pc) {
    return;
}







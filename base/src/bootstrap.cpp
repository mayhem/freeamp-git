
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

// bootstrap.cpp -- Main freeamp program.  Loads Player object and registers compiled in modules

#include <iostream.h>
#include <stdio.h>

#include "player.h"
#include "event.h"
#include "ctrlobj.h"
#include "vector.h"
#include "buffer.h"
#include "hashtable.h"
#include "semaphore.h"

#ifdef WIN32
#include "consoleCIO.h"
#include "consoleCOO.h"
#define DEFAULT_PROG_NAME "freeamp_console.exe"
#define DEFAULT_PROG_NAME_LENGTH 19
#define DEFAULT_CIO ConsoleCIO()
#define DEFAULT_COO ConsoleCOO()

#else

#include "CommandLineCIO.h"
#include "CommandLineCOO.h"
#include "Mpg123UI.h"
#define DEFAULT_PROG_NAME "freeamp"
#define DEFAULT_PROG_NAME_LENGTH 7
#define DEFAULT_CIO CommandLineCIO()
#define DEFAULT_COO CommandLineCOO()
#endif

Semaphore *termSemaphore;

void testVector();
void testBuffer();
void testHashTable();
class DummyCOO : public COO {
 public:
    virtual int32 acceptCOOEvent(Event *);
    virtual void setArgs(int a, char**b) {}
    Player *pPlayer;
    DummyCOO(Player *);
    virtual ~DummyCOO();
};

DummyCOO::DummyCOO(Player *p) {
    pPlayer = p;
    //printf("Dummy this: %x\n",this);
}

DummyCOO::~DummyCOO() {
    //cout << "DummyCOO: being deleted..." << endl;
}

int32 DummyCOO::acceptCOOEvent(Event *pe) {
    if (pe) {
  	//cout << "DummyCOO::acceptEvent: processing " << pe->getEvent() << "..." << endl;
	switch (pe->getEvent()) {
	    case CMD_Terminate:
		::termSemaphore->Signal();
		break;
	    case CMD_Cleanup: {
		Event *pE = new Event(INFO_ReadyToDieCOO,this);
		//pPlayer->acceptEvent(*pE);
		Player::getPlayer()->acceptEvent(*pE);
		delete pE;
		break; }
	    default:
		break;
	}
	return 0;
    } else {
	return 255;
    }
}


int main(int argc, char **argv) {
    //testVector();
    //testBuffer();
    //testHashTable();


    termSemaphore = new Semaphore();
    Player *pP = Player::getPlayer();
    //cout << "Got player..." << endl;
    DummyCOO *pDCOO = new DummyCOO(pP);
    //cout << "Created dcoo..." << endl;
    pP->registerCOO(pDCOO);
    //cout << "Registered DummyCOO" << endl;

    CIO *pCIO = NULL;
    COO *pCOO = NULL;
#ifdef __linux__
    int foo = strlen(argv[0]);
    int endOfArgv0 = ((int)(argv[0])) + foo;
    if (((int)(strstr(argv[0],DEFAULT_PROG_NAME)) + /*length of "freeamp" */ DEFAULT_PROG_NAME_LENGTH) == endOfArgv0) {
	cout << "Using FreeAmp interface..." << endl;
	pCIO = new DEFAULT_CIO;
	pP->registerCIO(pCIO);
	//cout << "Registered CommandLine CIO" << endl;
	pCOO = new DEFAULT_COO;
	pP->registerCOO(pCOO) ;
	//cout << "Registered CommandLine COO" << endl;
    } 

    else if (((int)(strstr(argv[0],"mpg123")) + 6) == endOfArgv0) {
	// mpg123, etc here
	//cout << "Using mpg123 interface..." << endl;
	Mpg123UI *pMpg = new Mpg123UI();
	pCIO = (CIO *)pMpg;
	pCOO = (COO *)pMpg;
	pP->registerCIO(pCIO);
	pP->registerCOO(pCOO) ;
    }
#else
	cout << "Using FreeAmp interface..." << endl;
	pCIO = new DEFAULT_CIO;
	pP->registerCIO(pCIO);
	pCOO = new DEFAULT_COO;
	pP->registerCOO(pCOO) ;
#endif

    if (!pCIO || !pCOO) {
	cout << "Error initializing UI." << endl;
	return 255;
    }
    pCIO->setArgs(argc,argv);
    pCIO = NULL;  // is deleted by player now that it is registered.
    pCOO = NULL;
    //cout << "Set the args..." << endl;
    //pP->testQueue();

//    Event *pC = new Event(CMD_Stop);
//    pP->acceptEvent(*pC);
//    delete pC;
//    pC = new Event(CMD_NextSong);
//    pP->acceptEvent(*pC);
//    delete pC;
//    pC = new Event(CMD_QuitPlayer);
//    pP->acceptEvent(*pC);
//    delete pC;
//    cout << "main: stopping..." << endl;

    termSemaphore->Wait();

//    while (globalTimeToTerminate == 0) {
//	usleep(300000);
//  }
    delete pP;
    return 0;
}

void testVector() {
    cout << "Beginning testVector..." << endl;
    Vector<char *> *pVect = new Vector<char *>(2);
    char *p1 = "Hey man!";
    char *p2 = "Yo dude!";
    char *p3 = "And, the third.";
    pVect->insert(p1);
    pVect->insert(p2);
    pVect->insert(p3);
    
    cout << pVect->elementAt(0) << endl;
    cout << pVect->elementAt(1) << endl;
    cout << pVect->elementAt(2) << endl;

    pVect->removeElementAt(1);

    cout << pVect->elementAt(0) << endl;
    cout << pVect->elementAt(1) << endl;

    pVect->RemoveAll();

    if (pVect->elementAt(0) != NULL) {
	cout << "Final Test failed!!" << endl;
    }

    Vector<DummyCOO *> *pVect2 = new Vector<DummyCOO *>(2);
    DummyCOO *pd = new DummyCOO(NULL);
    pVect2->insert(pd);
    pd = new DummyCOO(NULL);
    pVect2->insert(pd);
    pd = new DummyCOO(NULL);
    pVect2->insert(pd);

    pVect2->DeleteAll();
    if (pVect2->elementAt(0) != NULL) {
	cout << "Final Test failed!! (2nd one)" << endl;
    }
    cout << "Ending testVector..." << endl;
}
void testBuffer() {
    cout << "Begin testBuffer()...." << endl;
    Buffer *pBuf = new Buffer();
    cout << "Created buffer" << endl;
    int32 hRead0 = pBuf->registerReader();
    cout << "registered reader 0" << endl;
    int32 hRead1 = pBuf->registerReader();
    cout << "registered reader 1" << endl;
    int32 hRead2 = pBuf->registerReader();
    cout << "registered reader 2" << endl;


    char *pStr1 = new char[1024];
    char *pStr2 = new char[1024];
    char *pStr3 = new char[1024];
    sprintf(pStr1,"Test string 1");
    sprintf(pStr2,"Test string 2");
    sprintf(pStr3,"Test string 3");

    cout << "Created blocks..." << endl;

    pBuf->writeBlock(pStr1,1024);
    cout << "wrote block 1" << endl;
    pBuf->writeBlock(pStr2,1024);
    cout << "wrote block 2" << endl;
    pBuf->writeBlock(pStr3,1024);
    cout << "wrote block 3" << endl;


    
    delete pStr1;
    delete pStr2;
    delete pStr3;

    
    pBuf->getBlock(hRead0,&pStr1);
    cout << pStr1 << endl;
    delete pStr1;
    pBuf->getBlock(hRead0,&pStr1);
    cout << pStr1 << endl;
    delete pStr1;
    pBuf->getBlock(hRead0,&pStr1);
    cout << pStr1 << endl;
    delete pStr1;

    pBuf->getBlock(hRead1,&pStr1);
    cout << pStr1 << endl;
    delete pStr1;
    pBuf->getBlock(hRead2,&pStr1);
    cout << pStr1 << endl;
    delete pStr1;
    pBuf->getBlock(hRead1,&pStr1);
    cout << pStr1 << endl;
    delete pStr1;
    pBuf->getBlock(hRead2,&pStr1);
    cout << pStr1 << endl;
    delete pStr1;
    pBuf->getBlock(hRead1,&pStr1);
    cout << pStr1 << endl;
    delete pStr1;
    pBuf->getBlock(hRead2,&pStr1);
    cout << pStr1 << endl;
    delete pStr1;
    
    pBuf->releaseReader(hRead1);

    pStr1 = new char[1024];
    sprintf(pStr1,"Test String 4");
    pBuf->writeBlock(pStr1,1024);
    delete pStr1;

    pBuf->getBlock(hRead0,&pStr1);
    cout << pStr1 << endl;
    delete pStr1;
    
    pBuf->getBlock(hRead1,&pStr1);
    if (pStr1 != NULL) {
	cout << "Didn't return null!!" << endl;
    } else {
	cout << "Returned null properly!" << endl;
    }

    pBuf->getBlock(hRead2,&pStr1);
    cout << pStr1 << endl;
    delete pStr1;

    delete pBuf;
    cout << "End testBuffer()..." << endl;
}


void testHashTable() {
    HashTable *pHT = new HashTable();
    cout << "Adding 'foo'->1" << endl;
    char *foo = "foo";
    char *bar = "bar";
    pHT->Insert(foo,(void *)1);
    cout << "Adding 'bar'->2" << endl;
    pHT->Insert(bar,(void *)2);

    int out = (int32) pHT->Value(foo);
    cout << "out 1:  " << out << endl;
    out = (int32) pHT->Value(bar);
    cout << "out 2:  " << out << endl;
    exit(1);
}

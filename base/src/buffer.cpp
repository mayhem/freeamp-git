
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

// buffer.cpp

#include <iostream.h>

#include "config.h"
#include "buffer.h"
#include "vector.h"
#include "queue.h"


Buffer::Buffer(int32 blockSize) {
    Buffer::blockSize = blockSize;
    maxBlocks = 1000000;
    pReaders = new Vector<BufferReaderInfo *>(5);
    myMutex = new Mutex();
    numBlocks = 0;
}
Buffer::Buffer(int32 blockSize, int32 maxBlocks) {
    Buffer::blockSize = blockSize;
    Buffer::maxBlocks = maxBlocks;
    numBlocks = 0;
    pReaders = new Vector<BufferReaderInfo *>(5);
    myMutex = new Mutex();
}
Buffer::BufferReaderInfo::~BufferReaderInfo() {
    if (pUnreadBlocks) {
	delete pUnreadBlocks;
	pUnreadBlocks = NULL;
    }
}
Buffer::Block::~Block() {
    if (pBlock) {
	delete pBlock;
	pBlock = NULL;
    }
}
Buffer::~Buffer() {
    if (pReaders) {
	pReaders->DeleteAll();
	delete pReaders;
	pReaders = NULL;
    }
    if (myMutex) {
	delete myMutex;
	myMutex = NULL;
    }
}

void Buffer::getLock() {
    myMutex->Acquire(WAIT_FOREVER);
    return;
}

void Buffer::releaseLock() {
    myMutex->Release();
    return;
}

int32 Buffer::registerReader() {
//    cout << "Buffer::registerReader: beginning..." << endl;
    getLock();
//    cout << "Buffer::registerReader: got lock" << endl;
    BufferReaderInfo *pBRI = new BufferReaderInfo();
//    cout << "Buffer::registerReader: created readerinfo" << endl;
    pBRI->pUnreadBlocks = new Queue<Block *>();
//    cout << "Buffer::registerReader: created queue" << endl;
    pBRI->isRegistered = true;
    int32 hReader = pReaders->insert(pBRI);
//    cout << "Buffer::registerReader: inserted reader, got handle" << endl;
    releaseLock();
//    cout << "Buffer::registerReader: released lock" << endl;
    return hReader;
}

void Buffer::releaseReader(int32 hReader) {
    getLock();
    BufferReaderInfo *pBRI = pReaders->elementAt(hReader);
    if (pBRI) {
        delete pBRI->pUnreadBlocks;
	pBRI->pUnreadBlocks = NULL;
	pBRI->isRegistered = false;
    }
    releaseLock();
}

int32 Buffer::getBlock(int32 hReader, char **writeLocation, bool blockWhileEmpty) {
    getLock();
    int32 rtnVal = 0;
    BufferReaderInfo *pBRI = pReaders->elementAt(hReader);
    *writeLocation = NULL;
    if (pBRI) {
	if (pBRI->isRegistered) {
	    if (blockWhileEmpty) {
		while (pBRI->pUnreadBlocks->isEmpty()) { usleep(500000); }
	    }
	    Block *pB = pBRI->pUnreadBlocks->read();
	    if (pB) {
		*writeLocation = pB->pBlock;
		rtnVal = pB->numBytes;
		numBlocks--;
	    }
	}
    }
    releaseLock();
    return rtnVal;
}

void Buffer::writeBlock(char *pBlock, int32 actualSize) {
    while (numBlocks >= maxBlocks) { usleep(100000); }  // block until people start reading (actually, reader 0...)
    getLock();
    int32 index = 0;
    BufferReaderInfo *pBRI = pReaders->elementAt(index);
    char *pChar = NULL;
    while (pBRI != NULL) {
	if (pBRI->isRegistered) {
	    pChar = new char[actualSize];
	    memcpy(pChar,pBlock,actualSize);
	    Block *pB = new Block();
	    pB->numBytes = actualSize;
	    pB->pBlock = pChar;
	    pBRI->pUnreadBlocks->write(pB);
	}
	pBRI = pReaders->elementAt(++index);
    }
    numBlocks++;
    releaseLock();
}

int32 Buffer::getBlockSize() {
    return blockSize;
}










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

//buffer.h
// the one writer many readers buffer used to communicate between PMIs, LMCs and PMOs

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "config.h"
#include "queue.h"
#include "vector.h"
#include "mutex.h"

class Buffer {
 private:
    class Block {
    public:
	char *pBlock;
	int32 numBytes;
	~Block();
    };
    class BufferReaderInfo {
	// should put in a destructer here...
    public:
	Queue<Block *> *pUnreadBlocks;
	bool isRegistered;
	~BufferReaderInfo();
    };
    Vector<BufferReaderInfo *> *pReaders;
    int32 blockSize;
    int32 maxBlocks;
    int32 numBlocks;
    Mutex *myMutex;
    void getLock();
    void releaseLock();
 public:
    Buffer(int32 blockSize = 1024); // set blocksize
    Buffer(int32, int32); // blocksize, max blocks before write block
    ~Buffer();
    int32 registerReader();  // returns hReader
    void releaseReader(int32 hReader);
    int32 getBlock(int32 hReader,char **writeLocation, bool blockWhileEmpty = true); // returns num bytes actually there
    void writeBlock(char *pBlock,int32 actualSize);
    int32 getBlockSize();
};




#endif // _BUFFER_H_

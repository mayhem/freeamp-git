/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

        MP3 Decoder originally Copyright (C) 1995-1997 Xing Technology
        Corp.  http://www.xingtech.com

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

/* system headers */
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>

/* project headers */
#include "config.h"
#include "xinglmc.h"
#include "event.h"
#include "eventdata.h"
#include "player.h"
#include "mutex.h"

extern "C" {
#include "mhead.h"
#include "port.h"
	   }

int wait_n_times;

#define TEST_TIME 0

XingLMC::
//XingLMC(Input* input, Output* output, EventCallback callback, void* cookie)
XingLMC(PhysicalMediaInput* input, PhysicalMediaOutput* output)
{
    //cout << "XingLMC::XingLMC: Creating XingLMC..." << endl;
    wait_n_times = 0;
    decoderThread = NULL;
    xcqueue = new Queue<XingCommand *>(false); 
    seek_mutex = new Mutex();
    m_input				= input;
    m_output			= output;
    bs_bufbytes = 0;
    bs_trigger = 2500;
    bs_buffer = NULL;
    bs_bufptr = bs_buffer;
    bs_buffer = new unsigned char[BS_BUFBYTES];
    if (bs_fill()) {
	MPEG_HEAD head;
	int32 bitrate;
	// parse MPEG header
	framebytes = head_info2(bs_buffer, bs_bufbytes, &head, &bitrate);
	if (framebytes == 0) {
	    // BAD OR UNSUPPORTED MPEG FILE!!!
	    cout << "XingLMC::XingLMC: bad or unsupported MPEG file" << endl;
	    return;
	}
	{ // send track info (there is more info to be gotten, see towave.c : out_mpeg_info
	    static int sr_table[8] =
	    {22050L, 24000L, 16000L, 1L,
	     44100L, 48000L, 32000L, 1L};

	    int32 totalFrames = 0;
	    int32 bytesPerFrame = framebytes;
	    int32 backhere = m_input->Seek(0,SEEK_FROM_CURRENT);
//	    cout << "Back here: " << backhere << endl;
	    int32 end = m_input->Seek(0,SEEK_FROM_END);
	    m_input->Seek(-128,SEEK_FROM_CURRENT);
	    char buf[5];
	    m_input->Read(buf,4); // account for id3v1 / id3v1.1 tag
	    if ((buf[0] == 'T') &&
		(buf[1] == 'A') &&
		(buf[2] == 'G')) {
		end -= 128;
	    }
//	    cout << "now here: " << m_input->Seek(backhere,SEEK_FROM_START) << endl;
	    m_input->Seek(backhere,SEEK_FROM_START);
	    totalFrames = end / bytesPerFrame;

	    double tpf = (double)1152 / (double)(44100 << 0);
	    float totalTime = (float)((double)totalFrames * (double)tpf);

	    char *pbitrate = new char[64];
	    char *psamprate = new char[64];
	    char *streamname = m_input->GetStreamName();
	    sprintf(pbitrate,"%d kbits/s",bitrate/1000);
	    int32 samprate = sr_table[4*head.id+head.sr_index];
	    if ((head.sync & 1) == 0) samprate = samprate / 2;
	    sprintf(psamprate,"%d Hz",samprate);
	    MediaVitalInfo *mvi = new MediaVitalInfo(streamname,streamname,totalFrames,bytesPerFrame,bitrate,samprate,totalTime);
	    delete streamname;
	    Event *e = new Event(INFO_MediaVitalStats,mvi);
	    Player::getPlayer()->acceptEvent(*e);
	    delete e;
	}
	
	pcm_buffer = new unsigned char[PCM_BUFBYTES];
	pcm_trigger = (PCM_BUFBYTES - 2500 * sizeof(short));
	pcm_bufbytes = 0;
	
	if (audio_decode_init(&head,framebytes,0 /* reduction code */,0,0 /* convert code */, 24000 /* freq limit */)) {
	    DEC_INFO decinfo;
	    audio_decode_info(&decinfo);
	    { // send track info (there is more info to be gotten, see towave.c : out_mpeg_info
//		    int32 hours,minutes,seconds,milliseconds,frames;
//		    char *pbitrate = new char[64];
//		    char *psamprate = new char[64];
//		    sprintf(pbitrate,"%6dKb",decinfo.bits);
//		    sprintf(psamprate,"%6ld KHz",decinfo.samprate);
//		    MediaVitalInfo *mvi = new MediaVitalInfo(NULL,hours,minutes,seconds,milliseconds,frames,pbitrate,psamprate);
//		    Event *e = new Event(INFO_MediaVitalStats,mvi);
//		    Player::getPlayer()->acceptEvent(*e);
//		    delete e;
	    }
#if __BYTE_ORDER != __LITTLE_ENDIAN
	    cvt_to_wave_init(decinfo.bits);
#endif
	    OutputInfo info;
	    info.bits_per_sample = decinfo.bits;
	    info.number_of_channels = decinfo.channels;
	    info.samples_per_second = decinfo.samprate;
	    info.max_buffer_size = (info.number_of_channels * 2 * 1152) << 5;
	    m_output->Initialize(&info);
	} else {
	    cout << "Couldn't init decoder..." << endl;
	    return;
	}
    } else {
	cout << "Couldn't fill the manure pile..." << endl;
	return;
    }
    //cout << "XingLMC::XingLMC: constructor succeeded" << endl;
}

XingLMC::~XingLMC() {
    Stop();
    if (xcqueue) {
	delete xcqueue;
	xcqueue = NULL;
    }
    if (m_output) {
	delete m_output;
	m_output = NULL;
    }
    if (m_input) {
	delete m_input;
	m_input = NULL;
    }
    if (seek_mutex) {
	delete seek_mutex;
	seek_mutex = NULL;
    }
    if (bs_buffer) {
	delete bs_buffer;
	bs_buffer = NULL;
    }
    if (pcm_buffer) {
	delete pcm_buffer;
	pcm_buffer = NULL;
    }
}


void XingLMC::Stop() {
    //cout << "stopping..." << endl;
    if (decoderThread) {
	XingCommand *xc = new XingCommand[1];
	xc[0] = XING_Stop;
	xcqueue->write(xc);
	decoderThread->Join(); // wait for thread to exit
	delete decoderThread;
	decoderThread = NULL;
	//cout << "XingLMC deleted decoder thread.." << endl;
    }
}

bool XingLMC::Decode() {
    // kick off thread w/ DecodeWorkerThreadFunc(void *);
    if (!decoderThread) {
	decoderThread = new Thread();
	decoderThread->Create(XingLMC::DecodeWorkerThreadFunc,this);
    }

	return true;
}

THREAD_RETURN THREAD_LINKAGE XingLMC::DecodeWorkerThreadFunc(void *pxlmc) {
    if (pxlmc) {
	XingLMC *xlmc = (XingLMC *)pxlmc;
	xlmc->DecodeWork();
    }

	return 0;
}


#define PIECES 20
#if 1
#define CHECK_COMMAND   { while (!xcqueue->isEmpty()) { \
	XingCommand *xc = xcqueue->read(); \
	switch (*xc) { \
	    case XING_Stop: \
		return; \
		break; \
	    case XING_Pause: \
		m_output->Reset(true); \
		isPaused = true; \
		while (isPaused) { \
		    if (!xcqueue->isEmpty()) { \
			xc = xcqueue->read(); \
			switch (*xc) { \
			    case XING_Stop: \
				return; \
				break; \
			    case XING_Resume: \
				isPaused = false; \
				break; \
			    default: \
				break; \
			} \
		    } \
		    usleep(5000); \
		} \
		break; \
	    default: \
		break; \
	} \
    } \
}
#else
#define CHECK_COMMAND
#endif
void XingLMC::DecodeWork() {
    //cout << "XingLMC::Decode: Starting decode..." << endl;
    bool result = true;
    int32 in_bytes,out_bytes;
    int32 nwrite;
    in_bytes = out_bytes = 0;
    uint32 upper = 0;
    uint32 lower = 0;
    //uint32 oldupper;
    // cout.setf(ios::hex,ios::basefield);
    //cout.width(8);
    //cout.fill('0');

#if TEST_TIME
    __asm__ ("rdtsc" : "=d" (upper), "=a" (lower));
    oldupper = upper & 0x01;
    while (oldupper == (upper & 0x01)) {
	__asm__ ("rdtsc" : "=d" (upper), "=a" (lower));
    }
#endif // TEST_TIME

    for (int32 u = 0;;) {
//	cout << "I'm at: " << m_input->Seek(0,SEEK_FROM_CURRENT) << endl;
	if (u >= wait_n_times) {
	    double tpf = (double)1152 / (double)44100;
	    float totalTime = (float)((double)u * (double)tpf);
	    MediaTimePositionInfo *pmtpi = new MediaTimePositionInfo(totalTime,u);
	    Event *e = new Event(INFO_MediaTimePosition,pmtpi);
	    Player::getPlayer()->acceptEvent(*e);
	    delete e;
	}
	//cout << "Paused: " << isPaused << endl;
	CHECK_COMMAND;
	seek_mutex->Acquire(WAIT_FOREVER);
	if (!bs_fill()) break;
	if (bs_bufbytes < framebytes) break; // end of file
	IN_OUT x = audio_decode(bs_bufptr, (short *) (pcm_buffer + pcm_bufbytes));
	if (x.in_bytes <= 0) {
	    cout << "XingLMC: Bad sync in MPEG file" << endl;
	    // BAD SYNC IN MPEG FILE
	    break;
	}
	bs_bufptr += x.in_bytes;
	bs_bufbytes -= x.in_bytes;
	pcm_bufbytes += x.out_bytes;
	u++;
	int32 nwrite = 0;
//.byte 0x0f,0x31
	// should be rdtsc
	//__asm__ ("rdtsc" : "=d" (upper), "=a" (lower));
	//__asm__ (".byte 0x0f,0x31;movd %0,edx;movd %1,eax" :"=q" (upper), "=q" (lower) : /* no inputs */: "edx","eax");

	//cout << "Upper: " << upper << endl;
	//cout << "Lower: " << lower << endl;

	if (pcm_bufbytes > pcm_trigger) {
            #if __BYTE_ORDER != __LITTLE_ENDIAN
	    pcm_bufbytes = cvt_to_wave(pcm_buffer,pcm_bufbytes);
            #endif
	    

#if 0 // 0 = mini block write, 1 = total block write
	    nwrite = m_output->WriteThis(pcm_buffer,pcm_bufbytes);
#else
	    int32 writeBlockLength = pcm_bufbytes / PIECES;
	    int32 thisTime = 0;
	    void *pv = NULL; 
	    for (int i=0;i<PIECES;i++) {
		pv = (void *)((int32)pcm_buffer + (writeBlockLength*i));
		CHECK_COMMAND;
		if (u >= wait_n_times) 
		    thisTime = m_output->WriteThis(pv,writeBlockLength);
#if TEST_TIME
		__asm__ ("rdtsc" : "=d" (upper), "=a" (lower));
//    cout.width(8);
//    cout.fill('0');
//		cout << upper;
//    cout.width(8);
//    cout.fill('0');
		cout << lower << endl;
#endif // TEST_TIME
		nwrite += thisTime;
		if (u < wait_n_times) thisTime = writeBlockLength;
		if (thisTime != writeBlockLength) {
		    cout << "this time didn't work..." << endl;
		    break;
		}
	    }
	    int32 md = pcm_bufbytes % PIECES;
	    if ((nwrite == writeBlockLength*PIECES) && md) {
		pv = (void *)((int32)pcm_buffer + (writeBlockLength*PIECES));
		CHECK_COMMAND;
		if (u > wait_n_times) 
		    thisTime = m_output->WriteThis(pv,md);
#if TEST_TIME
		__asm__ ("rdtsc" : "=d" (upper), "=a" (lower));
//    cout.width(8);
//cout.fill('0');
//		cout << upper;
//    cout.width(8);
//    cout.fill('0');

		cout << lower << endl;
#endif // TEST_TIME
		nwrite += thisTime;
	    }
#endif    
	    if (u < wait_n_times) nwrite = (int32)pcm_bufbytes;
	    if (nwrite != (int32)pcm_bufbytes) {
		cout << "XingLMC: Write Error: bytes = " << nwrite << " pcmbufbytes: " << pcm_bufbytes << endl;
		// WRITE ERROR
		break;
	    }
	    out_bytes += pcm_bufbytes;
	    pcm_bufbytes = 0;
	}
	in_bytes += x.in_bytes;
	seek_mutex->Release();
    }
    if (pcm_bufbytes > 0) {  // write out last bit
        #if __BYTE_ORDER != __LITTLE_ENDIAN
	pcm_bufbytes = cvt_to_wave(pcm_buffer,pcm_bufbytes);
        #endif
	CHECK_COMMAND;
	nwrite = m_output->WriteThis(pcm_buffer,pcm_bufbytes);
	if (nwrite != (int32)pcm_bufbytes) {
	    cout << "XingLMC: Write Error 2" << endl;
	    // WRITE ERROR
	}
	out_bytes += pcm_bufbytes;
	pcm_bufbytes = 0;
    }
    Event *e = new Event(INFO_DoneOutputting);
    Player::getPlayer()->acceptEvent(*e);
    delete e;
    
    return;
}
void XingLMC::Pause() {
    XingCommand *xc = new XingCommand[1];
    xc[0] = XING_Pause;
    xcqueue->write(xc);
}

void XingLMC::Resume() {
    XingCommand *xc = new XingCommand[1];
    xc[0] = XING_Resume;
    xcqueue->write(xc);
}

void XingLMC::Reset() {

}


bool XingLMC::ChangePosition(int32 position) {
    bs_bufbytes = 0;
    bs_bufptr = bs_buffer;
    m_input->Seek(0,SEEK_FROM_START);
    
    wait_n_times = position;
    return true;
}

void XingLMC::bs_clear() {
}

int XingLMC::bs_fill() {
   unsigned int nread;

   if (bs_bufbytes < 0) {
      bs_bufbytes = 0;		// signed var could be negative
   }

   if (bs_bufbytes < bs_trigger) {
      memmove(bs_buffer, bs_bufptr, bs_bufbytes);
      //cout << "Reading from " << m_input->Seek(0,SEEK_FROM_CURRENT) << " to at most " << BS_BUFBYTES-bs_bufbytes << endl;
      nread = m_input->Read(bs_buffer + bs_bufbytes, BS_BUFBYTES - bs_bufbytes);
      //nread = read(handle, bs_buffer + bs_bufbytes, BS_BUFBYTES - bs_bufbytes);
      if ((nread + 1) == 0) {
         /*-- test for -1 = error --*/
	 bs_trigger = 0;
	 printf("\n FILE_READ_ERROR\n");
	 return 0;
      }
      bs_bufbytes += nread;
      bs_bufptr = bs_buffer;
   }

   return 1;
}

/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

        MP3 Decoder originally Copyright (C) 1995-1997 Xing Technology
        Corp.  http://www.xingtech.com

	Portions Copyright (C) 1998 GoodNoise
	Portions Copyright (C) 1998 "Michael Bruun Petersen" <mbp@image.dk>

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

/* system headers */
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>

/* project headers */
#include "config.h"
#include "xinglmc.h"
#include "event.h"
#include "eventdata.h"
#include "mutex.h"
#include "semaphore.h"

extern "C" {
int actually_decode;
}


static AUDIO audio_table[2][2] = {
    { // [0][]
	// non iteger mode
	{audio_decode_init, audio_decode_info, audio_decode},
	{audio_decode8_init, audio_decode8_info, audio_decode8}, // 8 bit methods
    },
    { // [1][]
	// integer mode
	{i_audio_decode_init, i_audio_decode_info, i_audio_decode},
	{audio_decode8_init, audio_decode8_info, audio_decode8},   // 8 bit methods
    }
};


#define SEND_EVENT(e) if (m_target) m_target->AcceptEvent(m_target, e);

void XingLMC::SetTarget(EventQueue *eq) {
    m_target = eq;
}

void XingLMC::SetPMI(PMIRef i) {
    m_input = i;
}

void XingLMC::SetPMO(PMORef o) {
    m_output = o;
}

void XingLMC::InitDecoder() {
    if (bs_fill()) {
	MPEG_HEAD head;
	int32 bitrate;
	// parse MPEG header
	m_frameBytes = head_info2(m_bsBuffer, m_bsBufBytes, &head, &bitrate);
	if (m_frameBytes == 0) {
	    // BAD OR UNSUPPORTED MPEG FILE!!!
	    cout << "XingLMC::XingLMC: head_info2 return 0 bytes/frame" << endl;
	    return;
	}

	// select decoder
	m_audioMethods = audio_table[0][0];  // not integer, non 8 bit mode

	{ // send track info (there is more info to be gotten, see towave.c : out_mpeg_info
	    static int sr_table[8] =
	    {22050L, 24000L, 16000L, 1L,
	     44100L, 48000L, 32000L, 1L};

	    int32 totalFrames = 0;
	    int32 bytesPerFrame = m_frameBytes;
	    int32 backhere = m_input->Seek(m_input,0,SEEK_FROM_CURRENT);
//	    cout << "Back here: " << backhere << endl;
	    int32 end = m_input->Seek(m_input,0,SEEK_FROM_END);
	    m_input->Seek(m_input, -128,SEEK_FROM_CURRENT);
	    // look for id3 tag
	    char buf[128];
	    memset(buf,0,sizeof(buf));
	    m_input->Read(m_input,buf,128);
	    Id3TagInfo tag_info(buf);
	    if (tag_info.m_containsInfo) {
		end -= 128;
	    }
	    m_input->Seek(m_input,backhere,SEEK_FROM_START);
	    totalFrames = end / bytesPerFrame;

	    double tpf = (double)1152 / (double)(44100 << 0);
	    float totalTime = (float)((double)totalFrames * (double)tpf);

	    char *pbitrate = new char[64];
	    char *psamprate = new char[64];
	    sprintf(pbitrate,"%d kbits/s",bitrate/1000);
	    int32 samprate = sr_table[4*head.id+head.sr_index];
	    if ((head.sync & 1) == 0) samprate = samprate / 2;
	    sprintf(psamprate,"%d Hz",samprate);
	    MediaVitalInfo *mvi = new MediaVitalInfo(m_input->Url(m_input),
                                                 m_input->Url(m_input),
                                                 totalFrames,
                                                 bytesPerFrame,
                                                 bitrate,samprate,
                                                 totalTime, 
                                                 tag_info);

	    Event *e = new Event(INFO_MediaVitalStats,mvi);
	    SEND_EVENT(e);
	}
	
	m_pcmBuffer = new unsigned char[PCM_BUFBYTES];
	m_pcmTrigger = (PCM_BUFBYTES - 2500 * sizeof(short));
	m_pcmBufBytes = 0;
	
	if (m_audioMethods.decode_init(  &head,
					 m_frameBytes,
					 0 /* reduction code */,
					 0 /* transform code (ignored) */,
					 0 /* convert code */, 
					 24000 /* freq limit */)) {

#if __BYTE_ORDER != __LITTLE_ENDIAN
	    cvt_to_wave_init(decinfo.bits);
#endif
	    DEC_INFO decinfo;
	    m_audioMethods.decode_info(&decinfo);
	    OutputInfo info;
	    cout << "bits: " << decinfo.bits << endl;
	    cout << "chans: " << decinfo.channels << endl;
	    cout << "samprate: " << decinfo.samprate << endl;
	    info.bits_per_sample = decinfo.bits;
	    info.number_of_channels = decinfo.channels;
	    info.samples_per_second = decinfo.samprate;
	    info.max_buffer_size = (info.number_of_channels * 2 * 1152) << 5;
	    m_output->Init(m_output, &info);
	} else {
	    cout << "Couldn't init decoder..." << endl;
	    return;
	}
    } else {
	cout << "Couldn't fill the manure pile..." << endl;
	return;
    }
}

XingLMC::
//XingLMC(Input* input, Output* output, EventCallback callback, void* cookie)
XingLMC()
{
    //cout << "XingLMC::XingLMC: Creating XingLMC..." << endl;
    m_target = NULL;
    m_decoderThread = NULL;
    m_xcqueue = new Queue<XingCommand *>(false); 
    m_seekMutex = new Mutex();
    m_pauseSemaphore = new Semaphore();
    m_bsBufBytes = 0;
    m_bsTrigger = 2500;
    m_bsBuffer = NULL;
    m_bsBufPtr = m_bsBuffer;
    m_bsBuffer = new unsigned char[BS_BUFBYTES];

    m_frameCounter = 0;
    m_frameWaitTill = 0;
    actually_decode = 0;
}

XingLMC::~XingLMC() {
    Stop();
    if (m_xcqueue) {
	delete m_xcqueue;
	m_xcqueue = NULL;
    }
    if (m_output) {
    m_output->Cleanup(m_output);
	delete m_output;
	m_output = NULL;
    }
    if (m_input) {
    m_input->Cleanup(m_input);
	delete m_input;
	m_input = NULL;
    }
    if (m_pauseSemaphore) {
	delete m_pauseSemaphore;
	m_pauseSemaphore = NULL;
    }
    if (m_seekMutex) {
	delete m_seekMutex;
	m_seekMutex = NULL;
    }
    if (m_bsBuffer) {
	delete m_bsBuffer;
	m_bsBuffer = NULL;
    }
    if (m_pcmBuffer) {
	delete m_pcmBuffer;
	m_pcmBuffer = NULL;
    }

    // NOTE: unlike the other refs (PMIRef, PMORef, UIRef) we do not call a 
    // cleanup function to delete the internal ref since it is the player
    if (m_target) {
	delete m_target;
	m_target = NULL;
    }
}


void XingLMC::Stop() {
    //cout << "stopping..." << endl;
    if (m_decoderThread) {
	XingCommand *xc = new XingCommand[1];
	xc[0] = XING_Stop;
	m_xcqueue->Write(xc);
	m_pauseSemaphore->Signal();
	m_decoderThread->Join(); // wait for thread to exit
	delete m_decoderThread;
	m_decoderThread = NULL;
	m_output->Reset(m_output, true);
	//cout << "XingLMC deleted decoder thread.." << endl;
    }
}

bool XingLMC::Decode() {
    // kick off thread w/ DecodeWorkerThreadFunc(void *);
    if (!m_decoderThread) {
        m_decoderThread = Thread::CreateThread();
	m_decoderThread->Create(XingLMC::DecodeWorkerThreadFunc,this);
    }
    
    return true;
}

void XingLMC::DecodeWorkerThreadFunc(void *pxlmc) {
    if (pxlmc) {
	XingLMC *xlmc = (XingLMC *)pxlmc;
	xlmc->DecodeWork();
    }
}


void XingLMC::DecodeWork() {
    //cout << "XingLMC::Decode: Starting decode..." << endl;
    bool result = true;
    int32 in_bytes,out_bytes;
    int32 nwrite;
    in_bytes = out_bytes = 0;


    for (m_frameCounter = 0;;) {
	if (m_frameCounter >= m_frameWaitTill) {
	    actually_decode = 1;
	}
	if (actually_decode) {
	    double tpf = (double)1152 / (double)44100;
	    float totalTime = (float)((double)m_frameCounter * (double)tpf);
	    MediaTimePositionInfo *pmtpi = new MediaTimePositionInfo(totalTime,m_frameCounter);
	    Event *e = new Event(INFO_MediaTimePosition,pmtpi);
	    SEND_EVENT(e);
	}
	
	while (!m_xcqueue->IsEmpty()) {
	    XingCommand *xc = m_xcqueue->Read();
	    switch (*xc) {
		case XING_Stop: return;
		case XING_Pause:
		    m_output->Reset(m_output, true);
		    m_pauseSemaphore->Wait();
		    break;
		default: break;
	    }
	}

	m_seekMutex->Acquire(WAIT_FOREVER);
	if (!bs_fill()) break;
	if (m_bsBufBytes < m_frameBytes) break; // end of file
	IN_OUT x = m_audioMethods.decode(m_bsBufPtr, (short *) (m_pcmBuffer + m_pcmBufBytes));
	if (x.in_bytes <= 0) {
	    cout << "XingLMC: Bad sync in MPEG file" << endl;
	    // BAD SYNC IN MPEG FILE
	    break;
	}
	m_bsBufPtr += x.in_bytes;
	m_bsBufBytes -= x.in_bytes;
	m_pcmBufBytes += x.out_bytes;
	m_frameCounter++;
	int32 nwrite = 0;

	if (m_pcmBufBytes > m_pcmTrigger) {
            #if __BYTE_ORDER != __LITTLE_ENDIAN
	    m_pcmBufBytes = cvt_to_wave(m_pcmBuffer,m_pcmBufBytes);
            #endif
	    if (actually_decode) {
		nwrite = m_output->Write(m_output, m_pcmBuffer,m_pcmBufBytes);
		
		if (nwrite != (int32)m_pcmBufBytes) {
		    cout << "XingLMC: Write Error: bytes = " << nwrite << " pcmbufbytes: " << m_pcmBufBytes << endl;
		    // WRITE ERROR
		    break;
		}
	    }
	    out_bytes += m_pcmBufBytes;
	    m_pcmBufBytes = 0;
	}
	in_bytes += x.in_bytes;
	m_seekMutex->Release();
    }
    if (m_pcmBufBytes > 0) {  // Write out last bit
        #if __BYTE_ORDER != __LITTLE_ENDIAN
	m_pcmBufBytes = cvt_to_wave(m_pcmBuffer,m_pcmBufBytes);
        #endif
	nwrite = m_output->Write(m_output, m_pcmBuffer,m_pcmBufBytes);
	if (nwrite != (int32)m_pcmBufBytes) {
	    cout << "XingLMC: Write Error 2" << endl;
	    // WRITE ERROR
	}
	out_bytes += m_pcmBufBytes;
	m_pcmBufBytes = 0;
    }
    Event *e = new Event(INFO_DoneOutputting);
    SEND_EVENT(e);
    
    return;
}
void XingLMC::Pause() {
    m_output->Pause(m_output);
    XingCommand *xc = new XingCommand[1];
    xc[0] = XING_Pause;
    m_xcqueue->Write(xc);
}

void XingLMC::Resume() {
    m_output->Resume(m_output);
    m_pauseSemaphore->Signal();
}

void XingLMC::Reset() {

}

bool XingLMC::ChangePosition(int32 position) {
    m_seekMutex->Acquire(WAIT_FOREVER);
    //cout << "Seeking to ..." << position << endl;
#if 1
    m_bsBufBytes = 0;
    m_bsBufPtr = m_bsBuffer;
    m_input->Seek(m_input, 0,SEEK_FROM_START);
    m_frameCounter = 0;
    m_frameWaitTill = position;
    actually_decode = 0;
#else
    m_input->Seek(m_input,(417*position)-50,SEEK_FROM_START);
    m_bsBufBytes = 0;
    m_bsBufPtr = m_bsBuffer;
    bs_fill();

    unsigned char buf[1024];
    int pBuf = 0;
    while ((m_bsBufPtr[pBuf] != 0xFF) && ((m_bsBufPtr[pBuf+1] & 0xF0) != 0xF0)) pBuf++;
    cout << "pBuf = " << pBuf << endl;
    m_bsBufPtr = &(m_bsBufPtr[pBuf]);

#endif
    m_seekMutex->Release();

    return true;
}

void XingLMC::bs_clear() {
}

int XingLMC::bs_fill() {
   unsigned int nread;

   if (m_bsBufBytes < 0) {
      m_bsBufBytes = 0;		// signed var could be negative
   }

   if (m_bsBufBytes < m_bsTrigger) {
      memmove(m_bsBuffer, m_bsBufPtr, m_bsBufBytes);
      //cout << "Reading from " << m_input->Seek(m_input, 0,SEEK_FROM_CURRENT) << " to at most " << BS_BUFBYTES-m_bsBufBytes << endl;
      nread = m_input->Read(m_input, m_bsBuffer + m_bsBufBytes, BS_BUFBYTES - m_bsBufBytes);
      //nread = Read(handle, m_bsBuffer + m_bsBufBytes, BS_BUFBYTES - m_bsBufBytes);
      if ((nread + 1) == 0) {
         /*-- test for -1 = error --*/
	 m_bsTrigger = 0;
	 printf("\n FILE_READ_ERROR\n");
	 return 0;
      }
      m_bsBufBytes += nread;
      m_bsBufPtr = m_bsBuffer;
   }

   return 1;
}

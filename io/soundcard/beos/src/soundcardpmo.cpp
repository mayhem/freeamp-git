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


/* system headers */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <iostream.h>
#include <media/SoundPlayer.h>
#include <support/Debug.h>

/* project headers */
#include <config.h>
#include "soundcardpmo.h"
#include "soundutils.h"
#include "ringbuffer.h"
#include "thread.h"

const uint32 PCMBUFFERSIZE = 8 * 4096;
const int32 BUFFER_SIZE		= 65536;
const int32 OVERFLOW_SIZE	= 9216;
const int32 TRIGGER_SIZE	= 9216;

extern "C" {
PhysicalMediaOutput* Initialize(FAContext *context)
{
	return new SoundCardPMO(context);
}
} /* extern "C" */

static char * g_ErrorArray[8] = {
	"Invalid Error Code",
	"dsp device open failed",
	"fcntl F_GETFL on /dev/dsp failed",
	"fcntl F_SETFL on /dev/dsp failed"
	"ioctl reset failed",
	"config of samplesize failed",
	"config of stereo failed",
	"config of speed failed"
};

const char *
SoundCardPMO::GetErrorString( int32 error )
{
	if ((error <= pmoError_MinimumError) || (error >= pmoError_MaximumError))
	{
		return g_ErrorArray[0];
	}
	return g_ErrorArray[error - pmoError_MinimumError];
}

SoundCardPMO::SoundCardPMO(FAContext *context)
:	m_buffer( NULL ),
	m_player( NULL ),
	m_pcmBuffer( NULL )
{
    m_context = context;
	m_buffer    = new RingBuffer( BUFFER_SIZE );
//	m_player    = new BSoundPlayer( "Freeamp", _PlayerHook, NULL, this );
	m_pcmBuffer = new int16[ PCMBUFFERSIZE ];
	m_properlyInitialized = false;
	myInfo = new OutputInfo();
}

SoundCardPMO::~SoundCardPMO()
{
	if ( myInfo )
	{
		delete myInfo;
		myInfo = NULL;
	}

	if ( m_buffer )
	{
		delete m_buffer;
		m_buffer = NULL;
	}

	if ( m_player )
	{
		delete m_player;
		m_player = NULL;
	}

	if ( m_pcmBuffer )
	{
		delete[] m_pcmBuffer;
		m_pcmBuffer = NULL;
	}

	PRINT(( "SoundCardPMO::!SoundCardPMO done\n" ));
}

Error
SoundCardPMO::Pause()
{
	return Reset( true );
}

Error
SoundCardPMO::Resume()
{
	return kError_NoErr;
}

Error
SoundCardPMO::Init( OutputInfo* info )
{
	cout << "initialize..." << endl;
	cout << "OI: bits_per_sample: " << info->bits_per_sample << endl;
	cout << "OI: number_of_channels: " << info->number_of_channels << endl;
	cout << "OI: samples_per_second: " << info->samples_per_second << endl;
	cout << "OI: max_buffer_size: " << info->max_buffer_size << endl;

	m_properlyInitialized = false;
	if ( !info )
	{
		info = myInfo;
	}
	else
	{
		// got info, so this is the beginning...
	}
	
	myInfo->bits_per_sample = info->bits_per_sample;
	myInfo->number_of_channels = info->number_of_channels;
	myInfo->samples_per_second = info->samples_per_second;
	myInfo->max_buffer_size = info->max_buffer_size;

	if ( ::get_audio_format( myInfo, &m_format ) < B_NO_ERROR )
	{
		printf( "error in output info\n" );
	}
	if ( m_player )
	{
		delete m_player;
	}
	m_player = new BSoundPlayer( &m_format, "FreeAmp", _PlayerHook, NULL, this );

	printf( "Frame Rate : %f\n", m_format.frame_rate );
	printf( "Format : %x\n", m_format.format );
	printf( "Channel Count : %d\n", m_format.channel_count );

#if USE_DUMMY_PLAYER
	m_dummyPlayerThread = Thread::CreateThread();
	m_dummyPlayerThread->Create( _DummyPlayerHook, this );
#else
	m_player->Start();
	m_player->SetHasData( true );
#endif

	m_properlyInitialized = true;

	return kError_NoErr;
}

Error
SoundCardPMO::Reset( bool user_stop )
{
	cout << "Reset..." << endl;

	m_player->SetHasData( false );
	m_player->Stop();

	if ( user_stop )
	{
	}
	else
	{
	}

	return kError_NoErr;
}

Error
SoundCardPMO::Write( int32& rtn, void* inBuf, int32 inLength )
{
	size_t	bytesWritten;
	size_t	bytesToWrite;
	void*	bufHead;
	Error	error;

#if 0
	if ( !m_player->HasData() )
	{
		m_player->Start();
		m_player->SetHasData( true );
	}
#endif

	bytesWritten = m_buffer->Write( inBuf, inLength );
	PRINT(( "%d bytes written\n", bytesWritten ));
	if ( bytesWritten != inLength )
	{
	}

	return kError_NoErr;
}

void
SoundCardPMO::_DummyPlayerHook( void* arg )
{
	return ((SoundCardPMO*)arg)->DummyPlayer();
}

void
SoundCardPMO::DummyPlayer( void )
{
	bool	done = false;
	size_t	bytesRead;
	uint8	buf[ 4096 ];

	while ( !done )
	{
		bytesRead = m_buffer->Read( buf, 4096 );
		PRINT(( "Dummy: %d bytes read\n", bytesRead ));
		if ( bytesRead == 0 )
		{
			done = true;
			PRINT(( "Dummy: done\n" ));
		}
		snooze( 10000 );
	}
}

void
SoundCardPMO::_PlayerHook(
				void*							cookie,
				void*							buffer,
				size_t							size,
				const media_raw_audio_format&	format
				)
{
	((SoundCardPMO*)cookie)->Player( buffer, size, format );
}

void
SoundCardPMO::Player(
				void*							buffer,
				size_t							size,
				const media_raw_audio_format&	format
				)
{
	PRINT(( "%d bytes reading %f Hz\n", size, format.frame_rate ));
	float*	buf = (float*)buffer;
	size_t	floatSize = size / 4;
	uint32	channelCount = format.channel_count;
	uint32	numSamples = floatSize;

	if ( format.format != m_format.format )
	{
		printf( "invalid format\n" );
		return;
	}

	size_t	bytesToRead = numSamples * channelCount;
	size_t	bytesRead;
	bytesRead = m_buffer->Read( (void*)m_pcmBuffer, bytesToRead, false );
	if ( bytesRead < bytesToRead )
	{
		PRINT(( "only %d bytes read\n", bytesRead ));
		memset( (m_pcmBuffer + bytesRead), 0, (bytesToRead - bytesRead) );
	}

	float*	destPtr = buf;
	int16*	srcPtr = m_pcmBuffer;
	for ( int i = 0; i < numSamples; i++ )
	{
		*destPtr++ = ( (float)(*srcPtr++) ) / 32767.0;
	}
}

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

#define DEBUG 0

/* system headers */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <iostream.h>

/* BeOS Kits */
#include <media/SoundPlayer.h>
#include <support/Debug.h>

/* project headers */
#include <config.h>
#include "soundcardpmo.h"
#include "soundutils.h"
#include "thread.h"
#include "mutex.h"
#include "facontext.h"
#include "log.h"

LogFile*	g_Log;

int32 SoundCardPMO::s_lastVolume = 100;

extern "C" {

PhysicalMediaOutput* Initialize(FAContext *context)
{
	return new SoundCardPMO(context);
}

} /* extern "C" */

SoundCardPMO::SoundCardPMO( FAContext* context )
:	PhysicalMediaOutput( context ),
	m_player( NULL ),
	m_dataSize( 0 ),
	m_eventSem( "EventSem" ),
	m_pauseLock( "pause lock" ),
    m_bytesPerSample( 0 ),
    m_lastFrame( -1 ),
    m_totalBytesWritten( 0 )
{
	PRINT(( "SoundCardPMO::ctor\n" ));
	m_properlyInitialized = false;

	m_outputInfo = new OutputInfo();

	m_context = context;

	m_eventBufferThread = Thread::CreateThread();
	m_eventBufferThread->Create( _EventBufferThreadHook, this );
//    m_eventBufferThread->SetPriority( B_REAL_TIME_PRIORITY ); // FIXME: don't use beos native prio
}

SoundCardPMO::~SoundCardPMO()
{
	Lock();

	PRINT(( "SoundCardPMO::dtor\n" ));

	m_bExit = true;
	m_pSleepSem->Signal();
	m_pPauseSem->Signal();
	m_pauseLock.Unlock();

	PRINT(( "SoundCardPMO::~SoundCardPMO\n" ));
	if ( m_player )
	{
		PRINT(( "SoundCardPMO::~SoundCardPMO:stopping\n" ));
		m_player->SetHasData( false );
		m_player->Stop();	// blocks till it's done
		PRINT(( "SoundCardPMO::~SoundCardPMO:stopped\n" ));

		delete m_player;
		m_player = NULL;
	}

	if ( m_outputInfo )
	{
		delete m_outputInfo;
		m_outputInfo = NULL;
	}

	if ( m_eventBufferThread )
	{
		delete m_eventBufferThread;
		m_eventBufferThread = NULL;
	}

#if DEBUG_SAVE_PCM
	fclose( m_pcmSaveFile );
#endif
	PRINT(( "SoundCardPMO::~SoundCardPMO done\n" ));
}

int32
SoundCardPMO::GetPrefInt32(kVolumePref,  void )
{
	PRINT(( "SoundCardPMO::GetVolume\n" ));
    if ( !m_player )
	{
		return s_lastVolume;
	}
	int32 volume = int32( m_player->Volume() * 100.0 );
	return volume;
}

void
SoundCardPMO::SetPrefInt32(kVolumePref,  int32 volume )
{
	PRINT(( "SoundCardPMO::SetVolume\n" ));
    if ( m_player )
    {
        m_player->SetPrefInt32(kVolumePref,  float( volume ) / 100.0 );
    }
	s_lastVolume = volume;
}

void
SoundCardPMO::Pause( void )
{
	PRINT(( "SoundCardPMO::Pause\n" ));
	PhysicalMediaOutput::Pause();
	m_pauseLock.Lock();

	if ( m_player )
	{
		m_player->SetHasData( false );
		m_player->Stop();
	}

	if ( false && m_properlyInitialized )
	{
		Reset( true );
	}
}

void
SoundCardPMO::Resume( void )
{
	PRINT(( "SoundCardPMO::Resume\n" ));
	PhysicalMediaOutput::Resume();
	m_pauseLock.Unlock();

	if ( m_player )
	{
		m_player->Start();
		m_player->SetHasData( true );
	}
	PRINT(( "SoundCardPMO::Resume done\n" ));
}

bool
SoundCardPMO::WaitForDrain( void )
{
	PRINT(( "SoundCardPMO::WaitForDrain\n" ));
#if 0
	if ( m_eventBufferThread )
	{
		m_eventBufferThread->Join();
		delete m_eventBufferThread;
		m_eventBufferThread = NULL;
	}
	PRINT(( "SoundCardPMO::WaitForDrain:event buffer terminated\n" ));
#endif

	if ( !m_player )
	{
		PRINT(( "SoundCardPMO::WaitForDrain:no BSoundPlayer exists?!\n" ));
		return false;
	}

	m_player->SetHasData( false );
	m_player->Stop();
	PRINT(( "SoundCardPMO::WaitForDrain:sound player terminated\n" ));

	return true;
}

Error
SoundCardPMO::Init( OutputInfo* info )
{
    Lock();

	PRINT(( "SoundCardPMO::Init\n" ));
	PRINT(( "OI: bits_per_sample: %d\n", info->bits_per_sample ));
	PRINT(( "OI: number_of_channels: %d\n", info->number_of_channels ));
	PRINT(( "OI: samples_per_second: %d\n", info->samples_per_second ));
	PRINT(( "OI: max_buffer_size: %d\n", info->max_buffer_size ));

	m_properlyInitialized = false;
	if ( !info )
	{
		info = m_outputInfo;
	}
	else
	{
		// got info, so this is the beginning..
		m_dataSize = info->max_buffer_size;
	}

	m_outputInfo->bits_per_sample = info->bits_per_sample;
	m_outputInfo->number_of_channels = info->number_of_channels;
	m_outputInfo->samples_per_second = info->samples_per_second;
	m_outputInfo->max_buffer_size = info->max_buffer_size;

    m_bytesPerSample = info->number_of_channels * ( info->bits_per_sample / 8 );

	if ( ::get_audio_format( m_outputInfo, &m_format ) < B_NO_ERROR )
	{
		printf( "error in output info\n" );
	}

	if ( m_player )
	{
		delete m_player;
	}

	m_player = new BSoundPlayer(
							&m_format,
							BRANDING,
							_PlayerHook,
							_NotifierHook,
							this
							);

	if ( m_player->InitCheck() < B_NO_ERROR )
	{
		PRINT(( "SoundPlayerPMO::Init:error constructing BSoundPlayer\n" ));
		return kError_InitFailed;
	}

	PRINT(( "Frame Rate : %f\n", m_format.frame_rate ));
	PRINT(( "Format : %x\n", (int)m_format.format ));
	PRINT(( "Channel Count : %d\n", (int)m_format.channel_count ));

#if USE_DUMMY_PLAYER
	m_dummyPlayerThread = Thread::CreateThread();
	m_dummyPlayerThread->Create( _DummyPlayerHook, this );
#else
    m_player->SetPrefInt32(kVolumePref,  float( s_lastVolume ) / 100.0 );
	m_player->Start();
	m_player->SetHasData( true );
#endif

#if DEBUG_SAVE_PCM
	m_pcmSaveFile = fopen( "freeamp.pcmsave", "w" );
#endif

	m_properlyInitialized = true;

    Unlock();

	return kError_NoErr;
}

Error
SoundCardPMO::Reset( bool user_stop )
{
	PRINT(( "SoundCardPMO::Reset...\n" ));

#if USE_DUMMY_PLAYER
#else
	if ( !m_player ) return kError_YouScrewedUp;

	if ( m_player )
	{
		m_player->SetHasData( false );
		PRINT(( "SetHasData done\n" ));
		m_player->Stop( false );
		PRINT(( "player stop done\n" ));
	}

	if ( false &&  m_eventBufferThread )
	{
		m_eventSem.Signal();
		m_eventBufferThread->Join();
	}

	if ( user_stop )
	{
	}
	else
	{
	}
#endif

	return kError_NoErr;
}

void
SoundCardPMO::HandleTimeInfoEvent( PMOTimeInfoEvent* event )
{
    int32 hours, minutes, seconds, total;
    MediaTimeInfoEvent* mtie;

    if ( !m_player ) return;

    if ( event->GetFrameNumber() != m_lastFrame + 1 )
    {
        m_totalBytesWritten = 1152 * event->GetFrameNumber() * m_bytesPerSample;
    }
    m_lastFrame = event->GetFrameNumber();

    if ( m_outputInfo->samples_per_second <= 0 || event->GetFrameNumber() < 2 )
    {
        return;
    }

    total = m_totalBytesWritten /
            ( m_bytesPerSample * m_outputInfo->samples_per_second );
    total %= 86400;

    hours = total / 3600;
    minutes = (total / 60) % 60;
    seconds = total % 60;
    mtie = new MediaTimeInfoEvent( hours, minutes, seconds, 0, total, 0 );
    m_pTarget->AcceptEvent( mtie );
}

void
SoundCardPMO::_EventBufferThreadHook( void* arg )
{
	return ((SoundCardPMO*)arg)->EventBufferThread();
}

void
SoundCardPMO::EventBufferThread( void )
{
	PRINT(( "SoundCardPMO::EventBufferThread:enter\n" ));

	// Don't do anything until resume is called.
	m_pPauseSem->Wait();

	// Prebuffer wait
	PRINT(( "SoundCardPMO::EventBufferThread: prebuffering...\n" ));
	PreBuffer();

	PRINT(( "SoundCardPMO::EventBufferThread: okay now ready to spin!\n" ));

	// don't know why this is necessary, but m_pInputBuffer is
	// still null sometimes.
	EventBuffer* eb = NULL;
	while ( !eb )
	{
		if ( m_bExit ) return;
		eb = dynamic_cast<EventBuffer*>( m_pInputBuffer );
		WasteTime();
	}

	// Handles event.
	while ( !m_bExit )
	{

		// First, wait for the PMO_Init event to propagate.
		if ( !m_properlyInitialized )
		{
			PRINT(( "SoundCardPMO::EventBufferThread:fetching possible init event\n" ));
			Event* event = eb->GetEvent();
			if ( event == NULL )
			{
				m_pLmc->Wake();
				WasteTime();
				continue;
			}

			if ( event->Type() == PMO_Init )
			{
				if ( IsError( Init( ((PMOInitEvent*)event)->GetInfo() ) ) )
				{
					delete event;
					break;
				}
			}
			delete event;

			continue;
		}

		// Now, work hand in hand with the Player method, dealing with
		// events as requested by the Player method.
		m_eventSem.Wait();
//		Event*	event = eb->GetEvent();
		if ( m_event != NULL )
		{
			if ( m_event->Type() == PMO_Init )
			{
				PRINT(( "SoundCardPMO::EventBufferThread:PMO_Init recv'd\n" ));
			}
			else if ( m_event->Type() == PMO_Reset )
			{
				PRINT(( "SoundCardPMO::EventBufferThread:PMO_Reset recv'd\n" ));
			}
			else if ( m_event->Type() == PMO_Info )
			{
				//PRINT(( "SoundCardPMO::EventBufferThread:PMO_Info recv'd\n" ));
                HandleTimeInfoEvent( (PMOTimeInfoEvent*)m_event );
			}
			else if ( m_event->Type() == PMO_Quit )
			{
				PRINT(( "SoundCardPMO::EventBufferThread:PMO_Quit recv'd\n" ));
				delete m_event;
                m_event = NULL;
				m_eventSem.Signal();
				if ( WaitForDrain() )
				{
					m_pTarget->AcceptEvent( new Event( INFO_DoneOutputting ) );
				}
				return;
			}
			delete m_event;
            m_event = NULL;
		}
//		m_eventSem.Signal();	// tell Player I'm done with the event.
	}
	PRINT(( "SoundCardPMO::EventBufferThread:exiting\n" ));
}

void
SoundCardPMO::_DummyPlayerHook( void* arg )
{
	return ((SoundCardPMO*)arg)->DummyPlayer();
}

void
SoundCardPMO::DummyPlayer( void )
{
	void*			bufferIn;
	size_t			bytesToCopy = m_dataSize;
	size_t			bytesCopied = 0;
	Error			err;
	EventBuffer*	eb = (EventBuffer*)m_pInputBuffer;

	while ( !m_bExit )
	{
		m_pauseLock.Lock();

		err = eb->BeginRead( bufferIn, bytesToCopy );
		if ( err == kError_EventPending )
		{
			Event* event = eb->GetEvent();
			delete event;
		}
		else
		{
			if ( IsError( err ) )
			{
				PRINT(( "SoundCardPMO::DummyPlayer: read failed(%d)\n", err ));
				bytesCopied = 0;
			}
			else
			{
				bytesCopied = bytesToCopy;
			}
			eb->EndRead( bytesCopied );
		}
		m_pauseLock.Unlock();
		PRINT(( "FIXME: %d bytes read\n", bytesCopied ));
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

// this function is a mess. evil, i know.
void
SoundCardPMO::Player(
				void*							bufferOut,
				size_t							size,
				const media_raw_audio_format&	format
				)
{
	Error	err;
	size_t	bytesToCopy;
	size_t	bytesCopied;
	void*	bufferIn;

	if ( format.format != m_format.format )
	{
		PRINT(( "SoundCardPMO::Player:invalid format\n" ));
		return;
	}

	bytesToCopy = size;

	if ( m_pauseLock.LockWithTimeout( 0.1e6 ) != B_OK )
	{
		return;
	}

	EventBuffer*	eb = (EventBuffer*)m_pInputBuffer;
	if ( !eb )
	{
		PRINT(( "SoundCardPMO::Player:null m_pInputBuffer\n" ));
		return;
	}

	for ( int retry = 0; retry < 10; retry++ )
	{
		bytesToCopy = size;
		err = eb->BeginRead( bufferIn, bytesToCopy );
		if ( IsntError( err ) )
		{
			break;
		}

		if ( err == kError_EventPending )
		{
            m_event = eb->GetEvent();
            m_eventSem.Signal();
#if 0
			Event*	event = eb->PeekEvent();
			switch ( event->Type() )
			{
			case PMO_Init:
				PRINT(( "SoundCardPMO::Player: PMO_Init recv'd\n" ));
				m_eventSem.Signal();
				m_eventSem.Wait();
				break;
			case PMO_Reset:
				PRINT(( "SoundCardPMO::Player: PMO_Reset recv'd\n" ));
				m_eventSem.Signal();
				m_eventSem.Wait();
				break;
			case PMO_Info:
				//PRINT(( "SoundCardPMO::Player: PMO_Info recv'd\n" ));
				event = eb->GetEvent();
                HandleTimeInfoEvent( (PMOTimeInfoEvent*)event );
				delete event;
				break;
			case PMO_Quit:
				PRINT(( "SoundCardPMO::Player: PMO_Quit recv'd\n" ));
				m_eventSem.Signal();
				m_eventSem.Wait();
				break;
			}
#endif
		}
		else if ( err == kError_NoDataAvail )
		{
			PRINT(( "SoundCardPMO::Player: killing some time to wait for data to pour in\n" ));
			m_pLmc->Wake();
			WasteTime();
		}
	}

	if ( IsError( err ) )
	{
		PRINT(( "SoundCardPMO::Player: error reading from the buffer(%d)\n", err ));
		bytesCopied = 0;
	}
	else
	{
		if ( bytesToCopy < size )
		{
			PRINT(( "SoundCardPMO::Player: end of data?\n" ));
		}
		memcpy( bufferOut, bufferIn, bytesToCopy );
#if DEBUG_SAVE_PCM
		fwrite( bufferIn, 1, bytesToCopy, m_pcmSaveFile );
#endif
		bytesCopied = bytesToCopy;
	}
	eb->EndRead( bytesCopied );
    m_totalBytesWritten += bytesCopied;

	m_pauseLock.Unlock();
    m_pLmc->Wake();
    UpdateBufferStatus();

	return;
}

void
SoundCardPMO::_NotifierHook(
				void*						cookie,
				BSoundPlayer::sound_player_notification
											what,
				...
				)
{
	((SoundCardPMO*)cookie)->Notifier( what );
}

void
SoundCardPMO::Notifier( BSoundPlayer::sound_player_notification what )
{
	switch ( what )
	{
	case BSoundPlayer::B_STARTED:
		PRINT(( "Notifier: SoundPlayer started\n" ));
		break;
	case BSoundPlayer::B_STOPPED:
		PRINT(( "Notifier: SoundPlayer stoped\n" ));
		break;
	case BSoundPlayer::B_SOUND_DONE:
		PRINT(( "Notifier: SoundPlayer sound done\n" ));
		break;
	default:
		PRINT(( "Notifier: unknown event is reported\n" ));
		break;
	}
}
// vi: set ts=4:

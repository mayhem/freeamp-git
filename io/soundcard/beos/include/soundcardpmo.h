/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player
	Portions copyright (C) 1998-1999 EMusic.com

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


#ifndef INCLUDED_SOUNDCARDPMO_H_
#define INCLUDED_SOUNDCARDPMO_H_

/* system headers */
#include <stdlib.h>
#include <stdio.h>

/* BeOS Kits */
#include <media/MediaDefs.h>
#include <media/SoundPlayer.h>

/* project headers */
#include <config.h>
#include "mutex.h"
#include "pmo.h"
#include "pmoevent.h"
#include "pullbuffer.h"
#include "eventbuffer.h"

#define USE_DUMMY_PLAYER 0
#define DEBUG_SAVE_PCM 0

class Thread;
class FAContext;
class VolumeManager;

class SoundCardPMO : public PhysicalMediaOutput
{
public:
							SoundCardPMO( FAContext* context );
	virtual					~SoundCardPMO();
	virtual Error			Init( OutputInfo* info );
	virtual void			GetVolume( int32& left, int32& right );
	virtual void			SetVolume( int32 left, int32 right );
	virtual void			Pause( void );
	virtual void 			Resume( void );

protected:
	void					Lock( void );
	void					Unlock( void );
	FAContext *				m_context;

private:
	virtual Error			Reset( bool user_stop );
	void					HandleTimeInfoEvent( PMOTimeInfoEvent* pEvent );
	bool					WaitForDrain( void );

	static void				_EventBufferThreadHook( void* arg );
	void					EventBufferThread( void );

	static void				_DummyPlayerHook( void* arg );
	void					DummyPlayer( void );

	static void				_PlayerHook(
								void*							cookie,
								void*							buffer,
								size_t							size,
								const media_raw_audio_format&	format
								);
	void					Player(
								void*							buffer,
								size_t							size,
								const media_raw_audio_format&	format
								);
	static void				_NotifierHook(
								void*							cookie,
								BSoundPlayer::sound_player_notification
																what,
								...
								);
	void					Notifier(
								BSoundPlayer::sound_player_notification
																what
								);

	Thread*					m_eventBufferThread;
	Thread*					m_dummyPlayerThread;
	Properties *			m_propManager;
	bool					m_properlyInitialized;
	uint32					channels;
	OutputInfo *			m_outputInfo;
	media_raw_audio_format	m_format;
	BSoundPlayer *			m_player;
	Mutex					m_lock;
	size_t					m_dataSize;
	Semaphore				m_eventSem;
	BLocker					m_pauseLock;
    int32                   m_bytesPerSample;
    int32                   m_lastFrame;
    int64                   m_totalBytesWritten;
    Event*                  m_event;
    static int32            s_lastVolume;
#if DEBUG_SAVE_PCM
    FILE*                   m_pcmSaveFile;
#endif
};

inline void
SoundCardPMO::Lock( void )
{
	m_lock.Acquire();
}

inline void
SoundCardPMO::Unlock( void )
{
	m_lock.Release();
}

#endif /* _SOUNDCARDPMO_H_ */
// vi: set ts=4:

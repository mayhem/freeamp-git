// $Id$

#include "ringbuffer.h"
#include <support/Debug.h>
#include <assert.h>

#define RB_PRINT(a)	PRINT(a)

#define memcpy_WDEBUG	memcpy
#define memcpy_RDEBUG	memcpy

RingBuffer::RingBuffer(	size_t	bufferSize )
:	m_bufferSize( bufferSize ),
	m_numBytesInBuffer( 0 ),
	m_buffer( NULL ),
	m_writerSem( "RingBufferWriter" ),
	m_readerSem( "RingBufferReader" ),
	m_readIndex( 0 ),
	m_writeIndex( 0 )
{
	m_buffer = new uint8[ m_bufferSize + 4096 ];
}

RingBuffer::~RingBuffer()
{
	if ( m_buffer )
	{
		delete[] m_buffer;
		m_buffer = NULL;
	}
}

ssize_t
RingBuffer::Write( const void* data, size_t length, bool waitforever )
{
	size_t	bytesToWrite = BufferSize() < length ? BufferSize() : length;

	assert( length > 0 );

	for ( ; ; )
	{
		Lock();
PRINT(( "%d bytes empty while %d bytes needed\n", NumBytesEmpty(), bytesToWrite ));
		if ( NumBytesEmpty() < bytesToWrite )
		{
			Unlock();
			PRINT(( "WriteWait\n" ));
			m_writerSem.Wait();
		}
		else
		{
			break;
		}
	}

	if ( m_readIndex <= m_writeIndex )
	{
		if ( bytesToWrite <= (m_bufferSize - m_writeIndex) )
		{
			//
			memcpy_WDEBUG( (m_buffer + m_writeIndex), data, bytesToWrite );
			m_writeIndex += bytesToWrite;
			if ( m_writeIndex == BufferSize() )
			{
				m_writeIndex = 0;
			}
		}
		else
		{
			// data (to be written) strides the tail of the buffer.
			// so i need two memcpy's
			size_t	len1 = m_bufferSize - m_writeIndex;
			memcpy_WDEBUG( (m_buffer + m_writeIndex), data, len1 );
			m_writeIndex = 0;
			size_t	len2 = bytesToWrite - len1;
			memcpy_WDEBUG( (m_buffer + m_writeIndex), ((uint8*)data + len1), len2 );
			m_writeIndex += len2;
			assert( m_writeIndex <= m_readIndex );
		}
	}
	else if ( m_readIndex > m_writeIndex )
	{
		memcpy_WDEBUG( (m_buffer + m_writeIndex), data, bytesToWrite );
		m_writeIndex += bytesToWrite;
		assert( m_readIndex >= m_writeIndex );
	}

	m_numBytesInBuffer += bytesToWrite;

	assert( m_writeIndex <= BufferSize() );
	PRINT(( "RingBuffer::Write:(%d -> %d, %d)\n", m_readIndex, m_writeIndex, m_numBytesInBuffer ));

	Unlock();

	m_readerSem.Signal();

	return bytesToWrite;
}

size_t
RingBuffer::Read( void* data, size_t length, bool waitforever )
{
	size_t	bytesToRead = BufferSize() < length ? BufferSize() : length;

	if ( length <= 0 ) return 0;

	for ( ; ; )
	{
		Lock();
		if ( NumBytesInBuffer() <= 0 )
		{
			Unlock();
			RB_PRINT(( "ReadWait\n" ));
			m_readerSem.Wait( 1 );
			if ( !m_readerSem.Wait( 1 ) && !waitforever )
			{
				return 0;
			}
		}
		else
		{
			break;
		}
	}
	if ( bytesToRead > NumBytesInBuffer() )
	{
		bytesToRead = NumBytesInBuffer();
	}
	RB_PRINT(( "Reading %d bytes\n", bytesToRead ));

	if ( m_readIndex < m_writeIndex )
	{
		memcpy_RDEBUG( data, (m_buffer + m_readIndex), bytesToRead );
		m_readIndex += bytesToRead;
		if ( m_readIndex == BufferSize() )
		{
			m_readIndex = 0;
		}
	}
	else if ( m_readIndex >= m_writeIndex )
	{
		if ( bytesToRead <= (m_bufferSize - m_readIndex) )
		{
			memcpy_RDEBUG( data, (m_buffer + m_readIndex), bytesToRead );
			m_readIndex += bytesToRead;
			if ( m_readIndex == BufferSize() )
			{
				m_readIndex = 0;
			}
		}
		else
		{
			// i need two memcpy_DEBUG's
			size_t	len1 = m_bufferSize - m_readIndex;
			memcpy_RDEBUG( data, (m_buffer + m_readIndex), len1 );
			m_readIndex = 0;
			size_t	len2 = bytesToRead - len1;
			memcpy_RDEBUG( ((uint8*)data + len1), (m_buffer + m_readIndex), len2 );
			m_readIndex += len2;
		}
	}

	m_numBytesInBuffer -= bytesToRead;
	assert( m_readIndex <= BufferSize() );
	RB_PRINT(( "*** RingBuffer::Read:(%d -> %d, %d)\n", m_readIndex, m_writeIndex, m_numBytesInBuffer ));

	Unlock();

	m_writerSem.Signal();

	return bytesToRead;
}

// $Id$
#ifndef _RING_BUFFER_H
#define _RING_BUFFER_H

#include "mutex.h"
#include "semaphore.h"

class RingBuffer
{
public:
					RingBuffer( size_t bufferSize );
	virtual			~RingBuffer();
	ssize_t			Write(
						const void*	data,
						size_t		length,
						bool		waitforever = false
						);
	size_t			Read(
						void*		data,
						size_t		length,
						bool		waitforever = false
						);
	size_t			NumBytesInBuffer( void ) const;
	size_t			NumBytesEmpty( void ) const;
	size_t			BufferSize( void ) const;

protected:
	void			Lock( void );
	void			Unlock( void );

private:
	size_t			m_bufferSize;
	size_t			m_numBytesInBuffer;
	uint8*			m_buffer;
	Mutex			m_lock;
	Semaphore		m_writerSem;
	Semaphore		m_readerSem;
	size_t			m_writeIndex;
	size_t			m_readIndex;
};

inline void
RingBuffer::Lock( void )
{
	m_lock.Acquire();
}

inline void
RingBuffer::Unlock( void )
{
	m_lock.Release();
}

inline size_t
RingBuffer::NumBytesInBuffer( void ) const
{
	return m_numBytesInBuffer;
}

inline size_t
RingBuffer::NumBytesEmpty( void ) const
{
	return ( m_bufferSize - m_numBytesInBuffer );
}

inline size_t
RingBuffer::BufferSize( void ) const
{
	return m_bufferSize;
}

#endif // _RING_BUFFER_H

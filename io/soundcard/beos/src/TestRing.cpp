#include "ringbuffer.h"
#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <iostream>

#define BUFSIZE 1024

RingBuffer*	ringBuffer;
char		filename[ 256 ];
bool		done = false;

void WriterThread( void* )
{
	FILE*	fp = fopen( filename, "r" );
	puts( "check" );
	char	buf[ 256 ];
	int		count = 0;

	while ( fgets( buf, 255, fp ) != NULL )
	{
		size_t	bytesToWrite = strlen( buf );
		ringBuffer->Write( buf, bytesToWrite );
		if ( ++count > 100 )
		{
			count = 0;
		}
	}

	fclose( fp );
	done = true;
	printf( "WRITE DONE\n" );
}

main( int argc, char** argv )
{
	if ( argc < 2 )
	{
		puts( "filename needed" );
		exit( EXIT_FAILURE );
	}

	strcpy( filename, argv[1] );

	puts( "1" );
	ringBuffer = new RingBuffer( BUFSIZE );
	puts( "2" );
	Thread*	writerThread = Thread::CreateThread();

	writerThread->Create( WriterThread, NULL );

	FILE*	outfp = fopen( "out", "w" );

	bool	readDone = false;
	while ( !readDone )
	{
		char	buf[ 256 ];
puts( "reading" );
		size_t	bytesRead = ringBuffer->Read( buf, 10 );
		if ( bytesRead == 0 )
		{
			printf( "done\n" );
			readDone = true;
		}
		buf[ bytesRead ] = '\0';
		fprintf( outfp, "%s", buf );
		snooze( 50000 );
	}

	fclose( outfp );
}

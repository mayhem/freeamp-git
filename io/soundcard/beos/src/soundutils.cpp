// $Id$

#include "config.h"
#include "soundutils.h"
#include "pmo.h"
#include <media/MediaDefs.h>

status_t
get_audio_format( const OutputInfo* info, media_raw_audio_format* format )
{
	status_t	ret = B_NO_ERROR;

	switch ( info->bits_per_sample )
	{
	case 8:
		format->format = media_raw_audio_format::B_AUDIO_UCHAR;
		break;
	case 16:
		format->format = media_raw_audio_format::B_AUDIO_SHORT;
		break;
	default:
		format->format = media_raw_audio_format::B_AUDIO_SHORT;
		ret = B_ERROR;
		break;
	}

//	format->format = media_raw_audio_format::B_AUDIO_SHORT;
	format->channel_count = info->number_of_channels;
	format->frame_rate = info->samples_per_second;
	format->byte_order = 2;	// LITTLE_ENDIAN
	format->buffer_size = info->max_buffer_size;

	return( ret );
}


// $Id$
#ifndef _SOUND_UTILS_H
#define _SOUND_UTILS_H

#include "config.h"
#include <support/SupportDefs.h> 

class OutputInfo;

status_t get_audio_format( const OutputInfo* info, media_raw_audio_format* format );

#endif // _SOUND_UTILS_H

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

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "config.h"
#include "dib.h"

class Renderer {
 public:
    Renderer(){}

    // Do a direct copy of bits from one DIB to another

    static bool Copy(   DIB* dest,      // destination bitmap
                        int32 dest_x,   // starting x coordinate
                        int32 dest_y,   // starting y coordinate
                        int32 width,    // how many pixels to copy
                        int32 height,   // how many pixels to copy
                        DIB* src,       // source bitmap
                        int32 src_x,    // starting x coordinate
                        int32 src_y);   // starting y coordinate

    static bool Copy(   DIB* dest,      // destination bitmap
                        DIB* src);      // source bitmap

    static bool Copy(   DIB* dest,      // destination bitmap
                        int32 dest_x,   // starting x coordinate
                        int32 dest_y,   // starting y coordinate
                        int32 width,    // how many pixels to copy
                        int32 height,   // how many pixels to copy
                        DIB* src,       // source bitmap
                        DIB* src_mask,  // mask used during copy
                        int32 src_x,    // starting x coordinate
                        int32 src_y);   // starting y coordinate

    static bool Copy(   DIB* dest,      // destination bitmap
                        DIB* src,       // source bitmap
                        DIB* src_mask); // mask used during copy    

    static bool Tile(   DIB* dest,      // destination bitmap
                        int32 dest_x,   // starting x coordinate
                        int32 dest_y,   // starting y coordinate
                        int32 width,    // how many pixels to copy
                        int32 height,   // how many pixels to copy
                        DIB* src);      // source bitmap

    
    // Fill a DIB with the specified color
    static bool Fill(   DIB* dest,      // destination bitmap
                        short r,        // red component
                        short g,        // green component
                        short b);       // blue component

    static bool Fill(   DIB* dest,      // destination bitmap
                        int32 dest_x,   // starting x coordinate
                        int32 dest_y,   // starting y coordinate
                        int32 width,    // how many pixels to copy
                        int32 height,   // how many pixels to copy
                        short r,        // red component
                        short g,        // green component
                        short b);       // blue component

    static bool Fill(   DIB* dest,      // destination bitmap
                        short r,        // red component
                        short g,        // green component
                        short b,        // blue component
                        short a);       // alpha component

    static bool Fill(   DIB* dest,      // destination bitmap
                        int32 dest_x,   // starting x coordinate
                        int32 dest_y,   // starting y coordinate
                        int32 width,    // how many pixels to copy
                        int32 height,   // how many pixels to copy
                        short r,        // red component
                        short g,        // green component
                        short b,        // blue component
                        short a);       // alpha component

    // Blend two DIBs using an alpha mask
    static bool Blend(  DIB* dest,      // destination bitmap
                        int32 dest_x,   // starting x coordinate
                        int32 dest_y,   // starting y coordinate
                        int32 width,    // how many pixels to copy
                        int32 height,   // how many pixels to copy
                        DIB* src,       // source bitmap
                        DIB* src_mask,  // mask used during blend
                        int32 src_x,    // starting x coordinate
                        int32 src_y);   // starting y coordinate

    static bool Blend(  DIB* dest,      // destination bitmap
                        DIB* src,       // source bitmap
                        DIB* src_mask); // mask used during blend    

    // Perform various effects between DIBs 
    static bool Darken( DIB* dest,      // destination bitmap
                        int32 dest_x,   // starting x coordinate
                        int32 dest_y,   // starting y coordinate
                        int32 width,    // how many pixels to copy
                        int32 height,   // how many pixels to copy
                        DIB* src,       // source bitmap
                        int32 src_x,    // starting x coordinate
                        int32 src_y);   // starting y coordinate

    static bool Darken( DIB* dest,      // destination bitmap
                        DIB* src);      // source bitmap

    static bool Difference( DIB* dest,      // destination bitmap
                            int32 dest_x,   // starting x coordinate
                            int32 dest_y,   // starting y coordinate
                            int32 width,    // how many pixels to copy
                            int32 height,   // how many pixels to copy
                            DIB* src,       // source bitmap
                            int32 src_x,    // starting x coordinate
                            int32 src_y);   // starting y coordinate

    static bool Difference( DIB* dest,      // destination bitmap
                            DIB* src);      // source bitmap

    static bool Lighten(    DIB* dest,      // destination bitmap
                            int32 dest_x,   // starting x coordinate
                            int32 dest_y,   // starting y coordinate
                            int32 width,    // how many pixels to copy
                            int32 height,   // how many pixels to copy
                            DIB* src,       // source bitmap
                            int32 src_x,    // starting x coordinate
                            int32 src_y);   // starting y coordinate

    static bool Lighten(    DIB* dest,      // destination bitmap
                            DIB* src);      // source bitmap

    static bool Multiply(   DIB* dest,      // destination bitmap
                            int32 dest_x,   // starting x coordinate
                            int32 dest_y,   // starting y coordinate
                            int32 width,    // how many pixels to copy
                            int32 height,   // how many pixels to copy
                            DIB* src,       // source bitmap
                            int32 src_x,    // starting x coordinate
                            int32 src_y);   // starting y coordinate

    static bool Multiply(   DIB* dest,      // destination bitmap
                            DIB* src);      // source bitmap
};

#endif /* _RENDERER_H_ */
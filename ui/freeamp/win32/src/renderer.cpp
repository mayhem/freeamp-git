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

#include <assert.h>

#include "config.h"
#include "renderer.h"


bool 
Renderer::
Copy(   DIB* dest,      // destination bitmap
        int32 dest_x,   // starting x coordinate
        int32 dest_y,   // starting y coordinate
        int32 width,    // how many pixels to copy
        int32 height,   // how many pixels to copy
        DIB* src,       // source bitmap
        int32 src_x,    // starting x coordinate
        int32 src_y)    // starting y coordinate
{
    bool result = true;

    assert(dest);
    assert(src);
    assert(dest->BitsPerPixel() == src->BitsPerPixel());

    BYTE* pdest = dest->Bits(dest_x, dest_y);
    BYTE* psrc  = src->Bits(src_x, src_y);
    int32 bytesPerPixel = src->BytesPerPixel();

    for(int32 y = 0; y < height; y++)
	{
		memcpy(pdest, psrc, width * bytesPerPixel );

		pdest -= dest->BytesPerLine();
		psrc -= src->BytesPerLine();
	}

    return result;
}

bool 
Renderer::
Copy(   DIB* dest,      // destination bitmap
        DIB* src)       // source bitmap
{
    bool result = true;

    assert(dest);
    assert(src);
    assert(dest->BitsPerPixel() == src->BitsPerPixel());
    assert(dest->Height() == src->Height());
    assert(dest->Width() == src->Width());

    BYTE* pdest = dest->Bits();
    BYTE* psrc  = src->Bits();
    int32 bytesPerPixel = src->BytesPerPixel();
    int32 height = dest->Height();
    int32 width = dest->Width();

    for(int32 y = 0; y < height; y++)
	{
		memcpy(pdest, psrc, width * bytesPerPixel );

		pdest += dest->BytesPerLine();
		psrc += src->BytesPerLine();
	}

    return result;
}

bool 
Renderer::
Copy(   DIB* dest,      // destination bitmap
        int32 dest_x,   // starting x coordinate
        int32 dest_y,   // starting y coordinate
        int32 width,    // how many pixels to copy
        int32 height,   // how many pixels to copy
        DIB* src,       // source bitmap
        DIB* src_mask,  // mask used during copy
        int32 src_x,    // starting x coordinate
        int32 src_y)    // starting y coordinate
{
    bool result = true;

    assert(dest);
    assert(src);
    assert(src_mask);

    assert(dest->BitsPerPixel() == src->BitsPerPixel());
    assert(src_mask->BitsPerPixel() == 8);

    BYTE* pdest = dest->Bits(dest_x, dest_y);
    BYTE* psrc  = src->Bits(src_x, src_y);
    BYTE* psrc_mask  = src_mask->Bits(src_x, src_y);
    int32 bytesPerPixel = src->BytesPerPixel();

    //if(dest->BitsPerPixel() == 24)
    {
        for(int32 y = 0; y < height; y++)
	    {
            for ( int x = 0; x < width; x++ )
		    {
                if(*psrc_mask)
                {
                    memcpy(pdest, psrc, bytesPerPixel);
                }
                
			    //pdest[0]=(BYTE)(((psrc[0]-pdest[0])*psrc_mask[0]+(pdest[0]<<8))>>8);
			    //pdest[1]=(BYTE)(((psrc[1]-pdest[1])*psrc_mask[0]+(pdest[1]<<8))>>8);
			    //pdest[2]=(BYTE)(((psrc[2]-pdest[2])*psrc_mask[0]+(pdest[2]<<8))>>8);	
			    pdest += bytesPerPixel;
			    psrc += bytesPerPixel;
                psrc_mask++;
		    }

		    pdest -= dest->BytesPerLine() + width * bytesPerPixel;
		    psrc -= src->BytesPerLine() + width * bytesPerPixel;
            psrc_mask -= src_mask->BytesPerLine() + width; 
	    }
    }
    
    return result;
}

bool 
Renderer::
Copy(   DIB* dest,      // destination bitmap
        DIB* src,       // source bitmap
        DIB* src_mask)  // mask used during copy   
{
    bool result = true;

    assert(dest);
    assert(src);
    assert(src_mask);

    assert(dest->BitsPerPixel() == src->BitsPerPixel());
    assert(src_mask->BitsPerPixel() == 8);

    BYTE* pdest = dest->Bits();
    BYTE* psrc  = src->Bits();
    BYTE* psrc_mask  = src_mask->Bits();
    int32 bytesPerPixel = src->BytesPerPixel();
    int32 height = dest->Height();
    int32 width = dest->Width();

    for(int32 y = 0; y < height; y++)
	{
        for ( int x = 0; x < width; x++ )
		{
            if(*psrc_mask)
            {
                memcpy(pdest, psrc, bytesPerPixel);
            }

			//pdest[0]=(BYTE)(((psrc[0]-pdest[0])*psrc_mask[0]+(pdest[0]<<8))>>8);
			//pdest[1]=(BYTE)(((psrc[1]-pdest[1])*psrc_mask[0]+(pdest[1]<<8))>>8);
			//pdest[2]=(BYTE)(((psrc[2]-pdest[2])*psrc_mask[0]+(pdest[2]<<8))>>8);	
			pdest += bytesPerPixel;
			psrc += bytesPerPixel;
            psrc_mask++;
		}

		pdest += dest->BytesPerLine() - width * bytesPerPixel;
		psrc += src->BytesPerLine() - width * bytesPerPixel;
        psrc_mask += src_mask->BytesPerLine() - width;
	}

    return result;
} 


bool 
Renderer::
Tile(   DIB* dest,      // destination bitmap
        int32 dest_x,   // starting x coordinate
        int32 dest_y,   // starting y coordinate
        int32 width,    // how many pixels to copy
        int32 height,   // how many pixels to copy
        DIB* src)       // source bitmap
{
    bool result = true;

    assert(dest);
    assert(src);
    assert(dest->BitsPerPixel() == src->BitsPerPixel());

    int32 src_x = dest_x%src->Width();
    int32 src_y = dest_y%src->Height();

    BYTE* pdest = dest->Bits(dest_x, dest_y);
    BYTE* psrc  = src->Bits(src_x, src_y);
    int32 bytesPerPixel = src->BytesPerPixel();

    int32 y_rep = height/src->Height();
    int32 x_rep = width/src->Width();
    int32 y_remainder = height%src->Height();
    int32 x_remainder = width%src->Width();

    // 369-8255

    for(int32 y = 0; y < height; y++)
    {
        int32 src_x1 = src_x;

        for(int32 x = 0; x < width - x_remainder; x++)
		{
            int32 numBytes = (src->Width() - src_x1) * bytesPerPixel;

			memcpy(pdest, psrc, numBytes);

			pdest += numBytes;
            psrc -= src_x1 * bytesPerPixel;
            src_x1 = 0;
		}

        if(x_remainder)
        {
            memcpy(pdest, psrc, x_remainder * bytesPerPixel );
            pdest += x_remainder * bytesPerPixel;
        }

        if(++src_y >= src->Height())
        {
            src_y = 0;
        }

        pdest -= dest->BytesPerLine() + width * bytesPerPixel;
        psrc = src->Bits(src_x, src_y);
    }

    return result;
}



// Fill a DIB with the specified color
bool 
Renderer::
Fill(   DIB* dest,      // destination bitmap
        short r,        // red component
        short g,        // green component
        short b)        // blue component
{
    bool result = true;

    assert(dest);

    BYTE* pdest = dest->Bits();
    int32 bytesPerPixel = dest->BytesPerPixel();
    int32 height = dest->Height();
    int32 width = dest->Width();

    if(dest->BitsPerPixel() == 24)
    {
        for(int32 y = 0; y < height; y++)
	    {
		    for ( int x = 0; x < width; x++ )
		    {
			    pdest[0]=(BYTE)b;
			    pdest[1]=(BYTE)g;
			    pdest[2]=(BYTE)r;	
			    pdest+=3;
		    }

		    pdest += dest->BytesPerLine() - width * 3;
	    }
    }
    else if(dest->BitsPerPixel() == 8)
    {
        int32 index = dest->IndexForColor(r, g, b);

        if(index >= 0)
        {
            for(int32 y = 0; y < height; y++)
	        {
		        for(int x = 0; x < width; x++ )
		        {
			        *pdest = index;
			        pdest++;
		        }

		        pdest += dest->BytesPerLine() - width;
	        }
        }
    }

    return result;
}

bool 
Renderer::
Fill(   DIB* dest,      // destination bitmap
        int32 dest_x,   // starting x coordinate
        int32 dest_y,   // starting y coordinate
        int32 width,    // how many pixels to copy
        int32 height,   // how many pixels to copy
        short r,        // red component
        short g,        // green component
        short b)        // blue component
{
    bool result = true;

    assert(dest);

    BYTE* pdest = dest->Bits(dest_x, dest_y);
    int32 bytesPerPixel = dest->BytesPerPixel();

    if(dest->BitsPerPixel() == 24)
    {
        for(int32 y = 0; y < height; y++)
	    {
		    for ( int x = 0; x < width; x++ )
		    {
			    pdest[0]=(BYTE)b;
			    pdest[1]=(BYTE)g;
			    pdest[2]=(BYTE)r;	
			    pdest+=3;
		    }

		    pdest -= dest->BytesPerLine() + width * 3;
	    }
    }
    else if(dest->BitsPerPixel() == 8)
    {
        int32 index = dest->IndexForColor(r, g, b);

        if(index >= 0)
        {
            for(int32 y = 0; y < height; y++)
	        {
		        for(int x = 0; x < width; x++ )
		        {
			        *pdest = index;
			        pdest++;
		        }

		        pdest -= dest->BytesPerLine() + width;
	        }
        }
    }

    return result;
}

bool 
Renderer::
Fill(   DIB* dest,      // destination bitmap
        int32 dest_x,   // starting x coordinate
        int32 dest_y,   // starting y coordinate
        int32 width,    // how many pixels to copy
        int32 height,   // how many pixels to copy
        short r,        // red component
        short g,        // green component
        short b,        // blue component
        short a)        // alpha component
{
    bool result = true;

    assert(dest);

    BYTE* pdest = dest->Bits(dest_x, dest_y);
    int32 bytesPerPixel = dest->BytesPerPixel();

    if(dest->BitsPerPixel() == 24)
    {
        for(int32 y = 0; y < height; y++)
	    {
		    for ( int x = 0; x < width; x++ )
		    {
			    pdest[0]=(BYTE)(((b-pdest[0])*a+(pdest[0]<<8))>>8);
			    pdest[1]=(BYTE)(((g-pdest[1])*a+(pdest[1]<<8))>>8);
			    pdest[2]=(BYTE)(((r-pdest[2])*a+(pdest[2]<<8))>>8);	
			    pdest+=3;
		    }

		    pdest -= dest->BytesPerLine() + width * 3;
	    }
    }
    else if(dest->BitsPerPixel() == 8)
    {
        for(int32 y = 0; y < height; y++)
	    {
		    for ( int x = 0; x < width; x++ )
		    {
                Color destColor;

                dest->ColorForIndex(pdest[0], &destColor);

			    destColor.b = (BYTE)(((b-destColor.b)*a+(destColor.b<<8))>>8);
			    destColor.g = (BYTE)(((g-destColor.g)*a+(destColor.g<<8))>>8);
			    destColor.r = (BYTE)(((r-destColor.r)*a+(destColor.r<<8))>>8);

                pdest[0] = dest->IndexForColor(&destColor);
                
			    pdest++;
		    }

		    pdest -= dest->BytesPerLine() + width;
	    }
    }

    return result;
}



bool 
Renderer::
Fill(   DIB* dest,      // destination bitmap
        short r,        // red component
        short g,        // green component
        short b,        // blue component
        short a)        // alpha component
{
    bool result = true;

    assert(dest);

    BYTE* pdest = dest->Bits(0, 0);
    int32 bytesPerPixel = dest->BytesPerPixel();
    int32 height = dest->Height();
    int32 width = dest->Width();

    if(dest->BitsPerPixel() == 24)
    {
        for(int32 y = 0; y < height; y++)
	    {
		    for ( int x = 0; x < width; x++ )
		    {
			    pdest[0]=(BYTE)(((b-pdest[0])*a+(pdest[0]<<8))>>8);
			    pdest[1]=(BYTE)(((g-pdest[1])*a+(pdest[1]<<8))>>8);
			    pdest[2]=(BYTE)(((r-pdest[2])*a+(pdest[2]<<8))>>8);		
			    pdest+=3;
		    }

		    pdest -= dest->BytesPerLine() + width * 3;
	    }
    }

    return result;
}

// Blend two DIBs using an alpha mask
bool 
Renderer::
Blend(  DIB* dest,      // destination bitmap
        int32 dest_x,   // starting x coordinate
        int32 dest_y,   // starting y coordinate
        int32 width,    // how many pixels to copy
        int32 height,   // how many pixels to copy
        DIB* src,       // source bitmap
        DIB* src_mask,  // mask used during blend
        int32 src_x,    // starting x coordinate
        int32 src_y)    // starting y coordinate
{
    bool result = true;

    assert(dest);
    assert(src);
    assert(src_mask);

    assert(dest->BitsPerPixel() == src->BitsPerPixel());
    assert(src->BitsPerPixel() == src_mask->BitsPerPixel());

    BYTE* pdest = dest->Bits(dest_x, dest_y);
    BYTE* psrc  = src->Bits(src_x, src_y);
    BYTE* psrc_mask  = src_mask->Bits(src_x, src_y);
    int32 bytesPerPixel = src->BytesPerPixel();

    for(int32 y = 0; y < height; y++)
	{
        for ( int x = 0; x < width; x++ )
		{
			pdest[0]=(BYTE)(((psrc[0]-pdest[0])*psrc_mask[0]+(pdest[0]<<8))>>8);
			pdest[1]=(BYTE)(((psrc[1]-pdest[1])*psrc_mask[1]+(pdest[1]<<8))>>8);
			pdest[2]=(BYTE)(((psrc[2]-pdest[2])*psrc_mask[2]+(pdest[2]<<8))>>8);	
			pdest += 3;
			psrc += 3;
            psrc_mask += 3;
		}

		pdest -= dest->BytesPerLine() + width * 3;
		psrc -= src->BytesPerLine() + width * 3;
        psrc_mask -= src_mask->BytesPerLine() + width * 3;
	}

    return result;
}

bool 
Renderer::
Blend(  DIB* dest,      // destination bitmap
        DIB* src,       // source bitmap
        DIB* src_mask)  // mask used during blend    
{
    bool result = true;

    assert(dest);
    assert(src);
    assert(src_mask);

    assert(dest->BitsPerPixel() == src->BitsPerPixel());
    assert(src->BitsPerPixel() == src_mask->BitsPerPixel());

    BYTE* pdest = dest->Bits();
    BYTE* psrc  = src->Bits();
    BYTE* psrc_mask  = src_mask->Bits();
    int32 bytesPerPixel = src->BytesPerPixel();
    int32 height = dest->Height();
    int32 width = dest->Width();

    for(int32 y = 0; y < height; y++)
	{
        for ( int x = 0; x < width; x++ )
		{
			pdest[0]=(BYTE)(((psrc[0]-pdest[0])*psrc_mask[0]+(pdest[0]<<8))>>8);
			pdest[1]=(BYTE)(((psrc[1]-pdest[1])*psrc_mask[1]+(pdest[1]<<8))>>8);
			pdest[2]=(BYTE)(((psrc[2]-pdest[2])*psrc_mask[2]+(pdest[2]<<8))>>8);	
			pdest += 3;
			psrc += 3;
            psrc_mask += 3;
		}

		pdest += dest->BytesPerLine() - width * 3;
		psrc += src->BytesPerLine() - width * 3;
        psrc_mask += src_mask->BytesPerLine() - width * 3;
	}

    return result;
}

// Perform various effects between DIBs 
bool 
Renderer::
Darken( DIB* dest,      // destination bitmap
        int32 dest_x,   // starting x coordinate
        int32 dest_y,   // starting y coordinate
        int32 width,    // how many pixels to copy
        int32 height,   // how many pixels to copy
        DIB* src,       // source bitmap
        int32 src_x,    // starting x coordinate
        int32 src_y)    // starting y coordinate
{
    bool result = false;

    return result;
}

bool 
Renderer::
Darken( DIB* dest,      // destination bitmap
        DIB* src)       // source bitmap
{
    bool result = false;

    return result;
}

bool 
Renderer::
Difference( DIB* dest,      // destination bitmap
            int32 dest_x,   // starting x coordinate
            int32 dest_y,   // starting y coordinate
            int32 width,    // how many pixels to copy
            int32 height,   // how many pixels to copy
            DIB* src,       // source bitmap
            int32 src_x,    // starting x coordinate
            int32 src_y)    // starting y coordinate
{
    bool result = false;

    return result;
}

bool 
Renderer::
Difference( DIB* dest,      // destination bitmap
            DIB* src)       // source bitmap
{
    bool result = false;

    return result;
}

bool 
Renderer::
Lighten(DIB* dest,      // destination bitmap
        int32 dest_x,   // starting x coordinate
        int32 dest_y,   // starting y coordinate
        int32 width,    // how many pixels to copy
        int32 height,   // how many pixels to copy
        DIB* src,       // source bitmap
        int32 src_x,    // starting x coordinate
        int32 src_y)    // starting y coordinate
{
    bool result = false;

    return result;
}

bool 
Renderer::
Lighten(DIB* dest,      // destination bitmap
        DIB* src)       // source bitmap
{
    bool result = false;

    return result;
}

bool 
Renderer::
Multiply(   DIB* dest,      // destination bitmap
            int32 dest_x,   // starting x coordinate
            int32 dest_y,   // starting y coordinate
            int32 width,    // how many pixels to copy
            int32 height,   // how many pixels to copy
            DIB* src,       // source bitmap
            int32 src_x,    // starting x coordinate
            int32 src_y)    // starting y coordinate
{
    bool result = false;

    return result;
}

bool 
Renderer::
Multiply(   DIB* dest,      // destination bitmap
            DIB* src)       // source bitmap
{
    bool result = false;

    return result;
}

/*____________________________________________________________________________

  FreeAmp - The Free MP3 Player
 
  Portions Copyright (C) 2000 Relatable
 
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

#ifndef INCLUDED_SIGFFT_H_
#define INCLUDED_SIGFFT_H_
//------------------------------------
//  fft.h
//  Fast Fourier Transform
//  modified by Sean Ward, 2000
//  portions (c) Reliable Software, 1996
//------------------------------------

#include "config.h"

#include "sigcomplex.h"
#include <assert.h>

class SampleIter;

class FFT
{
public:
    FFT  (int Points, long sampleRate);
    ~FFT ();
    int  Points () const { return _Points; }
    void Transform ();
    void CopyIn(char* pBuffer, int nNumSamples);

    double  GetIntensity (int i) const
    { 
        assert (i < _Points);
        return _X[i].Mod()/_sqrtPoints; 
    }

    int     GetFrequency (int point) const
    {
        // return frequency in Hz of a given point
        assert (point < _Points);
        long x =_sampleRate * point;
        return x / _Points;
    }

    int     HzToPoint (int freq) const 
    { 
        return (long)_Points * freq / _sampleRate; 
    }

    int     MaxFreq() const { return _sampleRate; }

    int     Tape (int i) const
    {
        assert (i < _Points);
        return (int) _aTape[i];
    }

private:

    void PutAt ( int i, double val )
    {
        _X [_aBitRev[i]] = Complex (val);
    }

    int        _Points;
    long       _sampleRate;
    int	       _logPoints;
    double     _sqrtPoints;
    int	      *_aBitRev;       // bit reverse vector
    Complex   *_X;             // in-place fft array
    Complex  **_W;             // exponentials
    double    *_aTape;         // recording tape
};

#endif

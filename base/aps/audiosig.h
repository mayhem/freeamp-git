/*___________________________________________________________________________

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
        along with this program; if not, Write to the Free Software
        Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
        
        $Id$
____________________________________________________________________________*/

#ifndef _AUDIOSIG_H_
#define _AUDIOSIG_H_

#define NUMSIGFIELDS 35

class AudioSig
{
public:
    AudioSig(float energy, float zxing, float length, int *spectrum)
    { m_fEnergy = energy; m_fZXing = zxing; m_fLength = length;
      for (int i = 0; i < 32; i++) m_iSpectrum[i] = spectrum[i]; 
    }
   ~AudioSig() {}

    float Energy()   { return m_fEnergy; }
    float ZXing()    { return m_fZXing; }
    float Length()   { return m_fLength; }
    int  *Spectrum() { return m_iSpectrum; }

private:
    float m_fEnergy;
    float m_fZXing;
    float m_fLength;
    int   m_iSpectrum[32];
};

#endif

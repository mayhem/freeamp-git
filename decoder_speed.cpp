
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


#include <iostream.h>

#include "lmc/xingmp3/include/xinglmc.h"
#include "base/unix/linux/include/soundcardpmo.h"
#include "io/local/localfileinput.h"



int main(int argc, char **argv) {
    if (argc == 0) return 255;
    cout << "Playing " << argv[1] << " for testing..." << endl;
    LocalFileInput *pLFI = new LocalFileInput(argv[1]);
    SoundCardPMO *pSCO = new SoundCardPMO();
    XingLMC *myLMC = new XingLMC();
    myLMC->SetPMI(pLFI);
    myLMC->SetPMO(pSCO);
    myLMC->Init();
    myLMC->DecodeWork();
    delete myLMC;
    return 0;
}

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

#pragma once

#include <LApplication.h>

class CFreeAmp : public LApplication {
public:
						CFreeAmp();
	virtual 			~CFreeAmp();
	
	virtual Boolean		ObeyCommand(CommandT inCommand, void * ioParam);	
	virtual void		FindCommandStatus(CommandT inCommand, Boolean & outEnabled,
							Boolean & outUsesMark, UInt16 & outMark, Str255 outName);
protected:
	virtual void		StartUp();

};

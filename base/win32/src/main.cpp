/*____________________________________________________________________________
	
	FreeAMP - The Free MP3 Player
	Portions copyright (C) 1998 GoodNoise

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

/* System Includes */
#define STRICT
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <string.h>
#include <iostream.h>

/* Project Includes */
#include "resource.h"
#include "player.h"
#include "event.h"
#include "utility.h"
#include "registrar.h"

int APIENTRY WinMain(	HINSTANCE hInstance, 
						HINSTANCE hPrevInstance,
		 				LPSTR lpszCmdLine, 
						int cmdShow)
{
   LMCRegistry* lmc;
   PMIRegistry* pmi;
   PMORegistry* pmo;
   UIRegistry*  ui;

   // Initialize Windows Registry
   InitWindowsRegistry();

    // find all the plug-ins we use
    lmc = new LMCRegistry;
    RegisterLMCs(lmc);

    pmi = new PMIRegistry;
    RegisterPMIs(pmi);

    pmo = new PMORegistry;
    RegisterPMOs(pmo);

    ui = new UIRegistry;
    RegisterUIs(ui);

	// create the player
	Player *player = Player::getPlayer();

    delete player;

    delete lmc;
    delete pmi;
    delete pmo;
    delete ui;

	return 0;
}


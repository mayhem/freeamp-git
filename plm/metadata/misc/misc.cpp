/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1999 EMusic.com

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

// The debugger can't handle symbols more than 255 characters long.
// STL often creates symbols longer than that.
// When symbols are longer than 255 characters, the warning is disabled.
#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#include "config.h"
#include "errors.h"
#include "utility.h"

#include "misc.h"
#include "lmc.h"
#include "pmi.h"



extern "C"
{
   MetaDataFormat *Initialize(FAContext* context)
   {
      return new Misc(context);
   }
}

Misc::Misc(FAContext* context):MetaDataFormat(context)
{
    m_context = context;

    Registrar registrar;

    registrar.SetSubDir("plugins");
    registrar.SetSearchString("*.lmc");
    registrar.InitializeRegistry(&m_lmcReg, m_context->prefs);

    registrar.SetSearchString("*.pmi");
    registrar.InitializeRegistry(&m_pmiReg, m_context->prefs);

    for (uint32 i = 0; i < m_lmcReg.CountItems(); i++)
    {
        LogicalMediaConverter* lmc;
        RegistryItem* temp = m_lmcReg.GetItem(i);

        lmc = (LogicalMediaConverter *)temp->InitFunction()(m_context);
        vector<char*>* extList = lmc->GetExtensions();

        for (uint32 j = 0; j < extList->size(); j++)
        {
            string ext = (*extList)[j];
            m_extensions[ext] = temp;
        }

        delete extList;
        delete lmc;
    }
}

Misc::~Misc()
{

}

bool Misc::ReadMetaData(const char* url, MetaData* metadata)
{
    bool result = false;

    assert(url);
    assert(metadata);

    // do we need to calculate the song length?
    if(!strncasecmp(url, "file://", 7) && !metadata->Time())
    {
        for (uint32 i = 0; i < m_pmiReg.CountItems(); i++)
        {
            PhysicalMediaInput* pmi;
            RegistryItem* pmiItem = m_pmiReg.GetItem(i);

            pmi = (PhysicalMediaInput*)pmiItem->InitFunction()(m_context);

            if(pmi->CanHandle(url, NULL))
            {
                pmi->SetTo(url);
                RegistryItem *lmcItem = NULL;
                char* cp;

                cp = strrchr(url, '.');

                if(cp)
                {
                    cp++;
                    
                    char temp[256];

                    strcpy(temp, cp);

                    cp = temp;

                    while(*cp)
                    {
                        *cp = toupper(*cp);
                        cp++;
                    }

                    string ext = temp;

                    ExtensionMap::const_iterator iter = m_extensions.find(ext);
                
                    if(iter != m_extensions.end())
                    {
                        LogicalMediaConverter* lmc;

                        lmcItem = iter->second; 

                        lmc = (LogicalMediaConverter*)lmcItem->InitFunction()(m_context);
                    
                        lmc->SetPMI(pmi);

                        uint32 length = lmc->CalculateSongLength();

                        metadata->SetTime(length);

                        delete lmc;
                    }
                }

                delete pmi;
                break;
            }

            delete pmi;
        }
    }

    // do we need to come up with a name?
    if(!strncasecmp(url, "file://", 7) && !metadata->Title().size())
    {
        char* temp = new char[strlen(url) + 1];

        strcpy(temp, url);

        char* ext = strrchr(temp, '.');
        char* file = strrchr(temp, '/'); // these are all URLs so we don't need DIR_MARKER
        
        if(ext)
            *ext = 0x00;
        
        if(!file)
            file = temp;
        else
            file++;
    
        metadata->SetTitle(file);

        delete [] temp;
    }
    else if(!strncasecmp(url, "http://", 7) && !metadata->Title().size())
    {
        metadata->SetTitle("HTTP Stream");
    }
    else if(!strncasecmp(url, "rtp://", 6) && !metadata->Title().size())
    {
        metadata->SetTitle("RTP Stream");
    }

    return true;
}

bool Misc::WriteMetaData(const char* url, const MetaData& metadata)
{
    bool result = false;
    return result;
}


/*____________________________________________________________________________

   FreeAmp - The Free MP3 Player

   Copyright (C) 2000 EMusic.com
   Portions Copyright (C) 1999 Bill Yuan

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

#include <math.h>
#include <stdio.h>
#include <string>
#include "eventdata.h"
#include "Equalizer.h"

#define	LOWER_LIMIT_DB	-20
#define	UPPER_LIMIT_DB	20
#define	NUM_TICKS		10
#define	LIMIT_DB_RATIO	3
#define	LOWER_LIMIT		LOWER_LIMIT_DB*LIMIT_DB_RATIO
#define	UPPER_LIMIT		UPPER_LIMIT_DB*LIMIT_DB_RATIO
#define	TOTAL_LIMIT		(UPPER_LIMIT-(LOWER_LIMIT))

Equalizer::Equalizer(FAContext *context)
{

    m_context = context;
    m_settingsChanged = false;
    m_enabled = false;

    int i;

	for(i=0; i<32; i++)
		m_equalizer[i] = 1.0;
	for(i=0; i<10; i++)
		m_sliders[i] = 0;
}

Equalizer::~Equalizer(void)
{
}

void Equalizer::Enable(bool enable)
{
   if (m_enabled != enable)
      m_settingsChanged = true;

   m_enabled = enable;
	m_context->target->AcceptEvent(new SetEqualizerDataEvent(enable));
}

bool Equalizer::IsEnabled(void)
{
   return m_enabled;
}

void Equalizer::LoadSettings(void)
{
   char   settings[255];
   int    enabled, i;
   uint32 len = 255;

   m_context->prefs->GetPrefString(kEqualizerSettingsPref, settings, &len);

   sscanf(settings, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
        &enabled, &m_sliders[0], &m_sliders[1], &m_sliders[2],
        &m_sliders[3], &m_sliders[4], &m_sliders[5], &m_sliders[6],
        &m_sliders[7], &m_sliders[8], &m_sliders[9]);

   for(i = 0; i < 10; i++)
      ChangeValue(i, m_sliders[i]);

   Enable((enabled != 0));
   m_settingsChanged = false;
}

void Equalizer::SaveSettings(void)
{
   char settings[255];

   if (!m_settingsChanged)
       return;

   sprintf(settings, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
        m_enabled, m_sliders[0], m_sliders[1], m_sliders[2],
        m_sliders[3], m_sliders[4], m_sliders[5], m_sliders[6],
        m_sliders[7], m_sliders[8], m_sliders[9]);
   m_context->prefs->SetPrefString(kEqualizerSettingsPref, settings);
}

void Equalizer::InitControls(Window *pWindow) 
{
   int    i;
   string nameString;
   char   name[20];

   for(i = 0; i < 10; i++)
   {
       sprintf(name, "Eq%d", i);
       nameString = string(name);
       pWindow->ControlIntValue(nameString, true, m_sliders[i]);
   }
}

void Equalizer::ChangeValue(int sliderNum, int value) 
{
   float temp;

   if (sliderNum < 0 || sliderNum > 9)
      return;

   m_sliders[sliderNum] = value;

   value = LOWER_LIMIT + ((UPPER_LIMIT - LOWER_LIMIT) * (100 - value)) / 100; 
   temp = (float)pow(10,(double)(0-value/LIMIT_DB_RATIO)/20);

   //printf("temp[%d]: %f %d slider: %d\n", 
   //        sliderNum, temp, value, m_sliders[sliderNum]);

	//                         power
	//power gain(db) = 10 * log 
	//                         10
	//
	//						     voltage
	//voltage gain(db) = 20 * log
	//                           10
	//1,1,1,1,1,1,2,4,8,12
	switch(sliderNum) 
   {
	case 0:
		m_equalizer[0] = temp;
		break;
	case 1:
		m_equalizer[1] = temp;
		break;
	case 2:
		m_equalizer[2] = temp;
		break;
	case 3:
		m_equalizer[3] = temp;
		break;
	case 4:
		m_equalizer[4] = temp;
		break;
	case 5:
		m_equalizer[5] = temp;
		break;
	case 6:
		m_equalizer[6] = temp;
		m_equalizer[7] = m_equalizer[6];
		break;
	case 7:
		m_equalizer[8] = temp;
		m_equalizer[9] = m_equalizer[8];
		m_equalizer[10] = m_equalizer[8];
		m_equalizer[11] = m_equalizer[8];
		break;
	case 8:
		m_equalizer[12] = temp;
		m_equalizer[13] = m_equalizer[12];
		m_equalizer[14] = m_equalizer[12];
		m_equalizer[15] = m_equalizer[12];
		m_equalizer[16] = m_equalizer[12];
		m_equalizer[17] = m_equalizer[12];
		m_equalizer[18] = m_equalizer[12];
		m_equalizer[19] = m_equalizer[12];
		break;
	case 9:
		m_equalizer[20] = temp;
		m_equalizer[21] = m_equalizer[20];
		m_equalizer[22] = m_equalizer[20];
		m_equalizer[23] = m_equalizer[20];
		m_equalizer[24] = m_equalizer[20];
		m_equalizer[25] = m_equalizer[20];
		m_equalizer[26] = m_equalizer[20];
		m_equalizer[27] = m_equalizer[20];
		m_equalizer[28] = m_equalizer[20];
		m_equalizer[29] = m_equalizer[20];
		m_equalizer[30] = m_equalizer[20];
		m_equalizer[31] = m_equalizer[20];

		break;
	default:
      return;
	}

   m_settingsChanged = true;
	m_context->target->AcceptEvent(new SetEqualizerDataEvent((float *)&m_equalizer));
}


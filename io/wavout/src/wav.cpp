/*____________________________________________________________________________

  FreeAmp - The Free MP3 Player

  Copyright (C) 2000 Micheal Rich

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

/* system headers */
#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <mmreg.h>

#include "wav.h"

WaveWriter::WaveWriter()
{
	// clear the structure memory
	memset(&m_mmioinfo, 0, sizeof(m_mmioinfo));

	m_hmmio = NULL;
	m_FP = NULL;
}

WaveWriter::~WaveWriter()
{
	if (m_hmmio != NULL)
		Close();
}

int WaveWriter::Create(const char *FileName, WAVEFORMATEX *waveformat)
{
	m_FP = fopen(FileName, "wb");

	memset(&m_WH, 0, sizeof(m_WH));

	m_WH.MainChunkID = mmioFOURCC('R', 'I', 'F', 'F');
	m_WH.Length = 0;		// temp, to be filled in as we go
	m_WH.ChunkTypeID = mmioFOURCC('W', 'A', 'V', 'E');
	m_WH.SubChunkID = mmioFOURCC('f', 'm', 't', ' ');
	m_WH.SubChunkLength = 16;

	m_WH.Format = waveformat->wFormatTag;
	m_WH.Modus = waveformat->nChannels;
	m_WH.SampleFreq = waveformat->nSamplesPerSec;
	m_WH.BytesPerSec = waveformat->nAvgBytesPerSec;
	m_WH.BytesPerSample = waveformat->nBlockAlign;
	m_WH.BitsPerSample = waveformat->wBitsPerSample;

	m_WH.DataChunkID = mmioFOURCC('d', 'a', 't', 'a');
	m_WH.DataLength = 0;		// temp, to be filled in as we go

	fwrite(&m_WH, sizeof(m_WH), 1, m_FP);

	return 0;
}

void WaveWriter::Close()
{
	if (m_FP == NULL) return;

	MessageBeep(MB_OK);

	m_WH.Length = m_WH.DataLength + sizeof(WaveHeader) - 4;
	
	fseek(m_FP, 0, SEEK_SET);
	fwrite(&m_WH, sizeof(m_WH), 1, m_FP);

	fclose(m_FP);

	m_FP = NULL;
}

DWORD WaveWriter::Write(const char *data, DWORD data_size)
{
	m_WH.DataLength+=data_size;
	return fwrite(data, data_size, 1, m_FP);
}

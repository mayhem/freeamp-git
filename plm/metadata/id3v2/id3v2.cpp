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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#ifdef WIN32
#include <winsock.h>
#else
#include <netinet/in.h>
#endif

#include "config.h"
#include "errors.h"
#include "utility.h"

#ifdef HAVE_ID3V2

#ifndef WIN32
#include <id3/tag.h>
#else
#include <tag.h>
#endif

#endif

#include "id3v2.h"

const int iDataFieldLen = 1024;

extern "C"
{
   MetaDataFormat *Initialize(FAContext* context)
   {
      return new ID3v2(context);
   }
}

ID3v2::ID3v2(FAContext* context):MetaDataFormat(context)
{
    m_context = context;
}

ID3v2::~ID3v2()
{

}

#ifdef HAVE_ID3V2
bool ID3v2::ReadMetaData(const char* url, MetaData* metadata)
{
    ID3_Tag   *pTag;
    ID3_Frame *pFrame;
    luint      ret;
    char      *pData;
    luint      len = 100;

    assert(url);
    assert(metadata);

    char path[_MAX_PATH];
    uint32 length = sizeof(path);

    URLToFilePath(url, path, &length);

    pTag = new ID3_Tag;
    ret = pTag->Link(path);
    if (ret <= 0)
    {
        delete pTag;
        return false;
    }

    pData = new char[iDataFieldLen];

    pFrame = pTag->Find(ID3FID_TITLE);
    if (pFrame)
    {
        pData[0] = 0;
        pFrame->Field(ID3FN_TEXT).Get(pData, len);
        if (strlen(pData) > 0)
           metadata->SetTitle(pData);
    }

    pFrame = pTag->Find(ID3FID_ALBUM);
    if (pFrame)
    {
        pData[0] = 0;
        pFrame->Field(ID3FN_TEXT).Get(pData, len);
        if (strlen(pData) > 0)
           metadata->SetAlbum(pData);
    }
    pFrame = pTag->Find(ID3FID_LEADARTIST);
    if (pFrame)
    {
        pData[0] = 0;
        pFrame->Field(ID3FN_TEXT).Get(pData, len);
        if (strlen(pData) > 0)
           metadata->SetArtist(pData);
    }
    pFrame = pTag->Find(ID3FID_SONGLEN);
    if (pFrame)
    {
        pData[0] = 0;
        pFrame->Field(ID3FN_TEXT).Get(pData, len);
        if (strlen(pData) > 0)
           metadata->SetTime(atoi(pData) / 1000);
    }
    pFrame = pTag->Find(ID3FID_YEAR);
    if (pFrame)
    {
        pData[0] = 0;
        pFrame->Field(ID3FN_TEXT).Get(pData, len);
        if (strlen(pData) > 0)
           metadata->SetYear(atoi(pData));
    }
    pFrame = pTag->Find(ID3FID_SIZE);
    if (pFrame)
    {
        pData[0] = 0;
        pFrame->Field(ID3FN_TEXT).Get(pData, len);
        if (strlen(pData) > 0)
           metadata->SetSize(atoi(pData));
    }

    delete pData;
    delete pTag;

    return true;
}

bool ID3v2::WriteMetaData(const char* url, const MetaData& metadata)
{
    ID3_Tag   *pTag;
    ID3_Frame *pFrame;
    vector<ID3_Frame *> cleanupList;
    vector<ID3_Frame *>::iterator i;
    char       dummy[20];
    bool       bWriteID3v1, bWriteID3v2;
    luint      whichTags;

    m_context->prefs->GetWriteID3v1(&bWriteID3v1);
    m_context->prefs->GetWriteID3v2(&bWriteID3v2);

    whichTags = (bWriteID3v1 && bWriteID3v2) ? BOTH_ID3_TAGS :
                (bWriteID3v1) ? V1_TAG :
                (bWriteID3v2) ? V2_TAG : 0;

    if (!whichTags)
        return true;

    assert(url);

    char path[_MAX_PATH];
    uint32 length = sizeof(path);

    URLToFilePath(url, path, &length);

    pTag = new ID3_Tag;
    pTag->Link(path);

    pFrame = pTag->Find(ID3FID_TITLE);
    if (!pFrame)
    {
        pFrame = new ID3_Frame(ID3FID_TITLE);
        pFrame->Field(ID3FN_TEXT).Set(metadata.Title().c_str());
        pTag->AddFrame(pFrame);
        cleanupList.push_back(pFrame);
    }
    else
    {
        pFrame->Field(ID3FN_TEXT).Set(metadata.Title().c_str());
    }
   
    pFrame = pTag->Find(ID3FID_ALBUM);
    if (!pFrame)
    {
        pFrame = new ID3_Frame(ID3FID_ALBUM);
        pFrame->Field(ID3FN_TEXT).Set(metadata.Album().c_str());
        pTag->AddFrame(pFrame);
        cleanupList.push_back(pFrame);
    }
    else
    {
        pFrame->Field(ID3FN_TEXT).Set(metadata.Title().c_str());
    }

    pFrame = pTag->Find(ID3FID_LEADARTIST);
    if (!pFrame)
    {
        pFrame = new ID3_Frame(ID3FID_LEADARTIST);
        pFrame->Field(ID3FN_TEXT).Set(metadata.Album().c_str());
        pTag->AddFrame(pFrame);
        cleanupList.push_back(pFrame);
    }
    else
    {
        pFrame->Field(ID3FN_TEXT).Set(metadata.Title().c_str());
    }

    sprintf(dummy, "%d", metadata.Size());
    pFrame = pTag->Find(ID3FID_SONGLEN);
    if (!pFrame)
    {
        pFrame = new ID3_Frame(ID3FID_SONGLEN);
        pFrame->Field(ID3FN_TEXT).Set(dummy);
        pTag->AddFrame(pFrame);
        cleanupList.push_back(pFrame);
    }
    else
    {
        pFrame->Field(ID3FN_TEXT).Set(dummy);
    }

    sprintf(dummy, "%d", metadata.Year());
    pFrame = pTag->Find(ID3FID_YEAR);
    if (!pFrame)
    {
        pFrame = new ID3_Frame(ID3FID_YEAR);
        pFrame->Field(ID3FN_TEXT).Set(dummy);
        pTag->AddFrame(pFrame);
        cleanupList.push_back(pFrame);
    }
    else
    {
        pFrame->Field(ID3FN_TEXT).Set(dummy);
    }

    sprintf(dummy, "%d", metadata.Size());
    pFrame = pTag->Find(ID3FID_SIZE);
    if (!pFrame)
    {
        pFrame = new ID3_Frame(ID3FID_SIZE);
        pFrame->Field(ID3FN_TEXT).Set(dummy);
        pTag->AddFrame(pFrame);
        cleanupList.push_back(pFrame);
    }
    else
    {
        pFrame->Field(ID3FN_TEXT).Set(dummy);
    }

    whichTags = pTag->Update(whichTags);

    delete pTag;

    for(i = cleanupList.begin(); i != cleanupList.end(); i++)
       delete i;

    return whichTags != NO_TAG;
}

#else

const int supportedVersion = 3;

struct ID3Header
{
   char          tag[3];
   unsigned char versionMajor;
   unsigned char versionRevision;
   unsigned char flags;
   unsigned char size[4];
};
struct FrameHeader
{
   char           tag[4];
   unsigned int   size;
   unsigned short flags;
};
const unsigned frameHeaderSize = 10;

void ID3v2::HandleFrame(char *tag, char *frameData, MetaData *metadata)
{
    char tagName[5];

    strncpy(tagName, tag, 4);
    tagName[4] = 0;

    if (strcmp(tagName, "TIT2") == 0)
        metadata->SetTitle(frameData);

    if (strcmp(tagName, "TALB") == 0)
        metadata->SetAlbum(frameData);

    if (strcmp(tagName, "TPE1") == 0)
        metadata->SetArtist(frameData);

    if (strcmp(tagName, "TLEN") == 0)
        metadata->SetTime(atoi(frameData) / 1000);

    if (strcmp(tagName, "TYER") == 0)
        metadata->SetYear(atoi(frameData));

    if (strcmp(tagName, "TSIZ") == 0)
        metadata->SetSize(atoi(frameData));
}

bool ID3v2::ReadMetaData(const char* url, MetaData* metadata)
{
    bool        result = false;
    FILE       *inFile;
    char        buffer[1024], *frameData;
    ID3Header   sHead;
    FrameHeader sFrame;
    int         ret;
    int         size;
    Error       error;

    assert(url);
    assert(metadata);

    char path[_MAX_PATH];
    uint32 length = sizeof(path);

    error = URLToFilePath(url, path, &length);

    if(IsntError(error))
    {
        inFile = fopen(path, "rb");
        if (inFile == NULL)
            return result;

        ret = fread(&sHead, 1, sizeof(ID3Header), inFile);
        if (ret != sizeof(ID3Header))
        {
            fclose(inFile);
            return result;
        }

        if (strncmp(sHead.tag, "ID3", 3))
        {
            fclose(inFile);
            return result;
        }

        if (sHead.versionMajor != supportedVersion)
        {
            fclose(inFile);
            return result;
        }
        size = ( sHead.size[3] & 0x7F       ) |
               ((sHead.size[2] & 0x7F) << 7 ) |
               ((sHead.size[1] & 0x7F) << 14) |
               ((sHead.size[0] & 0x7F) << 21);
        if (sHead.flags & (1 << 6))
        {
            unsigned extHeaderSize;

            if (fread(&extHeaderSize, 1, sizeof(int), inFile) != sizeof(int))
            {
                fclose(inFile);
                return result;
            }
            if (fread(buffer, 1, extHeaderSize, inFile) != extHeaderSize)
            {
                fclose(inFile);
                return result;
            }
        }
        for(; size > 0;)
        {
            if (fread(&sFrame, 1, frameHeaderSize, inFile) != frameHeaderSize)
            {
                fclose(inFile);
                return result;
            }
            sFrame.size = ntohl(sFrame.size);
            frameData = new char[sFrame.size + 1];
            if (fread(frameData, 1, sFrame.size, inFile) != sFrame.size)
            {
                delete frameData;
                fclose(inFile);
                return result;
            }
            frameData[sFrame.size] = 0;
            HandleFrame(sFrame.tag, &frameData[1], metadata);

            delete frameData;
            size -= sizeof(FrameHeader) + sFrame.size;
        }

        fclose(inFile);
    }
    return true;
}

bool ID3v2::WriteMetaData(const char* url, const MetaData& metadata)
{
    bool result = false;
    return result;
}

#endif

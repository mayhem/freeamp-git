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

#include <assert.h>
#include <errno.h>
#include <iostream>
#include <string>

using namespace std;

#include "config.h"
#include "errors.h"
#include "utility.h"
#include "rmp.h"

typedef struct FormatInfoStruct {
    char* extension;
    char* description;

} FormatInfoStruct; 

FormatInfoStruct formats[] = {
    {"rmp", "RMP Download Format"}
};

#define kNumFormats (sizeof(formats)/sizeof(FormatInfoStruct))

extern "C"
{
   DownloadFormat *Initialize(FAContext* context)
   {
      return new RMP(context);
   }
}

RMP::RMP(FAContext* context):DownloadFormat(context)
{
    m_context = context;
    m_pMetaData = NULL;
}

RMP::~RMP()
{
	delete m_pMetaData;
}

Error RMP::GetSupportedFormats(DownloadFormatInfo* info, uint32 index)
{
    Error result = kError_InvalidParam;

    assert(info);

    if(info)
    {
        result = kError_NoMoreFormats;

        if(index < kNumFormats)
        {
            info->SetExtension(formats[index].extension);
            info->SetDescription(formats[index].description);
            result = kError_NoErr;
        }
    }

    return result;
}

Error RMP::ReadDownloadFile(const char* url, vector<DownloadItem*>* list)
{
    Error result = kError_InvalidParam;

    assert(url);
    assert(list);

    if(url && list)
    {
        char* path = new char[_MAX_PATH];
        uint32 length = _MAX_PATH;

        URLToFilePath(url, path, &length);
        m_pList = list;

        result = ParseFile(string(path));
       
        m_pList = NULL; 
        delete [] path;
    }

    return result;

}

Error RMP::BeginElement(string &oElement, AttrMap &oAttrMap)
{
	m_oPath += string("/") + oElement;

	if (m_oPath == string("/PACKAGE/TRACKLIST/TRACK"))
	{
       delete m_pMetaData;
       m_pMetaData = new MetaData();
    }
    if (m_oPath == string("/PACKAGE/SERVER/COOKIE"))
    {
        if(m_oCookie.size())
        {
            m_oCookie += "; ";
        }

        m_oCookie += oAttrMap["NAME"];
        m_oCookie += "=";
        m_oCookie += oAttrMap["VALUE"];

        return kError_NoErr;
    }

	return kError_NoErr;
}

Error RMP::PCData(string &oData)
{
	if (m_oPath == string("/PACKAGE/PACKAGEID"))
    {
        m_oPackageId = oData;
        return kError_NoErr;
    }
    if (m_oPath == string("/PACKAGE/TARGET"))
    {
        m_oPlaylist = oData;
        return kError_NoErr;
    }
    if (m_oPath == string("/PACKAGE/SERVER/NETNAME"))
    {
    	m_oServer = oData;
        return kError_NoErr;
    }
	if (m_oPath == string("/PACKAGE/SERVER/LOCATION"))
    {
        m_oLocation = oData;
        return kError_NoErr;
    }
	if (m_oPath == string("/PACKAGE/TRACKLIST/LISTID"))
    {
    	m_oListId = oData;
        return kError_NoErr;
    }
	if (m_oPath == string("/PACKAGE/TRACKLIST/TRACK/TRACKID"))
    {
        m_oTrackId = oData;
    	m_pMetaData->SetTrack(atoi(oData.c_str()));
        return kError_NoErr;
    }
	if (m_oPath == string("/PACKAGE/TRACKLIST/TRACK/TITLE"))
    {
    	m_pMetaData->SetTitle(oData.c_str());
        return kError_NoErr;
    }
	if (m_oPath == string("/PACKAGE/TRACKLIST/TRACK/ALBUM"))
    {
    	m_pMetaData->SetAlbum(oData.c_str());
        return kError_NoErr;
    }
	if (m_oPath == string("/PACKAGE/TRACKLIST/TRACK/ARTIST"))
    {
    	m_pMetaData->SetArtist(oData.c_str());
        return kError_NoErr;
    }
	if (m_oPath == string("/PACKAGE/TRACKLIST/TRACK/GENRE"))
    {
    	m_pMetaData->SetGenre(oData.c_str());
        return kError_NoErr;
    }
	if (m_oPath == string("/PACKAGE/TRACKLIST/TRACK/FILENAME"))
    {
    	m_oFileName = oData;
        return kError_NoErr;
    }
	if (m_oPath == string("/PACKAGE/TRACKLIST/TRACK/SIZE"))
    {
    	m_pMetaData->SetSize(atoi(oData.c_str()));
        return kError_NoErr;
    }
    if (m_oPath == string("/PACKAGE/TRACKLIST/TRACK/YEAR"))
    {
    	m_pMetaData->SetYear(atoi(oData.c_str()));
        return kError_NoErr;
    }
    if (m_oPath == string("/PACKAGE/TRACKLIST/TRACK/DURATION"))
    {
    	m_pMetaData->SetTime(atoi(oData.c_str()));
        return kError_NoErr;
    }
        
	return kError_NoErr;
}


Error RMP::EndElement(string &oElement)
{
	char *pPtr;
    int   iOffset;
    
    pPtr = strrchr(m_oPath.c_str(), '/');
    if (pPtr == NULL)
       return kError_NoErr;
       
    iOffset = pPtr - m_oPath.c_str();
    m_oPath.erase(iOffset, m_oPath.length() - iOffset);   

	if (oElement == string("PACKAGE"))
    {
        DownloadItem *pItem;
        string        oFinal = "http://";
        
        oFinal += m_oServer;
        oFinal += m_oLocation;
      
        MangleLocation(oFinal);
        
        pItem = new DownloadItem(oFinal.c_str(), 
                                 m_oFileName.c_str(),
                                 m_pMetaData);
        pItem->SetPlaylistName(m_oPlaylist.c_str());
        pItem->SetTotalBytes(m_pMetaData->Size());
        pItem->SetSourceCookie(m_oCookie.c_str());

		m_pList->push_back(pItem);
        m_pMetaData = NULL;
    }
       
	return kError_NoErr;
}

void RMP::MangleLocation(string &oLocation)
{
	string::size_type iPos;
    string            oPattern;
    
    oPattern = "%fid";
    iPos = oLocation.find(oPattern, 0);
    if (iPos != string::npos)
        oLocation.replace(iPos, 4, m_oTrackId);
    
    oPattern = "%f";
    iPos = oLocation.find(oPattern, 0);
    if (iPos != string::npos)
        oLocation.replace(iPos, 2, m_oFileName);

    oPattern = "%lid";
    iPos = oLocation.find(oPattern, 0);
    if (iPos != string::npos)
        oLocation.replace(iPos, 4, m_oListId);

    oPattern = "%pid";
    iPos = oLocation.find(oPattern, 0);
    if (iPos != string::npos)
        oLocation.replace(iPos, 4, m_oPackageId);
}

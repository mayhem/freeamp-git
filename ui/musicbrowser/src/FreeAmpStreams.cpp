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
#include <assert.h>
#include <errno.h>
#include <iostream>
#include <string>

using namespace std;

#include "config.h"
#include "errors.h"
#include "FreeAmpStreams.h"

FreeAmpStreams::FreeAmpStreams(void)
{
    m_list = NULL;
    m_info = NULL;
}

FreeAmpStreams::~FreeAmpStreams(void)
{
    delete m_list;
    delete m_info;
}

Error FreeAmpStreams::ParseStreamXML(const string &xml, 
                                     vector<FreeAmpStreamInfo> &list)
{
    Error result = kError_InvalidParam;

    m_list = &list;
    result = ParseString(xml);
    m_list = NULL;

    return result;
}

Error FreeAmpStreams::BeginElement(string &oElement, AttrMap &oAttrMap)
{
    m_path += string("/") + oElement;
    m_curElement = oElement;

    printf("Path: '%s'\n", m_path.c_str());

    if (oElement == string("service"))
    {
        string name("name");

        if (oAttrMap.find(name) != oAttrMap.end())
            m_treePath += string("/") + oAttrMap[name];
        else
            return kError_YouScrewedUp;
 
        printf("TreePath: '%s'\n", m_treePath.c_str());
    }
    if (oElement == string("resource"))
    {
        string href("href");

        delete m_info;
        m_info = new FreeAmpStreamInfo();
        if (oAttrMap.find(href) != oAttrMap.end())
            m_info->m_streamUrl = oAttrMap[href];
    }

    return kError_NoErr;
}

Error FreeAmpStreams::PCData(string &data)
{
    if (m_curElement == string("resource"))
    {
        m_info->m_name = data;
    }
    if (m_curElement == string("url"))
    {
        m_info->m_webUrl = data;
    }
    if (m_curElement == string("genre"))
    {
        m_info->m_genre = data;
    }
    if (m_curElement == string("name"))
    {
        m_info->m_name = data;
    }
    if (m_curElement == string("description"))
    {
        m_info->m_desc = data;
    }
    if (m_curElement == string("bitrate"))
    {
        m_info->m_bitrate = atoi(data.c_str());
    }
    if (m_curElement == string("users"))
    {
        m_info->m_numUsers = atoi(data.c_str());
    }
    if (m_curElement == string("maxusers"))
    {
        m_info->m_maxUsers = atoi(data.c_str());
    }
        
    m_curElement = string("");

    return kError_NoErr;
}


Error FreeAmpStreams::EndElement(string &oElement)
{
    char *pPtr;
    int   iOffset;

    if (oElement == string("resource"))
    {
        m_info->m_treePath = m_treePath;
        m_list->push_back(*m_info);
        delete m_info;
        m_info = NULL;
    }

    if (oElement == string("service"))
    {
        pPtr = strrchr(m_treePath.c_str(), '/');
        if (pPtr == NULL)
           return kError_NoErr;
           
        iOffset = pPtr - m_treePath.c_str();
        m_treePath.erase(iOffset, m_treePath.length() - iOffset);   

        printf("/TreePath: '%s'\n", m_treePath.c_str());
    }

    pPtr = strrchr(m_path.c_str(), '/');
    if (pPtr == NULL)
       return kError_NoErr;
       
    iOffset = pPtr - m_path.c_str();
    m_path.erase(iOffset, m_path.length() - iOffset);   

    return kError_NoErr;
}

#if 0

const char *test = 
"<directory>\n"
"   <service name=\"Wired Planet\">\n"
"       <service name=\"Fuss\">\n"
"           <resource href=\"http://icecast.server.com:8000/stream\">\n"
"               <name>Fuss stream</name>\n"
"               <description>This is some description</description>\n"
"               <genre>fuss hop</genre>\n"
"               <url>http://www.icecast.org</url>\n"
"               <bitrate>64000</bitrate>\n"
"               <users>316</users>\n"
"               <maxusers>350</maxusers>\n"
"           </resource>\n"
"       </service>\n"
"       <resource href=\"http://icecast.server.com:8000/stream\">\n"
"           <name>Some stream</name>\n"
"           <description>This is some description</description>\n"
"           <genre>fuss hop</genre>\n"
"           <url>http://www.icecast.org</url>\n"
"           <bitrate>64000</bitrate>\n"
"           <users>316</users>\n"
"           <maxusers>350</maxusers>\n"
"        </resource>\n"
"   </service>\n"
"</directory>\n";
 

void main(void)
{
    vector<FreeAmpStreamInfo>           list;
    vector<FreeAmpStreamInfo>::iterator i;
    FreeAmpStreams                      o;
    Error                               e;

    e = o.ParseStreamXML(string(test), list);
    if (IsError(e))
    {
        string err;

        o.GetErrorString(err);
        printf("0 items in vector: %s (%d)\n", err.c_str(), e);
    }
    else
    {
        printf("%d items in vector:\n\n", list.size());

        for(i = list.begin(); i != list.end(); i++)
        {
            printf(" treepath: '%s'\n", i->m_treePath.c_str());
            printf("streamurl: '%s'\n", i->m_streamUrl.c_str());
            printf("     name: '%s'\n", i->m_name.c_str());
            printf("   weburl: '%s'\n", i->m_webUrl.c_str());
            printf("    genre: '%s'\n", i->m_genre.c_str());
            printf("     desc: '%s'\n", i->m_desc.c_str());
            printf("    users:  %d\n", i->m_numUsers);
            printf(" maxusers:  %d\n", i->m_maxUsers);
            printf("  bitrate:  %d\n\n", i->m_bitrate);
        }
    }
}
#endif

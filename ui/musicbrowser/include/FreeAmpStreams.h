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

#ifndef INCLUDED_FREEAMPSTREAMS_H
#define INCLUDED_FREEAMPSTREAMS_H

#include <string>
#include <vector>

using namespace std;

#include <assert.h>

#include "config.h"
#include "errors.h"
#include "Parse.h"

struct FreeAmpStreamInfo
{
    string m_treePath;
    string m_name;
    string m_streamUrl;
    string m_webUrl;
    string m_desc;
    string m_genre;
    int    m_bitrate, m_numUsers, m_maxUsers;
};

/*

The data will look like this:

And it should be available at:

<Directory>
   <Service Name="Wired Planet">
       <Service Name="Fuss">
           <Stream>
               <Url>http://icecast.server.com:8000/stream</Url>
               <Name>Fuss stream</Name>
               <Description>This is some description</Description>
               <Genre>fuss hop</Genre>
               <WebUrl>http://www.icecast.org</WebUrl>
               <Bitrate>64000</Bitrate>
               <Users>316</Users>
               <MaxUsers>350</MaxUsers>
           </Stream>
       </Service>
       <Stream> 
           <Url>http://icecast.server.com:8000/stream</Url>
           <Name>Some stream</Name>
           <Description>This is some description</Description>
           <Genre>fuss hop</Genre>
           <WebUrl>http://www.icecast.org</WebUrl>
           <Bitrate>64000</Bitrate>
           <Users>316</Users>
           <MaxUsers>350</MaxUsers>
        </Stream>
   </Service>
</Directory>

http://www.freeamp.org/streams.xml

*/

class FreeAmpStreams : public Parse 
{
 public:
             FreeAmpStreams(void);
    virtual ~FreeAmpStreams(void);

       Error ParseStreamXML(const string &xml, 
                            vector<FreeAmpStreamInfo> &list);
       Error ParseFileXML  (const string &file, 
                            vector<FreeAmpStreamInfo> &list);

 protected:
 
       Error BeginElement(string &oElement, AttrMap &oAttrMap);
       Error EndElement(string &oElement);
       Error PCData(string &oData);

 private:
 
     string                     m_path, m_treePath, m_curElement;
     FreeAmpStreamInfo         *m_info;
     vector<FreeAmpStreamInfo> *m_list;
};

#endif // INCLUDED_RMP_H

/*____________________________________________________________________________

   FreeAmp - The Free MP3 Player

   Copyright (C) 1999 EMusic

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

#include <time.h>
#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#include <winsock.h>
#define unlink(a) _unlink(a)
#else
#define TRUE 1
#define FALSE 0
#include <unistd.h>
#undef socklen_t
#include <netinet/in.h>
#endif

#include "zlib.h"
#include "config.h"
#include "ThemeZip.h"
#include "debug.h"

#define DB Debug_v("%s:%d\n", __FILE__, __LINE__);


const int iMajorVersion = 1;
const int iMinorVersion = 0;
const int iBufferSize = 8192;

ThemeZip::ThemeZip(void)
{
}

ThemeZip::~ThemeZip(void)
{
}

//
//
// Standard Archive Format - Standard TAR - USTAR
//
#define  RECORDSIZE  512
#define  NAMSIZ      100
#define  TUNMLEN      32
#define  TGNMLEN      32

#if WIN32
#pragma pack(push,1)
#else
#pragma pack(1)
#endif
union tar_record {
    char   charptr[RECORDSIZE];
    struct _header {
        char    name[NAMSIZ];
        char    mode[8];
        char    uid[8];
        char    gid[8];
        char    size[12];
        char    mtime[12];
        char    chksum[8];
        char    typeflag;
        char    linkname[NAMSIZ];
        char    magic[8];
        char    uname[TUNMLEN];
        char    gname[TGNMLEN];
        char    devmajor[8];
        char    devminor[8];
    } header;
};
#if WIN32
#pragma pack(pop)
#else
#pragma pack()
#endif

// The checksum field is filled with this while the checksum is computed.
#define    CHKBLANKS    "        "        // 8 blanks, no null 
// The magic field is filled with this if uname and gname are valid.
#define    TMAGIC    "ustar  "        // 7 chars and a null


Error ThemeZip::CompressThemeZip(const string &oDestFile, 
                                 vector<string *> &oFileList)
{
   gzFile  pOut;
   FILE   *pIn;
   char   *pBuffer;
   tar_record TarRecord;
   string  oFile;
   vector<string *>::iterator i;
   int     iWrite, iRead, iSize, iBlock,iPadding;

   pOut = gzopen(oDestFile.c_str(), "wb");
   if (pOut == NULL)
       return kError_FileNoAccess;

   pBuffer = new char[iBufferSize];
   for(i = oFileList.begin(); i != oFileList.end(); i++)
   {
       char *pPtr;
       time_t ltime;

       oFile = *(*i);
       pPtr = strrchr(oFile.c_str(), DIR_MARKER);
       if (pPtr)
           oFile.erase(0, ((int)pPtr - (int)oFile.c_str()) + 1);

       pIn = fopen((*i)->c_str(), "rb");
       if (pIn == NULL)
       {
           gzclose(pOut);
           delete pBuffer;
           return kError_FileNotFound;
       }

       // create header
       memset(&TarRecord,0,sizeof(tar_record));
       strcpy(TarRecord.header.name,oFile.c_str());
       strcpy(TarRecord.header.mode,"100644 ");
       strcpy(TarRecord.header.uid,"   764 ");
       strcpy(TarRecord.header.gid,"   144 ");

       fseek(pIn, 0, SEEK_END);
       iSize = ftell(pIn);
       fseek(pIn, 0, SEEK_SET);
       sprintf(TarRecord.header.size,"%11o",iSize);
       TarRecord.header.size[11]='\x20';

       time(&ltime);
       sprintf(TarRecord.header.mtime,"%11o",ltime);
       TarRecord.header.mtime[11]='\x20';

       TarRecord.header.typeflag = '0'; // normal file
       strcpy(TarRecord.header.magic,TMAGIC);


       // last, create checksum
       memcpy(TarRecord.header.chksum,CHKBLANKS,8);
       int ctr,sum;
       for (ctr = sum = 0; ctr < (int)sizeof(tar_record); ctr++) 
           sum += TarRecord.charptr[ctr];
       sprintf(TarRecord.header.chksum,"%6o",sum);

       if (gzwrite(pOut, (void *)&TarRecord, sizeof(tar_record)) != sizeof(tar_record))
       {
           fclose(pIn);
           gzclose(pOut);
           delete pBuffer;
           return kError_WriteFile;
       }


       iPadding = (iSize/512+1)*512-iSize;

       for(; iSize > 0;)
       {
           iBlock = min(iSize, iBufferSize);
           iRead = fread(pBuffer, 1, iBlock, pIn);
           if (iRead != iBlock)
           {
               fclose(pIn);
               gzclose(pOut);
               delete pBuffer;
               return kError_ReadFile;
           }
           iWrite = gzwrite(pOut, pBuffer, iBlock);
           if (iWrite != iBlock)
           {
               fclose(pIn);
               gzclose(pOut);
               delete pBuffer;
               return kError_WriteFile;
           }

           iSize -= iBlock;
       }          
       fclose(pIn);

       if(iPadding)
       {
           memset(pBuffer,0,iPadding);
           gzwrite(pOut,pBuffer,iPadding);
       }
   }
   delete pBuffer;

   // write end-marker (zero-ed tar-record)
   memset(&TarRecord,0,sizeof(tar_record));
   gzwrite(pOut,(void*)&TarRecord, sizeof(tar_record));
   // and padding (and why the hell TAR needs extra 4kb with zeroes?)
   iSize=gztell(pOut);
   iPadding = (iSize/512+1)*512-iSize;
   if(iPadding)
   {
       memset(pBuffer,0,0x1000+iPadding);
       gzwrite(pOut,pBuffer,0x1000+iPadding);
   }

   gzclose(pOut);

   return kError_NoErr;
}

// some trophy from TAR code -------------------------

#define ISODIGIT(Char) \
  ((unsigned char) (Char) >= '0' && (unsigned char) (Char) <= '7')
#define ISSPACE(Char) (__isascii (Char) && isspace (Char))


int from_oct (int digs, char *where)
{
  long value;

  while (ISSPACE (*where))
    {               /* skip spaces */
      where++;
      if (--digs <= 0)
    return -1;      /* all blank field */
    }
  value = 0;
  while (digs > 0 && ISODIGIT (*where))
    {
      /* Scan til nonoctal.  */

      value = (value << 3) | (*where++ - '0');
      --digs;
    }

  if (digs > 0 && *where && !ISSPACE (*where))
    return -1;          /* ended on non-space/nul */

  return value;
}

// ---------------------------- TAR code ends

Error ThemeZip::DecompressThemeZip(const string &oSrcFile,
                                   const string &oDestPath)
{
   FILE   *pOut;
   gzFile  pIn;
   char   *pBuffer;
   int     iWrite, iRead, iSize, iBlock, iPadding;
   int     fDone;
   string  oFile;
   tar_record TarRecord;

   if (oSrcFile.length() == 0)
      return kError_FileNotFound;

   m_oCreatedFiles.clear();

   pIn = gzopen(oSrcFile.c_str(), "rb");
   if (pIn == NULL)
       return kError_FileNotFound;

   pBuffer = new char[iBufferSize];
   fDone = FALSE;

   while(!fDone)
   {
       if (gzread(pIn, &TarRecord, sizeof(tar_record)) != sizeof(tar_record))
       {
           gzclose(pIn);
           return kError_ReadFile;
       }
       // check if we are done
       int ctr,sum;
       for (ctr = sum = 0; ctr < (int)sizeof(tar_record); ctr++) 
            sum += TarRecord.charptr[ctr];
       if(sum==0)
       {
           // empty record detected, done
           fDone = TRUE;
           break;
       }
       // check if checksum is still OK
       int our_sum = from_oct(6,TarRecord.header.chksum);
       memcpy(TarRecord.header.chksum,CHKBLANKS,8);
       for (ctr = sum = 0; ctr < (int)sizeof(tar_record); ctr++) 
            sum += TarRecord.charptr[ctr];
       if(sum!=our_sum)
       {
           gzclose(pIn);
           delete pBuffer;
           CleanupThemeZip();
           return kError_NoDataAvail;
       }
       // yes, can proceed

       oFile = oDestPath + string(DIR_MARKER_STR) + string(TarRecord.header.name);
       pOut = fopen(oFile.c_str(), "wb");
       if (pOut == NULL)
       {
           gzclose(pIn);
           delete pBuffer;
           return kError_FileNotFound;
       }
       m_oCreatedFiles.push_back(oFile);

       iSize = from_oct(11,TarRecord.header.size); 
       iPadding = (iSize/512+1)*512-iSize;

       for(; iSize > 0;)
       {
           iBlock = min(iSize, iBufferSize);
           iRead = gzread(pIn, pBuffer, iBlock);
           if (iRead != iBlock)
           {
               gzclose(pIn);
               fclose(pOut);
               delete pBuffer;
               CleanupThemeZip();
               return kError_ReadFile;
           }
           iWrite = fwrite(pBuffer, 1, iBlock, pOut);
           if (iWrite != iBlock)
           {
               gzclose(pIn);
               fclose(pOut);
               delete pBuffer;
               CleanupThemeZip();
               return kError_WriteFile;
           }

           iSize -= iBlock;
       }          
       fclose(pOut);

       if(iPadding)
       {
           gzseek(pIn, iPadding, SEEK_CUR);
       }

   }
   delete pBuffer;
   gzclose(pIn);

   return kError_NoErr;
}

Error ThemeZip::CleanupThemeZip(void)
{
   vector<string>::iterator i;
   
   for(i = m_oCreatedFiles.begin(); i != m_oCreatedFiles.end(); i++)
   {
   	   if (unlink((*i).c_str()))
           return kError_UnlinkFailed;
   }        

   return kError_NoErr;
}      

#if 0
void main(int argc, char **argv)
{
   ThemeZip a;
   vector<string *> list;
   string src;

   if (argv[1][0] == 'e')
   {
       int i;

       for(i = 3; i < argc; i++)
       {
           string *p;

           p = new string(argv[i]);
           list.push_back(p);
       }
       int r = a.CompressThemeZip(string(argv[2]), list);
       printf("ret: %d", r);
       exit(0);
   }  
   if (argv[1][0] == 'd')
   {
       int r = a.DecompressThemeZip(string(argv[2]), string(argv[3]));
       printf("ret: %d", r);
       exit(0);
   }  
   printf("Did nothing\n");
}
#endif

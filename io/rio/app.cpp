///////////////////////////////////////////////////////////////////////////////
//
//	app.cpp
//
//	see RIO.CPP for version history
//
///////////////////////////////////////////////////////////////////////////////
#include	<stdio.h>
#include	<string.h>
#include	<time.h>
#include	<sys/stat.h>
#include	"std.h"
#include	"rio.h"

// output
#define		INFOSTR					printf
#define		ERRORSTR				INFOSTR

// platform dependencies
#if defined(_WINNT)
	// MS VC++ v5.0 for WinNT v4
	#include	<stdlib.h>
	#define		SIZE_MAXPATH			_MAX_PATH

#elif defined(_WIN32)
	// MS VC++ v5.0 for Win9x
	#include	<stdlib.h>
	#define		SIZE_MAXPATH			_MAX_PATH

#elif defined(__linux__)
	// linux g++
	#include	<unistd.h>
	#include	<values.h>
	#define		SIZE_MAXPATH			PATH_MAX

#elif defined(__TURBOC__)
	// turboc v1.01
	#include	<dir.h>
	#define		SIZE_MAXPATH			MAXPATH

#else
	// not supported
	#error ! ! compiler/platform not supported ! !
#endif

// default port base
#if defined(__alpha)
	#define		PORT_BASE_DEFAULT		0x3bc
#else
	#define		PORT_BASE_DEFAULT		0x378
#endif

// return code
#define		CLEANUP_RETURN( ret )		\
{ 										\
	if ( pRio ) 						\
	{ 									\
		delete pRio; 					\
		pRio = NULL; 					\
	} 									\
	if ( pszFilePlaylistTemp ) 			\
	{ 									\
		unlink( pszFilePlaylistTemp );	\
		pszFilePlaylistTemp = NULL; 	\
	} 									\
	return ret; 						\
}

///////////////////////////////////////////////////////////////////////////////
// return pointer to static string containing datetime
static char* TimeStr( long lValue )
{
	static char szBuf[ 64 ];
	struct tm* psDateTime;
	psDateTime = localtime( &lValue );

	if ( !psDateTime )
		strcpy( szBuf, "INVALID DATE/TIME" );
	else
	{
		sprintf(
			szBuf,
			"%02u/%02u/%02u %02u:%02u:%02u",
			(UINT)psDateTime->tm_mday % 100,
			(UINT)psDateTime->tm_mon+1 % 100,
			(UINT)psDateTime->tm_year % 100,
			(UINT)psDateTime->tm_hour % 100,
			(UINT)psDateTime->tm_min % 100,
			(UINT)psDateTime->tm_sec % 100
		);
	}

	return szBuf;
}

///////////////////////////////////////////////////////////////////////////////
// return file size
static long GetFileSize( char* pszPathFile )
{
	long lReturn = 0;

	FILE* fpFile = fopen( pszPathFile, "rb" );
	if ( fpFile )
	{
		struct stat sStat;
		if ( !stat(pszPathFile, &sStat) )
			lReturn = sStat.st_size;

		fclose( fpFile );
	}

	return lReturn;
}

///////////////////////////////////////////////////////////////////////////////
// progress callback
static BOOL ProgressCallback( int iPos, int iCount )
{
	INFOSTR( "blocks %-5hd\r", iCount-iPos );
	fflush( stdout );
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// display directory
static void DisplayDirectory( CRio& cRio, BOOL bVerbose )
{
	CDirBlock& cDirBlock = cRio.GetDirectoryBlock();

	CDirHeader& cDirHeader = cDirBlock.m_cDirHeader;
	INFOSTR( "\n" );
	INFOSTR( "  entry count %hu\n", cDirHeader.m_usCountEntry );
	INFOSTR( " total memory %ld KB\n", ((long)cDirHeader.m_usCount32KBlockAvailable * CRIO_SIZE_32KBLOCK) / 1024 );
	INFOSTR( "  used memory %ld KB\n", ((long)cDirHeader.m_usCount32KBlockUsed * CRIO_SIZE_32KBLOCK) / 1024 );
	INFOSTR( "unused memory %ld KB\n", ((long)cDirHeader.m_usCount32KBlockRemaining * CRIO_SIZE_32KBLOCK) / 1024 );
	if ( bVerbose )
	{
		INFOSTR( "  last update %s\n", TimeStr(cDirHeader.m_lTimeLastUpdate) );
		INFOSTR( "    checksum1 0x%04hx\n", cDirHeader.m_usChecksum1 );
		INFOSTR( "    checksum2 0x%04hx\n", cDirHeader.m_usChecksum2 );
		INFOSTR( " host version 0x%04hx\n", cDirHeader.m_usVersion );
	}

	UINT uiCountEntry = cDirHeader.m_usCountEntry;
	if ( uiCountEntry )
	{
		CDirEntry* pDirEntry = cDirBlock.m_acDirEntry;
		if ( uiCountEntry > CRIO_MAX_DIRENTRY )
			uiCountEntry = CRIO_MAX_DIRENTRY;

		// extended output
		if ( bVerbose )
		{
			INFOSTR( "\n" );
			INFOSTR( "No 32KPos 32KCount Mod32K Size     Upload Date/Time  Title\n" );
			INFOSTR( "-----------------------------------------------------------------------------\n" );

			for( UINT uiA=0; uiA<uiCountEntry; ++uiA, ++pDirEntry )
			{
				INFOSTR( "%02u 0x%04hx %-4hu     0x%04hx %-8ld %s %-24.24s\n",
					uiA+1,
					pDirEntry->m_usPos32KBlock,
					pDirEntry->m_usCount32KBlock,
					pDirEntry->m_usSize32KMod,
					pDirEntry->m_lSize,
					TimeStr(pDirEntry->m_lTimeUpload),
					pDirEntry->m_szName
				);
			}
		}
		// normal output
		else
		{
			INFOSTR( "\n" );
			INFOSTR( "No  Size      Upload Date/Time   Title\n" );
			INFOSTR( "-----------------------------------------------------------------------------\n" );

			for( UINT uiA=0; uiA<uiCountEntry; ++uiA, ++pDirEntry )
			{
				INFOSTR( "%02u  %-8ld  %s  %-40.40s\n",
					uiA+1,
					pDirEntry->m_lSize,
					TimeStr(pDirEntry->m_lTimeUpload),
					pDirEntry->m_szName
				);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// process playlist file
static BOOL ProcessPlaylist( CRio& cRio, char* pszFile, BOOL bVerbose )
{
	char szBuf[ SIZE_MAXPATH ];

	// open playlist for read
	FILE* fpFile = fopen( pszFile, "r" );
	if ( !fpFile )
	{
		ERRORSTR( "unable to open '%s' for read\n", pszFile );
		return FALSE;
	}

	// check if device can accommodate all files in playlist
	CDirBlock& cDirBlock = cRio.GetDirectoryBlock();
	CDirHeader& cDirHeader = cDirBlock.m_cDirHeader;
	long lSizeAvailable = (long)cDirHeader.m_usCount32KBlockAvailable * CRIO_SIZE_32KBLOCK;
	long lSizeCurrent = (long)cDirHeader.m_usCount32KBlockUsed * CRIO_SIZE_32KBLOCK;
	int iCountEntryCurrent = cDirHeader.m_usCountEntry;
	while( fgets(szBuf, sizeof(szBuf), fpFile) )
	{
		// strip 'new line' char
		szBuf[ strlen(szBuf)-1 ] = 0;

		// get file size
		long lSize = GetFileSize( szBuf );
		if ( !lSize )
		{
			ERRORSTR( "unable to open '%s' for read\n", szBuf );
			fclose( fpFile );
			return FALSE;
		}

		// check space
		lSizeCurrent += lSize;
		if ( lSizeCurrent > lSizeAvailable )
		{
			ERRORSTR( "entry '%s' exceed's memory capacity of device\n", szBuf );
			fclose( fpFile );
			return FALSE;
		}

		// check enough entries
		++iCountEntryCurrent;
		if ( iCountEntryCurrent > CRIO_MAX_DIRENTRY )
		{
			ERRORSTR( "entry '%s' exceed's maximum directory entry count for device\n", szBuf );
			fclose( fpFile );
			return FALSE;
		}
	}

	// rewind playlist
	rewind( fpFile );

	// process entries
	while( fgets(szBuf, sizeof(szBuf), fpFile) )
	{
		// strip 'new line' char
		szBuf[ strlen(szBuf)-1 ] = 0;

		// upload
		if ( bVerbose )
			INFOSTR( "uploading %s\n", szBuf );
		if ( !cRio.TxFile(szBuf, bVerbose ? ProgressCallback : NULL) )
		{
			ERRORSTR( "tx file failed, %s\n", cRio.GetErrorStr() );
			fclose( fpFile );
			return FALSE;
		}
	}

	// close playlist file
	fclose( fpFile );

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// display help
static void Help( void )
{
	INFOSTR( "\nRio utility v%d.%02d - Ashpool Systems (c) 1999\n", CRIO_ID_VERSION/100, CRIO_ID_VERSION%100 );
	INFOSTR( "--------------------------------------------\n" );
	INFOSTR( "command line switches available :-\n" );
	INFOSTR( "	-d display directory\n" );
	INFOSTR( "	-i initialize\n" );
	INFOSTR( "	-u specify file to upload\n" );
	INFOSTR( "	-g specify file to download\n" );
	INFOSTR( "	-f specify text based playlist file which contains files to be upload\n" );
	INFOSTR( "	-z specify file to delete\n" );
	INFOSTR( "	-p specify parallel port base IO address, default=0x%x\n", PORT_BASE_DEFAULT );
	INFOSTR( "	-v enable verbose mode\n" );
	INFOSTR( "examples...\n" );
	INFOSTR( "	; display directory using parallel port at 0x278\n" );
	INFOSTR( "		rio -p 0x278 -d\n" );
	INFOSTR( "	; initialize and upload file\n" );
	INFOSTR( "		rio -i -u song.mp3\n" );
	INFOSTR( "	; initialize and upload files in playlist\n" );
	INFOSTR( "		rio -i -f playlist.txt\n" );
	INFOSTR( "	; initialize, upload files in playlist and then display directory\n" );
	INFOSTR( "		rio -d -i -f playlist.txt\n" );
	INFOSTR( "	; download file then delete it and finally display directory\n" );
	INFOSTR( "		rio -d -g mp3Files/song.mp3 -z song.mp3\n" );
}

///////////////////////////////////////////////////////////////////////////////
int main( int iCountArg, char* paszArg[] )
{
	// default settings
	BOOL bDisplayDir = FALSE;
	BOOL bInit = FALSE;
	BOOL bVerbose = FALSE;
	CRio* pRio = NULL;
	char* pszFileDelete = NULL;
	char* pszFileDownload = NULL;
	char* pszFilePlaylist = NULL;
	char* pszFilePlaylistTemp = NULL;
	int iPortBase = PORT_BASE_DEFAULT;

	// process command line args
	if ( iCountArg < 2 )
	{
		Help();
		CLEANUP_RETURN( FALSE );
	}
	for( int iA=1; iA<iCountArg; ++iA )
	{
		// check for display directory request
		if ( !strcmp(paszArg[iA], "-d") )
			bDisplayDir = TRUE;
		// check for init request
		else if ( !strcmp(paszArg[iA], "-i") )
			bInit = TRUE;
		// check for verbose mode request
		else if ( !strcmp(paszArg[iA], "-v") )
			bVerbose = TRUE;
		// check for delete file request
		else if ( !strcmp(paszArg[iA], "-z") )
		{
			if ( (iA+1) < iCountArg )
			{
				++iA;
				pszFileDelete = paszArg[iA];
			}
		}
		// check for upload file request
		else if ( !strcmp(paszArg[iA], "-u") )
		{
			if ( (iA+1) < iCountArg )
			{
				++iA;

				// append all command line upload requests to temp playlist
				if ( !pszFilePlaylistTemp )
					pszFilePlaylistTemp = tmpnam( NULL );

				if ( pszFilePlaylistTemp )
				{
					FILE* fpFile = fopen( pszFilePlaylistTemp, "a" );
					if ( !fpFile )
					{
						ERRORSTR( "unable to open '%s' for amend\n", pszFilePlaylistTemp );
						CLEANUP_RETURN( FALSE );
					}
					fprintf( fpFile, "%s\n", paszArg[iA] );
					fclose( fpFile );
				}
			}
		}
		// check for download file request
		else if ( !strcmp(paszArg[iA], "-g") )
		{
			if ( (iA+1) < iCountArg )
			{
				++iA;
				pszFileDownload = paszArg[iA];
			}
		}
		// check for playlist file request
		else if ( !strcmp(paszArg[iA], "-f") )
		{
			if ( (iA+1) < iCountArg )
			{
				++iA;
				pszFilePlaylist = paszArg[iA];
			}
		}
		// check for port base request
		else if ( !strcmp(paszArg[iA], "-p") )
		{
			if ( (iA+1) < iCountArg )
			{
				++iA;
				sscanf( paszArg[iA], "%x", &iPortBase );
			}
		}
		// else help
		else
		{
			Help();
			CLEANUP_RETURN( FALSE );
		}
	}

	// setup
	pRio = new CRio;
	if ( !pRio )
	{
		ERRORSTR( "new failed, unable to create CRio class\n" );
		CLEANUP_RETURN( FALSE );
	}
	CRio& cRio = *pRio;
	if ( !cRio.Set(iPortBase) )
	{
		ERRORSTR( "%s\n", cRio.GetErrorStr() );
		CLEANUP_RETURN( FALSE );
	}

	// check device present
	if ( bVerbose )
		INFOSTR( "check device present\n" );
	if ( !cRio.CheckPresent() )
	{
		ERRORSTR( "%s\n", cRio.GetErrorStr() );
		CLEANUP_RETURN( FALSE );
	}

	// update directory flag
	BOOL bUpdateDirectory = FALSE;

	// get current directory
	if ( bVerbose )
		INFOSTR( "downloading directory\n" );
	if ( !cRio.RxDirectory() )
	{
		if ( cRio.GetErrorID() == CRIO_ERROR_CORRUPT )
			ERRORSTR( "%s\n", cRio.GetErrorStr() );
		else
		{
			ERRORSTR( "rx directory failed, %s\n", cRio.GetErrorStr() );
			CLEANUP_RETURN( FALSE );
		}
	}

	// if init request
	if ( bInit )
	{
		if ( bVerbose )
			INFOSTR( "initializing\n" );

		cRio.Initialize();

		bUpdateDirectory = TRUE;
	}

	// if playlist request
	if ( pszFilePlaylist )
	{
		if ( !ProcessPlaylist(cRio, pszFilePlaylist, bVerbose) )
			CLEANUP_RETURN( FALSE );

		bUpdateDirectory = TRUE;
	}

	// if temp playlist request
	if ( pszFilePlaylistTemp )
	{
		if ( !ProcessPlaylist(cRio, pszFilePlaylistTemp, bVerbose) )
			CLEANUP_RETURN( FALSE );

		bUpdateDirectory = TRUE;
	}

	// if download request
	if ( pszFileDownload )
	{
		if ( bVerbose )
			INFOSTR( "downloading %s\n", pszFileDownload );

		if ( !cRio.RxFile(pszFileDownload, bVerbose ? ProgressCallback : NULL) )
		{
			ERRORSTR( "rx file failed, %s\n", cRio.GetErrorStr() );
			CLEANUP_RETURN( FALSE );
		}
	}

	// if delete request
	if ( pszFileDelete )
	{
		if ( bVerbose )
			INFOSTR( "deleting %s\n", pszFileDelete );

		if ( !cRio.RemoveFile(pszFileDelete) )
		{
			ERRORSTR( "delete file failed, %s\n", cRio.GetErrorStr() );
			CLEANUP_RETURN( FALSE );
		}

		bUpdateDirectory = TRUE;
	}

	// if directory update required
	if ( bUpdateDirectory )
	{
		if ( bVerbose )
			INFOSTR( "updating directory\n" );

		if ( !cRio.TxDirectory() )
		{
			ERRORSTR( "tx directory failed, %s\n", cRio.GetErrorStr() );
			CLEANUP_RETURN( FALSE );
		}
	}

	// if display directory request
	if ( bDisplayDir )
		DisplayDirectory( cRio, bVerbose );

	// restore
	CLEANUP_RETURN( TRUE );
}


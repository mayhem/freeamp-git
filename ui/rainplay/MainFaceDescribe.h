/***************************************************************/
//FileName:	MainFaceDescribe.h
//Function:	The define of skin coordinates of rainplay.
//Author:	Bill Yuan
//EMail:	rainman_yuan@hotmail.com
//			rainplay@263.net
//WWW:		http://rainman_yuan.yeah.net
//			http://rainplay.yeah.net
//Date:		Aug/18/1998
/***************************************************************/
#ifndef	_MAINFACEDESCRIBE_H_
#define	_MAINFACEDESCRIBE_H_
#include "StdAfx.h"

typedef struct _tagCoordinates {
	BOOL bIsButton;				//这个区域是否为按钮
	char szAreaName[MAX_PATH];	//区域名称
	int x;						//X
	int y;						//Y
	int iWidth;					//宽度
	int iHeight;				//高度
} COORDINATES;

#define MAX_AREA				24		//最大区域数目
#define	AREA_ABOUT				0
#define	AREA_MINIMIZE			1
#define	AREA_QUIT				2
#define	AREA_PREV				3
#define	AREA_STOP				4
#define	AREA_PAUSE				5
#define	AREA_PLAY				6
#define	AREA_NEXT				7
#define	AREA_OPEN				8
#define	AREA_VOLUMEDOWN			9
#define	AREA_VOLUMEUP			10
#define	AREA_PLAYLIST			11
#define	AREA_SHUFFLE			12
#define	AREA_REPEAT				13
#define	AREA_DISPLAYBAR			14
#define	AREA_DISPLAYTIME		15
#define	AREA_DISPLAYBITRATE		16
#define	AREA_DISPLAYKHZ			17
#define	AREA_DISPLAYREPEAT		18
#define	AREA_DISPLAYSHUFFLE		19
#define	AREA_DISPLAYSTEREO		20
#define	AREA_DISPLAYSPECTRUM	21
#define	AREA_SEEKBAR			22
#define	AREA_EQ					23
#define	AREA_RECT(r)			LPRECT(new CRect(coordinatesMain[r].x, \
										coordinatesMain[r].y, \
										coordinatesMain[r].x+coordinatesMain[r].iWidth, \
										coordinatesMain[r].y+coordinatesMain[r].iHeight))
CString	szFontName = "Arial";
CString oldszFontName = "Arial";
COLORREF rgbDisplayColur = RGB(255,255,255);
COLORREF oldrgbDisplayColur = RGB(255,255,255);

COORDINATES coordinatesMain[MAX_AREA] = {
	TRUE,	"About",			7, 8, 12, 13,			//About
	TRUE,	"Minimize",			325, 55, 10, 9,			//Minimize
	TRUE,	"Quit",				311, 9, 24, 14,			//Quit
	TRUE,	"Prev",				19, 9, 20, 14,			//Prev
	TRUE,	"Stop",				25, 43, 35, 17,			//Stop
	TRUE,	"Pause",			70, 49, 9, 9,			//Pause
	TRUE,	"Play",				25, 23, 35, 16,			//Play
	TRUE,	"Next",				46, 10, 20, 14,			//Next
	TRUE,	"Open",				310, 55, 11, 9,			//Open
	TRUE,	"VolumeDown",		0, 0, 0, 0,				//VolumeDown
	TRUE,	"VolumeUp",			0, 0, 0, 0,				//VolumeUp
	TRUE,	"Playlist",			70, 38, 9, 9,			//Playlist
	TRUE,	"Shuffle",			70, 16, 9, 9,			//Shuffle
	TRUE,	"Repeat",			70, 27, 9, 9,			//Repeat
	FALSE,	"DisplayBar",		81, 49, 159, 14,		//DisplayBar
	FALSE,	"DisplayTime",		240, 45, 50, 20,		//DisplayTime
	FALSE,	"DisplayBitrate",	245, 10, 18, 12,		//DisplayBitrate
	FALSE,	"DisplayKhz",		250, 21, 18, 12,		//DisplayKhz
	FALSE,	"DisplayRepeat",	82, 23, 38, 13,			//DisplayRepeat
	FALSE,	"DisplayShuffle",	82, 12, 38, 10,			//DisplayShuffle
	FALSE,	"DisplayStereo",	0, 0, 0, 0,				//DisplayStereo
	TRUE,	"DisplaySpectrum",	121, 12, 115, 22,		//DisplaySpectrum
	TRUE,	"SeekBar",			82, 36, 200, 8,			//SeekBar
	TRUE,	"Equalizer",		8, 55, 12, 12			//Equalizer
};

RECT	oldSkinCoord[MAX_AREA] = {
	7, 8, 12, 13,			//About
	325, 55, 10, 9,			//Minimize
	311, 9, 24, 14,			//Quit
	19, 9, 20, 14,			//Prev
	25, 43, 35, 17,			//Stop
	70, 49, 9, 9,			//Pause
	25, 23, 35, 16,			//Play
	46, 10, 20, 14,			//Next
	310, 55, 11, 9,			//Open
	0, 0, 0, 0,				//VolumeDown
	0, 0, 0, 0,				//VolumeUp
	70, 38, 9, 9,			//Playlist
	70, 16, 9, 9,			//Shuffle
	70, 27, 9, 9,			//Repeat
	81, 49, 159, 14,		//DisplayBar
	240, 45, 50, 20,		//DisplayTime
	245, 10, 18, 12,		//DisplayBitrate
	250, 21, 18, 12,		//DisplayKhz
	82, 23, 38, 13,			//DisplayRepeat
	82, 12, 38, 10,			//DisplayShuffle
	0, 0, 0, 0,				//DisplayStereo
	121, 12, 115, 22,		//DisplaySpectrum
	82, 36, 200, 8,			//SeekBar
	8, 55, 12, 12			//Equalizer
};

#endif //_MAINFACEDESCRIBE_H_
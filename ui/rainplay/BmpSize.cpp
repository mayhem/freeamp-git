/***************************************************************/
//FileName:	BmpSize.cpp
//Function:	bmp and it's size
//			implementation of the CBmpSize class.
//Author:	Bill Yuan
//EMail:	rainman_yuan@hotmail.com
//			rainplay@263.net
//WWW:		http://rainman_yuan.yeah.net
//			http://rainplay.yeah.net
//Date:		Feb/04/1998
/***************************************************************/
#include "stdafx.h"
#include "Rainplay.h"
#include "BmpSize.h"
#include "Mutex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/***************************************************************/
//FuncName:	CBmpSize
//Class:	CBmpSize
//Function:	construction with the ID of bitmap
//Author:	Bill Yuan
//Para:		uID		:		bitmap ID
//Return:	bool
//Date:		Feb/04/1998
/***************************************************************/
CBmpSize::CBmpSize(UINT uID /*= 0*/)
{
	m_iWidth = 0;
	m_iHeight = 0;
	m_Mutex = new Mutex();

	if (uID != 0)
		LoadBitmap(uID);
}

CBmpSize::~CBmpSize()
{
	//Delete the bitmap
	m_cBmp.DeleteObject();
	::DeleteObject(hBitmap);
	if (m_Mutex) {
		delete m_Mutex;
		m_Mutex = NULL;
	}

}


BOOL CBmpSize::LoadBitmap(LPCTSTR lpszBmpFile)
{
	GetBmpSizeLock();

	hBitmap=(HBITMAP)LoadImage( NULL, lpszBmpFile, IMAGE_BITMAP, 0,0,
		LR_DEFAULTCOLOR |
		LR_LOADFROMFILE |
		LR_CREATEDIBSECTION);
	if (hBitmap==NULL)
		return FALSE;
	if (m_cBmp.Attach(hBitmap)==FALSE)
		return FALSE;
	if (m_cBmp.GetBitmap(&m_sBmp)==0)
		return FALSE;
	m_iWidth = m_sBmp.bmWidth;
	m_iHeight = m_sBmp.bmHeight;

	ReleaseBmpSizeLock();
	return TRUE;
}

/***************************************************************/
//FuncName:	LoadBitmap
//Class:	CBmpSize
//Function:	Init the class with the bitmap ID.
//			Load the bitmap to get the width and height
//Author:	Bill Yuan
//Para:		uID		:		bitmap ID
//Return:	bool
//Date:		Feb/04/1998
/***************************************************************/
BOOL CBmpSize::LoadBitmap(UINT uID)
{
	GetBmpSizeLock();

	//Load the bitmap from the ID resource
	//Get the size of the loaded bitmap
	if ( m_cBmp.LoadBitmap(uID) &&
		m_cBmp.GetBitmap(&m_sBmp) ) {
		//Set the public value
		m_iWidth = m_sBmp.bmWidth;
		m_iHeight = m_sBmp.bmHeight;
		return TRUE;
	}

	ReleaseBmpSizeLock();
	return FALSE;
}


/***************************************************************/
//FuncName:	BmpBlt
//Class:	CBmpSize
//Function:	blt the bitmap
//Author:	Bill Yuan
//Para:		pWnd	:		parent
//			xDest	:		the 'x' of target.
//			yDest	:		the 'y' of target.
//			iDx		:		the delta of 'x' coord, default = the width of bitmap
//			iDy		:		the delta of 'y' coord, default = the height of bitmap
//			iFromWhereX:	the 'x' coord of the source to copy, default = 0
//			iFromWhereY:	the 'y' coord of the source to copy, default = 0
//Return:	bool
//Date:		Feb/04/1998
/***************************************************************/
BOOL CBmpSize::BmpBlt(BOOL IsPaint, CWnd *pWnd, int xDest, int yDest, int iDx, int iDy, int iFromWhereX, int iFromWhereY)
{
	GetBmpSizeLock();

	CDC dcMem;
	
	if ( IsPaint ) {
		CPaintDC dcPaint(pWnd);
		if( !dcMem.CreateCompatibleDC(&dcPaint) )
			return FALSE;
		dcMem.SelectObject(&m_cBmp);
		return( dcPaint.BitBlt(xDest, yDest,
		(iDx==-1) ? m_iWidth : iDx,
		(iDy==-1) ? m_iHeight : iDy, 
		&dcMem,
		(iFromWhereX==-1) ? 0 : iFromWhereX,
		(iFromWhereY==-1) ? 0 : iFromWhereY,
		SRCCOPY) );
	} else {
		CClientDC dcClient(pWnd);
		if ( !dcMem.CreateCompatibleDC(&dcClient) )
			return FALSE;
		dcMem.SelectObject(&m_cBmp);
		return( dcClient.BitBlt(xDest, yDest,
		(iDx==-1) ? m_iWidth : iDx,
		(iDy==-1) ? m_iHeight : iDy, 
		&dcMem,
		(iFromWhereX==-1) ? 0 : iFromWhereX,
		(iFromWhereY==-1) ? 0 : iFromWhereY,
		SRCCOPY) );
	}

	ReleaseBmpSizeLock();
}


/***************************************************************/
//FuncName:	MakeRegion
//Class:	CBmpSize
//Function:	Make region according to the bitmap
//Author:	Bill Yuan
//Para:		NONE
//Return:	HRGN : region
//Date:		Aug/07/1998
/***************************************************************/
HRGN CBmpSize::MakeRegion()
{
	GetBmpSizeLock();

	HRGN rgnc, rgnl;
	COLORREF tc;
	int x, y, l = 0;
	BOOL ff = TRUE, inf = FALSE;
	CDC cMemDC;
	cMemDC.CreateCompatibleDC(NULL);
	cMemDC.SelectObject(m_cBmp);

	for(y = 0; y < m_iHeight; y++) {
		for(x = 0; x <= m_iWidth; x++) {
			if (x==0||y==0)
				tc = cMemDC.GetPixel(0,0);
			if( (cMemDC.GetPixel(x, y) == tc) || (x == m_iWidth))	{
				if( inf ) {
					inf = FALSE;
					rgnl = CreateRectRgn(l, y, x, y+1);
					if( ff ) {
						rgnc = rgnl;
						ff = FALSE;
					} else {
						CombineRgn(rgnc, rgnc, rgnl, RGN_OR);
					}
				}
			} else {
				if( !inf ) {
					inf = TRUE;
					l = x;
				}
			}
		}
	}
	
	ReleaseBmpSizeLock();
	return rgnc;
}

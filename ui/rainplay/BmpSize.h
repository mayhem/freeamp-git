/***************************************************************/
//FileName:	BmpSize.h
//Function:	bmp and it's size
//			interface for the CBmpSize class.
//Author:	Bill Yuan
//EMail:	rainman_yuan@hotmail.com
//			rainplay@263.net
//WWW:		http://rainman_yuan.yeah.net
//			http://rainplay.yeah.net
//Date:		Feb/04/1998
/***************************************************************/
#if !defined(AFX_BMPSIZE_H__D2747CC3_9DA1_11D1_BF2C_0000B423931A__INCLUDED_)
#define AFX_BMPSIZE_H__D2747CC3_9DA1_11D1_BF2C_0000B423931A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CBmpSize : public CBitmap  
{
public:
	CBmpSize(UINT uID = 0);
	virtual ~CBmpSize();

protected:
	BITMAP	m_sBmp;
	HBITMAP hBitmap;

public:
	BOOL LoadBitmap(LPCTSTR lpszBmpFile);
	BOOL LoadBitmap( UINT uID );	//Load bitmap and get the size
	BmpBlt(BOOL IsPaint, CWnd *pWnd,
		int xDest, int yDest, int iDx = 0, int iDy = 0,
		int iFromWhereX = -1, int iFromWhereY = -1);
	HRGN MakeRegion();
	CBitmap m_cBmp;
	int	m_iWidth;
	int m_iHeight;
};
#endif // !defined(AFX_BMPSIZE_H__D2747CC3_9DA1_11D1_BF2C_0000B423931A__INCLUDED_)

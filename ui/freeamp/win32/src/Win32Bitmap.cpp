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

#include "string"
#include "Win32Bitmap.h"
#include "debug.h"


Win32Bitmap::Win32Bitmap(string &oName)
          :Bitmap(oName)
{
   m_oBitmapName = oName;
   m_hBitmap = NULL;
   m_hMaskBitmap = NULL;
}

Win32Bitmap::Win32Bitmap(int iWidth, int iHeight, string &oName)
           :Bitmap(oName)
{
   HDC hDc;
   
   hDc = GetDC(NULL);
   m_hBitmap = CreateCompatibleBitmap(hDc, iWidth, iHeight);
   ReleaseDC(NULL, hDc);
}

Win32Bitmap::~Win32Bitmap(void)
{
   if (m_hBitmap)
       DeleteObject(m_hBitmap);
}

Error Win32Bitmap::LoadBitmapFromDisk(string &oFile)
{
   m_hBitmap = (HBITMAP)LoadImage(NULL, oFile.c_str(), IMAGE_BITMAP,
                                  0, 0, LR_LOADFROMFILE);
   if (m_hBitmap == NULL)
   	   return kError_LoadBitmapFailed;

   if (m_bHasTransColor)
      CreateMaskBitmap();

   return kError_NoErr;
}

HBITMAP Win32Bitmap::GetBitmapHandle(void)
{
   return m_hBitmap;
}

HBITMAP Win32Bitmap::GetMaskBitmapHandle(void)
{
   return m_hMaskBitmap;
}

static int first = 0;

void Win32Bitmap::CreateMaskBitmap(void)
{
   BITMAP      sInfo;
   BITMAPINFO *pInfo, *pMaskInfo;
   HDC         hRootDC, hMemDC;
   int         iRet, iLine, iCol;
   char       *pData, *pMaskData;
   Color       sTrans, *pColorPtr;
   COLORREF   *pColorRefPtr;

   hRootDC = GetDC(NULL);
   hMemDC = CreateCompatibleDC(hRootDC);
   ReleaseDC(NULL, hRootDC);

   if (m_hMaskBitmap)
      DeleteObject(m_hMaskBitmap);

   GetObject(m_hBitmap, sizeof(BITMAP), (LPSTR)&sInfo);
   m_hMaskBitmap = CreateBitmap(sInfo.bmWidth, sInfo.bmHeight, 1, 1, NULL);
   
   pInfo = (BITMAPINFO *)new char[sizeof(BITMAPINFOHEADER)];
   pInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   pInfo->bmiHeader.biWidth = sInfo.bmWidth;
   pInfo->bmiHeader.biHeight = sInfo.bmHeight;
   pInfo->bmiHeader.biPlanes = 1;
   pInfo->bmiHeader.biBitCount = 24;
   pInfo->bmiHeader.biCompression = BI_RGB;
   pInfo->bmiHeader.biSizeImage = 0;
   pInfo->bmiHeader.biXPelsPerMeter = 0;
   pInfo->bmiHeader.biYPelsPerMeter = 0;
   pInfo->bmiHeader.biClrUsed = 0;
   pInfo->bmiHeader.biClrImportant = 0;

   pMaskInfo = (BITMAPINFO *)new char[sizeof(BITMAPINFOHEADER) + 
                                      64 * sizeof(RGBQUAD)];
   pMaskInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   pMaskInfo->bmiHeader.biWidth = sInfo.bmWidth;
   pMaskInfo->bmiHeader.biHeight = sInfo.bmHeight;
   pMaskInfo->bmiHeader.biPlanes = 1;
   pMaskInfo->bmiHeader.biBitCount = 1;
   pMaskInfo->bmiHeader.biCompression = BI_RGB;
   pMaskInfo->bmiHeader.biSizeImage = 0;
   pMaskInfo->bmiHeader.biXPelsPerMeter = 0;
   pMaskInfo->bmiHeader.biYPelsPerMeter = 0;
   pMaskInfo->bmiHeader.biClrUsed = 2;
   pMaskInfo->bmiHeader.biClrImportant = 0;
   pColorRefPtr = (COLORREF *)(&pMaskInfo->bmiColors);
   pColorRefPtr[0] = RGB(255,255,255);
   pColorRefPtr[1] = RGB(0,0,0);

   pData = new char[sInfo.bmWidth * 4];
   pMaskData = new char[(sInfo.bmWidth / 8) + 4];
   for(iLine = 0; iLine < sInfo.bmHeight; iLine++)
   {
       iRet = GetDIBits(hMemDC, m_hBitmap, 
                        (sInfo.bmHeight - 1) - iLine, 1, 
                        pData, pInfo, DIB_RGB_COLORS);
       if (!iRet)
       {
          Debug_v("GetDIBits failed. Last Error: %d", GetLastError());
          break;
       }   

       memset(pMaskData, 0x00, (sInfo.bmWidth / 8) + 4);
   	   for(iCol = 0, pColorPtr = (Color *)pData; 
           iCol < sInfo.bmWidth; iCol++, pColorPtr++)
       {    
   		  if (pColorPtr->red != m_oTransColor.blue ||
   		      pColorPtr->green != m_oTransColor.green ||
   		      pColorPtr->blue != m_oTransColor.red)
          {
             pMaskData[iCol / 8] |= (0x80 >> (iCol % 8));
          }   
       }      
             
       iRet = SetDIBits(hMemDC, m_hMaskBitmap, 
                        (sInfo.bmHeight - 1) - iLine, 1, 
                        pMaskData, pMaskInfo, DIB_RGB_COLORS);
       if (!iRet)
       {
          Debug_v("SetDIBits failed. Last Error: %d", GetLastError());
          break;
       }   
   }

   delete pData;
   delete pMaskData;
   delete pInfo;
   delete pMaskInfo;
   DeleteDC(hMemDC);
}

bool Win32Bitmap::IsPosVisible(Pos &oPos)
{
   HDC      hRootDC, hMemDC;
   COLORREF sColor;
   HBITMAP  hSaved;

   if (!m_hMaskBitmap)
      return true;

   hRootDC = GetDC(NULL);
   hMemDC = CreateCompatibleDC(hRootDC);
   ReleaseDC(NULL, hRootDC);

   hSaved = (HBITMAP)SelectObject(hMemDC, m_hMaskBitmap);
   sColor = GetPixel(hMemDC, oPos.x, oPos.y);
   SelectObject(hMemDC, hSaved);
   DeleteDC(hMemDC);
   
   return sColor == 0;
}

Error Win32Bitmap::BlitRect(Bitmap *pOther, Rect &oSrcRect, 
                            Rect &oDestRect)
{
   HDC hRootDC, hSrcDC, hDestDC;
   Win32Bitmap *pSrcBitmap = (Win32Bitmap *)pOther;
   
   hRootDC = GetDC(NULL);
   hSrcDC = CreateCompatibleDC(hRootDC);
   hDestDC = CreateCompatibleDC(hRootDC);
   ReleaseDC(NULL, hRootDC);
   
   DeleteObject(SelectObject(hSrcDC, pSrcBitmap->m_hBitmap));
   DeleteObject(SelectObject(hDestDC, m_hBitmap));

   BitBlt(hDestDC, oDestRect.x1, oDestRect.y1, 
          oDestRect.Width(), oDestRect.Height(),
   	      hSrcDC, oSrcRect.x1, oSrcRect.y1, SRCCOPY);

   DeleteDC(hSrcDC);
   DeleteDC(hDestDC);

   return kError_NoErr;
}

Error Win32Bitmap::MaskBlitRect(Bitmap *pOther, Rect &oSrcRect, 
                                Rect &oDestRect)
{
   HDC hRootDC, hSrcDC, hDestDC, hMaskDC;
   Win32Bitmap *pSrcBitmap = (Win32Bitmap *)pOther;

   if (!pSrcBitmap->m_hMaskBitmap)
   	  return BlitRect(pOther, oSrcRect, oDestRect);
   
   hRootDC = GetDC(NULL);
   hSrcDC = CreateCompatibleDC(hRootDC);
   hDestDC = CreateCompatibleDC(hRootDC);
   hMaskDC = CreateCompatibleDC(hRootDC);
   ReleaseDC(NULL, hRootDC);
   
   DeleteObject(SelectObject(hSrcDC, pSrcBitmap->m_hBitmap));
   DeleteObject(SelectObject(hMaskDC, pSrcBitmap->m_hMaskBitmap));
   DeleteObject(SelectObject(hDestDC, m_hBitmap));

   SetBkColor(hSrcDC, RGB(m_oTransColor.red,m_oTransColor.green,m_oTransColor.blue));
   BitBlt(hDestDC, oDestRect.x1, oDestRect.y1, 
                   oDestRect.Width(), oDestRect.Height(),
          hSrcDC, oSrcRect.x1, oSrcRect.y1, SRCINVERT);
   BitBlt(hDestDC, oDestRect.x1, oDestRect.y1, 
                   oDestRect.Width(), oDestRect.Height(),
          hMaskDC, oSrcRect.x1, oSrcRect.y1, SRCAND);
   BitBlt(hDestDC, oDestRect.x1, oDestRect.y1, 
                   oDestRect.Width(), oDestRect.Height(),
          hSrcDC, oSrcRect.x1, oSrcRect.y1, SRCINVERT);

   DeleteDC(hSrcDC);
   DeleteDC(hDestDC);
   DeleteDC(hMaskDC);

   return kError_NoErr;
}


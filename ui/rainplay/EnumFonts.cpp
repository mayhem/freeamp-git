#include "stdafx.h"

typedef struct _tagEnumFontPara {
	BOOL ok;
	CString szFont;
	CDC *dc;
	LPLOGFONT lpLogFont;
} ENUMFONTPARA, *LPENUMFONTPARA;

static CFont *oldFont = FALSE;
int FAR PASCAL DlgEnumFontNames (LPLOGFONT lpLogFont, 
								 LPTEXTMETRIC lpTextMetric, 
								 short nFontType, 
								 LPHANDLE lpData)
{
	CString szTemp = lpLogFont->lfFaceName;
	LPENUMFONTPARA lpPara = (LPENUMFONTPARA)lpData;
	if (szTemp == lpPara->szFont ) {
		lpPara->lpLogFont->lfCharSet		= lpLogFont->lfCharSet;
		lpPara->lpLogFont->lfClipPrecision	= lpLogFont->lfClipPrecision;
		lpPara->lpLogFont->lfEscapement		= lpLogFont->lfEscapement;
		//lpPara->lpLogFont->lfFaceName		= lpLogFont->lfFaceName;
		strcpy(lpPara->lpLogFont->lfFaceName, lpLogFont->lfFaceName);
		lpPara->lpLogFont->lfHeight			= lpLogFont->lfHeight;
		lpPara->lpLogFont->lfItalic			= lpLogFont->lfItalic;
		lpPara->lpLogFont->lfOrientation	= lpLogFont->lfOrientation;
		lpPara->lpLogFont->lfOutPrecision	= lpLogFont->lfOutPrecision;
		lpPara->lpLogFont->lfPitchAndFamily = lpLogFont->lfPitchAndFamily;
		lpPara->lpLogFont->lfQuality		= lpLogFont->lfQuality;
		lpPara->lpLogFont->lfStrikeOut		= lpLogFont->lfStrikeOut;
		lpPara->lpLogFont->lfUnderline		= lpLogFont->lfUnderline;
		lpPara->lpLogFont->lfWeight			= lpLogFont->lfWeight;
		lpPara->lpLogFont->lfWidth			= lpLogFont->lfWidth;

		lpPara->ok = TRUE;
		return FALSE;
	}

	return TRUE;
}

BOOL EnumUIFont(CClientDC *dc, LPLOGFONT lpLogFont, CString szFont)
{
	FARPROC lpEnumFonts;

	ENUMFONTPARA para;
	para.ok = FALSE;
	para.szFont = szFont;
	para.dc = dc;
	para.lpLogFont = lpLogFont;

    lpEnumFonts = MakeProcInstance((FARPROC)DlgEnumFontNames, GetModuleHandle(NULL));
    if (lpEnumFonts) {
        // The enumeration function will lock down the handle
        EnumFonts (dc->m_hDC, NULL, (FONTENUMPROC)lpEnumFonts, (long)&para);
        // The handle will come back to us properly unlocked
        FreeProcInstance (lpEnumFonts);
     }
	if ( para.ok ) {
		return TRUE;
	}
	else
		return FALSE;
}
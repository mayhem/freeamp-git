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

#include <sys/stat.h>
#include "GTKFont.h"
#include "GTKUtility.h"
#include "win32impl.h"
#include "utility.h"

GTKFont::GTKFont(FAContext *context, string &oName, string &oFace, 
                 string &oDefault) :
                Font(oName, oFace, oDefault)
{
    m_context = context;

    type = kFontTypeUnknown;

    string finalName;    
    vector<string> Names;
    vector<string>::iterator i;

    char *dup = strdup(m_oFace.c_str());
    char *token = strtok(dup, ",");
    while (token) {
         Names.push_back(string(token));
         token = strtok(NULL, ",");
    }

    bool gdkfound = false;
    bool ttffound = false;

#ifdef HAVE_FREETYPE
    char dir[_MAX_PATH];
    uint32 len = sizeof(dir);
    string ttfbase, ttfpath;
    WIN32_FIND_DATA find;
    HANDLE handle;
 
    m_context->prefs->GetInstallDirectory(dir, &len);
    ttfbase = string(dir) + "/../share/freeamp/fonts";
    ttfpath = ttfbase + "/*.ttf";
 
    struct stat st;
    if (-1 == stat(ttfbase.c_str(), &st))
        mkdir(ttfbase.c_str(), 0755);

    handle = FindFirstFile((char *)ttfpath.c_str(), &find);
    if (handle != INVALID_HANDLE_VALUE)
        do { 
            i = Names.begin(); 
            for (; i != Names.end(); i++) {
                if (!strncasecmp(find.cFileName, (*i).c_str(), (*i).length())) {
                    finalName = ttfbase + string("/") + string(find.cFileName);
                    ttffound = true;
                    break;
                }
            }
            if (ttffound == true)
                break;
        } while (FindNextFile(handle, &find));

    ttfbase = FreeampDir(NULL) + string("/fonts");
    if (-1 == stat(ttfbase.c_str(), &st))
        mkdir(ttfbase.c_str(), 0755);

    ttfpath = ttfbase + string("/*.ttf");
    handle = FindFirstFile((char *)ttfpath.c_str(), &find);
    if (handle != INVALID_HANDLE_VALUE)
        do {
            i = Names.begin();
            for (; i != Names.end(); i++) {
                if (!strncasecmp(find.cFileName, (*i).c_str(), (*i).length())) {
                    finalName = ttfbase + string("/") + string(find.cFileName);
                    ttffound = true;
                    break;
                }
            }
            if (ttffound == true)
                break;
        } while (FindNextFile(handle, &find));

    ttfbase = "./fonts";
    ttfpath = ttfbase + string("/*.ttf");
    handle = FindFirstFile((char *)ttfpath.c_str(), &find);
    if (handle != INVALID_HANDLE_VALUE)
        do {
            i = Names.begin();
            for (; i != Names.end(); i++) {
                if (!strncasecmp(find.cFileName, (*i).c_str(), (*i).length())) {
                    finalName = ttfbase + string("/") + string(find.cFileName);
                    ttffound = true;
                    break;
                }
            }
            if (ttffound == true)
                break;
        } while (FindNextFile(handle, &find));
#endif

    if (!ttffound) {
        gdk_threads_enter();
        i = Names.begin();
        for (; i != Names.end(); i++) {
            if (gdkfound = ListFonts((char *)(*i).c_str())) {
                finalName = *i;
                break;
            }
        }
        gdk_threads_leave();
    }

    if (gdkfound) {
        m_oFace = finalName; 
        type = kFontTypeGdk;
    }
#ifdef HAVE_FREETYPE
    else if (ttffound) { 
        m_oFace = finalName;
        type = kFontTypeTTF;
    }
#endif
    else {
        m_oFace = m_oDefault;
        type = kFontTypeGdk;
    }
    free(dup);

    gfont = NULL;
#ifdef HAVE_FREETYPE
    ttfont = NULL;
#endif
    bold = false;
    italic = false;
    underline = false;
    first = true;
}

Error GTKFont::Load(int iFontHeight, bool bBold, bool bItalic)
{
    if (type == kFontTypeGdk) {
        if (bold != bBold || italic != bItalic || size != iFontHeight || 
            !gfont) {
            bold = bBold;
            italic = bItalic;
            size = iFontHeight;
            gdk_threads_enter();
            if (gfont)
                gdk_font_unref(gfont);
            string fontname = BuildFontString(bold, italic, size);
            gfont = gdk_font_load(fontname.c_str());
            if (!gfont) {
                fontname = BuildFontString(bold, italic, size - 1);
                gfont = gdk_font_load(fontname.c_str());
                if (!gfont) {
//    cout << "fallback.. size is " << size << endl;
                    gfont = gdk_font_load("variable");  
                    if (!gfont) {
                        gfont = gdk_font_load("default");
                        if (!gfont) {
                            gdk_threads_leave();
                            cout << "oops, couldn't load _any_ fonts...\n";
                            return kError_YouScrewedUp;
                        }
                    }
                }
            } 
            gdk_threads_leave();
        }
    }
#ifdef HAVE_FREETYPE
    else {
        if (!ttfont || iFontHeight != size) {
            size = iFontHeight;
            if (ttfont)
                Efont_free(ttfont);
            ttfont = Efont_load((char *)m_oFace.c_str(), iFontHeight - 3);
            if (!ttfont) {
                cout << "ERROR loading ttf\n";
                return kError_YouScrewedUp;
            }
        }
    }
#endif
    return kError_NoErr;
}

int GTKFont::GetLength(string &oText)
{
    int retvalue = 0;

    if (type == kFontTypeGdk) {
        gdk_threads_enter();
        retvalue = gdk_string_measure(gfont, oText.c_str());
        gdk_threads_leave();
    } 
#ifdef HAVE_FREETYPE
    else if (type == kFontTypeTTF) {
        Efont_extents(ttfont, (char *)oText.c_str(), NULL, NULL, &retvalue, 
                      NULL, NULL, NULL, NULL);
    }
#endif
    return retvalue;
}

int GTKFont::GetHeight(string &oText)
{
    int retvalue = 0;

    if (type == kFontTypeGdk) {   
       gdk_threads_enter();
       retvalue = gdk_string_height(gfont, oText.c_str());
       gdk_threads_leave();
    }
#ifdef HAVE_FREETYPE
    else if (type == kFontTypeTTF) {
       int ascent = 0, descent = 0;
       Efont_extents(ttfont, (char *)oText.c_str(), &ascent, &descent, NULL, 
                     NULL, NULL, NULL, NULL);
       retvalue = ascent + descent;
    }
#endif
    return retvalue;
}

void GTKFont::Render(Rect &oClipRect, string &oText, int iOffset, 
                     const Color &oColor, GTKBitmap *bitmap, bool bUnderline)
{
    if (!bitmap->GetBitmap())
        return;

    gdk_threads_enter();
    GdkGC *gc = gdk_gc_new((GdkWindow *)bitmap->GetBitmap());
    gdk_rgb_gc_set_foreground(gc, (oColor.red << 16) | (oColor.green << 8) |
                              oColor.blue);

    GdkRectangle clipRect;
    clipRect.x = oClipRect.x1;
    clipRect.y = oClipRect.y1;
    clipRect.width = oClipRect.Width();
    clipRect.height = oClipRect.Height();

    gdk_gc_set_clip_rectangle(gc, &clipRect);

    if (type == kFontTypeGdk) {
       gdk_draw_string(bitmap->GetBitmap(), gfont, gc, oClipRect.x1 + iOffset, 
                       oClipRect.y1 + oClipRect.Height() - 3, oText.c_str());

    // TODO: Underline...
    }
#ifdef HAVE_FREETYPE
    else if (type == kFontTypeTTF) {
       EFont_draw_string(bitmap->GetBitmap(), gc, oClipRect.x1 + iOffset, 
                         oClipRect.y1 + oClipRect.Height() - 3, 
                         (char *)oText.c_str(), ttfont);
    }
#endif
    gdk_flush();
    gdk_gc_unref(gc);
    gdk_threads_leave();

    return;
}

string GTKFont::BuildFontString(bool bBold, bool bItalic, int iFontHeight)
{
   // This isn't the greatest method, as it will probably return stuff we
   // don't have....
   string retstring = "-*-" + m_oFace;
   if (bBold)
       retstring += string("-bold-");
   else
       retstring += string("-medium-");
   if (bItalic)
       retstring += string("i-normal-*-");
   else 
       retstring += string("r-normal-*-");
   char tempstr[16];
   sprintf(tempstr, "%d", iFontHeight);
   retstring += string(tempstr) + string("-*");
   return retstring;
}

GTKFont::~GTKFont(void)
{
    gdk_threads_enter();
    if (type == kFontTypeGdk) {
        if (gfont)
            gdk_font_unref(gfont);
    }
#ifdef HAVE_FREETYPE
    else {
        if (ttfont)
            Efont_free(ttfont);
    }
#endif
    gdk_threads_leave();
}

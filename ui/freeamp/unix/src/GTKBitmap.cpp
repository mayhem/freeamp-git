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
#include "GTKBitmap.h"

#include <gdk/gdk.h>
extern "C" {
#include <gdk/gdkx.h>
}
#include <sys/stat.h>
#include <unistd.h>

// This is basically bmp.c from xmms, although cleaned up and with a couple 
// additions/fixes from imlib's bmp loader.

typedef struct tagRGBQUAD
{
    guchar rgbBlue;
    guchar rgbGreen;
    guchar rgbRed;
    guchar rgbReserved;
}
RGBQUAD;

#define BI_RGB       0
#define BI_RLE8      1
#define BI_RLE4      2
#define BI_BITFIELDS 3

GTKBitmap::GTKBitmap(string &oName)
          :Bitmap(oName)
{
    m_oBitmapName = oName;
    m_Bitmap = NULL;
    m_MaskBitmap = NULL;
    shape_set = false;
    gdk_threads_enter();
    m_GC = gdk_gc_new(gdk_window_foreign_new(GDK_ROOT_WINDOW()));
    gdk_threads_leave();
}

GTKBitmap::GTKBitmap(int iWidth, int iHeight, string &oName) 
          :Bitmap(oName)
{
    m_oBitmapName = oName;
    m_MaskBitmap = NULL;
    shape_set = false;
    gdk_threads_enter();
    m_Bitmap = gdk_pixmap_new(NULL, iWidth, iHeight, 
                              gdk_visual_get_best_depth());
    m_GC = gdk_gc_new(gdk_window_foreign_new(GDK_ROOT_WINDOW()));
    gdk_threads_leave();
}

GTKBitmap::~GTKBitmap(void)
{
    gdk_threads_enter();
    if (m_Bitmap)
        gdk_pixmap_unref(m_Bitmap);
    m_Bitmap = NULL;

    if (m_MaskBitmap)
        gdk_pixmap_unref(m_MaskBitmap);
    m_MaskBitmap = NULL;

    if (m_GC)
        gdk_gc_unref(m_GC);
    m_GC = NULL; 
    gdk_threads_leave();
}

Error GTKBitmap::ReadleShort(FILE *file, gushort *ret)
{
    guchar b[2];

    if (fread(b, sizeof(guchar), 2, file) != 2)
        return kError_LoadBitmapFailed;

    *ret = (b[1] << 8) | b[0];
    return kError_NoErr;
}

Error GTKBitmap::ReadleLong(FILE *file, gulong *ret)
{
    guchar b[4];

    if (fread(b, sizeof(guchar), 4, file) != 4)
        return kError_LoadBitmapFailed;
 
    *ret = (b[3] << 24) | (b[2] << 16) | (b[1] << 8) | b[0];
    return kError_NoErr;
}

Error GTKBitmap::LoadBitmapFromDisk(string &oFile)
{
    FILE *file;
    gchar type[2];
    gulong size, offset, headSize, w, h, comp, imgsize, j, k, l;
    gushort tmpShort, planes, bitcount, ncols, skip;
    guchar tempchar, *data, *data_end, byte = 0, g, b, r, *buffer, *buffer_end;
    struct stat statbuf;
    register guchar *ptr, *buffer_ptr;
    register gulong i;
    register gushort x, y;
    RGBQUAD rgbQuads[256];

    gulong rmask = 0xff, gmask = 0xff, bmask = 0xff;
    gulong rshift = 0, gshift = 0, bshift = 0;

    if (stat(oFile.c_str(), &statbuf) == -1) 
        return kError_LoadBitmapFailed;
    size = statbuf.st_size;

    file = fopen(oFile.c_str(), "rb");
    if (!file) 
        return kError_LoadBitmapFailed;

    if (fread(type, 1, 2, file) != 2) {
        fclose(file);
        return kError_LoadBitmapFailed;
    }
    if (strncmp(type, "BM", 2)) {
        fclose(file);
        return kError_LoadBitmapFailed;
    }
    fseek(file, 8, SEEK_CUR);
    ReadleLong(file, &offset);
    ReadleLong(file, &headSize);
    if (headSize == 12) {
        ReadleShort(file, &tmpShort);
        w = tmpShort;
        ReadleShort(file, &tmpShort);
        h = tmpShort;
        ReadleShort(file, &planes);
        ReadleShort(file, &bitcount);
        imgsize = size - offset;
        comp = BI_RGB;
    }
    else if (headSize == 40) {
        ReadleLong(file, &w);
        ReadleLong(file, &h);
        ReadleShort(file, &planes);
        ReadleShort(file, &bitcount);
        ReadleLong(file, &comp);
        ReadleLong(file, &imgsize);
        imgsize = size - offset;

        fseek(file, 16, SEEK_CUR);
    }
    else {
        fclose(file);
        return kError_LoadBitmapFailed;
    }

    if (bitcount < 16) {
        ncols = (offset - headSize - 14);
        if (headSize == 12) {
            ncols /= 3;
            for (i = 0; i < ncols; i++)
                fread(&rgbQuads[i], 3, 1, file);
        }
        else {
            ncols /= 4;
            fread(rgbQuads, 4, ncols, file);
        }
    }
    else if (bitcount == 16 || bitcount == 32) {
        if (comp == BI_BITFIELDS) {
            ReadleLong(file, &bmask);
            ReadleLong(file, &gmask);
            ReadleLong(file, &rmask);
            for (int bit = bitcount - 1; bit >= 0; bit--) {
                if (bmask & (1 << bit))
                    bshift = bit;
                if (gmask & (1 << bit))
                    gshift = bit;
                if (rmask & (1 << bit))
                    rshift = bit;
            }
        }
        else if (bitcount == 16) {
            rmask = 0x7C00;
            gmask = 0x03E0;
            bmask = 0x001F;
            rshift = 10;
            gshift = 5;
            bshift = 0;
        }
        else if (bitcount == 32) {
            rmask = 0x00FF0000;
            gmask = 0x0000FF00;
            bmask = 0x000000FF;
            rshift = 16;
            gshift = 8;
            bshift = 0;
        }
    }


    fseek(file, offset, SEEK_SET);
    gdk_threads_enter();
    buffer = (guchar *)g_malloc(imgsize);
    data = (guchar *)g_malloc0((w * 3 * h) + 3); // +3 is just for safety
    gdk_threads_leave();
    fread(buffer, imgsize, 1, file);
    fclose(file);
    buffer_ptr = buffer;
    buffer_end = buffer + imgsize;

    data_end = data + (w * 3 * h);
    if (!data) 
        return kError_LoadBitmapFailed;
    ptr = data + ((h - 1) * w * 3);

    if (bitcount == 4) {
        if (comp == BI_RLE4) {
            x = 0;
            y = 0;
            for (i = 0, g = 1; i < imgsize && g && buffer_ptr < buffer_end; i++) {
                byte = *(buffer_ptr++);
                if (byte) {
                    guchar t1, t2;

                    l = byte;
                    byte = *(buffer_ptr++);
                    t1 = byte & 0xF;
                    t2 = (byte >> 4) & 0xF;
                    for (j = 0; j < l; j++) {
                        k = (j & 1) ? t1 : t2;

                        if (x >= w)
                            break;

                        *ptr++ = rgbQuads[k].rgbRed;
                        *ptr++ = rgbQuads[k].rgbGreen;
                        *ptr++ = rgbQuads[k].rgbBlue;
                        x++;
                        if (ptr > data_end)
                            ptr = data_end;

                    }
                }
                else {
                    byte = *(buffer_ptr++);
                    switch (byte) {
                        case 0:
                            x = 0;
                            y++;
                            ptr = data + ((h - y - 1) * w * 3) + (x * 3);
                            if (ptr > data_end)
                                ptr = data_end;
                            break;
                        case 1:
                            g = 0;
                            break;
                        case 2:
                            x += *(buffer_ptr++);
                            y += *(buffer_ptr++);
                            ptr = data + ((h - y - 1) * w * 3) + (x * 3);
                            if (ptr > data_end)
                                ptr = data_end;
                            break;
                        default:
                            l = byte;
                            for (j = 0; j < l; j++) {
                                guchar t1 = '\0', t2 = '\0';
 
                                if ((j & 1) == 0) {
                                    byte = *(buffer_ptr++);
                                    t1 = byte & 0xF;
                                    t2 = (byte >> 4) & 0xF;
                                }
                                k = (j & 1) ? t1 : t2;

                                if (x >= w) {
                                    buffer_ptr += (l - j) / 2;
                                    break;
                                }

                                *ptr++ = rgbQuads[k].rgbRed;
                                *ptr++ = rgbQuads[k].rgbGreen;
                                *ptr++ = rgbQuads[k].rgbBlue;

                                x++;

                                if (ptr > data_end)
                                    ptr = data_end;

                            }

                            if ((l & 3) == 1) {
                                tempchar = *(buffer_ptr++);
                                tempchar = *(buffer_ptr++);
                            }
                            else if ((l & 3) == 2)
                                buffer_ptr++;
                            break;
                    }
                }
            }
        }
        else if (comp == BI_RGB) {
            skip = ((((w + 7) / 8) * 8) - w) / 2;
            for (y = 0; y < h; y++) {
                for (x = 0; x < w && buffer_ptr < buffer_end; x++) {
                    if ((x & 1) == 0)
                        byte = *(buffer_ptr++);
                    k = (byte & 0xF0) >> 4;
                    *ptr++ = rgbQuads[k].rgbRed;
                    *ptr++ = rgbQuads[k].rgbGreen;
                    *ptr++ = rgbQuads[k].rgbBlue;
                    byte <<= 4;
                }
                buffer_ptr += skip;
                ptr -= w * 6;
            }
        }
    }
    if (bitcount == 8) {
        if (comp == BI_RLE8) {
            x = 0;
            y = 0;
            for (i = 0, g = 1; i < imgsize && buffer_ptr < buffer_end && g; i++) {
                byte = *(buffer_ptr++);
                if (byte) {
                    l = byte;
                    byte = *(buffer_ptr++);
                    for (j = 0; j < l; j++) {
                        if (x >= w)
                            break;

                        *ptr++ = rgbQuads[byte].rgbRed;
                        *ptr++ = rgbQuads[byte].rgbGreen;
                        *ptr++ = rgbQuads[byte].rgbBlue;
                        x++;
                        if (ptr > data_end)
                            ptr = data_end;
                    }
                }
                else {
                    byte = *(buffer_ptr++);
                    switch (byte) {
                        case 0:
                            x = 0;
                            y++;
                            ptr = data + ((h - y - 1) * w * 3) + (x * 3);
                            if (ptr > data_end)
                                ptr = data_end;
                            break;
                        case 1:
                            g = 0;
                            break;
                        case 2:
                            x += *(buffer_ptr++);
                            y += *(buffer_ptr++);
                            ptr = data + ((h - y - 1) * w * 3) + (x * 3);
                            if (ptr > data_end)
                                ptr = data_end;
                            break;
                        default:
                            l = byte;
                            for (j = 0; j < l; j++) {
                                byte = *(buffer_ptr++);

                                if (x >= w) {
                                    buffer_ptr += l - j;
                                    break;
                                }

                                *ptr++ = rgbQuads[byte].rgbRed;
                                *ptr++ = rgbQuads[byte].rgbGreen;
                                *ptr++ = rgbQuads[byte].rgbBlue;
                                x++;

                                if (ptr > data_end)
                                    ptr = data_end;
                            }
                            if (l & 1)
                                buffer_ptr++;
                            break;
                    }
                }
            }
        }
        else if (comp == BI_RGB) {
            skip = (((w + 3) / 4) * 4) - w;
            for (y = 0; y < h; y++) {
                for (x = 0; x < w && buffer_ptr < buffer_end; x++) {
                    byte = *(buffer_ptr++);
                    *ptr++ = rgbQuads[byte].rgbRed;
                    *ptr++ = rgbQuads[byte].rgbGreen;
                    *ptr++ = rgbQuads[byte].rgbBlue;
                }
                ptr -= w * 6;
                buffer_ptr += skip;
            }
        }

    }
    else if (bitcount == 16) {
        skip = (((w * 16 + 31) / 32) * 4) - (w * 2);
        for (y = 0; y < h; y++) {
            for (x = 0; x < w && buffer_ptr < buffer_end; x++) {
                r = ((gushort)(*buffer_ptr) & rmask) >> rshift;
                g = ((gushort)(*buffer_ptr) & gmask) >> gshift;
                b = ((gushort)(*(buffer_ptr++)) & bmask) >> bshift;
                *ptr++ = r;
                *ptr++ = g;
                *ptr++ = b;
            }
            ptr -= w * 6;
            buffer_ptr += skip;
        }
    }
    else if (bitcount == 24) {
        skip = (4 - ((w * 3) % 4)) & 3;
        for (y = 0; y < h; y++) {
            for (x = 0; x < w && buffer_ptr < buffer_end; x++) {
                b = *(buffer_ptr++);
                g = *(buffer_ptr++);
                r = *(buffer_ptr++);
                *ptr++ = r;
                *ptr++ = g;
                *ptr++ = b;
            }
            ptr -= w * 6;
            buffer_ptr += skip;
        }
    }
    else if (bitcount == 32) {
        skip = (((w * 32 + 31) / 32) * 4) - (w * 4);
        for (y = 0; y < h; y++) {
            for (x = 0; x < w && buffer_ptr < buffer_end; x++) {
                r = ((gulong)(*buffer_ptr) & rmask) >> rshift;
                g = ((gulong)(*buffer_ptr) & gmask) >> gshift;
                b = ((gulong)(*buffer_ptr) & bmask) >> bshift;
                *ptr++ = r;
                *ptr++ = g;
                *ptr++ = b;
                r = *(buffer_ptr++);
                r = *(buffer_ptr++);
            }
            ptr -= w * 6;
            buffer_ptr += skip;
        }
    }

    gdk_threads_enter();
    if (m_bHasTransColor) {
        ptr = data;
        m_MaskBitmap = gdk_pixmap_new(NULL, w, h, 1);
        GdkGC *gc = gdk_gc_new(m_MaskBitmap);
        GdkColor color;
        color.pixel = 1;
        gdk_gc_set_foreground(gc, &color);
        gdk_draw_rectangle(m_MaskBitmap, gc, TRUE, 0, 0, w, h);
        color.pixel = 0;
        gdk_gc_set_foreground(gc, &color);
        for (y = 0; y < h; y++)
            for (x = 0; x < w; x++) {
                guchar r = *ptr++;
                guchar g = *ptr++;
                guchar b = *ptr++;
                if (r == m_oTransColor.red && g == m_oTransColor.green &&
                    b == m_oTransColor.blue)
                    gdk_draw_point(m_MaskBitmap, gc, x, y);
            }
        gdk_gc_destroy(gc);
    }

    m_Bitmap = gdk_pixmap_new(NULL, w, h, gdk_visual_get_best_depth());
    gdk_draw_rgb_image(m_Bitmap, m_GC, 0, 0, w, h, GDK_RGB_DITHER_MAX, data, 
                       w * 3);

    gdk_threads_leave();
    g_free(data);
    g_free(buffer);

    return kError_NoErr;
}

bool GTKBitmap::IsPosVisible(Pos &oPos)
{
    return true;
}

Error GTKBitmap::BlitRect(Bitmap *pSrcBitmap, Rect &oSrcRect, 
                          Rect &oDestRect)
{
    GdkWindow *src = (GdkWindow *)(((GTKBitmap *)pSrcBitmap)->m_Bitmap);
    GdkWindow *dest = (GdkWindow *)m_Bitmap;

    gdk_threads_enter();
    gdk_window_copy_area(dest, m_GC, oDestRect.x1, oDestRect.y1, src,
                         oSrcRect.x1, oSrcRect.y1, oDestRect.Width(),
                         oDestRect.Height());
    gdk_threads_leave();

    return kError_NoErr;
}

Error GTKBitmap::MaskBlitRect(Bitmap *pSrcBitmap, Rect &oSrcRect,
                              Rect &oDestRect)
{
    GTKBitmap *src = (GTKBitmap *)pSrcBitmap;
    GdkWindow *dest = (GdkWindow *)m_Bitmap;
    int w = oDestRect.Width();
    int h = oDestRect.Height();

    if (!src->GetMask())
        return BlitRect(pSrcBitmap, oSrcRect, oDestRect);

    gdk_threads_enter();
    gdk_gc_set_clip_mask(m_GC, src->GetMask());
    gdk_gc_set_clip_origin(m_GC, oDestRect.x1 - oSrcRect.x1, oDestRect.y1 - 
                           oSrcRect.y1);
    gdk_draw_pixmap(dest, m_GC, src->GetBitmap(), oSrcRect.x1, oSrcRect.y1, 
                    oDestRect.x1, oDestRect.y1, w, h);
    gdk_threads_leave();    
    return kError_NoErr;
}

############################################################################
##      
##      FreeAmp - The Free MP3 Player
##
##      Portions Copyright (C) 1998 GoodNoise
##
##      This program is free software; you can redistribute it and/or modify
##      it under the terms of the GNU General Public License as published by
##      the Free Software Foundation; either version 2 of the License, or
##      (at your option) any later version.
##
##      This program is distributed in the hope that it will be useful,
##      but WITHOUT ANY WARRANTY; without even the implied warranty of
##      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##      GNU General Public License for more details.
##
##      You should have received a copy of the GNU General Public License
##      along with this program; if not, write to the Free Software
##      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##      
##      $Id$
##
############################################################################

include @top_srcdir@/config/Makefile.header

if AM_HAVE_WIN32
FILES = 
MYLIBS = 
TRGT_SYS_HEADS = @top_srcdir@/base/win32/include
else
FILES = lmcregistry.cpp
MYLIBS = libfalmcsrc.a
TRGT_SYS_HEADS = @top_srcdir@/base/unix/@host_os@/include
endif

if AM_HAVE_LINUXTHREADS
REENT = -D_REENTRANT
endif


noinst_LIBRARIES = $(MYLIBS)
libfalmcsrc_a_SOURCES = $(FILES)

INCLUDES = -I@top_srcdir@/base/include -I@srcdir@/../include -I@top_srcdir@/config -I$(TRGT_SYS_HEADS) -I@top_srcdir@/io/include -I@top_srcdir@/ui/include -I@top_srcdir@/lmc/include
CXXFLAGS = $(EXTRA_CXXFLAGS) $(REENT)
CFLAGS = $(EXTRA_CFLAGS) $(REENT)





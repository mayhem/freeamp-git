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

if AM_ENABLE_X11
FILES = freeamp.cpp windowhash.cpp fawindow.cpp res.cpp
MYLIBS = libfauicmdline.a
TRGT_SYS_HEADS = @top_srcdir@/base/unix/@target_os@/include

all-local: cmdline-local
else

FILES = 
MYLIBS = 
TRGT_SYS_HEADS =
all-local:
	@true
endif

cmdline-local: freeamp.o
	gcc -shared -Wl,-soname,freeamp-@target_os@.ui -o freeamp-@target_os@.ui freeamp.o windowhash.o fawindow.o res.o -L/usr/X11R6/lib -lX11 -lXext -lXpm
	cp freeamp-@target_os@.ui @top_srcdir@/plugins

if AM_HAVE_LINUXTHREADS
REENT = -D_REENTRANT
endif

noinst_LIBRARIES = $(MYLIBS)
libfauicmdline_a_SOURCES = $(FILES)

INCLUDES = -I@top_srcdir@/base/include -I@srcdir@/../include -I@top_srcdir@/config -I$(TRGT_SYS_HEADS) -I@top_srcdir@/lmc/include -I@top_srcdir@/io/include -I@top_srcdir@/ui/include -I@srcdir@/../res
CXXFLAGS = $(EXTRA_CXXFLAGS) $(REENT)



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
else
FILES	=	preferences.cpp		\
			bootstrap.cpp		\
			win32impl.cpp		\
			beosthread.cpp		\
			semaphore.cpp		\
			mutex.cpp			\
			debug.cpp			\
			utility.cpp			\
			mem.c				\
			volume.cpp

MYLIBS = libfabeos.a

#APPSTUBNAME = libfabeosapp.so
#all-local:	$(APPSTUBNAME)
#
#$(APPSTUBNAME):	$(libfabeos_a_OBJECTS) @top_srcdir@/base/src/libfabase.a
#	@echo 'Generating temporary _APP_ lib for link-time symbol resolution.'
#	g++ -nostart -Xlinker -soname=_APP_ -o $@ $(libfabeos_a_OBJECTS) -lbe
endif

noinst_LIBRARIES = $(MYLIBS)
libfabeos_a_SOURCES = $(FILES)

INCLUDES = -I@top_srcdir@/base/include -I@srcdir@/../include -I@top_srcdir@/config -I@top_srcdir@/io/include -I@top_srcdir@/ui/include -I@top_srcdir@/base/beos/include -I@top_srcdir@/ui/dummy/include -I@top_srcdir@/lmc/include
CXXFLAGS = $(EXTRA_CXXFLAGS) -DUNIX_LIBDIR="\"@libdir@\""


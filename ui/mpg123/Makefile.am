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

PLUGIN_DIR = @top_srcdir@/plugins/
PLUGIN_NAME = mpg123-@host_os@.ui
PLUGIN_FULL_NAME = $(PLUGIN_DIR)$(PLUGIN_NAME)

if AM_HAVE_WIN32
FILES = 
MYLIBS = 
TRGT_SYS_HEADS = @top_srcdir@/base/win32/include
else
FILES = Mpg123UI.cpp
MYLIBS = libfaui.a
TRGT_SYS_HEADS = @top_srcdir@/base/unix/@host_os@/include

all-local : $(PLUGIN_FULL_NAME)

$(PLUGIN_FULL_NAME) : $(libfaui_a_OBJECTS)
	@mkdir -p @top_srcdir@/plugins
	gcc -shared -Wl,-soname,$(PLUGIN_NAME) -o $(PLUGIN_FULL_NAME) $(libfaui_a_OBJECTS)
	@chmod 644 $(PLUGIN_FULL_NAME)


endif

if AM_HAVE_LINUXTHREADS
REENT = -D_REENTRANT
endif


noinst_LIBRARIES = $(MYLIBS)
libfaui_a_SOURCES = $(FILES)

INCLUDES = -I@top_srcdir@/base/include -I@srcdir@/../include -I@top_srcdir@/config -I$(TRGT_SYS_HEADS) -I@top_srcdir@/lmc/include -I@top_srcdir@/io/include
CXXFLAGS = $(EXTRA_CXXFLAGS) $(REENT)


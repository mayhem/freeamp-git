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
PLUGIN_NAME = soundcard-beos.pmo
PLUGIN_FULL_NAME = $(PLUGIN_DIR)$(PLUGIN_NAME)

if AM_HAVE_BEOS
bin_PROGRAMS = TestRing
TestRing_SOURCES = TestRing.cpp $(FILES)
TestRing_LDADD = $(MYLIBS) $(BEOSLIBS) @top_srcdir@/base/src/libfabase.a @top_srcdir@/base/beos/src/libfabeos.a
FILES = soundcardpmo.cpp ringbuffer.cpp soundutils.cpp
MYLIBS = libfaio.a
BEOSLIBS = @top_srcdir@/io/src/libfaiosrc.a \
           @top_srcdir@/base/src/libfabase.a \
           @top_srcdir@/base/beos/src/libfabeos.a \
           -lmedia -lbe

all-local : $(PLUGIN_FULL_NAME)

$(PLUGIN_FULL_NAME) : $(libfaio_a_OBJECTS)
	@mkdir -p @top_srcdir@/plugins
	g++ -nostart -Xlinker -soname=$(PLUGIN_NAME) -o $(PLUGIN_FULL_NAME) $(libfaio_a_OBJECTS) $(BEOSLIBS)
	@chmod 644 $(PLUGIN_FULL_NAME)

else
FILES = 
MYLIBS = 
endif

noinst_LIBRARIES = $(MYLIBS)
libfaio_a_SOURCES = $(FILES)

INCLUDES = -I@top_srcdir@/base/include -I@top_srcdir@/base/beos/include  -I@srcdir@/../include -I@top_srcdir@/config -I@top_srcdir@/io/include -I@top_srcdir@/ui/include
CXXFLAGS = $(EXTRA_CXXFLAGS)
## CXXFLAGS += -DDEBUG=1

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

SUBDIRS = base io ui lmc

if AM_HAVE_WIN32
PRG =
else
PRG = freeamp
ALLLDADD = @top_srcdir@/base/unix/@target_os@/src/*.o \
          @top_srcdir@/base/unix/src/*.o \
	  @top_srcdir@/lmc/src/*.o \
	  @top_srcdir@/io/src/*.o \
	  @top_srcdir@/ui/src/*.o \
	  -lpthread -lm -ldl
FALDADD = @top_srcdir@/base/src/*.o $(ALLLDADD)
DSLDADD = @top_srcdir@/base/src/player.o @top_srcdir@/base/src/playlist.o @top_srcdir@/base/src/command.o @top_srcdir@/base/src/thread.o $(ALLLDADD)
MYDEPS = @top_srcdir@/base/unix/@target_os@/src/libfasys.a \
         @top_srcdir@/base/src/libfabase.a \
	 @top_srcdir@/base/unix/src/libfaunix.a \
	 @top_srcdir@/lmc/src/libfalmcsrc.a \
	 @top_srcdir@/io/src/libfaiosrc.a \
	 @top_srcdir@/ui/src/libfauisrc.a
TRGT_SYS_HEADS = @top_srcdir@/base/unix/@target_os@/include
endif


bin_PROGRAMS = $(PRG)
freeamp_SOURCES = freeamp.cpp
freeamp_LDADD = $(FALDADD)
freeamp_DEPENDENCIES = $(MYDEPS)

## decspeed_SOURCES = decoder_speed.cpp
## decspeed_LDADD = $(DSLDADD)
## decspeed_DEPENDENCIES = $(MYDEPS)

INCLUDES = -I@top_srcdir@/base/include -I@top_srcdir@/config -I$(TRGT_SYS_HEADS) -I@top_srcdir@/io/include -I@top_srcdir@/ui/include -I@top_srcdir@/lmc/include
CXXFLAGS = $(EXTRA_CXXFLAGS) -D__@target_os@__ -Wl,-export-dynamic






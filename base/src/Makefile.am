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
TRGT_SYS_HEADS = @top_srcdir@/base/win32/include
PRG =
FILES = 
else
TRGT_SYS_HEADS = @top_srcdir@/base/unix/@target_os@/include
PRG = freeamp
FILES = player.cpp bootstrap.cpp command.cpp buffer.cpp playlist.cpp
DEPS = @top_srcdir@/base/unix/@target_os@/src/libfasys.a
MYLDADD = @top_srcdir@/base/unix/@target_os@/src/libfasys.a -lpthread
endif

## bin_PROGRAMS = $(PRG)
## freeamp_SOURCES = $(FILES)
## freeamp_LDADD = $(MYLDADD)
## freeamp_DEPENDENCIES = $(DEPS)

noinst_LIBRARIES = libfacore.a
libfacore_a_SOURCES = $(FILES)

INCLUDES = -I@top_srcdir@/base/include -I@top_srcdir@/config -I$(TRGT_SYS_HEADS) -I@top_srcdir@/io/include -I@top_srcdir@/ui/include -I@top_srcdir@/lmc/include
CXXFLAGS = -D__@target_os@__ $(EXTRA_CXXFLAGS)


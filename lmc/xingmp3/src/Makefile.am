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
PLUGIN_NAME = xingmp3-@host_os@.lmc
PLUGIN_FULL_NAME = $(PLUGIN_DIR)$(PLUGIN_NAME)

GAS2INTEL = @top_srcdir@/util/gas2intel

if AM_HAVE_WIN32
C_FILES =
ASM_FILES =
ASM_DEFS =
MYLIBS =
TRGT_SYS_HEADS = @top_srcdir@/base/win32/include
else
if AM_HAVE_X86
C_FILES = cdct.c cupl3.c hwin.c iup.c l3init.c msis.c wavep.c csbt.c \
          cwinm.c icdct.c mdct.c uph.c cup.c dec8.c isbt.c l3dq.c \
          mhead.c upsf.c iwinm.c xinglmc.cpp
ASM_FILES = x86gas.s
ASM_DEFS = -DASM_FDCT32 -DASM_WINDOW_DUAL
else
# A bug in automake forces us to redundantly declare C_FILES
C_FILES = cdct.c cupl3.c hwin.c iup.c l3init.c msis.c wavep.c csbt.c \
          cwinm.c icdct.c mdct.c uph.c cup.c dec8.c isbt.c l3dq.c \
          mhead.c upsf.c iwinm.c xinglmc.cpp
ASM_FILES =
ASM_DEFS =
endif
MYLIBS = libfalmc.a
TRGT_SYS_HEADS = @top_srcdir@/base/unix/@host_os@/include

all-local : $(PLUGIN_FULL_NAME)

$(PLUGIN_FULL_NAME) : $(libfalmc_a_OBJECTS)
	@mkdir -p @top_srcdir@/plugins
	gcc -shared -Wl,-soname,$(PLUGIN_NAME) -o $(PLUGIN_FULL_NAME) $(libfalmc_a_OBJECTS)
	@chmod 644 $(PLUGIN_FULL_NAME)

x86intel.asm: x86gas.s $(GAS2INTEL)
	$(GAS2INTEL) $< > $@

x86intel.c: x86gas.s $(GAS2INTEL)
	$(GAS2INTEL) -i $< > $@

endif

if AM_HAVE_LINUXTHREADS
REENT = -D_REENTRANT
endif

if AM_MP3_PROF
MP3_PROF = -pg
endif

noinst_LIBRARIES = $(MYLIBS)
libfalmc_a_SOURCES = $(C_FILES) $(ASM_FILES)

INCLUDES = -I@top_srcdir@/base/include -I@srcdir@/../include \
           -I@top_srcdir@/config -I$(TRGT_SYS_HEADS) \
           -I@top_srcdir@/io/include -I@top_srcdir@/ui/include \
           -I@top_srcdir@/lmc/include
CXXFLAGS = $(EXTRA_CXXFLAGS) $(REENT)
CFLAGS = $(MP3_PROF) $(EXTRA_CFLAGS) $(REENT) $(ASM_DEFS)

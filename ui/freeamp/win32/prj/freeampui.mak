# Microsoft Developer Studio Generated NMAKE File, Based on freeampui.dsp
!IF "$(CFG)" == ""
CFG=freeampui - Win32 NASM Debug
!MESSAGE No configuration specified. Defaulting to freeampui - Win32 NASM Debug.
!ENDIF 

!IF "$(CFG)" != "freeampui - Win32 Release" && "$(CFG)" != "freeampui - Win32 Debug" && "$(CFG)" != "freeampui - Win32 NASM Debug" && "$(CFG)" != "freeampui - Win32 NASM Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "freeampui.mak" CFG="freeampui - Win32 NASM Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "freeampui - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "freeampui - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "freeampui - Win32 NASM Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "freeampui - Win32 NASM Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "freeampui - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\freeamp.ui"


CLEAN :
	-@erase "$(INTDIR)\Bitmap.obj"
	-@erase "$(INTDIR)\ButtonControl.obj"
	-@erase "$(INTDIR)\Canvas.obj"
	-@erase "$(INTDIR)\Control.obj"
	-@erase "$(INTDIR)\DialControl.obj"
	-@erase "$(INTDIR)\Font.obj"
	-@erase "$(INTDIR)\FreeAmpTheme.obj"
	-@erase "$(INTDIR)\freeampui.res"
	-@erase "$(INTDIR)\Headlines.obj"
	-@erase "$(INTDIR)\Median.obj"
	-@erase "$(INTDIR)\MultiStateControl.obj"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\SliderControl.obj"
	-@erase "$(INTDIR)\TextControl.obj"
	-@erase "$(INTDIR)\Theme.obj"
	-@erase "$(INTDIR)\ThemeManager.obj"
	-@erase "$(INTDIR)\ThemeZip.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\VSliderControl.obj"
	-@erase "$(INTDIR)\Win32Bitmap.obj"
	-@erase "$(INTDIR)\Win32Canvas.obj"
	-@erase "$(INTDIR)\Win32Font.obj"
	-@erase "$(INTDIR)\Win32MessageDialog.obj"
	-@erase "$(INTDIR)\Win32PreferenceWindow.obj"
	-@erase "$(INTDIR)\Win32Window.obj"
	-@erase "$(INTDIR)\Window.obj"
	-@erase "$(OUTDIR)\freeamp.exp"
	-@erase "$(OUTDIR)\freeamp.lib"
	-@erase ".\freeamp.ui"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\res" /I "..\include" /I "..\..\include" /I "..\..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /I "..\..\..\..\lib\gdbm" /I "..\..\..\..\lib\xml\include" /I "..\..\..\..\lib\zlib\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\freeampui.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\freeampui.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\freeampui.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib zlib.lib gdbm.lib version.lib wsock32.lib winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x12000000" /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\freeamp.pdb" /machine:I386 /def:".\freeampui.def" /out:"freeamp.ui" /implib:"$(OUTDIR)\freeamp.lib" /libpath:"..\..\..\..\lib\zlib" /libpath:"..\..\..\..\lib\gdbm" /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\freeampui.def"
LINK32_OBJS= \
	"$(INTDIR)\Bitmap.obj" \
	"$(INTDIR)\ButtonControl.obj" \
	"$(INTDIR)\Canvas.obj" \
	"$(INTDIR)\Control.obj" \
	"$(INTDIR)\DialControl.obj" \
	"$(INTDIR)\Font.obj" \
	"$(INTDIR)\FreeAmpTheme.obj" \
	"$(INTDIR)\Headlines.obj" \
	"$(INTDIR)\Median.obj" \
	"$(INTDIR)\MultiStateControl.obj" \
	"$(INTDIR)\Parse.obj" \
	"$(INTDIR)\SliderControl.obj" \
	"$(INTDIR)\TextControl.obj" \
	"$(INTDIR)\Theme.obj" \
	"$(INTDIR)\ThemeManager.obj" \
	"$(INTDIR)\ThemeZip.obj" \
	"$(INTDIR)\VSliderControl.obj" \
	"$(INTDIR)\Win32Bitmap.obj" \
	"$(INTDIR)\Win32Canvas.obj" \
	"$(INTDIR)\Win32Font.obj" \
	"$(INTDIR)\Win32MessageDialog.obj" \
	"$(INTDIR)\Win32PreferenceWindow.obj" \
	"$(INTDIR)\Win32Window.obj" \
	"$(INTDIR)\Window.obj" \
	"$(INTDIR)\freeampui.res"

".\freeamp.ui" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : ".\freeamp.ui"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                                                          ..\..\..\..\base\win32\prj\plugins
	copy freeamp.ui                                         ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\..\config\config.h" ".\freeamp.ui"


CLEAN :
	-@erase "$(INTDIR)\Bitmap.obj"
	-@erase "$(INTDIR)\ButtonControl.obj"
	-@erase "$(INTDIR)\Canvas.obj"
	-@erase "$(INTDIR)\Control.obj"
	-@erase "$(INTDIR)\DialControl.obj"
	-@erase "$(INTDIR)\Font.obj"
	-@erase "$(INTDIR)\FreeAmpTheme.obj"
	-@erase "$(INTDIR)\freeampui.res"
	-@erase "$(INTDIR)\Headlines.obj"
	-@erase "$(INTDIR)\Median.obj"
	-@erase "$(INTDIR)\MultiStateControl.obj"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\SliderControl.obj"
	-@erase "$(INTDIR)\TextControl.obj"
	-@erase "$(INTDIR)\Theme.obj"
	-@erase "$(INTDIR)\ThemeManager.obj"
	-@erase "$(INTDIR)\ThemeZip.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VSliderControl.obj"
	-@erase "$(INTDIR)\Win32Bitmap.obj"
	-@erase "$(INTDIR)\Win32Canvas.obj"
	-@erase "$(INTDIR)\Win32Font.obj"
	-@erase "$(INTDIR)\Win32MessageDialog.obj"
	-@erase "$(INTDIR)\Win32PreferenceWindow.obj"
	-@erase "$(INTDIR)\Win32Window.obj"
	-@erase "$(INTDIR)\Window.obj"
	-@erase "$(OUTDIR)\freeamp.exp"
	-@erase "$(OUTDIR)\freeamp.lib"
	-@erase "$(OUTDIR)\freeamp.pdb"
	-@erase ".\freeamp.ilk"
	-@erase ".\freeamp.ui"
	-@erase "..\..\..\..\config\config.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\res" /I "..\include" /I "..\..\include" /I "..\..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /I "..\..\..\..\lib\gdbm" /I "..\..\..\..\lib\xml\include" /I "..\..\..\..\lib\zlib\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\freeampui.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\freeampui.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\freeampui.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib zlib.lib gdbm.lib version.lib wsock32.lib winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x12000000" /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\freeamp.pdb" /debug /machine:I386 /def:".\freeampui.def" /out:"freeamp.ui" /implib:"$(OUTDIR)\freeamp.lib" /pdbtype:sept /libpath:"..\..\..\..\lib\zlib" /libpath:"..\..\..\..\lib\gdbm" /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\freeampui.def"
LINK32_OBJS= \
	"$(INTDIR)\Bitmap.obj" \
	"$(INTDIR)\ButtonControl.obj" \
	"$(INTDIR)\Canvas.obj" \
	"$(INTDIR)\Control.obj" \
	"$(INTDIR)\DialControl.obj" \
	"$(INTDIR)\Font.obj" \
	"$(INTDIR)\FreeAmpTheme.obj" \
	"$(INTDIR)\Headlines.obj" \
	"$(INTDIR)\Median.obj" \
	"$(INTDIR)\MultiStateControl.obj" \
	"$(INTDIR)\Parse.obj" \
	"$(INTDIR)\SliderControl.obj" \
	"$(INTDIR)\TextControl.obj" \
	"$(INTDIR)\Theme.obj" \
	"$(INTDIR)\ThemeManager.obj" \
	"$(INTDIR)\ThemeZip.obj" \
	"$(INTDIR)\VSliderControl.obj" \
	"$(INTDIR)\Win32Bitmap.obj" \
	"$(INTDIR)\Win32Canvas.obj" \
	"$(INTDIR)\Win32Font.obj" \
	"$(INTDIR)\Win32MessageDialog.obj" \
	"$(INTDIR)\Win32PreferenceWindow.obj" \
	"$(INTDIR)\Win32Window.obj" \
	"$(INTDIR)\Window.obj" \
	"$(INTDIR)\freeampui.res"

".\freeamp.ui" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "..\..\..\..\config\config.h" ".\freeamp.ui"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                                                          ..\..\..\..\base\win32\prj\plugins
	copy freeamp.ui                                         ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "freeampui - Win32 NASM Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\..\config\config.h" ".\freeamp.ui"


CLEAN :
	-@erase "$(INTDIR)\Bitmap.obj"
	-@erase "$(INTDIR)\ButtonControl.obj"
	-@erase "$(INTDIR)\Canvas.obj"
	-@erase "$(INTDIR)\Control.obj"
	-@erase "$(INTDIR)\DialControl.obj"
	-@erase "$(INTDIR)\Font.obj"
	-@erase "$(INTDIR)\FreeAmpTheme.obj"
	-@erase "$(INTDIR)\freeampui.res"
	-@erase "$(INTDIR)\Headlines.obj"
	-@erase "$(INTDIR)\Median.obj"
	-@erase "$(INTDIR)\MultiStateControl.obj"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\SliderControl.obj"
	-@erase "$(INTDIR)\TextControl.obj"
	-@erase "$(INTDIR)\Theme.obj"
	-@erase "$(INTDIR)\ThemeManager.obj"
	-@erase "$(INTDIR)\ThemeZip.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VSliderControl.obj"
	-@erase "$(INTDIR)\Win32Bitmap.obj"
	-@erase "$(INTDIR)\Win32Canvas.obj"
	-@erase "$(INTDIR)\Win32Font.obj"
	-@erase "$(INTDIR)\Win32MessageDialog.obj"
	-@erase "$(INTDIR)\Win32PreferenceWindow.obj"
	-@erase "$(INTDIR)\Win32Window.obj"
	-@erase "$(INTDIR)\Window.obj"
	-@erase "$(OUTDIR)\freeamp.exp"
	-@erase "$(OUTDIR)\freeamp.lib"
	-@erase "$(OUTDIR)\freeamp.pdb"
	-@erase ".\freeamp.ilk"
	-@erase ".\freeamp.ui"
	-@erase "..\..\..\..\config\config.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\res" /I "..\include" /I "..\..\include" /I "..\..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /I "..\..\..\..\lib\gdbm" /I "..\..\..\..\lib\xml\include" /I "..\..\..\..\lib\zlib\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\freeampui.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\freeampui.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\freeampui.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib zlib.lib gdbm.lib version.lib wsock32.lib winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x12000000" /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\freeamp.pdb" /debug /machine:I386 /def:".\freeampui.def" /out:"freeamp.ui" /implib:"$(OUTDIR)\freeamp.lib" /pdbtype:sept /libpath:"..\..\..\..\lib\zlib" /libpath:"..\..\..\..\lib\gdbm" /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\freeampui.def"
LINK32_OBJS= \
	"$(INTDIR)\Bitmap.obj" \
	"$(INTDIR)\ButtonControl.obj" \
	"$(INTDIR)\Canvas.obj" \
	"$(INTDIR)\Control.obj" \
	"$(INTDIR)\DialControl.obj" \
	"$(INTDIR)\Font.obj" \
	"$(INTDIR)\FreeAmpTheme.obj" \
	"$(INTDIR)\Headlines.obj" \
	"$(INTDIR)\Median.obj" \
	"$(INTDIR)\MultiStateControl.obj" \
	"$(INTDIR)\Parse.obj" \
	"$(INTDIR)\SliderControl.obj" \
	"$(INTDIR)\TextControl.obj" \
	"$(INTDIR)\Theme.obj" \
	"$(INTDIR)\ThemeManager.obj" \
	"$(INTDIR)\ThemeZip.obj" \
	"$(INTDIR)\VSliderControl.obj" \
	"$(INTDIR)\Win32Bitmap.obj" \
	"$(INTDIR)\Win32Canvas.obj" \
	"$(INTDIR)\Win32Font.obj" \
	"$(INTDIR)\Win32MessageDialog.obj" \
	"$(INTDIR)\Win32PreferenceWindow.obj" \
	"$(INTDIR)\Win32Window.obj" \
	"$(INTDIR)\Window.obj" \
	"$(INTDIR)\freeampui.res"

".\freeamp.ui" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "..\..\..\..\config\config.h" ".\freeamp.ui"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                                                          ..\..\..\..\base\win32\prj\plugins
	copy freeamp.ui                                         ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "freeampui - Win32 NASM Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\..\config\config.h" ".\freeamp.ui"


CLEAN :
	-@erase "$(INTDIR)\Bitmap.obj"
	-@erase "$(INTDIR)\ButtonControl.obj"
	-@erase "$(INTDIR)\Canvas.obj"
	-@erase "$(INTDIR)\Control.obj"
	-@erase "$(INTDIR)\DialControl.obj"
	-@erase "$(INTDIR)\Font.obj"
	-@erase "$(INTDIR)\FreeAmpTheme.obj"
	-@erase "$(INTDIR)\freeampui.res"
	-@erase "$(INTDIR)\Headlines.obj"
	-@erase "$(INTDIR)\Median.obj"
	-@erase "$(INTDIR)\MultiStateControl.obj"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\SliderControl.obj"
	-@erase "$(INTDIR)\TextControl.obj"
	-@erase "$(INTDIR)\Theme.obj"
	-@erase "$(INTDIR)\ThemeManager.obj"
	-@erase "$(INTDIR)\ThemeZip.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\VSliderControl.obj"
	-@erase "$(INTDIR)\Win32Bitmap.obj"
	-@erase "$(INTDIR)\Win32Canvas.obj"
	-@erase "$(INTDIR)\Win32Font.obj"
	-@erase "$(INTDIR)\Win32MessageDialog.obj"
	-@erase "$(INTDIR)\Win32PreferenceWindow.obj"
	-@erase "$(INTDIR)\Win32Window.obj"
	-@erase "$(INTDIR)\Window.obj"
	-@erase "$(OUTDIR)\freeamp.exp"
	-@erase "$(OUTDIR)\freeamp.lib"
	-@erase ".\freeamp.ui"
	-@erase "..\..\..\..\config\config.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\res" /I "..\include" /I "..\..\include" /I "..\..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /I "..\..\..\..\lib\gdbm" /I "..\..\..\..\lib\xml\include" /I "..\..\..\..\lib\zlib\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\freeampui.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\freeampui.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\freeampui.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib zlib.lib gdbm.lib version.lib wsock32.lib winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x12000000" /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\freeamp.pdb" /machine:I386 /def:".\freeampui.def" /out:"freeamp.ui" /implib:"$(OUTDIR)\freeamp.lib" /libpath:"..\..\..\..\lib\zlib" /libpath:"..\..\..\..\lib\gdbm" /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\freeampui.def"
LINK32_OBJS= \
	"$(INTDIR)\Bitmap.obj" \
	"$(INTDIR)\ButtonControl.obj" \
	"$(INTDIR)\Canvas.obj" \
	"$(INTDIR)\Control.obj" \
	"$(INTDIR)\DialControl.obj" \
	"$(INTDIR)\Font.obj" \
	"$(INTDIR)\FreeAmpTheme.obj" \
	"$(INTDIR)\Headlines.obj" \
	"$(INTDIR)\Median.obj" \
	"$(INTDIR)\MultiStateControl.obj" \
	"$(INTDIR)\Parse.obj" \
	"$(INTDIR)\SliderControl.obj" \
	"$(INTDIR)\TextControl.obj" \
	"$(INTDIR)\Theme.obj" \
	"$(INTDIR)\ThemeManager.obj" \
	"$(INTDIR)\ThemeZip.obj" \
	"$(INTDIR)\VSliderControl.obj" \
	"$(INTDIR)\Win32Bitmap.obj" \
	"$(INTDIR)\Win32Canvas.obj" \
	"$(INTDIR)\Win32Font.obj" \
	"$(INTDIR)\Win32MessageDialog.obj" \
	"$(INTDIR)\Win32PreferenceWindow.obj" \
	"$(INTDIR)\Win32Window.obj" \
	"$(INTDIR)\Window.obj" \
	"$(INTDIR)\freeampui.res"

".\freeamp.ui" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "..\..\..\..\config\config.h" ".\freeamp.ui"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                                                          ..\..\..\..\base\win32\prj\plugins
	copy freeamp.ui                                         ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("freeampui.dep")
!INCLUDE "freeampui.dep"
!ELSE 
!MESSAGE Warning: cannot find "freeampui.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "freeampui - Win32 Release" || "$(CFG)" == "freeampui - Win32 Debug" || "$(CFG)" == "freeampui - Win32 NASM Debug" || "$(CFG)" == "freeampui - Win32 NASM Release"
SOURCE=..\..\src\Bitmap.cpp

"$(INTDIR)\Bitmap.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\ButtonControl.cpp

"$(INTDIR)\ButtonControl.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\Canvas.cpp

"$(INTDIR)\Canvas.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\config\config.win32

!IF  "$(CFG)" == "freeampui - Win32 Release"

InputPath=..\..\..\..\config\config.win32

"..\..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\..\config\config.win32 ..\..\..\..\config\config.h
<< 
	

!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

InputPath=..\..\..\..\config\config.win32

"..\..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\..\config\config.win32 ..\..\..\..\config\config.h
<< 
	

!ELSEIF  "$(CFG)" == "freeampui - Win32 NASM Debug"

InputPath=..\..\..\..\config\config.win32

"..\..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\..\config\config.win32 ..\..\..\..\config\config.h
<< 
	

!ELSEIF  "$(CFG)" == "freeampui - Win32 NASM Release"

InputPath=..\..\..\..\config\config.win32

"..\..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\..\config\config.win32 ..\..\..\..\config\config.h
<< 
	

!ENDIF 

SOURCE=..\..\src\Control.cpp

"$(INTDIR)\Control.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\DialControl.cpp

"$(INTDIR)\DialControl.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\Font.cpp

"$(INTDIR)\Font.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\FreeAmpTheme.cpp

"$(INTDIR)\FreeAmpTheme.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\Headlines.cpp

"$(INTDIR)\Headlines.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Median.cpp

"$(INTDIR)\Median.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\MultiStateControl.cpp

"$(INTDIR)\MultiStateControl.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\xml\src\Parse.cpp

"$(INTDIR)\Parse.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\SliderControl.cpp

"$(INTDIR)\SliderControl.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\TextControl.cpp

"$(INTDIR)\TextControl.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\Theme.cpp

"$(INTDIR)\Theme.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ThemeManager.cpp

"$(INTDIR)\ThemeManager.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\ThemeZip.cpp

"$(INTDIR)\ThemeZip.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\VSliderControl.cpp

"$(INTDIR)\VSliderControl.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Win32Bitmap.cpp

"$(INTDIR)\Win32Bitmap.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Win32Canvas.cpp

"$(INTDIR)\Win32Canvas.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Win32Font.cpp

"$(INTDIR)\Win32Font.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Win32MessageDialog.cpp

"$(INTDIR)\Win32MessageDialog.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Win32PreferenceWindow.cpp

"$(INTDIR)\Win32PreferenceWindow.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Win32Window.cpp

"$(INTDIR)\Win32Window.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\Window.cpp

"$(INTDIR)\Window.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\res\freeampui.rc

!IF  "$(CFG)" == "freeampui - Win32 Release"


"$(INTDIR)\freeampui.res" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(RSC) /l 0x409 /fo"$(INTDIR)\freeampui.res" /i "\Local\src\freeamp\ui\freeamp\win32\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"


"$(INTDIR)\freeampui.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\freeampui.res" /i "\Local\src\freeamp\ui\freeamp\win32\res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 NASM Debug"


"$(INTDIR)\freeampui.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\freeampui.res" /i "\Local\src\freeamp\ui\freeamp\win32\res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 NASM Release"


"$(INTDIR)\freeampui.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\freeampui.res" /i "\Local\src\freeamp\ui\freeamp\win32\res" /d "NDEBUG" $(SOURCE)


!ENDIF 


!ENDIF 


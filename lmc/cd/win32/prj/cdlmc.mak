# Microsoft Developer Studio Generated NMAKE File, Based on cdlmc.dsp
!IF "$(CFG)" == ""
CFG=cdlmc - Win32 NASM Debug
!MESSAGE No configuration specified. Defaulting to cdlmc - Win32 NASM Debug.
!ENDIF 

!IF "$(CFG)" != "cdlmc - Win32 Release" && "$(CFG)" != "cdlmc - Win32 Debug" && "$(CFG)" != "cdlmc - Win32 NASM Release" && "$(CFG)" != "cdlmc - Win32 NASM Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cdlmc.mak" CFG="cdlmc - Win32 NASM Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cdlmc - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cdlmc - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cdlmc - Win32 NASM Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cdlmc - Win32 NASM Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "cdlmc - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\cd.lmc"


CLEAN :
	-@erase "$(INTDIR)\cdct.obj"
	-@erase "$(INTDIR)\cdlmc.obj"
	-@erase "$(INTDIR)\cdlmc.res"
	-@erase "$(INTDIR)\csbt.obj"
	-@erase "$(INTDIR)\cup.obj"
	-@erase "$(INTDIR)\cupl3.obj"
	-@erase "$(INTDIR)\cwinm.obj"
	-@erase "$(INTDIR)\dec8.obj"
	-@erase "$(INTDIR)\eventbuffer.obj"
	-@erase "$(INTDIR)\hwin.obj"
	-@erase "$(INTDIR)\icdct.obj"
	-@erase "$(INTDIR)\isbt.obj"
	-@erase "$(INTDIR)\iup.obj"
	-@erase "$(INTDIR)\iwinm.obj"
	-@erase "$(INTDIR)\l3dq.obj"
	-@erase "$(INTDIR)\l3init.obj"
	-@erase "$(INTDIR)\mdct.obj"
	-@erase "$(INTDIR)\mhead.obj"
	-@erase "$(INTDIR)\msis.obj"
	-@erase "$(INTDIR)\pipeline.obj"
	-@erase "$(INTDIR)\pullbuffer.obj"
	-@erase "$(INTDIR)\uph.obj"
	-@erase "$(INTDIR)\upsf.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\wavep.obj"
	-@erase "$(OUTDIR)\cd.exp"
	-@erase "$(OUTDIR)\cd.lib"
	-@erase ".\cd.lmc"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\..\\" /I "..\..\..\include" /I "..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I "..\..\..\..\ui\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\cdlmc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\cdlmc.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cdlmc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x11800000" /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\cd.pdb" /machine:I386 /def:".\cdlmc.def" /out:"cd.lmc" /implib:"$(OUTDIR)\cd.lib" /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\cdlmc.def"
LINK32_OBJS= \
	"$(INTDIR)\eventbuffer.obj" \
	"$(INTDIR)\pipeline.obj" \
	"$(INTDIR)\pullbuffer.obj" \
	"$(INTDIR)\cdct.obj" \
	"$(INTDIR)\csbt.obj" \
	"$(INTDIR)\cup.obj" \
	"$(INTDIR)\cupl3.obj" \
	"$(INTDIR)\cwinm.obj" \
	"$(INTDIR)\dec8.obj" \
	"$(INTDIR)\hwin.obj" \
	"$(INTDIR)\icdct.obj" \
	"$(INTDIR)\isbt.obj" \
	"$(INTDIR)\iup.obj" \
	"$(INTDIR)\iwinm.obj" \
	"$(INTDIR)\l3dq.obj" \
	"$(INTDIR)\l3init.obj" \
	"$(INTDIR)\mdct.obj" \
	"$(INTDIR)\mhead.obj" \
	"$(INTDIR)\msis.obj" \
	"$(INTDIR)\uph.obj" \
	"$(INTDIR)\upsf.obj" \
	"$(INTDIR)\wavep.obj" \
	"$(INTDIR)\cdlmc.obj" \
	"$(INTDIR)\cdlmc.res"

".\cd.lmc" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : ".\cd.lmc"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                                    ..\..\..\..\base\win32\prj\plugins
	copy cdlmc.lmc                            ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "cdlmc - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\..\config\config.h" ".\cd.lmc"


CLEAN :
	-@erase "$(INTDIR)\cdct.obj"
	-@erase "$(INTDIR)\cdlmc.obj"
	-@erase "$(INTDIR)\cdlmc.res"
	-@erase "$(INTDIR)\csbt.obj"
	-@erase "$(INTDIR)\cup.obj"
	-@erase "$(INTDIR)\cupl3.obj"
	-@erase "$(INTDIR)\cwinm.obj"
	-@erase "$(INTDIR)\dec8.obj"
	-@erase "$(INTDIR)\eventbuffer.obj"
	-@erase "$(INTDIR)\hwin.obj"
	-@erase "$(INTDIR)\icdct.obj"
	-@erase "$(INTDIR)\isbt.obj"
	-@erase "$(INTDIR)\iup.obj"
	-@erase "$(INTDIR)\iwinm.obj"
	-@erase "$(INTDIR)\l3dq.obj"
	-@erase "$(INTDIR)\l3init.obj"
	-@erase "$(INTDIR)\mdct.obj"
	-@erase "$(INTDIR)\mhead.obj"
	-@erase "$(INTDIR)\msis.obj"
	-@erase "$(INTDIR)\pipeline.obj"
	-@erase "$(INTDIR)\pullbuffer.obj"
	-@erase "$(INTDIR)\uph.obj"
	-@erase "$(INTDIR)\upsf.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\wavep.obj"
	-@erase "$(OUTDIR)\cd.exp"
	-@erase "$(OUTDIR)\cd.lib"
	-@erase "$(OUTDIR)\cd.pdb"
	-@erase ".\cd.ilk"
	-@erase ".\cd.lmc"
	-@erase "..\..\..\..\config\config.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\\" /I "..\..\..\include" /I "..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I "..\..\..\..\ui\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\cdlmc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\cdlmc.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cdlmc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x11800000" /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\cd.pdb" /debug /machine:I386 /def:".\cdlmc.def" /out:"cd.lmc" /implib:"$(OUTDIR)\cd.lib" /pdbtype:sept /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\cdlmc.def"
LINK32_OBJS= \
	"$(INTDIR)\eventbuffer.obj" \
	"$(INTDIR)\pipeline.obj" \
	"$(INTDIR)\pullbuffer.obj" \
	"$(INTDIR)\cdct.obj" \
	"$(INTDIR)\csbt.obj" \
	"$(INTDIR)\cup.obj" \
	"$(INTDIR)\cupl3.obj" \
	"$(INTDIR)\cwinm.obj" \
	"$(INTDIR)\dec8.obj" \
	"$(INTDIR)\hwin.obj" \
	"$(INTDIR)\icdct.obj" \
	"$(INTDIR)\isbt.obj" \
	"$(INTDIR)\iup.obj" \
	"$(INTDIR)\iwinm.obj" \
	"$(INTDIR)\l3dq.obj" \
	"$(INTDIR)\l3init.obj" \
	"$(INTDIR)\mdct.obj" \
	"$(INTDIR)\mhead.obj" \
	"$(INTDIR)\msis.obj" \
	"$(INTDIR)\uph.obj" \
	"$(INTDIR)\upsf.obj" \
	"$(INTDIR)\wavep.obj" \
	"$(INTDIR)\cdlmc.obj" \
	"$(INTDIR)\cdlmc.res"

".\cd.lmc" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "..\..\..\..\config\config.h" ".\cd.lmc"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                                    ..\..\..\..\base\win32\prj\plugins
	copy cdlmc.lmc                            ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "cdlmc - Win32 NASM Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\..\config\config.h" ".\cd.lmc"


CLEAN :
	-@erase "$(INTDIR)\cdct.obj"
	-@erase "$(INTDIR)\cdlmc.obj"
	-@erase "$(INTDIR)\cdlmc.res"
	-@erase "$(INTDIR)\csbt.obj"
	-@erase "$(INTDIR)\cup.obj"
	-@erase "$(INTDIR)\cupl3.obj"
	-@erase "$(INTDIR)\cwinm.obj"
	-@erase "$(INTDIR)\dec8.obj"
	-@erase "$(INTDIR)\eventbuffer.obj"
	-@erase "$(INTDIR)\hwin.obj"
	-@erase "$(INTDIR)\icdct.obj"
	-@erase "$(INTDIR)\isbt.obj"
	-@erase "$(INTDIR)\iup.obj"
	-@erase "$(INTDIR)\iwinm.obj"
	-@erase "$(INTDIR)\l3dq.obj"
	-@erase "$(INTDIR)\l3init.obj"
	-@erase "$(INTDIR)\mdct.obj"
	-@erase "$(INTDIR)\mhead.obj"
	-@erase "$(INTDIR)\msis.obj"
	-@erase "$(INTDIR)\pipeline.obj"
	-@erase "$(INTDIR)\pullbuffer.obj"
	-@erase "$(INTDIR)\uph.obj"
	-@erase "$(INTDIR)\upsf.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\wavep.obj"
	-@erase "$(OUTDIR)\cd.exp"
	-@erase "$(OUTDIR)\cd.lib"
	-@erase ".\cd.lmc"
	-@erase "..\..\..\..\config\config.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\..\\" /I "..\..\..\include" /I "..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I "..\..\..\..\ui\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "ASM_X86" /Fp"$(INTDIR)\cdlmc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\cdlmc.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cdlmc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x11800000" /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\cd.pdb" /machine:I386 /def:".\cdlmc.def" /out:"cd.lmc" /implib:"$(OUTDIR)\cd.lib" /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\cdlmc.def"
LINK32_OBJS= \
	"$(INTDIR)\eventbuffer.obj" \
	"$(INTDIR)\pipeline.obj" \
	"$(INTDIR)\pullbuffer.obj" \
	"$(INTDIR)\cdct.obj" \
	"$(INTDIR)\csbt.obj" \
	"$(INTDIR)\cup.obj" \
	"$(INTDIR)\cupl3.obj" \
	"$(INTDIR)\cwinm.obj" \
	"$(INTDIR)\dec8.obj" \
	"$(INTDIR)\hwin.obj" \
	"$(INTDIR)\icdct.obj" \
	"$(INTDIR)\isbt.obj" \
	"$(INTDIR)\iup.obj" \
	"$(INTDIR)\iwinm.obj" \
	"$(INTDIR)\l3dq.obj" \
	"$(INTDIR)\l3init.obj" \
	"$(INTDIR)\mdct.obj" \
	"$(INTDIR)\mhead.obj" \
	"$(INTDIR)\msis.obj" \
	"$(INTDIR)\uph.obj" \
	"$(INTDIR)\upsf.obj" \
	"$(INTDIR)\wavep.obj" \
	"$(INTDIR)\cdlmc.obj" \
	"$(INTDIR)\cdlmc.res" \
	"$(INTDIR)\cdctasm.obj" \
	"$(INTDIR)\cwin8asm.obj" \
	"$(INTDIR)\cwinasm.obj" \
	"$(INTDIR)\mdctasm.obj" \
	"$(INTDIR)\msisasm.obj"

".\cd.lmc" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "..\..\..\..\config\config.h" ".\cd.lmc"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                                    ..\..\..\..\base\win32\prj\plugins
	copy cdlmc.lmc                            ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "cdlmc - Win32 NASM Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\..\config\config.h" ".\cd.lmc"


CLEAN :
	-@erase "$(INTDIR)\cdct.obj"
	-@erase "$(INTDIR)\cdlmc.obj"
	-@erase "$(INTDIR)\cdlmc.res"
	-@erase "$(INTDIR)\csbt.obj"
	-@erase "$(INTDIR)\cup.obj"
	-@erase "$(INTDIR)\cupl3.obj"
	-@erase "$(INTDIR)\cwinm.obj"
	-@erase "$(INTDIR)\dec8.obj"
	-@erase "$(INTDIR)\eventbuffer.obj"
	-@erase "$(INTDIR)\hwin.obj"
	-@erase "$(INTDIR)\icdct.obj"
	-@erase "$(INTDIR)\isbt.obj"
	-@erase "$(INTDIR)\iup.obj"
	-@erase "$(INTDIR)\iwinm.obj"
	-@erase "$(INTDIR)\l3dq.obj"
	-@erase "$(INTDIR)\l3init.obj"
	-@erase "$(INTDIR)\mdct.obj"
	-@erase "$(INTDIR)\mhead.obj"
	-@erase "$(INTDIR)\msis.obj"
	-@erase "$(INTDIR)\pipeline.obj"
	-@erase "$(INTDIR)\pullbuffer.obj"
	-@erase "$(INTDIR)\uph.obj"
	-@erase "$(INTDIR)\upsf.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\wavep.obj"
	-@erase "$(OUTDIR)\cd.exp"
	-@erase "$(OUTDIR)\cd.lib"
	-@erase "$(OUTDIR)\cd.pdb"
	-@erase ".\cd.ilk"
	-@erase ".\cd.lmc"
	-@erase "..\..\..\..\config\config.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\\" /I "..\..\..\include" /I "..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I "..\..\..\..\ui\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "ASM_X86" /Fp"$(INTDIR)\cdlmc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\cdlmc.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cdlmc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x11800000" /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\cd.pdb" /debug /machine:I386 /def:".\cdlmc.def" /out:"cd.lmc" /implib:"$(OUTDIR)\cd.lib" /pdbtype:sept /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\cdlmc.def"
LINK32_OBJS= \
	"$(INTDIR)\eventbuffer.obj" \
	"$(INTDIR)\pipeline.obj" \
	"$(INTDIR)\pullbuffer.obj" \
	"$(INTDIR)\cdct.obj" \
	"$(INTDIR)\csbt.obj" \
	"$(INTDIR)\cup.obj" \
	"$(INTDIR)\cupl3.obj" \
	"$(INTDIR)\cwinm.obj" \
	"$(INTDIR)\dec8.obj" \
	"$(INTDIR)\hwin.obj" \
	"$(INTDIR)\icdct.obj" \
	"$(INTDIR)\isbt.obj" \
	"$(INTDIR)\iup.obj" \
	"$(INTDIR)\iwinm.obj" \
	"$(INTDIR)\l3dq.obj" \
	"$(INTDIR)\l3init.obj" \
	"$(INTDIR)\mdct.obj" \
	"$(INTDIR)\mhead.obj" \
	"$(INTDIR)\msis.obj" \
	"$(INTDIR)\uph.obj" \
	"$(INTDIR)\upsf.obj" \
	"$(INTDIR)\wavep.obj" \
	"$(INTDIR)\cdlmc.obj" \
	"$(INTDIR)\cdlmc.res" \
	"$(INTDIR)\cdctasm.obj" \
	"$(INTDIR)\cwin8asm.obj" \
	"$(INTDIR)\cwinasm.obj" \
	"$(INTDIR)\mdctasm.obj" \
	"$(INTDIR)\msisasm.obj"

".\cd.lmc" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "..\..\..\..\config\config.h" ".\cd.lmc"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                                    ..\..\..\..\base\win32\prj\plugins
	copy cdlmc.lmc                            ..\..\..\..\base\win32\prj\plugins
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
!IF EXISTS("cdlmc.dep")
!INCLUDE "cdlmc.dep"
!ELSE 
!MESSAGE Warning: cannot find "cdlmc.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "cdlmc - Win32 Release" || "$(CFG)" == "cdlmc - Win32 Debug" || "$(CFG)" == "cdlmc - Win32 NASM Release" || "$(CFG)" == "cdlmc - Win32 NASM Debug"
SOURCE=..\res\cdlmc.rc

!IF  "$(CFG)" == "cdlmc - Win32 Release"


"$(INTDIR)\cdlmc.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\cdlmc.res" /i "\Local\src\freeamp\lmc\cd\win32\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "cdlmc - Win32 Debug"


"$(INTDIR)\cdlmc.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\cdlmc.res" /i "\Local\src\freeamp\lmc\cd\win32\res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "cdlmc - Win32 NASM Release"


"$(INTDIR)\cdlmc.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\cdlmc.res" /i "\Local\src\freeamp\lmc\cd\win32\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "cdlmc - Win32 NASM Debug"


"$(INTDIR)\cdlmc.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\cdlmc.res" /i "\Local\src\freeamp\lmc\cd\win32\res" /d "_DEBUG" $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\config\config.win32

!IF  "$(CFG)" == "cdlmc - Win32 Release"

InputPath=..\..\..\..\config\config.win32

"..\..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\..\config\config.win32 ..\..\..\..\config\config.h
<< 
	

!ELSEIF  "$(CFG)" == "cdlmc - Win32 Debug"

InputPath=..\..\..\..\config\config.win32

"..\..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\..\config\config.win32 ..\..\..\..\config\config.h
<< 
	

!ELSEIF  "$(CFG)" == "cdlmc - Win32 NASM Release"

InputPath=..\..\..\..\config\config.win32

"..\..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\..\config\config.win32 ..\..\..\..\config\config.h
<< 
	

!ELSEIF  "$(CFG)" == "cdlmc - Win32 NASM Debug"

InputPath=..\..\..\..\config\config.win32

"..\..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\..\config\config.win32 ..\..\..\..\config\config.h
<< 
	

!ENDIF 

SOURCE=..\..\..\..\io\src\eventbuffer.cpp

"$(INTDIR)\eventbuffer.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\io\src\pipeline.cpp

"$(INTDIR)\pipeline.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\io\src\pullbuffer.cpp

"$(INTDIR)\pullbuffer.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\cdct.c

"$(INTDIR)\cdct.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\cdlmc.cpp

"$(INTDIR)\cdlmc.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\csbt.c

"$(INTDIR)\csbt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\cup.c

"$(INTDIR)\cup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\cupl3.c

"$(INTDIR)\cupl3.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\cwinm.c

"$(INTDIR)\cwinm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\dec8.c

"$(INTDIR)\dec8.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\hwin.c

"$(INTDIR)\hwin.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\icdct.c

"$(INTDIR)\icdct.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\isbt.c

"$(INTDIR)\isbt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\iup.c

"$(INTDIR)\iup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\iwinm.c

"$(INTDIR)\iwinm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\l3dq.c

"$(INTDIR)\l3dq.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\l3init.c

"$(INTDIR)\l3init.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\mdct.c

"$(INTDIR)\mdct.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\mhead.c

"$(INTDIR)\mhead.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\msis.c

"$(INTDIR)\msis.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\uph.c

"$(INTDIR)\uph.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\upsf.c

"$(INTDIR)\upsf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\wavep.c

"$(INTDIR)\wavep.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\cdctasm.asm

!IF  "$(CFG)" == "cdlmc - Win32 Release"

!ELSEIF  "$(CFG)" == "cdlmc - Win32 Debug"

!ELSEIF  "$(CFG)" == "cdlmc - Win32 NASM Release"

IntDir=.\Release
InputPath=..\..\src\cdctasm.asm
InputName=cdctasm

"$(INTDIR)\cdctasm.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	nasm -f win32 -o $(IntDir)\$(InputName).obj $(InputPath)
<< 
	

!ELSEIF  "$(CFG)" == "cdlmc - Win32 NASM Debug"

IntDir=.\Debug
InputPath=..\..\src\cdctasm.asm
InputName=cdctasm

"$(INTDIR)\cdctasm.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	nasm -f win32 -o $(IntDir)\$(InputName).obj $(InputPath)
<< 
	

!ENDIF 

SOURCE=..\..\src\cwin8asm.asm

!IF  "$(CFG)" == "cdlmc - Win32 Release"

!ELSEIF  "$(CFG)" == "cdlmc - Win32 Debug"

!ELSEIF  "$(CFG)" == "cdlmc - Win32 NASM Release"

IntDir=.\Release
InputPath=..\..\src\cwin8asm.asm
InputName=cwin8asm

"$(INTDIR)\cwin8asm.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	nasm -f win32 -o $(IntDir)\$(InputName).obj $(InputPath)
<< 
	

!ELSEIF  "$(CFG)" == "cdlmc - Win32 NASM Debug"

IntDir=.\Debug
InputPath=..\..\src\cwin8asm.asm
InputName=cwin8asm

"$(INTDIR)\cwin8asm.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	nasm -f win32 -o $(IntDir)\$(InputName).obj $(InputPath)
<< 
	

!ENDIF 

SOURCE=..\..\src\cwinasm.asm

!IF  "$(CFG)" == "cdlmc - Win32 Release"

!ELSEIF  "$(CFG)" == "cdlmc - Win32 Debug"

!ELSEIF  "$(CFG)" == "cdlmc - Win32 NASM Release"

IntDir=.\Release
InputPath=..\..\src\cwinasm.asm
InputName=cwinasm

"$(INTDIR)\cwinasm.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	nasm -f win32 -o $(IntDir)\$(InputName).obj $(InputPath)
<< 
	

!ELSEIF  "$(CFG)" == "cdlmc - Win32 NASM Debug"

IntDir=.\Debug
InputPath=..\..\src\cwinasm.asm
InputName=cwinasm

"$(INTDIR)\cwinasm.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	nasm -f win32 -o $(IntDir)\$(InputName).obj $(InputPath)
<< 
	

!ENDIF 

SOURCE=..\..\src\mdctasm.asm

!IF  "$(CFG)" == "cdlmc - Win32 Release"

!ELSEIF  "$(CFG)" == "cdlmc - Win32 Debug"

!ELSEIF  "$(CFG)" == "cdlmc - Win32 NASM Release"

IntDir=.\Release
InputPath=..\..\src\mdctasm.asm
InputName=mdctasm

"$(INTDIR)\mdctasm.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	nasm -f win32 -o $(IntDir)\$(InputName).obj $(InputPath)
<< 
	

!ELSEIF  "$(CFG)" == "cdlmc - Win32 NASM Debug"

IntDir=.\Debug
InputPath=..\..\src\mdctasm.asm
InputName=mdctasm

"$(INTDIR)\mdctasm.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	nasm -f win32 -o $(IntDir)\$(InputName).obj $(InputPath)
<< 
	

!ENDIF 

SOURCE=..\..\src\msisasm.asm

!IF  "$(CFG)" == "cdlmc - Win32 Release"

!ELSEIF  "$(CFG)" == "cdlmc - Win32 Debug"

!ELSEIF  "$(CFG)" == "cdlmc - Win32 NASM Release"

IntDir=.\Release
InputPath=..\..\src\msisasm.asm
InputName=msisasm

"$(INTDIR)\msisasm.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	nasm -f win32 -o $(IntDir)\$(InputName).obj $(InputPath)
<< 
	

!ELSEIF  "$(CFG)" == "cdlmc - Win32 NASM Debug"

IntDir=.\Debug
InputPath=..\..\src\msisasm.asm
InputName=msisasm

"$(INTDIR)\msisasm.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	nasm -f win32 -o $(IntDir)\$(InputName).obj $(InputPath)
<< 
	

!ENDIF 


!ENDIF 


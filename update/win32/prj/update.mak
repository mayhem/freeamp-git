# Microsoft Developer Studio Generated NMAKE File, Based on update.dsp
!IF "$(CFG)" == ""
CFG=update - Win32 NASM Debug
!MESSAGE No configuration specified. Defaulting to update - Win32 NASM Debug.
!ENDIF 

!IF "$(CFG)" != "update - Win32 Release" && "$(CFG)" != "update - Win32 Debug" && "$(CFG)" != "update - Win32 NASM Debug" && "$(CFG)" != "update - Win32 NASM Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "update.mak" CFG="update - Win32 NASM Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "update - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "update - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "update - Win32 NASM Debug" (based on "Win32 (x86) Application")
!MESSAGE "update - Win32 NASM Release" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "update - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\update.exe"


CLEAN :
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\update.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\update.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\\" /I "..\..\include" /I "..\include" /I "..\..\..\io\include" /I "..\..\..\base\include" /I "..\..\..\base\win32\include" /I "..\..\..\config" /I "..\..\..\ui\include" /I "..\..\..\lib\xml\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\update.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\update.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\update.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x11700000" /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\update.pdb" /machine:I386 /out:"update.exe" /libpath:"..\..\..\base\win32" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\update.res"

".\update.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : ".\update.exe"
   copy update.exe  ..\..\..\base\win32\prj
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "update - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : ".\update.exe"


CLEAN :
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\update.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\update.pdb"
	-@erase ".\update.exe"
	-@erase ".\update.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\\" /I "..\..\include" /I "..\include" /I "..\..\..\io\include" /I "..\..\..\base\include" /I "..\..\..\base\win32\include" /I "..\..\..\config" /I "..\..\..\ui\include" /I "..\..\..\lib\xml\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\update.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\update.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\update.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x11700000" /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\update.pdb" /debug /machine:I386 /out:"update.exe" /pdbtype:sept /libpath:"..\..\..\base\win32" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\update.res"

".\update.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : ".\update.exe"
   copy update.exe  ..\..\..\base\win32\prj
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "update - Win32 NASM Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\update.exe" "$(OUTDIR)\update.bsc"


CLEAN :
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\update.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\update.bsc"
	-@erase "$(OUTDIR)\update.pdb"
	-@erase ".\update.exe"
	-@erase ".\update.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\\" /I "..\..\include" /I "..\include" /I "..\..\..\io\include" /I "..\..\..\base\include" /I "..\..\..\base\win32\include" /I "..\..\..\config" /I "..\..\..\ui\include" /I "..\..\..\lib\xml\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\update.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\update.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\update.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\main.sbr"

"$(OUTDIR)\update.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=fabaselib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x11700000" /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\update.pdb" /debug /machine:I386 /out:"update.exe" /pdbtype:sept /libpath:"..\..\..\base\win32" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\update.res"

".\update.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : ".\update.exe" "$(OUTDIR)\update.bsc"
   copy update.exe  ..\..\..\base\win32\prj
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "update - Win32 NASM Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\update.exe"


CLEAN :
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\update.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\update.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\\" /I "..\..\include" /I "..\include" /I "..\..\..\io\include" /I "..\..\..\base\include" /I "..\..\..\base\win32\include" /I "..\..\..\config" /I "..\..\..\ui\include" /I "..\..\..\lib\xml\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\update.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\update.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\update.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x11700000" /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\update.pdb" /machine:I386 /out:"update.exe" /libpath:"..\..\..\base\win32" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\update.res"

".\update.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : ".\update.exe"
   copy update.exe  ..\..\..\base\win32\prj
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
!IF EXISTS("update.dep")
!INCLUDE "update.dep"
!ELSE 
!MESSAGE Warning: cannot find "update.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "update - Win32 Release" || "$(CFG)" == "update - Win32 Debug" || "$(CFG)" == "update - Win32 NASM Debug" || "$(CFG)" == "update - Win32 NASM Release"
SOURCE=..\main.cpp

!IF  "$(CFG)" == "update - Win32 Release"


"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "update - Win32 Debug"


"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "update - Win32 NASM Debug"


"$(INTDIR)\main.obj"	"$(INTDIR)\main.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "update - Win32 NASM Release"


"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\res\update.rc

!IF  "$(CFG)" == "update - Win32 Release"


"$(INTDIR)\update.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\update.res" /i "\Local\src\freeamp\update\win32\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "update - Win32 Debug"


"$(INTDIR)\update.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\update.res" /i "\Local\src\freeamp\update\win32\res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "update - Win32 NASM Debug"


"$(INTDIR)\update.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\update.res" /i "\Local\src\freeamp\update\win32\res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "update - Win32 NASM Release"


"$(INTDIR)\update.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\update.res" /i "\Local\src\freeamp\update\win32\res" /d "NDEBUG" $(SOURCE)


!ENDIF 


!ENDIF 


# Microsoft Developer Studio Generated NMAKE File, Based on rmp.dsp
!IF "$(CFG)" == ""
CFG=rmp - Win32 NASM Debug
!MESSAGE No configuration specified. Defaulting to rmp - Win32 NASM Debug.
!ENDIF 

!IF "$(CFG)" != "rmp - Win32 Release" && "$(CFG)" != "rmp - Win32 Debug" &&\
 "$(CFG)" != "rmp - Win32 NASM Release" && "$(CFG)" != "rmp - Win32 NASM Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rmp.mak" CFG="rmp - Win32 NASM Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rmp - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "rmp - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "rmp - Win32 NASM Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "rmp - Win32 NASM Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "rmp - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\rmp.dlf"

!ELSE 

ALL : ".\rmp.dlf"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\rmp.obj"
	-@erase "$(INTDIR)\rmp.res"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\rmp.exp"
	-@erase "$(OUTDIR)\rmp.lib"
	-@erase ".\rmp.dlf"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\..\lmc\include" /I "..\\" /I\
 "..\..\include" /I "..\include" /I "..\..\..\io\include" /I\
 "..\..\..\base\include" /I "..\..\..\base\win32\include" /I "..\..\..\config"\
 /I "..\..\..\ui\include" /I "..\..\..\lib\xml\include" /D "WIN32" /D "NDEBUG"\
 /D "_WINDOWS" /Fp"$(INTDIR)\rmp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\rmp.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\rmp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\rmp.pdb" /machine:I386 /def:".\rmp.def" /out:"rmp.dlf"\
 /implib:"$(OUTDIR)\rmp.lib" 
DEF_FILE= \
	".\rmp.def"
LINK32_OBJS= \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\Parse.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\rmp.obj" \
	"$(INTDIR)\rmp.res" \
	"$(INTDIR)\utility.obj"

".\rmp.dlf" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : ".\rmp.dlf"
   IF NOT EXIST ..\..\..\base\win32\prj\plugins mkdir                                ..\..\..\base\win32\prj\plugins
	copy rmp.dlf  ..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "rmp - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\rmp.dlf"

!ELSE 

ALL : ".\rmp.dlf"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\rmp.obj"
	-@erase "$(INTDIR)\rmp.res"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\rmp.exp"
	-@erase "$(OUTDIR)\rmp.lib"
	-@erase "$(OUTDIR)\rmp.pdb"
	-@erase ".\rmp.dlf"
	-@erase ".\rmp.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\lmc\include" /I "..\\"\
 /I "..\..\include" /I "..\include" /I "..\..\..\io\include" /I\
 "..\..\..\base\include" /I "..\..\..\base\win32\include" /I "..\..\..\config"\
 /I "..\..\..\ui\include" /I "..\..\..\lib\xml\include" /D "WIN32" /D "_DEBUG"\
 /D "_WINDOWS" /Fp"$(INTDIR)\rmp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\rmp.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\rmp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)\rmp.pdb" /debug /machine:I386 /def:".\rmp.def" /out:"rmp.dlf"\
 /implib:"$(OUTDIR)\rmp.lib" /pdbtype:sept 
DEF_FILE= \
	".\rmp.def"
LINK32_OBJS= \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\Parse.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\rmp.obj" \
	"$(INTDIR)\rmp.res" \
	"$(INTDIR)\utility.obj"

".\rmp.dlf" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : ".\rmp.dlf"
   IF NOT EXIST ..\..\..\base\win32\prj\plugins mkdir                                ..\..\..\base\win32\prj\plugins
	copy rmp.dlf  ..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "rmp - Win32 NASM Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\rmp.dlf"

!ELSE 

ALL : ".\rmp.dlf"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\rmp.obj"
	-@erase "$(INTDIR)\rmp.res"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\rmp.exp"
	-@erase "$(OUTDIR)\rmp.lib"
	-@erase ".\rmp.dlf"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\..\lmc\include" /I "..\\" /I\
 "..\..\include" /I "..\include" /I "..\..\..\io\include" /I\
 "..\..\..\base\include" /I "..\..\..\base\win32\include" /I "..\..\..\config"\
 /I "..\..\..\ui\include" /I "..\..\..\lib\xml\include" /D "WIN32" /D "NDEBUG"\
 /D "_WINDOWS" /Fp"$(INTDIR)\rmp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\rmp.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\rmp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\rmp.pdb" /machine:I386 /def:".\rmp.def" /out:"rmp.dlf"\
 /implib:"$(OUTDIR)\rmp.lib" 
DEF_FILE= \
	".\rmp.def"
LINK32_OBJS= \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\Parse.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\rmp.obj" \
	"$(INTDIR)\rmp.res" \
	"$(INTDIR)\utility.obj"

".\rmp.dlf" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : ".\rmp.dlf"
   IF NOT EXIST ..\..\..\base\win32\prj\plugins mkdir                                ..\..\..\base\win32\prj\plugins
	copy rmp.dlf  ..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "rmp - Win32 NASM Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\rmp.dlf"

!ELSE 

ALL : ".\rmp.dlf"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\rmp.obj"
	-@erase "$(INTDIR)\rmp.res"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\rmp.exp"
	-@erase "$(OUTDIR)\rmp.lib"
	-@erase "$(OUTDIR)\rmp.pdb"
	-@erase ".\rmp.dlf"
	-@erase ".\rmp.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /I "..\..\include" /I\
 "..\include" /I "..\..\..\io\include" /I "..\..\..\base\include" /I\
 "..\..\..\base\win32\include" /I "..\..\..\config" /I "..\..\..\ui\include" /I\
 "..\..\..\lib\xml\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\rmp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\rmp.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\rmp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)\rmp.pdb" /debug /machine:I386 /def:".\rmp.def" /out:"rmp.dlf"\
 /implib:"$(OUTDIR)\rmp.lib" /pdbtype:sept 
DEF_FILE= \
	".\rmp.def"
LINK32_OBJS= \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\Parse.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\rmp.obj" \
	"$(INTDIR)\rmp.res" \
	"$(INTDIR)\utility.obj"

".\rmp.dlf" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : ".\rmp.dlf"
   IF NOT EXIST ..\..\..\base\win32\prj\plugins mkdir                                ..\..\..\base\win32\prj\plugins
	copy rmp.dlf  ..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(CFG)" == "rmp - Win32 Release" || "$(CFG)" == "rmp - Win32 Debug" ||\
 "$(CFG)" == "rmp - Win32 NASM Release" || "$(CFG)" == "rmp - Win32 NASM Debug"
SOURCE=..\..\..\base\src\debug.cpp
DEP_CPP_DEBUG=\
	"..\..\..\base\include\debug.h"\
	

"$(INTDIR)\debug.obj" : $(SOURCE) $(DEP_CPP_DEBUG) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\xml\src\Parse.cpp
DEP_CPP_PARSE=\
	"..\..\..\base\include\debug.h"\
	"..\..\..\base\include\errors.h"\
	"..\..\..\lib\xml\include\Parse.h"\
	
NODEP_CPP_PARSE=\
	"..\..\..\base\include\config.h"\
	

"$(INTDIR)\Parse.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\base\src\preferences.cpp
DEP_CPP_PREFE=\
	"..\..\..\base\include\errors.h"\
	"..\..\..\base\include\preferences.h"\
	
NODEP_CPP_PREFE=\
	"..\..\..\base\include\config.h"\
	

"$(INTDIR)\preferences.obj" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\rmp.cpp
DEP_CPP_RMP_C=\
	"..\..\..\base\include\downloadformat.h"\
	"..\..\..\base\include\downloadmanager.h"\
	"..\..\..\base\include\errors.h"\
	"..\..\..\base\include\facontext.h"\
	"..\..\..\base\include\log.h"\
	"..\..\..\base\include\metadata.h"\
	"..\..\..\base\include\plmevent.h"\
	"..\..\..\base\include\preferences.h"\
	"..\..\..\base\include\registry.h"\
	"..\..\..\base\include\thread.h"\
	"..\..\..\base\include\utility.h"\
	"..\..\..\base\win32\include\Mutex.h"\
	"..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\lib\xml\include\Parse.h"\
	"..\rmp.h"\
	
NODEP_CPP_RMP_C=\
	"..\..\..\base\include\config.h"\
	"..\config.h"\
	

"$(INTDIR)\rmp.obj" : $(SOURCE) $(DEP_CPP_RMP_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\base\src\utility.cpp
DEP_CPP_UTILI=\
	"..\..\..\base\include\errors.h"\
	"..\..\..\base\include\preferences.h"\
	"..\..\..\base\include\utility.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	
NODEP_CPP_UTILI=\
	"..\..\..\base\include\config.h"\
	"..\..\..\base\src\config.h"\
	

"$(INTDIR)\utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\res\rmp.rc

!IF  "$(CFG)" == "rmp - Win32 Release"


"$(INTDIR)\rmp.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\rmp.res" /i "\FreeAmp\freeamp\dlm\rmp\res" /d\
 "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "rmp - Win32 Debug"


"$(INTDIR)\rmp.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\rmp.res" /i "\FreeAmp\freeamp\dlm\rmp\res" /d\
 "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "rmp - Win32 NASM Release"


"$(INTDIR)\rmp.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\rmp.res" /i "\FreeAmp\freeamp\dlm\rmp\res" /d\
 "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "rmp - Win32 NASM Debug"


"$(INTDIR)\rmp.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\rmp.res" /i "\FreeAmp\freeamp\dlm\rmp\res" /d\
 "_DEBUG" $(SOURCE)


!ENDIF 


!ENDIF 


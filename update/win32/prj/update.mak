# Microsoft Developer Studio Generated NMAKE File, Based on update.dsp
!IF "$(CFG)" == ""
CFG=update - Win32 NASM Debug
!MESSAGE No configuration specified. Defaulting to update - Win32 NASM Debug.
!ENDIF 

!IF "$(CFG)" != "update - Win32 Release" && "$(CFG)" != "update - Win32 Debug"\
 && "$(CFG)" != "update - Win32 NASM Debug" && "$(CFG)" !=\
 "update - Win32 NASM Release"
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

!IF  "$(CFG)" == "update - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\update.exe"

!ELSE 

ALL : "fabaselib - Win32 Release" ".\update.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fabaselib - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\update.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase ".\update.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\\" /I "..\..\include" /I "..\include"\
 /I "..\..\..\io\include" /I "..\..\..\base\include" /I\
 "..\..\..\base\win32\include" /I "..\..\..\config" /I "..\..\..\ui\include" /I\
 "..\..\..\lib\xml\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\update.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\update.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\update.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\update.pdb" /machine:I386 /out:"update.exe"\
 /libpath:"..\..\..\base\win32" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\update.res" \
	"..\..\..\base\win32\fabaselib.lib"

".\update.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "fabaselib - Win32 Release" ".\update.exe"
   copy update.exe  ..\..\..\base\win32\prj
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "update - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\update.exe"

!ELSE 

ALL : "fabaselib - Win32 Debug" ".\update.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fabaselib - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\update.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\update.pdb"
	-@erase ".\update.exe"
	-@erase ".\update.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /I "..\..\include" /I\
 "..\include" /I "..\..\..\io\include" /I "..\..\..\base\include" /I\
 "..\..\..\base\win32\include" /I "..\..\..\config" /I "..\..\..\ui\include" /I\
 "..\..\..\lib\xml\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\update.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\update.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\update.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\update.pdb" /debug /machine:I386 /out:"update.exe"\
 /pdbtype:sept /libpath:"..\..\..\base\win32" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\update.res" \
	"..\..\..\base\win32\fabaselib.lib"

".\update.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "fabaselib - Win32 Debug" ".\update.exe"
   copy update.exe  ..\..\..\base\win32\prj
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "update - Win32 NASM Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : ".\update.exe" "$(OUTDIR)\update.bsc"

!ELSE 

ALL : "fabaselib - Win32 NASM Debug" ".\update.exe" "$(OUTDIR)\update.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fabaselib - Win32 NASM DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\update.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\update.bsc"
	-@erase "$(OUTDIR)\update.pdb"
	-@erase ".\update.exe"
	-@erase ".\update.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /I "..\..\include" /I\
 "..\include" /I "..\..\..\io\include" /I "..\..\..\base\include" /I\
 "..\..\..\base\win32\include" /I "..\..\..\config" /I "..\..\..\ui\include" /I\
 "..\..\..\lib\xml\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /FR"$(INTDIR)\\" /Fp"$(INTDIR)\update.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/

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
LINK32_FLAGS=fabaselib.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\update.pdb" /debug /machine:I386 /out:"update.exe"\
 /pdbtype:sept /libpath:"..\..\..\base\win32" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\update.res" \
	"..\..\..\base\win32\fabaselib.lib"

".\update.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "fabaselib - Win32 NASM Debug" ".\update.exe"\
 "$(OUTDIR)\update.bsc"
   copy update.exe  ..\..\..\base\win32\prj
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "update - Win32 NASM Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\update.exe"

!ELSE 

ALL : "fabaselib - Win32 NASM Release" ".\update.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fabaselib - Win32 NASM ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\update.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase ".\update.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\\" /I "..\..\include" /I "..\include"\
 /I "..\..\..\io\include" /I "..\..\..\base\include" /I\
 "..\..\..\base\win32\include" /I "..\..\..\config" /I "..\..\..\ui\include" /I\
 "..\..\..\lib\xml\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\update.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\update.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\update.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\update.pdb" /machine:I386 /out:"update.exe"\
 /libpath:"..\..\..\base\win32" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\update.res" \
	"..\..\..\base\win32\fabaselib.lib"

".\update.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "fabaselib - Win32 NASM Release" ".\update.exe"
   copy update.exe  ..\..\..\base\win32\prj
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(CFG)" == "update - Win32 Release" || "$(CFG)" == "update - Win32 Debug"\
 || "$(CFG)" == "update - Win32 NASM Debug" || "$(CFG)" ==\
 "update - Win32 NASM Release"
SOURCE=..\main.cpp

!IF  "$(CFG)" == "update - Win32 Release"

DEP_CPP_MAIN_=\
	"..\..\..\base\include\errors.h"\
	"..\..\..\base\include\facontext.h"\
	"..\..\..\base\include\log.h"\
	"..\..\..\base\include\preferences.h"\
	"..\..\..\base\include\thread.h"\
	"..\..\..\base\include\updatemanager.h"\
	"..\..\..\base\win32\include\mutex.h"\
	"..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\base\win32\include\win32prefs.h"\
	"..\..\..\config\config.h"\
	"..\..\..\lib\xml\include\Parse.h"\
	

"$(INTDIR)\main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "update - Win32 Debug"

DEP_CPP_MAIN_=\
	"..\..\..\base\include\errors.h"\
	"..\..\..\base\include\facontext.h"\
	"..\..\..\base\include\log.h"\
	"..\..\..\base\include\preferences.h"\
	"..\..\..\base\include\thread.h"\
	"..\..\..\base\include\updatemanager.h"\
	"..\..\..\base\win32\include\mutex.h"\
	"..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\base\win32\include\win32prefs.h"\
	"..\..\..\config\config.h"\
	"..\..\..\lib\xml\include\Parse.h"\
	

"$(INTDIR)\main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "update - Win32 NASM Debug"

DEP_CPP_MAIN_=\
	"..\..\..\base\include\errors.h"\
	"..\..\..\base\include\facontext.h"\
	"..\..\..\base\include\log.h"\
	"..\..\..\base\include\preferences.h"\
	"..\..\..\base\include\thread.h"\
	"..\..\..\base\include\updatemanager.h"\
	"..\..\..\base\win32\include\mutex.h"\
	"..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\base\win32\include\win32prefs.h"\
	"..\..\..\config\config.h"\
	"..\..\..\lib\xml\include\Parse.h"\
	

"$(INTDIR)\main.obj"	"$(INTDIR)\main.sbr" : $(SOURCE) $(DEP_CPP_MAIN_)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "update - Win32 NASM Release"

DEP_CPP_MAIN_=\
	"..\..\..\base\include\errors.h"\
	"..\..\..\base\include\facontext.h"\
	"..\..\..\base\include\log.h"\
	"..\..\..\base\include\preferences.h"\
	"..\..\..\base\include\thread.h"\
	"..\..\..\base\include\updatemanager.h"\
	"..\..\..\base\win32\include\mutex.h"\
	"..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\base\win32\include\win32prefs.h"\
	"..\..\..\config\config.h"\
	"..\..\..\lib\xml\include\Parse.h"\
	

"$(INTDIR)\main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\res\update.rc

!IF  "$(CFG)" == "update - Win32 Release"


"$(INTDIR)\update.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\update.res" /i\
 "\FreeAmp\freeamp\update\win32\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "update - Win32 Debug"


"$(INTDIR)\update.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\update.res" /i\
 "\FreeAmp\freeamp\update\win32\res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "update - Win32 NASM Debug"


"$(INTDIR)\update.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\update.res" /i\
 "\FreeAmp\freeamp\update\win32\res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "update - Win32 NASM Release"


"$(INTDIR)\update.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\update.res" /i\
 "\FreeAmp\freeamp\update\win32\res" /d "NDEBUG" $(SOURCE)


!ENDIF 

!IF  "$(CFG)" == "update - Win32 Release"

"fabaselib - Win32 Release" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 Release" 
   cd "..\..\..\update\win32\prj"

"fabaselib - Win32 ReleaseCLEAN" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak\
 CFG="fabaselib - Win32 Release" RECURSE=1 
   cd "..\..\..\update\win32\prj"

!ELSEIF  "$(CFG)" == "update - Win32 Debug"

"fabaselib - Win32 Debug" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 Debug" 
   cd "..\..\..\update\win32\prj"

"fabaselib - Win32 DebugCLEAN" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak CFG="fabaselib - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\update\win32\prj"

!ELSEIF  "$(CFG)" == "update - Win32 NASM Debug"

"fabaselib - Win32 NASM Debug" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 NASM Debug" 
   cd "..\..\..\update\win32\prj"

"fabaselib - Win32 NASM DebugCLEAN" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak\
 CFG="fabaselib - Win32 NASM Debug" RECURSE=1 
   cd "..\..\..\update\win32\prj"

!ELSEIF  "$(CFG)" == "update - Win32 NASM Release"

"fabaselib - Win32 NASM Release" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak\
 CFG="fabaselib - Win32 NASM Release" 
   cd "..\..\..\update\win32\prj"

"fabaselib - Win32 NASM ReleaseCLEAN" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak\
 CFG="fabaselib - Win32 NASM Release" RECURSE=1 
   cd "..\..\..\update\win32\prj"

!ENDIF 


!ENDIF 


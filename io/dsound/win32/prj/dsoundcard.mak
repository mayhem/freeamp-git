# Microsoft Developer Studio Generated NMAKE File, Based on dsoundcard.dsp
!IF "$(CFG)" == ""
CFG=dsoundcard - Win32 Debug
!MESSAGE No configuration specified. Defaulting to dsoundcard - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "dsoundcard - Win32 Release" && "$(CFG)" != "dsoundcard - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dsoundcard.mak" CFG="dsoundcard - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dsoundcard - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "dsoundcard - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "dsoundcard - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\dsoundcard.pmo"


CLEAN :
	-@erase "$(INTDIR)\dsoundcardpmo.obj"
	-@erase "$(INTDIR)\eventbuffer.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\pullbuffer.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(OUTDIR)\dsoundcard.exp"
	-@erase "$(OUTDIR)\dsoundcard.lib"
	-@erase ".\dsoundcard.pmo"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /Ob2 /I "..\include" /I "..\..\include" /I "..\..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I "..\..\..\..\ui\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\dsoundcard.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\dsoundcard.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=dsound.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\dsoundcard.pdb" /machine:I386 /def:".\dsoundcard.def" /out:"dsoundcard.pmo" /implib:"$(OUTDIR)\dsoundcard.lib" 
DEF_FILE= \
	".\dsoundcard.def"
LINK32_OBJS= \
	"$(INTDIR)\dsoundcardpmo.obj" \
	"$(INTDIR)\eventbuffer.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\pullbuffer.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\win32thread.obj"

".\dsoundcard.pmo" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : ".\dsoundcard.pmo"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                    ..\..\..\..\base\win32\prj\plugins
	copy dsoundcard.pmo               ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "dsoundcard - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\..\..\config\config.h" ".\dsoundcard.pmo" "$(OUTDIR)\dsoundcard.bsc"


CLEAN :
	-@erase "$(INTDIR)\dsoundcardpmo.obj"
	-@erase "$(INTDIR)\dsoundcardpmo.sbr"
	-@erase "$(INTDIR)\eventbuffer.obj"
	-@erase "$(INTDIR)\eventbuffer.sbr"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\log.sbr"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\mutex.sbr"
	-@erase "$(INTDIR)\pullbuffer.obj"
	-@erase "$(INTDIR)\pullbuffer.sbr"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\semaphore.sbr"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\thread.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(INTDIR)\win32thread.sbr"
	-@erase "$(OUTDIR)\dsoundcard.bsc"
	-@erase "$(OUTDIR)\dsoundcard.exp"
	-@erase "$(OUTDIR)\dsoundcard.lib"
	-@erase "$(OUTDIR)\dsoundcard.pdb"
	-@erase ".\dsoundcard.ilk"
	-@erase ".\dsoundcard.pmo"
	-@erase "..\..\..\..\config\config.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\..\include" /I "..\..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I "..\..\..\..\ui\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\dsoundcard.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\dsoundcard.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\dsoundcardpmo.sbr" \
	"$(INTDIR)\eventbuffer.sbr" \
	"$(INTDIR)\log.sbr" \
	"$(INTDIR)\mutex.sbr" \
	"$(INTDIR)\pullbuffer.sbr" \
	"$(INTDIR)\semaphore.sbr" \
	"$(INTDIR)\thread.sbr" \
	"$(INTDIR)\win32thread.sbr"

"$(OUTDIR)\dsoundcard.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=dsound.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\dsoundcard.pdb" /debug /machine:I386 /def:".\dsoundcard.def" /out:"dsoundcard.pmo" /implib:"$(OUTDIR)\dsoundcard.lib" /pdbtype:sept 
DEF_FILE= \
	".\dsoundcard.def"
LINK32_OBJS= \
	"$(INTDIR)\dsoundcardpmo.obj" \
	"$(INTDIR)\eventbuffer.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\pullbuffer.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\win32thread.obj"

".\dsoundcard.pmo" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "..\..\..\..\config\config.h" ".\dsoundcard.pmo" "$(OUTDIR)\dsoundcard.bsc"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                    ..\..\..\..\base\win32\prj\plugins
	copy dsoundcard.pmo               ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("dsoundcard.dep")
!INCLUDE "dsoundcard.dep"
!ELSE 
!MESSAGE Warning: cannot find "dsoundcard.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "dsoundcard - Win32 Release" || "$(CFG)" == "dsoundcard - Win32 Debug"
SOURCE=..\..\..\..\config\config.win32

!IF  "$(CFG)" == "dsoundcard - Win32 Release"

InputPath=..\..\..\..\config\config.win32

"..\..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\..\config\config.win32 ..\..\..\..\config\config.h
<< 
	

!ELSEIF  "$(CFG)" == "dsoundcard - Win32 Debug"

InputPath=..\..\..\..\config\config.win32

"..\..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\..\config\config.win32 ..\..\..\..\config\config.h
<< 
	

!ENDIF 

SOURCE=..\src\dsoundcardpmo.cpp

!IF  "$(CFG)" == "dsoundcard - Win32 Release"


"$(INTDIR)\dsoundcardpmo.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "dsoundcard - Win32 Debug"


"$(INTDIR)\dsoundcardpmo.obj"	"$(INTDIR)\dsoundcardpmo.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\src\eventbuffer.cpp

!IF  "$(CFG)" == "dsoundcard - Win32 Release"


"$(INTDIR)\eventbuffer.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "dsoundcard - Win32 Debug"


"$(INTDIR)\eventbuffer.obj"	"$(INTDIR)\eventbuffer.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\base\src\log.cpp

!IF  "$(CFG)" == "dsoundcard - Win32 Release"


"$(INTDIR)\log.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "dsoundcard - Win32 Debug"


"$(INTDIR)\log.obj"	"$(INTDIR)\log.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\base\win32\src\mutex.cpp

!IF  "$(CFG)" == "dsoundcard - Win32 Release"


"$(INTDIR)\mutex.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "dsoundcard - Win32 Debug"


"$(INTDIR)\mutex.obj"	"$(INTDIR)\mutex.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\src\pullbuffer.cpp

!IF  "$(CFG)" == "dsoundcard - Win32 Release"


"$(INTDIR)\pullbuffer.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "dsoundcard - Win32 Debug"


"$(INTDIR)\pullbuffer.obj"	"$(INTDIR)\pullbuffer.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\base\win32\src\semaphore.cpp

!IF  "$(CFG)" == "dsoundcard - Win32 Release"


"$(INTDIR)\semaphore.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "dsoundcard - Win32 Debug"


"$(INTDIR)\semaphore.obj"	"$(INTDIR)\semaphore.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\base\src\thread.cpp

!IF  "$(CFG)" == "dsoundcard - Win32 Release"


"$(INTDIR)\thread.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "dsoundcard - Win32 Debug"


"$(INTDIR)\thread.obj"	"$(INTDIR)\thread.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\base\win32\src\win32thread.cpp

!IF  "$(CFG)" == "dsoundcard - Win32 Release"


"$(INTDIR)\win32thread.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "dsoundcard - Win32 Debug"


"$(INTDIR)\win32thread.obj"	"$(INTDIR)\win32thread.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 


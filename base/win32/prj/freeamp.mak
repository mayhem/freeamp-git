# Microsoft Developer Studio Generated NMAKE File, Based on freeamp.dsp
!IF "$(CFG)" == ""
CFG=freeamp - Win32 Debug
!MESSAGE No configuration specified. Defaulting to freeamp - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "freeamp - Win32 Release" && "$(CFG)" !=\
 "freeamp - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "freeamp.mak" CFG="freeamp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "freeamp - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "freeamp - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\freeamp.exe"

!ELSE 

ALL : "freeampui - Win32 Release" "simple - Win32 Release"\
 "xing - Win32 Release" "soundcard - Win32 Release" "fileinput - Win32 Release"\
 ".\freeamp.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fileinput - Win32 ReleaseCLEAN" "soundcard - Win32 ReleaseCLEAN"\
 "xing - Win32 ReleaseCLEAN" "simple - Win32 ReleaseCLEAN"\
 "freeampui - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\hashtable.obj"
	-@erase "$(INTDIR)\lmcregistry.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\player.obj"
	-@erase "$(INTDIR)\playlist.obj"
	-@erase "$(INTDIR)\pmiregistry.obj"
	-@erase "$(INTDIR)\pmoregistry.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\registrar.obj"
	-@erase "$(INTDIR)\registry.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\uiregistry.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase ".\freeamp.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\include" /I "..\..\include" /I\
 "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I\
 "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include"\
 /I "..\..\..\lmc\include" /I "..\..\..\ui\dummy\include" /D "WIN32" /D "NDEBUG"\
 /D "_WINDOWS" /Fp"$(INTDIR)\freeamp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /c 
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
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\freeamp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\freeamp.pdb" /machine:I386 /out:"freeamp.exe" 
LINK32_OBJS= \
	"$(INTDIR)\hashtable.obj" \
	"$(INTDIR)\lmcregistry.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\player.obj" \
	"$(INTDIR)\playlist.obj" \
	"$(INTDIR)\pmiregistry.obj" \
	"$(INTDIR)\pmoregistry.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\registrar.obj" \
	"$(INTDIR)\registry.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\uiregistry.obj" \
	"$(INTDIR)\win32thread.obj" \
	"..\..\..\io\local\win32\prj\Release\fileinput.lib" \
	"..\..\..\io\soundcard\win32\prj\Release\soundcard.lib" \
	"..\..\..\lmc\xingmp3\win32\prj\Release\xing.lib" \
	"..\..\..\ui\freeamp\win32\prj\Release\freeamp.lib" \
	"..\..\..\ui\simple\win32\prj\Release\simple.lib"

".\freeamp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\freeamp.exe"

!ELSE 

ALL : "freeampui - Win32 Debug" "simple - Win32 Debug" "xing - Win32 Debug"\
 "soundcard - Win32 Debug" "fileinput - Win32 Debug" ".\freeamp.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fileinput - Win32 DebugCLEAN" "soundcard - Win32 DebugCLEAN"\
 "xing - Win32 DebugCLEAN" "simple - Win32 DebugCLEAN"\
 "freeampui - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\hashtable.obj"
	-@erase "$(INTDIR)\lmcregistry.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\player.obj"
	-@erase "$(INTDIR)\playlist.obj"
	-@erase "$(INTDIR)\pmiregistry.obj"
	-@erase "$(INTDIR)\pmoregistry.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\registrar.obj"
	-@erase "$(INTDIR)\registry.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\uiregistry.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(OUTDIR)\freeamp.pdb"
	-@erase ".\freeamp.exe"
	-@erase ".\freeamp.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\ui\dummy\include" /I\
 "..\include" /I "..\..\include" /I "..\..\..\config" /I\
 "..\..\..\ui\win32Test\include" /I "..\..\..\ui\win32Test\res" /I\
 "..\..\..\io\include" /I "..\..\..\ui\include" /I "..\..\..\lmc\include" /D\
 "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\freeamp.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\freeamp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\freeamp.pdb" /debug /machine:I386 /out:"freeamp.exe"\
 /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\hashtable.obj" \
	"$(INTDIR)\lmcregistry.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\player.obj" \
	"$(INTDIR)\playlist.obj" \
	"$(INTDIR)\pmiregistry.obj" \
	"$(INTDIR)\pmoregistry.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\registrar.obj" \
	"$(INTDIR)\registry.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\uiregistry.obj" \
	"$(INTDIR)\win32thread.obj" \
	"..\..\..\io\local\win32\prj\Debug\fileinput.lib" \
	"..\..\..\io\soundcard\win32\prj\Debug\soundcard.lib" \
	"..\..\..\lmc\xingmp3\win32\prj\Debug\xing.lib" \
	"..\..\..\ui\freeamp\win32\prj\Debug\freeamp.lib" \
	"..\..\..\ui\simple\win32\prj\Debug\simple.lib"

".\freeamp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "freeamp - Win32 Release" || "$(CFG)" ==\
 "freeamp - Win32 Debug"
SOURCE=..\..\src\hashtable.cpp
DEP_CPP_HASHT=\
	"..\..\..\config\config.h"\
	"..\..\include\hashtable.h"\
	

"$(INTDIR)\hashtable.obj" : $(SOURCE) $(DEP_CPP_HASHT) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\main.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_MAIN_=\
	"..\..\..\config\config.h"\
	"..\..\..\io\include\pmi.h"\
	"..\..\..\io\include\pmiregistry.h"\
	"..\..\..\io\include\pmo.h"\
	"..\..\..\io\include\pmoregistry.h"\
	"..\..\..\lmc\include\lmc.h"\
	"..\..\..\lmc\include\lmcregistry.h"\
	"..\..\..\ui\include\ui.h"\
	"..\..\..\ui\include\uiregistry.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\player.h"\
	"..\..\include\playlist.h"\
	"..\..\include\queue.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\vector.h"\
	"..\include\mutex.h"\
	"..\include\preferences.h"\
	"..\include\semaphore.h"\
	
NODEP_CPP_MAIN_=\
	"..\..\include\win32impl.h"\
	

"$(INTDIR)\main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_MAIN_=\
	"..\..\..\config\config.h"\
	"..\..\..\io\include\pmi.h"\
	"..\..\..\io\include\pmiregistry.h"\
	"..\..\..\io\include\pmo.h"\
	"..\..\..\io\include\pmoregistry.h"\
	"..\..\..\lmc\include\lmc.h"\
	"..\..\..\lmc\include\lmcregistry.h"\
	"..\..\..\ui\include\ui.h"\
	"..\..\..\ui\include\uiregistry.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\player.h"\
	"..\..\include\playlist.h"\
	"..\..\include\queue.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\vector.h"\
	"..\include\mutex.h"\
	"..\include\preferences.h"\
	"..\include\semaphore.h"\
	

"$(INTDIR)\main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\player.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_PLAYE=\
	"..\..\..\config\config.h"\
	"..\..\..\io\include\pmi.h"\
	"..\..\..\io\include\pmiregistry.h"\
	"..\..\..\io\include\pmo.h"\
	"..\..\..\io\include\pmoregistry.h"\
	"..\..\..\lmc\include\lmc.h"\
	"..\..\..\lmc\include\lmcregistry.h"\
	"..\..\..\ui\include\ui.h"\
	"..\..\..\ui\include\uiregistry.h"\
	"..\..\include\debug.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\id3v1.h"\
	"..\..\include\player.h"\
	"..\..\include\playlist.h"\
	"..\..\include\queue.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\vector.h"\
	"..\include\mutex.h"\
	"..\include\preferences.h"\
	"..\include\semaphore.h"\
	
NODEP_CPP_PLAYE=\
	"..\..\include\win32impl.h"\
	

"$(INTDIR)\player.obj" : $(SOURCE) $(DEP_CPP_PLAYE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_PLAYE=\
	"..\..\..\config\config.h"\
	"..\..\..\io\include\pmi.h"\
	"..\..\..\io\include\pmiregistry.h"\
	"..\..\..\io\include\pmo.h"\
	"..\..\..\io\include\pmoregistry.h"\
	"..\..\..\lmc\include\lmc.h"\
	"..\..\..\lmc\include\lmcregistry.h"\
	"..\..\..\ui\include\ui.h"\
	"..\..\..\ui\include\uiregistry.h"\
	"..\..\include\debug.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\id3v1.h"\
	"..\..\include\player.h"\
	"..\..\include\playlist.h"\
	"..\..\include\queue.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\vector.h"\
	"..\include\mutex.h"\
	"..\include\preferences.h"\
	"..\include\semaphore.h"\
	

"$(INTDIR)\player.obj" : $(SOURCE) $(DEP_CPP_PLAYE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\playlist.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_PLAYL=\
	"..\..\..\config\config.h"\
	"..\..\include\playlist.h"\
	"..\..\include\vector.h"\
	

"$(INTDIR)\playlist.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_PLAYL=\
	"..\..\..\config\config.h"\
	"..\..\include\playlist.h"\
	"..\..\include\vector.h"\
	

"$(INTDIR)\playlist.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\registrar.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_REGIS=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\vector.h"\
	"..\include\preferences.h"\
	
NODEP_CPP_REGIS=\
	"..\..\include\win32impl.h"\
	

"$(INTDIR)\registrar.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_REGIS=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\vector.h"\
	"..\include\preferences.h"\
	

"$(INTDIR)\registrar.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\thread.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_THREA=\
	"..\..\..\config\config.h"\
	"..\..\include\thread.h"\
	"..\include\win32thread.h"\
	
NODEP_CPP_THREA=\
	"..\..\src\linuxthread.h"\
	

"$(INTDIR)\thread.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_THREA=\
	"..\..\..\config\config.h"\
	"..\..\include\thread.h"\
	"..\include\win32thread.h"\
	

"$(INTDIR)\thread.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\config\config.win32

!IF  "$(CFG)" == "freeamp - Win32 Release"

InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h

!ENDIF 

SOURCE=..\src\mutex.cpp
DEP_CPP_MUTEX=\
	"..\include\mutex.h"\
	

"$(INTDIR)\mutex.obj" : $(SOURCE) $(DEP_CPP_MUTEX) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\preferences.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_PREFE=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\include\preferences.h"\
	

"$(INTDIR)\preferences.obj" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_PREFE=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\include\preferences.h"\
	

"$(INTDIR)\preferences.obj" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\semaphore.cpp
DEP_CPP_SEMAP=\
	"..\include\semaphore.h"\
	

"$(INTDIR)\semaphore.obj" : $(SOURCE) $(DEP_CPP_SEMAP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\win32thread.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_WIN32=\
	"..\..\..\config\config.h"\
	"..\..\include\thread.h"\
	"..\include\win32thread.h"\
	

"$(INTDIR)\win32thread.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_WIN32=\
	"..\..\..\config\config.h"\
	"..\..\include\thread.h"\
	"..\include\win32thread.h"\
	

"$(INTDIR)\win32thread.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\lmc\src\lmcregistry.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_LMCRE=\
	"..\..\..\config\config.h"\
	"..\..\..\lmc\include\lmcregistry.h"\
	"..\..\include\registry.h"\
	"..\..\include\vector.h"\
	

"$(INTDIR)\lmcregistry.obj" : $(SOURCE) $(DEP_CPP_LMCRE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_LMCRE=\
	"..\..\..\config\config.h"\
	"..\..\..\lmc\include\lmcregistry.h"\
	"..\..\include\registry.h"\
	"..\..\include\vector.h"\
	

"$(INTDIR)\lmcregistry.obj" : $(SOURCE) $(DEP_CPP_LMCRE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\io\src\pmiregistry.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_PMIRE=\
	"..\..\..\config\config.h"\
	"..\..\..\io\include\pmiregistry.h"\
	"..\..\include\registry.h"\
	"..\..\include\vector.h"\
	

"$(INTDIR)\pmiregistry.obj" : $(SOURCE) $(DEP_CPP_PMIRE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_PMIRE=\
	"..\..\..\config\config.h"\
	"..\..\..\io\include\pmiregistry.h"\
	"..\..\include\registry.h"\
	"..\..\include\vector.h"\
	

"$(INTDIR)\pmiregistry.obj" : $(SOURCE) $(DEP_CPP_PMIRE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\io\src\pmoregistry.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_PMORE=\
	"..\..\..\config\config.h"\
	"..\..\..\io\include\pmoregistry.h"\
	"..\..\include\registry.h"\
	"..\..\include\vector.h"\
	

"$(INTDIR)\pmoregistry.obj" : $(SOURCE) $(DEP_CPP_PMORE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_PMORE=\
	"..\..\..\config\config.h"\
	"..\..\..\io\include\pmoregistry.h"\
	"..\..\include\registry.h"\
	"..\..\include\vector.h"\
	

"$(INTDIR)\pmoregistry.obj" : $(SOURCE) $(DEP_CPP_PMORE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\registry.cpp
DEP_CPP_REGIST=\
	"..\..\..\config\config.h"\
	"..\..\include\registry.h"\
	"..\..\include\vector.h"\
	

"$(INTDIR)\registry.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\ui\src\uiregistry.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_UIREG=\
	"..\..\..\config\config.h"\
	"..\..\..\ui\include\uiregistry.h"\
	"..\..\include\registry.h"\
	"..\..\include\vector.h"\
	

"$(INTDIR)\uiregistry.obj" : $(SOURCE) $(DEP_CPP_UIREG) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_UIREG=\
	"..\..\..\config\config.h"\
	"..\..\..\ui\include\uiregistry.h"\
	"..\..\include\registry.h"\
	"..\..\include\vector.h"\
	

"$(INTDIR)\uiregistry.obj" : $(SOURCE) $(DEP_CPP_UIREG) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"fileinput - Win32 Release" : 
   cd "\Local\src\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"fileinput - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fileinput.mak\
 CFG="fileinput - Win32 Release" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"fileinput - Win32 Debug" : 
   cd "\Local\src\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"fileinput - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fileinput.mak CFG="fileinput - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"soundcard - Win32 Release" : 
   cd "\Local\src\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"soundcard - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\soundcard.mak\
 CFG="soundcard - Win32 Release" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"soundcard - Win32 Debug" : 
   cd "\Local\src\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"soundcard - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\soundcard.mak CFG="soundcard - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"xing - Win32 Release" : 
   cd "\Local\src\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"xing - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\xing.mak CFG="xing - Win32 Release"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"xing - Win32 Debug" : 
   cd "\Local\src\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"xing - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\xing.mak CFG="xing - Win32 Debug" RECURSE=1\
 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"simple - Win32 Release" : 
   cd "\Local\src\freeamp\ui\simple\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\simple.mak CFG="simple - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"simple - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\ui\simple\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\simple.mak CFG="simple - Win32 Release"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"simple - Win32 Debug" : 
   cd "\Local\src\freeamp\ui\simple\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\simple.mak CFG="simple - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"simple - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\ui\simple\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\simple.mak CFG="simple - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"freeampui - Win32 Release" : 
   cd "\Local\src\freeamp\ui\freeamp\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\freeampui.mak CFG="freeampui - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"freeampui - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\ui\freeamp\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\freeampui.mak\
 CFG="freeampui - Win32 Release" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"freeampui - Win32 Debug" : 
   cd "\Local\src\freeamp\ui\freeamp\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\freeampui.mak CFG="freeampui - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"freeampui - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\ui\freeamp\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\freeampui.mak CFG="freeampui - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 


!ENDIF 

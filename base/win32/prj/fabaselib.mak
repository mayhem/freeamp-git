# Microsoft Developer Studio Generated NMAKE File, Based on fabaselib.dsp
!IF "$(CFG)" == ""
CFG=fabaselib - Win32 NASM Debug
!MESSAGE No configuration specified. Defaulting to fabaselib - Win32 NASM\
 Debug.
!ENDIF 

!IF "$(CFG)" != "fabaselib - Win32 Release" && "$(CFG)" !=\
 "fabaselib - Win32 Debug" && "$(CFG)" != "fabaselib - Win32 NASM Debug" &&\
 "$(CFG)" != "fabaselib - Win32 NASM Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fabaselib.mak" CFG="fabaselib - Win32 NASM Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fabaselib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "fabaselib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "fabaselib - Win32 NASM Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "fabaselib - Win32 NASM Release" (based on\
 "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe

!IF  "$(CFG)" == "fabaselib - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\fabaselib.lib"

!ELSE 

ALL : "..\fabaselib.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\database.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\downloadmanager.obj"
	-@erase "$(INTDIR)\errors.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\musiccatalog.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\player.obj"
	-@erase "$(INTDIR)\playlist.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\propimpl.obj"
	-@erase "$(INTDIR)\registrar.obj"
	-@erase "$(INTDIR)\registry.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\updatemanager.obj"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\win32prefs.obj"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(INTDIR)\win32updatemanager.obj"
	-@erase "..\fabaselib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\..\lib\zlib\include" /I\
 "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I\
 "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I\
 "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include"\
 /I "..\..\..\lmc\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\fabaselib.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\fabaselib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\fabaselib.lib" 
LIB32_OBJS= \
	"$(INTDIR)\database.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\downloadmanager.obj" \
	"$(INTDIR)\errors.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\musiccatalog.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\player.obj" \
	"$(INTDIR)\playlist.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\propimpl.obj" \
	"$(INTDIR)\registrar.obj" \
	"$(INTDIR)\registry.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\updatemanager.obj" \
	"$(INTDIR)\utility.obj" \
	"$(INTDIR)\win32prefs.obj" \
	"$(INTDIR)\win32thread.obj" \
	"$(INTDIR)\win32updatemanager.obj"

"..\fabaselib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\fabaselib.lib"

!ELSE 

ALL : "..\fabaselib.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\database.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\downloadmanager.obj"
	-@erase "$(INTDIR)\errors.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\musiccatalog.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\player.obj"
	-@erase "$(INTDIR)\playlist.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\propimpl.obj"
	-@erase "$(INTDIR)\registrar.obj"
	-@erase "$(INTDIR)\registry.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\updatemanager.obj"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\win32prefs.obj"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(INTDIR)\win32updatemanager.obj"
	-@erase "..\fabaselib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /GX /Z7 /Od /I "..\..\..\lib\zlib\include" /I\
 "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I\
 "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I\
 "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include"\
 /I "..\..\..\lmc\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\fabaselib.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\fabaselib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\fabaselib.lib" 
LIB32_OBJS= \
	"$(INTDIR)\database.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\downloadmanager.obj" \
	"$(INTDIR)\errors.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\musiccatalog.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\player.obj" \
	"$(INTDIR)\playlist.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\propimpl.obj" \
	"$(INTDIR)\registrar.obj" \
	"$(INTDIR)\registry.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\updatemanager.obj" \
	"$(INTDIR)\utility.obj" \
	"$(INTDIR)\win32prefs.obj" \
	"$(INTDIR)\win32thread.obj" \
	"$(INTDIR)\win32updatemanager.obj"

"..\fabaselib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\fabaselib.lib"

!ELSE 

ALL : "..\fabaselib.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\database.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\downloadmanager.obj"
	-@erase "$(INTDIR)\errors.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\musiccatalog.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\player.obj"
	-@erase "$(INTDIR)\playlist.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\propimpl.obj"
	-@erase "$(INTDIR)\registrar.obj"
	-@erase "$(INTDIR)\registry.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\updatemanager.obj"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\win32prefs.obj"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(INTDIR)\win32updatemanager.obj"
	-@erase "..\fabaselib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /GX /Z7 /Od /I "..\..\..\lib\zlib\include" /I\
 "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I\
 "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I\
 "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include"\
 /I "..\..\..\lmc\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\fabaselib.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\fabaselib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\fabaselib.lib" 
LIB32_OBJS= \
	"$(INTDIR)\database.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\downloadmanager.obj" \
	"$(INTDIR)\errors.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\musiccatalog.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\player.obj" \
	"$(INTDIR)\playlist.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\propimpl.obj" \
	"$(INTDIR)\registrar.obj" \
	"$(INTDIR)\registry.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\updatemanager.obj" \
	"$(INTDIR)\utility.obj" \
	"$(INTDIR)\win32prefs.obj" \
	"$(INTDIR)\win32thread.obj" \
	"$(INTDIR)\win32updatemanager.obj"

"..\fabaselib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\fabaselib.lib"

!ELSE 

ALL : "..\fabaselib.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\database.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\downloadmanager.obj"
	-@erase "$(INTDIR)\errors.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\musiccatalog.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\player.obj"
	-@erase "$(INTDIR)\playlist.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\propimpl.obj"
	-@erase "$(INTDIR)\registrar.obj"
	-@erase "$(INTDIR)\registry.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\updatemanager.obj"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\win32prefs.obj"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(INTDIR)\win32updatemanager.obj"
	-@erase "..\fabaselib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\..\lib\zlib\include" /I\
 "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I\
 "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I\
 "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include"\
 /I "..\..\..\lmc\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\fabaselib.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\fabaselib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\fabaselib.lib" 
LIB32_OBJS= \
	"$(INTDIR)\database.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\downloadmanager.obj" \
	"$(INTDIR)\errors.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\musiccatalog.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\player.obj" \
	"$(INTDIR)\playlist.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\propimpl.obj" \
	"$(INTDIR)\registrar.obj" \
	"$(INTDIR)\registry.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\updatemanager.obj" \
	"$(INTDIR)\utility.obj" \
	"$(INTDIR)\win32prefs.obj" \
	"$(INTDIR)\win32thread.obj" \
	"$(INTDIR)\win32updatemanager.obj"

"..\fabaselib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

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


!IF "$(CFG)" == "fabaselib - Win32 Release" || "$(CFG)" ==\
 "fabaselib - Win32 Debug" || "$(CFG)" == "fabaselib - Win32 NASM Debug" ||\
 "$(CFG)" == "fabaselib - Win32 NASM Release"
SOURCE=..\..\..\config\config.win32

!IF  "$(CFG)" == "fabaselib - Win32 Release"

InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h

!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"

InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h

!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"

InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h

!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h

!ENDIF 

SOURCE=..\..\src\database.cpp

!IF  "$(CFG)" == "fabaselib - Win32 Release"

DEP_CPP_DATAB=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\include\database.h"\
	"..\..\include\errors.h"\
	"..\include\mutex.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\database.obj" : $(SOURCE) $(DEP_CPP_DATAB) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"

DEP_CPP_DATAB=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\include\database.h"\
	"..\..\include\errors.h"\
	"..\include\mutex.h"\
	

"$(INTDIR)\database.obj" : $(SOURCE) $(DEP_CPP_DATAB) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"

DEP_CPP_DATAB=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\include\database.h"\
	"..\..\include\errors.h"\
	"..\include\mutex.h"\
	

"$(INTDIR)\database.obj" : $(SOURCE) $(DEP_CPP_DATAB) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

DEP_CPP_DATAB=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\include\database.h"\
	"..\..\include\errors.h"\
	"..\include\mutex.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\database.obj" : $(SOURCE) $(DEP_CPP_DATAB) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\debug.cpp

!IF  "$(CFG)" == "fabaselib - Win32 Release"

DEP_CPP_DEBUG=\
	"..\..\include\debug.h"\
	

"$(INTDIR)\debug.obj" : $(SOURCE) $(DEP_CPP_DEBUG) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"


"$(INTDIR)\debug.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"


"$(INTDIR)\debug.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

DEP_CPP_DEBUG=\
	"..\..\include\debug.h"\
	

"$(INTDIR)\debug.obj" : $(SOURCE) $(DEP_CPP_DEBUG) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\downloadmanager.cpp

!IF  "$(CFG)" == "fabaselib - Win32 Release"

DEP_CPP_DOWNL=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\include\database.h"\
	"..\..\include\downloadformat.h"\
	"..\..\include\downloadmanager.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\musicbrowser.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	"..\include\semaphore.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	
NODEP_CPP_DOWNL=\
	"..\..\include\win32impl.h"\
	

"$(INTDIR)\downloadmanager.obj" : $(SOURCE) $(DEP_CPP_DOWNL) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"

DEP_CPP_DOWNL=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\include\database.h"\
	"..\..\include\downloadformat.h"\
	"..\..\include\downloadmanager.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\musicbrowser.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	"..\include\semaphore.h"\
	

"$(INTDIR)\downloadmanager.obj" : $(SOURCE) $(DEP_CPP_DOWNL) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"

DEP_CPP_DOWNL=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\include\database.h"\
	"..\..\include\downloadformat.h"\
	"..\..\include\downloadmanager.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\musicbrowser.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	"..\include\semaphore.h"\
	

"$(INTDIR)\downloadmanager.obj" : $(SOURCE) $(DEP_CPP_DOWNL) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

DEP_CPP_DOWNL=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\include\database.h"\
	"..\..\include\downloadformat.h"\
	"..\..\include\downloadmanager.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\musicbrowser.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	"..\include\semaphore.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	
NODEP_CPP_DOWNL=\
	"..\..\include\win32impl.h"\
	

"$(INTDIR)\downloadmanager.obj" : $(SOURCE) $(DEP_CPP_DOWNL) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\errors.cpp
DEP_CPP_ERROR=\
	"..\..\..\config\config.h"\
	

"$(INTDIR)\errors.obj" : $(SOURCE) $(DEP_CPP_ERROR) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\log.cpp

!IF  "$(CFG)" == "fabaselib - Win32 Release"

DEP_CPP_LOG_C=\
	"..\..\include\log.h"\
	

"$(INTDIR)\log.obj" : $(SOURCE) $(DEP_CPP_LOG_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"


"$(INTDIR)\log.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"


"$(INTDIR)\log.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

DEP_CPP_LOG_C=\
	"..\..\include\log.h"\
	

"$(INTDIR)\log.obj" : $(SOURCE) $(DEP_CPP_LOG_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\musiccatalog.cpp

!IF  "$(CFG)" == "fabaselib - Win32 Release"

DEP_CPP_MUSIC=\
	"..\..\..\config\config.h"\
	"..\..\..\io\include\pipeline.h"\
	"..\..\..\io\include\pullbuffer.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\lmc\include\lmc.h"\
	"..\..\..\ui\include\ui.h"\
	"..\..\include\database.h"\
	"..\..\include\debug.h"\
	"..\..\include\downloadformat.h"\
	"..\..\include\downloadmanager.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\musicbrowser.h"\
	"..\..\include\player.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\properties.h"\
	"..\..\include\propimpl.h"\
	"..\..\include\queue.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	"..\include\semaphore.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	
NODEP_CPP_MUSIC=\
	"..\..\src\win32impl.h"\
	

"$(INTDIR)\musiccatalog.obj" : $(SOURCE) $(DEP_CPP_MUSIC) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"

DEP_CPP_MUSIC=\
	"..\..\..\config\config.h"\
	"..\..\..\io\include\pipeline.h"\
	"..\..\..\io\include\pullbuffer.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\lmc\include\lmc.h"\
	"..\..\..\ui\include\ui.h"\
	"..\..\include\database.h"\
	"..\..\include\debug.h"\
	"..\..\include\downloadformat.h"\
	"..\..\include\downloadmanager.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\musicbrowser.h"\
	"..\..\include\player.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\properties.h"\
	"..\..\include\propimpl.h"\
	"..\..\include\queue.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	"..\include\semaphore.h"\
	

"$(INTDIR)\musiccatalog.obj" : $(SOURCE) $(DEP_CPP_MUSIC) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"

DEP_CPP_MUSIC=\
	"..\..\..\config\config.h"\
	"..\..\..\io\include\pipeline.h"\
	"..\..\..\io\include\pullbuffer.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\lmc\include\lmc.h"\
	"..\..\..\ui\include\ui.h"\
	"..\..\include\database.h"\
	"..\..\include\debug.h"\
	"..\..\include\downloadformat.h"\
	"..\..\include\downloadmanager.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\musicbrowser.h"\
	"..\..\include\player.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\properties.h"\
	"..\..\include\propimpl.h"\
	"..\..\include\queue.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	"..\include\semaphore.h"\
	

"$(INTDIR)\musiccatalog.obj" : $(SOURCE) $(DEP_CPP_MUSIC) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

DEP_CPP_MUSIC=\
	"..\..\..\config\config.h"\
	"..\..\..\io\include\pipeline.h"\
	"..\..\..\io\include\pullbuffer.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\lmc\include\lmc.h"\
	"..\..\..\ui\include\ui.h"\
	"..\..\include\database.h"\
	"..\..\include\debug.h"\
	"..\..\include\downloadformat.h"\
	"..\..\include\downloadmanager.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\musicbrowser.h"\
	"..\..\include\player.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\properties.h"\
	"..\..\include\propimpl.h"\
	"..\..\include\queue.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	"..\include\semaphore.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	
NODEP_CPP_MUSIC=\
	"..\..\src\win32impl.h"\
	

"$(INTDIR)\musiccatalog.obj" : $(SOURCE) $(DEP_CPP_MUSIC) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\mutex.cpp

!IF  "$(CFG)" == "fabaselib - Win32 Release"

DEP_CPP_MUTEX=\
	"..\include\mutex.h"\
	

"$(INTDIR)\mutex.obj" : $(SOURCE) $(DEP_CPP_MUTEX) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"


"$(INTDIR)\mutex.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"


"$(INTDIR)\mutex.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

DEP_CPP_MUTEX=\
	"..\include\mutex.h"\
	

"$(INTDIR)\mutex.obj" : $(SOURCE) $(DEP_CPP_MUTEX) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\player.cpp

!IF  "$(CFG)" == "fabaselib - Win32 Release"

DEP_CPP_PLAYE=\
	"..\..\..\config\config.h"\
	"..\..\..\io\include\pipeline.h"\
	"..\..\..\io\include\pmi.h"\
	"..\..\..\io\include\pmo.h"\
	"..\..\..\io\include\pullbuffer.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\lmc\include\lmc.h"\
	"..\..\..\ui\include\ui.h"\
	"..\..\include\database.h"\
	"..\..\include\debug.h"\
	"..\..\include\downloadformat.h"\
	"..\..\include\downloadmanager.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\musicbrowser.h"\
	"..\..\include\player.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\properties.h"\
	"..\..\include\propimpl.h"\
	"..\..\include\queue.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	"..\include\semaphore.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	
NODEP_CPP_PLAYE=\
	"..\..\include\win32impl.h"\
	

"$(INTDIR)\player.obj" : $(SOURCE) $(DEP_CPP_PLAYE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"

DEP_CPP_PLAYE=\
	"..\..\..\config\config.h"\
	"..\..\..\io\include\pipeline.h"\
	"..\..\..\io\include\pmi.h"\
	"..\..\..\io\include\pmo.h"\
	"..\..\..\io\include\pullbuffer.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\lmc\include\lmc.h"\
	"..\..\..\ui\include\ui.h"\
	"..\..\include\database.h"\
	"..\..\include\debug.h"\
	"..\..\include\downloadformat.h"\
	"..\..\include\downloadmanager.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\musicbrowser.h"\
	"..\..\include\player.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\properties.h"\
	"..\..\include\propimpl.h"\
	"..\..\include\queue.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	"..\include\semaphore.h"\
	

"$(INTDIR)\player.obj" : $(SOURCE) $(DEP_CPP_PLAYE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"

DEP_CPP_PLAYE=\
	"..\..\..\config\config.h"\
	"..\..\..\io\include\pipeline.h"\
	"..\..\..\io\include\pmi.h"\
	"..\..\..\io\include\pmo.h"\
	"..\..\..\io\include\pullbuffer.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\lmc\include\lmc.h"\
	"..\..\..\ui\include\ui.h"\
	"..\..\include\database.h"\
	"..\..\include\debug.h"\
	"..\..\include\downloadformat.h"\
	"..\..\include\downloadmanager.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\musicbrowser.h"\
	"..\..\include\player.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\properties.h"\
	"..\..\include\propimpl.h"\
	"..\..\include\queue.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	"..\include\semaphore.h"\
	

"$(INTDIR)\player.obj" : $(SOURCE) $(DEP_CPP_PLAYE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

DEP_CPP_PLAYE=\
	"..\..\..\config\config.h"\
	"..\..\..\io\include\pipeline.h"\
	"..\..\..\io\include\pmi.h"\
	"..\..\..\io\include\pmo.h"\
	"..\..\..\io\include\pullbuffer.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\lmc\include\lmc.h"\
	"..\..\..\ui\include\ui.h"\
	"..\..\include\database.h"\
	"..\..\include\debug.h"\
	"..\..\include\downloadformat.h"\
	"..\..\include\downloadmanager.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\musicbrowser.h"\
	"..\..\include\player.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\properties.h"\
	"..\..\include\propimpl.h"\
	"..\..\include\queue.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	"..\include\semaphore.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	
NODEP_CPP_PLAYE=\
	"..\..\include\win32impl.h"\
	

"$(INTDIR)\player.obj" : $(SOURCE) $(DEP_CPP_PLAYE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\playlist.cpp

!IF  "$(CFG)" == "fabaselib - Win32 Release"

DEP_CPP_PLAYL=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\include\database.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\musicbrowser.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	
NODEP_CPP_PLAYL=\
	"..\..\include\win32impl.h"\
	

"$(INTDIR)\playlist.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"

DEP_CPP_PLAYL=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\include\database.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\musicbrowser.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	

"$(INTDIR)\playlist.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"

DEP_CPP_PLAYL=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\include\database.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\musicbrowser.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	

"$(INTDIR)\playlist.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

DEP_CPP_PLAYL=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\include\database.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\musicbrowser.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	
NODEP_CPP_PLAYL=\
	"..\..\include\win32impl.h"\
	

"$(INTDIR)\playlist.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\preferences.cpp

!IF  "$(CFG)" == "fabaselib - Win32 Release"

DEP_CPP_PREFE=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	

"$(INTDIR)\preferences.obj" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"

DEP_CPP_PREFE=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	

"$(INTDIR)\preferences.obj" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"

DEP_CPP_PREFE=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	

"$(INTDIR)\preferences.obj" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

DEP_CPP_PREFE=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	

"$(INTDIR)\preferences.obj" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\propimpl.cpp

!IF  "$(CFG)" == "fabaselib - Win32 Release"

DEP_CPP_PROPI=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\properties.h"\
	"..\..\include\propimpl.h"\
	"..\include\mutex.h"\
	

"$(INTDIR)\propimpl.obj" : $(SOURCE) $(DEP_CPP_PROPI) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"

DEP_CPP_PROPI=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\properties.h"\
	"..\..\include\propimpl.h"\
	"..\include\mutex.h"\
	

"$(INTDIR)\propimpl.obj" : $(SOURCE) $(DEP_CPP_PROPI) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"

DEP_CPP_PROPI=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\properties.h"\
	"..\..\include\propimpl.h"\
	"..\include\mutex.h"\
	

"$(INTDIR)\propimpl.obj" : $(SOURCE) $(DEP_CPP_PROPI) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

DEP_CPP_PROPI=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\properties.h"\
	"..\..\include\propimpl.h"\
	"..\include\mutex.h"\
	

"$(INTDIR)\propimpl.obj" : $(SOURCE) $(DEP_CPP_PROPI) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\registrar.cpp

!IF  "$(CFG)" == "fabaselib - Win32 Release"

DEP_CPP_REGIS=\
	"..\..\..\config\config.h"\
	"..\..\include\debug.h"\
	"..\..\include\errors.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	
NODEP_CPP_REGIS=\
	"..\..\include\win32impl.h"\
	

"$(INTDIR)\registrar.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"

DEP_CPP_REGIS=\
	"..\..\..\config\config.h"\
	"..\..\include\debug.h"\
	"..\..\include\errors.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	

"$(INTDIR)\registrar.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"

DEP_CPP_REGIS=\
	"..\..\..\config\config.h"\
	"..\..\include\debug.h"\
	"..\..\include\errors.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	

"$(INTDIR)\registrar.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

DEP_CPP_REGIS=\
	"..\..\..\config\config.h"\
	"..\..\include\debug.h"\
	"..\..\include\errors.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	
NODEP_CPP_REGIS=\
	"..\..\include\win32impl.h"\
	

"$(INTDIR)\registrar.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\registry.cpp

!IF  "$(CFG)" == "fabaselib - Win32 Release"

DEP_CPP_REGIST=\
	"..\..\..\config\config.h"\
	"..\..\include\registry.h"\
	

"$(INTDIR)\registry.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"


"$(INTDIR)\registry.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"


"$(INTDIR)\registry.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

DEP_CPP_REGIST=\
	"..\..\..\config\config.h"\
	"..\..\include\registry.h"\
	

"$(INTDIR)\registry.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\semaphore.cpp

!IF  "$(CFG)" == "fabaselib - Win32 Release"

DEP_CPP_SEMAP=\
	"..\include\semaphore.h"\
	

"$(INTDIR)\semaphore.obj" : $(SOURCE) $(DEP_CPP_SEMAP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"


"$(INTDIR)\semaphore.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"


"$(INTDIR)\semaphore.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

DEP_CPP_SEMAP=\
	"..\include\semaphore.h"\
	

"$(INTDIR)\semaphore.obj" : $(SOURCE) $(DEP_CPP_SEMAP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\thread.cpp

!IF  "$(CFG)" == "fabaselib - Win32 Release"

DEP_CPP_THREA=\
	"..\..\..\config\config.h"\
	"..\..\include\thread.h"\
	"..\include\win32thread.h"\
	
NODEP_CPP_THREA=\
	"..\..\src\beosthread.h"\
	"..\..\src\linuxthread.h"\
	"..\..\src\solaristhread.h"\
	

"$(INTDIR)\thread.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"


"$(INTDIR)\thread.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"


"$(INTDIR)\thread.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

DEP_CPP_THREA=\
	"..\..\..\config\config.h"\
	"..\..\include\thread.h"\
	"..\include\win32thread.h"\
	
NODEP_CPP_THREA=\
	"..\..\src\beosthread.h"\
	"..\..\src\linuxthread.h"\
	"..\..\src\solaristhread.h"\
	

"$(INTDIR)\thread.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\updatemanager.cpp

!IF  "$(CFG)" == "fabaselib - Win32 Release"

DEP_CPP_UPDAT=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\xml\include\Parse.h"\
	"..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\lib\zlib\include\zlib.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\updatemanager.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	"..\include\semaphore.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\updatemanager.obj" : $(SOURCE) $(DEP_CPP_UPDAT) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"

DEP_CPP_UPDAT=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\xml\include\Parse.h"\
	"..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\lib\zlib\include\zlib.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\updatemanager.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	"..\include\semaphore.h"\
	

"$(INTDIR)\updatemanager.obj" : $(SOURCE) $(DEP_CPP_UPDAT) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"

DEP_CPP_UPDAT=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\xml\include\Parse.h"\
	"..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\lib\zlib\include\zlib.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\updatemanager.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	"..\include\semaphore.h"\
	

"$(INTDIR)\updatemanager.obj" : $(SOURCE) $(DEP_CPP_UPDAT) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

DEP_CPP_UPDAT=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\xml\include\Parse.h"\
	"..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\lib\zlib\include\zlib.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\updatemanager.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	"..\include\semaphore.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\updatemanager.obj" : $(SOURCE) $(DEP_CPP_UPDAT) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\utility.cpp

!IF  "$(CFG)" == "fabaselib - Win32 Release"

DEP_CPP_UTILI=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	"..\..\include\utility.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"

DEP_CPP_UTILI=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	"..\..\include\utility.h"\
	

"$(INTDIR)\utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"

DEP_CPP_UTILI=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	"..\..\include\utility.h"\
	

"$(INTDIR)\utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

DEP_CPP_UTILI=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	"..\..\include\utility.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\win32prefs.cpp

!IF  "$(CFG)" == "fabaselib - Win32 Release"

DEP_CPP_WIN32=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	"..\include\win32prefs.h"\
	

"$(INTDIR)\win32prefs.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"

DEP_CPP_WIN32=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	"..\include\win32prefs.h"\
	

"$(INTDIR)\win32prefs.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"

DEP_CPP_WIN32=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	"..\include\win32prefs.h"\
	

"$(INTDIR)\win32prefs.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

DEP_CPP_WIN32=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	"..\include\win32prefs.h"\
	

"$(INTDIR)\win32prefs.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\win32thread.cpp

!IF  "$(CFG)" == "fabaselib - Win32 Release"

DEP_CPP_WIN32T=\
	"..\..\..\config\config.h"\
	"..\..\include\thread.h"\
	"..\include\win32thread.h"\
	

"$(INTDIR)\win32thread.obj" : $(SOURCE) $(DEP_CPP_WIN32T) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"


"$(INTDIR)\win32thread.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"


"$(INTDIR)\win32thread.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

DEP_CPP_WIN32T=\
	"..\..\..\config\config.h"\
	"..\..\include\thread.h"\
	"..\include\win32thread.h"\
	

"$(INTDIR)\win32thread.obj" : $(SOURCE) $(DEP_CPP_WIN32T) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\win32updatemanager.cpp

!IF  "$(CFG)" == "fabaselib - Win32 Release"

DEP_CPP_WIN32U=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\xml\include\Parse.h"\
	"..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\lib\zlib\include\zlib.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\updatemanager.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	"..\include\semaphore.h"\
	"..\include\win32updatemanager.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\win32updatemanager.obj" : $(SOURCE) $(DEP_CPP_WIN32U) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"

DEP_CPP_WIN32U=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\xml\include\Parse.h"\
	"..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\lib\zlib\include\zlib.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\updatemanager.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	"..\include\semaphore.h"\
	"..\include\win32updatemanager.h"\
	

"$(INTDIR)\win32updatemanager.obj" : $(SOURCE) $(DEP_CPP_WIN32U) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"

DEP_CPP_WIN32U=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\xml\include\Parse.h"\
	"..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\lib\zlib\include\zlib.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\updatemanager.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	"..\include\semaphore.h"\
	"..\include\win32updatemanager.h"\
	

"$(INTDIR)\win32updatemanager.obj" : $(SOURCE) $(DEP_CPP_WIN32U) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

DEP_CPP_WIN32U=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\xml\include\Parse.h"\
	"..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\lib\zlib\include\zlib.h"\
	"..\..\include\errors.h"\
	"..\..\include\event.h"\
	"..\..\include\eventdata.h"\
	"..\..\include\facontext.h"\
	"..\..\include\log.h"\
	"..\..\include\metadata.h"\
	"..\..\include\playlist.h"\
	"..\..\include\playlistformat.h"\
	"..\..\include\plmevent.h"\
	"..\..\include\portabledevice.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\updatemanager.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	"..\include\semaphore.h"\
	"..\include\win32updatemanager.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\win32updatemanager.obj" : $(SOURCE) $(DEP_CPP_WIN32U) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 


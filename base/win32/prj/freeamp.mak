# Microsoft Developer Studio Generated NMAKE File, Based on freeamp.dsp
!IF "$(CFG)" == ""
CFG=freeamp - Win32 NASM Debug
!MESSAGE No configuration specified. Defaulting to freeamp - Win32 NASM Debug.
!ENDIF 

!IF "$(CFG)" != "freeamp - Win32 Release" && "$(CFG)" !=\
 "freeamp - Win32 Debug" && "$(CFG)" != "freeamp - Win32 NASM Release" &&\
 "$(CFG)" != "freeamp - Win32 NASM Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "freeamp.mak" CFG="freeamp - Win32 NASM Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "freeamp - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "freeamp - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "freeamp - Win32 NASM Release" (based on "Win32 (x86) Application")
!MESSAGE "freeamp - Win32 NASM Debug" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "freeamp - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\portio.sys" "..\..\..\ReadMe.txt" "..\..\..\Copying.txt"\
 "..\..\..\config\config.h" "..\..\..\Changes.txt" "..\..\..\Authors.txt"\
 ".\freeamp.exe"

!ELSE 

ALL : "musicbrowser - Win32 Release" "pmp300 - Win32 Release"\
 "zlib - Win32 Release" "xing - Win32 Release" "update - Win32 Release"\
 "soundcard - Win32 Release" "rmp - Win32 Release" "obsinput - Win32 Release"\
 "m3u - Win32 Release" "id3v2 - Win32 Release" "id3v1 - Win32 Release"\
 "httpinput - Win32 Release" "gdbm - Win32 Release" "freeampui - Win32 Release"\
 "fileinput - Win32 Release" "dsoundcard - Win32 Release"\
 "download - Win32 Release" ".\portio.sys" "..\..\..\ReadMe.txt"\
 "..\..\..\Copying.txt" "..\..\..\config\config.h" "..\..\..\Changes.txt"\
 "..\..\..\Authors.txt" ".\freeamp.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"download - Win32 ReleaseCLEAN" "dsoundcard - Win32 ReleaseCLEAN"\
 "fileinput - Win32 ReleaseCLEAN" "freeampui - Win32 ReleaseCLEAN"\
 "gdbm - Win32 ReleaseCLEAN" "httpinput - Win32 ReleaseCLEAN"\
 "id3v1 - Win32 ReleaseCLEAN" "id3v2 - Win32 ReleaseCLEAN"\
 "m3u - Win32 ReleaseCLEAN" "obsinput - Win32 ReleaseCLEAN"\
 "rmp - Win32 ReleaseCLEAN" "soundcard - Win32 ReleaseCLEAN"\
 "update - Win32 ReleaseCLEAN" "xing - Win32 ReleaseCLEAN"\
 "zlib - Win32 ReleaseCLEAN" "pmp300 - Win32 ReleaseCLEAN"\
 "musicbrowser - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\database.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\downloadmanager.obj"
	-@erase "$(INTDIR)\freeamp.res"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\musiccatalog.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\player.obj"
	-@erase "$(INTDIR)\playlist.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\propimpl.obj"
	-@erase "$(INTDIR)\registrar.obj"
	-@erase "$(INTDIR)\registry.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\win32prefs.obj"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase ".\freeamp.exe"
	-@erase "..\..\..\Authors.txt"
	-@erase "..\..\..\Changes.txt"
	-@erase "..\..\..\config\config.h"
	-@erase "..\..\..\Copying.txt"
	-@erase "..\..\..\ReadMe.txt"
	-@erase ".\portio.sys"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\..\..\lib\zlib\include" /I\
 "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I\
 "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I\
 "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include"\
 /I "..\..\..\lmc\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\freeamp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\freeamp.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\freeamp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=zlib.lib gdbm.lib version.lib winmm.lib wsock32.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo\
 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\freeamp.pdb" /machine:I386\
 /out:"freeamp.exe" /libpath:"..\..\..\lib\gdbm" /libpath:"..\..\..\lib\zlib" 
LINK32_OBJS= \
	"$(INTDIR)\database.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\downloadmanager.obj" \
	"$(INTDIR)\freeamp.res" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\musiccatalog.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\Parse.obj" \
	"$(INTDIR)\player.obj" \
	"$(INTDIR)\playlist.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\propimpl.obj" \
	"$(INTDIR)\registrar.obj" \
	"$(INTDIR)\registry.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\utility.obj" \
	"$(INTDIR)\win32prefs.obj" \
	"$(INTDIR)\win32thread.obj" \
	"..\..\..\dlm\rmp\prj\Release\rmp.lib" \
	"..\..\..\io\dsound\win32\prj\Release\dsoundcard.lib" \
	"..\..\..\io\http\win32\prj\Release\httpinput.lib" \
	"..\..\..\io\local\win32\prj\Release\fileinput.lib" \
	"..\..\..\io\obs\win32\prj\Release\obsinput.lib" \
	"..\..\..\io\soundcard\win32\prj\Release\soundcard.lib" \
	"..\..\..\lib\gdbm\gdbm.lib" \
	"..\..\..\lib\zlib\zlib.lib" \
	"..\..\..\lmc\xingmp3\win32\prj\Release\xing.lib" \
	"..\..\..\plm\metadata\id3v1\prj\Release\id3v1.lib" \
	"..\..\..\plm\metadata\id3v2\prj\Release\id3v2.lib" \
	"..\..\..\plm\playlist\m3u\prj\Release\m3u.lib" \
	"..\..\..\plm\portable\pmp300\prj\Release\pmp300.lib" \
	"..\..\..\ui\download\win32\prj\Release\download.lib" \
	"..\..\..\ui\freeamp\win32\prj\Release\freeamp.lib" \
	"..\..\..\ui\musicbrowser\win32\prj\Release\musicbrowser.lib"

".\freeamp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "musicbrowser - Win32 Release" "pmp300 - Win32 Release"\
 "zlib - Win32 Release" "xing - Win32 Release" "update - Win32 Release"\
 "soundcard - Win32 Release" "rmp - Win32 Release" "obsinput - Win32 Release"\
 "m3u - Win32 Release" "id3v2 - Win32 Release" "id3v1 - Win32 Release"\
 "httpinput - Win32 Release" "gdbm - Win32 Release" "freeampui - Win32 Release"\
 "fileinput - Win32 Release" "dsoundcard - Win32 Release"\
 "download - Win32 Release" ".\portio.sys" "..\..\..\ReadMe.txt"\
 "..\..\..\Copying.txt" "..\..\..\config\config.h" "..\..\..\Changes.txt"\
 "..\..\..\Authors.txt" ".\freeamp.exe"
   IF NOT EXIST themes mkdir themes
	copy        ..\..\..\themes\freeamp.fat themes
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\portio.sys" "..\..\..\ReadMe.txt" "..\..\..\Copying.txt"\
 "..\..\..\Changes.txt" "..\..\..\Authors.txt" ".\freeamp.exe"

!ELSE 

ALL : "musicbrowser - Win32 Debug" "pmp300 - Win32 Debug" "zlib - Win32 Debug"\
 "xing - Win32 Debug" "update - Win32 Debug" "soundcard - Win32 Debug"\
 "rmp - Win32 Debug" "obsinput - Win32 Debug" "m3u - Win32 Debug"\
 "id3v2 - Win32 Debug" "id3v1 - Win32 Debug" "httpinput - Win32 Debug"\
 "gdbm - Win32 Debug" "freeampui - Win32 Debug" "fileinput - Win32 Debug"\
 "dsoundcard - Win32 Debug" "download - Win32 Debug" ".\portio.sys"\
 "..\..\..\ReadMe.txt" "..\..\..\Copying.txt" "..\..\..\Changes.txt"\
 "..\..\..\Authors.txt" ".\freeamp.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"download - Win32 DebugCLEAN" "dsoundcard - Win32 DebugCLEAN"\
 "fileinput - Win32 DebugCLEAN" "freeampui - Win32 DebugCLEAN"\
 "gdbm - Win32 DebugCLEAN" "httpinput - Win32 DebugCLEAN"\
 "id3v1 - Win32 DebugCLEAN" "id3v2 - Win32 DebugCLEAN" "m3u - Win32 DebugCLEAN"\
 "obsinput - Win32 DebugCLEAN" "rmp - Win32 DebugCLEAN"\
 "soundcard - Win32 DebugCLEAN" "update - Win32 DebugCLEAN"\
 "xing - Win32 DebugCLEAN" "zlib - Win32 DebugCLEAN" "pmp300 - Win32 DebugCLEAN"\
 "musicbrowser - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\database.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\downloadmanager.obj"
	-@erase "$(INTDIR)\freeamp.res"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\musiccatalog.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\player.obj"
	-@erase "$(INTDIR)\playlist.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\propimpl.obj"
	-@erase "$(INTDIR)\registrar.obj"
	-@erase "$(INTDIR)\registry.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\win32prefs.obj"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(OUTDIR)\freeamp.pdb"
	-@erase ".\freeamp.exe"
	-@erase ".\freeamp.ilk"
	-@erase "..\..\..\Authors.txt"
	-@erase "..\..\..\Changes.txt"
	-@erase "..\..\..\Copying.txt"
	-@erase "..\..\..\ReadMe.txt"
	-@erase ".\portio.sys"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\lib\zlib\include" /I\
 "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I\
 "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I\
 "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include"\
 /I "..\..\..\lmc\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\freeamp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\freeamp.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\freeamp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=zlib.lib gdbm.lib version.lib winmm.lib wsock32.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo\
 /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\freeamp.pdb" /debug\
 /machine:I386 /out:"freeamp.exe" /pdbtype:sept /libpath:"..\..\..\lib\gdbm"\
 /libpath:"..\..\..\lib\zlib" 
LINK32_OBJS= \
	"$(INTDIR)\database.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\downloadmanager.obj" \
	"$(INTDIR)\freeamp.res" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\musiccatalog.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\Parse.obj" \
	"$(INTDIR)\player.obj" \
	"$(INTDIR)\playlist.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\propimpl.obj" \
	"$(INTDIR)\registrar.obj" \
	"$(INTDIR)\registry.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\utility.obj" \
	"$(INTDIR)\win32prefs.obj" \
	"$(INTDIR)\win32thread.obj" \
	"..\..\..\dlm\rmp\prj\Debug\rmp.lib" \
	"..\..\..\io\dsound\win32\prj\Debug\dsoundcard.lib" \
	"..\..\..\io\http\win32\prj\Debug\httpinput.lib" \
	"..\..\..\io\local\win32\prj\Debug\fileinput.lib" \
	"..\..\..\io\obs\win32\prj\Debug\obsinput.lib" \
	"..\..\..\io\soundcard\win32\prj\Debug\soundcard.lib" \
	"..\..\..\lib\gdbm\gdbm.lib" \
	"..\..\..\lib\zlib\zlib.lib" \
	"..\..\..\lmc\xingmp3\win32\prj\Debug\xing.lib" \
	"..\..\..\plm\metadata\id3v1\prj\Debug\id3v1.lib" \
	"..\..\..\plm\metadata\id3v2\prj\Debug\id3v2.lib" \
	"..\..\..\plm\playlist\m3u\prj\Debug\m3u.lib" \
	"..\..\..\plm\portable\pmp300\prj\Debug\pmp300.lib" \
	"..\..\..\ui\download\win32\prj\Debug\download.lib" \
	"..\..\..\ui\freeamp\win32\prj\Debug\freeamp.lib" \
	"..\..\..\ui\musicbrowser\win32\prj\Debug\musicbrowser.lib"

".\freeamp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "musicbrowser - Win32 Debug" "pmp300 - Win32 Debug"\
 "zlib - Win32 Debug" "xing - Win32 Debug" "update - Win32 Debug"\
 "soundcard - Win32 Debug" "rmp - Win32 Debug" "obsinput - Win32 Debug"\
 "m3u - Win32 Debug" "id3v2 - Win32 Debug" "id3v1 - Win32 Debug"\
 "httpinput - Win32 Debug" "gdbm - Win32 Debug" "freeampui - Win32 Debug"\
 "fileinput - Win32 Debug" "dsoundcard - Win32 Debug" "download - Win32 Debug"\
 ".\portio.sys" "..\..\..\ReadMe.txt" "..\..\..\Copying.txt"\
 "..\..\..\Changes.txt" "..\..\..\Authors.txt" ".\freeamp.exe"
   IF NOT EXIST themes mkdir themes
	copy        ..\..\..\themes\freeamp.fat themes
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\portio.sys" "..\..\..\ReadMe.txt" "..\..\..\Copying.txt"\
 "..\..\..\config\config.h" "..\..\..\Changes.txt" "..\..\..\Authors.txt"\
 ".\freeamp.exe"

!ELSE 

ALL : "musicbrowser - Win32 NASM Release" "pmp300 - Win32 NASM Release"\
 "zlib - Win32 NASM Release" "xing - Win32 NASM Release"\
 "update - Win32 NASM Release" "soundcard - Win32 NASM Release"\
 "rmp - Win32 NASM Release" "obsinput - Win32 NASM Release"\
 "m3u - Win32 NASM Release" "id3v2 - Win32 NASM Release"\
 "id3v1 - Win32 NASM Release" "httpinput - Win32 NASM Release"\
 "gdbm - Win32 NASM Release" "freeampui - Win32 NASM Release"\
 "fileinput - Win32 NASM Release" "dsoundcard - Win32 NASM Release"\
 "download - Win32 NASM Release" ".\portio.sys" "..\..\..\ReadMe.txt"\
 "..\..\..\Copying.txt" "..\..\..\config\config.h" "..\..\..\Changes.txt"\
 "..\..\..\Authors.txt" ".\freeamp.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"download - Win32 NASM ReleaseCLEAN"\
 "dsoundcard - Win32 NASM ReleaseCLEAN" "fileinput - Win32 NASM ReleaseCLEAN"\
 "freeampui - Win32 NASM ReleaseCLEAN" "gdbm - Win32 NASM ReleaseCLEAN"\
 "httpinput - Win32 NASM ReleaseCLEAN" "id3v1 - Win32 NASM ReleaseCLEAN"\
 "id3v2 - Win32 NASM ReleaseCLEAN" "m3u - Win32 NASM ReleaseCLEAN"\
 "obsinput - Win32 NASM ReleaseCLEAN" "rmp - Win32 NASM ReleaseCLEAN"\
 "soundcard - Win32 NASM ReleaseCLEAN" "update - Win32 NASM ReleaseCLEAN"\
 "xing - Win32 NASM ReleaseCLEAN" "zlib - Win32 NASM ReleaseCLEAN"\
 "pmp300 - Win32 NASM ReleaseCLEAN" "musicbrowser - Win32 NASM ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\database.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\downloadmanager.obj"
	-@erase "$(INTDIR)\freeamp.res"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\musiccatalog.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\player.obj"
	-@erase "$(INTDIR)\playlist.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\propimpl.obj"
	-@erase "$(INTDIR)\registrar.obj"
	-@erase "$(INTDIR)\registry.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\win32prefs.obj"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase ".\freeamp.exe"
	-@erase "..\..\..\Authors.txt"
	-@erase "..\..\..\Changes.txt"
	-@erase "..\..\..\config\config.h"
	-@erase "..\..\..\Copying.txt"
	-@erase "..\..\..\ReadMe.txt"
	-@erase ".\portio.sys"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\..\..\lib\zlib\include" /I\
 "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I\
 "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I\
 "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include"\
 /I "..\..\..\lmc\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\freeamp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\freeamp.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\freeamp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=zlib.lib gdbm.lib version.lib winmm.lib wsock32.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo\
 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\freeamp.pdb" /machine:I386\
 /out:"freeamp.exe" /libpath:"..\..\..\lib\gdbm" /libpath:"..\..\..\lib\zlib" 
LINK32_OBJS= \
	"$(INTDIR)\database.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\downloadmanager.obj" \
	"$(INTDIR)\freeamp.res" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\musiccatalog.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\Parse.obj" \
	"$(INTDIR)\player.obj" \
	"$(INTDIR)\playlist.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\propimpl.obj" \
	"$(INTDIR)\registrar.obj" \
	"$(INTDIR)\registry.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\utility.obj" \
	"$(INTDIR)\win32prefs.obj" \
	"$(INTDIR)\win32thread.obj" \
	"..\..\..\dlm\rmp\prj\Release\rmp.lib" \
	"..\..\..\io\dsound\win32\prj\Release\dsoundcard.lib" \
	"..\..\..\io\http\win32\prj\Release\httpinput.lib" \
	"..\..\..\io\local\win32\prj\Release\fileinput.lib" \
	"..\..\..\io\obs\win32\prj\Release\obsinput.lib" \
	"..\..\..\io\soundcard\win32\prj\Release\soundcard.lib" \
	"..\..\..\lib\gdbm\gdbm.lib" \
	"..\..\..\lib\zlib\zlib.lib" \
	"..\..\..\lmc\xingmp3\win32\prj\Release\xing.lib" \
	"..\..\..\plm\metadata\id3v1\prj\Release\id3v1.lib" \
	"..\..\..\plm\metadata\id3v2\prj\Release\id3v2.lib" \
	"..\..\..\plm\playlist\m3u\prj\Release\m3u.lib" \
	"..\..\..\plm\portable\pmp300\prj\Release\pmp300.lib" \
	"..\..\..\ui\download\win32\prj\Release\download.lib" \
	"..\..\..\ui\freeamp\win32\prj\Release\freeamp.lib" \
	"..\..\..\ui\musicbrowser\win32\prj\Release\musicbrowser.lib"

".\freeamp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "musicbrowser - Win32 NASM Release"\
 "pmp300 - Win32 NASM Release" "zlib - Win32 NASM Release"\
 "xing - Win32 NASM Release" "update - Win32 NASM Release"\
 "soundcard - Win32 NASM Release" "rmp - Win32 NASM Release"\
 "obsinput - Win32 NASM Release" "m3u - Win32 NASM Release"\
 "id3v2 - Win32 NASM Release" "id3v1 - Win32 NASM Release"\
 "httpinput - Win32 NASM Release" "gdbm - Win32 NASM Release"\
 "freeampui - Win32 NASM Release" "fileinput - Win32 NASM Release"\
 "dsoundcard - Win32 NASM Release" "download - Win32 NASM Release"\
 ".\portio.sys" "..\..\..\ReadMe.txt" "..\..\..\Copying.txt"\
 "..\..\..\config\config.h" "..\..\..\Changes.txt" "..\..\..\Authors.txt"\
 ".\freeamp.exe"
   IF NOT EXIST themes mkdir themes
	copy        ..\..\..\themes\freeamp.fat themes
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\portio.sys" "..\..\..\ReadMe.txt" "..\..\..\Copying.txt"\
 "..\..\..\Changes.txt" "..\..\..\Authors.txt" ".\freeamp.exe"

!ELSE 

ALL : "musicbrowser - Win32 NASM Debug" "pmp300 - Win32 NASM Debug"\
 "zlib - Win32 NASM Debug" "xing - Win32 NASM Debug" "update - Win32 NASM Debug"\
 "soundcard - Win32 NASM Debug" "rmp - Win32 NASM Debug"\
 "obsinput - Win32 NASM Debug" "m3u - Win32 NASM Debug"\
 "id3v2 - Win32 NASM Debug" "id3v1 - Win32 NASM Debug"\
 "httpinput - Win32 NASM Debug" "gdbm - Win32 NASM Debug"\
 "freeampui - Win32 NASM Debug" "fileinput - Win32 NASM Debug"\
 "dsoundcard - Win32 NASM Debug" "download - Win32 NASM Debug" ".\portio.sys"\
 "..\..\..\ReadMe.txt" "..\..\..\Copying.txt" "..\..\..\Changes.txt"\
 "..\..\..\Authors.txt" ".\freeamp.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"download - Win32 NASM DebugCLEAN" "dsoundcard - Win32 NASM DebugCLEAN"\
 "fileinput - Win32 NASM DebugCLEAN" "freeampui - Win32 NASM DebugCLEAN"\
 "gdbm - Win32 NASM DebugCLEAN" "httpinput - Win32 NASM DebugCLEAN"\
 "id3v1 - Win32 NASM DebugCLEAN" "id3v2 - Win32 NASM DebugCLEAN"\
 "m3u - Win32 NASM DebugCLEAN" "obsinput - Win32 NASM DebugCLEAN"\
 "rmp - Win32 NASM DebugCLEAN" "soundcard - Win32 NASM DebugCLEAN"\
 "update - Win32 NASM DebugCLEAN" "xing - Win32 NASM DebugCLEAN"\
 "zlib - Win32 NASM DebugCLEAN" "pmp300 - Win32 NASM DebugCLEAN"\
 "musicbrowser - Win32 NASM DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\database.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\downloadmanager.obj"
	-@erase "$(INTDIR)\freeamp.res"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\musiccatalog.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\player.obj"
	-@erase "$(INTDIR)\playlist.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\propimpl.obj"
	-@erase "$(INTDIR)\registrar.obj"
	-@erase "$(INTDIR)\registry.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\win32prefs.obj"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(OUTDIR)\freeamp.pdb"
	-@erase ".\freeamp.exe"
	-@erase ".\freeamp.ilk"
	-@erase "..\..\..\Authors.txt"
	-@erase "..\..\..\Changes.txt"
	-@erase "..\..\..\Copying.txt"
	-@erase "..\..\..\ReadMe.txt"
	-@erase ".\portio.sys"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\lib\zlib\include" /I\
 "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I\
 "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I\
 "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include"\
 /I "..\..\..\lmc\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\freeamp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\freeamp.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\freeamp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=zlib.lib gdbm.lib version.lib winmm.lib wsock32.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo\
 /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\freeamp.pdb" /debug\
 /machine:I386 /out:"freeamp.exe" /pdbtype:sept /libpath:"..\..\..\lib\gdbm"\
 /libpath:"..\..\..\lib\zlib" 
LINK32_OBJS= \
	"$(INTDIR)\database.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\downloadmanager.obj" \
	"$(INTDIR)\freeamp.res" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\musiccatalog.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\Parse.obj" \
	"$(INTDIR)\player.obj" \
	"$(INTDIR)\playlist.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\propimpl.obj" \
	"$(INTDIR)\registrar.obj" \
	"$(INTDIR)\registry.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\utility.obj" \
	"$(INTDIR)\win32prefs.obj" \
	"$(INTDIR)\win32thread.obj" \
	"..\..\..\dlm\rmp\prj\Debug\rmp.lib" \
	"..\..\..\io\dsound\win32\prj\Debug\dsoundcard.lib" \
	"..\..\..\io\http\win32\prj\Debug\httpinput.lib" \
	"..\..\..\io\local\win32\prj\Debug\fileinput.lib" \
	"..\..\..\io\obs\win32\prj\Debug\obsinput.lib" \
	"..\..\..\io\soundcard\win32\prj\Debug\soundcard.lib" \
	"..\..\..\lib\gdbm\gdbm.lib" \
	"..\..\..\lib\zlib\zlib.lib" \
	"..\..\..\lmc\xingmp3\win32\prj\Debug\xing.lib" \
	"..\..\..\plm\metadata\id3v1\prj\Debug\id3v1.lib" \
	"..\..\..\plm\metadata\id3v2\prj\Debug\id3v2.lib" \
	"..\..\..\plm\playlist\m3u\prj\Debug\m3u.lib" \
	"..\..\..\plm\portable\pmp300\prj\Debug\pmp300.lib" \
	"..\..\..\ui\download\win32\prj\Debug\download.lib" \
	"..\..\..\ui\freeamp\win32\prj\Debug\freeamp.lib" \
	"..\..\..\ui\musicbrowser\win32\prj\Debug\musicbrowser.lib"

".\freeamp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "musicbrowser - Win32 NASM Debug"\
 "pmp300 - Win32 NASM Debug" "zlib - Win32 NASM Debug" "xing - Win32 NASM Debug"\
 "update - Win32 NASM Debug" "soundcard - Win32 NASM Debug"\
 "rmp - Win32 NASM Debug" "obsinput - Win32 NASM Debug" "m3u - Win32 NASM Debug"\
 "id3v2 - Win32 NASM Debug" "id3v1 - Win32 NASM Debug"\
 "httpinput - Win32 NASM Debug" "gdbm - Win32 NASM Debug"\
 "freeampui - Win32 NASM Debug" "fileinput - Win32 NASM Debug"\
 "dsoundcard - Win32 NASM Debug" "download - Win32 NASM Debug" ".\portio.sys"\
 "..\..\..\ReadMe.txt" "..\..\..\Copying.txt" "..\..\..\Changes.txt"\
 "..\..\..\Authors.txt" ".\freeamp.exe"
   IF NOT EXIST themes mkdir themes
	copy        ..\..\..\themes\freeamp.fat themes
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

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


!IF "$(CFG)" == "freeamp - Win32 Release" || "$(CFG)" ==\
 "freeamp - Win32 Debug" || "$(CFG)" == "freeamp - Win32 NASM Release" ||\
 "$(CFG)" == "freeamp - Win32 NASM Debug"
SOURCE=..\..\src\database.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_DATAB=\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\include\database.h"\
	"..\..\include\errors.h"\
	"..\include\mutex.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\database.obj" : $(SOURCE) $(DEP_CPP_DATAB) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_DATAB=\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\include\database.h"\
	"..\..\include\errors.h"\
	"..\include\mutex.h"\
	
NODEP_CPP_DATAB=\
	"..\..\..\config\config.h"\
	

"$(INTDIR)\database.obj" : $(SOURCE) $(DEP_CPP_DATAB) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

DEP_CPP_DATAB=\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\include\database.h"\
	"..\..\include\errors.h"\
	"..\include\mutex.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\database.obj" : $(SOURCE) $(DEP_CPP_DATAB) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

DEP_CPP_DATAB=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\include\database.h"\
	"..\..\include\errors.h"\
	"..\include\mutex.h"\
	

"$(INTDIR)\database.obj" : $(SOURCE) $(DEP_CPP_DATAB) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\debug.cpp
DEP_CPP_DEBUG=\
	"..\..\include\debug.h"\
	

"$(INTDIR)\debug.obj" : $(SOURCE) $(DEP_CPP_DEBUG) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\downloadmanager.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_DOWNL=\
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
	

"$(INTDIR)\downloadmanager.obj" : $(SOURCE) $(DEP_CPP_DOWNL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_DOWNL=\
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
	
NODEP_CPP_DOWNL=\
	"..\..\..\config\config.h"\
	

"$(INTDIR)\downloadmanager.obj" : $(SOURCE) $(DEP_CPP_DOWNL) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

DEP_CPP_DOWNL=\
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
	

"$(INTDIR)\downloadmanager.obj" : $(SOURCE) $(DEP_CPP_DOWNL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

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


!ENDIF 

SOURCE=..\..\src\log.cpp
DEP_CPP_LOG_C=\
	"..\..\include\log.h"\
	

"$(INTDIR)\log.obj" : $(SOURCE) $(DEP_CPP_LOG_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\main.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_MAIN_=\
	"..\..\..\io\include\pipeline.h"\
	"..\..\..\io\include\pullbuffer.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\lmc\include\lmc.h"\
	"..\..\..\ui\include\ui.h"\
	"..\..\include\database.h"\
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
	"..\include\win32prefs.h"\
	
NODEP_CPP_MAIN_=\
	"..\..\include\win32impl.h"\
	

"$(INTDIR)\main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_MAIN_=\
	"..\..\..\io\include\pipeline.h"\
	"..\..\..\io\include\pullbuffer.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\lmc\include\lmc.h"\
	"..\..\..\ui\include\ui.h"\
	"..\..\include\database.h"\
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
	"..\include\win32prefs.h"\
	
NODEP_CPP_MAIN_=\
	"..\..\..\config\config.h"\
	

"$(INTDIR)\main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

DEP_CPP_MAIN_=\
	"..\..\..\io\include\pipeline.h"\
	"..\..\..\io\include\pullbuffer.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\lmc\include\lmc.h"\
	"..\..\..\ui\include\ui.h"\
	"..\..\include\database.h"\
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
	"..\include\win32prefs.h"\
	
NODEP_CPP_MAIN_=\
	"..\..\include\win32impl.h"\
	

"$(INTDIR)\main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

DEP_CPP_MAIN_=\
	"..\..\..\config\config.h"\
	"..\..\..\io\include\pipeline.h"\
	"..\..\..\io\include\pullbuffer.h"\
	"..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\lmc\include\lmc.h"\
	"..\..\..\ui\include\ui.h"\
	"..\..\include\database.h"\
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
	"..\include\win32prefs.h"\
	

"$(INTDIR)\main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\musiccatalog.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_MUSIC=\
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
	

"$(INTDIR)\musiccatalog.obj" : $(SOURCE) $(DEP_CPP_MUSIC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_MUSIC=\
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
	
NODEP_CPP_MUSIC=\
	"..\..\..\config\config.h"\
	

"$(INTDIR)\musiccatalog.obj" : $(SOURCE) $(DEP_CPP_MUSIC) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

DEP_CPP_MUSIC=\
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
	

"$(INTDIR)\musiccatalog.obj" : $(SOURCE) $(DEP_CPP_MUSIC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

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


!ENDIF 

SOURCE=..\..\..\lib\xml\src\Parse.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_PARSE=\
	"..\..\..\lib\xml\include\parse.h"\
	"..\..\include\debug.h"\
	"..\..\include\errors.h"\
	

"$(INTDIR)\Parse.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_PARSE=\
	"..\..\..\lib\xml\include\parse.h"\
	"..\..\include\debug.h"\
	"..\..\include\errors.h"\
	
NODEP_CPP_PARSE=\
	"..\..\..\config\config.h"\
	

"$(INTDIR)\Parse.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

DEP_CPP_PARSE=\
	"..\..\..\lib\xml\include\parse.h"\
	"..\..\include\debug.h"\
	"..\..\include\errors.h"\
	

"$(INTDIR)\Parse.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

DEP_CPP_PARSE=\
	"..\..\..\config\config.h"\
	"..\..\..\lib\xml\include\parse.h"\
	"..\..\include\debug.h"\
	"..\..\include\errors.h"\
	

"$(INTDIR)\Parse.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\player.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_PLAYE=\
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
	

"$(INTDIR)\player.obj" : $(SOURCE) $(DEP_CPP_PLAYE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_PLAYE=\
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
	
NODEP_CPP_PLAYE=\
	"..\..\..\config\config.h"\
	

"$(INTDIR)\player.obj" : $(SOURCE) $(DEP_CPP_PLAYE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

DEP_CPP_PLAYE=\
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
	

"$(INTDIR)\player.obj" : $(SOURCE) $(DEP_CPP_PLAYE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

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


!ENDIF 

SOURCE=..\..\src\playlist.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_PLAYL=\
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
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	
NODEP_CPP_PLAYL=\
	"..\..\include\win32impl.h"\
	

"$(INTDIR)\playlist.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_PLAYL=\
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
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	
NODEP_CPP_PLAYL=\
	"..\..\..\config\config.h"\
	

"$(INTDIR)\playlist.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

DEP_CPP_PLAYL=\
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
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	
NODEP_CPP_PLAYL=\
	"..\..\include\win32impl.h"\
	

"$(INTDIR)\playlist.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

DEP_CPP_PLAYL=\
	"..\..\..\config\config.h"\
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
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	"..\..\include\thread.h"\
	"..\..\include\utility.h"\
	"..\include\mutex.h"\
	

"$(INTDIR)\playlist.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\propimpl.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_PROPI=\
	"..\..\include\errors.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\properties.h"\
	"..\..\include\propimpl.h"\
	"..\include\mutex.h"\
	

"$(INTDIR)\propimpl.obj" : $(SOURCE) $(DEP_CPP_PROPI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_PROPI=\
	"..\..\include\errors.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\properties.h"\
	"..\..\include\propimpl.h"\
	"..\include\mutex.h"\
	
NODEP_CPP_PROPI=\
	"..\..\..\config\config.h"\
	

"$(INTDIR)\propimpl.obj" : $(SOURCE) $(DEP_CPP_PROPI) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

DEP_CPP_PROPI=\
	"..\..\include\errors.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\properties.h"\
	"..\..\include\propimpl.h"\
	"..\include\mutex.h"\
	

"$(INTDIR)\propimpl.obj" : $(SOURCE) $(DEP_CPP_PROPI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

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

SOURCE=..\..\src\thread.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_THREA=\
	"..\..\include\thread.h"\
	"..\include\win32thread.h"\
	
NODEP_CPP_THREA=\
	"..\..\src\beosthread.h"\
	"..\..\src\linuxthread.h"\
	"..\..\src\solaristhread.h"\
	

"$(INTDIR)\thread.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_THREA=\
	"..\..\include\thread.h"\
	"..\include\win32thread.h"\
	
NODEP_CPP_THREA=\
	"..\..\..\config\config.h"\
	

"$(INTDIR)\thread.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

DEP_CPP_THREA=\
	"..\..\include\thread.h"\
	"..\include\win32thread.h"\
	
NODEP_CPP_THREA=\
	"..\..\src\beosthread.h"\
	"..\..\src\linuxthread.h"\
	"..\..\src\solaristhread.h"\
	

"$(INTDIR)\thread.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

DEP_CPP_THREA=\
	"..\..\..\config\config.h"\
	"..\..\include\thread.h"\
	"..\include\win32thread.h"\
	

"$(INTDIR)\thread.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\utility.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_UTILI=\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	"..\..\include\utility.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_UTILI=\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	"..\..\include\utility.h"\
	
NODEP_CPP_UTILI=\
	"..\..\..\config\config.h"\
	

"$(INTDIR)\utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

DEP_CPP_UTILI=\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	"..\..\include\utility.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

DEP_CPP_UTILI=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	"..\..\include\utility.h"\
	

"$(INTDIR)\utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\config\config.win32

!IF  "$(CFG)" == "freeamp - Win32 Release"

InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h

!ENDIF 

SOURCE=..\src\mutex.cpp
DEP_CPP_MUTEX=\
	"..\include\mutex.h"\
	

"$(INTDIR)\mutex.obj" : $(SOURCE) $(DEP_CPP_MUTEX) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\preferences.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_PREFE=\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	

"$(INTDIR)\preferences.obj" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_PREFE=\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	
NODEP_CPP_PREFE=\
	"..\..\..\config\config.h"\
	

"$(INTDIR)\preferences.obj" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

DEP_CPP_PREFE=\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	

"$(INTDIR)\preferences.obj" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

DEP_CPP_PREFE=\
	"..\..\..\config\config.h"\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	

"$(INTDIR)\preferences.obj" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\semaphore.cpp
DEP_CPP_SEMAP=\
	"..\include\semaphore.h"\
	

"$(INTDIR)\semaphore.obj" : $(SOURCE) $(DEP_CPP_SEMAP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\win32prefs.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_WIN32=\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	"..\include\win32prefs.h"\
	

"$(INTDIR)\win32prefs.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_WIN32=\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	"..\include\win32prefs.h"\
	
NODEP_CPP_WIN32=\
	"..\..\..\config\config.h"\
	

"$(INTDIR)\win32prefs.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

DEP_CPP_WIN32=\
	"..\..\include\errors.h"\
	"..\..\include\preferences.h"\
	"..\include\win32prefs.h"\
	

"$(INTDIR)\win32prefs.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

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

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_WIN32T=\
	"..\..\include\thread.h"\
	"..\include\win32thread.h"\
	

"$(INTDIR)\win32thread.obj" : $(SOURCE) $(DEP_CPP_WIN32T) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_WIN32T=\
	"..\..\include\thread.h"\
	"..\include\win32thread.h"\
	
NODEP_CPP_WIN32T=\
	"..\..\..\config\config.h"\
	

"$(INTDIR)\win32thread.obj" : $(SOURCE) $(DEP_CPP_WIN32T) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

DEP_CPP_WIN32T=\
	"..\..\include\thread.h"\
	"..\include\win32thread.h"\
	

"$(INTDIR)\win32thread.obj" : $(SOURCE) $(DEP_CPP_WIN32T) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

DEP_CPP_WIN32T=\
	"..\..\..\config\config.h"\
	"..\..\include\thread.h"\
	"..\include\win32thread.h"\
	

"$(INTDIR)\win32thread.obj" : $(SOURCE) $(DEP_CPP_WIN32T) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\registrar.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_REGIS=\
	"..\..\include\debug.h"\
	"..\..\include\errors.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	
NODEP_CPP_REGIS=\
	"..\..\include\win32impl.h"\
	

"$(INTDIR)\registrar.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_REGIS=\
	"..\..\include\debug.h"\
	"..\..\include\errors.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	
NODEP_CPP_REGIS=\
	"..\..\..\config\config.h"\
	

"$(INTDIR)\registrar.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

DEP_CPP_REGIS=\
	"..\..\include\debug.h"\
	"..\..\include\errors.h"\
	"..\..\include\hashtable.h"\
	"..\..\include\preferences.h"\
	"..\..\include\registrar.h"\
	"..\..\include\registry.h"\
	
NODEP_CPP_REGIS=\
	"..\..\include\win32impl.h"\
	

"$(INTDIR)\registrar.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

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


!ENDIF 

SOURCE=..\..\src\registry.cpp

!IF  "$(CFG)" == "freeamp - Win32 Release"

DEP_CPP_REGIST=\
	"..\..\include\registry.h"\
	

"$(INTDIR)\registry.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

DEP_CPP_REGIST=\
	"..\..\include\registry.h"\
	
NODEP_CPP_REGIST=\
	"..\..\..\config\config.h"\
	

"$(INTDIR)\registry.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

DEP_CPP_REGIST=\
	"..\..\include\registry.h"\
	

"$(INTDIR)\registry.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

DEP_CPP_REGIST=\
	"..\..\..\config\config.h"\
	"..\..\include\registry.h"\
	

"$(INTDIR)\registry.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"\
 "..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\AUTHORS

!IF  "$(CFG)" == "freeamp - Win32 Release"

InputPath=..\..\..\AUTHORS

"..\..\..\Authors.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\AUTHORS ..\..\..\Authors.txt

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

InputPath=..\..\..\AUTHORS

"..\..\..\Authors.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\AUTHORS ..\..\..\Authors.txt

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

InputPath=..\..\..\AUTHORS

"..\..\..\Authors.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\AUTHORS ..\..\..\Authors.txt

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

InputPath=..\..\..\AUTHORS

"..\..\..\Authors.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\AUTHORS ..\..\..\Authors.txt

!ENDIF 

SOURCE=..\..\..\CHANGES

!IF  "$(CFG)" == "freeamp - Win32 Release"

InputPath=..\..\..\CHANGES

"..\..\..\Changes.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\CHANGES ..\..\..\Changes.txt

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

InputPath=..\..\..\CHANGES

"..\..\..\Changes.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\CHANGES ..\..\..\Changes.txt

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

InputPath=..\..\..\CHANGES

"..\..\..\Changes.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\CHANGES ..\..\..\Changes.txt

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

InputPath=..\..\..\CHANGES

"..\..\..\Changes.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\CHANGES ..\..\..\Changes.txt

!ENDIF 

SOURCE=..\..\..\COPYING

!IF  "$(CFG)" == "freeamp - Win32 Release"

InputPath=..\..\..\COPYING

"..\..\..\Copying.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\COPYING ..\..\..\Copying.txt

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

InputPath=..\..\..\COPYING

"..\..\..\Copying.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\COPYING ..\..\..\Copying.txt

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

InputPath=..\..\..\COPYING

"..\..\..\Copying.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\COPYING ..\..\..\Copying.txt

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

InputPath=..\..\..\COPYING

"..\..\..\Copying.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\COPYING ..\..\..\Copying.txt

!ENDIF 

SOURCE=..\..\..\README

!IF  "$(CFG)" == "freeamp - Win32 Release"

InputPath=..\..\..\README

"..\..\..\ReadMe.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\README ..\..\..\ReadMe.txt

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

InputPath=..\..\..\README

"..\..\..\ReadMe.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\README ..\..\..\ReadMe.txt

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

InputPath=..\..\..\README

"..\..\..\ReadMe.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\README ..\..\..\ReadMe.txt

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

InputPath=..\..\..\README

"..\..\..\ReadMe.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\README ..\..\..\ReadMe.txt

!ENDIF 

SOURCE=..\res\freeamp.rc
DEP_RSC_FREEA=\
	"..\res\icon1.ico"\
	

!IF  "$(CFG)" == "freeamp - Win32 Release"


"$(INTDIR)\freeamp.res" : $(SOURCE) $(DEP_RSC_FREEA) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\freeamp.res" /i\
 "\Local\src\freeamp1.5\base\win32\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"


"$(INTDIR)\freeamp.res" : $(SOURCE) $(DEP_RSC_FREEA) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\freeamp.res" /i\
 "\Local\src\freeamp1.5\base\win32\res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"


"$(INTDIR)\freeamp.res" : $(SOURCE) $(DEP_RSC_FREEA) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\freeamp.res" /i\
 "\Local\src\freeamp1.5\base\win32\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"


"$(INTDIR)\freeamp.res" : $(SOURCE) $(DEP_RSC_FREEA) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\freeamp.res" /i\
 "\Local\src\freeamp1.5\base\win32\res" /d "_DEBUG" $(SOURCE)


!ENDIF 

SOURCE=..\..\..\lib\portio\i386\free\portio.sys

!IF  "$(CFG)" == "freeamp - Win32 Release"

InputPath=..\..\..\lib\portio\i386\free\portio.sys

".\portio.sys" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\lib\portio\i386\free\portio.sys .\portio.sys

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

InputPath=..\..\..\lib\portio\i386\free\portio.sys

".\portio.sys" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\lib\portio\i386\free\portio.sys .\portio.sys

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

InputPath=..\..\..\lib\portio\i386\free\portio.sys

".\portio.sys" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\lib\portio\i386\free\portio.sys .\portio.sys

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

InputPath=..\..\..\lib\portio\i386\free\portio.sys

".\portio.sys" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\lib\portio\i386\free\portio.sys .\portio.sys

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"download - Win32 Release" : 
   cd "\Local\src\freeamp1.5\ui\download\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\download.mak CFG="download - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"download - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\ui\download\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\download.mak CFG="download - Win32 Release"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"download - Win32 Debug" : 
   cd "\Local\src\freeamp1.5\ui\download\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\download.mak CFG="download - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"download - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\ui\download\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\download.mak CFG="download - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"download - Win32 NASM Release" : 
   cd "\Local\src\freeamp1.5\ui\download\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\download.mak CFG="download - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"download - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\ui\download\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\download.mak\
 CFG="download - Win32 NASM Release" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"download - Win32 NASM Debug" : 
   cd "\Local\src\freeamp1.5\ui\download\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\download.mak CFG="download - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"download - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\ui\download\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\download.mak\
 CFG="download - Win32 NASM Debug" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"dsoundcard - Win32 Release" : 
   cd "\Local\src\freeamp1.5\io\dsound\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\dsoundcard.mak CFG="dsoundcard - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"dsoundcard - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\io\dsound\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\dsoundcard.mak\
 CFG="dsoundcard - Win32 Release" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"dsoundcard - Win32 Debug" : 
   cd "\Local\src\freeamp1.5\io\dsound\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\dsoundcard.mak CFG="dsoundcard - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"dsoundcard - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\io\dsound\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\dsoundcard.mak\
 CFG="dsoundcard - Win32 Debug" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"dsoundcard - Win32 NASM Release" : 
   cd "\Local\src\freeamp1.5\io\dsound\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\dsoundcard.mak\
 CFG="dsoundcard - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"dsoundcard - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\io\dsound\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\dsoundcard.mak\
 CFG="dsoundcard - Win32 NASM Release" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"dsoundcard - Win32 NASM Debug" : 
   cd "\Local\src\freeamp1.5\io\dsound\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\dsoundcard.mak\
 CFG="dsoundcard - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"dsoundcard - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\io\dsound\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\dsoundcard.mak\
 CFG="dsoundcard - Win32 NASM Debug" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"fileinput - Win32 Release" : 
   cd "\Local\src\freeamp1.5\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"fileinput - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fileinput.mak\
 CFG="fileinput - Win32 Release" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"fileinput - Win32 Debug" : 
   cd "\Local\src\freeamp1.5\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"fileinput - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fileinput.mak CFG="fileinput - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"fileinput - Win32 NASM Release" : 
   cd "\Local\src\freeamp1.5\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak\
 CFG="fileinput - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"fileinput - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fileinput.mak\
 CFG="fileinput - Win32 NASM Release" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"fileinput - Win32 NASM Debug" : 
   cd "\Local\src\freeamp1.5\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"fileinput - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fileinput.mak\
 CFG="fileinput - Win32 NASM Debug" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"freeampui - Win32 Release" : 
   cd "\Local\src\freeamp1.5\ui\freeamp\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\freeampui.mak CFG="freeampui - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"freeampui - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\ui\freeamp\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\freeampui.mak\
 CFG="freeampui - Win32 Release" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"freeampui - Win32 Debug" : 
   cd "\Local\src\freeamp1.5\ui\freeamp\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\freeampui.mak CFG="freeampui - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"freeampui - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\ui\freeamp\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\freeampui.mak CFG="freeampui - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"freeampui - Win32 NASM Release" : 
   cd "\Local\src\freeamp1.5\ui\freeamp\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\freeampui.mak\
 CFG="freeampui - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"freeampui - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\ui\freeamp\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\freeampui.mak\
 CFG="freeampui - Win32 NASM Release" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"freeampui - Win32 NASM Debug" : 
   cd "\Local\src\freeamp1.5\ui\freeamp\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\freeampui.mak CFG="freeampui - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"freeampui - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\ui\freeamp\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\freeampui.mak\
 CFG="freeampui - Win32 NASM Debug" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"gdbm - Win32 Release" : 
   cd "\Local\src\freeamp1.5\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\gdbm.mak CFG="gdbm - Win32 Release" 
   cd "..\..\..\base\win32\prj"

"gdbm - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\gdbm.mak CFG="gdbm - Win32 Release"\
 RECURSE=1 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"gdbm - Win32 Debug" : 
   cd "\Local\src\freeamp1.5\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\gdbm.mak CFG="gdbm - Win32 Debug" 
   cd "..\..\..\base\win32\prj"

"gdbm - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\gdbm.mak CFG="gdbm - Win32 Debug" RECURSE=1\
 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"gdbm - Win32 NASM Release" : 
   cd "\Local\src\freeamp1.5\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\gdbm.mak CFG="gdbm - Win32 NASM Release" 
   cd "..\..\..\base\win32\prj"

"gdbm - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\gdbm.mak CFG="gdbm - Win32 NASM Release"\
 RECURSE=1 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"gdbm - Win32 NASM Debug" : 
   cd "\Local\src\freeamp1.5\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\gdbm.mak CFG="gdbm - Win32 NASM Debug" 
   cd "..\..\..\base\win32\prj"

"gdbm - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\gdbm.mak CFG="gdbm - Win32 NASM Debug"\
 RECURSE=1 
   cd "..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"httpinput - Win32 Release" : 
   cd "\Local\src\freeamp1.5\io\http\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\httpinput.mak CFG="httpinput - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"httpinput - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\io\http\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\httpinput.mak\
 CFG="httpinput - Win32 Release" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"httpinput - Win32 Debug" : 
   cd "\Local\src\freeamp1.5\io\http\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\httpinput.mak CFG="httpinput - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"httpinput - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\io\http\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\httpinput.mak CFG="httpinput - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"httpinput - Win32 NASM Release" : 
   cd "\Local\src\freeamp1.5\io\http\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\httpinput.mak\
 CFG="httpinput - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"httpinput - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\io\http\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\httpinput.mak\
 CFG="httpinput - Win32 NASM Release" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"httpinput - Win32 NASM Debug" : 
   cd "\Local\src\freeamp1.5\io\http\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\httpinput.mak CFG="httpinput - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"httpinput - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\io\http\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\httpinput.mak\
 CFG="httpinput - Win32 NASM Debug" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"id3v1 - Win32 Release" : 
   cd "\Local\src\freeamp1.5\plm\metadata\id3v1\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v1.mak CFG="id3v1 - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"id3v1 - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\plm\metadata\id3v1\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\id3v1.mak CFG="id3v1 - Win32 Release"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"id3v1 - Win32 Debug" : 
   cd "\Local\src\freeamp1.5\plm\metadata\id3v1\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v1.mak CFG="id3v1 - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"id3v1 - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\plm\metadata\id3v1\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\id3v1.mak CFG="id3v1 - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"id3v1 - Win32 NASM Release" : 
   cd "\Local\src\freeamp1.5\plm\metadata\id3v1\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v1.mak CFG="id3v1 - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"id3v1 - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\plm\metadata\id3v1\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\id3v1.mak CFG="id3v1 - Win32 NASM Release"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"id3v1 - Win32 NASM Debug" : 
   cd "\Local\src\freeamp1.5\plm\metadata\id3v1\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v1.mak CFG="id3v1 - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"id3v1 - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\plm\metadata\id3v1\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\id3v1.mak CFG="id3v1 - Win32 NASM Debug"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"id3v2 - Win32 Release" : 
   cd "\Local\src\freeamp1.5\plm\metadata\id3v2\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v2.mak CFG="id3v2 - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"id3v2 - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\plm\metadata\id3v2\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\id3v2.mak CFG="id3v2 - Win32 Release"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"id3v2 - Win32 Debug" : 
   cd "\Local\src\freeamp1.5\plm\metadata\id3v2\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v2.mak CFG="id3v2 - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"id3v2 - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\plm\metadata\id3v2\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\id3v2.mak CFG="id3v2 - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"id3v2 - Win32 NASM Release" : 
   cd "\Local\src\freeamp1.5\plm\metadata\id3v2\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v2.mak CFG="id3v2 - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"id3v2 - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\plm\metadata\id3v2\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\id3v2.mak CFG="id3v2 - Win32 NASM Release"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"id3v2 - Win32 NASM Debug" : 
   cd "\Local\src\freeamp1.5\plm\metadata\id3v2\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v2.mak CFG="id3v2 - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"id3v2 - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\plm\metadata\id3v2\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\id3v2.mak CFG="id3v2 - Win32 NASM Debug"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"m3u - Win32 Release" : 
   cd "\Local\src\freeamp1.5\plm\playlist\m3u\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\m3u.mak CFG="m3u - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"m3u - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\plm\playlist\m3u\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\m3u.mak CFG="m3u - Win32 Release" RECURSE=1\
 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"m3u - Win32 Debug" : 
   cd "\Local\src\freeamp1.5\plm\playlist\m3u\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\m3u.mak CFG="m3u - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"m3u - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\plm\playlist\m3u\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\m3u.mak CFG="m3u - Win32 Debug" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"m3u - Win32 NASM Release" : 
   cd "\Local\src\freeamp1.5\plm\playlist\m3u\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\m3u.mak CFG="m3u - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"m3u - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\plm\playlist\m3u\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\m3u.mak CFG="m3u - Win32 NASM Release"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"m3u - Win32 NASM Debug" : 
   cd "\Local\src\freeamp1.5\plm\playlist\m3u\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\m3u.mak CFG="m3u - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"m3u - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\plm\playlist\m3u\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\m3u.mak CFG="m3u - Win32 NASM Debug"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"obsinput - Win32 Release" : 
   cd "\Local\src\freeamp1.5\io\obs\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\obsinput.mak CFG="obsinput - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"obsinput - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\io\obs\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\obsinput.mak CFG="obsinput - Win32 Release"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"obsinput - Win32 Debug" : 
   cd "\Local\src\freeamp1.5\io\obs\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\obsinput.mak CFG="obsinput - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"obsinput - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\io\obs\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\obsinput.mak CFG="obsinput - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"obsinput - Win32 NASM Release" : 
   cd "\Local\src\freeamp1.5\io\obs\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\obsinput.mak CFG="obsinput - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"obsinput - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\io\obs\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\obsinput.mak\
 CFG="obsinput - Win32 NASM Release" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"obsinput - Win32 NASM Debug" : 
   cd "\Local\src\freeamp1.5\io\obs\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\obsinput.mak CFG="obsinput - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"obsinput - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\io\obs\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\obsinput.mak\
 CFG="obsinput - Win32 NASM Debug" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"rmp - Win32 Release" : 
   cd "\Local\src\freeamp1.5\dlm\rmp\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\rmp.mak CFG="rmp - Win32 Release" 
   cd "..\..\..\base\win32\prj"

"rmp - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\dlm\rmp\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\rmp.mak CFG="rmp - Win32 Release" RECURSE=1\
 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"rmp - Win32 Debug" : 
   cd "\Local\src\freeamp1.5\dlm\rmp\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\rmp.mak CFG="rmp - Win32 Debug" 
   cd "..\..\..\base\win32\prj"

"rmp - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\dlm\rmp\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\rmp.mak CFG="rmp - Win32 Debug" RECURSE=1 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"rmp - Win32 NASM Release" : 
   cd "\Local\src\freeamp1.5\dlm\rmp\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\rmp.mak CFG="rmp - Win32 NASM Release" 
   cd "..\..\..\base\win32\prj"

"rmp - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\dlm\rmp\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\rmp.mak CFG="rmp - Win32 NASM Release"\
 RECURSE=1 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"rmp - Win32 NASM Debug" : 
   cd "\Local\src\freeamp1.5\dlm\rmp\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\rmp.mak CFG="rmp - Win32 NASM Debug" 
   cd "..\..\..\base\win32\prj"

"rmp - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\dlm\rmp\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\rmp.mak CFG="rmp - Win32 NASM Debug"\
 RECURSE=1 
   cd "..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"soundcard - Win32 Release" : 
   cd "\Local\src\freeamp1.5\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"soundcard - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\soundcard.mak\
 CFG="soundcard - Win32 Release" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"soundcard - Win32 Debug" : 
   cd "\Local\src\freeamp1.5\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"soundcard - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\soundcard.mak CFG="soundcard - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"soundcard - Win32 NASM Release" : 
   cd "\Local\src\freeamp1.5\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak\
 CFG="soundcard - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"soundcard - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\soundcard.mak\
 CFG="soundcard - Win32 NASM Release" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"soundcard - Win32 NASM Debug" : 
   cd "\Local\src\freeamp1.5\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"soundcard - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\soundcard.mak\
 CFG="soundcard - Win32 NASM Debug" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"update - Win32 Release" : 
   cd "\Local\src\freeamp1.5\update\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\update.mak CFG="update - Win32 Release" 
   cd "..\..\..\base\win32\prj"

"update - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\update\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\update.mak CFG="update - Win32 Release"\
 RECURSE=1 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"update - Win32 Debug" : 
   cd "\Local\src\freeamp1.5\update\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\update.mak CFG="update - Win32 Debug" 
   cd "..\..\..\base\win32\prj"

"update - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\update\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\update.mak CFG="update - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"update - Win32 NASM Release" : 
   cd "\Local\src\freeamp1.5\update\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\update.mak CFG="update - Win32 NASM Release" 
   cd "..\..\..\base\win32\prj"

"update - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\update\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\update.mak\
 CFG="update - Win32 NASM Release" RECURSE=1 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"update - Win32 NASM Debug" : 
   cd "\Local\src\freeamp1.5\update\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\update.mak CFG="update - Win32 NASM Debug" 
   cd "..\..\..\base\win32\prj"

"update - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\update\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\update.mak CFG="update - Win32 NASM Debug"\
 RECURSE=1 
   cd "..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"xing - Win32 Release" : 
   cd "\Local\src\freeamp1.5\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"xing - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\xing.mak CFG="xing - Win32 Release"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"xing - Win32 Debug" : 
   cd "\Local\src\freeamp1.5\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"xing - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\xing.mak CFG="xing - Win32 Debug" RECURSE=1\
 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"xing - Win32 NASM Release" : 
   cd "\Local\src\freeamp1.5\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"xing - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\xing.mak CFG="xing - Win32 NASM Release"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"xing - Win32 NASM Debug" : 
   cd "\Local\src\freeamp1.5\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"xing - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\xing.mak CFG="xing - Win32 NASM Debug"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"zlib - Win32 Release" : 
   cd "\Local\src\freeamp1.5\lib\zlib\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 Release" 
   cd "..\..\..\base\win32\prj"

"zlib - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\lib\zlib\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\zlib.mak CFG="zlib - Win32 Release"\
 RECURSE=1 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"zlib - Win32 Debug" : 
   cd "\Local\src\freeamp1.5\lib\zlib\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 Debug" 
   cd "..\..\..\base\win32\prj"

"zlib - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\lib\zlib\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\zlib.mak CFG="zlib - Win32 Debug" RECURSE=1\
 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"zlib - Win32 NASM Release" : 
   cd "\Local\src\freeamp1.5\lib\zlib\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 NASM Release" 
   cd "..\..\..\base\win32\prj"

"zlib - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\lib\zlib\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\zlib.mak CFG="zlib - Win32 NASM Release"\
 RECURSE=1 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"zlib - Win32 NASM Debug" : 
   cd "\Local\src\freeamp1.5\lib\zlib\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 NASM Debug" 
   cd "..\..\..\base\win32\prj"

"zlib - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\lib\zlib\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\zlib.mak CFG="zlib - Win32 NASM Debug"\
 RECURSE=1 
   cd "..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"pmp300 - Win32 Release" : 
   cd "\Local\src\freeamp1.5\plm\portable\pmp300\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\pmp300.mak CFG="pmp300 - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"pmp300 - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\plm\portable\pmp300\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\pmp300.mak CFG="pmp300 - Win32 Release"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"pmp300 - Win32 Debug" : 
   cd "\Local\src\freeamp1.5\plm\portable\pmp300\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\pmp300.mak CFG="pmp300 - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"pmp300 - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\plm\portable\pmp300\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\pmp300.mak CFG="pmp300 - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"pmp300 - Win32 NASM Release" : 
   cd "\Local\src\freeamp1.5\plm\portable\pmp300\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\pmp300.mak CFG="pmp300 - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"pmp300 - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\plm\portable\pmp300\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\pmp300.mak\
 CFG="pmp300 - Win32 NASM Release" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"pmp300 - Win32 NASM Debug" : 
   cd "\Local\src\freeamp1.5\plm\portable\pmp300\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\pmp300.mak CFG="pmp300 - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"pmp300 - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\plm\portable\pmp300\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\pmp300.mak CFG="pmp300 - Win32 NASM Debug"\
 RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"musicbrowser - Win32 Release" : 
   cd "\Local\src\freeamp1.5\ui\musicbrowser\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\musicbrowser.mak\
 CFG="musicbrowser - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"musicbrowser - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\ui\musicbrowser\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\musicbrowser.mak\
 CFG="musicbrowser - Win32 Release" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"musicbrowser - Win32 Debug" : 
   cd "\Local\src\freeamp1.5\ui\musicbrowser\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\musicbrowser.mak CFG="musicbrowser - Win32 Debug"\
 
   cd "..\..\..\..\base\win32\prj"

"musicbrowser - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\ui\musicbrowser\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\musicbrowser.mak\
 CFG="musicbrowser - Win32 Debug" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"musicbrowser - Win32 NASM Release" : 
   cd "\Local\src\freeamp1.5\ui\musicbrowser\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\musicbrowser.mak\
 CFG="musicbrowser - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"musicbrowser - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp1.5\ui\musicbrowser\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\musicbrowser.mak\
 CFG="musicbrowser - Win32 NASM Release" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"musicbrowser - Win32 NASM Debug" : 
   cd "\Local\src\freeamp1.5\ui\musicbrowser\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\musicbrowser.mak\
 CFG="musicbrowser - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"musicbrowser - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp1.5\ui\musicbrowser\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\musicbrowser.mak\
 CFG="musicbrowser - Win32 NASM Debug" RECURSE=1 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 


!ENDIF 


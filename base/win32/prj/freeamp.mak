# Microsoft Developer Studio Generated NMAKE File, Based on freeamp.dsp
!IF "$(CFG)" == ""
CFG=freeamp - Win32 NASM Debug
!MESSAGE No configuration specified. Defaulting to freeamp - Win32 NASM Debug.
!ENDIF 

!IF "$(CFG)" != "freeamp - Win32 Release" && "$(CFG)" != "freeamp - Win32 Debug" && "$(CFG)" != "freeamp - Win32 NASM Release" && "$(CFG)" != "freeamp - Win32 NASM Debug"
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

ALL : ".\freeamp.hlp" ".\freeamp.cnt" ".\themes\FreeAmpClassic.fat" ".\themes\FreeAmp.fat" ".\themes\EMusic.fat" ".\themes\Aquatica.fat" ".\portio.sys" ".\id3lib.dll" "..\..\..\ReadMe.txt" "..\..\..\Copying.txt" "..\..\..\config\config.h" "..\..\..\Changes.txt" "..\..\..\Authors.txt" ".\freeamp.exe"

!ELSE 

ALL : "id3v2 - Win32 Release" "dde - Win32 Release" "cddb - Win32 Release" "cdoutput - Win32 Release" "cdlmc - Win32 Release" "winamp - Win32 Release" "pls - Win32 Release" "MakeTheme - Win32 Release" "misc - Win32 Release" "fabaselib - Win32 Release" "toolbar - Win32 Release" "musicbrowser - Win32 Release" "pmp300 - Win32 Release" "zlib - Win32 Release" "xing - Win32 Release" "update - Win32 Release" "soundcard - Win32 Release" "rmp - Win32 Release" "obsinput - Win32 Release" "m3u - Win32 Release" "id3v1 - Win32 Release" "httpinput - Win32 Release" "gdbm - Win32 Release" "freeampui - Win32 Release" "fileinput - Win32 Release" "dsoundcard - Win32 Release" "download - Win32 Release" ".\freeamp.hlp" ".\freeamp.cnt" ".\themes\FreeAmpClassic.fat" ".\themes\FreeAmp.fat" ".\themes\EMusic.fat" ".\themes\Aquatica.fat" ".\portio.sys" ".\id3lib.dll" "..\..\..\ReadMe.txt" "..\..\..\Copying.txt" "..\..\..\config\config.h" "..\..\..\Changes.txt" "..\..\..\Authors.txt" ".\freeamp.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"download - Win32 ReleaseCLEAN" "dsoundcard - Win32 ReleaseCLEAN" "fileinput - Win32 ReleaseCLEAN" "freeampui - Win32 ReleaseCLEAN" "gdbm - Win32 ReleaseCLEAN" "httpinput - Win32 ReleaseCLEAN" "id3v1 - Win32 ReleaseCLEAN" "m3u - Win32 ReleaseCLEAN" "obsinput - Win32 ReleaseCLEAN" "rmp - Win32 ReleaseCLEAN" "soundcard - Win32 ReleaseCLEAN" "update - Win32 ReleaseCLEAN" "xing - Win32 ReleaseCLEAN" "zlib - Win32 ReleaseCLEAN" "pmp300 - Win32 ReleaseCLEAN" "musicbrowser - Win32 ReleaseCLEAN" "toolbar - Win32 ReleaseCLEAN" "fabaselib - Win32 ReleaseCLEAN" "misc - Win32 ReleaseCLEAN" "MakeTheme - Win32 ReleaseCLEAN" "pls - Win32 ReleaseCLEAN" "winamp - Win32 ReleaseCLEAN" "cdlmc - Win32 ReleaseCLEAN" "cdoutput - Win32 ReleaseCLEAN" "cddb - Win32 ReleaseCLEAN" "dde - Win32 ReleaseCLEAN" "id3v2 - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\freeamp.res"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\freeamp.exe"
	-@erase "..\..\..\Authors.txt"
	-@erase "..\..\..\Changes.txt"
	-@erase "..\..\..\config\config.h"
	-@erase "..\..\..\Copying.txt"
	-@erase "..\..\..\ReadMe.txt"
	-@erase ".\id3lib.dll"
	-@erase ".\portio.sys"
	-@erase ".\themes\Aquatica.fat"
	-@erase ".\themes\EMusic.fat"
	-@erase ".\themes\FreeAmp.fat"
	-@erase ".\themes\FreeAmpClassic.fat"
	-@erase "freeamp.cnt"
	-@erase "freeamp.hlp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\..\..\lib\zlib\include" /I "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include" /I "..\..\..\lmc\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\freeamp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\freeamp.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\freeamp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib zlib.lib gdbm.lib version.lib winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\freeamp.pdb" /machine:I386 /out:"freeamp.exe" /libpath:"..\..\..\lib\gdbm" /libpath:"..\..\..\lib\zlib" /libpath:"..\\" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\Parse.obj" \
	"$(INTDIR)\freeamp.res" \
	"..\..\..\ui\download\win32\prj\Release\download.lib" \
	"..\..\..\io\dsound\win32\prj\Release\dsoundcard.lib" \
	"..\..\..\io\local\win32\prj\Release\fileinput.lib" \
	"..\..\..\ui\freeamp\win32\prj\Release\freeamp.lib" \
	"..\..\..\lib\gdbm\gdbm.lib" \
	"..\..\..\io\http\win32\prj\Release\httpinput.lib" \
	"..\..\..\plm\metadata\id3v1\prj\Release\id3v1.lib" \
	"..\..\..\plm\playlist\m3u\prj\Release\m3u.lib" \
	"..\..\..\io\obs\win32\prj\Release\obsinput.lib" \
	"..\..\..\dlm\rmp\prj\Release\rmp.lib" \
	"..\..\..\io\soundcard\win32\prj\Release\soundcard.lib" \
	"..\..\..\lmc\xingmp3\win32\prj\Release\xing.lib" \
	"..\..\..\lib\zlib\zlib.lib" \
	"..\..\..\plm\portable\pmp300\prj\Release\pmp300.lib" \
	"..\..\..\ui\musicbrowser\win32\prj\Release\musicbrowser.lib" \
	"..\..\..\ui\toolbar\win32\prj\Release\toolbar.lib" \
	"..\fabaselib.lib" \
	"..\..\..\plm\metadata\misc\prj\Release\misc.lib" \
	"..\..\..\plm\playlist\pls\prj\Release\pls.lib" \
	"..\..\..\ftc\winamp\prj\Release\winamp.lib" \
	"..\..\..\lmc\cd\win32\prj\Release\cd.lib" \
	"..\..\..\io\cd\win32\prj\Release\cd.lib" \
	"..\..\..\plm\metadata\cddb\prj\Release\cddb.lib" \
	"..\..\..\ui\dde\win32\prj\Release\dde.lib" \
	"..\..\..\plm\metadata\id3v2\prj\Release\id3v2.lib"

".\freeamp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\freeamp.hlp" ".\freeamp.cnt" ".\themes\FreeAmpClassic.fat" ".\themes\FreeAmp.fat" ".\themes\EMusic.fat" ".\themes\Aquatica.fat" ".\portio.sys" ".\id3lib.dll" "..\..\..\ReadMe.txt" "..\..\..\Copying.txt" "..\..\..\config\config.h" "..\..\..\Changes.txt" "..\..\..\Authors.txt" ".\freeamp.exe"

!ELSE 

ALL : "id3v2 - Win32 Debug" "dde - Win32 Debug" "cddb - Win32 Debug" "cdoutput - Win32 Debug" "cdlmc - Win32 Debug" "winamp - Win32 Debug" "pls - Win32 Debug" "MakeTheme - Win32 Debug" "misc - Win32 Debug" "fabaselib - Win32 Debug" "toolbar - Win32 Debug" "musicbrowser - Win32 Debug" "pmp300 - Win32 Debug" "zlib - Win32 Debug" "xing - Win32 Debug" "update - Win32 Debug" "soundcard - Win32 Debug" "rmp - Win32 Debug" "obsinput - Win32 Debug" "m3u - Win32 Debug" "id3v1 - Win32 Debug" "httpinput - Win32 Debug" "gdbm - Win32 Debug" "freeampui - Win32 Debug" "fileinput - Win32 Debug" "dsoundcard - Win32 Debug" "download - Win32 Debug" ".\freeamp.hlp" ".\freeamp.cnt" ".\themes\FreeAmpClassic.fat" ".\themes\FreeAmp.fat" ".\themes\EMusic.fat" ".\themes\Aquatica.fat" ".\portio.sys" ".\id3lib.dll" "..\..\..\ReadMe.txt" "..\..\..\Copying.txt" "..\..\..\config\config.h" "..\..\..\Changes.txt" "..\..\..\Authors.txt" ".\freeamp.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"download - Win32 DebugCLEAN" "dsoundcard - Win32 DebugCLEAN" "fileinput - Win32 DebugCLEAN" "freeampui - Win32 DebugCLEAN" "gdbm - Win32 DebugCLEAN" "httpinput - Win32 DebugCLEAN" "id3v1 - Win32 DebugCLEAN" "m3u - Win32 DebugCLEAN" "obsinput - Win32 DebugCLEAN" "rmp - Win32 DebugCLEAN" "soundcard - Win32 DebugCLEAN" "update - Win32 DebugCLEAN" "xing - Win32 DebugCLEAN" "zlib - Win32 DebugCLEAN" "pmp300 - Win32 DebugCLEAN" "musicbrowser - Win32 DebugCLEAN" "toolbar - Win32 DebugCLEAN" "fabaselib - Win32 DebugCLEAN" "misc - Win32 DebugCLEAN" "MakeTheme - Win32 DebugCLEAN" "pls - Win32 DebugCLEAN" "winamp - Win32 DebugCLEAN" "cdlmc - Win32 DebugCLEAN" "cdoutput - Win32 DebugCLEAN" "cddb - Win32 DebugCLEAN" "dde - Win32 DebugCLEAN" "id3v2 - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\freeamp.res"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\freeamp.pdb"
	-@erase ".\freeamp.exe"
	-@erase ".\freeamp.ilk"
	-@erase "..\..\..\Authors.txt"
	-@erase "..\..\..\Changes.txt"
	-@erase "..\..\..\config\config.h"
	-@erase "..\..\..\Copying.txt"
	-@erase "..\..\..\ReadMe.txt"
	-@erase ".\id3lib.dll"
	-@erase ".\portio.sys"
	-@erase ".\themes\Aquatica.fat"
	-@erase ".\themes\EMusic.fat"
	-@erase ".\themes\FreeAmp.fat"
	-@erase ".\themes\FreeAmpClassic.fat"
	-@erase "freeamp.cnt"
	-@erase "freeamp.hlp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\lib\zlib\include" /I "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include" /I "..\..\..\lmc\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\freeamp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\freeamp.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\freeamp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib zlib.lib gdbm.lib version.lib winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\freeamp.pdb" /debug /machine:I386 /out:"freeamp.exe" /pdbtype:sept /libpath:"..\..\..\lib\gdbm" /libpath:"..\..\..\lib\zlib" /libpath:"..\\" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\Parse.obj" \
	"$(INTDIR)\freeamp.res" \
	"..\..\..\ui\download\win32\prj\Debug\download.lib" \
	"..\..\..\io\dsound\win32\prj\Debug\dsoundcard.lib" \
	"..\..\..\io\local\win32\prj\Debug\fileinput.lib" \
	"..\..\..\ui\freeamp\win32\prj\Debug\freeamp.lib" \
	"..\..\..\lib\gdbm\gdbm.lib" \
	"..\..\..\io\http\win32\prj\Debug\httpinput.lib" \
	"..\..\..\plm\metadata\id3v1\prj\Debug\id3v1.lib" \
	"..\..\..\plm\playlist\m3u\prj\Debug\m3u.lib" \
	"..\..\..\io\obs\win32\prj\Debug\obsinput.lib" \
	"..\..\..\dlm\rmp\prj\Debug\rmp.lib" \
	"..\..\..\io\soundcard\win32\prj\Debug\soundcard.lib" \
	"..\..\..\lmc\xingmp3\win32\prj\Debug\xing.lib" \
	"..\..\..\lib\zlib\zlib.lib" \
	"..\..\..\plm\portable\pmp300\prj\Debug\pmp300.lib" \
	"..\..\..\ui\musicbrowser\win32\prj\Debug\musicbrowser.lib" \
	"..\..\..\ui\toolbar\win32\prj\Debug\toolbar.lib" \
	"..\fabaselib.lib" \
	"..\..\..\plm\metadata\misc\prj\Debug\misc.lib" \
	"..\..\..\plm\playlist\pls\prj\Debug\pls.lib" \
	"..\..\..\ftc\winamp\prj\Debug\winamp.lib" \
	"..\..\..\lmc\cd\win32\prj\Debug\cd.lib" \
	"..\..\..\io\cd\win32\prj\Debug\cd.lib" \
	"..\..\..\plm\metadata\cddb\prj\Debug\cddb.lib" \
	"..\..\..\ui\dde\win32\prj\Debug\dde.lib" \
	"..\..\..\plm\metadata\id3v2\prj\Debug\id3v2.lib"

".\freeamp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\freeamp.hlp" ".\freeamp.cnt" ".\themes\FreeAmpClassic.fat" ".\themes\FreeAmp.fat" ".\themes\EMusic.fat" ".\themes\Aquatica.fat" ".\portio.sys" ".\id3lib.dll" "..\..\..\ReadMe.txt" "..\..\..\Copying.txt" "..\..\..\config\config.h" "..\..\..\Changes.txt" "..\..\..\Authors.txt" ".\freeamp.exe"

!ELSE 

ALL : "id3v2 - Win32 NASM Release" "dde - Win32 NASM Release" "cddb - Win32 NASM Release" "cdoutput - Win32 NASM Release" "cdlmc - Win32 NASM Release" "winamp - Win32 NASM Release" "pls - Win32 NASM Release" "MakeTheme - Win32 NASM Release" "misc - Win32 NASM Release" "fabaselib - Win32 NASM Release" "toolbar - Win32 NASM Release" "musicbrowser - Win32 NASM Release" "pmp300 - Win32 NASM Release" "zlib - Win32 NASM Release" "xing - Win32 NASM Release" "update - Win32 NASM Release" "soundcard - Win32 NASM Release" "rmp - Win32 NASM Release" "obsinput - Win32 NASM Release" "m3u - Win32 NASM Release" "id3v1 - Win32 NASM Release" "httpinput - Win32 NASM Release" "gdbm - Win32 NASM Release" "freeampui - Win32 NASM Release" "fileinput - Win32 NASM Release" "dsoundcard - Win32 NASM Release" "download - Win32 NASM Release" ".\freeamp.hlp" ".\freeamp.cnt" ".\themes\FreeAmpClassic.fat" ".\themes\FreeAmp.fat" ".\themes\EMusic.fat" ".\themes\Aquatica.fat" ".\portio.sys" ".\id3lib.dll" "..\..\..\ReadMe.txt" "..\..\..\Copying.txt" "..\..\..\config\config.h" "..\..\..\Changes.txt" "..\..\..\Authors.txt" ".\freeamp.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"download - Win32 NASM ReleaseCLEAN" "dsoundcard - Win32 NASM ReleaseCLEAN" "fileinput - Win32 NASM ReleaseCLEAN" "freeampui - Win32 NASM ReleaseCLEAN" "gdbm - Win32 NASM ReleaseCLEAN" "httpinput - Win32 NASM ReleaseCLEAN" "id3v1 - Win32 NASM ReleaseCLEAN" "m3u - Win32 NASM ReleaseCLEAN" "obsinput - Win32 NASM ReleaseCLEAN" "rmp - Win32 NASM ReleaseCLEAN" "soundcard - Win32 NASM ReleaseCLEAN" "update - Win32 NASM ReleaseCLEAN" "xing - Win32 NASM ReleaseCLEAN" "zlib - Win32 NASM ReleaseCLEAN" "pmp300 - Win32 NASM ReleaseCLEAN" "musicbrowser - Win32 NASM ReleaseCLEAN" "toolbar - Win32 NASM ReleaseCLEAN" "fabaselib - Win32 NASM ReleaseCLEAN" "misc - Win32 NASM ReleaseCLEAN" "MakeTheme - Win32 NASM ReleaseCLEAN" "pls - Win32 NASM ReleaseCLEAN" "winamp - Win32 NASM ReleaseCLEAN" "cdlmc - Win32 NASM ReleaseCLEAN" "cdoutput - Win32 NASM ReleaseCLEAN" "cddb - Win32 NASM ReleaseCLEAN" "dde - Win32 NASM ReleaseCLEAN" "id3v2 - Win32 NASM ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\freeamp.res"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\freeamp.exe"
	-@erase "..\..\..\Authors.txt"
	-@erase "..\..\..\Changes.txt"
	-@erase "..\..\..\config\config.h"
	-@erase "..\..\..\Copying.txt"
	-@erase "..\..\..\ReadMe.txt"
	-@erase ".\id3lib.dll"
	-@erase ".\portio.sys"
	-@erase ".\themes\Aquatica.fat"
	-@erase ".\themes\EMusic.fat"
	-@erase ".\themes\FreeAmp.fat"
	-@erase ".\themes\FreeAmpClassic.fat"
	-@erase "freeamp.cnt"
	-@erase "freeamp.hlp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\..\..\lib\zlib\include" /I "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include" /I "..\..\..\lmc\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\freeamp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\freeamp.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\freeamp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib zlib.lib gdbm.lib version.lib winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\freeamp.pdb" /machine:I386 /out:"freeamp.exe" /libpath:"..\..\..\lib\gdbm" /libpath:"..\..\..\lib\zlib" /libpath:"..\\" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\Parse.obj" \
	"$(INTDIR)\freeamp.res" \
	"..\..\..\ui\download\win32\prj\Release\download.lib" \
	"..\..\..\io\dsound\win32\prj\Release\dsoundcard.lib" \
	"..\..\..\io\local\win32\prj\Release\fileinput.lib" \
	"..\..\..\ui\freeamp\win32\prj\Release\freeamp.lib" \
	"..\..\..\lib\gdbm\gdbm.lib" \
	"..\..\..\io\http\win32\prj\Release\httpinput.lib" \
	"..\..\..\plm\metadata\id3v1\prj\Release\id3v1.lib" \
	"..\..\..\plm\playlist\m3u\prj\Release\m3u.lib" \
	"..\..\..\io\obs\win32\prj\Release\obsinput.lib" \
	"..\..\..\dlm\rmp\prj\Release\rmp.lib" \
	"..\..\..\io\soundcard\win32\prj\Release\soundcard.lib" \
	"..\..\..\lmc\xingmp3\win32\prj\Release\xing.lib" \
	"..\..\..\lib\zlib\zlib.lib" \
	"..\..\..\plm\portable\pmp300\prj\Release\pmp300.lib" \
	"..\..\..\ui\musicbrowser\win32\prj\Release\musicbrowser.lib" \
	"..\..\..\ui\toolbar\win32\prj\Release\toolbar.lib" \
	"..\fabaselib.lib" \
	"..\..\..\plm\metadata\misc\prj\Release\misc.lib" \
	"..\..\..\plm\playlist\pls\prj\Release\pls.lib" \
	"..\..\..\ftc\winamp\prj\Release\winamp.lib" \
	"..\..\..\lmc\cd\win32\prj\Release\cd.lib" \
	"..\..\..\io\cd\win32\prj\Release\cd.lib" \
	"..\..\..\plm\metadata\cddb\prj\Release\cddb.lib" \
	"..\..\..\ui\dde\win32\prj\Release\dde.lib" \
	"..\..\..\plm\metadata\id3v2\prj\Release\id3v2.lib"

".\freeamp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\freeamp.hlp" ".\freeamp.cnt" ".\themes\FreeAmpClassic.fat" ".\themes\FreeAmp.fat" ".\themes\EMusic.fat" ".\themes\Aquatica.fat" ".\portio.sys" ".\id3lib.dll" "..\..\..\ReadMe.txt" "..\..\..\Copying.txt" "..\..\..\Changes.txt" "..\..\..\Authors.txt" ".\freeamp.exe"

!ELSE 

ALL : "id3v2 - Win32 NASM Debug" "dde - Win32 NASM Debug" "cddb - Win32 NASM Debug" "cdoutput - Win32 NASM Debug" "cdlmc - Win32 NASM Debug" "winamp - Win32 NASM Debug" "pls - Win32 NASM Debug" "MakeTheme - Win32 NASM Debug" "misc - Win32 NASM Debug" "fabaselib - Win32 NASM Debug" "toolbar - Win32 NASM Debug" "musicbrowser - Win32 NASM Debug" "pmp300 - Win32 NASM Debug" "zlib - Win32 NASM Debug" "xing - Win32 NASM Debug" "update - Win32 NASM Debug" "soundcard - Win32 NASM Debug" "rmp - Win32 NASM Debug" "obsinput - Win32 NASM Debug" "m3u - Win32 NASM Debug" "id3v1 - Win32 NASM Debug" "httpinput - Win32 NASM Debug" "gdbm - Win32 NASM Debug" "freeampui - Win32 NASM Debug" "fileinput - Win32 NASM Debug" "dsoundcard - Win32 NASM Debug" "download - Win32 NASM Debug" ".\freeamp.hlp" ".\freeamp.cnt" ".\themes\FreeAmpClassic.fat" ".\themes\FreeAmp.fat" ".\themes\EMusic.fat" ".\themes\Aquatica.fat" ".\portio.sys" ".\id3lib.dll" "..\..\..\ReadMe.txt" "..\..\..\Copying.txt" "..\..\..\Changes.txt" "..\..\..\Authors.txt" ".\freeamp.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"download - Win32 NASM DebugCLEAN" "dsoundcard - Win32 NASM DebugCLEAN" "fileinput - Win32 NASM DebugCLEAN" "freeampui - Win32 NASM DebugCLEAN" "gdbm - Win32 NASM DebugCLEAN" "httpinput - Win32 NASM DebugCLEAN" "id3v1 - Win32 NASM DebugCLEAN" "m3u - Win32 NASM DebugCLEAN" "obsinput - Win32 NASM DebugCLEAN" "rmp - Win32 NASM DebugCLEAN" "soundcard - Win32 NASM DebugCLEAN" "update - Win32 NASM DebugCLEAN" "xing - Win32 NASM DebugCLEAN" "zlib - Win32 NASM DebugCLEAN" "pmp300 - Win32 NASM DebugCLEAN" "musicbrowser - Win32 NASM DebugCLEAN" "toolbar - Win32 NASM DebugCLEAN" "fabaselib - Win32 NASM DebugCLEAN" "misc - Win32 NASM DebugCLEAN" "MakeTheme - Win32 NASM DebugCLEAN" "pls - Win32 NASM DebugCLEAN" "winamp - Win32 NASM DebugCLEAN" "cdlmc - Win32 NASM DebugCLEAN" "cdoutput - Win32 NASM DebugCLEAN" "cddb - Win32 NASM DebugCLEAN" "dde - Win32 NASM DebugCLEAN" "id3v2 - Win32 NASM DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\freeamp.res"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\freeamp.pdb"
	-@erase ".\freeamp.exe"
	-@erase ".\freeamp.ilk"
	-@erase "..\..\..\Authors.txt"
	-@erase "..\..\..\Changes.txt"
	-@erase "..\..\..\Copying.txt"
	-@erase "..\..\..\ReadMe.txt"
	-@erase ".\id3lib.dll"
	-@erase ".\portio.sys"
	-@erase ".\themes\Aquatica.fat"
	-@erase ".\themes\EMusic.fat"
	-@erase ".\themes\FreeAmp.fat"
	-@erase ".\themes\FreeAmpClassic.fat"
	-@erase "freeamp.cnt"
	-@erase "freeamp.hlp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\lib\zlib\include" /I "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include" /I "..\..\..\lmc\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\freeamp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\freeamp.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\freeamp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib zlib.lib gdbm.lib version.lib winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\freeamp.pdb" /debug /machine:I386 /out:"freeamp.exe" /pdbtype:sept /libpath:"..\..\..\lib\gdbm" /libpath:"..\..\..\lib\zlib" /libpath:"..\\" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\Parse.obj" \
	"$(INTDIR)\freeamp.res" \
	"..\..\..\ui\download\win32\prj\Debug\download.lib" \
	"..\..\..\io\dsound\win32\prj\Debug\dsoundcard.lib" \
	"..\..\..\io\local\win32\prj\Debug\fileinput.lib" \
	"..\..\..\ui\freeamp\win32\prj\Debug\freeamp.lib" \
	"..\..\..\lib\gdbm\gdbm.lib" \
	"..\..\..\io\http\win32\prj\Debug\httpinput.lib" \
	"..\..\..\plm\metadata\id3v1\prj\Debug\id3v1.lib" \
	"..\..\..\plm\playlist\m3u\prj\Debug\m3u.lib" \
	"..\..\..\io\obs\win32\prj\Debug\obsinput.lib" \
	"..\..\..\dlm\rmp\prj\Debug\rmp.lib" \
	"..\..\..\io\soundcard\win32\prj\Debug\soundcard.lib" \
	"..\..\..\lmc\xingmp3\win32\prj\Debug\xing.lib" \
	"..\..\..\lib\zlib\zlib.lib" \
	"..\..\..\plm\portable\pmp300\prj\Debug\pmp300.lib" \
	"..\..\..\ui\musicbrowser\win32\prj\Debug\musicbrowser.lib" \
	"..\..\..\ui\toolbar\win32\prj\Debug\toolbar.lib" \
	"..\fabaselib.lib" \
	"..\..\..\plm\metadata\misc\prj\Debug\misc.lib" \
	"..\..\..\plm\playlist\pls\prj\Debug\pls.lib" \
	"..\..\..\ftc\winamp\prj\Debug\winamp.lib" \
	"..\..\..\lmc\cd\win32\prj\Debug\cd.lib" \
	"..\..\..\io\cd\win32\prj\Debug\cd.lib" \
	"..\..\..\plm\metadata\cddb\prj\Debug\cddb.lib" \
	"..\..\..\ui\dde\win32\prj\Debug\dde.lib" \
	"..\..\..\plm\metadata\id3v2\prj\Debug\id3v2.lib"

".\freeamp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

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
!IF EXISTS("freeamp.dep")
!INCLUDE "freeamp.dep"
!ELSE 
!MESSAGE Warning: cannot find "freeamp.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "freeamp - Win32 Release" || "$(CFG)" == "freeamp - Win32 Debug" || "$(CFG)" == "freeamp - Win32 NASM Release" || "$(CFG)" == "freeamp - Win32 NASM Debug"
SOURCE=..\..\..\config\config.win32

!IF  "$(CFG)" == "freeamp - Win32 Release"

InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h
<< 
	

!ENDIF 

SOURCE=..\src\main.cpp

"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\xml\src\Parse.cpp

"$(INTDIR)\Parse.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\AUTHORS

!IF  "$(CFG)" == "freeamp - Win32 Release"

InputPath=..\..\..\AUTHORS

"..\..\..\Authors.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\AUTHORS ..\..\..\Authors.txt
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

InputPath=..\..\..\AUTHORS

"..\..\..\Authors.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\AUTHORS ..\..\..\Authors.txt
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

InputPath=..\..\..\AUTHORS

"..\..\..\Authors.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\AUTHORS ..\..\..\Authors.txt
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

InputPath=..\..\..\AUTHORS

"..\..\..\Authors.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\AUTHORS ..\..\..\Authors.txt
<< 
	

!ENDIF 

SOURCE=..\..\..\CHANGES

!IF  "$(CFG)" == "freeamp - Win32 Release"

InputPath=..\..\..\CHANGES

"..\..\..\Changes.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\CHANGES ..\..\..\Changes.txt
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

InputPath=..\..\..\CHANGES

"..\..\..\Changes.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\CHANGES ..\..\..\Changes.txt
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

InputPath=..\..\..\CHANGES

"..\..\..\Changes.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\CHANGES ..\..\..\Changes.txt
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

InputPath=..\..\..\CHANGES

"..\..\..\Changes.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\CHANGES ..\..\..\Changes.txt
<< 
	

!ENDIF 

SOURCE=..\..\..\COPYING

!IF  "$(CFG)" == "freeamp - Win32 Release"

InputPath=..\..\..\COPYING

"..\..\..\Copying.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\COPYING ..\..\..\Copying.txt
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

InputPath=..\..\..\COPYING

"..\..\..\Copying.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\COPYING ..\..\..\Copying.txt
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

InputPath=..\..\..\COPYING

"..\..\..\Copying.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\COPYING ..\..\..\Copying.txt
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

InputPath=..\..\..\COPYING

"..\..\..\Copying.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\COPYING ..\..\..\Copying.txt
<< 
	

!ENDIF 

SOURCE=..\..\..\README

!IF  "$(CFG)" == "freeamp - Win32 Release"

InputPath=..\..\..\README

"..\..\..\ReadMe.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\README ..\..\..\ReadMe.txt
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

InputPath=..\..\..\README

"..\..\..\ReadMe.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\README ..\..\..\ReadMe.txt
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

InputPath=..\..\..\README

"..\..\..\ReadMe.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\README ..\..\..\ReadMe.txt
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

InputPath=..\..\..\README

"..\..\..\ReadMe.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\README ..\..\..\ReadMe.txt
<< 
	

!ENDIF 

SOURCE=..\..\..\help\win32\freeamp.cnt

!IF  "$(CFG)" == "freeamp - Win32 Release"

InputPath=..\..\..\help\win32\freeamp.cnt

".\freeamp.cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy  ..\..\..\help\win32\freeamp.cnt .
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

InputPath=..\..\..\help\win32\freeamp.cnt

".\freeamp.cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy  ..\..\..\help\win32\freeamp.cnt .
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

InputPath=..\..\..\help\win32\freeamp.cnt

".\freeamp.cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy  ..\..\..\help\win32\freeamp.cnt .
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

InputPath=..\..\..\help\win32\freeamp.cnt

".\freeamp.cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy  ..\..\..\help\win32\freeamp.cnt .
<< 
	

!ENDIF 

SOURCE=..\..\..\help\win32\freeamp.hlp

!IF  "$(CFG)" == "freeamp - Win32 Release"

InputPath=..\..\..\help\win32\freeamp.hlp

".\freeamp.hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy  ..\..\..\help\win32\freeamp.hlp .
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

InputPath=..\..\..\help\win32\freeamp.hlp

".\freeamp.hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy  ..\..\..\help\win32\freeamp.hlp .
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

InputPath=..\..\..\help\win32\freeamp.hlp

".\freeamp.hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy  ..\..\..\help\win32\freeamp.hlp .
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

InputPath=..\..\..\help\win32\freeamp.hlp

".\freeamp.hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy  ..\..\..\help\win32\freeamp.hlp .
<< 
	

!ENDIF 

SOURCE=..\res\freeamp.rc

!IF  "$(CFG)" == "freeamp - Win32 Release"


"$(INTDIR)\freeamp.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\freeamp.res" /i "\Local\src\freeamp\base\win32\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"


"$(INTDIR)\freeamp.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\freeamp.res" /i "\Local\src\freeamp\base\win32\res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"


"$(INTDIR)\freeamp.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\freeamp.res" /i "\Local\src\freeamp\base\win32\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"


"$(INTDIR)\freeamp.res" : $(SOURCE) "$(INTDIR)" "..\..\..\config\config.h"
	$(RSC) /l 0x409 /fo"$(INTDIR)\freeamp.res" /i "\Local\src\freeamp\base\win32\res" /d "_DEBUG" $(SOURCE)


!ENDIF 

SOURCE=..\..\..\lib\portio\i386\free\portio.sys

!IF  "$(CFG)" == "freeamp - Win32 Release"

InputPath=..\..\..\lib\portio\i386\free\portio.sys

".\portio.sys" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\lib\portio\i386\free\portio.sys .\portio.sys
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

InputPath=..\..\..\lib\portio\i386\free\portio.sys

".\portio.sys" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\lib\portio\i386\free\portio.sys .\portio.sys
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

InputPath=..\..\..\lib\portio\i386\free\portio.sys

".\portio.sys" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\lib\portio\i386\free\portio.sys .\portio.sys
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

InputPath=..\..\..\lib\portio\i386\free\portio.sys

".\portio.sys" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\..\..\lib\portio\i386\free\portio.sys .\portio.sys
<< 
	

!ENDIF 

SOURCE=..\..\..\themes\Aquatica.fat

!IF  "$(CFG)" == "freeamp - Win32 Release"

InputPath=..\..\..\themes\Aquatica.fat
InputName=Aquatica

".\themes\Aquatica.fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

InputPath=..\..\..\themes\Aquatica.fat
InputName=Aquatica

".\themes\Aquatica.fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

InputPath=..\..\..\themes\Aquatica.fat
InputName=Aquatica

".\themes\Aquatica.fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

InputPath=..\..\..\themes\Aquatica.fat
InputName=Aquatica

".\themes\Aquatica.fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
<< 
	

!ENDIF 

SOURCE=..\..\..\themes\EMusic.fat

!IF  "$(CFG)" == "freeamp - Win32 Release"

InputPath=..\..\..\themes\EMusic.fat
InputName=EMusic

".\themes\EMusic.fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

InputPath=..\..\..\themes\EMusic.fat
InputName=EMusic

".\themes\EMusic.fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

InputPath=..\..\..\themes\EMusic.fat
InputName=EMusic

".\themes\EMusic.fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

InputPath=..\..\..\themes\EMusic.fat
InputName=EMusic

".\themes\EMusic.fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
<< 
	

!ENDIF 

SOURCE=..\..\..\themes\FreeAmp.fat

!IF  "$(CFG)" == "freeamp - Win32 Release"

InputPath=..\..\..\themes\FreeAmp.fat
InputName=FreeAmp

".\themes\FreeAmp.fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

InputPath=..\..\..\themes\FreeAmp.fat
InputName=FreeAmp

".\themes\FreeAmp.fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

InputPath=..\..\..\themes\FreeAmp.fat
InputName=FreeAmp

".\themes\FreeAmp.fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

InputPath=..\..\..\themes\FreeAmp.fat
InputName=FreeAmp

".\themes\FreeAmp.fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
<< 
	

!ENDIF 

SOURCE=..\..\..\themes\FreeAmpClassic.fat

!IF  "$(CFG)" == "freeamp - Win32 Release"

InputPath=..\..\..\themes\FreeAmpClassic.fat
InputName=FreeAmpClassic

".\themes\FreeAmpClassic.fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

InputPath=..\..\..\themes\FreeAmpClassic.fat
InputName=FreeAmpClassic

".\themes\FreeAmpClassic.fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

InputPath=..\..\..\themes\FreeAmpClassic.fat
InputName=FreeAmpClassic

".\themes\FreeAmpClassic.fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

InputPath=..\..\..\themes\FreeAmpClassic.fat
InputName=FreeAmpClassic

".\themes\FreeAmpClassic.fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
<< 
	

!ENDIF 

SOURCE=..\..\..\lib\id3\id3lib.dll

!IF  "$(CFG)" == "freeamp - Win32 Release"

InputPath=..\..\..\lib\id3\id3lib.dll
InputName=id3lib

".\id3lib.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy $(InputPath) .
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

InputPath=..\..\..\lib\id3\id3lib.dll
InputName=id3lib

".\id3lib.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy $(InputPath) .
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

InputPath=..\..\..\lib\id3\id3lib.dll
InputName=id3lib

".\id3lib.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy $(InputPath) .
<< 
	

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

InputPath=..\..\..\lib\id3\id3lib.dll
InputName=id3lib

".\id3lib.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy $(InputPath) .
<< 
	

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"download - Win32 Release" : 
   cd "\Local\src\freeamp\ui\download\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\download.mak CFG="download - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"download - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\ui\download\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\download.mak CFG="download - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"download - Win32 Debug" : 
   cd "\Local\src\freeamp\ui\download\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\download.mak CFG="download - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"download - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\ui\download\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\download.mak CFG="download - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"download - Win32 NASM Release" : 
   cd "\Local\src\freeamp\ui\download\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\download.mak CFG="download - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"download - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\ui\download\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\download.mak CFG="download - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"download - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\ui\download\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\download.mak CFG="download - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"download - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\ui\download\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\download.mak CFG="download - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"dsoundcard - Win32 Release" : 
   cd "\Local\src\freeamp\io\dsound\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\dsoundcard.mak CFG="dsoundcard - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"dsoundcard - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\io\dsound\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\dsoundcard.mak CFG="dsoundcard - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"dsoundcard - Win32 Debug" : 
   cd "\Local\src\freeamp\io\dsound\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\dsoundcard.mak CFG="dsoundcard - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"dsoundcard - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\io\dsound\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\dsoundcard.mak CFG="dsoundcard - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"dsoundcard - Win32 NASM Release" : 
   cd "\Local\src\freeamp\io\dsound\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\dsoundcard.mak CFG="dsoundcard - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"dsoundcard - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\io\dsound\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\dsoundcard.mak CFG="dsoundcard - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"dsoundcard - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\io\dsound\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\dsoundcard.mak CFG="dsoundcard - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"dsoundcard - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\io\dsound\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\dsoundcard.mak CFG="dsoundcard - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"fileinput - Win32 Release" : 
   cd "\Local\src\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"fileinput - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"fileinput - Win32 Debug" : 
   cd "\Local\src\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"fileinput - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"fileinput - Win32 NASM Release" : 
   cd "\Local\src\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"fileinput - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"fileinput - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"fileinput - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"freeampui - Win32 Release" : 
   cd "\Local\src\freeamp\ui\freeamp\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\freeampui.mak CFG="freeampui - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"freeampui - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\ui\freeamp\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\freeampui.mak CFG="freeampui - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"freeampui - Win32 Debug" : 
   cd "\Local\src\freeamp\ui\freeamp\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\freeampui.mak CFG="freeampui - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"freeampui - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\ui\freeamp\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\freeampui.mak CFG="freeampui - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"freeampui - Win32 NASM Release" : 
   cd "\Local\src\freeamp\ui\freeamp\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\freeampui.mak CFG="freeampui - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"freeampui - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\ui\freeamp\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\freeampui.mak CFG="freeampui - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"freeampui - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\ui\freeamp\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\freeampui.mak CFG="freeampui - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"freeampui - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\ui\freeamp\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\freeampui.mak CFG="freeampui - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"gdbm - Win32 Release" : 
   cd "\Local\src\freeamp\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\gdbm.mak CFG="gdbm - Win32 Release" 
   cd "..\..\..\base\win32\prj"

"gdbm - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\gdbm.mak CFG="gdbm - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"gdbm - Win32 Debug" : 
   cd "\Local\src\freeamp\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\gdbm.mak CFG="gdbm - Win32 Debug" 
   cd "..\..\..\base\win32\prj"

"gdbm - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\gdbm.mak CFG="gdbm - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"gdbm - Win32 NASM Release" : 
   cd "\Local\src\freeamp\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\gdbm.mak CFG="gdbm - Win32 NASM Release" 
   cd "..\..\..\base\win32\prj"

"gdbm - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\gdbm.mak CFG="gdbm - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"gdbm - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\gdbm.mak CFG="gdbm - Win32 NASM Debug" 
   cd "..\..\..\base\win32\prj"

"gdbm - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\gdbm.mak CFG="gdbm - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"httpinput - Win32 Release" : 
   cd "\Local\src\freeamp\io\http\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\httpinput.mak CFG="httpinput - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"httpinput - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\io\http\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\httpinput.mak CFG="httpinput - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"httpinput - Win32 Debug" : 
   cd "\Local\src\freeamp\io\http\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\httpinput.mak CFG="httpinput - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"httpinput - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\io\http\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\httpinput.mak CFG="httpinput - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"httpinput - Win32 NASM Release" : 
   cd "\Local\src\freeamp\io\http\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\httpinput.mak CFG="httpinput - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"httpinput - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\io\http\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\httpinput.mak CFG="httpinput - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"httpinput - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\io\http\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\httpinput.mak CFG="httpinput - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"httpinput - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\io\http\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\httpinput.mak CFG="httpinput - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"id3v1 - Win32 Release" : 
   cd "\Local\src\freeamp\plm\metadata\id3v1\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v1.mak CFG="id3v1 - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"id3v1 - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\plm\metadata\id3v1\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v1.mak CFG="id3v1 - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"id3v1 - Win32 Debug" : 
   cd "\Local\src\freeamp\plm\metadata\id3v1\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v1.mak CFG="id3v1 - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"id3v1 - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\plm\metadata\id3v1\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v1.mak CFG="id3v1 - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"id3v1 - Win32 NASM Release" : 
   cd "\Local\src\freeamp\plm\metadata\id3v1\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v1.mak CFG="id3v1 - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"id3v1 - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\plm\metadata\id3v1\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v1.mak CFG="id3v1 - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"id3v1 - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\plm\metadata\id3v1\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v1.mak CFG="id3v1 - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"id3v1 - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\plm\metadata\id3v1\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v1.mak CFG="id3v1 - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"m3u - Win32 Release" : 
   cd "\Local\src\freeamp\plm\playlist\m3u\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\m3u.mak CFG="m3u - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"m3u - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\plm\playlist\m3u\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\m3u.mak CFG="m3u - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"m3u - Win32 Debug" : 
   cd "\Local\src\freeamp\plm\playlist\m3u\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\m3u.mak CFG="m3u - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"m3u - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\plm\playlist\m3u\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\m3u.mak CFG="m3u - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"m3u - Win32 NASM Release" : 
   cd "\Local\src\freeamp\plm\playlist\m3u\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\m3u.mak CFG="m3u - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"m3u - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\plm\playlist\m3u\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\m3u.mak CFG="m3u - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"m3u - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\plm\playlist\m3u\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\m3u.mak CFG="m3u - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"m3u - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\plm\playlist\m3u\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\m3u.mak CFG="m3u - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"obsinput - Win32 Release" : 
   cd "\Local\src\freeamp\io\obs\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\obsinput.mak CFG="obsinput - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"obsinput - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\io\obs\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\obsinput.mak CFG="obsinput - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"obsinput - Win32 Debug" : 
   cd "\Local\src\freeamp\io\obs\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\obsinput.mak CFG="obsinput - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"obsinput - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\io\obs\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\obsinput.mak CFG="obsinput - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"obsinput - Win32 NASM Release" : 
   cd "\Local\src\freeamp\io\obs\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\obsinput.mak CFG="obsinput - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"obsinput - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\io\obs\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\obsinput.mak CFG="obsinput - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"obsinput - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\io\obs\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\obsinput.mak CFG="obsinput - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"obsinput - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\io\obs\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\obsinput.mak CFG="obsinput - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"rmp - Win32 Release" : 
   cd "\Local\src\freeamp\dlm\rmp\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\rmp.mak CFG="rmp - Win32 Release" 
   cd "..\..\..\base\win32\prj"

"rmp - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\dlm\rmp\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\rmp.mak CFG="rmp - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"rmp - Win32 Debug" : 
   cd "\Local\src\freeamp\dlm\rmp\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\rmp.mak CFG="rmp - Win32 Debug" 
   cd "..\..\..\base\win32\prj"

"rmp - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\dlm\rmp\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\rmp.mak CFG="rmp - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"rmp - Win32 NASM Release" : 
   cd "\Local\src\freeamp\dlm\rmp\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\rmp.mak CFG="rmp - Win32 NASM Release" 
   cd "..\..\..\base\win32\prj"

"rmp - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\dlm\rmp\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\rmp.mak CFG="rmp - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"rmp - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\dlm\rmp\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\rmp.mak CFG="rmp - Win32 NASM Debug" 
   cd "..\..\..\base\win32\prj"

"rmp - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\dlm\rmp\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\rmp.mak CFG="rmp - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"soundcard - Win32 Release" : 
   cd "\Local\src\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"soundcard - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"soundcard - Win32 Debug" : 
   cd "\Local\src\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"soundcard - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"soundcard - Win32 NASM Release" : 
   cd "\Local\src\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"soundcard - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"soundcard - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"soundcard - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"update - Win32 Release" : 
   cd "\Local\src\freeamp\update\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\update.mak CFG="update - Win32 Release" 
   cd "..\..\..\base\win32\prj"

"update - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\update\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\update.mak CFG="update - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"update - Win32 Debug" : 
   cd "\Local\src\freeamp\update\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\update.mak CFG="update - Win32 Debug" 
   cd "..\..\..\base\win32\prj"

"update - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\update\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\update.mak CFG="update - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"update - Win32 NASM Release" : 
   cd "\Local\src\freeamp\update\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\update.mak CFG="update - Win32 NASM Release" 
   cd "..\..\..\base\win32\prj"

"update - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\update\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\update.mak CFG="update - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"update - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\update\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\update.mak CFG="update - Win32 NASM Debug" 
   cd "..\..\..\base\win32\prj"

"update - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\update\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\update.mak CFG="update - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"xing - Win32 Release" : 
   cd "\Local\src\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"xing - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"xing - Win32 Debug" : 
   cd "\Local\src\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"xing - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"xing - Win32 NASM Release" : 
   cd "\Local\src\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"xing - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"xing - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"xing - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"zlib - Win32 Release" : 
   cd "\Local\src\freeamp\lib\zlib\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 Release" 
   cd "..\..\..\base\win32\prj"

"zlib - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\lib\zlib\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"zlib - Win32 Debug" : 
   cd "\Local\src\freeamp\lib\zlib\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 Debug" 
   cd "..\..\..\base\win32\prj"

"zlib - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\lib\zlib\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"zlib - Win32 NASM Release" : 
   cd "\Local\src\freeamp\lib\zlib\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 NASM Release" 
   cd "..\..\..\base\win32\prj"

"zlib - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\lib\zlib\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"zlib - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\lib\zlib\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 NASM Debug" 
   cd "..\..\..\base\win32\prj"

"zlib - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\lib\zlib\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"pmp300 - Win32 Release" : 
   cd "\Local\src\freeamp\plm\portable\pmp300\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\pmp300.mak CFG="pmp300 - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"pmp300 - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\plm\portable\pmp300\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\pmp300.mak CFG="pmp300 - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"pmp300 - Win32 Debug" : 
   cd "\Local\src\freeamp\plm\portable\pmp300\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\pmp300.mak CFG="pmp300 - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"pmp300 - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\plm\portable\pmp300\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\pmp300.mak CFG="pmp300 - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"pmp300 - Win32 NASM Release" : 
   cd "\Local\src\freeamp\plm\portable\pmp300\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\pmp300.mak CFG="pmp300 - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"pmp300 - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\plm\portable\pmp300\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\pmp300.mak CFG="pmp300 - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"pmp300 - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\plm\portable\pmp300\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\pmp300.mak CFG="pmp300 - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"pmp300 - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\plm\portable\pmp300\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\pmp300.mak CFG="pmp300 - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"musicbrowser - Win32 Release" : 
   cd "\Local\src\freeamp\ui\musicbrowser\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\musicbrowser.mak CFG="musicbrowser - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"musicbrowser - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\ui\musicbrowser\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\musicbrowser.mak CFG="musicbrowser - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"musicbrowser - Win32 Debug" : 
   cd "\Local\src\freeamp\ui\musicbrowser\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\musicbrowser.mak CFG="musicbrowser - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"musicbrowser - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\ui\musicbrowser\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\musicbrowser.mak CFG="musicbrowser - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"musicbrowser - Win32 NASM Release" : 
   cd "\Local\src\freeamp\ui\musicbrowser\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\musicbrowser.mak CFG="musicbrowser - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"musicbrowser - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\ui\musicbrowser\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\musicbrowser.mak CFG="musicbrowser - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"musicbrowser - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\ui\musicbrowser\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\musicbrowser.mak CFG="musicbrowser - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"musicbrowser - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\ui\musicbrowser\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\musicbrowser.mak CFG="musicbrowser - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"toolbar - Win32 Release" : 
   cd "\Local\src\freeamp\ui\toolbar\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\toolbar.mak CFG="toolbar - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"toolbar - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\ui\toolbar\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\toolbar.mak CFG="toolbar - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"toolbar - Win32 Debug" : 
   cd "\Local\src\freeamp\ui\toolbar\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\toolbar.mak CFG="toolbar - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"toolbar - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\ui\toolbar\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\toolbar.mak CFG="toolbar - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"toolbar - Win32 NASM Release" : 
   cd "\Local\src\freeamp\ui\toolbar\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\toolbar.mak CFG="toolbar - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"toolbar - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\ui\toolbar\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\toolbar.mak CFG="toolbar - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"toolbar - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\ui\toolbar\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\toolbar.mak CFG="toolbar - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"toolbar - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\ui\toolbar\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\toolbar.mak CFG="toolbar - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"fabaselib - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 Release" 
   cd "."

"fabaselib - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"fabaselib - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 Debug" 
   cd "."

"fabaselib - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"fabaselib - Win32 NASM Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 NASM Release" 
   cd "."

"fabaselib - Win32 NASM ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"fabaselib - Win32 NASM Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 NASM Debug" 
   cd "."

"fabaselib - Win32 NASM DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"misc - Win32 Release" : 
   cd "\Local\src\freeamp\plm\metadata\misc\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\misc.mak CFG="misc - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"misc - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\plm\metadata\misc\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\misc.mak CFG="misc - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"misc - Win32 Debug" : 
   cd "\Local\src\freeamp\plm\metadata\misc\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\misc.mak CFG="misc - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"misc - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\plm\metadata\misc\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\misc.mak CFG="misc - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"misc - Win32 NASM Release" : 
   cd "\Local\src\freeamp\plm\metadata\misc\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\misc.mak CFG="misc - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"misc - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\plm\metadata\misc\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\misc.mak CFG="misc - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"misc - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\plm\metadata\misc\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\misc.mak CFG="misc - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"misc - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\plm\metadata\misc\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\misc.mak CFG="misc - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"MakeTheme - Win32 Release" : 
   cd "\Local\src\freeamp\ui\freeamp\tools\win32"
   $(MAKE) /$(MAKEFLAGS) /F .\MakeTheme.mak CFG="MakeTheme - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"MakeTheme - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\ui\freeamp\tools\win32"
   $(MAKE) /$(MAKEFLAGS) /F .\MakeTheme.mak CFG="MakeTheme - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"MakeTheme - Win32 Debug" : 
   cd "\Local\src\freeamp\ui\freeamp\tools\win32"
   $(MAKE) /$(MAKEFLAGS) /F .\MakeTheme.mak CFG="MakeTheme - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"MakeTheme - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\ui\freeamp\tools\win32"
   $(MAKE) /$(MAKEFLAGS) /F .\MakeTheme.mak CFG="MakeTheme - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"MakeTheme - Win32 NASM Release" : 
   cd "\Local\src\freeamp\ui\freeamp\tools\win32"
   $(MAKE) /$(MAKEFLAGS) /F .\MakeTheme.mak CFG="MakeTheme - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"MakeTheme - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\ui\freeamp\tools\win32"
   $(MAKE) /$(MAKEFLAGS) /F .\MakeTheme.mak CFG="MakeTheme - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"MakeTheme - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\ui\freeamp\tools\win32"
   $(MAKE) /$(MAKEFLAGS) /F .\MakeTheme.mak CFG="MakeTheme - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"MakeTheme - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\ui\freeamp\tools\win32"
   $(MAKE) /$(MAKEFLAGS) /F .\MakeTheme.mak CFG="MakeTheme - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"pls - Win32 Release" : 
   cd "\Local\src\freeamp\plm\playlist\pls\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\pls.mak CFG="pls - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"pls - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\plm\playlist\pls\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\pls.mak CFG="pls - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"pls - Win32 Debug" : 
   cd "\Local\src\freeamp\plm\playlist\pls\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\pls.mak CFG="pls - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"pls - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\plm\playlist\pls\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\pls.mak CFG="pls - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"pls - Win32 NASM Release" : 
   cd "\Local\src\freeamp\plm\playlist\pls\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\pls.mak CFG="pls - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"pls - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\plm\playlist\pls\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\pls.mak CFG="pls - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"pls - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\plm\playlist\pls\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\pls.mak CFG="pls - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"pls - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\plm\playlist\pls\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\pls.mak CFG="pls - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"winamp - Win32 Release" : 
   cd "\Local\src\freeamp\ftc\winamp\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\winamp.mak CFG="winamp - Win32 Release" 
   cd "..\..\..\base\win32\prj"

"winamp - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\ftc\winamp\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\winamp.mak CFG="winamp - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"winamp - Win32 Debug" : 
   cd "\Local\src\freeamp\ftc\winamp\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\winamp.mak CFG="winamp - Win32 Debug" 
   cd "..\..\..\base\win32\prj"

"winamp - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\ftc\winamp\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\winamp.mak CFG="winamp - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"winamp - Win32 NASM Release" : 
   cd "\Local\src\freeamp\ftc\winamp\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\winamp.mak CFG="winamp - Win32 NASM Release" 
   cd "..\..\..\base\win32\prj"

"winamp - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\ftc\winamp\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\winamp.mak CFG="winamp - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"winamp - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\ftc\winamp\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\winamp.mak CFG="winamp - Win32 NASM Debug" 
   cd "..\..\..\base\win32\prj"

"winamp - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\ftc\winamp\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\winamp.mak CFG="winamp - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"cdlmc - Win32 Release" : 
   cd "\Local\src\freeamp\lmc\cd\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cdlmc.mak CFG="cdlmc - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"cdlmc - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\lmc\cd\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cdlmc.mak CFG="cdlmc - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"cdlmc - Win32 Debug" : 
   cd "\Local\src\freeamp\lmc\cd\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cdlmc.mak CFG="cdlmc - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"cdlmc - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\lmc\cd\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cdlmc.mak CFG="cdlmc - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"cdlmc - Win32 NASM Release" : 
   cd "\Local\src\freeamp\lmc\cd\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cdlmc.mak CFG="cdlmc - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"cdlmc - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\lmc\cd\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cdlmc.mak CFG="cdlmc - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"cdlmc - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\lmc\cd\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cdlmc.mak CFG="cdlmc - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"cdlmc - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\lmc\cd\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cdlmc.mak CFG="cdlmc - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"cdoutput - Win32 Release" : 
   cd "\Local\src\freeamp\io\cd\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cdoutput.mak CFG="cdoutput - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"cdoutput - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\io\cd\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cdoutput.mak CFG="cdoutput - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"cdoutput - Win32 Debug" : 
   cd "\Local\src\freeamp\io\cd\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cdoutput.mak CFG="cdoutput - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"cdoutput - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\io\cd\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cdoutput.mak CFG="cdoutput - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"cdoutput - Win32 NASM Release" : 
   cd "\Local\src\freeamp\io\cd\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cdoutput.mak CFG="cdoutput - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"cdoutput - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\io\cd\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cdoutput.mak CFG="cdoutput - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"cdoutput - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\io\cd\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cdoutput.mak CFG="cdoutput - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"cdoutput - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\io\cd\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cdoutput.mak CFG="cdoutput - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"cddb - Win32 Release" : 
   cd "\Local\src\freeamp\plm\metadata\cddb\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cddb.mak CFG="cddb - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"cddb - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\plm\metadata\cddb\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cddb.mak CFG="cddb - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"cddb - Win32 Debug" : 
   cd "\Local\src\freeamp\plm\metadata\cddb\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cddb.mak CFG="cddb - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"cddb - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\plm\metadata\cddb\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cddb.mak CFG="cddb - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"cddb - Win32 NASM Release" : 
   cd "\Local\src\freeamp\plm\metadata\cddb\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cddb.mak CFG="cddb - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"cddb - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\plm\metadata\cddb\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cddb.mak CFG="cddb - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"cddb - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\plm\metadata\cddb\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cddb.mak CFG="cddb - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"cddb - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\plm\metadata\cddb\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\cddb.mak CFG="cddb - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"dde - Win32 Release" : 
   cd "\Local\src\freeamp\ui\dde\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\dde.mak CFG="dde - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"dde - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\ui\dde\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\dde.mak CFG="dde - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"dde - Win32 Debug" : 
   cd "\Local\src\freeamp\ui\dde\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\dde.mak CFG="dde - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"dde - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\ui\dde\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\dde.mak CFG="dde - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"dde - Win32 NASM Release" : 
   cd "\Local\src\freeamp\ui\dde\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\dde.mak CFG="dde - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"dde - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\ui\dde\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\dde.mak CFG="dde - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"dde - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\ui\dde\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\dde.mak CFG="dde - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"dde - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\ui\dde\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\dde.mak CFG="dde - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeamp - Win32 Release"

"id3v2 - Win32 Release" : 
   cd "\Local\src\freeamp\plm\metadata\id3v2\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v2.mak CFG="id3v2 - Win32 Release" 
   cd "..\..\..\..\base\win32\prj"

"id3v2 - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\plm\metadata\id3v2\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v2.mak CFG="id3v2 - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

"id3v2 - Win32 Debug" : 
   cd "\Local\src\freeamp\plm\metadata\id3v2\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v2.mak CFG="id3v2 - Win32 Debug" 
   cd "..\..\..\..\base\win32\prj"

"id3v2 - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\plm\metadata\id3v2\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v2.mak CFG="id3v2 - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

"id3v2 - Win32 NASM Release" : 
   cd "\Local\src\freeamp\plm\metadata\id3v2\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v2.mak CFG="id3v2 - Win32 NASM Release" 
   cd "..\..\..\..\base\win32\prj"

"id3v2 - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\plm\metadata\id3v2\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v2.mak CFG="id3v2 - Win32 NASM Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

"id3v2 - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\plm\metadata\id3v2\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v2.mak CFG="id3v2 - Win32 NASM Debug" 
   cd "..\..\..\..\base\win32\prj"

"id3v2 - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\plm\metadata\id3v2\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\id3v2.mak CFG="id3v2 - Win32 NASM Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\base\win32\prj"

!ENDIF 


!ENDIF 


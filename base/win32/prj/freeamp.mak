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

ALL : ".\freeamp.hlp" ".\freeamp.cnt" ".\themes\FreeAmpClassic.fat" ".\themes\FreeAmp.fat" ".\themes\EMusic.fat" ".\themes\Aquatica.fat" ".\portio.sys" "..\..\..\ReadMe.txt" "..\..\..\Copying.txt" "..\..\..\Changes.txt" "..\..\..\Authors.txt" ".\freeamp.exe"


CLEAN :
	-@erase "$(INTDIR)\freeamp.res"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\Parse.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\freeamp.exe"
	-@erase "..\..\..\Authors.txt"
	-@erase "..\..\..\Changes.txt"
	-@erase "..\..\..\Copying.txt"
	-@erase "..\..\..\ReadMe.txt"
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
	"$(INTDIR)\freeamp.res"

".\freeamp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : ".\freeamp.hlp" ".\freeamp.cnt" ".\themes\FreeAmpClassic.fat" ".\themes\FreeAmp.fat" ".\themes\EMusic.fat" ".\themes\Aquatica.fat" ".\portio.sys" "..\..\..\ReadMe.txt" "..\..\..\Copying.txt" "..\..\..\config\config.h" "..\..\..\Changes.txt" "..\..\..\Authors.txt" ".\freeamp.exe"


CLEAN :
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
	"$(INTDIR)\freeamp.res"

".\freeamp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\freeamp.hlp" ".\freeamp.cnt" ".\themes\FreeAmpClassic.fat" ".\themes\FreeAmp.fat" ".\themes\EMusic.fat" ".\themes\Aquatica.fat" ".\portio.sys" "..\..\..\ReadMe.txt" "..\..\..\Copying.txt" "..\..\..\config\config.h" "..\..\..\Changes.txt" "..\..\..\Authors.txt" ".\freeamp.exe"


CLEAN :
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
	"$(INTDIR)\freeamp.res"

".\freeamp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : ".\freeamp.hlp" ".\freeamp.cnt" ".\themes\FreeAmpClassic.fat" ".\themes\FreeAmp.fat" ".\themes\EMusic.fat" ".\themes\Aquatica.fat" ".\portio.sys" "..\..\..\ReadMe.txt" "..\..\..\Copying.txt" "..\..\..\config\config.h" "..\..\..\Changes.txt" "..\..\..\Authors.txt" ".\freeamp.exe"


CLEAN :
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
	"$(INTDIR)\freeamp.res"

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

"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)" "..\..\..\config\config.h"
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


"$(INTDIR)\freeamp.res" : $(SOURCE) "$(INTDIR)" "..\..\..\config\config.h"
	$(RSC) /l 0x409 /fo"$(INTDIR)\freeamp.res" /i "\Local\src\freeamp\base\win32\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"


"$(INTDIR)\freeamp.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\freeamp.res" /i "\Local\src\freeamp\base\win32\res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"


"$(INTDIR)\freeamp.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\freeamp.res" /i "\Local\src\freeamp\base\win32\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"


"$(INTDIR)\freeamp.res" : $(SOURCE) "$(INTDIR)"
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


!ENDIF 


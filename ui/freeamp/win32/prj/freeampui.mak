# Microsoft Developer Studio Generated NMAKE File, Based on freeampui.dsp
!IF "$(CFG)" == ""
CFG=freeampui - Win32 Debug
!MESSAGE No configuration specified. Defaulting to freeampui - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "freeampui - Win32 Release" && "$(CFG)" !=\
 "freeampui - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "freeampui.mak" CFG="freeampui - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "freeampui - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "freeampui - Win32 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "freeampui - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\freeamp.ui"

!ELSE 

ALL : "xing - Win32 Release" "soundcard - Win32 Release"\
 "fileinput - Win32 Release" ".\freeamp.ui"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fileinput - Win32 ReleaseCLEAN" "soundcard - Win32 ReleaseCLEAN"\
 "xing - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\drawplayer.obj"
	-@erase "$(INTDIR)\freeamp.res"
	-@erase "$(INTDIR)\freeampui.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\playlist.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(OUTDIR)\freeamp.exp"
	-@erase "$(OUTDIR)\freeamp.lib"
	-@erase ".\freeamp.ui"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\res" /I "..\include" /I\
 "..\..\include" /I "..\..\..\include" /I "..\..\..\..\io\include" /I\
 "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I\
 "..\..\..\..\config" /I "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include"\
 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\freeampui.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\freeamp.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\freeampui.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll\
 /incremental:no /pdb:"$(OUTDIR)\freeamp.pdb" /machine:I386\
 /def:".\freeampui.def" /out:"freeamp.ui" /implib:"$(OUTDIR)\freeamp.lib" 
DEF_FILE= \
	".\freeampui.def"
LINK32_OBJS= \
	"$(INTDIR)\drawplayer.obj" \
	"$(INTDIR)\freeamp.res" \
	"$(INTDIR)\freeampui.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\playlist.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\win32thread.obj" \
	"..\..\..\..\io\local\win32\prj\Release\fileinput.lib" \
	"..\..\..\..\io\soundcard\win32\prj\Release\soundcard.lib" \
	"..\..\..\..\lmc\xingmp3\win32\prj\Release\xing.lib"

".\freeamp.ui" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "xing - Win32 Release" "soundcard - Win32 Release"\
 "fileinput - Win32 Release" ".\freeamp.ui"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                          ..\..\..\..\base\win32\prj\plugins
	copy freeamp.ui         ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\freeamp.ui"

!ELSE 

ALL : "xing - Win32 Debug" "soundcard - Win32 Debug" "fileinput - Win32 Debug"\
 ".\freeamp.ui"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fileinput - Win32 DebugCLEAN" "soundcard - Win32 DebugCLEAN"\
 "xing - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\drawplayer.obj"
	-@erase "$(INTDIR)\freeamp.res"
	-@erase "$(INTDIR)\freeampui.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\playlist.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(OUTDIR)\freeamp.exp"
	-@erase "$(OUTDIR)\freeamp.lib"
	-@erase "$(OUTDIR)\freeamp.pdb"
	-@erase ".\freeamp.ilk"
	-@erase ".\freeamp.ui"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\res" /I "..\include" /I\
 "..\..\include" /I "..\..\..\include" /I "..\..\..\..\io\include" /I\
 "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I\
 "..\..\..\..\config" /I "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include"\
 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\freeampui.pch" /YX\
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\freeamp.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\freeampui.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll\
 /incremental:yes /pdb:"$(OUTDIR)\freeamp.pdb" /debug /machine:I386\
 /def:".\freeampui.def" /out:"freeamp.ui" /implib:"$(OUTDIR)\freeamp.lib"\
 /pdbtype:sept 
DEF_FILE= \
	".\freeampui.def"
LINK32_OBJS= \
	"$(INTDIR)\drawplayer.obj" \
	"$(INTDIR)\freeamp.res" \
	"$(INTDIR)\freeampui.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\playlist.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\win32thread.obj" \
	"..\..\..\..\io\local\win32\prj\Debug\fileinput.lib" \
	"..\..\..\..\io\soundcard\win32\prj\Debug\soundcard.lib" \
	"..\..\..\..\lmc\xingmp3\win32\prj\Debug\xing.lib"

".\freeamp.ui" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "xing - Win32 Debug" "soundcard - Win32 Debug"\
 "fileinput - Win32 Debug" ".\freeamp.ui"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                          ..\..\..\..\base\win32\prj\plugins
	copy freeamp.ui         ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(CFG)" == "freeampui - Win32 Release" || "$(CFG)" ==\
 "freeampui - Win32 Debug"
SOURCE=..\..\..\..\config\config.win32

!IF  "$(CFG)" == "freeampui - Win32 Release"

InputPath=..\..\..\..\config\config.win32

"..\..\..\..\config\config.h"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\..\config\config.win32 ..\..\..\..\config\config.h

!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

InputPath=..\..\..\..\config\config.win32

"..\..\..\..\config\config.h"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\..\config\config.win32 ..\..\..\..\config\config.h

!ENDIF 

SOURCE=..\src\drawplayer.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_DRAWP=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\id3v1.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\queue.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\vector.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\..\io\include\pmo.h"\
	"..\..\..\..\lmc\include\lmc.h"\
	"..\..\..\include\ui.h"\
	"..\include\drawplayer.h"\
	"..\include\font_width.h"\
	"..\include\freeampui.h"\
	{$(INCLUDE)}"\mmsystem.h"\
	

"$(INTDIR)\drawplayer.obj" : $(SOURCE) $(DEP_CPP_DRAWP) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_DRAWP=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\id3v1.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\queue.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\vector.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\..\io\include\pmo.h"\
	"..\..\..\..\lmc\include\lmc.h"\
	"..\..\..\include\ui.h"\
	"..\include\drawplayer.h"\
	"..\include\font_width.h"\
	"..\include\freeampui.h"\
	

"$(INTDIR)\drawplayer.obj" : $(SOURCE) $(DEP_CPP_DRAWP) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\res\freeamp.rc
DEP_RSC_FREEA=\
	"..\res\all_icon.bmp"\
	"..\res\all_icon256.bmp"\
	"..\res\arrow.cur"\
	"..\res\big_font.bmp"\
	"..\res\big_font256.bmp"\
	"..\res\close_buttons.bmp"\
	"..\res\close_buttons256.bmp"\
	"..\res\colors.bmp"\
	"..\res\dials.bmp"\
	"..\res\dials256.bmp"\
	"..\res\icon1.ico"\
	"..\res\last_buttons.bmp"\
	"..\res\last_buttons256.bmp"\
	"..\res\lcd.bmp"\
	"..\res\lcd256.bmp"\
	"..\res\leftside.bmp"\
	"..\res\leftside256.bmp"\
	"..\res\logo.bmp"\
	"..\res\logo256.bmp"\
	"..\res\minimize_buttons.bmp"\
	"..\res\minimize_buttons256.bmp"\
	"..\res\mode_buttons.bmp"\
	"..\res\mode_buttons256.bmp"\
	"..\res\next_buttons.bmp"\
	"..\res\next_buttons256.bmp"\
	"..\res\open_buttons.bmp"\
	"..\res\open_buttons256.bmp"\
	"..\res\pause_buttons.bmp"\
	"..\res\pause_buttons256.bmp"\
	"..\res\play_buttons.bmp"\
	"..\res\play_buttons256.bmp"\
	"..\res\player_mask.bmp"\
	"..\res\repeat_buttons.bmp"\
	"..\res\repeat_buttons256.bmp"\
	"..\res\repeat_icon.bmp"\
	"..\res\repeat_icon256.bmp"\
	"..\res\rightside.bmp"\
	"..\res\rightside256.bmp"\
	"..\res\shuffle_buttons.bmp"\
	"..\res\shuffle_buttons256.bmp"\
	"..\res\shuffled_icon.bmp"\
	"..\res\shuffled_icon256.bmp"\
	"..\res\small_font.bmp"\
	"..\res\small_font256.bmp"\
	"..\res\stop_buttons.bmp"\
	"..\res\stop_buttons256.bmp"\
	

!IF  "$(CFG)" == "freeampui - Win32 Release"


"$(INTDIR)\freeamp.res" : $(SOURCE) $(DEP_RSC_FREEA) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\freeamp.res" /i\
 "\Local\src\freeamp\ui\freeamp\win32\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"


"$(INTDIR)\freeamp.res" : $(SOURCE) $(DEP_RSC_FREEA) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\freeamp.res" /i\
 "\Local\src\freeamp\ui\freeamp\win32\res" /d "_DEBUG" $(SOURCE)


!ENDIF 

SOURCE=..\src\freeampui.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_FREEAM=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\id3v1.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\queue.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\vector.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\..\io\include\pmo.h"\
	"..\..\..\..\lmc\include\lmc.h"\
	"..\..\..\include\ui.h"\
	"..\include\drawplayer.h"\
	"..\include\font_width.h"\
	"..\include\freeampui.h"\
	{$(INCLUDE)}"\mmsystem.h"\
	

"$(INTDIR)\freeampui.obj" : $(SOURCE) $(DEP_CPP_FREEAM) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_FREEAM=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\id3v1.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\queue.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\vector.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\..\io\include\pmo.h"\
	"..\..\..\..\lmc\include\lmc.h"\
	"..\..\..\include\ui.h"\
	"..\include\drawplayer.h"\
	"..\include\font_width.h"\
	"..\include\freeampui.h"\
	

"$(INTDIR)\freeampui.obj" : $(SOURCE) $(DEP_CPP_FREEAM) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\base\win32\src\mutex.cpp
DEP_CPP_MUTEX=\
	"..\..\..\..\base\win32\include\mutex.h"\
	

"$(INTDIR)\mutex.obj" : $(SOURCE) $(DEP_CPP_MUTEX) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\base\src\playlist.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_PLAYL=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\id3v1.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\vector.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\..\io\include\pmo.h"\
	"..\..\..\..\lmc\include\lmc.h"\
	

"$(INTDIR)\playlist.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_PLAYL=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\id3v1.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\vector.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\..\io\include\pmo.h"\
	"..\..\..\..\lmc\include\lmc.h"\
	

"$(INTDIR)\playlist.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\base\win32\src\preferences.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_PREFE=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\win32\include\preferences.h"\
	"..\..\..\..\config\config.h"\
	

"$(INTDIR)\preferences.obj" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_PREFE=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\win32\include\preferences.h"\
	"..\..\..\..\config\config.h"\
	

"$(INTDIR)\preferences.obj" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\base\win32\src\semaphore.cpp
DEP_CPP_SEMAP=\
	"..\..\..\..\base\win32\include\semaphore.h"\
	

"$(INTDIR)\semaphore.obj" : $(SOURCE) $(DEP_CPP_SEMAP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\base\src\thread.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_THREA=\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\win32thread.h"\
	"..\..\..\..\config\config.h"\
	
NODEP_CPP_THREA=\
	"..\..\..\..\base\src\linuxthread.h"\
	

"$(INTDIR)\thread.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_THREA=\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\win32thread.h"\
	"..\..\..\..\config\config.h"\
	

"$(INTDIR)\thread.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\base\win32\src\win32thread.cpp
DEP_CPP_WIN32=\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\win32thread.h"\
	"..\..\..\..\config\config.h"\
	

"$(INTDIR)\win32thread.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "freeampui - Win32 Release"

"fileinput - Win32 Release" : 
   cd "\Local\src\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 Release" 
   cd "..\..\..\..\ui\freeamp\win32\prj"

"fileinput - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fileinput.mak\
 CFG="fileinput - Win32 Release" RECURSE=1 
   cd "..\..\..\..\ui\freeamp\win32\prj"

!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

"fileinput - Win32 Debug" : 
   cd "\Local\src\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 Debug" 
   cd "..\..\..\..\ui\freeamp\win32\prj"

"fileinput - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fileinput.mak CFG="fileinput - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\ui\freeamp\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeampui - Win32 Release"

"soundcard - Win32 Release" : 
   cd "\Local\src\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 Release" 
   cd "..\..\..\..\ui\freeamp\win32\prj"

"soundcard - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\soundcard.mak\
 CFG="soundcard - Win32 Release" RECURSE=1 
   cd "..\..\..\..\ui\freeamp\win32\prj"

!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

"soundcard - Win32 Debug" : 
   cd "\Local\src\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 Debug" 
   cd "..\..\..\..\ui\freeamp\win32\prj"

"soundcard - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\soundcard.mak CFG="soundcard - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\ui\freeamp\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeampui - Win32 Release"

"xing - Win32 Release" : 
   cd "\Local\src\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 Release" 
   cd "..\..\..\..\ui\freeamp\win32\prj"

"xing - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\xing.mak CFG="xing - Win32 Release"\
 RECURSE=1 
   cd "..\..\..\..\ui\freeamp\win32\prj"

!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

"xing - Win32 Debug" : 
   cd "\Local\src\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 Debug" 
   cd "..\..\..\..\ui\freeamp\win32\prj"

"xing - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\xing.mak CFG="xing - Win32 Debug" RECURSE=1\
 
   cd "..\..\..\..\ui\freeamp\win32\prj"

!ENDIF 


!ENDIF 


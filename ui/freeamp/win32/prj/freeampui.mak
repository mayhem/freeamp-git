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

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

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
	-@erase "$(INTDIR)\bitmapview.obj"
	-@erase "$(INTDIR)\buttonview.obj"
	-@erase "$(INTDIR)\control.obj"
	-@erase "$(INTDIR)\controlinfo.obj"
	-@erase "$(INTDIR)\dialview.obj"
	-@erase "$(INTDIR)\dib.obj"
	-@erase "$(INTDIR)\fontwidth.obj"
	-@erase "$(INTDIR)\freeamp.res"
	-@erase "$(INTDIR)\freeampui.obj"
	-@erase "$(INTDIR)\listview.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\playlist.obj"
	-@erase "$(INTDIR)\prefdialog.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\registrar.obj"
	-@erase "$(INTDIR)\registry.obj"
	-@erase "$(INTDIR)\renderer.obj"
	-@erase "$(INTDIR)\resizeview.obj"
	-@erase "$(INTDIR)\rio.obj"
	-@erase "$(INTDIR)\scrollview.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\statusview.obj"
	-@erase "$(INTDIR)\stringitem.obj"
	-@erase "$(INTDIR)\textview.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\timeview.obj"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\view.obj"
	-@erase "$(INTDIR)\volumeview.obj"
	-@erase "$(INTDIR)\win32prefs.obj"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(OUTDIR)\freeamp.exp"
	-@erase "$(OUTDIR)\freeamp.lib"
	-@erase ".\freeamp.ui"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\res" /I "..\include" /I\
 "..\..\include" /I "..\..\..\include" /I "..\..\..\..\io\include" /I\
 "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I\
 "..\..\..\..\config" /I "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include"\
 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\freeampui.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
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
	"$(INTDIR)\bitmapview.obj" \
	"$(INTDIR)\buttonview.obj" \
	"$(INTDIR)\control.obj" \
	"$(INTDIR)\controlinfo.obj" \
	"$(INTDIR)\dialview.obj" \
	"$(INTDIR)\dib.obj" \
	"$(INTDIR)\fontwidth.obj" \
	"$(INTDIR)\freeamp.res" \
	"$(INTDIR)\freeampui.obj" \
	"$(INTDIR)\listview.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\playlist.obj" \
	"$(INTDIR)\prefdialog.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\registrar.obj" \
	"$(INTDIR)\registry.obj" \
	"$(INTDIR)\renderer.obj" \
	"$(INTDIR)\resizeview.obj" \
	"$(INTDIR)\rio.obj" \
	"$(INTDIR)\scrollview.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\statusview.obj" \
	"$(INTDIR)\stringitem.obj" \
	"$(INTDIR)\textview.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\timeview.obj" \
	"$(INTDIR)\utility.obj" \
	"$(INTDIR)\view.obj" \
	"$(INTDIR)\volumeview.obj" \
	"$(INTDIR)\win32prefs.obj" \
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
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                                     ..\..\..\..\base\win32\prj\plugins
	copy freeamp.ui                    ..\..\..\..\base\win32\prj\plugins
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
	-@erase "$(INTDIR)\bitmapview.obj"
	-@erase "$(INTDIR)\buttonview.obj"
	-@erase "$(INTDIR)\control.obj"
	-@erase "$(INTDIR)\controlinfo.obj"
	-@erase "$(INTDIR)\dialview.obj"
	-@erase "$(INTDIR)\dib.obj"
	-@erase "$(INTDIR)\fontwidth.obj"
	-@erase "$(INTDIR)\freeamp.res"
	-@erase "$(INTDIR)\freeampui.obj"
	-@erase "$(INTDIR)\listview.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\playlist.obj"
	-@erase "$(INTDIR)\prefdialog.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\registrar.obj"
	-@erase "$(INTDIR)\registry.obj"
	-@erase "$(INTDIR)\renderer.obj"
	-@erase "$(INTDIR)\resizeview.obj"
	-@erase "$(INTDIR)\rio.obj"
	-@erase "$(INTDIR)\scrollview.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\statusview.obj"
	-@erase "$(INTDIR)\stringitem.obj"
	-@erase "$(INTDIR)\textview.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\timeview.obj"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\view.obj"
	-@erase "$(INTDIR)\volumeview.obj"
	-@erase "$(INTDIR)\win32prefs.obj"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(OUTDIR)\freeamp.exp"
	-@erase "$(OUTDIR)\freeamp.lib"
	-@erase "$(OUTDIR)\freeamp.pdb"
	-@erase ".\freeamp.ilk"
	-@erase ".\freeamp.ui"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\res" /I "..\include" /I\
 "..\..\include" /I "..\..\..\include" /I "..\..\..\..\io\include" /I\
 "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I\
 "..\..\..\..\config" /I "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include"\
 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\freeampui.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
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
	"$(INTDIR)\bitmapview.obj" \
	"$(INTDIR)\buttonview.obj" \
	"$(INTDIR)\control.obj" \
	"$(INTDIR)\controlinfo.obj" \
	"$(INTDIR)\dialview.obj" \
	"$(INTDIR)\dib.obj" \
	"$(INTDIR)\fontwidth.obj" \
	"$(INTDIR)\freeamp.res" \
	"$(INTDIR)\freeampui.obj" \
	"$(INTDIR)\listview.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\playlist.obj" \
	"$(INTDIR)\prefdialog.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\registrar.obj" \
	"$(INTDIR)\registry.obj" \
	"$(INTDIR)\renderer.obj" \
	"$(INTDIR)\resizeview.obj" \
	"$(INTDIR)\rio.obj" \
	"$(INTDIR)\scrollview.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\statusview.obj" \
	"$(INTDIR)\stringitem.obj" \
	"$(INTDIR)\textview.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\timeview.obj" \
	"$(INTDIR)\utility.obj" \
	"$(INTDIR)\view.obj" \
	"$(INTDIR)\volumeview.obj" \
	"$(INTDIR)\win32prefs.obj" \
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
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                                     ..\..\..\..\base\win32\prj\plugins
	copy freeamp.ui                    ..\..\..\..\base\win32\prj\plugins
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


!IF "$(CFG)" == "freeampui - Win32 Release" || "$(CFG)" ==\
 "freeampui - Win32 Debug"
SOURCE=..\src\bitmapview.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_BITMA=\
	"..\..\..\..\config\config.h"\
	"..\include\bitmapview.h"\
	"..\include\dib.h"\
	"..\include\renderer.h"\
	"..\include\view.h"\
	

"$(INTDIR)\bitmapview.obj" : $(SOURCE) $(DEP_CPP_BITMA) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_BITMA=\
	"..\..\..\..\config\config.h"\
	"..\include\bitmapview.h"\
	"..\include\dib.h"\
	"..\include\renderer.h"\
	"..\include\view.h"\
	

"$(INTDIR)\bitmapview.obj" : $(SOURCE) $(DEP_CPP_BITMA) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\buttonview.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_BUTTO=\
	"..\..\..\..\config\config.h"\
	"..\include\buttonview.h"\
	"..\include\dib.h"\
	"..\include\renderer.h"\
	"..\include\view.h"\
	

"$(INTDIR)\buttonview.obj" : $(SOURCE) $(DEP_CPP_BUTTO) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_BUTTO=\
	"..\..\..\..\config\config.h"\
	"..\include\buttonview.h"\
	"..\include\dib.h"\
	"..\include\renderer.h"\
	"..\include\view.h"\
	

"$(INTDIR)\buttonview.obj" : $(SOURCE) $(DEP_CPP_BUTTO) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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

SOURCE=..\src\control.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_CONTR=\
	"..\..\..\..\config\config.h"\
	"..\include\control.h"\
	

"$(INTDIR)\control.obj" : $(SOURCE) $(DEP_CPP_CONTR) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_CONTR=\
	"..\..\..\..\config\config.h"\
	"..\include\control.h"\
	

"$(INTDIR)\control.obj" : $(SOURCE) $(DEP_CPP_CONTR) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\controlinfo.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_CONTRO=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\list.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	"..\include\utility.h"\
	

"$(INTDIR)\controlinfo.obj" : $(SOURCE) $(DEP_CPP_CONTRO) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_CONTRO=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\list.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	"..\include\utility.h"\
	

"$(INTDIR)\controlinfo.obj" : $(SOURCE) $(DEP_CPP_CONTRO) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\dialview.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_DIALV=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\id3v1.h"\
	"..\..\..\..\base\include\linkedlist.h"\
	"..\..\..\..\base\include\list.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\queue.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\include\ui.h"\
	"..\include\bitmapview.h"\
	"..\include\buttonview.h"\
	"..\include\control.h"\
	"..\include\controlinfo.h"\
	"..\include\dialview.h"\
	"..\include\dib.h"\
	"..\include\freeampui.h"\
	"..\include\listitem.h"\
	"..\include\listview.h"\
	"..\include\renderer.h"\
	"..\include\resizeview.h"\
	"..\include\scrollview.h"\
	"..\include\statusview.h"\
	"..\include\textview.h"\
	"..\include\timeview.h"\
	"..\include\utility.h"\
	"..\include\view.h"\
	"..\include\volumeview.h"\
	

"$(INTDIR)\dialview.obj" : $(SOURCE) $(DEP_CPP_DIALV) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_DIALV=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\id3v1.h"\
	"..\..\..\..\base\include\linkedlist.h"\
	"..\..\..\..\base\include\list.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\queue.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\..\io\include\std.h"\
	"..\..\..\include\ui.h"\
	"..\include\bitmapview.h"\
	"..\include\buttonview.h"\
	"..\include\control.h"\
	"..\include\controlinfo.h"\
	"..\include\dialview.h"\
	"..\include\dib.h"\
	"..\include\freeampui.h"\
	"..\include\listitem.h"\
	"..\include\listview.h"\
	"..\include\renderer.h"\
	"..\include\resizeview.h"\
	"..\include\scrollview.h"\
	"..\include\statusview.h"\
	"..\include\textview.h"\
	"..\include\timeview.h"\
	"..\include\utility.h"\
	"..\include\view.h"\
	"..\include\volumeview.h"\
	

"$(INTDIR)\dialview.obj" : $(SOURCE) $(DEP_CPP_DIALV) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\dib.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_DIB_C=\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	

"$(INTDIR)\dib.obj" : $(SOURCE) $(DEP_CPP_DIB_C) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_DIB_C=\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	

"$(INTDIR)\dib.obj" : $(SOURCE) $(DEP_CPP_DIB_C) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\fontwidth.cpp
DEP_CPP_FONTW=\
	"..\..\..\..\config\config.h"\
	

"$(INTDIR)\fontwidth.obj" : $(SOURCE) $(DEP_CPP_FONTW) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\res\freeamp.rc
DEP_RSC_FREEA=\
	"..\res\add_buttons.bmp"\
	"..\res\add_buttons256.bmp"\
	"..\res\all_icon.bmp"\
	"..\res\all_icon256.bmp"\
	"..\res\arrow.cur"\
	"..\res\big_font.bmp"\
	"..\res\big_font256.bmp"\
	"..\res\body.bmp"\
	"..\res\body256.bmp"\
	"..\res\close_buttons.bmp"\
	"..\res\close_buttons256.bmp"\
	"..\res\control_mask_mid.bmp"\
	"..\res\delete_buttons.bmp"\
	"..\res\delete_buttons256.bmp"\
	"..\res\dials.bmp"\
	"..\res\dials256.bmp"\
	"..\res\drawer.bmp"\
	"..\res\drawer256.bmp"\
	"..\res\drawer_mask.bmp"\
	"..\res\icon1.ico"\
	"..\res\last_buttons.bmp"\
	"..\res\last_buttons256.bmp"\
	"..\res\load_buttons.bmp"\
	"..\res\load_buttons256.bmp"\
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
	"..\res\panel_backing.bmp"\
	"..\res\panel_backing256.bmp"\
	"..\res\panel_backing_mask.bmp"\
	"..\res\pause_buttons.bmp"\
	"..\res\pause_buttons256.bmp"\
	"..\res\play_buttons.bmp"\
	"..\res\play_buttons256.bmp"\
	"..\res\playlist_back.bmp"\
	"..\res\playlist_back256.bmp"\
	"..\res\playlist_control_mask.bmp"\
	"..\res\repeat_buttons.bmp"\
	"..\res\repeat_buttons256.bmp"\
	"..\res\repeat_icon.bmp"\
	"..\res\repeat_icon256.bmp"\
	"..\res\save_buttons.bmp"\
	"..\res\save_buttons256.bmp"\
	"..\res\scrollbar.bmp"\
	"..\res\scrollbar256.bmp"\
	"..\res\shuffle_buttons.bmp"\
	"..\res\shuffle_buttons256.bmp"\
	"..\res\shuffled_icon.bmp"\
	"..\res\shuffled_icon256.bmp"\
	"..\res\small_font.bmp"\
	"..\res\small_font256.bmp"\
	"..\res\stop_buttons.bmp"\
	"..\res\stop_buttons256.bmp"\
	"..\res\time_background.bmp"\
	"..\res\time_background256.bmp"\
	"..\res\window_mask_mid.bmp"\
	

!IF  "$(CFG)" == "freeampui - Win32 Release"


"$(INTDIR)\freeamp.res" : $(SOURCE) $(DEP_RSC_FREEA) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\freeamp.res" /i\
 "\TEMP\merge\freeamp\ui\freeamp\win32\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"


"$(INTDIR)\freeamp.res" : $(SOURCE) $(DEP_RSC_FREEA) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\freeamp.res" /i\
 "\TEMP\merge\freeamp\ui\freeamp\win32\res" /d "_DEBUG" $(SOURCE)


!ENDIF 

SOURCE=..\src\freeampui.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_FREEAM=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\hashtable.h"\
	"..\..\..\..\base\include\id3v1.h"\
	"..\..\..\..\base\include\linkedlist.h"\
	"..\..\..\..\base\include\list.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\player.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\propimpl.h"\
	"..\..\..\..\base\include\queue.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\..\io\include\pmiregistry.h"\
	"..\..\..\..\io\include\pmo.h"\
	"..\..\..\..\io\include\pmoregistry.h"\
	"..\..\..\..\lmc\include\lmc.h"\
	"..\..\..\..\lmc\include\lmcregistry.h"\
	"..\..\..\include\ui.h"\
	"..\..\..\include\uiregistry.h"\
	"..\include\bitmapview.h"\
	"..\include\buttonview.h"\
	"..\include\control.h"\
	"..\include\controlinfo.h"\
	"..\include\dialview.h"\
	"..\include\dib.h"\
	"..\include\fontwidth.h"\
	"..\include\freeampui.h"\
	"..\include\listitem.h"\
	"..\include\listview.h"\
	"..\include\prefdialog.h"\
	"..\include\renderer.h"\
	"..\include\resizeview.h"\
	"..\include\scrollview.h"\
	"..\include\statusview.h"\
	"..\include\stringitem.h"\
	"..\include\textview.h"\
	"..\include\timeview.h"\
	"..\include\utility.h"\
	"..\include\view.h"\
	"..\include\volumeview.h"\
	

"$(INTDIR)\freeampui.obj" : $(SOURCE) $(DEP_CPP_FREEAM) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_FREEAM=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\hashtable.h"\
	"..\..\..\..\base\include\id3v1.h"\
	"..\..\..\..\base\include\linkedlist.h"\
	"..\..\..\..\base\include\list.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\player.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\propimpl.h"\
	"..\..\..\..\base\include\queue.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\..\io\include\pmiregistry.h"\
	"..\..\..\..\io\include\pmo.h"\
	"..\..\..\..\io\include\pmoregistry.h"\
	"..\..\..\..\io\include\std.h"\
	"..\..\..\..\lmc\include\lmc.h"\
	"..\..\..\..\lmc\include\lmcregistry.h"\
	"..\..\..\include\ui.h"\
	"..\..\..\include\uiregistry.h"\
	"..\include\bitmapview.h"\
	"..\include\buttonview.h"\
	"..\include\control.h"\
	"..\include\controlinfo.h"\
	"..\include\dialview.h"\
	"..\include\dib.h"\
	"..\include\fontwidth.h"\
	"..\include\freeampui.h"\
	"..\include\listitem.h"\
	"..\include\listview.h"\
	"..\include\prefdialog.h"\
	"..\include\renderer.h"\
	"..\include\resizeview.h"\
	"..\include\scrollview.h"\
	"..\include\statusview.h"\
	"..\include\stringitem.h"\
	"..\include\textview.h"\
	"..\include\timeview.h"\
	"..\include\utility.h"\
	"..\include\view.h"\
	"..\include\volumeview.h"\
	{$(INCLUDE)}"\mmsystem.h"\
	

"$(INTDIR)\freeampui.obj" : $(SOURCE) $(DEP_CPP_FREEAM) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\listview.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_LISTV=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\id3v1.h"\
	"..\..\..\..\base\include\linkedlist.h"\
	"..\..\..\..\base\include\list.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\queue.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\include\ui.h"\
	"..\include\bitmapview.h"\
	"..\include\buttonview.h"\
	"..\include\control.h"\
	"..\include\controlinfo.h"\
	"..\include\dialview.h"\
	"..\include\dib.h"\
	"..\include\freeampui.h"\
	"..\include\listitem.h"\
	"..\include\listview.h"\
	"..\include\renderer.h"\
	"..\include\resizeview.h"\
	"..\include\scrollview.h"\
	"..\include\statusview.h"\
	"..\include\textview.h"\
	"..\include\timeview.h"\
	"..\include\utility.h"\
	"..\include\view.h"\
	"..\include\volumeview.h"\
	

"$(INTDIR)\listview.obj" : $(SOURCE) $(DEP_CPP_LISTV) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_LISTV=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\id3v1.h"\
	"..\..\..\..\base\include\linkedlist.h"\
	"..\..\..\..\base\include\list.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\queue.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\..\io\include\std.h"\
	"..\..\..\include\ui.h"\
	"..\include\bitmapview.h"\
	"..\include\buttonview.h"\
	"..\include\control.h"\
	"..\include\controlinfo.h"\
	"..\include\dialview.h"\
	"..\include\dib.h"\
	"..\include\freeampui.h"\
	"..\include\listitem.h"\
	"..\include\listview.h"\
	"..\include\renderer.h"\
	"..\include\resizeview.h"\
	"..\include\scrollview.h"\
	"..\include\statusview.h"\
	"..\include\textview.h"\
	"..\include\timeview.h"\
	"..\include\utility.h"\
	"..\include\view.h"\
	"..\include\volumeview.h"\
	

"$(INTDIR)\listview.obj" : $(SOURCE) $(DEP_CPP_LISTV) "$(INTDIR)"\
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
	"..\..\..\..\base\include\list.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\win32thread.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\..\io\include\rio.h"\
	"..\..\..\..\io\include\std.h"\
	

"$(INTDIR)\playlist.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_PLAYL=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\id3v1.h"\
	"..\..\..\..\base\include\list.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\win32thread.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\..\io\include\rio.h"\
	"..\..\..\..\io\include\std.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\playlist.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\prefdialog.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_PREFD=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\hashtable.h"\
	"..\..\..\..\base\include\id3v1.h"\
	"..\..\..\..\base\include\list.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\player.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\propimpl.h"\
	"..\..\..\..\base\include\queue.h"\
	"..\..\..\..\base\include\registrar.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\..\io\include\pmiregistry.h"\
	"..\..\..\..\io\include\pmo.h"\
	"..\..\..\..\io\include\pmoregistry.h"\
	"..\..\..\..\lmc\include\lmc.h"\
	"..\..\..\..\lmc\include\lmcregistry.h"\
	"..\..\..\include\ui.h"\
	"..\..\..\include\uiregistry.h"\
	"..\include\prefdialog.h"\
	

"$(INTDIR)\prefdialog.obj" : $(SOURCE) $(DEP_CPP_PREFD) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_PREFD=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\hashtable.h"\
	"..\..\..\..\base\include\id3v1.h"\
	"..\..\..\..\base\include\list.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\player.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\propimpl.h"\
	"..\..\..\..\base\include\queue.h"\
	"..\..\..\..\base\include\registrar.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\..\io\include\pmiregistry.h"\
	"..\..\..\..\io\include\pmo.h"\
	"..\..\..\..\io\include\pmoregistry.h"\
	"..\..\..\..\io\include\std.h"\
	"..\..\..\..\lmc\include\lmc.h"\
	"..\..\..\..\lmc\include\lmcregistry.h"\
	"..\..\..\include\ui.h"\
	"..\..\..\include\uiregistry.h"\
	"..\include\prefdialog.h"\
	
NODEP_CPP_PREFD=\
	"..\..\..\..\base\include\win32impl.h"\
	

"$(INTDIR)\prefdialog.obj" : $(SOURCE) $(DEP_CPP_PREFD) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\base\src\registrar.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_REGIS=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\hashtable.h"\
	"..\..\..\..\base\include\list.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\registrar.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\config\config.h"\
	

"$(INTDIR)\registrar.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_REGIS=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\hashtable.h"\
	"..\..\..\..\base\include\list.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\registrar.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\config\config.h"\
	
NODEP_CPP_REGIS=\
	"..\..\..\..\base\include\win32impl.h"\
	

"$(INTDIR)\registrar.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\base\src\registry.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_REGIST=\
	"..\..\..\..\base\include\list.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\config\config.h"\
	

"$(INTDIR)\registry.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_REGIST=\
	"..\..\..\..\base\include\list.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\config\config.h"\
	

"$(INTDIR)\registry.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\renderer.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_RENDE=\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	"..\include\renderer.h"\
	

"$(INTDIR)\renderer.obj" : $(SOURCE) $(DEP_CPP_RENDE) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_RENDE=\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	"..\include\renderer.h"\
	

"$(INTDIR)\renderer.obj" : $(SOURCE) $(DEP_CPP_RENDE) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\resizeview.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_RESIZ=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\id3v1.h"\
	"..\..\..\..\base\include\linkedlist.h"\
	"..\..\..\..\base\include\list.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\queue.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\include\ui.h"\
	"..\include\bitmapview.h"\
	"..\include\buttonview.h"\
	"..\include\control.h"\
	"..\include\controlinfo.h"\
	"..\include\dialview.h"\
	"..\include\dib.h"\
	"..\include\freeampui.h"\
	"..\include\listitem.h"\
	"..\include\listview.h"\
	"..\include\renderer.h"\
	"..\include\resizeview.h"\
	"..\include\scrollview.h"\
	"..\include\statusview.h"\
	"..\include\textview.h"\
	"..\include\timeview.h"\
	"..\include\utility.h"\
	"..\include\view.h"\
	"..\include\volumeview.h"\
	

"$(INTDIR)\resizeview.obj" : $(SOURCE) $(DEP_CPP_RESIZ) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_RESIZ=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\id3v1.h"\
	"..\..\..\..\base\include\linkedlist.h"\
	"..\..\..\..\base\include\list.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\queue.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\..\io\include\std.h"\
	"..\..\..\include\ui.h"\
	"..\include\bitmapview.h"\
	"..\include\buttonview.h"\
	"..\include\control.h"\
	"..\include\controlinfo.h"\
	"..\include\dialview.h"\
	"..\include\dib.h"\
	"..\include\freeampui.h"\
	"..\include\listitem.h"\
	"..\include\listview.h"\
	"..\include\renderer.h"\
	"..\include\resizeview.h"\
	"..\include\scrollview.h"\
	"..\include\statusview.h"\
	"..\include\textview.h"\
	"..\include\timeview.h"\
	"..\include\utility.h"\
	"..\include\view.h"\
	"..\include\volumeview.h"\
	

"$(INTDIR)\resizeview.obj" : $(SOURCE) $(DEP_CPP_RESIZ) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\io\rio\rio.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_RIO_C=\
	"..\..\..\..\io\include\binary.h"\
	"..\..\..\..\io\include\rio.h"\
	"..\..\..\..\io\include\std.h"\
	

"$(INTDIR)\rio.obj" : $(SOURCE) $(DEP_CPP_RIO_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_RIO_C=\
	"..\..\..\..\io\include\binary.h"\
	"..\..\..\..\io\include\rio.h"\
	"..\..\..\..\io\include\rioioctl.h"\
	"..\..\..\..\io\include\std.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\rio.obj" : $(SOURCE) $(DEP_CPP_RIO_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\scrollview.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_SCROL=\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	"..\include\renderer.h"\
	"..\include\scrollview.h"\
	"..\include\view.h"\
	

"$(INTDIR)\scrollview.obj" : $(SOURCE) $(DEP_CPP_SCROL) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_SCROL=\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	"..\include\renderer.h"\
	"..\include\scrollview.h"\
	"..\include\view.h"\
	

"$(INTDIR)\scrollview.obj" : $(SOURCE) $(DEP_CPP_SCROL) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\base\win32\src\semaphore.cpp
DEP_CPP_SEMAP=\
	"..\..\..\..\base\win32\include\semaphore.h"\
	

"$(INTDIR)\semaphore.obj" : $(SOURCE) $(DEP_CPP_SEMAP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\statusview.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_STATU=\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	"..\include\renderer.h"\
	"..\include\statusview.h"\
	"..\include\view.h"\
	

"$(INTDIR)\statusview.obj" : $(SOURCE) $(DEP_CPP_STATU) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_STATU=\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	"..\include\renderer.h"\
	"..\include\statusview.h"\
	"..\include\view.h"\
	

"$(INTDIR)\statusview.obj" : $(SOURCE) $(DEP_CPP_STATU) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\stringitem.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_STRIN=\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	"..\include\listitem.h"\
	"..\include\renderer.h"\
	"..\include\stringitem.h"\
	

"$(INTDIR)\stringitem.obj" : $(SOURCE) $(DEP_CPP_STRIN) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_STRIN=\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	"..\include\listitem.h"\
	"..\include\renderer.h"\
	"..\include\stringitem.h"\
	

"$(INTDIR)\stringitem.obj" : $(SOURCE) $(DEP_CPP_STRIN) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\textview.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_TEXTV=\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	"..\include\renderer.h"\
	"..\include\textview.h"\
	"..\include\view.h"\
	

"$(INTDIR)\textview.obj" : $(SOURCE) $(DEP_CPP_TEXTV) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_TEXTV=\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	"..\include\renderer.h"\
	"..\include\textview.h"\
	"..\include\view.h"\
	

"$(INTDIR)\textview.obj" : $(SOURCE) $(DEP_CPP_TEXTV) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\base\src\thread.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_THREA=\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\win32thread.h"\
	"..\..\..\..\config\config.h"\
	

"$(INTDIR)\thread.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_THREA=\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\win32thread.h"\
	"..\..\..\..\config\config.h"\
	
NODEP_CPP_THREA=\
	"..\..\..\..\base\src\linuxthread.h"\
	"..\..\..\..\base\src\solaristhread.h"\
	

"$(INTDIR)\thread.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\timeview.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_TIMEV=\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	"..\include\renderer.h"\
	"..\include\timeview.h"\
	"..\include\view.h"\
	

"$(INTDIR)\timeview.obj" : $(SOURCE) $(DEP_CPP_TIMEV) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_TIMEV=\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	"..\include\renderer.h"\
	"..\include\timeview.h"\
	"..\include\view.h"\
	

"$(INTDIR)\timeview.obj" : $(SOURCE) $(DEP_CPP_TIMEV) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\utility.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_UTILI=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\list.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	"..\include\utility.h"\
	

"$(INTDIR)\utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_UTILI=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\list.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	"..\include\utility.h"\
	

"$(INTDIR)\utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\view.cpp
DEP_CPP_VIEW_=\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	"..\include\view.h"\
	

"$(INTDIR)\view.obj" : $(SOURCE) $(DEP_CPP_VIEW_) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\volumeview.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_VOLUM=\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	"..\include\renderer.h"\
	"..\include\view.h"\
	"..\include\volumeview.h"\
	

"$(INTDIR)\volumeview.obj" : $(SOURCE) $(DEP_CPP_VOLUM) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_VOLUM=\
	"..\..\..\..\config\config.h"\
	"..\include\dib.h"\
	"..\include\renderer.h"\
	"..\include\view.h"\
	"..\include\volumeview.h"\
	

"$(INTDIR)\volumeview.obj" : $(SOURCE) $(DEP_CPP_VOLUM) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\base\win32\src\win32thread.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_WIN32=\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\win32thread.h"\
	"..\..\..\..\config\config.h"\
	

"$(INTDIR)\win32thread.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_WIN32=\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\win32\include\win32thread.h"\
	"..\..\..\..\config\config.h"\
	

"$(INTDIR)\win32thread.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

!IF  "$(CFG)" == "freeampui - Win32 Release"

"fileinput - Win32 Release" : 
   cd "\TEMP\merge\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 Release" 
   cd "..\..\..\..\ui\freeamp\win32\prj"

"fileinput - Win32 ReleaseCLEAN" : 
   cd "\TEMP\merge\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fileinput.mak\
 CFG="fileinput - Win32 Release" RECURSE=1 
   cd "..\..\..\..\ui\freeamp\win32\prj"

!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

"fileinput - Win32 Debug" : 
   cd "\TEMP\merge\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 Debug" 
   cd "..\..\..\..\ui\freeamp\win32\prj"

"fileinput - Win32 DebugCLEAN" : 
   cd "\TEMP\merge\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fileinput.mak CFG="fileinput - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\ui\freeamp\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeampui - Win32 Release"

"soundcard - Win32 Release" : 
   cd "\TEMP\merge\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 Release" 
   cd "..\..\..\..\ui\freeamp\win32\prj"

"soundcard - Win32 ReleaseCLEAN" : 
   cd "\TEMP\merge\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\soundcard.mak\
 CFG="soundcard - Win32 Release" RECURSE=1 
   cd "..\..\..\..\ui\freeamp\win32\prj"

!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

"soundcard - Win32 Debug" : 
   cd "\TEMP\merge\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 Debug" 
   cd "..\..\..\..\ui\freeamp\win32\prj"

"soundcard - Win32 DebugCLEAN" : 
   cd "\TEMP\merge\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\soundcard.mak CFG="soundcard - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\ui\freeamp\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "freeampui - Win32 Release"

"xing - Win32 Release" : 
   cd "\TEMP\merge\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 Release" 
   cd "..\..\..\..\ui\freeamp\win32\prj"

"xing - Win32 ReleaseCLEAN" : 
   cd "\TEMP\merge\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\xing.mak CFG="xing - Win32 Release"\
 RECURSE=1 
   cd "..\..\..\..\ui\freeamp\win32\prj"

!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

"xing - Win32 Debug" : 
   cd "\TEMP\merge\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 Debug" 
   cd "..\..\..\..\ui\freeamp\win32\prj"

"xing - Win32 DebugCLEAN" : 
   cd "\TEMP\merge\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\xing.mak CFG="xing - Win32 Debug" RECURSE=1\
 
   cd "..\..\..\..\ui\freeamp\win32\prj"

!ENDIF 

SOURCE=..\..\..\..\base\src\preferences.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_PREFE=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\config\config.h"\
	

"$(INTDIR)\preferences.obj" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_PREFE=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\config\config.h"\
	

"$(INTDIR)\preferences.obj" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\base\win32\src\win32prefs.cpp

!IF  "$(CFG)" == "freeampui - Win32 Release"

DEP_CPP_WIN32P=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\win32\include\win32prefs.h"\
	"..\..\..\..\config\config.h"\
	

"$(INTDIR)\win32prefs.obj" : $(SOURCE) $(DEP_CPP_WIN32P) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

DEP_CPP_WIN32P=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\win32\include\win32prefs.h"\
	"..\..\..\..\config\config.h"\
	

"$(INTDIR)\win32prefs.obj" : $(SOURCE) $(DEP_CPP_WIN32P) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 


# Microsoft Developer Studio Generated NMAKE File, Based on Rainplay.dsp
!IF "$(CFG)" == ""
CFG=Rainplay - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Rainplay - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Rainplay - Win32 Release" && "$(CFG)" != "Rainplay - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Rainplay.mak" CFG="Rainplay - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Rainplay - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Rainplay - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Rainplay - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : ".\rainplay.ui" "$(OUTDIR)\Rainplay.pch"

!ELSE 

ALL : "xing - Win32 Release" "soundcard - Win32 Release" "fileinput - Win32 Release" ".\rainplay.ui" "$(OUTDIR)\Rainplay.pch"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fileinput - Win32 ReleaseCLEAN" "soundcard - Win32 ReleaseCLEAN" "xing - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AboutDlg.obj"
	-@erase "$(INTDIR)\active.obj"
	-@erase "$(INTDIR)\BmpSize.obj"
	-@erase "$(INTDIR)\EnumFonts.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\playlist.obj"
	-@erase "$(INTDIR)\PlayListDlg.obj"
	-@erase "$(INTDIR)\Rainplay.obj"
	-@erase "$(INTDIR)\Rainplay.pch"
	-@erase "$(INTDIR)\Rainplay.res"
	-@erase "$(INTDIR)\RainplayDlg.obj"
	-@erase "$(INTDIR)\RainplayUI.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\VisualView.obj"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(OUTDIR)\rainplay.exp"
	-@erase "$(OUTDIR)\rainplay.lib"
	-@erase ".\rainplay.ui"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\..\config" /I "..\include" /I "..\..\base\include" /I "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /D "__FREEAMP_UI__" /Fp"$(INTDIR)\Rainplay.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\Rainplay.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Rainplay.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\rainplay.pdb" /machine:I386 /def:".\Rainplay.def" /out:"rainplay.ui" /implib:"$(OUTDIR)\rainplay.lib" 
DEF_FILE= \
	".\Rainplay.def"
LINK32_OBJS= \
	"$(INTDIR)\AboutDlg.obj" \
	"$(INTDIR)\active.obj" \
	"$(INTDIR)\BmpSize.obj" \
	"$(INTDIR)\EnumFonts.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\playlist.obj" \
	"$(INTDIR)\PlayListDlg.obj" \
	"$(INTDIR)\Rainplay.obj" \
	"$(INTDIR)\RainplayDlg.obj" \
	"$(INTDIR)\RainplayUI.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\VisualView.obj" \
	"$(INTDIR)\win32thread.obj" \
	"$(INTDIR)\Rainplay.res" \
	"..\..\io\local\win32\prj\Release\fileinput.lib" \
	"..\..\io\soundcard\win32\prj\Release\soundcard.lib" \
	"..\..\lmc\xingmp3\win32\prj\Release\xing.lib"

".\rainplay.ui" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "xing - Win32 Release" "soundcard - Win32 Release" "fileinput - Win32 Release" ".\rainplay.ui" "$(OUTDIR)\Rainplay.pch"
   IF NOT EXIST ..\..\base\win32\prj\plugins mkdir                ..\..\base\win32\prj\plugins
	copy rainplay.ui     ..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : ".\rainplay.ui" "$(OUTDIR)\Rainplay.pch"

!ELSE 

ALL : "xing - Win32 Debug" "soundcard - Win32 Debug" "fileinput - Win32 Debug" ".\rainplay.ui" "$(OUTDIR)\Rainplay.pch"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fileinput - Win32 DebugCLEAN" "soundcard - Win32 DebugCLEAN" "xing - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AboutDlg.obj"
	-@erase "$(INTDIR)\active.obj"
	-@erase "$(INTDIR)\BmpSize.obj"
	-@erase "$(INTDIR)\EnumFonts.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\playlist.obj"
	-@erase "$(INTDIR)\PlayListDlg.obj"
	-@erase "$(INTDIR)\Rainplay.obj"
	-@erase "$(INTDIR)\Rainplay.pch"
	-@erase "$(INTDIR)\Rainplay.res"
	-@erase "$(INTDIR)\RainplayDlg.obj"
	-@erase "$(INTDIR)\RainplayUI.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VisualView.obj"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(OUTDIR)\rainplay.exp"
	-@erase "$(OUTDIR)\rainplay.lib"
	-@erase "$(OUTDIR)\rainplay.pdb"
	-@erase ".\rainplay.ilk"
	-@erase ".\rainplay.ui"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /GX /ZI /Od /I "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /I "..\..\config" /I "..\include" /I "..\..\base\include" /D "_DEBUG" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /D "__FREEAMP_UI__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\Rainplay.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Rainplay.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\rainplay.pdb" /debug /machine:I386 /def:".\Rainplay.def" /out:"rainplay.ui" /implib:"$(OUTDIR)\rainplay.lib" /pdbtype:sept 
DEF_FILE= \
	".\Rainplay.def"
LINK32_OBJS= \
	"$(INTDIR)\AboutDlg.obj" \
	"$(INTDIR)\active.obj" \
	"$(INTDIR)\BmpSize.obj" \
	"$(INTDIR)\EnumFonts.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\playlist.obj" \
	"$(INTDIR)\PlayListDlg.obj" \
	"$(INTDIR)\Rainplay.obj" \
	"$(INTDIR)\RainplayDlg.obj" \
	"$(INTDIR)\RainplayUI.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\VisualView.obj" \
	"$(INTDIR)\win32thread.obj" \
	"$(INTDIR)\Rainplay.res" \
	"..\..\io\local\win32\prj\Debug\fileinput.lib" \
	"..\..\io\soundcard\win32\prj\Debug\soundcard.lib" \
	"..\..\lmc\xingmp3\win32\prj\Debug\xing.lib"

".\rainplay.ui" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "xing - Win32 Debug" "soundcard - Win32 Debug" "fileinput - Win32 Debug" ".\rainplay.ui" "$(OUTDIR)\Rainplay.pch"
   IF NOT EXIST ..\..\base\win32\prj\plugins mkdir                ..\..\base\win32\prj\plugins
	copy rainplay.ui     ..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Rainplay.dep")
!INCLUDE "Rainplay.dep"
!ELSE 
!MESSAGE Warning: cannot find "Rainplay.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Rainplay - Win32 Release" || "$(CFG)" == "Rainplay - Win32 Debug"
SOURCE=.\AboutDlg.cpp

"$(INTDIR)\AboutDlg.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\active.cpp

"$(INTDIR)\active.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\BmpSize.cpp

"$(INTDIR)\BmpSize.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\EnumFonts.cpp

"$(INTDIR)\EnumFonts.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=..\..\base\win32\src\mutex.cpp

"$(INTDIR)\mutex.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\base\src\playlist.cpp

"$(INTDIR)\playlist.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\PlayListDlg.cpp

"$(INTDIR)\PlayListDlg.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Rainplay.cpp

"$(INTDIR)\Rainplay.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Rainplay.rc

"$(INTDIR)\Rainplay.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\RainplayDlg.cpp

"$(INTDIR)\RainplayDlg.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RainplayUI.cpp

"$(INTDIR)\RainplayUI.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=..\..\base\win32\src\semaphore.cpp

"$(INTDIR)\semaphore.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\..\config" /I "..\include" /I "..\..\base\include" /I "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /D "__FREEAMP_UI__" /Fp"$(INTDIR)\Rainplay.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Rainplay.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /I "..\..\config" /I "..\include" /I "..\..\base\include" /D "_DEBUG" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /D "__FREEAMP_UI__" /Fp"$(INTDIR)\Rainplay.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Rainplay.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\base\src\thread.cpp

"$(INTDIR)\thread.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\VisualView.cpp

"$(INTDIR)\VisualView.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=..\..\base\win32\src\win32thread.cpp

"$(INTDIR)\win32thread.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "Rainplay - Win32 Release"

"fileinput - Win32 Release" : 
   cd "\Work\GoodNoise\freeamp-snap\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F ".\fileinput.mak" CFG="fileinput - Win32 Release" 
   cd "..\..\..\..\ui\rainplay"

"fileinput - Win32 ReleaseCLEAN" : 
   cd "\Work\GoodNoise\freeamp-snap\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F ".\fileinput.mak" CFG="fileinput - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\ui\rainplay"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

"fileinput - Win32 Debug" : 
   cd "\Work\GoodNoise\freeamp-snap\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F ".\fileinput.mak" CFG="fileinput - Win32 Debug" 
   cd "..\..\..\..\ui\rainplay"

"fileinput - Win32 DebugCLEAN" : 
   cd "\Work\GoodNoise\freeamp-snap\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F ".\fileinput.mak" CFG="fileinput - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\ui\rainplay"

!ENDIF 

!IF  "$(CFG)" == "Rainplay - Win32 Release"

"soundcard - Win32 Release" : 
   cd "\Work\GoodNoise\freeamp-snap\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F ".\soundcard.mak" CFG="soundcard - Win32 Release" 
   cd "..\..\..\..\ui\rainplay"

"soundcard - Win32 ReleaseCLEAN" : 
   cd "\Work\GoodNoise\freeamp-snap\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F ".\soundcard.mak" CFG="soundcard - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\ui\rainplay"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

"soundcard - Win32 Debug" : 
   cd "\Work\GoodNoise\freeamp-snap\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F ".\soundcard.mak" CFG="soundcard - Win32 Debug" 
   cd "..\..\..\..\ui\rainplay"

"soundcard - Win32 DebugCLEAN" : 
   cd "\Work\GoodNoise\freeamp-snap\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F ".\soundcard.mak" CFG="soundcard - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\ui\rainplay"

!ENDIF 

!IF  "$(CFG)" == "Rainplay - Win32 Release"

"xing - Win32 Release" : 
   cd "\Work\GoodNoise\freeamp-snap\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F ".\xing.mak" CFG="xing - Win32 Release" 
   cd "..\..\..\..\ui\rainplay"

"xing - Win32 ReleaseCLEAN" : 
   cd "\Work\GoodNoise\freeamp-snap\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F ".\xing.mak" CFG="xing - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\ui\rainplay"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

"xing - Win32 Debug" : 
   cd "\Work\GoodNoise\freeamp-snap\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F ".\xing.mak" CFG="xing - Win32 Debug" 
   cd "..\..\..\..\ui\rainplay"

"xing - Win32 DebugCLEAN" : 
   cd "\Work\GoodNoise\freeamp-snap\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F ".\xing.mak" CFG="xing - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\ui\rainplay"

!ENDIF 


!ENDIF 


# Microsoft Developer Studio Generated NMAKE File, Based on Rainplay.dsp
!IF "$(CFG)" == ""
CFG=Rainplay - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Rainplay - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Rainplay - Win32 Release" && "$(CFG)" !=\
 "Rainplay - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Rainplay.mak" CFG="Rainplay - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Rainplay - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
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

ALL : ".\Rainplay.ui" "$(OUTDIR)\Rainplay.pch"

!ELSE 

ALL : "xing - Win32 Release" "soundcard - Win32 Release"\
 "fileinput - Win32 Release" ".\Rainplay.ui" "$(OUTDIR)\Rainplay.pch"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fileinput - Win32 ReleaseCLEAN" "soundcard - Win32 ReleaseCLEAN"\
 "xing - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\BmpSize.obj"
	-@erase "$(INTDIR)\Rainplay.obj"
	-@erase "$(INTDIR)\Rainplay.pch"
	-@erase "$(INTDIR)\Rainplay.res"
	-@erase "$(INTDIR)\RainplayDlg.obj"
	-@erase "$(INTDIR)\RainplayUI.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(OUTDIR)\Rainplay.exp"
	-@erase "$(OUTDIR)\Rainplay.lib"
	-@erase ".\Rainplay.ui"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\..\config" /I "..\include" /I\
 "..\..\base\include" /I "..\..\base\win32\include" /I "..\..\lmc\include" /I\
 "..\..\io\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS"\
 /D "_USRDLL" /D "__FREEAMP_UI__" /Fp"$(INTDIR)\Rainplay.pch" /YX\
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
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\Rainplay.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Rainplay.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\Rainplay.pdb" /machine:I386 /def:".\Rainplay.def"\
 /out:"Rainplay.ui" /implib:"$(OUTDIR)\Rainplay.lib" 
DEF_FILE= \
	".\Rainplay.def"
LINK32_OBJS= \
	"$(INTDIR)\BmpSize.obj" \
	"$(INTDIR)\Rainplay.obj" \
	"$(INTDIR)\Rainplay.res" \
	"$(INTDIR)\RainplayDlg.obj" \
	"$(INTDIR)\RainplayUI.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\win32thread.obj" \
	"..\..\io\local\win32\prj\Release\fileinput.lib" \
	"..\..\io\soundcard\win32\prj\Release\soundcard.lib" \
	"..\..\lmc\xingmp3\win32\prj\Release\xing.lib"

".\Rainplay.ui" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "xing - Win32 Release" "soundcard - Win32 Release"\
 "fileinput - Win32 Release" ".\Rainplay.ui" "$(OUTDIR)\Rainplay.pch"
   IF NOT EXIST ..\..\base\win32\prj\plugins mkdir               ..\..\base\win32\prj\plugins
	copy plugins\rainplay.ui    ..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : ".\plugins\rainplay.ui" "$(OUTDIR)\Rainplay.pch"

!ELSE 

ALL : "xing - Win32 Debug" "soundcard - Win32 Debug" "fileinput - Win32 Debug"\
 ".\plugins\rainplay.ui" "$(OUTDIR)\Rainplay.pch"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fileinput - Win32 DebugCLEAN" "soundcard - Win32 DebugCLEAN"\
 "xing - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\BmpSize.obj"
	-@erase "$(INTDIR)\Rainplay.obj"
	-@erase "$(INTDIR)\Rainplay.pch"
	-@erase "$(INTDIR)\Rainplay.res"
	-@erase "$(INTDIR)\RainplayDlg.obj"
	-@erase "$(INTDIR)\RainplayUI.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(OUTDIR)\rainplay.exp"
	-@erase "$(OUTDIR)\rainplay.lib"
	-@erase "$(OUTDIR)\rainplay.pdb"
	-@erase ".\plugins\rainplay.ilk"
	-@erase ".\plugins\rainplay.ui"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /GX /Od /I "..\..\base\win32\include" /I\
 "..\..\lmc\include" /I "..\..\io\include" /I "..\..\config" /I "..\include" /I\
 "..\..\base\include" /D "_DEBUG" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D\
 "_WINDLL" /D "_MBCS" /D "_USRDLL" /D "__FREEAMP_UI__" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /ZI /GZ /c 
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
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\Rainplay.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Rainplay.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)\rainplay.pdb" /debug /machine:I386 /def:".\Rainplay.def"\
 /out:"plugins\rainplay.ui" /implib:"$(OUTDIR)\rainplay.lib" /pdbtype:sept 
DEF_FILE= \
	".\Rainplay.def"
LINK32_OBJS= \
	"$(INTDIR)\BmpSize.obj" \
	"$(INTDIR)\Rainplay.obj" \
	"$(INTDIR)\Rainplay.res" \
	"$(INTDIR)\RainplayDlg.obj" \
	"$(INTDIR)\RainplayUI.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\win32thread.obj" \
	"..\..\io\local\win32\prj\Debug\fileinput.lib" \
	"..\..\io\soundcard\win32\prj\Debug\soundcard.lib" \
	"..\..\lmc\xingmp3\win32\prj\Debug\xing.lib"

".\plugins\rainplay.ui" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "xing - Win32 Debug" "soundcard - Win32 Debug"\
 "fileinput - Win32 Debug" ".\plugins\rainplay.ui" "$(OUTDIR)\Rainplay.pch"
   IF NOT EXIST ..\..\base\win32\prj\plugins mkdir               ..\..\base\win32\prj\plugins
	copy plugins\rainplay.ui    ..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(CFG)" == "Rainplay - Win32 Release" || "$(CFG)" ==\
 "Rainplay - Win32 Debug"
SOURCE=.\BmpSize.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_BMPSI=\
	".\BmpSize.h"\
	".\Rainplay.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\BmpSize.obj" : $(SOURCE) $(DEP_CPP_BMPSI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_BMPSI=\
	".\BmpSize.h"\
	".\Rainplay.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\BmpSize.obj" : $(SOURCE) $(DEP_CPP_BMPSI) "$(INTDIR)"


!ENDIF 

SOURCE=.\Rainplay.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_RAINP=\
	".\BmpSize.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Rainplay.obj" : $(SOURCE) $(DEP_CPP_RAINP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_RAINP=\
	".\BmpSize.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Rainplay.obj" : $(SOURCE) $(DEP_CPP_RAINP) "$(INTDIR)"


!ENDIF 

SOURCE=.\Rainplay.rc
DEP_RSC_RAINPL=\
	".\res\Maindown.bmp"\
	".\res\MainUp.bmp"\
	".\res\Rainplay.ico"\
	".\res\Rainplay.rc2"\
	

"$(INTDIR)\Rainplay.res" : $(SOURCE) $(DEP_RSC_RAINPL) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\RainplayDlg.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_RAINPLA=\
	".\BmpSize.h"\
	".\MainFaceDescribe.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RainplayDlg.obj" : $(SOURCE) $(DEP_CPP_RAINPLA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_RAINPLA=\
	".\BmpSize.h"\
	".\MainFaceDescribe.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RainplayDlg.obj" : $(SOURCE) $(DEP_CPP_RAINPLA) "$(INTDIR)"


!ENDIF 

SOURCE=.\RainplayUI.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_RAINPLAY=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\event.h"\
	"..\..\base\include\eventdata.h"\
	"..\..\base\include\id3v1.h"\
	"..\..\base\include\playlist.h"\
	"..\..\base\include\queue.h"\
	"..\..\base\include\registry.h"\
	"..\..\base\include\thread.h"\
	"..\..\base\include\vector.h"\
	"..\..\base\win32\include\mutex.h"\
	"..\..\base\win32\include\semaphore.h"\
	"..\..\config\config.h"\
	"..\..\io\include\pmi.h"\
	"..\..\io\include\pmo.h"\
	"..\..\lmc\include\lmc.h"\
	"..\include\ui.h"\
	".\BmpSize.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\RainplayUI.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RainplayUI.obj" : $(SOURCE) $(DEP_CPP_RAINPLAY) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_RAINPLAY=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\event.h"\
	"..\..\base\include\eventdata.h"\
	"..\..\base\include\id3v1.h"\
	"..\..\base\include\playlist.h"\
	"..\..\base\include\queue.h"\
	"..\..\base\include\registry.h"\
	"..\..\base\include\thread.h"\
	"..\..\base\include\vector.h"\
	"..\..\base\win32\include\mutex.h"\
	"..\..\base\win32\include\semaphore.h"\
	"..\..\config\config.h"\
	"..\..\io\include\pmi.h"\
	"..\..\io\include\pmo.h"\
	"..\..\lmc\include\lmc.h"\
	"..\include\ui.h"\
	".\BmpSize.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\RainplayUI.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RainplayUI.obj" : $(SOURCE) $(DEP_CPP_RAINPLAY) "$(INTDIR)"


!ENDIF 

SOURCE=..\..\base\win32\src\semaphore.cpp
DEP_CPP_SEMAP=\
	"..\..\base\win32\include\semaphore.h"\
	

"$(INTDIR)\semaphore.obj" : $(SOURCE) $(DEP_CPP_SEMAP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "Rainplay - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\..\config" /I "..\include"\
 /I "..\..\base\include" /I "..\..\base\win32\include" /I "..\..\lmc\include" /I\
 "..\..\io\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS"\
 /D "_USRDLL" /D "__FREEAMP_UI__" /Fp"$(INTDIR)\Rainplay.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Rainplay.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /Od /I "..\..\base\win32\include" /I\
 "..\..\lmc\include" /I "..\..\io\include" /I "..\..\config" /I "..\include" /I\
 "..\..\base\include" /D "_DEBUG" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D\
 "_WINDLL" /D "_MBCS" /D "_USRDLL" /D "__FREEAMP_UI__"\
 /Fp"$(INTDIR)\Rainplay.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /ZI /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Rainplay.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\base\src\thread.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_THREA=\
	"..\..\base\include\thread.h"\
	"..\..\base\win32\include\win32thread.h"\
	"..\..\config\config.h"\
	
NODEP_CPP_THREA=\
	"..\..\base\src\linuxthread.h"\
	

"$(INTDIR)\thread.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_THREA=\
	"..\..\base\include\thread.h"\
	"..\..\base\win32\include\win32thread.h"\
	"..\..\config\config.h"\
	

"$(INTDIR)\thread.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\base\win32\src\win32thread.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_WIN32=\
	"..\..\base\include\thread.h"\
	"..\..\base\win32\include\win32thread.h"\
	"..\..\config\config.h"\
	

"$(INTDIR)\win32thread.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_WIN32=\
	"..\..\base\include\thread.h"\
	"..\..\base\win32\include\win32thread.h"\
	"..\..\config\config.h"\
	

"$(INTDIR)\win32thread.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

!IF  "$(CFG)" == "Rainplay - Win32 Release"

"fileinput - Win32 Release" : 
   cd "\Local\src\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 Release" 
   cd "..\..\..\..\ui\rainplay"

"fileinput - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fileinput.mak\
 CFG="fileinput - Win32 Release" RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

"fileinput - Win32 Debug" : 
   cd "\Local\src\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 Debug" 
   cd "..\..\..\..\ui\rainplay"

"fileinput - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fileinput.mak CFG="fileinput - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ENDIF 

!IF  "$(CFG)" == "Rainplay - Win32 Release"

"soundcard - Win32 Release" : 
   cd "\Local\src\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 Release" 
   cd "..\..\..\..\ui\rainplay"

"soundcard - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\soundcard.mak\
 CFG="soundcard - Win32 Release" RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

"soundcard - Win32 Debug" : 
   cd "\Local\src\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 Debug" 
   cd "..\..\..\..\ui\rainplay"

"soundcard - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\soundcard.mak CFG="soundcard - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ENDIF 

!IF  "$(CFG)" == "Rainplay - Win32 Release"

"xing - Win32 Release" : 
   cd "\Local\src\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 Release" 
   cd "..\..\..\..\ui\rainplay"

"xing - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\xing.mak CFG="xing - Win32 Release"\
 RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

"xing - Win32 Debug" : 
   cd "\Local\src\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 Debug" 
   cd "..\..\..\..\ui\rainplay"

"xing - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\xing.mak CFG="xing - Win32 Debug" RECURSE=1\
 
   cd "..\..\..\..\ui\rainplay"

!ENDIF 


!ENDIF 


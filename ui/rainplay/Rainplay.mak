# Microsoft Developer Studio Generated NMAKE File, Based on Rainplay.dsp
!IF "$(CFG)" == ""
CFG=Rainplay - Win32 NASM Debug
!MESSAGE No configuration specified. Defaulting to Rainplay - Win32 NASM Debug.
!ENDIF 

!IF "$(CFG)" != "Rainplay - Win32 Release" && "$(CFG)" !=\
 "Rainplay - Win32 Debug" && "$(CFG)" != "Rainplay - Win32 NASM Debug" &&\
 "$(CFG)" != "Rainplay - Win32 NASM Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Rainplay.mak" CFG="Rainplay - Win32 NASM Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Rainplay - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Rainplay - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Rainplay - Win32 NASM Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Rainplay - Win32 NASM Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
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

ALL : "..\..\config\config.h" ".\rainplay.ui" "$(OUTDIR)\Rainplay.pch"

!ELSE 

ALL : "xing - Win32 Release" "soundcard - Win32 Release"\
 "fileinput - Win32 Release" "..\..\config\config.h" ".\rainplay.ui"\
 "$(OUTDIR)\Rainplay.pch"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fileinput - Win32 ReleaseCLEAN" "soundcard - Win32 ReleaseCLEAN"\
 "xing - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AboutDlg.obj"
	-@erase "$(INTDIR)\active.obj"
	-@erase "$(INTDIR)\BmpSize.obj"
	-@erase "$(INTDIR)\EnumFonts.obj"
	-@erase "$(INTDIR)\EQDlg.obj"
	-@erase "$(INTDIR)\Fft.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\optionsetupdlg.obj"
	-@erase "$(INTDIR)\playlist.obj"
	-@erase "$(INTDIR)\PlayListDlg.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\PreferencesDlg.obj"
	-@erase "$(INTDIR)\Rainplay.obj"
	-@erase "$(INTDIR)\Rainplay.pch"
	-@erase "$(INTDIR)\Rainplay.res"
	-@erase "$(INTDIR)\RainplayDlg.obj"
	-@erase "$(INTDIR)\RainplayUI.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\VisualView.obj"
	-@erase "$(INTDIR)\win32prefs.obj"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(OUTDIR)\rainplay.exp"
	-@erase "$(OUTDIR)\rainplay.lib"
	-@erase ".\rainplay.ui"
	-@erase "..\..\config\config.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /G6 /MD /W3 /GX /O2 /Op /Ob2 /I "." /I ".\Equalizer" /I\
 ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I ".\Preferences" /I\
 "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /I\
 "..\..\config" /I "..\include" /I "..\..\base\include" /D "NDEBUG" /D "WIN32"\
 /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /Fp"$(INTDIR)\Rainplay.pch"\
 /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Rainplay.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Rainplay.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\rainplay.pdb" /machine:I386 /def:".\Rainplay.def"\
 /out:"rainplay.ui" /implib:"$(OUTDIR)\rainplay.lib" 
DEF_FILE= \
	".\Rainplay.def"
LINK32_OBJS= \
	"$(INTDIR)\AboutDlg.obj" \
	"$(INTDIR)\active.obj" \
	"$(INTDIR)\BmpSize.obj" \
	"$(INTDIR)\EnumFonts.obj" \
	"$(INTDIR)\EQDlg.obj" \
	"$(INTDIR)\Fft.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\optionsetupdlg.obj" \
	"$(INTDIR)\playlist.obj" \
	"$(INTDIR)\PlayListDlg.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\PreferencesDlg.obj" \
	"$(INTDIR)\Rainplay.obj" \
	"$(INTDIR)\Rainplay.res" \
	"$(INTDIR)\RainplayDlg.obj" \
	"$(INTDIR)\RainplayUI.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\VisualView.obj" \
	"$(INTDIR)\win32prefs.obj" \
	"$(INTDIR)\win32thread.obj" \
	"..\..\io\local\win32\prj\Release\fileinput.lib" \
	"..\..\io\soundcard\win32\prj\Release\soundcard.lib" \
	"..\..\lmc\xingmp3\win32\prj\Release\xing.lib"

".\rainplay.ui" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
 "fileinput - Win32 Release" "..\..\config\config.h" ".\rainplay.ui"\
 "$(OUTDIR)\Rainplay.pch"
   IF NOT EXIST ..\..\base\win32\prj\plugins mkdir                            ..\..\base\win32\prj\plugins
	copy rainplay.ui     ..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\..\config\config.h" ".\rainplay.ui" "$(OUTDIR)\Rainplay.pch"

!ELSE 

ALL : "xing - Win32 Debug" "soundcard - Win32 Debug" "fileinput - Win32 Debug"\
 "..\..\config\config.h" ".\rainplay.ui" "$(OUTDIR)\Rainplay.pch"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fileinput - Win32 DebugCLEAN" "soundcard - Win32 DebugCLEAN"\
 "xing - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AboutDlg.obj"
	-@erase "$(INTDIR)\active.obj"
	-@erase "$(INTDIR)\BmpSize.obj"
	-@erase "$(INTDIR)\EnumFonts.obj"
	-@erase "$(INTDIR)\EQDlg.obj"
	-@erase "$(INTDIR)\Fft.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\optionsetupdlg.obj"
	-@erase "$(INTDIR)\playlist.obj"
	-@erase "$(INTDIR)\PlayListDlg.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\PreferencesDlg.obj"
	-@erase "$(INTDIR)\Rainplay.obj"
	-@erase "$(INTDIR)\Rainplay.pch"
	-@erase "$(INTDIR)\Rainplay.res"
	-@erase "$(INTDIR)\RainplayDlg.obj"
	-@erase "$(INTDIR)\RainplayUI.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\VisualView.obj"
	-@erase "$(INTDIR)\win32prefs.obj"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(OUTDIR)\rainplay.exp"
	-@erase "$(OUTDIR)\rainplay.lib"
	-@erase "$(OUTDIR)\rainplay.pdb"
	-@erase ".\rainplay.ilk"
	-@erase ".\rainplay.ui"
	-@erase "..\..\config\config.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /G6 /MDd /W3 /Gm /Gi /GR /GX /Zi /Od /I "." /I ".\Equalizer"\
 /I ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I ".\Preferences" /I\
 "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /I\
 "..\..\config" /I "..\include" /I "..\..\base\include" /D "_DEBUG" /D "_AFXDLL"\
 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL"\
 /Fp"$(INTDIR)\Rainplay.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Rainplay.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Rainplay.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)\rainplay.pdb" /debug /machine:I386 /def:".\Rainplay.def"\
 /out:"rainplay.ui" /implib:"$(OUTDIR)\rainplay.lib" /pdbtype:sept 
DEF_FILE= \
	".\Rainplay.def"
LINK32_OBJS= \
	"$(INTDIR)\AboutDlg.obj" \
	"$(INTDIR)\active.obj" \
	"$(INTDIR)\BmpSize.obj" \
	"$(INTDIR)\EnumFonts.obj" \
	"$(INTDIR)\EQDlg.obj" \
	"$(INTDIR)\Fft.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\optionsetupdlg.obj" \
	"$(INTDIR)\playlist.obj" \
	"$(INTDIR)\PlayListDlg.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\PreferencesDlg.obj" \
	"$(INTDIR)\Rainplay.obj" \
	"$(INTDIR)\Rainplay.res" \
	"$(INTDIR)\RainplayDlg.obj" \
	"$(INTDIR)\RainplayUI.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\VisualView.obj" \
	"$(INTDIR)\win32prefs.obj" \
	"$(INTDIR)\win32thread.obj" \
	"..\..\io\local\win32\prj\Debug\fileinput.lib" \
	"..\..\io\soundcard\win32\prj\Debug\soundcard.lib" \
	"..\..\lmc\xingmp3\win32\prj\Debug\xing.lib"

".\rainplay.ui" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
 "fileinput - Win32 Debug" "..\..\config\config.h" ".\rainplay.ui"\
 "$(OUTDIR)\Rainplay.pch"
   IF NOT EXIST ..\..\base\win32\prj\plugins mkdir                            ..\..\base\win32\prj\plugins
	copy rainplay.ui     ..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 NASM Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\..\config\config.h" ".\rainplay.ui" "$(OUTDIR)\Rainplay.pch"

!ELSE 

ALL : "xing - Win32 NASM Debug" "soundcard - Win32 NASM Debug"\
 "fileinput - Win32 NASM Debug" "..\..\config\config.h" ".\rainplay.ui"\
 "$(OUTDIR)\Rainplay.pch"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fileinput - Win32 NASM DebugCLEAN" "soundcard - Win32 NASM DebugCLEAN"\
 "xing - Win32 NASM DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AboutDlg.obj"
	-@erase "$(INTDIR)\active.obj"
	-@erase "$(INTDIR)\BmpSize.obj"
	-@erase "$(INTDIR)\EnumFonts.obj"
	-@erase "$(INTDIR)\EQDlg.obj"
	-@erase "$(INTDIR)\Fft.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\optionsetupdlg.obj"
	-@erase "$(INTDIR)\playlist.obj"
	-@erase "$(INTDIR)\PlayListDlg.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\PreferencesDlg.obj"
	-@erase "$(INTDIR)\Rainplay.obj"
	-@erase "$(INTDIR)\Rainplay.pch"
	-@erase "$(INTDIR)\Rainplay.res"
	-@erase "$(INTDIR)\RainplayDlg.obj"
	-@erase "$(INTDIR)\RainplayUI.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\VisualView.obj"
	-@erase "$(INTDIR)\win32prefs.obj"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(OUTDIR)\rainplay.exp"
	-@erase "$(OUTDIR)\rainplay.lib"
	-@erase "$(OUTDIR)\rainplay.pdb"
	-@erase ".\rainplay.ilk"
	-@erase ".\rainplay.ui"
	-@erase "..\..\config\config.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /G6 /MDd /W3 /Gm /Gi /GR /GX /Zi /Od /I "." /I ".\Equalizer"\
 /I ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I ".\Preferences" /I\
 "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /I\
 "..\..\config" /I "..\include" /I "..\..\base\include" /D "_DEBUG" /D "_AFXDLL"\
 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL"\
 /Fp"$(INTDIR)\Rainplay.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Rainplay.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Rainplay.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)\rainplay.pdb" /debug /machine:I386 /def:".\Rainplay.def"\
 /out:"rainplay.ui" /implib:"$(OUTDIR)\rainplay.lib" /pdbtype:sept 
DEF_FILE= \
	".\Rainplay.def"
LINK32_OBJS= \
	"$(INTDIR)\AboutDlg.obj" \
	"$(INTDIR)\active.obj" \
	"$(INTDIR)\BmpSize.obj" \
	"$(INTDIR)\EnumFonts.obj" \
	"$(INTDIR)\EQDlg.obj" \
	"$(INTDIR)\Fft.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\optionsetupdlg.obj" \
	"$(INTDIR)\playlist.obj" \
	"$(INTDIR)\PlayListDlg.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\PreferencesDlg.obj" \
	"$(INTDIR)\Rainplay.obj" \
	"$(INTDIR)\Rainplay.res" \
	"$(INTDIR)\RainplayDlg.obj" \
	"$(INTDIR)\RainplayUI.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\VisualView.obj" \
	"$(INTDIR)\win32prefs.obj" \
	"$(INTDIR)\win32thread.obj" \
	"..\..\io\local\win32\prj\Debug\fileinput.lib" \
	"..\..\io\soundcard\win32\prj\Debug\soundcard.lib" \
	"..\..\lmc\xingmp3\win32\prj\Debug\xing.lib"

".\rainplay.ui" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "xing - Win32 NASM Debug" "soundcard - Win32 NASM Debug"\
 "fileinput - Win32 NASM Debug" "..\..\config\config.h" ".\rainplay.ui"\
 "$(OUTDIR)\Rainplay.pch"
   IF NOT EXIST ..\..\base\win32\prj\plugins mkdir                            ..\..\base\win32\prj\plugins
	copy rainplay.ui     ..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 NASM Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\..\config\config.h" ".\rainplay.ui" "$(OUTDIR)\Rainplay.pch"

!ELSE 

ALL : "xing - Win32 NASM Release" "soundcard - Win32 NASM Release"\
 "fileinput - Win32 NASM Release" "..\..\config\config.h" ".\rainplay.ui"\
 "$(OUTDIR)\Rainplay.pch"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fileinput - Win32 NASM ReleaseCLEAN"\
 "soundcard - Win32 NASM ReleaseCLEAN" "xing - Win32 NASM ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AboutDlg.obj"
	-@erase "$(INTDIR)\active.obj"
	-@erase "$(INTDIR)\BmpSize.obj"
	-@erase "$(INTDIR)\EnumFonts.obj"
	-@erase "$(INTDIR)\EQDlg.obj"
	-@erase "$(INTDIR)\Fft.obj"
	-@erase "$(INTDIR)\mutex.obj"
	-@erase "$(INTDIR)\optionsetupdlg.obj"
	-@erase "$(INTDIR)\playlist.obj"
	-@erase "$(INTDIR)\PlayListDlg.obj"
	-@erase "$(INTDIR)\preferences.obj"
	-@erase "$(INTDIR)\PreferencesDlg.obj"
	-@erase "$(INTDIR)\Rainplay.obj"
	-@erase "$(INTDIR)\Rainplay.pch"
	-@erase "$(INTDIR)\Rainplay.res"
	-@erase "$(INTDIR)\RainplayDlg.obj"
	-@erase "$(INTDIR)\RainplayUI.obj"
	-@erase "$(INTDIR)\semaphore.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\thread.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\VisualView.obj"
	-@erase "$(INTDIR)\win32prefs.obj"
	-@erase "$(INTDIR)\win32thread.obj"
	-@erase "$(OUTDIR)\rainplay.exp"
	-@erase "$(OUTDIR)\rainplay.lib"
	-@erase ".\rainplay.ui"
	-@erase "..\..\config\config.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /G6 /MD /W3 /GX /O2 /Op /Ob2 /I "." /I ".\Equalizer" /I\
 ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I ".\Preferences" /I\
 "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /I\
 "..\..\config" /I "..\include" /I "..\..\base\include" /D "NDEBUG" /D "WIN32"\
 /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /Fp"$(INTDIR)\Rainplay.pch"\
 /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Rainplay.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Rainplay.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\rainplay.pdb" /machine:I386 /def:".\Rainplay.def"\
 /out:"rainplay.ui" /implib:"$(OUTDIR)\rainplay.lib" 
DEF_FILE= \
	".\Rainplay.def"
LINK32_OBJS= \
	"$(INTDIR)\AboutDlg.obj" \
	"$(INTDIR)\active.obj" \
	"$(INTDIR)\BmpSize.obj" \
	"$(INTDIR)\EnumFonts.obj" \
	"$(INTDIR)\EQDlg.obj" \
	"$(INTDIR)\Fft.obj" \
	"$(INTDIR)\mutex.obj" \
	"$(INTDIR)\optionsetupdlg.obj" \
	"$(INTDIR)\playlist.obj" \
	"$(INTDIR)\PlayListDlg.obj" \
	"$(INTDIR)\preferences.obj" \
	"$(INTDIR)\PreferencesDlg.obj" \
	"$(INTDIR)\Rainplay.obj" \
	"$(INTDIR)\Rainplay.res" \
	"$(INTDIR)\RainplayDlg.obj" \
	"$(INTDIR)\RainplayUI.obj" \
	"$(INTDIR)\semaphore.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\thread.obj" \
	"$(INTDIR)\VisualView.obj" \
	"$(INTDIR)\win32prefs.obj" \
	"$(INTDIR)\win32thread.obj" \
	"..\..\io\local\win32\prj\Release\fileinput.lib" \
	"..\..\io\soundcard\win32\prj\Release\soundcard.lib" \
	"..\..\lmc\xingmp3\win32\prj\Release\xing.lib"

".\rainplay.ui" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "xing - Win32 NASM Release"\
 "soundcard - Win32 NASM Release" "fileinput - Win32 NASM Release"\
 "..\..\config\config.h" ".\rainplay.ui" "$(OUTDIR)\Rainplay.pch"
   IF NOT EXIST ..\..\base\win32\prj\plugins mkdir                            ..\..\base\win32\prj\plugins
	copy rainplay.ui     ..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(CFG)" == "Rainplay - Win32 Release" || "$(CFG)" ==\
 "Rainplay - Win32 Debug" || "$(CFG)" == "Rainplay - Win32 NASM Debug" ||\
 "$(CFG)" == "Rainplay - Win32 NASM Release"
SOURCE=.\AboutDlg.cpp
DEP_CPP_ABOUT=\
	"..\..\base\win32\include\Mutex.h"\
	".\AboutDlg.h"\
	".\BmpSize.h"\
	".\Rainplay.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\AboutDlg.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


SOURCE=.\active.cpp
DEP_CPP_ACTIV=\
	".\active.h"\
	".\cthread.h"\
	

"$(INTDIR)\active.obj" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"


SOURCE=.\BmpSize.cpp
DEP_CPP_BMPSI=\
	"..\..\base\win32\include\Mutex.h"\
	".\BmpSize.h"\
	".\Rainplay.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\BmpSize.obj" : $(SOURCE) $(DEP_CPP_BMPSI) "$(INTDIR)"


SOURCE=..\..\config\config.win32

!IF  "$(CFG)" == "Rainplay - Win32 Release"

InputPath=..\..\config\config.win32

"..\..\config\config.h"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\config\config.win32 ..\..\config\config.h

!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

InputPath=..\..\config\config.win32

"..\..\config\config.h"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\config\config.win32 ..\..\config\config.h

!ELSEIF  "$(CFG)" == "Rainplay - Win32 NASM Debug"

InputPath=..\..\config\config.win32

"..\..\config\config.h"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\config\config.win32 ..\..\config\config.h

!ELSEIF  "$(CFG)" == "Rainplay - Win32 NASM Release"

InputPath=..\..\config\config.win32

"..\..\config\config.h"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\config\config.win32 ..\..\config\config.h

!ENDIF 

SOURCE=.\EnumFonts.cpp
DEP_CPP_ENUMF=\
	".\StdAfx.h"\
	

"$(INTDIR)\EnumFonts.obj" : $(SOURCE) $(DEP_CPP_ENUMF) "$(INTDIR)"


SOURCE=..\..\base\win32\src\mutex.cpp
DEP_CPP_MUTEX=\
	"..\..\base\win32\include\Mutex.h"\
	

"$(INTDIR)\mutex.obj" : $(SOURCE) $(DEP_CPP_MUTEX) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\base\src\playlist.cpp
DEP_CPP_PLAYL=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\event.h"\
	"..\..\base\include\eventdata.h"\
	"..\..\base\include\facontext.h"\
	"..\..\base\include\log.h"\
	"..\..\base\include\metadata.h"\
	"..\..\base\include\playlist.h"\
	"..\..\base\include\playlistformat.h"\
	"..\..\base\include\plmevent.h"\
	"..\..\base\include\portabledevice.h"\
	"..\..\base\include\preferences.h"\
	"..\..\base\include\registrar.h"\
	"..\..\base\include\registry.h"\
	"..\..\base\include\thread.h"\
	"..\..\base\include\utility.h"\
	"..\..\base\win32\include\Mutex.h"\
	
NODEP_CPP_PLAYL=\
	"..\..\base\include\win32impl.h"\
	

"$(INTDIR)\playlist.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Rainplay.cpp
DEP_CPP_RAINP=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\preferences.h"\
	"..\..\base\win32\include\Mutex.h"\
	".\AboutDlg.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\EQDlg.h"\
	".\PlayListDlg.h"\
	".\Preferences\optionsetupdlg.h"\
	".\Preferences\PreferencesDlg.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	

"$(INTDIR)\Rainplay.obj" : $(SOURCE) $(DEP_CPP_RAINP) "$(INTDIR)"


SOURCE=.\Rainplay.rc
DEP_RSC_RAINPL=\
	".\res\About.bmp"\
	".\res\Maindown.bmp"\
	".\res\MainUp.bmp"\
	".\res\Rainplay.ico"\
	

"$(INTDIR)\Rainplay.res" : $(SOURCE) $(DEP_RSC_RAINPL) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\RainplayDlg.cpp
DEP_CPP_RAINPLA=\
	"..\..\base\include\database.h"\
	"..\..\base\include\downloadformat.h"\
	"..\..\base\include\downloadmanager.h"\
	"..\..\base\include\errors.h"\
	"..\..\base\include\event.h"\
	"..\..\base\include\eventdata.h"\
	"..\..\base\include\facontext.h"\
	"..\..\base\include\hashtable.h"\
	"..\..\base\include\log.h"\
	"..\..\base\include\metadata.h"\
	"..\..\base\include\musicbrowser.h"\
	"..\..\base\include\player.h"\
	"..\..\base\include\playlist.h"\
	"..\..\base\include\playlistformat.h"\
	"..\..\base\include\plmevent.h"\
	"..\..\base\include\portabledevice.h"\
	"..\..\base\include\preferences.h"\
	"..\..\base\include\properties.h"\
	"..\..\base\include\propimpl.h"\
	"..\..\base\include\queue.h"\
	"..\..\base\include\registry.h"\
	"..\..\base\include\thread.h"\
	"..\..\base\include\utility.h"\
	"..\..\base\win32\include\Mutex.h"\
	"..\..\base\win32\include\semaphore.h"\
	"..\..\io\include\pipeline.h"\
	"..\..\io\include\pullbuffer.h"\
	"..\..\lmc\include\lmc.h"\
	"..\include\ui.h"\
	".\AboutDlg.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\EQDlg.h"\
	".\MainFaceDescribe.h"\
	".\PlayListDlg.h"\
	".\Preferences\optionsetupdlg.h"\
	".\Preferences\PreferencesDlg.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\RainplayUI.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	
NODEP_CPP_RAINPLA=\
	"..\..\base\include\gdbm_fa.h"\
	

"$(INTDIR)\RainplayDlg.obj" : $(SOURCE) $(DEP_CPP_RAINPLA) "$(INTDIR)"


SOURCE=.\RainplayUI.cpp
DEP_CPP_RAINPLAY=\
	"..\..\base\include\database.h"\
	"..\..\base\include\downloadformat.h"\
	"..\..\base\include\downloadmanager.h"\
	"..\..\base\include\errors.h"\
	"..\..\base\include\event.h"\
	"..\..\base\include\eventdata.h"\
	"..\..\base\include\facontext.h"\
	"..\..\base\include\hashtable.h"\
	"..\..\base\include\log.h"\
	"..\..\base\include\metadata.h"\
	"..\..\base\include\musicbrowser.h"\
	"..\..\base\include\player.h"\
	"..\..\base\include\playlist.h"\
	"..\..\base\include\playlistformat.h"\
	"..\..\base\include\plmevent.h"\
	"..\..\base\include\portabledevice.h"\
	"..\..\base\include\preferences.h"\
	"..\..\base\include\properties.h"\
	"..\..\base\include\propimpl.h"\
	"..\..\base\include\queue.h"\
	"..\..\base\include\registry.h"\
	"..\..\base\include\thread.h"\
	"..\..\base\include\utility.h"\
	"..\..\base\win32\include\Mutex.h"\
	"..\..\base\win32\include\semaphore.h"\
	"..\..\io\include\pipeline.h"\
	"..\..\io\include\pullbuffer.h"\
	"..\..\lmc\include\lmc.h"\
	"..\include\ui.h"\
	".\AboutDlg.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\EQDlg.h"\
	".\PlayListDlg.h"\
	".\Preferences\optionsetupdlg.h"\
	".\Preferences\PreferencesDlg.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\RainplayUI.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	
NODEP_CPP_RAINPLAY=\
	"..\..\base\include\gdbm_fa.h"\
	

"$(INTDIR)\RainplayUI.obj" : $(SOURCE) $(DEP_CPP_RAINPLAY) "$(INTDIR)"


SOURCE=..\..\base\win32\src\semaphore.cpp
DEP_CPP_SEMAP=\
	"..\..\base\win32\include\semaphore.h"\
	

"$(INTDIR)\semaphore.obj" : $(SOURCE) $(DEP_CPP_SEMAP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "Rainplay - Win32 Release"

CPP_SWITCHES=/nologo /G6 /MD /W3 /GX /O2 /Op /Ob2 /I "." /I ".\Equalizer" /I\
 ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I ".\Preferences" /I\
 "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /I\
 "..\..\config" /I "..\include" /I "..\..\base\include" /D "NDEBUG" /D "WIN32"\
 /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /Fp"$(INTDIR)\Rainplay.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Rainplay.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

CPP_SWITCHES=/nologo /G6 /MDd /W3 /Gm /Gi /GR /GX /Zi /Od /I "." /I\
 ".\Equalizer" /I ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I\
 ".\Preferences" /I "..\..\base\win32\include" /I "..\..\lmc\include" /I\
 "..\..\io\include" /I "..\..\config" /I "..\include" /I "..\..\base\include" /D\
 "_DEBUG" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D\
 "_USRDLL" /Fp"$(INTDIR)\Rainplay.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Rainplay.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Rainplay - Win32 NASM Debug"

CPP_SWITCHES=/nologo /G6 /MDd /W3 /Gm /Gi /GR /GX /Zi /Od /I "." /I\
 ".\Equalizer" /I ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I\
 ".\Preferences" /I "..\..\base\win32\include" /I "..\..\lmc\include" /I\
 "..\..\io\include" /I "..\..\config" /I "..\include" /I "..\..\base\include" /D\
 "_DEBUG" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D\
 "_USRDLL" /Fp"$(INTDIR)\Rainplay.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Rainplay.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Rainplay - Win32 NASM Release"

CPP_SWITCHES=/nologo /G6 /MD /W3 /GX /O2 /Op /Ob2 /I "." /I ".\Equalizer" /I\
 ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I ".\Preferences" /I\
 "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /I\
 "..\..\config" /I "..\include" /I "..\..\base\include" /D "NDEBUG" /D "WIN32"\
 /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /Fp"$(INTDIR)\Rainplay.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Rainplay.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\base\src\thread.cpp
DEP_CPP_THREA=\
	"..\..\base\include\thread.h"\
	"..\..\base\win32\include\win32thread.h"\
	
NODEP_CPP_THREA=\
	"..\..\base\src\beosthread.h"\
	"..\..\base\src\linuxthread.h"\
	"..\..\base\src\solaristhread.h"\
	

"$(INTDIR)\thread.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\base\win32\src\win32thread.cpp
DEP_CPP_WIN32=\
	"..\..\base\include\thread.h"\
	"..\..\base\win32\include\win32thread.h"\
	

"$(INTDIR)\win32thread.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Equalizer\EQDlg.cpp
DEP_CPP_EQDLG=\
	"..\..\base\include\database.h"\
	"..\..\base\include\downloadformat.h"\
	"..\..\base\include\downloadmanager.h"\
	"..\..\base\include\errors.h"\
	"..\..\base\include\event.h"\
	"..\..\base\include\eventdata.h"\
	"..\..\base\include\facontext.h"\
	"..\..\base\include\hashtable.h"\
	"..\..\base\include\log.h"\
	"..\..\base\include\metadata.h"\
	"..\..\base\include\musicbrowser.h"\
	"..\..\base\include\player.h"\
	"..\..\base\include\playlist.h"\
	"..\..\base\include\playlistformat.h"\
	"..\..\base\include\plmevent.h"\
	"..\..\base\include\portabledevice.h"\
	"..\..\base\include\preferences.h"\
	"..\..\base\include\properties.h"\
	"..\..\base\include\propimpl.h"\
	"..\..\base\include\queue.h"\
	"..\..\base\include\registry.h"\
	"..\..\base\include\thread.h"\
	"..\..\base\include\utility.h"\
	"..\..\base\win32\include\Mutex.h"\
	"..\..\base\win32\include\semaphore.h"\
	"..\..\io\include\pipeline.h"\
	"..\..\io\include\pullbuffer.h"\
	"..\..\lmc\include\lmc.h"\
	"..\include\ui.h"\
	".\AboutDlg.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\EQDlg.h"\
	".\Equalizer\EQDlg.h"\
	".\PlayListDlg.h"\
	".\Preferences\optionsetupdlg.h"\
	".\Preferences\PreferencesDlg.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\RainplayUI.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	
NODEP_CPP_EQDLG=\
	"..\..\base\include\gdbm_fa.h"\
	

"$(INTDIR)\EQDlg.obj" : $(SOURCE) $(DEP_CPP_EQDLG) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\PlayList\PlayListDlg.cpp
DEP_CPP_PLAYLI=\
	"..\..\base\include\database.h"\
	"..\..\base\include\downloadformat.h"\
	"..\..\base\include\downloadmanager.h"\
	"..\..\base\include\errors.h"\
	"..\..\base\include\event.h"\
	"..\..\base\include\eventdata.h"\
	"..\..\base\include\facontext.h"\
	"..\..\base\include\hashtable.h"\
	"..\..\base\include\log.h"\
	"..\..\base\include\metadata.h"\
	"..\..\base\include\musicbrowser.h"\
	"..\..\base\include\player.h"\
	"..\..\base\include\playlist.h"\
	"..\..\base\include\playlistformat.h"\
	"..\..\base\include\plmevent.h"\
	"..\..\base\include\portabledevice.h"\
	"..\..\base\include\preferences.h"\
	"..\..\base\include\properties.h"\
	"..\..\base\include\propimpl.h"\
	"..\..\base\include\queue.h"\
	"..\..\base\include\registry.h"\
	"..\..\base\include\thread.h"\
	"..\..\base\include\utility.h"\
	"..\..\base\win32\include\Mutex.h"\
	"..\..\base\win32\include\semaphore.h"\
	"..\..\io\include\pipeline.h"\
	"..\..\io\include\pullbuffer.h"\
	"..\..\lmc\include\lmc.h"\
	"..\include\ui.h"\
	".\AboutDlg.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\EQDlg.h"\
	".\PlayList\PlayListDlg.h"\
	".\PlayListDlg.h"\
	".\Preferences\optionsetupdlg.h"\
	".\Preferences\PreferencesDlg.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\RainplayUI.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	
NODEP_CPP_PLAYLI=\
	"..\..\base\include\gdbm_fa.h"\
	

"$(INTDIR)\PlayListDlg.obj" : $(SOURCE) $(DEP_CPP_PLAYLI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\VisualView\Fft\Fft.cpp
DEP_CPP_FFT_C=\
	"..\..\base\win32\include\Mutex.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	

"$(INTDIR)\Fft.obj" : $(SOURCE) $(DEP_CPP_FFT_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\VisualView\VisualView.cpp
DEP_CPP_VISUA=\
	"..\..\base\win32\include\Mutex.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	

"$(INTDIR)\VisualView.obj" : $(SOURCE) $(DEP_CPP_VISUA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Preferences\optionsetupdlg.cpp
DEP_CPP_OPTIO=\
	".\Preferences\optionsetupdlg.h"\
	".\Rainplay.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "Rainplay - Win32 Release"

CPP_SWITCHES=/nologo /G6 /MD /W3 /GX /O2 /Op /Ob2 /I "." /I ".\Equalizer" /I\
 ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I ".\Preferences" /I\
 "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /I\
 "..\..\config" /I "..\include" /I "..\..\base\include" /I ".." /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL"\
 /Fp"$(INTDIR)\Rainplay.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\optionsetupdlg.obj" : $(SOURCE) $(DEP_CPP_OPTIO) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

CPP_SWITCHES=/nologo /G6 /MDd /W3 /Gm /Gi /GR /GX /Zi /Od /I "." /I\
 ".\Equalizer" /I ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I\
 ".\Preferences" /I "..\..\base\win32\include" /I "..\..\lmc\include" /I\
 "..\..\io\include" /I "..\..\config" /I "..\include" /I "..\..\base\include" /I\
 ".." /D "_DEBUG" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS"\
 /D "_USRDLL" /Fp"$(INTDIR)\Rainplay.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /c 

"$(INTDIR)\optionsetupdlg.obj" : $(SOURCE) $(DEP_CPP_OPTIO) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Rainplay - Win32 NASM Debug"

CPP_SWITCHES=/nologo /G6 /MDd /W3 /Gm /Gi /GR /GX /Zi /Od /I "." /I\
 ".\Equalizer" /I ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I\
 ".\Preferences" /I "..\..\base\win32\include" /I "..\..\lmc\include" /I\
 "..\..\io\include" /I "..\..\config" /I "..\include" /I "..\..\base\include" /I\
 ".." /D "_DEBUG" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS"\
 /D "_USRDLL" /Fp"$(INTDIR)\Rainplay.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /c 

"$(INTDIR)\optionsetupdlg.obj" : $(SOURCE) $(DEP_CPP_OPTIO) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Rainplay - Win32 NASM Release"

CPP_SWITCHES=/nologo /G6 /MD /W3 /GX /O2 /Op /Ob2 /I "." /I ".\Equalizer" /I\
 ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I ".\Preferences" /I\
 "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /I\
 "..\..\config" /I "..\include" /I "..\..\base\include" /I ".." /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL"\
 /Fp"$(INTDIR)\Rainplay.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\optionsetupdlg.obj" : $(SOURCE) $(DEP_CPP_OPTIO) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\base\src\preferences.cpp
DEP_CPP_PREFE=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\preferences.h"\
	

"$(INTDIR)\preferences.obj" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Preferences\PreferencesDlg.cpp
DEP_CPP_PREFER=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\preferences.h"\
	".\Preferences\optionsetupdlg.h"\
	".\Preferences\PreferencesDlg.h"\
	".\Rainplay.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\PreferencesDlg.obj" : $(SOURCE) $(DEP_CPP_PREFER) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\base\win32\src\win32prefs.cpp
DEP_CPP_WIN32P=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\preferences.h"\
	"..\..\base\win32\include\win32prefs.h"\
	

"$(INTDIR)\win32prefs.obj" : $(SOURCE) $(DEP_CPP_WIN32P) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "Rainplay - Win32 Release"

"fileinput - Win32 Release" : 
   cd "\FreeAmp\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 Release" 
   cd "..\..\..\..\ui\rainplay"

"fileinput - Win32 ReleaseCLEAN" : 
   cd "\FreeAmp\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fileinput.mak\
 CFG="fileinput - Win32 Release" RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

"fileinput - Win32 Debug" : 
   cd "\FreeAmp\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 Debug" 
   cd "..\..\..\..\ui\rainplay"

"fileinput - Win32 DebugCLEAN" : 
   cd "\FreeAmp\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fileinput.mak CFG="fileinput - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 NASM Debug"

"fileinput - Win32 NASM Debug" : 
   cd "\FreeAmp\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 NASM Debug" 
   cd "..\..\..\..\ui\rainplay"

"fileinput - Win32 NASM DebugCLEAN" : 
   cd "\FreeAmp\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fileinput.mak\
 CFG="fileinput - Win32 NASM Debug" RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 NASM Release"

"fileinput - Win32 NASM Release" : 
   cd "\FreeAmp\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak\
 CFG="fileinput - Win32 NASM Release" 
   cd "..\..\..\..\ui\rainplay"

"fileinput - Win32 NASM ReleaseCLEAN" : 
   cd "\FreeAmp\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fileinput.mak\
 CFG="fileinput - Win32 NASM Release" RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ENDIF 

!IF  "$(CFG)" == "Rainplay - Win32 Release"

"soundcard - Win32 Release" : 
   cd "\FreeAmp\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 Release" 
   cd "..\..\..\..\ui\rainplay"

"soundcard - Win32 ReleaseCLEAN" : 
   cd "\FreeAmp\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\soundcard.mak\
 CFG="soundcard - Win32 Release" RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

"soundcard - Win32 Debug" : 
   cd "\FreeAmp\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 Debug" 
   cd "..\..\..\..\ui\rainplay"

"soundcard - Win32 DebugCLEAN" : 
   cd "\FreeAmp\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\soundcard.mak CFG="soundcard - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 NASM Debug"

"soundcard - Win32 NASM Debug" : 
   cd "\FreeAmp\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 NASM Debug" 
   cd "..\..\..\..\ui\rainplay"

"soundcard - Win32 NASM DebugCLEAN" : 
   cd "\FreeAmp\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\soundcard.mak\
 CFG="soundcard - Win32 NASM Debug" RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 NASM Release"

"soundcard - Win32 NASM Release" : 
   cd "\FreeAmp\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak\
 CFG="soundcard - Win32 NASM Release" 
   cd "..\..\..\..\ui\rainplay"

"soundcard - Win32 NASM ReleaseCLEAN" : 
   cd "\FreeAmp\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\soundcard.mak\
 CFG="soundcard - Win32 NASM Release" RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ENDIF 

!IF  "$(CFG)" == "Rainplay - Win32 Release"

"xing - Win32 Release" : 
   cd "\FreeAmp\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 Release" 
   cd "..\..\..\..\ui\rainplay"

"xing - Win32 ReleaseCLEAN" : 
   cd "\FreeAmp\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\xing.mak CFG="xing - Win32 Release"\
 RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

"xing - Win32 Debug" : 
   cd "\FreeAmp\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 Debug" 
   cd "..\..\..\..\ui\rainplay"

"xing - Win32 DebugCLEAN" : 
   cd "\FreeAmp\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\xing.mak CFG="xing - Win32 Debug" RECURSE=1\
 
   cd "..\..\..\..\ui\rainplay"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 NASM Debug"

"xing - Win32 NASM Debug" : 
   cd "\FreeAmp\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 NASM Debug" 
   cd "..\..\..\..\ui\rainplay"

"xing - Win32 NASM DebugCLEAN" : 
   cd "\FreeAmp\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\xing.mak CFG="xing - Win32 NASM Debug"\
 RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 NASM Release"

"xing - Win32 NASM Release" : 
   cd "\FreeAmp\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 NASM Release" 
   cd "..\..\..\..\ui\rainplay"

"xing - Win32 NASM ReleaseCLEAN" : 
   cd "\FreeAmp\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\xing.mak CFG="xing - Win32 NASM Release"\
 RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ENDIF 


!ENDIF 


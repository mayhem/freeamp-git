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

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Rainplay - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : ".\rainplay.ui" "$(OUTDIR)\Rainplay.pch"

!ELSE 

ALL : "xing - Win32 Release" "soundcard - Win32 Release"\
 "fileinput - Win32 Release" ".\rainplay.ui" "$(OUTDIR)\Rainplay.pch"

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
	-@erase "$(INTDIR)\rio.obj"
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

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G6 /MT /W3 /GX /O2 /Op /Ob2 /I "." /I ".\Equalizer" /I\
 ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I ".\Preferences" /I\
 "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /I\
 "..\..\config" /I "..\include" /I "..\..\base\include" /D "NDEBUG" /D "WIN32"\
 /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /Fp"$(INTDIR)\Rainplay.pch"\
 /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
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
	"$(INTDIR)\rio.obj" \
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
 "fileinput - Win32 Release" ".\rainplay.ui" "$(OUTDIR)\Rainplay.pch"
   IF NOT EXIST ..\..\base\win32\prj\plugins mkdir                         ..\..\base\win32\prj\plugins
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

ALL : "xing - Win32 Debug" "soundcard - Win32 Debug" "fileinput - Win32 Debug"\
 ".\rainplay.ui" "$(OUTDIR)\Rainplay.pch"

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
	-@erase "$(INTDIR)\rio.obj"
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

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G6 /MDd /W3 /Gm /Gi /GR /GX /Zi /Od /I "." /I ".\Equalizer"\
 /I ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I ".\Preferences" /I\
 "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /I\
 "..\..\config" /I "..\include" /I "..\..\base\include" /D "_DEBUG" /D "_AFXDLL"\
 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL"\
 /Fp"$(INTDIR)\Rainplay.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /ZI /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
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
	"$(INTDIR)\rio.obj" \
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
 "fileinput - Win32 Debug" ".\rainplay.ui" "$(OUTDIR)\Rainplay.pch"
   IF NOT EXIST ..\..\base\win32\prj\plugins mkdir                         ..\..\base\win32\prj\plugins
	copy rainplay.ui     ..\..\base\win32\prj\plugins
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


!IF "$(CFG)" == "Rainplay - Win32 Release" || "$(CFG)" ==\
 "Rainplay - Win32 Debug"
SOURCE=.\AboutDlg.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_ABOUT=\
	"..\..\base\win32\include\mutex.h"\
	".\AboutDlg.h"\
	".\BmpSize.h"\
	".\Rainplay.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\AboutDlg.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_ABOUT=\
	"..\..\base\win32\include\mutex.h"\
	".\AboutDlg.h"\
	".\BmpSize.h"\
	".\Rainplay.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\AboutDlg.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ENDIF 

SOURCE=.\active.cpp
DEP_CPP_ACTIV=\
	".\active.h"\
	".\cthread.h"\
	

"$(INTDIR)\active.obj" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"


SOURCE=.\BmpSize.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_BMPSI=\
	"..\..\base\win32\include\mutex.h"\
	".\BmpSize.h"\
	".\Rainplay.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\BmpSize.obj" : $(SOURCE) $(DEP_CPP_BMPSI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_BMPSI=\
	"..\..\base\win32\include\mutex.h"\
	".\BmpSize.h"\
	".\Rainplay.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\BmpSize.obj" : $(SOURCE) $(DEP_CPP_BMPSI) "$(INTDIR)"


!ENDIF 

SOURCE=..\..\config\config.win32

!IF  "$(CFG)" == "Rainplay - Win32 Release"

InputPath=..\..\config\config.win32

"..\..\config\config.h"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\config\config.win32 ..\..\config\config.h

!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

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
	"..\..\base\win32\include\mutex.h"\
	

"$(INTDIR)\mutex.obj" : $(SOURCE) $(DEP_CPP_MUTEX) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\base\src\playlist.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_PLAYL=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\event.h"\
	"..\..\base\include\eventdata.h"\
	"..\..\base\include\id3v1.h"\
	"..\..\base\include\list.h"\
	"..\..\base\include\log.h"\
	"..\..\base\include\playlist.h"\
	"..\..\base\include\properties.h"\
	"..\..\base\include\registry.h"\
	"..\..\base\include\thread.h"\
	"..\..\base\win32\include\mutex.h"\
	"..\..\base\win32\include\win32thread.h"\
	"..\..\config\config.h"\
	"..\..\io\include\pmi.h"\
	"..\..\io\include\rio.h"\
	"..\..\io\include\std.h"\
	

"$(INTDIR)\playlist.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"\
 "..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_PLAYL=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\event.h"\
	"..\..\base\include\eventdata.h"\
	"..\..\base\include\id3v1.h"\
	"..\..\base\include\list.h"\
	"..\..\base\include\log.h"\
	"..\..\base\include\playlist.h"\
	"..\..\base\include\properties.h"\
	"..\..\base\include\registry.h"\
	"..\..\base\include\thread.h"\
	"..\..\base\win32\include\mutex.h"\
	"..\..\base\win32\include\win32thread.h"\
	"..\..\config\config.h"\
	"..\..\io\include\pmi.h"\
	"..\..\io\include\rio.h"\
	"..\..\io\include\std.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\playlist.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"\
 "..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Rainplay.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_RAINP=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\preferences.h"\
	"..\..\base\win32\include\mutex.h"\
	"..\..\config\config.h"\
	".\AboutDlg.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\eqdlg.h"\
	".\playlistdlg.h"\
	".\Preferences\optionsetupdlg.h"\
	".\Preferences\PreferencesDlg.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	

"$(INTDIR)\Rainplay.obj" : $(SOURCE) $(DEP_CPP_RAINP) "$(INTDIR)"\
 "..\..\config\config.h"


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_RAINP=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\preferences.h"\
	"..\..\base\win32\include\mutex.h"\
	"..\..\config\config.h"\
	".\AboutDlg.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\eqdlg.h"\
	".\playlistdlg.h"\
	".\Preferences\optionsetupdlg.h"\
	".\Preferences\PreferencesDlg.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	

"$(INTDIR)\Rainplay.obj" : $(SOURCE) $(DEP_CPP_RAINP) "$(INTDIR)"\
 "..\..\config\config.h"


!ENDIF 

SOURCE=.\Rainplay.rc
DEP_RSC_RAINPL=\
	".\res\About.bmp"\
	".\res\Maindown.bmp"\
	".\res\MainUp.bmp"\
	".\res\Rainplay.ico"\
	

"$(INTDIR)\Rainplay.res" : $(SOURCE) $(DEP_RSC_RAINPL) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\RainplayDlg.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_RAINPLA=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\event.h"\
	"..\..\base\include\eventdata.h"\
	"..\..\base\include\facontext.h"\
	"..\..\base\include\hashtable.h"\
	"..\..\base\include\id3v1.h"\
	"..\..\base\include\list.h"\
	"..\..\base\include\log.h"\
	"..\..\base\include\player.h"\
	"..\..\base\include\playlist.h"\
	"..\..\base\include\preferences.h"\
	"..\..\base\include\properties.h"\
	"..\..\base\include\propimpl.h"\
	"..\..\base\include\queue.h"\
	"..\..\base\include\registry.h"\
	"..\..\base\include\thread.h"\
	"..\..\base\win32\include\mutex.h"\
	"..\..\base\win32\include\semaphore.h"\
	"..\..\config\config.h"\
	"..\..\io\include\pmi.h"\
	"..\..\io\include\pmiregistry.h"\
	"..\..\io\include\pmo.h"\
	"..\..\io\include\pmoregistry.h"\
	"..\..\lmc\include\lmc.h"\
	"..\..\lmc\include\lmcregistry.h"\
	"..\include\ui.h"\
	"..\include\uiregistry.h"\
	".\AboutDlg.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\eqdlg.h"\
	".\MainFaceDescribe.h"\
	".\playlistdlg.h"\
	".\Preferences\optionsetupdlg.h"\
	".\Preferences\PreferencesDlg.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\RainplayUI.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	

"$(INTDIR)\RainplayDlg.obj" : $(SOURCE) $(DEP_CPP_RAINPLA) "$(INTDIR)"\
 "..\..\config\config.h"


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_RAINPLA=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\event.h"\
	"..\..\base\include\eventdata.h"\
	"..\..\base\include\facontext.h"\
	"..\..\base\include\hashtable.h"\
	"..\..\base\include\id3v1.h"\
	"..\..\base\include\list.h"\
	"..\..\base\include\log.h"\
	"..\..\base\include\player.h"\
	"..\..\base\include\playlist.h"\
	"..\..\base\include\preferences.h"\
	"..\..\base\include\properties.h"\
	"..\..\base\include\propimpl.h"\
	"..\..\base\include\queue.h"\
	"..\..\base\include\registry.h"\
	"..\..\base\include\thread.h"\
	"..\..\base\win32\include\mutex.h"\
	"..\..\base\win32\include\semaphore.h"\
	"..\..\config\config.h"\
	"..\..\io\include\pmi.h"\
	"..\..\io\include\pmiregistry.h"\
	"..\..\io\include\pmo.h"\
	"..\..\io\include\pmoregistry.h"\
	"..\..\io\include\std.h"\
	"..\..\lmc\include\lmc.h"\
	"..\..\lmc\include\lmcregistry.h"\
	"..\include\ui.h"\
	"..\include\uiregistry.h"\
	".\AboutDlg.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\eqdlg.h"\
	".\MainFaceDescribe.h"\
	".\playlistdlg.h"\
	".\Preferences\optionsetupdlg.h"\
	".\Preferences\PreferencesDlg.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\RainplayUI.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	

"$(INTDIR)\RainplayDlg.obj" : $(SOURCE) $(DEP_CPP_RAINPLA) "$(INTDIR)"\
 "..\..\config\config.h"


!ENDIF 

SOURCE=.\RainplayUI.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_RAINPLAY=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\event.h"\
	"..\..\base\include\eventdata.h"\
	"..\..\base\include\facontext.h"\
	"..\..\base\include\hashtable.h"\
	"..\..\base\include\id3v1.h"\
	"..\..\base\include\list.h"\
	"..\..\base\include\log.h"\
	"..\..\base\include\player.h"\
	"..\..\base\include\playlist.h"\
	"..\..\base\include\preferences.h"\
	"..\..\base\include\properties.h"\
	"..\..\base\include\propimpl.h"\
	"..\..\base\include\queue.h"\
	"..\..\base\include\registry.h"\
	"..\..\base\include\thread.h"\
	"..\..\base\win32\include\mutex.h"\
	"..\..\base\win32\include\semaphore.h"\
	"..\..\config\config.h"\
	"..\..\io\include\pmi.h"\
	"..\..\io\include\pmiregistry.h"\
	"..\..\io\include\pmo.h"\
	"..\..\io\include\pmoregistry.h"\
	"..\..\lmc\include\lmc.h"\
	"..\..\lmc\include\lmcregistry.h"\
	"..\include\ui.h"\
	"..\include\uiregistry.h"\
	".\AboutDlg.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\eqdlg.h"\
	".\playlistdlg.h"\
	".\Preferences\optionsetupdlg.h"\
	".\Preferences\PreferencesDlg.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\RainplayUI.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	

"$(INTDIR)\RainplayUI.obj" : $(SOURCE) $(DEP_CPP_RAINPLAY) "$(INTDIR)"\
 "..\..\config\config.h"


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_RAINPLAY=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\event.h"\
	"..\..\base\include\eventdata.h"\
	"..\..\base\include\facontext.h"\
	"..\..\base\include\hashtable.h"\
	"..\..\base\include\id3v1.h"\
	"..\..\base\include\list.h"\
	"..\..\base\include\log.h"\
	"..\..\base\include\player.h"\
	"..\..\base\include\playlist.h"\
	"..\..\base\include\preferences.h"\
	"..\..\base\include\properties.h"\
	"..\..\base\include\propimpl.h"\
	"..\..\base\include\queue.h"\
	"..\..\base\include\registry.h"\
	"..\..\base\include\thread.h"\
	"..\..\base\win32\include\mutex.h"\
	"..\..\base\win32\include\semaphore.h"\
	"..\..\config\config.h"\
	"..\..\io\include\pmi.h"\
	"..\..\io\include\pmiregistry.h"\
	"..\..\io\include\pmo.h"\
	"..\..\io\include\pmoregistry.h"\
	"..\..\io\include\std.h"\
	"..\..\lmc\include\lmc.h"\
	"..\..\lmc\include\lmcregistry.h"\
	"..\include\ui.h"\
	"..\include\uiregistry.h"\
	".\AboutDlg.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\eqdlg.h"\
	".\playlistdlg.h"\
	".\Preferences\optionsetupdlg.h"\
	".\Preferences\PreferencesDlg.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\RainplayUI.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	

"$(INTDIR)\RainplayUI.obj" : $(SOURCE) $(DEP_CPP_RAINPLAY) "$(INTDIR)"\
 "..\..\config\config.h"


!ENDIF 

SOURCE=..\..\io\rio\rio.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_RIO_C=\
	"..\..\io\include\binary.h"\
	"..\..\io\include\rio.h"\
	"..\..\io\include\std.h"\
	

"$(INTDIR)\rio.obj" : $(SOURCE) $(DEP_CPP_RIO_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_RIO_C=\
	"..\..\io\include\binary.h"\
	"..\..\io\include\rio.h"\
	"..\..\io\include\rioioctl.h"\
	"..\..\io\include\std.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\rio.obj" : $(SOURCE) $(DEP_CPP_RIO_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


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

CPP_SWITCHES=/nologo /G6 /MT /W3 /GX /O2 /Op /Ob2 /I "." /I ".\Equalizer" /I\
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
 /Fd"$(INTDIR)\\" /FD /ZI /c 

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
	

"$(INTDIR)\thread.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"\
 "..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_THREA=\
	"..\..\base\include\thread.h"\
	"..\..\base\win32\include\win32thread.h"\
	"..\..\config\config.h"\
	
NODEP_CPP_THREA=\
	"..\..\base\src\linuxthread.h"\
	"..\..\base\src\solaristhread.h"\
	

"$(INTDIR)\thread.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"\
 "..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\base\win32\src\win32thread.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_WIN32=\
	"..\..\base\include\thread.h"\
	"..\..\base\win32\include\win32thread.h"\
	"..\..\config\config.h"\
	

"$(INTDIR)\win32thread.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"\
 "..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_WIN32=\
	"..\..\base\include\thread.h"\
	"..\..\base\win32\include\win32thread.h"\
	"..\..\config\config.h"\
	

"$(INTDIR)\win32thread.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"\
 "..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Equalizer\EQDlg.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_EQDLG=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\event.h"\
	"..\..\base\include\eventdata.h"\
	"..\..\base\include\facontext.h"\
	"..\..\base\include\hashtable.h"\
	"..\..\base\include\id3v1.h"\
	"..\..\base\include\list.h"\
	"..\..\base\include\log.h"\
	"..\..\base\include\player.h"\
	"..\..\base\include\playlist.h"\
	"..\..\base\include\preferences.h"\
	"..\..\base\include\properties.h"\
	"..\..\base\include\propimpl.h"\
	"..\..\base\include\queue.h"\
	"..\..\base\include\registry.h"\
	"..\..\base\include\thread.h"\
	"..\..\base\win32\include\mutex.h"\
	"..\..\base\win32\include\semaphore.h"\
	"..\..\config\config.h"\
	"..\..\io\include\pmi.h"\
	"..\..\io\include\pmiregistry.h"\
	"..\..\io\include\pmo.h"\
	"..\..\io\include\pmoregistry.h"\
	"..\..\lmc\include\lmc.h"\
	"..\..\lmc\include\lmcregistry.h"\
	"..\include\ui.h"\
	"..\include\uiregistry.h"\
	".\AboutDlg.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\eqdlg.h"\
	".\Equalizer\EQDlg.h"\
	".\playlistdlg.h"\
	".\Preferences\optionsetupdlg.h"\
	".\Preferences\PreferencesDlg.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\RainplayUI.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	

"$(INTDIR)\EQDlg.obj" : $(SOURCE) $(DEP_CPP_EQDLG) "$(INTDIR)"\
 "..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_EQDLG=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\event.h"\
	"..\..\base\include\eventdata.h"\
	"..\..\base\include\facontext.h"\
	"..\..\base\include\hashtable.h"\
	"..\..\base\include\id3v1.h"\
	"..\..\base\include\list.h"\
	"..\..\base\include\log.h"\
	"..\..\base\include\player.h"\
	"..\..\base\include\playlist.h"\
	"..\..\base\include\preferences.h"\
	"..\..\base\include\properties.h"\
	"..\..\base\include\propimpl.h"\
	"..\..\base\include\queue.h"\
	"..\..\base\include\registry.h"\
	"..\..\base\include\thread.h"\
	"..\..\base\win32\include\mutex.h"\
	"..\..\base\win32\include\semaphore.h"\
	"..\..\config\config.h"\
	"..\..\io\include\pmi.h"\
	"..\..\io\include\pmiregistry.h"\
	"..\..\io\include\pmo.h"\
	"..\..\io\include\pmoregistry.h"\
	"..\..\io\include\std.h"\
	"..\..\lmc\include\lmc.h"\
	"..\..\lmc\include\lmcregistry.h"\
	"..\include\ui.h"\
	"..\include\uiregistry.h"\
	".\AboutDlg.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\eqdlg.h"\
	".\Equalizer\EQDlg.h"\
	".\playlistdlg.h"\
	".\Preferences\optionsetupdlg.h"\
	".\Preferences\PreferencesDlg.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\RainplayUI.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	

"$(INTDIR)\EQDlg.obj" : $(SOURCE) $(DEP_CPP_EQDLG) "$(INTDIR)"\
 "..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\PlayList\PlayListDlg.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_PLAYLI=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\event.h"\
	"..\..\base\include\eventdata.h"\
	"..\..\base\include\facontext.h"\
	"..\..\base\include\hashtable.h"\
	"..\..\base\include\id3v1.h"\
	"..\..\base\include\list.h"\
	"..\..\base\include\log.h"\
	"..\..\base\include\player.h"\
	"..\..\base\include\playlist.h"\
	"..\..\base\include\preferences.h"\
	"..\..\base\include\properties.h"\
	"..\..\base\include\propimpl.h"\
	"..\..\base\include\queue.h"\
	"..\..\base\include\registry.h"\
	"..\..\base\include\thread.h"\
	"..\..\base\win32\include\mutex.h"\
	"..\..\base\win32\include\semaphore.h"\
	"..\..\config\config.h"\
	"..\..\io\include\pmi.h"\
	"..\..\io\include\pmiregistry.h"\
	"..\..\io\include\pmo.h"\
	"..\..\io\include\pmoregistry.h"\
	"..\..\lmc\include\lmc.h"\
	"..\..\lmc\include\lmcregistry.h"\
	"..\include\ui.h"\
	"..\include\uiregistry.h"\
	".\AboutDlg.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\eqdlg.h"\
	".\PlayList\PlayListDlg.h"\
	".\playlistdlg.h"\
	".\Preferences\optionsetupdlg.h"\
	".\Preferences\PreferencesDlg.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\RainplayUI.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	

"$(INTDIR)\PlayListDlg.obj" : $(SOURCE) $(DEP_CPP_PLAYLI) "$(INTDIR)"\
 "..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_PLAYLI=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\event.h"\
	"..\..\base\include\eventdata.h"\
	"..\..\base\include\facontext.h"\
	"..\..\base\include\hashtable.h"\
	"..\..\base\include\id3v1.h"\
	"..\..\base\include\list.h"\
	"..\..\base\include\log.h"\
	"..\..\base\include\player.h"\
	"..\..\base\include\playlist.h"\
	"..\..\base\include\preferences.h"\
	"..\..\base\include\properties.h"\
	"..\..\base\include\propimpl.h"\
	"..\..\base\include\queue.h"\
	"..\..\base\include\registry.h"\
	"..\..\base\include\thread.h"\
	"..\..\base\win32\include\mutex.h"\
	"..\..\base\win32\include\semaphore.h"\
	"..\..\config\config.h"\
	"..\..\io\include\pmi.h"\
	"..\..\io\include\pmiregistry.h"\
	"..\..\io\include\pmo.h"\
	"..\..\io\include\pmoregistry.h"\
	"..\..\io\include\std.h"\
	"..\..\lmc\include\lmc.h"\
	"..\..\lmc\include\lmcregistry.h"\
	"..\include\ui.h"\
	"..\include\uiregistry.h"\
	".\AboutDlg.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\eqdlg.h"\
	".\PlayList\PlayListDlg.h"\
	".\playlistdlg.h"\
	".\Preferences\optionsetupdlg.h"\
	".\Preferences\PreferencesDlg.h"\
	".\Rainplay.h"\
	".\RainplayDlg.h"\
	".\RainplayUI.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	

"$(INTDIR)\PlayListDlg.obj" : $(SOURCE) $(DEP_CPP_PLAYLI) "$(INTDIR)"\
 "..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\VisualView\Fft\Fft.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_FFT_C=\
	"..\..\base\win32\include\mutex.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	

"$(INTDIR)\Fft.obj" : $(SOURCE) $(DEP_CPP_FFT_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_FFT_C=\
	"..\..\base\win32\include\mutex.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	

"$(INTDIR)\Fft.obj" : $(SOURCE) $(DEP_CPP_FFT_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\VisualView\VisualView.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_VISUA=\
	"..\..\base\win32\include\mutex.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	

"$(INTDIR)\VisualView.obj" : $(SOURCE) $(DEP_CPP_VISUA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_VISUA=\
	"..\..\base\win32\include\mutex.h"\
	".\active.h"\
	".\BmpSize.h"\
	".\cthread.h"\
	".\StdAfx.h"\
	".\VisualView\Fft\Complex.h"\
	".\VisualView\Fft\Fft.h"\
	".\VisualView\VisualView.h"\
	

"$(INTDIR)\VisualView.obj" : $(SOURCE) $(DEP_CPP_VISUA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Preferences\optionsetupdlg.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_OPTIO=\
	".\Preferences\optionsetupdlg.h"\
	".\Rainplay.h"\
	".\StdAfx.h"\
	
CPP_SWITCHES=/nologo /G6 /MT /W3 /GX /O2 /Op /Ob2 /I "." /I ".\Equalizer" /I\
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

DEP_CPP_OPTIO=\
	".\Preferences\optionsetupdlg.h"\
	".\Rainplay.h"\
	".\StdAfx.h"\
	
CPP_SWITCHES=/nologo /G6 /MDd /W3 /Gm /Gi /GR /GX /Zi /Od /I "." /I\
 ".\Equalizer" /I ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I\
 ".\Preferences" /I "..\..\base\win32\include" /I "..\..\lmc\include" /I\
 "..\..\io\include" /I "..\..\config" /I "..\include" /I "..\..\base\include" /I\
 ".." /D "_DEBUG" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS"\
 /D "_USRDLL" /Fp"$(INTDIR)\Rainplay.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /ZI /c 

"$(INTDIR)\optionsetupdlg.obj" : $(SOURCE) $(DEP_CPP_OPTIO) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\base\src\preferences.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_PREFE=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\preferences.h"\
	"..\..\config\config.h"\
	

"$(INTDIR)\preferences.obj" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"\
 "..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_PREFE=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\preferences.h"\
	"..\..\config\config.h"\
	

"$(INTDIR)\preferences.obj" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"\
 "..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Preferences\PreferencesDlg.cpp

!IF  "$(CFG)" == "Rainplay - Win32 Release"

DEP_CPP_PREFER=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\preferences.h"\
	"..\..\config\config.h"\
	".\Preferences\optionsetupdlg.h"\
	".\Preferences\PreferencesDlg.h"\
	".\Rainplay.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\PreferencesDlg.obj" : $(SOURCE) $(DEP_CPP_PREFER) "$(INTDIR)"\
 "..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

DEP_CPP_PREFER=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\preferences.h"\
	"..\..\config\config.h"\
	".\Preferences\optionsetupdlg.h"\
	".\Preferences\PreferencesDlg.h"\
	".\Rainplay.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\PreferencesDlg.obj" : $(SOURCE) $(DEP_CPP_PREFER) "$(INTDIR)"\
 "..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\base\win32\src\win32prefs.cpp
DEP_CPP_WIN32P=\
	"..\..\base\include\errors.h"\
	"..\..\base\include\preferences.h"\
	"..\..\base\win32\include\win32prefs.h"\
	"..\..\config\config.h"\
	

"$(INTDIR)\win32prefs.obj" : $(SOURCE) $(DEP_CPP_WIN32P) "$(INTDIR)"\
 "..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "Rainplay - Win32 Release"

"fileinput - Win32 Release" : 
   cd "\TEMP\merge\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 Release" 
   cd "..\..\..\..\ui\rainplay"

"fileinput - Win32 ReleaseCLEAN" : 
   cd "\TEMP\merge\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fileinput.mak\
 CFG="fileinput - Win32 Release" RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

"fileinput - Win32 Debug" : 
   cd "\TEMP\merge\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fileinput.mak CFG="fileinput - Win32 Debug" 
   cd "..\..\..\..\ui\rainplay"

"fileinput - Win32 DebugCLEAN" : 
   cd "\TEMP\merge\freeamp\io\local\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fileinput.mak CFG="fileinput - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ENDIF 

!IF  "$(CFG)" == "Rainplay - Win32 Release"

"soundcard - Win32 Release" : 
   cd "\TEMP\merge\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 Release" 
   cd "..\..\..\..\ui\rainplay"

"soundcard - Win32 ReleaseCLEAN" : 
   cd "\TEMP\merge\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\soundcard.mak\
 CFG="soundcard - Win32 Release" RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

"soundcard - Win32 Debug" : 
   cd "\TEMP\merge\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\soundcard.mak CFG="soundcard - Win32 Debug" 
   cd "..\..\..\..\ui\rainplay"

"soundcard - Win32 DebugCLEAN" : 
   cd "\TEMP\merge\freeamp\io\soundcard\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\soundcard.mak CFG="soundcard - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ENDIF 

!IF  "$(CFG)" == "Rainplay - Win32 Release"

"xing - Win32 Release" : 
   cd "\TEMP\merge\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 Release" 
   cd "..\..\..\..\ui\rainplay"

"xing - Win32 ReleaseCLEAN" : 
   cd "\TEMP\merge\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\xing.mak CFG="xing - Win32 Release"\
 RECURSE=1 
   cd "..\..\..\..\ui\rainplay"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

"xing - Win32 Debug" : 
   cd "\TEMP\merge\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\xing.mak CFG="xing - Win32 Debug" 
   cd "..\..\..\..\ui\rainplay"

"xing - Win32 DebugCLEAN" : 
   cd "\TEMP\merge\freeamp\lmc\xingmp3\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\xing.mak CFG="xing - Win32 Debug" RECURSE=1\
 
   cd "..\..\..\..\ui\rainplay"

!ENDIF 


!ENDIF 


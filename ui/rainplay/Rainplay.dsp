# Microsoft Developer Studio Project File - Name="Rainplay" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Rainplay - Win32 NASM Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Rainplay.mak".
!MESSAGE 
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

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Rainplay - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GX /O2 /Op /Ob2 /I "." /I ".\Equalizer" /I ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I ".\Preferences" /I "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /I "..\..\config" /I "..\include" /I "..\..\base\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /dll /machine:I386 /out:"rainplay.ui"
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Cmds=IF NOT EXIST ..\..\base\win32\prj\plugins mkdir\
                         ..\..\base\win32\prj\plugins	copy rainplay.ui     ..\..\base\win32\prj\plugins
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /ZI /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GR /GX /Zi /Od /I "." /I ".\Equalizer" /I ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I ".\Preferences" /I "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /I "..\..\config" /I "..\include" /I "..\..\base\include" /D "_DEBUG" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /YX /FD /ZI /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"rainplay.ui" /pdbtype:sept
# SUBTRACT LINK32 /profile
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Cmds=IF NOT EXIST ..\..\base\win32\prj\plugins mkdir\
                         ..\..\base\win32\prj\plugins	copy rainplay.ui     ..\..\base\win32\prj\plugins
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Rainplay - Win32 NASM Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Rainplay"
# PROP BASE Intermediate_Dir "Rainplay"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /Gi /GR /GX /Zi /Od /I "." /I ".\Equalizer" /I ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I ".\Preferences" /I "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /I "..\..\config" /I "..\include" /I "..\..\base\include" /D "_DEBUG" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /YX /FD /ZI /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GR /GX /Zi /Od /I "." /I ".\Equalizer" /I ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I ".\Preferences" /I "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /I "..\..\config" /I "..\include" /I "..\..\base\include" /D "_DEBUG" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /YX /FD /ZI /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"rainplay.ui" /pdbtype:sept
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 winmm.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"rainplay.ui" /pdbtype:sept
# SUBTRACT LINK32 /profile
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Cmds=IF NOT EXIST ..\..\base\win32\prj\plugins mkdir\
                         ..\..\base\win32\prj\plugins	copy rainplay.ui     ..\..\base\win32\prj\plugins
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Rainplay - Win32 NASM Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Rainpla0"
# PROP BASE Intermediate_Dir "Rainpla0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MT /W3 /GX /O2 /Op /Ob2 /I "." /I ".\Equalizer" /I ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I ".\Preferences" /I "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /I "..\..\config" /I "..\include" /I "..\..\base\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /YX /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GX /O2 /Op /Ob2 /I "." /I ".\Equalizer" /I ".\PlayList" /I ".\VisualView" /I ".\VisualView\Fft" /I ".\Preferences" /I "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /I "..\..\config" /I "..\include" /I "..\..\base\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib /nologo /subsystem:windows /dll /machine:I386 /out:"rainplay.ui"
# ADD LINK32 winmm.lib /nologo /subsystem:windows /dll /machine:I386 /out:"rainplay.ui"
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Cmds=IF NOT EXIST ..\..\base\win32\prj\plugins mkdir\
                         ..\..\base\win32\prj\plugins	copy rainplay.ui     ..\..\base\win32\prj\plugins
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Rainplay - Win32 Release"
# Name "Rainplay - Win32 Debug"
# Name "Rainplay - Win32 NASM Debug"
# Name "Rainplay - Win32 NASM Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\active.cpp
# End Source File
# Begin Source File

SOURCE=.\BmpSize.cpp
# End Source File
# Begin Source File

SOURCE=..\..\config\config.win32

!IF  "$(CFG)" == "Rainplay - Win32 Release"

# Begin Custom Build
InputPath=..\..\config\config.win32

"..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\config\config.win32 ..\..\config\config.h

# End Custom Build

!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

# Begin Custom Build
InputPath=..\..\config\config.win32

"..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\config\config.win32 ..\..\config\config.h

# End Custom Build

!ELSEIF  "$(CFG)" == "Rainplay - Win32 NASM Debug"

# Begin Custom Build
InputPath=..\..\config\config.win32

"..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\config\config.win32 ..\..\config\config.h

# End Custom Build

!ELSEIF  "$(CFG)" == "Rainplay - Win32 NASM Release"

# Begin Custom Build
InputPath=..\..\config\config.win32

"..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\config\config.win32 ..\..\config\config.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\EnumFonts.cpp
# End Source File
# Begin Source File

SOURCE=..\..\base\win32\src\mutex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\base\src\playlist.cpp
# End Source File
# Begin Source File

SOURCE=.\Rainplay.cpp
# End Source File
# Begin Source File

SOURCE=.\Rainplay.def
# End Source File
# Begin Source File

SOURCE=.\Rainplay.rc
# End Source File
# Begin Source File

SOURCE=.\RainplayDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RainplayUI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\io\rio\rio.cpp
# End Source File
# Begin Source File

SOURCE=..\..\base\win32\src\semaphore.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\..\base\src\thread.cpp
# End Source File
# Begin Source File

SOURCE=..\..\base\win32\src\win32thread.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\active.h
# End Source File
# Begin Source File

SOURCE=.\BmpSize.h
# End Source File
# Begin Source File

SOURCE=.\cthread.h
# End Source File
# Begin Source File

SOURCE=.\MainFaceDescribe.h
# End Source File
# Begin Source File

SOURCE=.\Rainplay.h
# End Source File
# Begin Source File

SOURCE=.\RainplayDlg.h
# End Source File
# Begin Source File

SOURCE=.\RainplayUI.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\About.bmp
# End Source File
# Begin Source File

SOURCE=..\simple\win32\res\about8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Maindown.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MainUp.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Rainplay.ico
# End Source File
# Begin Source File

SOURCE=.\res\Rainplay.rc2
# End Source File
# End Group
# Begin Group "Equalizer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Equalizer\EQDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Equalizer\EQDlg.h
# End Source File
# End Group
# Begin Group "PlayList"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PlayList\PlayListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayList\PlayListDlg.h
# End Source File
# End Group
# Begin Group "VisualView"

# PROP Default_Filter ""
# Begin Group "FFT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VisualView\Fft\Complex.h
# End Source File
# Begin Source File

SOURCE=.\VisualView\Fft\Fft.cpp
# End Source File
# Begin Source File

SOURCE=.\VisualView\Fft\Fft.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\VisualView\VisualView.cpp
# End Source File
# Begin Source File

SOURCE=.\VisualView\VisualView.h
# End Source File
# End Group
# Begin Group "Preferences"

# PROP Default_Filter ""
# Begin Group "Setup"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Preferences\optionsetupdlg.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\Preferences\optionsetupdlg.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\base\src\preferences.cpp
# End Source File
# Begin Source File

SOURCE=.\Preferences\PreferencesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Preferences\PreferencesDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\base\win32\src\win32prefs.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

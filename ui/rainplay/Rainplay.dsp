# Microsoft Developer Studio Project File - Name="Rainplay" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Rainplay - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Rainplay.mak".
!MESSAGE 
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
# ADD CPP /nologo /G6 /MT /W3 /GX /O2 /Op /Ob2 /I "..\..\config" /I "..\include" /I "..\..\base\include" /I "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
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
# ADD CPP /nologo /G6 /MDd /W3 /GX /Zi /Od /I "..\..\base\win32\include" /I "..\..\lmc\include" /I "..\..\io\include" /I "..\..\config" /I "..\include" /I "..\..\base\include" /D "_DEBUG" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /FD /ZI /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"rainplay.ui" /pdbtype:sept
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Cmds=IF NOT EXIST ..\..\base\win32\prj\plugins mkdir\
                  ..\..\base\win32\prj\plugins	copy rainplay.ui     ..\..\base\win32\prj\plugins
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Rainplay - Win32 Release"
# Name "Rainplay - Win32 Debug"
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

SOURCE=.\EnumFonts.cpp
# End Source File
# Begin Source File

SOURCE=.\EQDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\base\win32\src\mutex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\base\src\playlist.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Rainplay.cpp
# End Source File
# Begin Source File

SOURCE=.\Rainplay.def
# End Source File
# Begin Source File

SOURCE=.\Rainplay.rc

!IF  "$(CFG)" == "Rainplay - Win32 Release"

!ELSEIF  "$(CFG)" == "Rainplay - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RainplayDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RainplayUI.cpp
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

SOURCE=.\VisualView.cpp
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

SOURCE=.\EQDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFaceDescribe.h
# End Source File
# Begin Source File

SOURCE=.\PlayListDlg.h
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
# Begin Source File

SOURCE=.\VisualView.h
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
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

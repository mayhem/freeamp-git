# Microsoft Developer Studio Project File - Name="freeamp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=freeamp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "freeamp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "freeamp.mak" CFG="freeamp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "freeamp - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "freeamp - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "freeamp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "c:\local\src\gamp\config" /I "c:\local\src\gamp\corefunc\include" /I "c:\local\src\gamp\io\include" /I "c:\local\src\gamp\lmc\include" /I "c:\local\src\gamp\ui\include" /I "c:\local\src\gamp\system\win32\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "c:\local\src\gamp\config" /I "c:\local\src\gamp\corefunc\include" /I "c:\local\src\gamp\io\include" /I "c:\local\src\gamp\lmc\include" /I "c:\local\src\gamp\ui\include" /I "c:\local\src\gamp\system\win32\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "freeamp - Win32 Release"
# Name "freeamp - Win32 Debug"
# Begin Group "core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\corefunc\src\bootstrap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\corefunc\src\buffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\corefunc\src\command.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\corefunc\src\player.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\corefunc\src\playlist.cpp
# End Source File
# End Group
# Begin Group "io"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\io\src\localfileinput.cpp
# End Source File
# End Group
# Begin Group "lmc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\lmc\src\cdct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\src\csbt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\src\cup.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\src\cupl3.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\src\cwinm.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\src\dec8.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\src\hwin.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\src\icdct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\src\isbt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\src\iup.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\src\iwinm.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\src\l3dq.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\src\l3init.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\src\mdct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\src\mhead.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\src\msis.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\src\uph.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\src\upsf.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\src\wavep.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\src\xinglmc.cpp
# End Source File
# End Group
# Begin Group "ui"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\ui\src\consoleCIO.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\ui\src\consoleCOO.cpp
# End Source File
# End Group
# Begin Group "system"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\mutex.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semaphore.cpp
# End Source File
# Begin Source File

SOURCE=..\src\soundcardpmo.cpp
# End Source File
# Begin Source File

SOURCE=..\src\thread.cpp
# End Source File
# End Group
# End Target
# End Project

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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\include" /I "..\..\include" /I "..\..\..\config" /I "..\..\..\io\include" /I "..\..\..\ui\include" /I "..\..\..\lmc\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

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
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\include" /I "..\..\include" /I "..\..\..\config" /I "..\..\..\io\include" /I "..\..\..\ui\include" /I "..\..\..\lmc\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "freeamp - Win32 Release"
# Name "freeamp - Win32 Debug"
# Begin Group "base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\buffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\command.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\player.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\playlist.cpp
# End Source File
# End Group
# Begin Group "io"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\io\local\localfileinput.cpp
# End Source File
# End Group
# Begin Group "lmc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\lmc\xingmp3\cdct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\xingmp3\csbt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\xingmp3\cup.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\xingmp3\cupl3.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\xingmp3\cwinm.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\xingmp3\dec8.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\xingmp3\hwin.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\xingmp3\icdct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\xingmp3\isbt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\xingmp3\iup.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\xingmp3\iwinm.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\xingmp3\l3dq.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\xingmp3\l3init.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\xingmp3\mdct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\xingmp3\mhead.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\xingmp3\msis.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\xingmp3\uph.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\xingmp3\upsf.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\xingmp3\wavep.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\xingmp3\xinglmc.cpp
# End Source File
# End Group
# Begin Group "ui"

# PROP Default_Filter ""
# End Group
# Begin Group "system"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\config\config.win32

!IF  "$(CFG)" == "freeamp - Win32 Release"

# Begin Custom Build
InputPath=..\..\..\config\config.win32

"..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\config\config.win32 ..\..\config\config.h

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

# Begin Custom Build
InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h

# End Custom Build

!ENDIF 

# End Source File
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

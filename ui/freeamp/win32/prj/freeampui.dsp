# Microsoft Developer Studio Project File - Name="freeampui" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=freeampui - Win32 NASM Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "freeampui.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "freeampui.mak" CFG="freeampui - Win32 NASM Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "freeampui - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "freeampui - Win32 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "freeampui - Win32 NASM Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "freeampui - Win32 NASM Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "freeampui - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\res" /I "..\include" /I "..\..\include" /I "..\..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /I "..\..\..\..\lib\gdbm" /I "..\..\..\..\lib\xml\include" /I "..\..\..\..\lib\zlib\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 zlib.lib version.lib wsock32.lib winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"freeamp.ui" /libpath:"..\..\..\..\lib\zlib"
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Cmds=IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir\
                                            ..\..\..\..\base\win32\prj\plugins	copy freeamp.ui\
                           ..\..\..\..\base\win32\prj\plugins
# End Special Build Tool

!ELSEIF  "$(CFG)" == "freeampui - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\res" /I "..\include" /I "..\..\include" /I "..\..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /I "..\..\..\..\lib\gdbm" /I "..\..\..\..\lib\xml\include" /I "..\..\..\..\lib\zlib\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 zlib.lib version.lib wsock32.lib winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"freeamp.ui" /pdbtype:sept /libpath:"..\..\..\..\lib\zlib"
# SUBTRACT LINK32 /profile
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Cmds=IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir\
                                            ..\..\..\..\base\win32\prj\plugins	copy freeamp.ui\
                           ..\..\..\..\base\win32\prj\plugins
# End Special Build Tool

!ELSEIF  "$(CFG)" == "freeampui - Win32 NASM Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "freeampu"
# PROP BASE Intermediate_Dir "freeampu"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\res" /I "..\include" /I "..\..\include" /I "..\..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\res" /I "..\include" /I "..\..\include" /I "..\..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /I "..\..\..\..\lib\gdbm" /I "..\..\..\..\lib\xml\include" /I "..\..\..\..\lib\zlib\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"freeamp.ui" /pdbtype:sept
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 zlib.lib version.lib wsock32.lib winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"freeamp.ui" /pdbtype:sept /libpath:"..\..\..\..\lib\zlib"
# SUBTRACT LINK32 /profile
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Cmds=IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir\
                                            ..\..\..\..\base\win32\prj\plugins	copy freeamp.ui\
                           ..\..\..\..\base\win32\prj\plugins
# End Special Build Tool

!ELSEIF  "$(CFG)" == "freeampui - Win32 NASM Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "freeamp0"
# PROP BASE Intermediate_Dir "freeamp0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\res" /I "..\include" /I "..\..\include" /I "..\..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\res" /I "..\include" /I "..\..\include" /I "..\..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /I "..\..\..\..\lib\gdbm" /I "..\..\..\..\lib\xml\include" /I "..\..\..\..\lib\zlib\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"freeamp.ui"
# ADD LINK32 zlib.lib version.lib wsock32.lib winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"freeamp.ui" /libpath:"..\..\..\..\lib\zlib"
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Cmds=IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir\
                                            ..\..\..\..\base\win32\prj\plugins	copy freeamp.ui\
                           ..\..\..\..\base\win32\prj\plugins
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "freeampui - Win32 Release"
# Name "freeampui - Win32 Debug"
# Name "freeampui - Win32 NASM Debug"
# Name "freeampui - Win32 NASM Release"
# Begin Group "source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Bitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\ButtonControl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Canvas.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Control.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\DialControl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Font.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\FreeAmpTheme.cpp
# End Source File
# Begin Source File

SOURCE=.\freeampui.def
# End Source File
# Begin Source File

SOURCE=..\..\src\MultiStateControl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\xml\src\Parse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\SliderControl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\TextControl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Theme.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ThemeManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\ThemeZip.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Win32Bitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Win32Canvas.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Win32Font.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Win32MessageDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Win32PreferenceWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\base\win32\src\win32updatemanager.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Win32Window.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Window.cpp
# End Source File
# End Group
# Begin Group "common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\base\src\database.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\base\src\debug.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\base\src\downloadmanager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\base\src\log.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\base\src\musiccatalog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\base\win32\src\mutex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\base\src\player.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\base\src\playlist.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\base\src\preferences.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\base\src\propimpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\base\src\registrar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\base\src\registry.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\base\win32\src\semaphore.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\base\src\thread.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\base\src\updatemanager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\base\src\utility.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\base\win32\src\win32prefs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\base\win32\src\win32thread.cpp
# End Source File
# End Group
# Begin Group "resource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\res\freeampui.rc
# End Source File
# Begin Source File

SOURCE=..\res\icon1.ico
# End Source File
# End Group
# End Target
# End Project

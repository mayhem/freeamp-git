# Microsoft Developer Studio Project File - Name="fabaselib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=fabaselib - Win32 NASM Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fabaselib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fabaselib.mak" CFG="fabaselib - Win32 NASM Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fabaselib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "fabaselib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "fabaselib - Win32 NASM Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "fabaselib - Win32 NASM Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fabaselib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\lib\http\include" /I "..\..\..\lib\zlib\include" /I "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include" /I "..\..\..\lmc\include" /I "..\..\aps" /I "..\..\..\lib\musicbrainz\lib" /I "..\..\..\lib\musicbrainz\expat\xmlparse" /I "..\..\..\lib\musicbrainz\expat\xmltok" /I "..\..\..\lib\musicbrainz\osdep" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "NOMINMAX" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\fabaselib.lib"

!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /I "..\..\..\lib\http\include" /I "..\..\..\lib\zlib\include" /I "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include" /I "..\..\..\lmc\include" /I "..\..\aps" /I "..\..\..\lib\musicbrainz\lib" /I "..\..\..\lib\musicbrainz\expat\xmlparse" /I "..\..\..\lib\musicbrainz\expat\xmltok" /I "..\..\..\lib\musicbrainz\osdep" /D "_DEBUG" /D "_WINDOWS" /D "WIN32" /D "NOMINMAX" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\fabaselib.lib"

!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "fabaseli"
# PROP BASE Intermediate_Dir "fabaseli"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Z7 /Od /I "..\..\..\lib\zlib\include" /I "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include" /I "..\..\..\lmc\include" /I "..\..\aps" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /I "..\..\..\lib\http\include" /I "..\..\..\lib\zlib\include" /I "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include" /I "..\..\..\lmc\include" /I "..\..\aps" /I "..\..\..\lib\musicbrainz\lib" /I "..\..\..\lib\musicbrainz\expat\xmlparse" /I "..\..\..\lib\musicbrainz\expat\xmltok" /I "..\..\..\lib\musicbrainz\osdep" /D "_DEBUG" /D "_WINDOWS" /D "WIN32" /D "NOMINMAX" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"fabaselib.lib"
# ADD LIB32 /nologo /out:"..\fabaselib.lib"

!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "fabasel0"
# PROP BASE Intermediate_Dir "fabasel0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\lib\zlib\include" /I "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include" /I "..\..\..\lmc\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\lib\http\include" /I "..\..\..\lib\zlib\include" /I "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include" /I "..\..\..\lmc\include" /I "..\..\aps" /I "..\..\..\lib\musicbrainz\lib" /I "..\..\..\lib\musicbrainz\expat\xmlparse" /I "..\..\..\lib\musicbrainz\expat\xmltok" /I "..\..\..\lib\musicbrainz\osdep" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "NOMINMAX" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"fabaselib.lib"
# ADD LIB32 /nologo /out:"..\fabaselib.lib"

!ENDIF 

# Begin Target

# Name "fabaselib - Win32 Release"
# Name "fabaselib - Win32 Debug"
# Name "fabaselib - Win32 NASM Debug"
# Name "fabaselib - Win32 NASM Release"
# Begin Source File

SOURCE=..\..\aps\apsconvert.cpp
# End Source File
# Begin Source File

SOURCE=..\..\aps\apsinterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\aps\apsplaylist.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\musicbrainz\lib\base64.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\musicbrainz\osdep\cdi_win32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\musicbrainz\lib\cdindex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\config\config.win32

!IF  "$(CFG)" == "fabaselib - Win32 Release"

# Begin Custom Build
InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h

# End Custom Build

!ELSEIF  "$(CFG)" == "fabaselib - Win32 Debug"

# Begin Custom Build
InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h

# End Custom Build

!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Debug"

# Begin Custom Build
InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h

# End Custom Build

!ELSEIF  "$(CFG)" == "fabaselib - Win32 NASM Release"

# Begin Custom Build
InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\database.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\debug.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\musicbrainz\lib\diskid.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\downloadmanager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\errors.cpp
# End Source File
# Begin Source File

SOURCE=..\..\aps\FAMetaUnit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\aps\gencrc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\musicbrainz\expat\xmlparse\hashtable.c
# End Source File
# Begin Source File

SOURCE=..\..\aps\hosttonet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\http\src\Http.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\log.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\musicbrainz\lib\mb_http.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\missingfile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\musiccatalog.cpp
# End Source File
# Begin Source File

SOURCE=..\src\mutex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\aps\nettohost.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\xml\src\Parse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\player.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\playlist.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\preferences.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\propimpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\registrar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\registry.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semaphore.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\musicbrainz\lib\sha_func.c
# End Source File
# Begin Source File

SOURCE=..\..\aps\sigclient.cpp
# End Source File
# Begin Source File

SOURCE=..\..\aps\sigxdr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\thread.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\timer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\undomanager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\updatemanager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\utility.cpp
# End Source File
# Begin Source File

SOURCE=..\..\aps\uuid.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32prefs.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32thread.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32updatemanager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\aps\wincomsocket.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\musicbrainz\expat\xmlparse\xmlparse.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\musicbrainz\expat\xmltok\xmlrole.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\musicbrainz\expat\xmltok\xmltok.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\musicbrainz\lib\xparse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\musicbrainz\lib\xql.cpp
# End Source File
# Begin Source File

SOURCE=..\..\aps\ypclient.cpp
# End Source File
# End Target
# End Project

# Microsoft Developer Studio Project File - Name="freeamp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=freeamp - Win32 NASM Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "freeamp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "freeamp.mak" CFG="freeamp - Win32 NASM Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "freeamp - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "freeamp - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "freeamp - Win32 NASM Release" (based on "Win32 (x86) Application")
!MESSAGE "freeamp - Win32 NASM Debug" (based on "Win32 (x86) Application")
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
# ADD CPP /nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\..\..\lib\zlib\include" /I "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include" /I "..\..\..\lmc\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
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
# ADD LINK32 fabaselib.lib zlib.lib gdbm.lib version.lib winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"freeamp.exe" /libpath:"..\..\..\lib\gdbm" /libpath:"..\..\..\lib\zlib" /libpath:"..\\"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\lib\zlib\include" /I "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include" /I "..\..\..\lmc\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
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
# ADD LINK32 fabaselib.lib zlib.lib gdbm.lib version.lib winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"freeamp.exe" /pdbtype:sept /libpath:"..\..\..\lib\gdbm" /libpath:"..\..\..\lib\zlib" /libpath:"..\\"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "freeamp_"
# PROP BASE Intermediate_Dir "freeamp_"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\include" /I "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include" /I "..\..\..\lmc\include" /I "..\..\..\ui\dummy\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# SUBTRACT BASE CPP /X
# ADD CPP /nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\..\..\lib\zlib\include" /I "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include" /I "..\..\..\lmc\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"freeamp.exe"
# ADD LINK32 fabaselib.lib zlib.lib gdbm.lib version.lib winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"freeamp.exe" /libpath:"..\..\..\lib\gdbm" /libpath:"..\..\..\lib\zlib" /libpath:"..\\"

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "freeamp0"
# PROP BASE Intermediate_Dir "freeamp0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\ui\dummy\include" /I "..\include" /I "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include" /I "..\..\..\lmc\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# SUBTRACT BASE CPP /X
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\lib\zlib\include" /I "..\..\..\lib\xml\include" /I "..\..\..\lib\gdbm" /I "..\include" /I "..\..\include" /I "..\..\..\config" /I "..\..\..\ui\win32Test\include" /I "..\..\..\ui\win32Test\res" /I "..\..\..\io\include" /I "..\..\..\ui\include" /I "..\..\..\lmc\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"freeamp.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 fabaselib.lib zlib.lib gdbm.lib version.lib winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"freeamp.exe" /pdbtype:sept /libpath:"..\..\..\lib\gdbm" /libpath:"..\..\..\lib\zlib" /libpath:"..\\"
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "freeamp - Win32 Release"
# Name "freeamp - Win32 Debug"
# Name "freeamp - Win32 NASM Release"
# Name "freeamp - Win32 NASM Debug"
# Begin Group "source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\config\config.win32

!IF  "$(CFG)" == "freeamp - Win32 Release"

# Begin Custom Build
InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

# Begin Custom Build
InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

# Begin Custom Build
InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

# Begin Custom Build
InputPath=..\..\..\config\config.win32

"..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\config\config.win32 ..\..\..\config\config.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\main.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\xml\src\Parse.cpp
# End Source File
# End Group
# Begin Group "text files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\AUTHORS

!IF  "$(CFG)" == "freeamp - Win32 Release"

# Begin Custom Build
InputPath=..\..\..\AUTHORS

"..\..\..\Authors.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\AUTHORS ..\..\..\Authors.txt

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

# Begin Custom Build
InputPath=..\..\..\AUTHORS

"..\..\..\Authors.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\AUTHORS ..\..\..\Authors.txt

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

# Begin Custom Build
InputPath=..\..\..\AUTHORS

"..\..\..\Authors.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\AUTHORS ..\..\..\Authors.txt

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

# Begin Custom Build
InputPath=..\..\..\AUTHORS

"..\..\..\Authors.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\AUTHORS ..\..\..\Authors.txt

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\CHANGES

!IF  "$(CFG)" == "freeamp - Win32 Release"

# Begin Custom Build
InputPath=..\..\..\CHANGES

"..\..\..\Changes.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\CHANGES ..\..\..\Changes.txt

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

# Begin Custom Build
InputPath=..\..\..\CHANGES

"..\..\..\Changes.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\CHANGES ..\..\..\Changes.txt

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

# Begin Custom Build
InputPath=..\..\..\CHANGES

"..\..\..\Changes.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\CHANGES ..\..\..\Changes.txt

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

# Begin Custom Build
InputPath=..\..\..\CHANGES

"..\..\..\Changes.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\CHANGES ..\..\..\Changes.txt

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\COPYING

!IF  "$(CFG)" == "freeamp - Win32 Release"

# Begin Custom Build
InputPath=..\..\..\COPYING

"..\..\..\Copying.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\COPYING ..\..\..\Copying.txt

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

# Begin Custom Build
InputPath=..\..\..\COPYING

"..\..\..\Copying.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\COPYING ..\..\..\Copying.txt

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

# Begin Custom Build
InputPath=..\..\..\COPYING

"..\..\..\Copying.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\COPYING ..\..\..\Copying.txt

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

# Begin Custom Build
InputPath=..\..\..\COPYING

"..\..\..\Copying.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\COPYING ..\..\..\Copying.txt

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\README

!IF  "$(CFG)" == "freeamp - Win32 Release"

# Begin Custom Build
InputPath=..\..\..\README

"..\..\..\ReadMe.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\README ..\..\..\ReadMe.txt

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

# Begin Custom Build
InputPath=..\..\..\README

"..\..\..\ReadMe.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\README ..\..\..\ReadMe.txt

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

# Begin Custom Build
InputPath=..\..\..\README

"..\..\..\ReadMe.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\README ..\..\..\ReadMe.txt

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

# Begin Custom Build
InputPath=..\..\..\README

"..\..\..\ReadMe.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\README ..\..\..\ReadMe.txt

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "help files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\help\win32\freeamp.cnt

!IF  "$(CFG)" == "freeamp - Win32 Release"

# Begin Custom Build
InputPath=..\..\..\help\win32\freeamp.cnt

"freeamp.cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy  ..\..\..\help\win32\freeamp.cnt .

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

# Begin Custom Build
InputPath=..\..\..\help\win32\freeamp.cnt

"freeamp.cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy  ..\..\..\help\win32\freeamp.cnt .

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

# Begin Custom Build
InputPath=..\..\..\help\win32\freeamp.cnt

"freeamp.cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy  ..\..\..\help\win32\freeamp.cnt .

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

# Begin Custom Build
InputPath=..\..\..\help\win32\freeamp.cnt

"freeamp.cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy  ..\..\..\help\win32\freeamp.cnt .

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\help\win32\freeamp.hlp

!IF  "$(CFG)" == "freeamp - Win32 Release"

# Begin Custom Build
InputPath=..\..\..\help\win32\freeamp.hlp

"freeamp.hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy  ..\..\..\help\win32\freeamp.hlp .

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

# Begin Custom Build
InputPath=..\..\..\help\win32\freeamp.hlp

"freeamp.hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy  ..\..\..\help\win32\freeamp.hlp .

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

# Begin Custom Build
InputPath=..\..\..\help\win32\freeamp.hlp

"freeamp.hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy  ..\..\..\help\win32\freeamp.hlp .

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

# Begin Custom Build
InputPath=..\..\..\help\win32\freeamp.hlp

"freeamp.hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy  ..\..\..\help\win32\freeamp.hlp .

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "resources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\res\freeamp.rc
# End Source File
# Begin Source File

SOURCE=..\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=..\res\music1.ico
# End Source File
# Begin Source File

SOURCE=..\res\music2.ico
# End Source File
# Begin Source File

SOURCE=..\res\music3.ico
# End Source File
# Begin Source File

SOURCE=..\res\music4.ico
# End Source File
# Begin Source File

SOURCE=..\res\music5.ico
# End Source File
# Begin Source File

SOURCE=..\res\music6.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\portio\i386\free\portio.sys

!IF  "$(CFG)" == "freeamp - Win32 Release"

# Begin Custom Build
InputPath=..\..\..\lib\portio\i386\free\portio.sys

".\portio.sys" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\lib\portio\i386\free\portio.sys .\portio.sys

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

# Begin Custom Build
InputPath=..\..\..\lib\portio\i386\free\portio.sys

".\portio.sys" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\lib\portio\i386\free\portio.sys .\portio.sys

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

# Begin Custom Build
InputPath=..\..\..\lib\portio\i386\free\portio.sys

".\portio.sys" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\lib\portio\i386\free\portio.sys .\portio.sys

# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

# Begin Custom Build
InputPath=..\..\..\lib\portio\i386\free\portio.sys

".\portio.sys" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\lib\portio\i386\free\portio.sys .\portio.sys

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\res\warn.ico
# End Source File
# End Group
# Begin Group "themes"

# PROP Default_Filter "fat"
# Begin Source File

SOURCE=..\..\..\themes\Aquatica.fat

!IF  "$(CFG)" == "freeamp - Win32 Release"

# Begin Custom Build
InputPath=..\..\..\themes\Aquatica.fat
InputName=Aquatica

".\themes\$(InputName).fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

# Begin Custom Build
InputPath=..\..\..\themes\Aquatica.fat
InputName=Aquatica

".\themes\$(InputName).fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

# Begin Custom Build
InputPath=..\..\..\themes\Aquatica.fat
InputName=Aquatica

".\themes\$(InputName).fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

# Begin Custom Build
InputPath=..\..\..\themes\Aquatica.fat
InputName=Aquatica

".\themes\$(InputName).fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\themes\EMusic.fat

!IF  "$(CFG)" == "freeamp - Win32 Release"

# Begin Custom Build
InputPath=..\..\..\themes\EMusic.fat
InputName=EMusic

".\themes\$(InputName).fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

# Begin Custom Build
InputPath=..\..\..\themes\EMusic.fat
InputName=EMusic

".\themes\$(InputName).fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

# Begin Custom Build
InputPath=..\..\..\themes\EMusic.fat
InputName=EMusic

".\themes\$(InputName).fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

# Begin Custom Build
InputPath=..\..\..\themes\EMusic.fat
InputName=EMusic

".\themes\$(InputName).fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\themes\FreeAmp.fat

!IF  "$(CFG)" == "freeamp - Win32 Release"

# Begin Custom Build
InputPath=..\..\..\themes\FreeAmp.fat
InputName=FreeAmp

".\themes\$(InputName).fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

# Begin Custom Build
InputPath=..\..\..\themes\FreeAmp.fat
InputName=FreeAmp

".\themes\$(InputName).fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

# Begin Custom Build
InputPath=..\..\..\themes\FreeAmp.fat
InputName=FreeAmp

".\themes\$(InputName).fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

# Begin Custom Build
InputPath=..\..\..\themes\FreeAmp.fat
InputName=FreeAmp

".\themes\$(InputName).fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\themes\FreeAmpClassic.fat

!IF  "$(CFG)" == "freeamp - Win32 Release"

# Begin Custom Build
InputPath=..\..\..\themes\FreeAmpClassic.fat
InputName=FreeAmpClassic

".\themes\$(InputName).fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 Debug"

# Begin Custom Build
InputPath=..\..\..\themes\FreeAmpClassic.fat
InputName=FreeAmpClassic

".\themes\$(InputName).fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Release"

# Begin Custom Build
InputPath=..\..\..\themes\FreeAmpClassic.fat
InputName=FreeAmpClassic

".\themes\$(InputName).fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "freeamp - Win32 NASM Debug"

# Begin Custom Build
InputPath=..\..\..\themes\FreeAmpClassic.fat
InputName=FreeAmpClassic

".\themes\$(InputName).fat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST themes mkdir themes 
	copy $(InputPath) themes 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "includes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\config\config.h
# End Source File
# Begin Source File

SOURCE=..\..\include\database.h
# End Source File
# Begin Source File

SOURCE=..\..\include\debug.h
# End Source File
# Begin Source File

SOURCE=..\..\include\downloadformat.h
# End Source File
# Begin Source File

SOURCE=..\..\include\downloadmanager.h
# End Source File
# Begin Source File

SOURCE=..\..\include\errors.h
# End Source File
# Begin Source File

SOURCE=..\..\include\event.h
# End Source File
# Begin Source File

SOURCE=..\..\include\eventdata.h
# End Source File
# Begin Source File

SOURCE=..\..\include\facontext.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\gdbm\gdbm_fa.h
# End Source File
# Begin Source File

SOURCE=..\..\include\hashtable.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lmc\include\lmc.h
# End Source File
# Begin Source File

SOURCE=..\..\include\log.h
# End Source File
# Begin Source File

SOURCE=..\..\include\metadata.h
# End Source File
# Begin Source File

SOURCE=..\..\include\musiccatalog.h
# End Source File
# Begin Source File

SOURCE=..\include\mutex.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\xml\include\Parse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\io\include\pipeline.h
# End Source File
# Begin Source File

SOURCE=..\..\include\player.h
# End Source File
# Begin Source File

SOURCE=..\..\include\playlist.h
# End Source File
# Begin Source File

SOURCE=..\..\include\playlistformat.h
# End Source File
# Begin Source File

SOURCE=..\..\include\plmevent.h
# End Source File
# Begin Source File

SOURCE=..\..\include\portabledevice.h
# End Source File
# Begin Source File

SOURCE=..\..\include\preferences.h
# End Source File
# Begin Source File

SOURCE=..\..\include\properties.h
# End Source File
# Begin Source File

SOURCE=..\..\include\propimpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\io\include\pullbuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\include\queue.h
# End Source File
# Begin Source File

SOURCE=..\..\include\registrar.h
# End Source File
# Begin Source File

SOURCE=..\..\include\registry.h
# End Source File
# Begin Source File

SOURCE=..\include\semaphore.h
# End Source File
# Begin Source File

SOURCE=..\..\include\thread.h
# End Source File
# Begin Source File

SOURCE=..\..\..\ui\include\ui.h
# End Source File
# Begin Source File

SOURCE=..\..\include\undomanager.h
# End Source File
# Begin Source File

SOURCE=..\..\include\utility.h
# End Source File
# Begin Source File

SOURCE=..\include\win32prefs.h
# End Source File
# End Group
# End Target
# End Project

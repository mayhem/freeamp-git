# Microsoft Developer Studio Generated NMAKE File, Based on httpinput.dsp
!IF "$(CFG)" == ""
CFG=httpinput - Win32 NASM Debug
!MESSAGE No configuration specified. Defaulting to httpinput - Win32 NASM\
 Debug.
!ENDIF 

!IF "$(CFG)" != "httpinput - Win32 Release" && "$(CFG)" !=\
 "httpinput - Win32 Debug" && "$(CFG)" != "httpinput - Win32 NASM Debug" &&\
 "$(CFG)" != "httpinput - Win32 NASM Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "httpinput.mak" CFG="httpinput - Win32 NASM Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "httpinput - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "httpinput - Win32 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "httpinput - Win32 NASM Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "httpinput - Win32 NASM Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "httpinput - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\httpinput.pmi"

!ELSE 

ALL : "fabaselib - Win32 Release" ".\httpinput.pmi"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fabaselib - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\http.res"
	-@erase "$(INTDIR)\httpinput.obj"
	-@erase "$(INTDIR)\pipeline.obj"
	-@erase "$(INTDIR)\pmi.obj"
	-@erase "$(INTDIR)\pullbuffer.obj"
	-@erase "$(INTDIR)\streambuffer.obj"
	-@erase "$(INTDIR)\tstream.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\httpinput.exp"
	-@erase "$(OUTDIR)\httpinput.lib"
	-@erase ".\httpinput.pmi"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\..\\" /I "..\..\..\include" /I\
 "..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I\
 "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I\
 "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\httpinput.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\http.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\httpinput.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x10400000" /subsystem:windows\
 /dll /incremental:no /pdb:"$(OUTDIR)\httpinput.pdb" /machine:I386\
 /def:".\httpinput.def" /out:"httpinput.pmi" /implib:"$(OUTDIR)\httpinput.lib"\
 /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\httpinput.def"
LINK32_OBJS= \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\http.res" \
	"$(INTDIR)\httpinput.obj" \
	"$(INTDIR)\pipeline.obj" \
	"$(INTDIR)\pmi.obj" \
	"$(INTDIR)\pullbuffer.obj" \
	"$(INTDIR)\streambuffer.obj" \
	"$(INTDIR)\tstream.obj" \
	"..\..\..\..\base\win32\fabaselib.lib"

".\httpinput.pmi" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "fabaselib - Win32 Release" ".\httpinput.pmi"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                              ..\..\..\..\base\win32\prj\plugins
	copy httpinput.pmi                         ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "httpinput - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\httpinput.pmi"

!ELSE 

ALL : "fabaselib - Win32 Debug" ".\httpinput.pmi"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fabaselib - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\http.res"
	-@erase "$(INTDIR)\httpinput.obj"
	-@erase "$(INTDIR)\pipeline.obj"
	-@erase "$(INTDIR)\pmi.obj"
	-@erase "$(INTDIR)\pullbuffer.obj"
	-@erase "$(INTDIR)\streambuffer.obj"
	-@erase "$(INTDIR)\tstream.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\httpinput.exp"
	-@erase "$(OUTDIR)\httpinput.lib"
	-@erase "$(OUTDIR)\httpinput.pdb"
	-@erase ".\httpinput.ilk"
	-@erase ".\httpinput.pmi"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\\" /I "..\..\..\include" /I\
 "..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I\
 "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I\
 "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\httpinput.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\http.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\httpinput.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x10400000" /subsystem:windows\
 /dll /incremental:yes /pdb:"$(OUTDIR)\httpinput.pdb" /debug /machine:I386\
 /def:".\httpinput.def" /out:"httpinput.pmi" /implib:"$(OUTDIR)\httpinput.lib"\
 /pdbtype:sept /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\httpinput.def"
LINK32_OBJS= \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\http.res" \
	"$(INTDIR)\httpinput.obj" \
	"$(INTDIR)\pipeline.obj" \
	"$(INTDIR)\pmi.obj" \
	"$(INTDIR)\pullbuffer.obj" \
	"$(INTDIR)\streambuffer.obj" \
	"$(INTDIR)\tstream.obj" \
	"..\..\..\..\base\win32\fabaselib.lib"

".\httpinput.pmi" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "fabaselib - Win32 Debug" ".\httpinput.pmi"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                              ..\..\..\..\base\win32\prj\plugins
	copy httpinput.pmi                         ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "httpinput - Win32 NASM Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\httpinput.pmi"

!ELSE 

ALL : "fabaselib - Win32 NASM Debug" ".\httpinput.pmi"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fabaselib - Win32 NASM DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\http.res"
	-@erase "$(INTDIR)\httpinput.obj"
	-@erase "$(INTDIR)\pipeline.obj"
	-@erase "$(INTDIR)\pmi.obj"
	-@erase "$(INTDIR)\pullbuffer.obj"
	-@erase "$(INTDIR)\streambuffer.obj"
	-@erase "$(INTDIR)\tstream.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\httpinput.exp"
	-@erase "$(OUTDIR)\httpinput.lib"
	-@erase "$(OUTDIR)\httpinput.pdb"
	-@erase ".\httpinput.ilk"
	-@erase ".\httpinput.pmi"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\\" /I "..\..\..\include" /I\
 "..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I\
 "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I\
 "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\httpinput.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\http.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\httpinput.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x10400000" /subsystem:windows\
 /dll /incremental:yes /pdb:"$(OUTDIR)\httpinput.pdb" /debug /machine:I386\
 /def:".\httpinput.def" /out:"httpinput.pmi" /implib:"$(OUTDIR)\httpinput.lib"\
 /pdbtype:sept /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\httpinput.def"
LINK32_OBJS= \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\http.res" \
	"$(INTDIR)\httpinput.obj" \
	"$(INTDIR)\pipeline.obj" \
	"$(INTDIR)\pmi.obj" \
	"$(INTDIR)\pullbuffer.obj" \
	"$(INTDIR)\streambuffer.obj" \
	"$(INTDIR)\tstream.obj" \
	"..\..\..\..\base\win32\fabaselib.lib"

".\httpinput.pmi" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "fabaselib - Win32 NASM Debug" ".\httpinput.pmi"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                              ..\..\..\..\base\win32\prj\plugins
	copy httpinput.pmi                         ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "httpinput - Win32 NASM Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\httpinput.pmi"

!ELSE 

ALL : "fabaselib - Win32 NASM Release" ".\httpinput.pmi"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fabaselib - Win32 NASM ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\http.res"
	-@erase "$(INTDIR)\httpinput.obj"
	-@erase "$(INTDIR)\pipeline.obj"
	-@erase "$(INTDIR)\pmi.obj"
	-@erase "$(INTDIR)\pullbuffer.obj"
	-@erase "$(INTDIR)\streambuffer.obj"
	-@erase "$(INTDIR)\tstream.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\httpinput.exp"
	-@erase "$(OUTDIR)\httpinput.lib"
	-@erase ".\httpinput.pmi"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /Op /Ob2 /I "..\..\\" /I "..\..\..\include" /I\
 "..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I\
 "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I\
 "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\httpinput.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\http.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\httpinput.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x10400000" /subsystem:windows\
 /dll /incremental:no /pdb:"$(OUTDIR)\httpinput.pdb" /machine:I386\
 /def:".\httpinput.def" /out:"httpinput.pmi" /implib:"$(OUTDIR)\httpinput.lib"\
 /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\httpinput.def"
LINK32_OBJS= \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\http.res" \
	"$(INTDIR)\httpinput.obj" \
	"$(INTDIR)\pipeline.obj" \
	"$(INTDIR)\pmi.obj" \
	"$(INTDIR)\pullbuffer.obj" \
	"$(INTDIR)\streambuffer.obj" \
	"$(INTDIR)\tstream.obj" \
	"..\..\..\..\base\win32\fabaselib.lib"

".\httpinput.pmi" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "fabaselib - Win32 NASM Release" ".\httpinput.pmi"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                              ..\..\..\..\base\win32\prj\plugins
	copy httpinput.pmi                         ..\..\..\..\base\win32\prj\plugins
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


!IF "$(CFG)" == "httpinput - Win32 Release" || "$(CFG)" ==\
 "httpinput - Win32 Debug" || "$(CFG)" == "httpinput - Win32 NASM Debug" ||\
 "$(CFG)" == "httpinput - Win32 NASM Release"
SOURCE=..\..\..\..\base\src\debug.cpp
DEP_CPP_DEBUG=\
	"..\..\..\..\base\include\debug.h"\
	

"$(INTDIR)\debug.obj" : $(SOURCE) $(DEP_CPP_DEBUG) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\httpinput.cpp

!IF  "$(CFG)" == "httpinput - Win32 Release"

DEP_CPP_HTTPI=\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\include\pipeline.h"\
	"..\..\..\include\pmi.h"\
	"..\..\..\include\pullbuffer.h"\
	"..\..\..\include\tstream.h"\
	"..\..\httpinput.h"\
	

"$(INTDIR)\httpinput.obj" : $(SOURCE) $(DEP_CPP_HTTPI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 Debug"

DEP_CPP_HTTPI=\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\include\pipeline.h"\
	"..\..\..\include\pmi.h"\
	"..\..\..\include\pullbuffer.h"\
	"..\..\..\include\tstream.h"\
	"..\..\httpinput.h"\
	

"$(INTDIR)\httpinput.obj" : $(SOURCE) $(DEP_CPP_HTTPI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 NASM Debug"

DEP_CPP_HTTPI=\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\include\pipeline.h"\
	"..\..\..\include\pmi.h"\
	"..\..\..\include\pullbuffer.h"\
	"..\..\..\include\tstream.h"\
	"..\..\httpinput.h"\
	

"$(INTDIR)\httpinput.obj" : $(SOURCE) $(DEP_CPP_HTTPI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 NASM Release"

DEP_CPP_HTTPI=\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\include\pipeline.h"\
	"..\..\..\include\pmi.h"\
	"..\..\..\include\pullbuffer.h"\
	"..\..\..\include\tstream.h"\
	"..\..\httpinput.h"\
	

"$(INTDIR)\httpinput.obj" : $(SOURCE) $(DEP_CPP_HTTPI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\src\pipeline.cpp

!IF  "$(CFG)" == "httpinput - Win32 Release"

DEP_CPP_PIPEL=\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\queue.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lmc\include\lmc.h"\
	"..\..\..\include\eventbuffer.h"\
	"..\..\..\include\pipeline.h"\
	"..\..\..\include\pmi.h"\
	"..\..\..\include\pmo.h"\
	"..\..\..\include\pmoevent.h"\
	"..\..\..\include\pullbuffer.h"\
	

"$(INTDIR)\pipeline.obj" : $(SOURCE) $(DEP_CPP_PIPEL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 Debug"

DEP_CPP_PIPEL=\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\queue.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lmc\include\lmc.h"\
	"..\..\..\include\eventbuffer.h"\
	"..\..\..\include\pipeline.h"\
	"..\..\..\include\pmi.h"\
	"..\..\..\include\pmo.h"\
	"..\..\..\include\pmoevent.h"\
	"..\..\..\include\pullbuffer.h"\
	

"$(INTDIR)\pipeline.obj" : $(SOURCE) $(DEP_CPP_PIPEL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 NASM Debug"

DEP_CPP_PIPEL=\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\queue.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lmc\include\lmc.h"\
	"..\..\..\include\eventbuffer.h"\
	"..\..\..\include\pipeline.h"\
	"..\..\..\include\pmi.h"\
	"..\..\..\include\pmo.h"\
	"..\..\..\include\pmoevent.h"\
	"..\..\..\include\pullbuffer.h"\
	

"$(INTDIR)\pipeline.obj" : $(SOURCE) $(DEP_CPP_PIPEL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 NASM Release"

DEP_CPP_PIPEL=\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\queue.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lmc\include\lmc.h"\
	"..\..\..\include\eventbuffer.h"\
	"..\..\..\include\pipeline.h"\
	"..\..\..\include\pmi.h"\
	"..\..\..\include\pmo.h"\
	"..\..\..\include\pmoevent.h"\
	"..\..\..\include\pullbuffer.h"\
	

"$(INTDIR)\pipeline.obj" : $(SOURCE) $(DEP_CPP_PIPEL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\src\pmi.cpp

!IF  "$(CFG)" == "httpinput - Win32 Release"

DEP_CPP_PMI_C=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\include\pipeline.h"\
	"..\..\..\include\pmi.h"\
	"..\..\..\include\pullbuffer.h"\
	

"$(INTDIR)\pmi.obj" : $(SOURCE) $(DEP_CPP_PMI_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 Debug"

DEP_CPP_PMI_C=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\include\pipeline.h"\
	"..\..\..\include\pmi.h"\
	"..\..\..\include\pullbuffer.h"\
	

"$(INTDIR)\pmi.obj" : $(SOURCE) $(DEP_CPP_PMI_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 NASM Debug"

DEP_CPP_PMI_C=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\include\pipeline.h"\
	"..\..\..\include\pmi.h"\
	"..\..\..\include\pullbuffer.h"\
	

"$(INTDIR)\pmi.obj" : $(SOURCE) $(DEP_CPP_PMI_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 NASM Release"

DEP_CPP_PMI_C=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\include\pipeline.h"\
	"..\..\..\include\pmi.h"\
	"..\..\..\include\pullbuffer.h"\
	

"$(INTDIR)\pmi.obj" : $(SOURCE) $(DEP_CPP_PMI_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\src\pullbuffer.cpp

!IF  "$(CFG)" == "httpinput - Win32 Release"

DEP_CPP_PULLB=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\include\pullbuffer.h"\
	

"$(INTDIR)\pullbuffer.obj" : $(SOURCE) $(DEP_CPP_PULLB) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 Debug"

DEP_CPP_PULLB=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\include\pullbuffer.h"\
	

"$(INTDIR)\pullbuffer.obj" : $(SOURCE) $(DEP_CPP_PULLB) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 NASM Debug"

DEP_CPP_PULLB=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\include\pullbuffer.h"\
	

"$(INTDIR)\pullbuffer.obj" : $(SOURCE) $(DEP_CPP_PULLB) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 NASM Release"

DEP_CPP_PULLB=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\include\pullbuffer.h"\
	

"$(INTDIR)\pullbuffer.obj" : $(SOURCE) $(DEP_CPP_PULLB) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\src\streambuffer.cpp

!IF  "$(CFG)" == "httpinput - Win32 Release"

DEP_CPP_STREA=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\include\pullbuffer.h"\
	"..\..\..\include\streambuffer.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\streambuffer.obj" : $(SOURCE) $(DEP_CPP_STREA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 Debug"

DEP_CPP_STREA=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\include\pullbuffer.h"\
	"..\..\..\include\streambuffer.h"\
	

"$(INTDIR)\streambuffer.obj" : $(SOURCE) $(DEP_CPP_STREA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 NASM Debug"

DEP_CPP_STREA=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\include\pullbuffer.h"\
	"..\..\..\include\streambuffer.h"\
	

"$(INTDIR)\streambuffer.obj" : $(SOURCE) $(DEP_CPP_STREA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 NASM Release"

DEP_CPP_STREA=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\include\pullbuffer.h"\
	"..\..\..\include\streambuffer.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\streambuffer.obj" : $(SOURCE) $(DEP_CPP_STREA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\src\tstream.cpp

!IF  "$(CFG)" == "httpinput - Win32 Release"

DEP_CPP_TSTRE=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\include\tstream.h"\
	

"$(INTDIR)\tstream.obj" : $(SOURCE) $(DEP_CPP_TSTRE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 Debug"

DEP_CPP_TSTRE=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\include\tstream.h"\
	

"$(INTDIR)\tstream.obj" : $(SOURCE) $(DEP_CPP_TSTRE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 NASM Debug"

DEP_CPP_TSTRE=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\include\tstream.h"\
	

"$(INTDIR)\tstream.obj" : $(SOURCE) $(DEP_CPP_TSTRE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 NASM Release"

DEP_CPP_TSTRE=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\include\tstream.h"\
	

"$(INTDIR)\tstream.obj" : $(SOURCE) $(DEP_CPP_TSTRE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\res\http.rc

!IF  "$(CFG)" == "httpinput - Win32 Release"


"$(INTDIR)\http.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\http.res" /i\
 "\FreeAmp\freeamp\io\http\win32\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 Debug"


"$(INTDIR)\http.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\http.res" /i\
 "\FreeAmp\freeamp\io\http\win32\res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 NASM Debug"


"$(INTDIR)\http.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\http.res" /i\
 "\FreeAmp\freeamp\io\http\win32\res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "httpinput - Win32 NASM Release"


"$(INTDIR)\http.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\http.res" /i\
 "\FreeAmp\freeamp\io\http\win32\res" /d "NDEBUG" $(SOURCE)


!ENDIF 

!IF  "$(CFG)" == "httpinput - Win32 Release"

"fabaselib - Win32 Release" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 Release" 
   cd "..\..\..\io\http\win32\prj"

"fabaselib - Win32 ReleaseCLEAN" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak\
 CFG="fabaselib - Win32 Release" RECURSE=1 
   cd "..\..\..\io\http\win32\prj"

!ELSEIF  "$(CFG)" == "httpinput - Win32 Debug"

"fabaselib - Win32 Debug" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 Debug" 
   cd "..\..\..\io\http\win32\prj"

"fabaselib - Win32 DebugCLEAN" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak CFG="fabaselib - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\io\http\win32\prj"

!ELSEIF  "$(CFG)" == "httpinput - Win32 NASM Debug"

"fabaselib - Win32 NASM Debug" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 NASM Debug" 
   cd "..\..\..\io\http\win32\prj"

"fabaselib - Win32 NASM DebugCLEAN" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak\
 CFG="fabaselib - Win32 NASM Debug" RECURSE=1 
   cd "..\..\..\io\http\win32\prj"

!ELSEIF  "$(CFG)" == "httpinput - Win32 NASM Release"

"fabaselib - Win32 NASM Release" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak\
 CFG="fabaselib - Win32 NASM Release" 
   cd "..\..\..\io\http\win32\prj"

"fabaselib - Win32 NASM ReleaseCLEAN" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak\
 CFG="fabaselib - Win32 NASM Release" RECURSE=1 
   cd "..\..\..\io\http\win32\prj"

!ENDIF 


!ENDIF 


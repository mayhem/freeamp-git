# Microsoft Developer Studio Generated NMAKE File, Based on misc.dsp
!IF "$(CFG)" == ""
CFG=misc - Win32 NASM Debug
!MESSAGE No configuration specified. Defaulting to misc - Win32 NASM Debug.
!ENDIF 

!IF "$(CFG)" != "misc - Win32 Release" && "$(CFG)" != "misc - Win32 Debug" &&\
 "$(CFG)" != "misc - Win32 NASM Release" && "$(CFG)" !=\
 "misc - Win32 NASM Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "misc.mak" CFG="misc - Win32 NASM Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "misc - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "misc - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "misc - Win32 NASM Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "misc - Win32 NASM Debug" (based on\
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

!IF  "$(CFG)" == "misc - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\misc.mdf"

!ELSE 

ALL : "fabaselib - Win32 Release" ".\misc.mdf"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fabaselib - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\misc.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\misc.exp"
	-@erase "$(OUTDIR)\misc.lib"
	-@erase ".\misc.mdf"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\\" /I "..\..\..\include" /I\
 "..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I\
 "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I\
 "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\misc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c\
 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\misc.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\misc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib version.lib wsock32.lib kernel32.lib user32.lib\
 gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib\
 oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x10800000"\
 /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\misc.pdb" /machine:I386\
 /def:".\misc.def" /out:"misc.mdf" /implib:"$(OUTDIR)\misc.lib"\
 /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\misc.def"
LINK32_OBJS= \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\misc.res" \
	"..\..\..\..\base\win32\fabaselib.lib"

".\misc.mdf" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "fabaselib - Win32 Release" ".\misc.mdf"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                                  ..\..\..\..\base\win32\prj\plugins
	copy misc.mdf         ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "misc - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\misc.mdf"

!ELSE 

ALL : "fabaselib - Win32 Debug" ".\misc.mdf"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fabaselib - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\misc.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\misc.exp"
	-@erase "$(OUTDIR)\misc.lib"
	-@erase "$(OUTDIR)\misc.pdb"
	-@erase ".\misc.ilk"
	-@erase ".\misc.mdf"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\\" /I "..\..\..\include" /I\
 "..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I\
 "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I\
 "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\misc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c\
 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\misc.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\misc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib version.lib wsock32.lib kernel32.lib user32.lib\
 gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib\
 oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x10800000"\
 /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\misc.pdb" /debug\
 /machine:I386 /def:".\misc.def" /out:"misc.mdf" /implib:"$(OUTDIR)\misc.lib"\
 /pdbtype:sept /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\misc.def"
LINK32_OBJS= \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\misc.res" \
	"..\..\..\..\base\win32\fabaselib.lib"

".\misc.mdf" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "fabaselib - Win32 Debug" ".\misc.mdf"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                                  ..\..\..\..\base\win32\prj\plugins
	copy misc.mdf         ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "misc - Win32 NASM Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\misc.mdf"

!ELSE 

ALL : "fabaselib - Win32 NASM Release" ".\misc.mdf"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fabaselib - Win32 NASM ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\misc.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\misc.exp"
	-@erase "$(OUTDIR)\misc.lib"
	-@erase ".\misc.mdf"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\\" /I "..\..\..\include" /I\
 "..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I\
 "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I\
 "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\misc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c\
 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\misc.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\misc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib version.lib wsock32.lib kernel32.lib user32.lib\
 gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib\
 oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x10800000"\
 /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\misc.pdb" /machine:I386\
 /def:".\misc.def" /out:"misc.mdf" /implib:"$(OUTDIR)\misc.lib"\
 /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\misc.def"
LINK32_OBJS= \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\misc.res" \
	"..\..\..\..\base\win32\fabaselib.lib"

".\misc.mdf" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "fabaselib - Win32 NASM Release" ".\misc.mdf"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                                  ..\..\..\..\base\win32\prj\plugins
	copy misc.mdf         ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "misc - Win32 NASM Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\misc.mdf"

!ELSE 

ALL : "fabaselib - Win32 NASM Debug" ".\misc.mdf"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fabaselib - Win32 NASM DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\misc.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\misc.exp"
	-@erase "$(OUTDIR)\misc.lib"
	-@erase "$(OUTDIR)\misc.pdb"
	-@erase ".\misc.ilk"
	-@erase ".\misc.mdf"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\\" /I "..\..\..\include" /I\
 "..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I\
 "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I\
 "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\misc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c\
 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\misc.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\misc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib version.lib wsock32.lib kernel32.lib user32.lib\
 gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib\
 oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x10800000"\
 /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\misc.pdb" /debug\
 /machine:I386 /def:".\misc.def" /out:"misc.mdf" /implib:"$(OUTDIR)\misc.lib"\
 /pdbtype:sept /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\misc.def"
LINK32_OBJS= \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\misc.res" \
	"..\..\..\..\base\win32\fabaselib.lib"

".\misc.mdf" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "fabaselib - Win32 NASM Debug" ".\misc.mdf"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                                  ..\..\..\..\base\win32\prj\plugins
	copy misc.mdf         ..\..\..\..\base\win32\prj\plugins
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


!IF "$(CFG)" == "misc - Win32 Release" || "$(CFG)" == "misc - Win32 Debug" ||\
 "$(CFG)" == "misc - Win32 NASM Release" || "$(CFG)" ==\
 "misc - Win32 NASM Debug"
SOURCE=..\misc.cpp

!IF  "$(CFG)" == "misc - Win32 Release"

DEP_CPP_MISC_=\
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
	"..\..\..\..\base\include\registrar.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pipeline.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\..\io\include\pullbuffer.h"\
	"..\..\..\..\lmc\include\lmc.h"\
	"..\misc.h"\
	
NODEP_CPP_MISC_=\
	"..\..\..\..\base\include\win32impl.h"\
	

"$(INTDIR)\misc.obj" : $(SOURCE) $(DEP_CPP_MISC_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "misc - Win32 Debug"

DEP_CPP_MISC_=\
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
	"..\..\..\..\base\include\registrar.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pipeline.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\..\io\include\pullbuffer.h"\
	"..\..\..\..\lmc\include\lmc.h"\
	"..\misc.h"\
	

"$(INTDIR)\misc.obj" : $(SOURCE) $(DEP_CPP_MISC_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "misc - Win32 NASM Release"

DEP_CPP_MISC_=\
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
	"..\..\..\..\base\include\registrar.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pipeline.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\..\io\include\pullbuffer.h"\
	"..\..\..\..\lmc\include\lmc.h"\
	"..\misc.h"\
	
NODEP_CPP_MISC_=\
	"..\..\..\..\base\include\win32impl.h"\
	

"$(INTDIR)\misc.obj" : $(SOURCE) $(DEP_CPP_MISC_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "misc - Win32 NASM Debug"

DEP_CPP_MISC_=\
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
	"..\..\..\..\base\include\registrar.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\io\include\pipeline.h"\
	"..\..\..\..\io\include\pmi.h"\
	"..\..\..\..\io\include\pullbuffer.h"\
	"..\..\..\..\lmc\include\lmc.h"\
	"..\misc.h"\
	

"$(INTDIR)\misc.obj" : $(SOURCE) $(DEP_CPP_MISC_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\res\misc.rc

!IF  "$(CFG)" == "misc - Win32 Release"


"$(INTDIR)\misc.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\misc.res" /i\
 "\FreeAmp\freeamp\plm\metadata\misc\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "misc - Win32 Debug"


"$(INTDIR)\misc.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\misc.res" /i\
 "\FreeAmp\freeamp\plm\metadata\misc\res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "misc - Win32 NASM Release"


"$(INTDIR)\misc.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\misc.res" /i\
 "\FreeAmp\freeamp\plm\metadata\misc\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "misc - Win32 NASM Debug"


"$(INTDIR)\misc.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\misc.res" /i\
 "\FreeAmp\freeamp\plm\metadata\misc\res" /d "_DEBUG" $(SOURCE)


!ENDIF 

!IF  "$(CFG)" == "misc - Win32 Release"

"fabaselib - Win32 Release" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 Release" 
   cd "..\..\..\plm\metadata\misc\prj"

"fabaselib - Win32 ReleaseCLEAN" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak\
 CFG="fabaselib - Win32 Release" RECURSE=1 
   cd "..\..\..\plm\metadata\misc\prj"

!ELSEIF  "$(CFG)" == "misc - Win32 Debug"

"fabaselib - Win32 Debug" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 Debug" 
   cd "..\..\..\plm\metadata\misc\prj"

"fabaselib - Win32 DebugCLEAN" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak CFG="fabaselib - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\plm\metadata\misc\prj"

!ELSEIF  "$(CFG)" == "misc - Win32 NASM Release"

"fabaselib - Win32 NASM Release" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak\
 CFG="fabaselib - Win32 NASM Release" 
   cd "..\..\..\plm\metadata\misc\prj"

"fabaselib - Win32 NASM ReleaseCLEAN" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak\
 CFG="fabaselib - Win32 NASM Release" RECURSE=1 
   cd "..\..\..\plm\metadata\misc\prj"

!ELSEIF  "$(CFG)" == "misc - Win32 NASM Debug"

"fabaselib - Win32 NASM Debug" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 NASM Debug" 
   cd "..\..\..\plm\metadata\misc\prj"

"fabaselib - Win32 NASM DebugCLEAN" : 
   cd "\FreeAmp\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak\
 CFG="fabaselib - Win32 NASM Debug" RECURSE=1 
   cd "..\..\..\plm\metadata\misc\prj"

!ENDIF 


!ENDIF 


# Microsoft Developer Studio Generated NMAKE File, Based on pmp300.dsp
!IF "$(CFG)" == ""
CFG=pmp300 - Win32 NASM Debug
!MESSAGE No configuration specified. Defaulting to pmp300 - Win32 NASM Debug.
!ENDIF 

!IF "$(CFG)" != "pmp300 - Win32 Release" && "$(CFG)" != "pmp300 - Win32 Debug"\
 && "$(CFG)" != "pmp300 - Win32 NASM Release" && "$(CFG)" !=\
 "pmp300 - Win32 NASM Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pmp300.mak" CFG="pmp300 - Win32 NASM Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pmp300 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "pmp300 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "pmp300 - Win32 NASM Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "pmp300 - Win32 NASM Debug" (based on\
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

!IF  "$(CFG)" == "pmp300 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\pmp300.ppp"

!ELSE 

ALL : "fabaselib - Win32 Release" "riolib - Win32 Release" ".\pmp300.ppp"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"riolib - Win32 ReleaseCLEAN" "fabaselib - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\pmp300.obj"
	-@erase "$(INTDIR)\pmp300.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\pmp300.exp"
	-@erase "$(OUTDIR)\pmp300.lib"
	-@erase ".\pmp300.ppp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\sba" /I "..\..\\" /I "..\..\..\include"\
 /I "..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I\
 "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I\
 "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\pmp300.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\pmp300.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\pmp300.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib riolib.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll\
 /incremental:no /pdb:"$(OUTDIR)\pmp300.pdb" /machine:I386 /def:".\pmp300.def"\
 /out:"pmp300.ppp" /implib:"$(OUTDIR)\pmp300.lib" /libpath:"..\sba"\
 /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\pmp300.def"
LINK32_OBJS= \
	"$(INTDIR)\pmp300.obj" \
	"$(INTDIR)\pmp300.res" \
	"..\..\..\..\base\win32\fabaselib.lib" \
	"..\sba\riolib.lib"

".\pmp300.ppp" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "fabaselib - Win32 Release" "riolib - Win32 Release"\
 ".\pmp300.ppp"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                               ..\..\..\..\base\win32\prj\plugins
	copy pmp300.ppp      ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "pmp300 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\pmp300.ppp"

!ELSE 

ALL : "fabaselib - Win32 Debug" "riolib - Win32 Debug" ".\pmp300.ppp"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"riolib - Win32 DebugCLEAN" "fabaselib - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\pmp300.obj"
	-@erase "$(INTDIR)\pmp300.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\pmp300.exp"
	-@erase "$(OUTDIR)\pmp300.lib"
	-@erase "$(OUTDIR)\pmp300.pdb"
	-@erase ".\pmp300.ilk"
	-@erase ".\pmp300.ppp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\sba" /I "..\..\\" /I\
 "..\..\..\include" /I "..\..\include" /I "..\..\..\..\io\include" /I\
 "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I\
 "..\..\..\..\config" /I "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include"\
 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\pmp300.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\pmp300.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\pmp300.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib riolib.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll\
 /incremental:yes /pdb:"$(OUTDIR)\pmp300.pdb" /debug /machine:I386\
 /def:".\pmp300.def" /out:"pmp300.ppp" /implib:"$(OUTDIR)\pmp300.lib"\
 /pdbtype:sept /libpath:"..\sba" /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\pmp300.def"
LINK32_OBJS= \
	"$(INTDIR)\pmp300.obj" \
	"$(INTDIR)\pmp300.res" \
	"..\..\..\..\base\win32\fabaselib.lib" \
	"..\sba\riolib.lib"

".\pmp300.ppp" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "fabaselib - Win32 Debug" "riolib - Win32 Debug"\
 ".\pmp300.ppp"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                               ..\..\..\..\base\win32\prj\plugins
	copy pmp300.ppp      ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "pmp300 - Win32 NASM Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\pmp300.ppp"

!ELSE 

ALL : "fabaselib - Win32 NASM Release" "riolib - Win32 NASM Release"\
 ".\pmp300.ppp"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"riolib - Win32 NASM ReleaseCLEAN" "fabaselib - Win32 NASM ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\pmp300.obj"
	-@erase "$(INTDIR)\pmp300.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\pmp300.exp"
	-@erase "$(OUTDIR)\pmp300.lib"
	-@erase ".\pmp300.ppp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\sba" /I "..\..\\" /I "..\..\..\include"\
 /I "..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I\
 "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I\
 "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\pmp300.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\pmp300.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\pmp300.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib riolib.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll\
 /incremental:no /pdb:"$(OUTDIR)\pmp300.pdb" /machine:I386 /def:".\pmp300.def"\
 /out:"pmp300.ppp" /implib:"$(OUTDIR)\pmp300.lib" /libpath:"..\sba"\
 /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\pmp300.def"
LINK32_OBJS= \
	"$(INTDIR)\pmp300.obj" \
	"$(INTDIR)\pmp300.res" \
	"..\..\..\..\base\win32\fabaselib.lib" \
	"..\sba\riolib.lib"

".\pmp300.ppp" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "fabaselib - Win32 NASM Release"\
 "riolib - Win32 NASM Release" ".\pmp300.ppp"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                               ..\..\..\..\base\win32\prj\plugins
	copy pmp300.ppp      ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "pmp300 - Win32 NASM Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\pmp300.ppp"

!ELSE 

ALL : "fabaselib - Win32 NASM Debug" "riolib - Win32 NASM Debug" ".\pmp300.ppp"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"riolib - Win32 NASM DebugCLEAN" "fabaselib - Win32 NASM DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\pmp300.obj"
	-@erase "$(INTDIR)\pmp300.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\pmp300.exp"
	-@erase "$(OUTDIR)\pmp300.lib"
	-@erase "$(OUTDIR)\pmp300.pdb"
	-@erase ".\pmp300.ilk"
	-@erase ".\pmp300.ppp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\sba" /I "..\..\\" /I\
 "..\..\..\include" /I "..\..\include" /I "..\..\..\..\io\include" /I\
 "..\..\..\..\base\include" /I "..\..\..\..\base\win32\include" /I\
 "..\..\..\..\config" /I "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include"\
 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\pmp300.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\pmp300.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\pmp300.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib riolib.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll\
 /incremental:yes /pdb:"$(OUTDIR)\pmp300.pdb" /debug /machine:I386\
 /def:".\pmp300.def" /out:"pmp300.ppp" /implib:"$(OUTDIR)\pmp300.lib"\
 /pdbtype:sept /libpath:"..\sba" /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\pmp300.def"
LINK32_OBJS= \
	"$(INTDIR)\pmp300.obj" \
	"$(INTDIR)\pmp300.res" \
	"..\..\..\..\base\win32\fabaselib.lib" \
	"..\sba\riolib.lib"

".\pmp300.ppp" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "fabaselib - Win32 NASM Debug"\
 "riolib - Win32 NASM Debug" ".\pmp300.ppp"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                               ..\..\..\..\base\win32\prj\plugins
	copy pmp300.ppp      ..\..\..\..\base\win32\prj\plugins
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


!IF "$(CFG)" == "pmp300 - Win32 Release" || "$(CFG)" == "pmp300 - Win32 Debug"\
 || "$(CFG)" == "pmp300 - Win32 NASM Release" || "$(CFG)" ==\
 "pmp300 - Win32 NASM Debug"
SOURCE=..\pmp300.cpp
DEP_CPP_PMP30=\
	"..\..\..\..\base\include\errors.h"\
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
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\config\config.h"\
	"..\pmp300.h"\
	"..\sba\rio.h"\
	"..\sba\std.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\pmp300.obj" : $(SOURCE) $(DEP_CPP_PMP30) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\res\pmp300.rc

!IF  "$(CFG)" == "pmp300 - Win32 Release"


"$(INTDIR)\pmp300.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\pmp300.res" /i\
 "\TEMP\freeamp\plm\portable\pmp300\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "pmp300 - Win32 Debug"


"$(INTDIR)\pmp300.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\pmp300.res" /i\
 "\TEMP\freeamp\plm\portable\pmp300\res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "pmp300 - Win32 NASM Release"


"$(INTDIR)\pmp300.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\pmp300.res" /i\
 "\TEMP\freeamp\plm\portable\pmp300\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "pmp300 - Win32 NASM Debug"


"$(INTDIR)\pmp300.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\pmp300.res" /i\
 "\TEMP\freeamp\plm\portable\pmp300\res" /d "_DEBUG" $(SOURCE)


!ENDIF 

!IF  "$(CFG)" == "pmp300 - Win32 Release"

"riolib - Win32 Release" : 
   cd "\TEMP\freeamp\plm\portable\pmp300\sba\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\riolib.mak CFG="riolib - Win32 Release" 
   cd "..\..\prj"

"riolib - Win32 ReleaseCLEAN" : 
   cd "\TEMP\freeamp\plm\portable\pmp300\sba\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\riolib.mak CFG="riolib - Win32 Release"\
 RECURSE=1 
   cd "..\..\prj"

!ELSEIF  "$(CFG)" == "pmp300 - Win32 Debug"

"riolib - Win32 Debug" : 
   cd "\TEMP\freeamp\plm\portable\pmp300\sba\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\riolib.mak CFG="riolib - Win32 Debug" 
   cd "..\..\prj"

"riolib - Win32 DebugCLEAN" : 
   cd "\TEMP\freeamp\plm\portable\pmp300\sba\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\riolib.mak CFG="riolib - Win32 Debug"\
 RECURSE=1 
   cd "..\..\prj"

!ELSEIF  "$(CFG)" == "pmp300 - Win32 NASM Release"

"riolib - Win32 NASM Release" : 
   cd "\TEMP\freeamp\plm\portable\pmp300\sba\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\riolib.mak CFG="riolib - Win32 NASM Release" 
   cd "..\..\prj"

"riolib - Win32 NASM ReleaseCLEAN" : 
   cd "\TEMP\freeamp\plm\portable\pmp300\sba\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\riolib.mak\
 CFG="riolib - Win32 NASM Release" RECURSE=1 
   cd "..\..\prj"

!ELSEIF  "$(CFG)" == "pmp300 - Win32 NASM Debug"

"riolib - Win32 NASM Debug" : 
   cd "\TEMP\freeamp\plm\portable\pmp300\sba\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\riolib.mak CFG="riolib - Win32 NASM Debug" 
   cd "..\..\prj"

"riolib - Win32 NASM DebugCLEAN" : 
   cd "\TEMP\freeamp\plm\portable\pmp300\sba\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\riolib.mak CFG="riolib - Win32 NASM Debug"\
 RECURSE=1 
   cd "..\..\prj"

!ENDIF 

!IF  "$(CFG)" == "pmp300 - Win32 Release"

"fabaselib - Win32 Release" : 
   cd "\TEMP\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 Release" 
   cd "..\..\..\plm\portable\pmp300\prj"

"fabaselib - Win32 ReleaseCLEAN" : 
   cd "\TEMP\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak\
 CFG="fabaselib - Win32 Release" RECURSE=1 
   cd "..\..\..\plm\portable\pmp300\prj"

!ELSEIF  "$(CFG)" == "pmp300 - Win32 Debug"

"fabaselib - Win32 Debug" : 
   cd "\TEMP\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 Debug" 
   cd "..\..\..\plm\portable\pmp300\prj"

"fabaselib - Win32 DebugCLEAN" : 
   cd "\TEMP\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak CFG="fabaselib - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\plm\portable\pmp300\prj"

!ELSEIF  "$(CFG)" == "pmp300 - Win32 NASM Release"

"fabaselib - Win32 NASM Release" : 
   cd "\TEMP\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak\
 CFG="fabaselib - Win32 NASM Release" 
   cd "..\..\..\plm\portable\pmp300\prj"

"fabaselib - Win32 NASM ReleaseCLEAN" : 
   cd "\TEMP\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak\
 CFG="fabaselib - Win32 NASM Release" RECURSE=1 
   cd "..\..\..\plm\portable\pmp300\prj"

!ELSEIF  "$(CFG)" == "pmp300 - Win32 NASM Debug"

"fabaselib - Win32 NASM Debug" : 
   cd "\TEMP\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 NASM Debug" 
   cd "..\..\..\plm\portable\pmp300\prj"

"fabaselib - Win32 NASM DebugCLEAN" : 
   cd "\TEMP\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak\
 CFG="fabaselib - Win32 NASM Debug" RECURSE=1 
   cd "..\..\..\plm\portable\pmp300\prj"

!ENDIF 


!ENDIF 


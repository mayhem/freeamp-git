# Microsoft Developer Studio Generated NMAKE File, Based on id3v1.dsp
!IF "$(CFG)" == ""
CFG=id3v1 - Win32 NASM Debug
!MESSAGE No configuration specified. Defaulting to id3v1 - Win32 NASM Debug.
!ENDIF 

!IF "$(CFG)" != "id3v1 - Win32 Release" && "$(CFG)" != "id3v1 - Win32 Debug" &&\
 "$(CFG)" != "id3v1 - Win32 NASM Release" && "$(CFG)" !=\
 "id3v1 - Win32 NASM Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "id3v1.mak" CFG="id3v1 - Win32 NASM Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "id3v1 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "id3v1 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "id3v1 - Win32 NASM Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "id3v1 - Win32 NASM Debug" (based on\
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

!IF  "$(CFG)" == "id3v1 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\id3v1.mdf"

!ELSE 

ALL : "fabaselib - Win32 Release" ".\id3v1.mdf"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fabaselib - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\id3v1.obj"
	-@erase "$(INTDIR)\id3v1.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\id3v1.exp"
	-@erase "$(OUTDIR)\id3v1.lib"
	-@erase ".\id3v1.mdf"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\\" /I "..\..\..\include" /I\
 "..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I\
 "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I\
 "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\id3v1.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\id3v1.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\id3v1.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\id3v1.pdb" /machine:I386 /def:".\id3v1.def" /out:"id3v1.mdf"\
 /implib:"$(OUTDIR)\id3v1.lib" /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\id3v1.def"
LINK32_OBJS= \
	"$(INTDIR)\id3v1.obj" \
	"$(INTDIR)\id3v1.res" \
	"..\..\..\..\base\win32\fabaselib.lib"

".\id3v1.mdf" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "fabaselib - Win32 Release" ".\id3v1.mdf"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                             ..\..\..\..\base\win32\prj\plugins
	copy id3v1.mdf    ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "id3v1 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\id3v1.mdf"

!ELSE 

ALL : "fabaselib - Win32 Debug" ".\id3v1.mdf"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fabaselib - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\id3v1.obj"
	-@erase "$(INTDIR)\id3v1.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\id3v1.exp"
	-@erase "$(OUTDIR)\id3v1.lib"
	-@erase "$(OUTDIR)\id3v1.pdb"
	-@erase ".\id3v1.ilk"
	-@erase ".\id3v1.mdf"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\\" /I "..\..\..\include" /I\
 "..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I\
 "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I\
 "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\id3v1.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\id3v1.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\id3v1.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)\id3v1.pdb" /debug /machine:I386 /def:".\id3v1.def"\
 /out:"id3v1.mdf" /implib:"$(OUTDIR)\id3v1.lib" /pdbtype:sept\
 /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\id3v1.def"
LINK32_OBJS= \
	"$(INTDIR)\id3v1.obj" \
	"$(INTDIR)\id3v1.res" \
	"..\..\..\..\base\win32\fabaselib.lib"

".\id3v1.mdf" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "fabaselib - Win32 Debug" ".\id3v1.mdf"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                             ..\..\..\..\base\win32\prj\plugins
	copy id3v1.mdf    ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "id3v1 - Win32 NASM Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\id3v1.mdf"

!ELSE 

ALL : "fabaselib - Win32 NASM Release" ".\id3v1.mdf"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fabaselib - Win32 NASM ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\id3v1.obj"
	-@erase "$(INTDIR)\id3v1.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\id3v1.exp"
	-@erase "$(OUTDIR)\id3v1.lib"
	-@erase ".\id3v1.mdf"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\\" /I "..\..\..\include" /I\
 "..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I\
 "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I\
 "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\id3v1.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\id3v1.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\id3v1.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\id3v1.pdb" /machine:I386 /def:".\id3v1.def" /out:"id3v1.mdf"\
 /implib:"$(OUTDIR)\id3v1.lib" /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\id3v1.def"
LINK32_OBJS= \
	"$(INTDIR)\id3v1.obj" \
	"$(INTDIR)\id3v1.res" \
	"..\..\..\..\base\win32\fabaselib.lib"

".\id3v1.mdf" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "fabaselib - Win32 NASM Release" ".\id3v1.mdf"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                             ..\..\..\..\base\win32\prj\plugins
	copy id3v1.mdf    ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "id3v1 - Win32 NASM Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\id3v1.mdf"

!ELSE 

ALL : "fabaselib - Win32 NASM Debug" ".\id3v1.mdf"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fabaselib - Win32 NASM DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\id3v1.obj"
	-@erase "$(INTDIR)\id3v1.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\id3v1.exp"
	-@erase "$(OUTDIR)\id3v1.lib"
	-@erase "$(OUTDIR)\id3v1.pdb"
	-@erase ".\id3v1.ilk"
	-@erase ".\id3v1.mdf"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\\" /I "..\..\..\include" /I\
 "..\..\include" /I "..\..\..\..\io\include" /I "..\..\..\..\base\include" /I\
 "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I\
 "..\..\..\..\ui\include" /I "..\..\..\..\lmc\include" /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\id3v1.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\id3v1.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\id3v1.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)\id3v1.pdb" /debug /machine:I386 /def:".\id3v1.def"\
 /out:"id3v1.mdf" /implib:"$(OUTDIR)\id3v1.lib" /pdbtype:sept\
 /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\id3v1.def"
LINK32_OBJS= \
	"$(INTDIR)\id3v1.obj" \
	"$(INTDIR)\id3v1.res" \
	"..\..\..\..\base\win32\fabaselib.lib"

".\id3v1.mdf" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "fabaselib - Win32 NASM Debug" ".\id3v1.mdf"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                             ..\..\..\..\base\win32\prj\plugins
	copy id3v1.mdf    ..\..\..\..\base\win32\prj\plugins
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


!IF "$(CFG)" == "id3v1 - Win32 Release" || "$(CFG)" == "id3v1 - Win32 Debug" ||\
 "$(CFG)" == "id3v1 - Win32 NASM Release" || "$(CFG)" ==\
 "id3v1 - Win32 NASM Debug"
SOURCE=..\id3v1.cpp

!IF  "$(CFG)" == "id3v1 - Win32 Release"

DEP_CPP_ID3V1=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\config\config.h"\
	"..\id3v1.h"\
	

"$(INTDIR)\id3v1.obj" : $(SOURCE) $(DEP_CPP_ID3V1) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "id3v1 - Win32 Debug"


"$(INTDIR)\id3v1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "id3v1 - Win32 NASM Release"

DEP_CPP_ID3V1=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\config\config.h"\
	"..\id3v1.h"\
	

"$(INTDIR)\id3v1.obj" : $(SOURCE) $(DEP_CPP_ID3V1) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "id3v1 - Win32 NASM Debug"


"$(INTDIR)\id3v1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\res\id3v1.rc

!IF  "$(CFG)" == "id3v1 - Win32 Release"


"$(INTDIR)\id3v1.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\id3v1.res" /i\
 "\TEMP\freeamp\plm\metadata\id3v1\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "id3v1 - Win32 Debug"


"$(INTDIR)\id3v1.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\id3v1.res" /i\
 "\TEMP\freeamp\plm\metadata\id3v1\res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "id3v1 - Win32 NASM Release"


"$(INTDIR)\id3v1.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\id3v1.res" /i\
 "\TEMP\freeamp\plm\metadata\id3v1\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "id3v1 - Win32 NASM Debug"


"$(INTDIR)\id3v1.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\id3v1.res" /i\
 "\TEMP\freeamp\plm\metadata\id3v1\res" /d "_DEBUG" $(SOURCE)


!ENDIF 

!IF  "$(CFG)" == "id3v1 - Win32 Release"

"fabaselib - Win32 Release" : 
   cd "\TEMP\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 Release" 
   cd "..\..\..\plm\metadata\id3v1\prj"

"fabaselib - Win32 ReleaseCLEAN" : 
   cd "\TEMP\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak\
 CFG="fabaselib - Win32 Release" RECURSE=1 
   cd "..\..\..\plm\metadata\id3v1\prj"

!ELSEIF  "$(CFG)" == "id3v1 - Win32 Debug"

"fabaselib - Win32 Debug" : 
   cd "\TEMP\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 Debug" 
   cd "..\..\..\plm\metadata\id3v1\prj"

"fabaselib - Win32 DebugCLEAN" : 
   cd "\TEMP\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak CFG="fabaselib - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\plm\metadata\id3v1\prj"

!ELSEIF  "$(CFG)" == "id3v1 - Win32 NASM Release"

"fabaselib - Win32 NASM Release" : 
   cd "\TEMP\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak\
 CFG="fabaselib - Win32 NASM Release" 
   cd "..\..\..\plm\metadata\id3v1\prj"

"fabaselib - Win32 NASM ReleaseCLEAN" : 
   cd "\TEMP\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak\
 CFG="fabaselib - Win32 NASM Release" RECURSE=1 
   cd "..\..\..\plm\metadata\id3v1\prj"

!ELSEIF  "$(CFG)" == "id3v1 - Win32 NASM Debug"

"fabaselib - Win32 NASM Debug" : 
   cd "\TEMP\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 NASM Debug" 
   cd "..\..\..\plm\metadata\id3v1\prj"

"fabaselib - Win32 NASM DebugCLEAN" : 
   cd "\TEMP\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak\
 CFG="fabaselib - Win32 NASM Debug" RECURSE=1 
   cd "..\..\..\plm\metadata\id3v1\prj"

!ENDIF 


!ENDIF 


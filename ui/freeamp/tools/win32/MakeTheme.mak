# Microsoft Developer Studio Generated NMAKE File, Based on MakeTheme.dsp
!IF "$(CFG)" == ""
CFG=MakeTheme - Win32 NASM Debug
!MESSAGE No configuration specified. Defaulting to MakeTheme - Win32 NASM\
 Debug.
!ENDIF 

!IF "$(CFG)" != "MakeTheme - Win32 Release" && "$(CFG)" !=\
 "MakeTheme - Win32 Debug" && "$(CFG)" != "MakeTheme - Win32 NASM Debug" &&\
 "$(CFG)" != "MakeTheme - Win32 NASM Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MakeTheme.mak" CFG="MakeTheme - Win32 NASM Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MakeTheme - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "MakeTheme - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "MakeTheme - Win32 NASM Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "MakeTheme - Win32 NASM Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\MakeTheme.exe"

!ELSE 

ALL : ".\MakeTheme.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\adler32.obj"
	-@erase "$(INTDIR)\compress.obj"
	-@erase "$(INTDIR)\crc32.obj"
	-@erase "$(INTDIR)\deflate.obj"
	-@erase "$(INTDIR)\gzio.obj"
	-@erase "$(INTDIR)\infblock.obj"
	-@erase "$(INTDIR)\infcodes.obj"
	-@erase "$(INTDIR)\inffast.obj"
	-@erase "$(INTDIR)\inflate.obj"
	-@erase "$(INTDIR)\inftrees.obj"
	-@erase "$(INTDIR)\infutil.obj"
	-@erase "$(INTDIR)\MakeTheme.obj"
	-@erase "$(INTDIR)\MakeTheme.res"
	-@erase "$(INTDIR)\ThemeZip.obj"
	-@erase "$(INTDIR)\trees.obj"
	-@erase "$(INTDIR)\uncompr.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\zutil.obj"
	-@erase ".\MakeTheme.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "../../include" /I\
 "../../../../lib/zlib/include" /I "../../../../config" /I\
 "../../../../base/include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS"\
 /Fp"$(INTDIR)\MakeTheme.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\MakeTheme.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MakeTheme.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=setargv.obj kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)\MakeTheme.pdb" /machine:I386 /out:"MakeTheme.exe" 
LINK32_OBJS= \
	"$(INTDIR)\adler32.obj" \
	"$(INTDIR)\compress.obj" \
	"$(INTDIR)\crc32.obj" \
	"$(INTDIR)\deflate.obj" \
	"$(INTDIR)\gzio.obj" \
	"$(INTDIR)\infblock.obj" \
	"$(INTDIR)\infcodes.obj" \
	"$(INTDIR)\inffast.obj" \
	"$(INTDIR)\inflate.obj" \
	"$(INTDIR)\inftrees.obj" \
	"$(INTDIR)\infutil.obj" \
	"$(INTDIR)\MakeTheme.obj" \
	"$(INTDIR)\MakeTheme.res" \
	"$(INTDIR)\ThemeZip.obj" \
	"$(INTDIR)\trees.obj" \
	"$(INTDIR)\uncompr.obj" \
	"$(INTDIR)\zutil.obj"

".\MakeTheme.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : ".\MakeTheme.exe"
   IF NOT EXIST ..\..\..\..\base\win32\prj\tools mkdir                                                   ..\..\..\..\base\win32\prj\tools
	copy MakeTheme.exe    ..\..\..\..\base\win32\prj\tools
	copy ..\..\howto\ThemeHowTo.txt    ..\..\..\..\base\win32\prj\tools
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\MakeTheme.exe"

!ELSE 

ALL : ".\MakeTheme.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\adler32.obj"
	-@erase "$(INTDIR)\compress.obj"
	-@erase "$(INTDIR)\crc32.obj"
	-@erase "$(INTDIR)\deflate.obj"
	-@erase "$(INTDIR)\gzio.obj"
	-@erase "$(INTDIR)\infblock.obj"
	-@erase "$(INTDIR)\infcodes.obj"
	-@erase "$(INTDIR)\inffast.obj"
	-@erase "$(INTDIR)\inflate.obj"
	-@erase "$(INTDIR)\inftrees.obj"
	-@erase "$(INTDIR)\infutil.obj"
	-@erase "$(INTDIR)\MakeTheme.obj"
	-@erase "$(INTDIR)\MakeTheme.res"
	-@erase "$(INTDIR)\ThemeZip.obj"
	-@erase "$(INTDIR)\trees.obj"
	-@erase "$(INTDIR)\uncompr.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\zutil.obj"
	-@erase "$(OUTDIR)\MakeTheme.pdb"
	-@erase ".\MakeTheme.exe"
	-@erase ".\MakeTheme.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /I "../../include" /I\
 "../../../../lib/zlib/include" /I "../../../../config" /I\
 "../../../../base/include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS"\
 /Fp"$(INTDIR)\MakeTheme.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\MakeTheme.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MakeTheme.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=wsock32.lib setargv.obj kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)\MakeTheme.pdb" /debug /machine:I386 /out:"MakeTheme.exe"\
 /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\adler32.obj" \
	"$(INTDIR)\compress.obj" \
	"$(INTDIR)\crc32.obj" \
	"$(INTDIR)\deflate.obj" \
	"$(INTDIR)\gzio.obj" \
	"$(INTDIR)\infblock.obj" \
	"$(INTDIR)\infcodes.obj" \
	"$(INTDIR)\inffast.obj" \
	"$(INTDIR)\inflate.obj" \
	"$(INTDIR)\inftrees.obj" \
	"$(INTDIR)\infutil.obj" \
	"$(INTDIR)\MakeTheme.obj" \
	"$(INTDIR)\MakeTheme.res" \
	"$(INTDIR)\ThemeZip.obj" \
	"$(INTDIR)\trees.obj" \
	"$(INTDIR)\uncompr.obj" \
	"$(INTDIR)\zutil.obj"

".\MakeTheme.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : ".\MakeTheme.exe"
   IF NOT EXIST ..\..\..\..\base\win32\prj\tools mkdir                                                   ..\..\..\..\base\win32\prj\tools
	copy MakeTheme.exe    ..\..\..\..\base\win32\prj\tools
	copy ..\..\howto\ThemeHowTo.txt    ..\..\..\..\base\win32\prj\tools
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\MakeTheme.exe"

!ELSE 

ALL : ".\MakeTheme.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\adler32.obj"
	-@erase "$(INTDIR)\compress.obj"
	-@erase "$(INTDIR)\crc32.obj"
	-@erase "$(INTDIR)\deflate.obj"
	-@erase "$(INTDIR)\gzio.obj"
	-@erase "$(INTDIR)\infblock.obj"
	-@erase "$(INTDIR)\infcodes.obj"
	-@erase "$(INTDIR)\inffast.obj"
	-@erase "$(INTDIR)\inflate.obj"
	-@erase "$(INTDIR)\inftrees.obj"
	-@erase "$(INTDIR)\infutil.obj"
	-@erase "$(INTDIR)\MakeTheme.obj"
	-@erase "$(INTDIR)\MakeTheme.res"
	-@erase "$(INTDIR)\ThemeZip.obj"
	-@erase "$(INTDIR)\trees.obj"
	-@erase "$(INTDIR)\uncompr.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\zutil.obj"
	-@erase "$(OUTDIR)\MakeTheme.pdb"
	-@erase ".\MakeTheme.exe"
	-@erase ".\MakeTheme.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /I "../../include" /I\
 "../../../../lib/zlib/include" /I "../../../../config" /I\
 "../../../../base/include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS"\
 /Fp"$(INTDIR)\MakeTheme.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\MakeTheme.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MakeTheme.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=wsock32.lib setargv.obj kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)\MakeTheme.pdb" /debug /machine:I386 /out:"MakeTheme.exe"\
 /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\adler32.obj" \
	"$(INTDIR)\compress.obj" \
	"$(INTDIR)\crc32.obj" \
	"$(INTDIR)\deflate.obj" \
	"$(INTDIR)\gzio.obj" \
	"$(INTDIR)\infblock.obj" \
	"$(INTDIR)\infcodes.obj" \
	"$(INTDIR)\inffast.obj" \
	"$(INTDIR)\inflate.obj" \
	"$(INTDIR)\inftrees.obj" \
	"$(INTDIR)\infutil.obj" \
	"$(INTDIR)\MakeTheme.obj" \
	"$(INTDIR)\MakeTheme.res" \
	"$(INTDIR)\ThemeZip.obj" \
	"$(INTDIR)\trees.obj" \
	"$(INTDIR)\uncompr.obj" \
	"$(INTDIR)\zutil.obj"

".\MakeTheme.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : ".\MakeTheme.exe"
   IF NOT EXIST ..\..\..\..\base\win32\prj\tools mkdir                                                   ..\..\..\..\base\win32\prj\tools
	copy MakeTheme.exe    ..\..\..\..\base\win32\prj\tools
	copy ..\..\howto\ThemeHowTo.txt    ..\..\..\..\base\win32\prj\tools
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\MakeTheme.exe"

!ELSE 

ALL : ".\MakeTheme.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\adler32.obj"
	-@erase "$(INTDIR)\compress.obj"
	-@erase "$(INTDIR)\crc32.obj"
	-@erase "$(INTDIR)\deflate.obj"
	-@erase "$(INTDIR)\gzio.obj"
	-@erase "$(INTDIR)\infblock.obj"
	-@erase "$(INTDIR)\infcodes.obj"
	-@erase "$(INTDIR)\inffast.obj"
	-@erase "$(INTDIR)\inflate.obj"
	-@erase "$(INTDIR)\inftrees.obj"
	-@erase "$(INTDIR)\infutil.obj"
	-@erase "$(INTDIR)\MakeTheme.obj"
	-@erase "$(INTDIR)\MakeTheme.res"
	-@erase "$(INTDIR)\ThemeZip.obj"
	-@erase "$(INTDIR)\trees.obj"
	-@erase "$(INTDIR)\uncompr.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\zutil.obj"
	-@erase ".\MakeTheme.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "../../include" /I\
 "../../../../lib/zlib/include" /I "../../../../config" /I\
 "../../../../base/include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS"\
 /Fp"$(INTDIR)\MakeTheme.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\MakeTheme.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MakeTheme.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=setargv.obj kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)\MakeTheme.pdb" /machine:I386 /out:"MakeTheme.exe" 
LINK32_OBJS= \
	"$(INTDIR)\adler32.obj" \
	"$(INTDIR)\compress.obj" \
	"$(INTDIR)\crc32.obj" \
	"$(INTDIR)\deflate.obj" \
	"$(INTDIR)\gzio.obj" \
	"$(INTDIR)\infblock.obj" \
	"$(INTDIR)\infcodes.obj" \
	"$(INTDIR)\inffast.obj" \
	"$(INTDIR)\inflate.obj" \
	"$(INTDIR)\inftrees.obj" \
	"$(INTDIR)\infutil.obj" \
	"$(INTDIR)\MakeTheme.obj" \
	"$(INTDIR)\MakeTheme.res" \
	"$(INTDIR)\ThemeZip.obj" \
	"$(INTDIR)\trees.obj" \
	"$(INTDIR)\uncompr.obj" \
	"$(INTDIR)\zutil.obj"

".\MakeTheme.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : ".\MakeTheme.exe"
   IF NOT EXIST ..\..\..\..\base\win32\prj\tools mkdir                                                   ..\..\..\..\base\win32\prj\tools
	copy MakeTheme.exe    ..\..\..\..\base\win32\prj\tools
	copy ..\..\howto\ThemeHowTo.txt    ..\..\..\..\base\win32\prj\tools
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


!IF "$(CFG)" == "MakeTheme - Win32 Release" || "$(CFG)" ==\
 "MakeTheme - Win32 Debug" || "$(CFG)" == "MakeTheme - Win32 NASM Debug" ||\
 "$(CFG)" == "MakeTheme - Win32 NASM Release"
SOURCE=..\..\..\..\lib\zlib\src\adler32.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

DEP_CPP_ADLER=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\adler32.obj" : $(SOURCE) $(DEP_CPP_ADLER) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_ADLER=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	

"$(INTDIR)\adler32.obj" : $(SOURCE) $(DEP_CPP_ADLER) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Debug"

DEP_CPP_ADLER=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	

"$(INTDIR)\adler32.obj" : $(SOURCE) $(DEP_CPP_ADLER) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Release"

DEP_CPP_ADLER=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\adler32.obj" : $(SOURCE) $(DEP_CPP_ADLER) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\compress.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

DEP_CPP_COMPR=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\compress.obj" : $(SOURCE) $(DEP_CPP_COMPR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_COMPR=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	

"$(INTDIR)\compress.obj" : $(SOURCE) $(DEP_CPP_COMPR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Debug"

DEP_CPP_COMPR=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	

"$(INTDIR)\compress.obj" : $(SOURCE) $(DEP_CPP_COMPR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Release"

DEP_CPP_COMPR=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\compress.obj" : $(SOURCE) $(DEP_CPP_COMPR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\config\config.win32

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

InputPath=..\..\..\..\config\config.win32

"..\..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\..\config\config.win32 ..\..\..\..\config\config.h

!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

InputPath=..\..\..\..\config\config.win32

"..\..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\..\config\config.win32 ..\..\..\..\config\config.h

!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Debug"

InputPath=..\..\..\..\config\config.win32

"..\..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\..\config\config.win32 ..\..\..\..\config\config.h

!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Release"

InputPath=..\..\..\..\config\config.win32

"..\..\..\..\config\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\..\..\config\config.win32 ..\..\..\..\config\config.h

!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\crc32.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

DEP_CPP_CRC32=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\crc32.obj" : $(SOURCE) $(DEP_CPP_CRC32) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_CRC32=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	

"$(INTDIR)\crc32.obj" : $(SOURCE) $(DEP_CPP_CRC32) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Debug"

DEP_CPP_CRC32=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	

"$(INTDIR)\crc32.obj" : $(SOURCE) $(DEP_CPP_CRC32) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Release"

DEP_CPP_CRC32=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\crc32.obj" : $(SOURCE) $(DEP_CPP_CRC32) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\deflate.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

DEP_CPP_DEFLA=\
	"..\..\..\..\lib\zlib\include\deflate.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\deflate.obj" : $(SOURCE) $(DEP_CPP_DEFLA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_DEFLA=\
	"..\..\..\..\lib\zlib\include\deflate.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\deflate.obj" : $(SOURCE) $(DEP_CPP_DEFLA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Debug"

DEP_CPP_DEFLA=\
	"..\..\..\..\lib\zlib\include\deflate.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\deflate.obj" : $(SOURCE) $(DEP_CPP_DEFLA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Release"

DEP_CPP_DEFLA=\
	"..\..\..\..\lib\zlib\include\deflate.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\deflate.obj" : $(SOURCE) $(DEP_CPP_DEFLA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\gzio.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

DEP_CPP_GZIO_=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\gzio.obj" : $(SOURCE) $(DEP_CPP_GZIO_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_GZIO_=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\gzio.obj" : $(SOURCE) $(DEP_CPP_GZIO_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Debug"

DEP_CPP_GZIO_=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\gzio.obj" : $(SOURCE) $(DEP_CPP_GZIO_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Release"

DEP_CPP_GZIO_=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\gzio.obj" : $(SOURCE) $(DEP_CPP_GZIO_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\infblock.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

DEP_CPP_INFBL=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\infcodes.h"\
	"..\..\..\..\lib\zlib\include\inftrees.h"\
	"..\..\..\..\lib\zlib\include\infutil.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\infblock.obj" : $(SOURCE) $(DEP_CPP_INFBL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_INFBL=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\infcodes.h"\
	"..\..\..\..\lib\zlib\include\inftrees.h"\
	"..\..\..\..\lib\zlib\include\infutil.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\infblock.obj" : $(SOURCE) $(DEP_CPP_INFBL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Debug"

DEP_CPP_INFBL=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\infcodes.h"\
	"..\..\..\..\lib\zlib\include\inftrees.h"\
	"..\..\..\..\lib\zlib\include\infutil.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\infblock.obj" : $(SOURCE) $(DEP_CPP_INFBL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Release"

DEP_CPP_INFBL=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\infcodes.h"\
	"..\..\..\..\lib\zlib\include\inftrees.h"\
	"..\..\..\..\lib\zlib\include\infutil.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\infblock.obj" : $(SOURCE) $(DEP_CPP_INFBL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\infcodes.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

DEP_CPP_INFCO=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\infcodes.h"\
	"..\..\..\..\lib\zlib\include\inffast.h"\
	"..\..\..\..\lib\zlib\include\inftrees.h"\
	"..\..\..\..\lib\zlib\include\infutil.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\infcodes.obj" : $(SOURCE) $(DEP_CPP_INFCO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_INFCO=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\infcodes.h"\
	"..\..\..\..\lib\zlib\include\inffast.h"\
	"..\..\..\..\lib\zlib\include\inftrees.h"\
	"..\..\..\..\lib\zlib\include\infutil.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\infcodes.obj" : $(SOURCE) $(DEP_CPP_INFCO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Debug"

DEP_CPP_INFCO=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\infcodes.h"\
	"..\..\..\..\lib\zlib\include\inffast.h"\
	"..\..\..\..\lib\zlib\include\inftrees.h"\
	"..\..\..\..\lib\zlib\include\infutil.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\infcodes.obj" : $(SOURCE) $(DEP_CPP_INFCO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Release"

DEP_CPP_INFCO=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\infcodes.h"\
	"..\..\..\..\lib\zlib\include\inffast.h"\
	"..\..\..\..\lib\zlib\include\inftrees.h"\
	"..\..\..\..\lib\zlib\include\infutil.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\infcodes.obj" : $(SOURCE) $(DEP_CPP_INFCO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\inffast.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

DEP_CPP_INFFA=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\infcodes.h"\
	"..\..\..\..\lib\zlib\include\inffast.h"\
	"..\..\..\..\lib\zlib\include\inftrees.h"\
	"..\..\..\..\lib\zlib\include\infutil.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\inffast.obj" : $(SOURCE) $(DEP_CPP_INFFA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_INFFA=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\infcodes.h"\
	"..\..\..\..\lib\zlib\include\inffast.h"\
	"..\..\..\..\lib\zlib\include\inftrees.h"\
	"..\..\..\..\lib\zlib\include\infutil.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\inffast.obj" : $(SOURCE) $(DEP_CPP_INFFA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Debug"

DEP_CPP_INFFA=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\infcodes.h"\
	"..\..\..\..\lib\zlib\include\inffast.h"\
	"..\..\..\..\lib\zlib\include\inftrees.h"\
	"..\..\..\..\lib\zlib\include\infutil.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\inffast.obj" : $(SOURCE) $(DEP_CPP_INFFA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Release"

DEP_CPP_INFFA=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\infcodes.h"\
	"..\..\..\..\lib\zlib\include\inffast.h"\
	"..\..\..\..\lib\zlib\include\inftrees.h"\
	"..\..\..\..\lib\zlib\include\infutil.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\inffast.obj" : $(SOURCE) $(DEP_CPP_INFFA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\inflate.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

DEP_CPP_INFLA=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\inflate.obj" : $(SOURCE) $(DEP_CPP_INFLA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_INFLA=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\inflate.obj" : $(SOURCE) $(DEP_CPP_INFLA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Debug"

DEP_CPP_INFLA=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\inflate.obj" : $(SOURCE) $(DEP_CPP_INFLA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Release"

DEP_CPP_INFLA=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\inflate.obj" : $(SOURCE) $(DEP_CPP_INFLA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\inftrees.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

DEP_CPP_INFTR=\
	"..\..\..\..\lib\zlib\include\inffixed.h"\
	"..\..\..\..\lib\zlib\include\inftrees.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\inftrees.obj" : $(SOURCE) $(DEP_CPP_INFTR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_INFTR=\
	"..\..\..\..\lib\zlib\include\inffixed.h"\
	"..\..\..\..\lib\zlib\include\inftrees.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\inftrees.obj" : $(SOURCE) $(DEP_CPP_INFTR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Debug"

DEP_CPP_INFTR=\
	"..\..\..\..\lib\zlib\include\inffixed.h"\
	"..\..\..\..\lib\zlib\include\inftrees.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\inftrees.obj" : $(SOURCE) $(DEP_CPP_INFTR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Release"

DEP_CPP_INFTR=\
	"..\..\..\..\lib\zlib\include\inffixed.h"\
	"..\..\..\..\lib\zlib\include\inftrees.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\inftrees.obj" : $(SOURCE) $(DEP_CPP_INFTR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\infutil.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

DEP_CPP_INFUT=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\infcodes.h"\
	"..\..\..\..\lib\zlib\include\inftrees.h"\
	"..\..\..\..\lib\zlib\include\infutil.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\infutil.obj" : $(SOURCE) $(DEP_CPP_INFUT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_INFUT=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\infcodes.h"\
	"..\..\..\..\lib\zlib\include\inftrees.h"\
	"..\..\..\..\lib\zlib\include\infutil.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\infutil.obj" : $(SOURCE) $(DEP_CPP_INFUT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Debug"

DEP_CPP_INFUT=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\infcodes.h"\
	"..\..\..\..\lib\zlib\include\inftrees.h"\
	"..\..\..\..\lib\zlib\include\infutil.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\infutil.obj" : $(SOURCE) $(DEP_CPP_INFUT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Release"

DEP_CPP_INFUT=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\infcodes.h"\
	"..\..\..\..\lib\zlib\include\inftrees.h"\
	"..\..\..\..\lib\zlib\include\infutil.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\infutil.obj" : $(SOURCE) $(DEP_CPP_INFUT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\MakeTheme.cpp

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

DEP_CPP_MAKET=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\config\config.h"\
	"..\..\include\ThemeZip.h"\
	

"$(INTDIR)\MakeTheme.obj" : $(SOURCE) $(DEP_CPP_MAKET) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_MAKET=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\config\config.h"\
	"..\..\include\ThemeZip.h"\
	

"$(INTDIR)\MakeTheme.obj" : $(SOURCE) $(DEP_CPP_MAKET) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Debug"

DEP_CPP_MAKET=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\include\ThemeZip.h"\
	
NODEP_CPP_MAKET=\
	"..\..\..\..\config\config.h"\
	

"$(INTDIR)\MakeTheme.obj" : $(SOURCE) $(DEP_CPP_MAKET) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Release"

DEP_CPP_MAKET=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\config\config.h"\
	"..\..\include\ThemeZip.h"\
	

"$(INTDIR)\MakeTheme.obj" : $(SOURCE) $(DEP_CPP_MAKET) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\MakeTheme.rc

"$(INTDIR)\MakeTheme.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=..\..\src\ThemeZip.cpp

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

DEP_CPP_THEME=\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\include\ThemeZip.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\ThemeZip.obj" : $(SOURCE) $(DEP_CPP_THEME) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_THEME=\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\include\ThemeZip.h"\
	

"$(INTDIR)\ThemeZip.obj" : $(SOURCE) $(DEP_CPP_THEME) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Debug"

DEP_CPP_THEME=\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\include\ThemeZip.h"\
	
NODEP_CPP_THEME=\
	"..\..\..\..\config\config.h"\
	

"$(INTDIR)\ThemeZip.obj" : $(SOURCE) $(DEP_CPP_THEME) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Release"

DEP_CPP_THEME=\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\include\ThemeZip.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\ThemeZip.obj" : $(SOURCE) $(DEP_CPP_THEME) "$(INTDIR)"\
 "..\..\..\..\config\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\trees.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

DEP_CPP_TREES=\
	"..\..\..\..\lib\zlib\include\deflate.h"\
	"..\..\..\..\lib\zlib\include\trees.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\trees.obj" : $(SOURCE) $(DEP_CPP_TREES) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_TREES=\
	"..\..\..\..\lib\zlib\include\deflate.h"\
	"..\..\..\..\lib\zlib\include\trees.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\trees.obj" : $(SOURCE) $(DEP_CPP_TREES) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Debug"

DEP_CPP_TREES=\
	"..\..\..\..\lib\zlib\include\deflate.h"\
	"..\..\..\..\lib\zlib\include\trees.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\trees.obj" : $(SOURCE) $(DEP_CPP_TREES) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Release"

DEP_CPP_TREES=\
	"..\..\..\..\lib\zlib\include\deflate.h"\
	"..\..\..\..\lib\zlib\include\trees.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\trees.obj" : $(SOURCE) $(DEP_CPP_TREES) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\uncompr.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

DEP_CPP_UNCOM=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\uncompr.obj" : $(SOURCE) $(DEP_CPP_UNCOM) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_UNCOM=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	

"$(INTDIR)\uncompr.obj" : $(SOURCE) $(DEP_CPP_UNCOM) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Debug"

DEP_CPP_UNCOM=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	

"$(INTDIR)\uncompr.obj" : $(SOURCE) $(DEP_CPP_UNCOM) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Release"

DEP_CPP_UNCOM=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\uncompr.obj" : $(SOURCE) $(DEP_CPP_UNCOM) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\zutil.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

DEP_CPP_ZUTIL=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\zutil.obj" : $(SOURCE) $(DEP_CPP_ZUTIL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_ZUTIL=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\zutil.obj" : $(SOURCE) $(DEP_CPP_ZUTIL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Debug"

DEP_CPP_ZUTIL=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\zutil.obj" : $(SOURCE) $(DEP_CPP_ZUTIL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 NASM Release"

DEP_CPP_ZUTIL=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\zutil.obj" : $(SOURCE) $(DEP_CPP_ZUTIL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 


# Microsoft Developer Studio Generated NMAKE File, Based on MakeTheme.dsp
!IF "$(CFG)" == ""
CFG=MakeTheme - Win32 Debug
!MESSAGE No configuration specified. Defaulting to MakeTheme - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MakeTheme - Win32 Release" && "$(CFG)" !=\
 "MakeTheme - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MakeTheme.mak" CFG="MakeTheme - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MakeTheme - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "MakeTheme - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\MakeTheme.exe"

!ELSE 

ALL : "$(OUTDIR)\MakeTheme.exe"

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
	-@erase "$(INTDIR)\ThemeZip.obj"
	-@erase "$(INTDIR)\trees.obj"
	-@erase "$(INTDIR)\uncompr.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\zutil.obj"
	-@erase "$(OUTDIR)\MakeTheme.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR)\MakeTheme.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MakeTheme.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)\MakeTheme.pdb" /machine:I386 /out:"$(OUTDIR)\MakeTheme.exe" 
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
	"$(INTDIR)\ThemeZip.obj" \
	"$(INTDIR)\trees.obj" \
	"$(INTDIR)\uncompr.obj" \
	"$(INTDIR)\zutil.obj" \
	"..\..\..\..\..\..\Program Files\DevStudio\VC\lib\SETARGV.OBJ"

"$(OUTDIR)\MakeTheme.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\MakeTheme.exe"

!ELSE 

ALL : "$(OUTDIR)\MakeTheme.exe"

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
	-@erase "$(INTDIR)\ThemeZip.obj"
	-@erase "$(INTDIR)\trees.obj"
	-@erase "$(INTDIR)\uncompr.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\zutil.obj"
	-@erase "$(OUTDIR)\MakeTheme.exe"
	-@erase "$(OUTDIR)\MakeTheme.ilk"
	-@erase "$(OUTDIR)\MakeTheme.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /I "../../include" /I\
 "../../../../lib/zlib/include" /I "../../../../config" /I\
 "../../../../base/include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS"\
 /Fp"$(INTDIR)\MakeTheme.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MakeTheme.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib wsock32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)\MakeTheme.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\MakeTheme.exe" /pdbtype:sept 
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
	"$(INTDIR)\ThemeZip.obj" \
	"$(INTDIR)\trees.obj" \
	"$(INTDIR)\uncompr.obj" \
	"$(INTDIR)\zutil.obj" \
	"..\..\..\..\..\..\Program Files\DevStudio\VC\lib\SETARGV.OBJ"

"$(OUTDIR)\MakeTheme.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

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
 "MakeTheme - Win32 Debug"
SOURCE=..\..\..\..\lib\zlib\src\adler32.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

NODEP_CPP_ADLER=\
	"..\..\..\..\lib\zlib\src\zlib.h"\
	

"$(INTDIR)\adler32.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_ADLER=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	

"$(INTDIR)\adler32.obj" : $(SOURCE) $(DEP_CPP_ADLER) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\compress.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

NODEP_CPP_COMPR=\
	"..\..\..\..\lib\zlib\src\zlib.h"\
	

"$(INTDIR)\compress.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_COMPR=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	

"$(INTDIR)\compress.obj" : $(SOURCE) $(DEP_CPP_COMPR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\crc32.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

NODEP_CPP_CRC32=\
	"..\..\..\..\lib\zlib\src\zlib.h"\
	

"$(INTDIR)\crc32.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_CRC32=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	

"$(INTDIR)\crc32.obj" : $(SOURCE) $(DEP_CPP_CRC32) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\deflate.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

NODEP_CPP_DEFLA=\
	"..\..\..\..\lib\zlib\src\deflate.h"\
	

"$(INTDIR)\deflate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_DEFLA=\
	"..\..\..\..\lib\zlib\include\deflate.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\deflate.obj" : $(SOURCE) $(DEP_CPP_DEFLA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\gzio.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

NODEP_CPP_GZIO_=\
	"..\..\..\..\lib\zlib\src\zutil.h"\
	

"$(INTDIR)\gzio.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_GZIO_=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\gzio.obj" : $(SOURCE) $(DEP_CPP_GZIO_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\infblock.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

NODEP_CPP_INFBL=\
	"..\..\..\..\lib\zlib\src\infblock.h"\
	"..\..\..\..\lib\zlib\src\infcodes.h"\
	"..\..\..\..\lib\zlib\src\inftrees.h"\
	"..\..\..\..\lib\zlib\src\infutil.h"\
	"..\..\..\..\lib\zlib\src\zutil.h"\
	

"$(INTDIR)\infblock.obj" : $(SOURCE) "$(INTDIR)"
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


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\infcodes.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

NODEP_CPP_INFCO=\
	"..\..\..\..\lib\zlib\src\infblock.h"\
	"..\..\..\..\lib\zlib\src\infcodes.h"\
	"..\..\..\..\lib\zlib\src\inffast.h"\
	"..\..\..\..\lib\zlib\src\inftrees.h"\
	"..\..\..\..\lib\zlib\src\infutil.h"\
	"..\..\..\..\lib\zlib\src\zutil.h"\
	

"$(INTDIR)\infcodes.obj" : $(SOURCE) "$(INTDIR)"
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


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\inffast.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

NODEP_CPP_INFFA=\
	"..\..\..\..\lib\zlib\src\infblock.h"\
	"..\..\..\..\lib\zlib\src\infcodes.h"\
	"..\..\..\..\lib\zlib\src\inffast.h"\
	"..\..\..\..\lib\zlib\src\inftrees.h"\
	"..\..\..\..\lib\zlib\src\infutil.h"\
	"..\..\..\..\lib\zlib\src\zutil.h"\
	

"$(INTDIR)\inffast.obj" : $(SOURCE) "$(INTDIR)"
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


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\inflate.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

NODEP_CPP_INFLA=\
	"..\..\..\..\lib\zlib\src\infblock.h"\
	"..\..\..\..\lib\zlib\src\zutil.h"\
	

"$(INTDIR)\inflate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_INFLA=\
	"..\..\..\..\lib\zlib\include\infblock.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\inflate.obj" : $(SOURCE) $(DEP_CPP_INFLA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\inftrees.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

NODEP_CPP_INFTR=\
	"..\..\..\..\lib\zlib\src\inffixed.h"\
	"..\..\..\..\lib\zlib\src\inftrees.h"\
	"..\..\..\..\lib\zlib\src\zutil.h"\
	

"$(INTDIR)\inftrees.obj" : $(SOURCE) "$(INTDIR)"
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


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\infutil.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

NODEP_CPP_INFUT=\
	"..\..\..\..\lib\zlib\src\infblock.h"\
	"..\..\..\..\lib\zlib\src\infcodes.h"\
	"..\..\..\..\lib\zlib\src\inftrees.h"\
	"..\..\..\..\lib\zlib\src\infutil.h"\
	"..\..\..\..\lib\zlib\src\zutil.h"\
	

"$(INTDIR)\infutil.obj" : $(SOURCE) "$(INTDIR)"
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


!ENDIF 

SOURCE=..\src\MakeTheme.cpp

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

NODEP_CPP_MAKET=\
	"..\src\ThemeZip.h"\
	

"$(INTDIR)\MakeTheme.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_MAKET=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\config\config.h"\
	"..\..\include\ThemeZip.h"\
	

"$(INTDIR)\MakeTheme.obj" : $(SOURCE) $(DEP_CPP_MAKET) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\ThemeZip.cpp

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

NODEP_CPP_THEME=\
	"..\..\src\config.h"\
	"..\..\src\ThemeZip.h"\
	"..\..\src\zlib.h"\
	

"$(INTDIR)\ThemeZip.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_THEME=\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\include\ThemeZip.h"\
	

"$(INTDIR)\ThemeZip.obj" : $(SOURCE) $(DEP_CPP_THEME) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\trees.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

NODEP_CPP_TREES=\
	"..\..\..\..\lib\zlib\src\deflate.h"\
	"..\..\..\..\lib\zlib\src\trees.h"\
	

"$(INTDIR)\trees.obj" : $(SOURCE) "$(INTDIR)"
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


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\uncompr.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

NODEP_CPP_UNCOM=\
	"..\..\..\..\lib\zlib\src\zlib.h"\
	

"$(INTDIR)\uncompr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_UNCOM=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	

"$(INTDIR)\uncompr.obj" : $(SOURCE) $(DEP_CPP_UNCOM) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\lib\zlib\src\zutil.c

!IF  "$(CFG)" == "MakeTheme - Win32 Release"

NODEP_CPP_ZUTIL=\
	"..\..\..\..\lib\zlib\src\zutil.h"\
	

"$(INTDIR)\zutil.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MakeTheme - Win32 Debug"

DEP_CPP_ZUTIL=\
	"..\..\..\..\lib\zlib\include\zconf.h"\
	"..\..\..\..\lib\zlib\include\zlib.h"\
	"..\..\..\..\lib\zlib\include\zutil.h"\
	

"$(INTDIR)\zutil.obj" : $(SOURCE) $(DEP_CPP_ZUTIL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 


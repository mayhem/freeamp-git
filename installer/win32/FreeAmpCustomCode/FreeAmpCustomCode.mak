# Microsoft Developer Studio Generated NMAKE File, Based on FreeAmpCustomCode.dsp
!IF "$(CFG)" == ""
CFG=FreeAmpCustomCode - Win32 Debug
!MESSAGE No configuration specified. Defaulting to FreeAmpCustomCode - Win32\
 Debug.
!ENDIF 

!IF "$(CFG)" != "FreeAmpCustomCode - Win32 Release" && "$(CFG)" !=\
 "FreeAmpCustomCode - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FreeAmpCustomCode.mak" CFG="FreeAmpCustomCode - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FreeAmpCustomCode - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FreeAmpCustomCode - Win32 Debug" (based on\
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

!IF  "$(CFG)" == "FreeAmpCustomCode - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : \
"..\Setup Files\Compressed Files\0009-English\Intel 32\FreeAmpCustomCode.dll"

!ELSE 

ALL : \
"..\Setup Files\Compressed Files\0009-English\Intel 32\FreeAmpCustomCode.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DDEUtilities.obj"
	-@erase "$(INTDIR)\FreeAmpCustomCode.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\FreeAmpCustomCode.exp"
	-@erase "$(OUTDIR)\FreeAmpCustomCode.lib"
	-@erase "..\Setup Files\Compressed Files\0009-English\Intel 32\FreeAmpCustomCode.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\FreeAmpCustomCode.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\FreeAmpCustomCode.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\FreeAmpCustomCode.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\FreeAmpCustomCode.pdb" /machine:I386\
 /def:".\FreeAmpCustomCode.def"\
 /out:"..\Setup Files\Compressed Files\0009-English\Intel 32\FreeAmpCustomCode.dll"\
 /implib:"$(OUTDIR)\FreeAmpCustomCode.lib" 
DEF_FILE= \
	".\FreeAmpCustomCode.def"
LINK32_OBJS= \
	"$(INTDIR)\DDEUtilities.obj" \
	"$(INTDIR)\FreeAmpCustomCode.res"

"..\Setup Files\Compressed Files\0009-English\Intel 32\FreeAmpCustomCode.dll" :\
 "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "FreeAmpCustomCode - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : \
"..\Setup Files\Compressed Files\0009-English\Intel 32\FreeAmpCustomCode.dll"

!ELSE 

ALL : \
"..\Setup Files\Compressed Files\0009-English\Intel 32\FreeAmpCustomCode.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DDEUtilities.obj"
	-@erase "$(INTDIR)\FreeAmpCustomCode.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\FreeAmpCustomCode.exp"
	-@erase "$(OUTDIR)\FreeAmpCustomCode.lib"
	-@erase "$(OUTDIR)\FreeAmpCustomCode.pdb"
	-@erase "..\Setup Files\Compressed Files\0009-English\Intel 32\FreeAmpCustomCode.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\FreeAmpCustomCode.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\FreeAmpCustomCode.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\FreeAmpCustomCode.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\FreeAmpCustomCode.pdb" /debug /machine:I386\
 /def:".\FreeAmpCustomCode.def"\
 /out:"..\Setup Files\Compressed Files\0009-English\Intel 32\FreeAmpCustomCode.dll"\
 /implib:"$(OUTDIR)\FreeAmpCustomCode.lib" /pdbtype:sept 
DEF_FILE= \
	".\FreeAmpCustomCode.def"
LINK32_OBJS= \
	"$(INTDIR)\DDEUtilities.obj" \
	"$(INTDIR)\FreeAmpCustomCode.res"

"..\Setup Files\Compressed Files\0009-English\Intel 32\FreeAmpCustomCode.dll" :\
 "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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


!IF "$(CFG)" == "FreeAmpCustomCode - Win32 Release" || "$(CFG)" ==\
 "FreeAmpCustomCode - Win32 Debug"
SOURCE=.\DDEUtilities.cpp
DEP_CPP_DDEUT=\
	".\ddeutilities.h"\
	

"$(INTDIR)\DDEUtilities.obj" : $(SOURCE) $(DEP_CPP_DDEUT) "$(INTDIR)"


SOURCE=.\FreeAmpCustomCode.rc

"$(INTDIR)\FreeAmpCustomCode.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 


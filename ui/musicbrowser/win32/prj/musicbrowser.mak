# Microsoft Developer Studio Generated NMAKE File, Based on musicbrowser.dsp
!IF "$(CFG)" == ""
CFG=musicbrowser - Win32 NASM Debug
!MESSAGE No configuration specified. Defaulting to musicbrowser - Win32 NASM\
 Debug.
!ENDIF 

!IF "$(CFG)" != "musicbrowser - Win32 Release" && "$(CFG)" !=\
 "musicbrowser - Win32 Debug" && "$(CFG)" != "musicbrowser - Win32 NASM Debug"\
 && "$(CFG)" != "musicbrowser - Win32 NASM Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "musicbrowser.mak" CFG="musicbrowser - Win32 NASM Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "musicbrowser - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "musicbrowser - Win32 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "musicbrowser - Win32 NASM Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "musicbrowser - Win32 NASM Release" (based on\
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

!IF  "$(CFG)" == "musicbrowser - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\musicbrowser.ui"

!ELSE 

ALL : "gdbm - Win32 Release" "fabaselib - Win32 Release" ".\musicbrowser.ui"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fabaselib - Win32 ReleaseCLEAN" "gdbm - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\DataIndex.obj"
	-@erase "$(INTDIR)\Dialog.obj"
	-@erase "$(INTDIR)\DropObject.obj"
	-@erase "$(INTDIR)\DropSource.obj"
	-@erase "$(INTDIR)\DropTarget.obj"
	-@erase "$(INTDIR)\EditTrackInfoDialog.obj"
	-@erase "$(INTDIR)\EnumFormatEtc.obj"
	-@erase "$(INTDIR)\Event.obj"
	-@erase "$(INTDIR)\FooCast.obj"
	-@erase "$(INTDIR)\IntroductionWizard.obj"
	-@erase "$(INTDIR)\MultiSelectTreeView.obj"
	-@erase "$(INTDIR)\musicbrowser.res"
	-@erase "$(INTDIR)\MusicSearchDialog.obj"
	-@erase "$(INTDIR)\MusicTree.obj"
	-@erase "$(INTDIR)\OpenSavePlaylists.obj"
	-@erase "$(INTDIR)\PlaylistView.obj"
	-@erase "$(INTDIR)\RemoveTracksDialog.obj"
	-@erase "$(INTDIR)\SavePlaylistDialog.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\Win32MusicBrowser.obj"
	-@erase "$(OUTDIR)\musicbrowser.exp"
	-@erase "$(OUTDIR)\musicbrowser.lib"
	-@erase ".\musicbrowser.ui"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\include" /I "..\..\include" /I\
 "..\..\..\include" /I "..\..\..\..\base\include" /I\
 "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I\
 "..\..\..\..\ui\include" /I "..\res" /I "..\..\..\..\lib\gdbm" /I\
 "..\..\..\..\lmc\include" /I "..\..\..\..\io\include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\musicbrowser.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\musicbrowser.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\musicbrowser.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib gdbm.lib comctl32.lib version.lib wsock32.lib\
 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo\
 /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\musicbrowser.pdb"\
 /machine:I386 /def:".\musicbrowser.def" /out:"musicbrowser.ui"\
 /implib:"$(OUTDIR)\musicbrowser.lib" /libpath:"..\..\..\..\lib\gdbm"\
 /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\musicbrowser.def"
LINK32_OBJS= \
	"$(INTDIR)\DataIndex.obj" \
	"$(INTDIR)\Dialog.obj" \
	"$(INTDIR)\DropObject.obj" \
	"$(INTDIR)\DropSource.obj" \
	"$(INTDIR)\DropTarget.obj" \
	"$(INTDIR)\EditTrackInfoDialog.obj" \
	"$(INTDIR)\EnumFormatEtc.obj" \
	"$(INTDIR)\Event.obj" \
	"$(INTDIR)\FooCast.obj" \
	"$(INTDIR)\IntroductionWizard.obj" \
	"$(INTDIR)\MultiSelectTreeView.obj" \
	"$(INTDIR)\musicbrowser.res" \
	"$(INTDIR)\MusicSearchDialog.obj" \
	"$(INTDIR)\MusicTree.obj" \
	"$(INTDIR)\OpenSavePlaylists.obj" \
	"$(INTDIR)\PlaylistView.obj" \
	"$(INTDIR)\RemoveTracksDialog.obj" \
	"$(INTDIR)\SavePlaylistDialog.obj" \
	"$(INTDIR)\Win32MusicBrowser.obj" \
	"..\..\..\..\base\win32\fabaselib.lib" \
	"..\..\..\..\lib\gdbm\gdbm.lib"

".\musicbrowser.ui" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "gdbm - Win32 Release" "fabaselib - Win32 Release"\
 ".\musicbrowser.ui"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                                                                                                  ..\..\..\..\base\win32\prj\plugins
	copy musicbrowser.ui                                                                                    ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "musicbrowser - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\musicbrowser.ui"

!ELSE 

ALL : "gdbm - Win32 Debug" "fabaselib - Win32 Debug" ".\musicbrowser.ui"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fabaselib - Win32 DebugCLEAN" "gdbm - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\DataIndex.obj"
	-@erase "$(INTDIR)\Dialog.obj"
	-@erase "$(INTDIR)\DropObject.obj"
	-@erase "$(INTDIR)\DropSource.obj"
	-@erase "$(INTDIR)\DropTarget.obj"
	-@erase "$(INTDIR)\EditTrackInfoDialog.obj"
	-@erase "$(INTDIR)\EnumFormatEtc.obj"
	-@erase "$(INTDIR)\Event.obj"
	-@erase "$(INTDIR)\FooCast.obj"
	-@erase "$(INTDIR)\IntroductionWizard.obj"
	-@erase "$(INTDIR)\MultiSelectTreeView.obj"
	-@erase "$(INTDIR)\musicbrowser.res"
	-@erase "$(INTDIR)\MusicSearchDialog.obj"
	-@erase "$(INTDIR)\MusicTree.obj"
	-@erase "$(INTDIR)\OpenSavePlaylists.obj"
	-@erase "$(INTDIR)\PlaylistView.obj"
	-@erase "$(INTDIR)\RemoveTracksDialog.obj"
	-@erase "$(INTDIR)\SavePlaylistDialog.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\Win32MusicBrowser.obj"
	-@erase "$(OUTDIR)\musicbrowser.exp"
	-@erase "$(OUTDIR)\musicbrowser.lib"
	-@erase "$(OUTDIR)\musicbrowser.pdb"
	-@erase ".\musicbrowser.ilk"
	-@erase ".\musicbrowser.ui"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\include" /I "..\..\include" /I\
 "..\..\..\include" /I "..\..\..\..\base\include" /I\
 "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I\
 "..\..\..\..\ui\include" /I "..\res" /I "..\..\..\..\lib\gdbm" /I\
 "..\..\..\..\lmc\include" /I "..\..\..\..\io\include" /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\musicbrowser.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\musicbrowser.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\musicbrowser.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib gdbm.lib comctl32.lib version.lib wsock32.lib\
 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo\
 /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\musicbrowser.pdb"\
 /debug /machine:I386 /def:".\musicbrowser.def" /out:"musicbrowser.ui"\
 /implib:"$(OUTDIR)\musicbrowser.lib" /pdbtype:sept\
 /libpath:"..\..\..\..\lib\gdbm" /libpath:"..\..\..\..\base\win32" 
LINK32_OBJS= \
	"$(INTDIR)\DataIndex.obj" \
	"$(INTDIR)\Dialog.obj" \
	"$(INTDIR)\DropObject.obj" \
	"$(INTDIR)\DropSource.obj" \
	"$(INTDIR)\DropTarget.obj" \
	"$(INTDIR)\EditTrackInfoDialog.obj" \
	"$(INTDIR)\EnumFormatEtc.obj" \
	"$(INTDIR)\Event.obj" \
	"$(INTDIR)\FooCast.obj" \
	"$(INTDIR)\IntroductionWizard.obj" \
	"$(INTDIR)\MultiSelectTreeView.obj" \
	"$(INTDIR)\musicbrowser.res" \
	"$(INTDIR)\MusicSearchDialog.obj" \
	"$(INTDIR)\MusicTree.obj" \
	"$(INTDIR)\OpenSavePlaylists.obj" \
	"$(INTDIR)\PlaylistView.obj" \
	"$(INTDIR)\RemoveTracksDialog.obj" \
	"$(INTDIR)\SavePlaylistDialog.obj" \
	"$(INTDIR)\Win32MusicBrowser.obj" \
	"..\..\..\..\base\win32\fabaselib.lib" \
	"..\..\..\..\lib\gdbm\gdbm.lib"

".\musicbrowser.ui" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "gdbm - Win32 Debug" "fabaselib - Win32 Debug"\
 ".\musicbrowser.ui"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                                                                                                  ..\..\..\..\base\win32\prj\plugins
	copy musicbrowser.ui                                                                                    ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\musicbrowser.ui"

!ELSE 

ALL : "gdbm - Win32 NASM Debug" "fabaselib - Win32 NASM Debug"\
 ".\musicbrowser.ui"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fabaselib - Win32 NASM DebugCLEAN" "gdbm - Win32 NASM DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\DataIndex.obj"
	-@erase "$(INTDIR)\Dialog.obj"
	-@erase "$(INTDIR)\DropObject.obj"
	-@erase "$(INTDIR)\DropSource.obj"
	-@erase "$(INTDIR)\DropTarget.obj"
	-@erase "$(INTDIR)\EditTrackInfoDialog.obj"
	-@erase "$(INTDIR)\EnumFormatEtc.obj"
	-@erase "$(INTDIR)\Event.obj"
	-@erase "$(INTDIR)\FooCast.obj"
	-@erase "$(INTDIR)\IntroductionWizard.obj"
	-@erase "$(INTDIR)\MultiSelectTreeView.obj"
	-@erase "$(INTDIR)\musicbrowser.res"
	-@erase "$(INTDIR)\MusicSearchDialog.obj"
	-@erase "$(INTDIR)\MusicTree.obj"
	-@erase "$(INTDIR)\OpenSavePlaylists.obj"
	-@erase "$(INTDIR)\PlaylistView.obj"
	-@erase "$(INTDIR)\RemoveTracksDialog.obj"
	-@erase "$(INTDIR)\SavePlaylistDialog.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\Win32MusicBrowser.obj"
	-@erase "$(OUTDIR)\musicbrowser.exp"
	-@erase "$(OUTDIR)\musicbrowser.lib"
	-@erase "$(OUTDIR)\musicbrowser.pdb"
	-@erase ".\musicbrowser.ilk"
	-@erase ".\musicbrowser.ui"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\include" /I "..\..\include" /I\
 "..\..\..\include" /I "..\..\..\..\base\include" /I\
 "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I\
 "..\..\..\..\ui\include" /I "..\res" /I "..\..\..\..\lib\gdbm" /I\
 "..\..\..\..\lmc\include" /I "..\..\..\..\io\include" /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\musicbrowser.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\musicbrowser.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\musicbrowser.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib gdbm.lib comctl32.lib version.lib wsock32.lib\
 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo\
 /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\musicbrowser.pdb"\
 /debug /machine:I386 /def:".\musicbrowser.def" /out:"musicbrowser.ui"\
 /implib:"$(OUTDIR)\musicbrowser.lib" /pdbtype:sept\
 /libpath:"..\..\..\..\lib\gdbm" /libpath:"..\..\..\..\base\win32" 
LINK32_OBJS= \
	"$(INTDIR)\DataIndex.obj" \
	"$(INTDIR)\Dialog.obj" \
	"$(INTDIR)\DropObject.obj" \
	"$(INTDIR)\DropSource.obj" \
	"$(INTDIR)\DropTarget.obj" \
	"$(INTDIR)\EditTrackInfoDialog.obj" \
	"$(INTDIR)\EnumFormatEtc.obj" \
	"$(INTDIR)\Event.obj" \
	"$(INTDIR)\FooCast.obj" \
	"$(INTDIR)\IntroductionWizard.obj" \
	"$(INTDIR)\MultiSelectTreeView.obj" \
	"$(INTDIR)\musicbrowser.res" \
	"$(INTDIR)\MusicSearchDialog.obj" \
	"$(INTDIR)\MusicTree.obj" \
	"$(INTDIR)\OpenSavePlaylists.obj" \
	"$(INTDIR)\PlaylistView.obj" \
	"$(INTDIR)\RemoveTracksDialog.obj" \
	"$(INTDIR)\SavePlaylistDialog.obj" \
	"$(INTDIR)\Win32MusicBrowser.obj" \
	"..\..\..\..\base\win32\fabaselib.lib" \
	"..\..\..\..\lib\gdbm\gdbm.lib"

".\musicbrowser.ui" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "gdbm - Win32 NASM Debug" "fabaselib - Win32 NASM Debug"\
 ".\musicbrowser.ui"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                                                                                                  ..\..\..\..\base\win32\prj\plugins
	copy musicbrowser.ui                                                                                    ..\..\..\..\base\win32\prj\plugins
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\musicbrowser.ui"

!ELSE 

ALL : "gdbm - Win32 NASM Release" "fabaselib - Win32 NASM Release"\
 ".\musicbrowser.ui"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"fabaselib - Win32 NASM ReleaseCLEAN" "gdbm - Win32 NASM ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\DataIndex.obj"
	-@erase "$(INTDIR)\Dialog.obj"
	-@erase "$(INTDIR)\DropObject.obj"
	-@erase "$(INTDIR)\DropSource.obj"
	-@erase "$(INTDIR)\DropTarget.obj"
	-@erase "$(INTDIR)\EditTrackInfoDialog.obj"
	-@erase "$(INTDIR)\EnumFormatEtc.obj"
	-@erase "$(INTDIR)\Event.obj"
	-@erase "$(INTDIR)\FooCast.obj"
	-@erase "$(INTDIR)\IntroductionWizard.obj"
	-@erase "$(INTDIR)\MultiSelectTreeView.obj"
	-@erase "$(INTDIR)\musicbrowser.res"
	-@erase "$(INTDIR)\MusicSearchDialog.obj"
	-@erase "$(INTDIR)\MusicTree.obj"
	-@erase "$(INTDIR)\OpenSavePlaylists.obj"
	-@erase "$(INTDIR)\PlaylistView.obj"
	-@erase "$(INTDIR)\RemoveTracksDialog.obj"
	-@erase "$(INTDIR)\SavePlaylistDialog.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\Win32MusicBrowser.obj"
	-@erase "$(OUTDIR)\musicbrowser.exp"
	-@erase "$(OUTDIR)\musicbrowser.lib"
	-@erase ".\musicbrowser.ui"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\include" /I "..\..\include" /I\
 "..\..\..\include" /I "..\..\..\..\base\include" /I\
 "..\..\..\..\base\win32\include" /I "..\..\..\..\config" /I\
 "..\..\..\..\ui\include" /I "..\res" /I "..\..\..\..\lib\gdbm" /I\
 "..\..\..\..\lmc\include" /I "..\..\..\..\io\include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\musicbrowser.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\musicbrowser.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\musicbrowser.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=fabaselib.lib gdbm.lib comctl32.lib version.lib wsock32.lib\
 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo\
 /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\musicbrowser.pdb"\
 /machine:I386 /def:".\musicbrowser.def" /out:"musicbrowser.ui"\
 /implib:"$(OUTDIR)\musicbrowser.lib" /libpath:"..\..\..\..\lib\gdbm"\
 /libpath:"..\..\..\..\base\win32" 
DEF_FILE= \
	".\musicbrowser.def"
LINK32_OBJS= \
	"$(INTDIR)\DataIndex.obj" \
	"$(INTDIR)\Dialog.obj" \
	"$(INTDIR)\DropObject.obj" \
	"$(INTDIR)\DropSource.obj" \
	"$(INTDIR)\DropTarget.obj" \
	"$(INTDIR)\EditTrackInfoDialog.obj" \
	"$(INTDIR)\EnumFormatEtc.obj" \
	"$(INTDIR)\Event.obj" \
	"$(INTDIR)\FooCast.obj" \
	"$(INTDIR)\IntroductionWizard.obj" \
	"$(INTDIR)\MultiSelectTreeView.obj" \
	"$(INTDIR)\musicbrowser.res" \
	"$(INTDIR)\MusicSearchDialog.obj" \
	"$(INTDIR)\MusicTree.obj" \
	"$(INTDIR)\OpenSavePlaylists.obj" \
	"$(INTDIR)\PlaylistView.obj" \
	"$(INTDIR)\RemoveTracksDialog.obj" \
	"$(INTDIR)\SavePlaylistDialog.obj" \
	"$(INTDIR)\Win32MusicBrowser.obj" \
	"..\..\..\..\base\win32\fabaselib.lib" \
	"..\..\..\..\lib\gdbm\gdbm.lib"

".\musicbrowser.ui" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "gdbm - Win32 NASM Release"\
 "fabaselib - Win32 NASM Release" ".\musicbrowser.ui"
   IF NOT EXIST ..\..\..\..\base\win32\prj\plugins mkdir                                                                                                  ..\..\..\..\base\win32\prj\plugins
	copy musicbrowser.ui                                                                                    ..\..\..\..\base\win32\prj\plugins
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


!IF "$(CFG)" == "musicbrowser - Win32 Release" || "$(CFG)" ==\
 "musicbrowser - Win32 Debug" || "$(CFG)" == "musicbrowser - Win32 NASM Debug"\
 || "$(CFG)" == "musicbrowser - Win32 NASM Release"
SOURCE=..\src\DataIndex.cpp

!IF  "$(CFG)" == "musicbrowser - Win32 Release"

DEP_CPP_DATAI=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\DataIndex.obj" : $(SOURCE) $(DEP_CPP_DATAI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 Debug"

DEP_CPP_DATAI=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\DataIndex.obj" : $(SOURCE) $(DEP_CPP_DATAI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Debug"

DEP_CPP_DATAI=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\DataIndex.obj" : $(SOURCE) $(DEP_CPP_DATAI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Release"

DEP_CPP_DATAI=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\DataIndex.obj" : $(SOURCE) $(DEP_CPP_DATAI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Dialog.cpp

!IF  "$(CFG)" == "musicbrowser - Win32 Release"

DEP_CPP_DIALO=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\base\win32\include\help.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\Dialog.obj" : $(SOURCE) $(DEP_CPP_DIALO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 Debug"

DEP_CPP_DIALO=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\base\win32\include\help.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\Dialog.obj" : $(SOURCE) $(DEP_CPP_DIALO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Debug"

DEP_CPP_DIALO=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\base\win32\include\help.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\Dialog.obj" : $(SOURCE) $(DEP_CPP_DIALO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Release"

DEP_CPP_DIALO=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\base\win32\include\help.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\Dialog.obj" : $(SOURCE) $(DEP_CPP_DIALO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\DropObject.cpp

!IF  "$(CFG)" == "musicbrowser - Win32 Release"

DEP_CPP_DROPO=\
	"..\include\DropObject.h"\
	"..\include\EnumFormatEtc.h"\
	

"$(INTDIR)\DropObject.obj" : $(SOURCE) $(DEP_CPP_DROPO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 Debug"

DEP_CPP_DROPO=\
	"..\include\DropObject.h"\
	"..\include\EnumFormatEtc.h"\
	

"$(INTDIR)\DropObject.obj" : $(SOURCE) $(DEP_CPP_DROPO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Debug"

DEP_CPP_DROPO=\
	"..\include\DropObject.h"\
	"..\include\EnumFormatEtc.h"\
	

"$(INTDIR)\DropObject.obj" : $(SOURCE) $(DEP_CPP_DROPO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Release"

DEP_CPP_DROPO=\
	"..\include\DropObject.h"\
	"..\include\EnumFormatEtc.h"\
	

"$(INTDIR)\DropObject.obj" : $(SOURCE) $(DEP_CPP_DROPO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\DropSource.cpp
DEP_CPP_DROPS=\
	"..\include\DropSource.h"\
	

"$(INTDIR)\DropSource.obj" : $(SOURCE) $(DEP_CPP_DROPS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\DropTarget.cpp
DEP_CPP_DROPT=\
	"..\include\DropObject.h"\
	"..\include\DropTarget.h"\
	

"$(INTDIR)\DropTarget.obj" : $(SOURCE) $(DEP_CPP_DROPT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\EditTrackInfoDialog.cpp

!IF  "$(CFG)" == "musicbrowser - Win32 Release"

DEP_CPP_EDITT=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\help.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\include\DataIndex.h"\
	"..\include\EditTrackInfoDialog.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\EditTrackInfoDialog.obj" : $(SOURCE) $(DEP_CPP_EDITT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 Debug"

DEP_CPP_EDITT=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\help.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\include\DataIndex.h"\
	"..\include\EditTrackInfoDialog.h"\
	

"$(INTDIR)\EditTrackInfoDialog.obj" : $(SOURCE) $(DEP_CPP_EDITT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Debug"

DEP_CPP_EDITT=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\help.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\include\DataIndex.h"\
	"..\include\EditTrackInfoDialog.h"\
	

"$(INTDIR)\EditTrackInfoDialog.obj" : $(SOURCE) $(DEP_CPP_EDITT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Release"

DEP_CPP_EDITT=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\include\utility.h"\
	"..\..\..\..\base\win32\include\help.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\include\DataIndex.h"\
	"..\include\EditTrackInfoDialog.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\EditTrackInfoDialog.obj" : $(SOURCE) $(DEP_CPP_EDITT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\EnumFormatEtc.cpp
DEP_CPP_ENUMF=\
	"..\include\EnumFormatEtc.h"\
	

"$(INTDIR)\EnumFormatEtc.obj" : $(SOURCE) $(DEP_CPP_ENUMF) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Event.cpp

!IF  "$(CFG)" == "musicbrowser - Win32 Release"

DEP_CPP_EVENT=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\EditTrackInfoDialog.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\Event.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 Debug"

DEP_CPP_EVENT=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\EditTrackInfoDialog.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\Event.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Debug"

DEP_CPP_EVENT=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\EditTrackInfoDialog.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\Event.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Release"

DEP_CPP_EVENT=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\EditTrackInfoDialog.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\Event.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\FooCast.cpp

!IF  "$(CFG)" == "musicbrowser - Win32 Release"

DEP_CPP_FOOCA=\
	"..\..\..\..\config\config.h"\
	"..\include\FooCast.h"\
	

"$(INTDIR)\FooCast.obj" : $(SOURCE) $(DEP_CPP_FOOCA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 Debug"

DEP_CPP_FOOCA=\
	"..\..\..\..\config\config.h"\
	"..\include\FooCast.h"\
	

"$(INTDIR)\FooCast.obj" : $(SOURCE) $(DEP_CPP_FOOCA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Debug"

DEP_CPP_FOOCA=\
	"..\..\..\..\config\config.h"\
	"..\include\FooCast.h"\
	

"$(INTDIR)\FooCast.obj" : $(SOURCE) $(DEP_CPP_FOOCA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Release"

DEP_CPP_FOOCA=\
	"..\..\..\..\config\config.h"\
	"..\include\FooCast.h"\
	

"$(INTDIR)\FooCast.obj" : $(SOURCE) $(DEP_CPP_FOOCA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\IntroductionWizard.cpp

!IF  "$(CFG)" == "musicbrowser - Win32 Release"

DEP_CPP_INTRO=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\IntroductionWizard.obj" : $(SOURCE) $(DEP_CPP_INTRO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 Debug"

DEP_CPP_INTRO=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\IntroductionWizard.obj" : $(SOURCE) $(DEP_CPP_INTRO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Debug"

DEP_CPP_INTRO=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\IntroductionWizard.obj" : $(SOURCE) $(DEP_CPP_INTRO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Release"

DEP_CPP_INTRO=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
	"..\..\..\..\base\include\playlist.h"\
	"..\..\..\..\base\include\playlistformat.h"\
	"..\..\..\..\base\include\plmevent.h"\
	"..\..\..\..\base\include\portabledevice.h"\
	"..\..\..\..\base\include\preferences.h"\
	"..\..\..\..\base\include\properties.h"\
	"..\..\..\..\base\include\registry.h"\
	"..\..\..\..\base\include\thread.h"\
	"..\..\..\..\base\include\undomanager.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\IntroductionWizard.obj" : $(SOURCE) $(DEP_CPP_INTRO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\MultiSelectTreeView.cpp

!IF  "$(CFG)" == "musicbrowser - Win32 Release"

DEP_CPP_MULTI=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropObject.h"\
	"..\include\DropSource.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\MultiSelectTreeView.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 Debug"

DEP_CPP_MULTI=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropObject.h"\
	"..\include\DropSource.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\MultiSelectTreeView.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Debug"

DEP_CPP_MULTI=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropObject.h"\
	"..\include\DropSource.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\MultiSelectTreeView.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Release"

DEP_CPP_MULTI=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropObject.h"\
	"..\include\DropSource.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\MultiSelectTreeView.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\MusicSearchDialog.cpp

!IF  "$(CFG)" == "musicbrowser - Win32 Release"

DEP_CPP_MUSIC=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\base\win32\include\help.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\MusicSearchDialog.obj" : $(SOURCE) $(DEP_CPP_MUSIC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 Debug"

DEP_CPP_MUSIC=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\base\win32\include\help.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\MusicSearchDialog.obj" : $(SOURCE) $(DEP_CPP_MUSIC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Debug"

DEP_CPP_MUSIC=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\base\win32\include\help.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\MusicSearchDialog.obj" : $(SOURCE) $(DEP_CPP_MUSIC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Release"

DEP_CPP_MUSIC=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\base\win32\include\help.h"\
	"..\..\..\..\base\win32\include\mutex.h"\
	"..\..\..\..\base\win32\include\semaphore.h"\
	"..\..\..\..\config\config.h"\
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\MusicSearchDialog.obj" : $(SOURCE) $(DEP_CPP_MUSIC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\MusicTree.cpp

!IF  "$(CFG)" == "musicbrowser - Win32 Release"

DEP_CPP_MUSICT=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropObject.h"\
	"..\include\DropSource.h"\
	"..\include\DropTarget.h"\
	"..\include\FooCast.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	
NODEP_CPP_MUSICT=\
	"..\..\..\..\base\include\win32impl.h"\
	

"$(INTDIR)\MusicTree.obj" : $(SOURCE) $(DEP_CPP_MUSICT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 Debug"

DEP_CPP_MUSICT=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropObject.h"\
	"..\include\DropSource.h"\
	"..\include\DropTarget.h"\
	"..\include\FooCast.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\MusicTree.obj" : $(SOURCE) $(DEP_CPP_MUSICT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Debug"

DEP_CPP_MUSICT=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropObject.h"\
	"..\include\DropSource.h"\
	"..\include\DropTarget.h"\
	"..\include\FooCast.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\MusicTree.obj" : $(SOURCE) $(DEP_CPP_MUSICT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Release"

DEP_CPP_MUSICT=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropObject.h"\
	"..\include\DropSource.h"\
	"..\include\DropTarget.h"\
	"..\include\FooCast.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	
NODEP_CPP_MUSICT=\
	"..\..\..\..\base\include\win32impl.h"\
	

"$(INTDIR)\MusicTree.obj" : $(SOURCE) $(DEP_CPP_MUSICT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\OpenSavePlaylists.cpp

!IF  "$(CFG)" == "musicbrowser - Win32 Release"

DEP_CPP_OPENS=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\OpenSavePlaylists.obj" : $(SOURCE) $(DEP_CPP_OPENS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 Debug"

DEP_CPP_OPENS=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\OpenSavePlaylists.obj" : $(SOURCE) $(DEP_CPP_OPENS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Debug"

DEP_CPP_OPENS=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\OpenSavePlaylists.obj" : $(SOURCE) $(DEP_CPP_OPENS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Release"

DEP_CPP_OPENS=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\OpenSavePlaylists.obj" : $(SOURCE) $(DEP_CPP_OPENS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\PlaylistView.cpp

!IF  "$(CFG)" == "musicbrowser - Win32 Release"

DEP_CPP_PLAYL=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropObject.h"\
	"..\include\DropSource.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\PlaylistView.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 Debug"

DEP_CPP_PLAYL=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropObject.h"\
	"..\include\DropSource.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\PlaylistView.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Debug"

DEP_CPP_PLAYL=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropObject.h"\
	"..\include\DropSource.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\PlaylistView.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Release"

DEP_CPP_PLAYL=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropObject.h"\
	"..\include\DropSource.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\PlaylistView.obj" : $(SOURCE) $(DEP_CPP_PLAYL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\RemoveTracksDialog.cpp

!IF  "$(CFG)" == "musicbrowser - Win32 Release"

DEP_CPP_REMOV=\
	"..\..\..\..\config\config.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\RemoveTracksDialog.obj" : $(SOURCE) $(DEP_CPP_REMOV) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 Debug"

DEP_CPP_REMOV=\
	"..\..\..\..\config\config.h"\
	

"$(INTDIR)\RemoveTracksDialog.obj" : $(SOURCE) $(DEP_CPP_REMOV) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Debug"

DEP_CPP_REMOV=\
	"..\..\..\..\config\config.h"\
	

"$(INTDIR)\RemoveTracksDialog.obj" : $(SOURCE) $(DEP_CPP_REMOV) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Release"

DEP_CPP_REMOV=\
	"..\..\..\..\config\config.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\RemoveTracksDialog.obj" : $(SOURCE) $(DEP_CPP_REMOV) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\SavePlaylistDialog.cpp

!IF  "$(CFG)" == "musicbrowser - Win32 Release"

DEP_CPP_SAVEP=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\SavePlaylistDialog.obj" : $(SOURCE) $(DEP_CPP_SAVEP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 Debug"

DEP_CPP_SAVEP=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\SavePlaylistDialog.obj" : $(SOURCE) $(DEP_CPP_SAVEP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Debug"

DEP_CPP_SAVEP=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\SavePlaylistDialog.obj" : $(SOURCE) $(DEP_CPP_SAVEP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Release"

DEP_CPP_SAVEP=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\SavePlaylistDialog.obj" : $(SOURCE) $(DEP_CPP_SAVEP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Win32MusicBrowser.cpp

!IF  "$(CFG)" == "musicbrowser - Win32 Release"

DEP_CPP_WIN32=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\Win32MusicBrowser.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 Debug"

DEP_CPP_WIN32=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\Win32MusicBrowser.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Debug"

DEP_CPP_WIN32=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	

"$(INTDIR)\Win32MusicBrowser.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Release"

DEP_CPP_WIN32=\
	"..\..\..\..\base\include\database.h"\
	"..\..\..\..\base\include\debug.h"\
	"..\..\..\..\base\include\errors.h"\
	"..\..\..\..\base\include\event.h"\
	"..\..\..\..\base\include\eventdata.h"\
	"..\..\..\..\base\include\facontext.h"\
	"..\..\..\..\base\include\log.h"\
	"..\..\..\..\base\include\metadata.h"\
	"..\..\..\..\base\include\musiccatalog.h"\
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
	"..\..\..\..\lib\gdbm\gdbm_fa.h"\
	"..\..\..\include\ui.h"\
	"..\include\DataIndex.h"\
	"..\include\DropTarget.h"\
	"..\include\Win32MusicBrowser.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\Win32MusicBrowser.obj" : $(SOURCE) $(DEP_CPP_WIN32) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\res\musicbrowser.rc
DEP_RSC_MUSICB=\
	"..\res\album.bmp"\
	"..\res\all.bmp"\
	"..\res\artist.bmp"\
	"..\res\busy.bmp"\
	"..\res\catalog.bmp"\
	"..\res\drag.cur"\
	"..\res\icecast.bmp"\
	"..\res\icon1.ico"\
	"..\res\playing.bmp"\
	"..\res\playlist.bmp"\
	"..\res\portable.bmp"\
	"..\res\shoutcast.bmp"\
	"..\res\splitter.cur"\
	"..\res\streams.bmp"\
	"..\res\toolbar.bmp"\
	"..\res\track.bmp"\
	"..\res\uncatagorized.bmp"\
	"..\res\warn.ico"\
	"..\res\wiredplanet.bmp"\
	"..\res\wizard.bmp"\
	"..\res\wizard_small.bmp"\
	

!IF  "$(CFG)" == "musicbrowser - Win32 Release"


"$(INTDIR)\musicbrowser.res" : $(SOURCE) $(DEP_RSC_MUSICB) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\musicbrowser.res" /i\
 "\Local\src\freeamp\ui\musicbrowser\win32\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 Debug"


"$(INTDIR)\musicbrowser.res" : $(SOURCE) $(DEP_RSC_MUSICB) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\musicbrowser.res" /i\
 "\Local\src\freeamp\ui\musicbrowser\win32\res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Debug"


"$(INTDIR)\musicbrowser.res" : $(SOURCE) $(DEP_RSC_MUSICB) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\musicbrowser.res" /i\
 "\Local\src\freeamp\ui\musicbrowser\win32\res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Release"


"$(INTDIR)\musicbrowser.res" : $(SOURCE) $(DEP_RSC_MUSICB) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\musicbrowser.res" /i\
 "\Local\src\freeamp\ui\musicbrowser\win32\res" /d "NDEBUG" $(SOURCE)


!ENDIF 

!IF  "$(CFG)" == "musicbrowser - Win32 Release"

"fabaselib - Win32 Release" : 
   cd "\Local\src\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 Release" 
   cd "..\..\..\ui\musicbrowser\win32\prj"

"fabaselib - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak\
 CFG="fabaselib - Win32 Release" RECURSE=1 
   cd "..\..\..\ui\musicbrowser\win32\prj"

!ELSEIF  "$(CFG)" == "musicbrowser - Win32 Debug"

"fabaselib - Win32 Debug" : 
   cd "\Local\src\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 Debug" 
   cd "..\..\..\ui\musicbrowser\win32\prj"

"fabaselib - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak CFG="fabaselib - Win32 Debug"\
 RECURSE=1 
   cd "..\..\..\ui\musicbrowser\win32\prj"

!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Debug"

"fabaselib - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak CFG="fabaselib - Win32 NASM Debug" 
   cd "..\..\..\ui\musicbrowser\win32\prj"

"fabaselib - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak\
 CFG="fabaselib - Win32 NASM Debug" RECURSE=1 
   cd "..\..\..\ui\musicbrowser\win32\prj"

!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Release"

"fabaselib - Win32 NASM Release" : 
   cd "\Local\src\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\fabaselib.mak\
 CFG="fabaselib - Win32 NASM Release" 
   cd "..\..\..\ui\musicbrowser\win32\prj"

"fabaselib - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\base\win32\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\fabaselib.mak\
 CFG="fabaselib - Win32 NASM Release" RECURSE=1 
   cd "..\..\..\ui\musicbrowser\win32\prj"

!ENDIF 

!IF  "$(CFG)" == "musicbrowser - Win32 Release"

"gdbm - Win32 Release" : 
   cd "\Local\src\freeamp\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\gdbm.mak CFG="gdbm - Win32 Release" 
   cd "..\..\..\ui\musicbrowser\win32\prj"

"gdbm - Win32 ReleaseCLEAN" : 
   cd "\Local\src\freeamp\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\gdbm.mak CFG="gdbm - Win32 Release"\
 RECURSE=1 
   cd "..\..\..\ui\musicbrowser\win32\prj"

!ELSEIF  "$(CFG)" == "musicbrowser - Win32 Debug"

"gdbm - Win32 Debug" : 
   cd "\Local\src\freeamp\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\gdbm.mak CFG="gdbm - Win32 Debug" 
   cd "..\..\..\ui\musicbrowser\win32\prj"

"gdbm - Win32 DebugCLEAN" : 
   cd "\Local\src\freeamp\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\gdbm.mak CFG="gdbm - Win32 Debug" RECURSE=1\
 
   cd "..\..\..\ui\musicbrowser\win32\prj"

!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Debug"

"gdbm - Win32 NASM Debug" : 
   cd "\Local\src\freeamp\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\gdbm.mak CFG="gdbm - Win32 NASM Debug" 
   cd "..\..\..\ui\musicbrowser\win32\prj"

"gdbm - Win32 NASM DebugCLEAN" : 
   cd "\Local\src\freeamp\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\gdbm.mak CFG="gdbm - Win32 NASM Debug"\
 RECURSE=1 
   cd "..\..\..\ui\musicbrowser\win32\prj"

!ELSEIF  "$(CFG)" == "musicbrowser - Win32 NASM Release"

"gdbm - Win32 NASM Release" : 
   cd "\Local\src\freeamp\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) /F .\gdbm.mak CFG="gdbm - Win32 NASM Release" 
   cd "..\..\..\ui\musicbrowser\win32\prj"

"gdbm - Win32 NASM ReleaseCLEAN" : 
   cd "\Local\src\freeamp\lib\gdbm\prj"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F .\gdbm.mak CFG="gdbm - Win32 NASM Release"\
 RECURSE=1 
   cd "..\..\..\ui\musicbrowser\win32\prj"

!ENDIF 


!ENDIF 


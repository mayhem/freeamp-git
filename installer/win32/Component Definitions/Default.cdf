[System DLLs]
SELECTED=Yes
FILENEED=CRITICAL
HTTPLOCATION=
STATUS=
UNINSTALLABLE=Yes
TARGET=<WINSYSDIR>
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=These components are System DLLs you might need in order to run FreeAmp.
DISPLAYTEXT=
IMAGE=
DEFSELECTION=Yes
filegroup0=System DLLs
requiredby0=Program Files
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=NEWERVERSION\NEWERDATE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=Windows Operating System\Windows System Folder

[Input and Output\HTTP Input]
SELECTED=Yes
FILENEED=HIGHLYRECOMMENDED
HTTPLOCATION=
STATUS=
UNINSTALLABLE=Yes
TARGET=<TARGETDIR>\Plugins
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=This component allows FreeAmp to receive data from HTTP and ShoutCast servers.
DISPLAYTEXT=
IMAGE=
DEFSELECTION=Yes
filegroup0=HTTP Input
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=ALWAYSOVERWRITE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=General Application Destination\Plugins

[Components]
component9=User Interfaces
component0=Input and Output\HTTP Input
component1=System DLLs
component10=Input and Output\File Input
component2=User Interfaces\FreeAmp
component11=User Interfaces\RainPlay
component3=Decoders
component12=Input and Output
component4=Input and Output\Sound Card Output
component13=Input and Output\DirectSound Output
component5=Input and Output\OBS Input
component6=Program Files
component7=Decoders\Xing Decoder
component8=User Interfaces\Simple

[TopComponents]
component0=Program Files
component1=Input and Output
component2=Decoders
component3=User Interfaces
component4=System DLLs

[SetupType]
setuptype0=Typical
setuptype1=Custom

[Decoders]
SELECTED=Yes
FILENEED=HIGHLYRECOMMENDED
HTTPLOCATION=
STATUS=
UNINSTALLABLE=Yes
TARGET=<TARGETDIR>
member0=Decoders\Xing Decoder
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=These components allow FreeAmp to decode various audio formats.
DISPLAYTEXT=
IMAGE=
DEFSELECTION=Yes
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=ALWAYSOVERWRITE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=General Application Destination

[User Interfaces\FreeAmp]
SELECTED=Yes
FILENEED=HIGHLYRECOMMENDED
HTTPLOCATION=
STATUS=
UNINSTALLABLE=Yes
TARGET=<TARGETDIR>\Plugins
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=This component is the default FreeAmp user interface.
DISPLAYTEXT=
IMAGE=
DEFSELECTION=Yes
filegroup0=FreeAmp UI
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=ALWAYSOVERWRITE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=General Application Destination\Plugins

[Input and Output\Sound Card Output]
SELECTED=Yes
FILENEED=HIGHLYRECOMMENDED
HTTPLOCATION=
STATUS=
UNINSTALLABLE=Yes
TARGET=<TARGETDIR>\Plugins
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=This component allows FreeAmp to play audio through your sound card.
DISPLAYTEXT=
IMAGE=
DEFSELECTION=Yes
filegroup0=Sound Card Output
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=ALWAYSOVERWRITE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=General Application Destination\Plugins

[Decoders\Xing Decoder]
SELECTED=Yes
FILENEED=STANDARD
HTTPLOCATION=
STATUS=
UNINSTALLABLE=Yes
TARGET=<TARGETDIR>\Plugins
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=This component allows FreeAmp to play MPEG 1 (layer 1, 2, and 3), 2, and 2.5 audio streams.
DISPLAYTEXT=
IMAGE=
DEFSELECTION=Yes
filegroup0=Xing Decoder
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=ALWAYSOVERWRITE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=General Application Destination

[Program Files]
required0=System DLLs
SELECTED=Yes
FILENEED=CRITICAL
HTTPLOCATION=
STATUS=
UNINSTALLABLE=Yes
TARGET=<TARGETDIR>
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=This component is the core FreeAmp executable.
DISPLAYTEXT=
IMAGE=
DEFSELECTION=Yes
filegroup0=Program Executable Files
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=SAMEORNEWERDATE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=General Application Destination

[Input and Output\OBS Input]
SELECTED=Yes
FILENEED=HIGHLYRECOMMENDED
HTTPLOCATION=
STATUS=
UNINSTALLABLE=Yes
TARGET=<TARGETDIR>\Plugins
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=This component allows FreeAmp to receive data from Obsequiem and other RTP compliant servers.
DISPLAYTEXT=
IMAGE=
DEFSELECTION=Yes
filegroup0=OBS Input
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=ALWAYSOVERWRITE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=General Application Destination\Plugins

[User Interfaces\Simple]
SELECTED=Yes
FILENEED=STANDARD
HTTPLOCATION=
STATUS=
UNINSTALLABLE=Yes
TARGET=<TARGETDIR>\Plugins
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=This component is a dialog based, screen reader compatible user interface.
DISPLAYTEXT=
IMAGE=
DEFSELECTION=Yes
filegroup0=Simple UI
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=ALWAYSOVERWRITE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=General Application Destination\Plugins

[SetupTypeItem-Custom]
item5=Input and Output\OBS Input
item10=Input and Output
item6=Program Files
item7=Decoders\Xing Decoder
item8=User Interfaces
item9=Input and Output\File Input
Comment=
item0=Input and Output\HTTP Input
item1=System DLLs
item2=User Interfaces\FreeAmp
item3=Decoders
Descrip=This choice allows you to select the plugins you would like to install.
item4=Input and Output\Sound Card Output
DisplayText=

[User Interfaces]
SELECTED=Yes
FILENEED=STANDARD
HTTPLOCATION=
STATUS=
UNINSTALLABLE=Yes
TARGET=<TARGETDIR>
member0=User Interfaces\FreeAmp
member1=User Interfaces\RainPlay
member2=User Interfaces\Simple
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=These components control the way FreeAmp looks and also allow FreeAmp to communicate with various devices.
DISPLAYTEXT=
IMAGE=
DEFSELECTION=Yes
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=ALWAYSOVERWRITE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=General Application Destination

[Input and Output\File Input]
SELECTED=Yes
FILENEED=HIGHLYRECOMMENDED
HTTPLOCATION=
STATUS=
UNINSTALLABLE=Yes
TARGET=<TARGETDIR>\Plugins
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=This component allows FreeAmp to read data from local media files.
DISPLAYTEXT=
IMAGE=
DEFSELECTION=Yes
filegroup0=File Input
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=ALWAYSOVERWRITE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=General Application Destination\Plugins

[User Interfaces\RainPlay]
SELECTED=Yes
FILENEED=STANDARD
HTTPLOCATION=
STATUS=
UNINSTALLABLE=Yes
TARGET=<TARGETDIR>\Plugins
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=This component is an alternate user interface for FreeAmp written by Bill Yuan.
DISPLAYTEXT=
IMAGE=
DEFSELECTION=Yes
filegroup0=RainPlay UI
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=ALWAYSOVERWRITE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=General Application Destination\Plugins

[Info]
Type=CompDef
Version=1.00.000
Name=

[SetupTypeItem-Typical]
item5=Input and Output\OBS Input
item10=Input and Output
item6=Program Files
item7=Decoders\Xing Decoder
item8=User Interfaces
item9=Input and Output\File Input
Comment=
item0=Input and Output\HTTP Input
item1=System DLLs
item2=User Interfaces\FreeAmp
item3=Decoders
Descrip=This choice will install the FreeAmp program and all plugins.
item4=Input and Output\Sound Card Output
DisplayText=

[Input and Output]
SELECTED=Yes
FILENEED=HIGHLYRECOMMENDED
HTTPLOCATION=
STATUS=
UNINSTALLABLE=Yes
TARGET=<TARGETDIR>\Plugins
member0=Input and Output\File Input
member1=Input and Output\HTTP Input
member2=Input and Output\OBS Input
member3=Input and Output\Sound Card Output
member4=Input and Output\DirectSound Output
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=These components allow FreeAmp to receive data from various sources and also to output audio to various destinations.
DISPLAYTEXT=
IMAGE=
DEFSELECTION=Yes
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=ALWAYSOVERWRITE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=General Application Destination\Plugins

[Input and Output\DirectSound Output]
SELECTED=Yes
FILENEED=STANDARD
HTTPLOCATION=
STATUS=
UNINSTALLABLE=Yes
TARGET=<TARGETDIR>\Plugins
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=This component allows FreeAmp to play audio through your sound card via DirectSound on Windows 95 and 98.
DISPLAYTEXT=
IMAGE=
DEFSELECTION=Yes
filegroup0=Direct Sound Output
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=ALWAYSOVERWRITE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=General Application Destination\Plugins


        RIO utility v1.05 - Ashpool Systems (c) 1999
        --------------------------------------------

        Files
        -----
        README          This file
        rio.txt         Protocol information

        std.h           standard defines
        binary.h        defines for binary numbers from 0-255
        rioioctl.h      ioctl constants used for WinNT version
        rio.h           rio class header
        rio.cpp         rio class
        app.cpp         utility which uses rio class

        playlist.txt    example playlist file

        makefile        linux makefile
        winnt.mak       MSVC++ v5.0 makefile for WinNT v4
        win9x.mak       MSVC++ v5.0 makefile for Win9x
        dos.mak         Borland Turbo C++ v1.01 for DOS

        Overview
        --------
        'rio' is a simple command line utility which allows
        you to display and manage files on your Diamond Rio
        portable music player.

        At present the Rio utility only supports the Diamond Rio PMP300
        with 32M internal flash memory. Additional external flash memory
        is not supported.

        command line switches available :-

                -d display directory
                -i initialize
                -u specify file to upload
                -g specify file to download
                -f specify text based playlist file which contains files
                        to be uploaded
                -z delete file
                -p specify parallel port base IO address, default=0x378
                -v enable verbose mode

        examples...

                ; display directory using parallel port at 0x278
                rio -p 0x278 -d

                ; initialize and upload two file
                rio -i -u song.mp3 -u song2.mp3

                ; initialize and upload files in playlist
                rio -i -f playlist.txt

                ; initialize, upload files in playlist and then
                ; display directory
                rio -d -i -f playlist.txt

                ; download file then delete it and finally
                ; display directory
                rio -d -g mp3Files/song.mp3 -z song.mp3

        Building and running
        --------------------

                Linux, GCC
                ----------
                To build and run the application you must be
                logged on as root, then use 'make'.
                (rio utility has been built and tested
                using gcc compiler v2.7.2.3 under Linux v2.0.34)

                Windows NT v4.0, Visual C++ v5
                ------------------------------
                Use 'nmake /fwinnt.mak'.
                (rio utility has been built and tested using
                MS Visual C++ compiler v5 under
                Windows Workstation NT 4.0)

                Windows 95/98, Visual C++ v5
                ----------------------------
                Use 'nmake /fwin9x.mak'.
                (rio utility has been built and tested
                using MS Visual C++ compiler v5 under Windows 98)

                DOS, Borland Turbo C++ v1.01
                ----------------------------
                Use 'make -fdos.mak'
                (rio utility has been built and tested
                using Borland Turbo C++ v1.01 under DOS v6.22 and
                Windows 98).

        Known Bugs
        ----------
        Because of the difference in timings between different machines
        it is possible that you may experience problems when downloading
        and displaying the directory.
        If you encounter this problem try one or both of the following...

                Try doubling the value of IODELAY_1 and/or IODELAY_2
                #define's in mpm.cpp. This will only increase the time
                it takes to download and will not have an effect on
                file upload times.

                Use the utility when the device is powered on as this
                seems to improve IO performance.

        Bug Reports
        -----------
        If you encounter any problems using this utility please send
        e-mail outlining the problem along with details about your
        Rio (memory installed, memory remaining and tracks loaded) to...

		sba@tessier-ashpool.cix.co.uk



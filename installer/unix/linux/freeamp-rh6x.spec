#
# NOTES: This script is intended for building rpms for redhat 6.x
#        systems. Automatic depedency checking has been turned off for
#        this script becuase the rpm will check to see if ALSA/ESD
#        are installed on the system automatically. If we let
#        rpm determine the dependecies automatically it will complain
#        to anyone who does not have ALSA/ESD installed.
#
#        To update this script, remove the Requires line and set
#        AutoReqProv to yes. Then build the RPM, and copy the
#        Requires line that it spits out and paste it where the
#        Requires line is. Make sure to remove references to
#        libesd, libasound, and libncurses, as the install
#        script below will handle them correctly. Finally,
#        Turn AutoReqProv back on and then build the rpm.
# 
Summary: MP3 audio player with streaming support
Name: freeamp
Version: 2.0
Release: 1
Copyright: GPL
Group: Applications/Multimedia
Source: www.freeamp.org:/pub/freeamp/freeamp-1.3.1-linux-x86-glibc2.tar.gz

# Please read note above  
AutoReqProv: No
Requires: ld-linux.so.2 libc.so.6 libdl.so.2 libm.so.6 libnsl.so.1 libpthread.so.0 libstdc++-libc6.1-1.so.2 libX11.so.6 libXext.so.6 libXpm.so.4 libc.so.6(GLIBC_2.0) libm.so.6(GLIBC_2.1) libc.so.6(GLIBC_2.1) libdl.so.2(GLIBC_2.1) libdl.so.2(GLIBC_2.0) libpthread.so.0(GLIBC_2.1) libpthread.so.0(GLIBC_2.0)

%description
This program plays MP3 (MPEG-1 audio layer 3) files and streams. The
player will run on Windows, Linux and Solaris.

%prep
%setup
%build
%install

prefix=/usr

bindir="$prefix/bin"
libdir="$prefix/lib"
fadir="$libdir/freeamp"
plugdir="$fadir/plugins"
sharedir="$prefix/share"
fathemedir="$sharedir/freeamp"
themedir="$fathemedir/themes"

# We could use mkdir -p, but not all unix systems' mkdir's support -p
for dir in "$bindir" "$libdir" "$fadir" "$plugdir" "$sharedir" "$fathemedir" "$themedir"; do
    if [ \! -d "$dir" ]; then
	echo "mkdir $dir"
	mkdir "$dir"
	echo "chown root $dir"
	chown root "$dir"
	echo "chmod 755 $dir"
	chmod 755 "$dir"
    fi
done

install -c -m 755 freeamp "$bindir"
echo "$bindir/freeamp" > installed.files
if test -f "MakeTheme"; then 
    install -c -m 755 MakeTheme "$bindir"
    echo "$bindir/MakeTheme" >> installed.files
fi
for file in themes/*.*; do
    echo "install -c -o root -m 644 $file $themedir"
    install -c -o root -m 644 "$file" "$themedir"
    echo "$themedir/$file" >> installed.files
done
for file in plugins/*.*; do
    enable=f
    case "$file" in
    	*/alsa-*.pmo)
	      if ldconfig -p | grep 'libasound\.so' > /dev/null; then
	      	enable=t
	      else
	      	enable=f
		      rm -f "$fadir/$file"
	      fi
	   ;;
    	*/esound-*.pmo)
	      if ldconfig -p | grep 'libesd\.so' > /dev/null; then
	      	enable=t
	      else
	      	enable=f
		      rm -f "$fadir/$file"
	      fi
	   ;;
    	*/ncurses-*.ui)
	      if ldconfig -p | grep 'libncurses\.so' > /dev/null; then
	      	enable=t
	      else
	      	enable=f
		      rm -f "$fadir/$file"
	      fi
	   ;;
	   *)
	   enable=t
	   ;;
    esac
    if [ "$enable" = t ]; then
        install -c -o root -m 644 "$file" "$plugdir"
        echo $fadir/$file >> installed.files
    fi
done

%files -f installed.files
%dir /usr/lib/freeamp 
%dir /usr/lib/freeamp/plugins
%dir /usr/share/freeamp
%dir /usr/share/freeamp/plugins
%doc AUTHORS CHANGES COPYING ChangeLog INSTALL NEWS README


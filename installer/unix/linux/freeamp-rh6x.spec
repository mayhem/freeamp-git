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
Summary: MP3 audio player with theme user interface and streaming support
Name: freeamp
Version: 2.0.1
Release: 1
Copyright: GPL
Group: Applications/Multimedia
Source: www.freeamp.org:/pub/freeamp/freeamp-2.0.0-linux-x86-glibc2.1.tar.gz

# Please read note above  
AutoReqProv: No
Requires: ld-linux.so.2 libc.so.6 libdl.so.2 libm.so.6 libnsl.so.1 libpthread.so.0 libstdc++-libc6.1-1.so.2 libX11.so.6 libXext.so.6 libgdk-1.2.so.0 libglib-1.2.so.0 libgmodule-1.2.so.0 libgthread-1.2.so.0 libgtk-1.2.so.0 libncurses.so.4 libttf.so.2 libc.so.6(GLIBC_2.0) libm.so.6(GLIBC_2.1) libpthread.so.0(GLIBC_2.1) libpthread.so.0(GLIBC_2.0) libc.so.6(GLIBC_2.1) libdl.so.2(GLIBC_2.1) libdl.so.2(GLIBC_2.0)

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
fasharedir="$sharedir/freeamp"
themedir="$fasharedir/themes"

# We could use mkdir -p, but not all unix systems' mkdir's support -p
for dir in "$bindir" "$libdir" "$fadir" "$plugdir" "$sharedir" "$fasharedir" "$themedir"; do
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
    echo "$fasharedir/$file" >> installed.files
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
gzip -d -c < help/freeamphelp.tar.gz | (cd $fasharedir; rm -rf help; tar -xmf -)
for file in `ls $fasharedir/help`; do
    echo $fasharedir/help/$file >> installed.files
done

%files -f installed.files
%dir /usr/lib/freeamp 
%dir /usr/lib/freeamp/plugins
%dir /usr/share/freeamp
%dir /usr/share/freeamp/themes
%dir /usr/share/freeamp/help
%doc AUTHORS CHANGES COPYING INSTALL NEWS README README.linux


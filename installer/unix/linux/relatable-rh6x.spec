Summary: File and streaming MP3/OggVorbis/Audio CD player/jukebox.
Name: relatable
Version: 2.1.beta6
Release: 1
Copyright: GPL
Group: Applications/Multimedia
Source: ftp://www.freeamp.org/pub/freeamp/src/relatable-%{version}.tar.gz
Prefix: /usr
BuildRoot: /var/tmp/%{name}-%{version}-root
ExcludeArch: sparc
AutoReqProv: No 
Requires: id3lib >= 3.7.8 ld-linux.so.2 libc.so.6 libdl.so.2 libm.so.6 libnsl.so.1 libpthread.so.0 libstdc++-libc6.1-1.so.2 libX11.so.6 libXext.so.6 libgdk-1.2.so.0 libglib-1.2.so.0 libgmodule-1.2.so.0 libgthread-1.2.so.0 libgtk-1.2.so.0 libttf.so.2 libc.so.6(GLIBC_2.0) libm.so.6(GLIBC_2.1) libpthread.so.0(GLIBC_2.1) libpthread.so.0(GLIBC_2.0) libc.so.6(GLIBC_2.1) libdl.so.2(GLIBC_2.1) libdl.so.2(GLIBC_2.0)

%description
The Relatable Player is an MP3/OggVorbis/AudioCD player/jukebox that can play 
local files, SHOUTcast/icecast streams and RTP MPEG streams. It features a 
Themed interface, which also supports WinAmp and KJofol skins, and it also 
supports automated download from RMP enabled music sites like emusic.com.

%prep
%setup -q -n relatable

%build
./configure --prefix=%{prefix}
make

%install
PATH=$PATH:/sbin:/usr/sbin ; export PATH
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT%{prefix}{/bin,/lib,/share}
mkdir -p $RPM_BUILD_ROOT%{prefix}/lib/relatable/plugins
mkdir -p $RPM_BUILD_ROOT%{prefix}/share/relatable/{help,themes}

strip MakeTheme
strip relatable
install -s {MakeTheme,relatable} $RPM_BUILD_ROOT%{prefix}/bin
install themes/* $RPM_BUILD_ROOT%{prefix}/share/relatable/themes
for file in plugins/*.*; do
    enable=f
    case "$file" in
      */alsa*.pmo)
         if ldd $file | grep 'not found' > /dev/null; then
            enable=f
         else
            enable=t
         fi
      ;;
      */esound*.pmo)
         if ldd $file | grep 'not found' > /dev/null; then
            enable=f
         else
            enable=t
         fi
      ;;
      */ncurses*.ui)
         if ldd $file | grep 'not found' > /dev/null; then
            enable=f
         else
            enable=t
         fi
      ;;
      *)
      enable=t
      ;;
    esac
    if [ "$enable" = t ]; then
        strip $file
        install -s $file $RPM_BUILD_ROOT%{prefix}/lib/relatable/plugins
    fi
done  

(cd $RPM_BUILD_ROOT%{prefix}/share/relatable/ ;
tar xfz $RPM_BUILD_DIR/%{name}/help/unix/relatablehelp.tar.gz)

%clean
rm -rf $RPM_BUILD_ROOT

%files 
%defattr(-,root,root)
%doc AUTHORS CHANGES COPYING INSTALL NEWS README README.linux
/usr/bin/*
/usr/lib/relatable 
/usr/share/relatable

%changelog
* Wed Aug  9 2000 Robert Kaye <rob@freeamp.org>
- conversion from freeamp to relatable rpm

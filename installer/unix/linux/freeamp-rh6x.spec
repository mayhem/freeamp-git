Summary: MP3 audio player with theme user interface and streaming support
Name: freeamp
Version: 2.0.4
Release: 1
Copyright: GPL
Group: Applications/Multimedia
Source: ftp://www.freeamp.org/pub/freeamp/src/freeamp-%{version}.tar.gz
Prefix: /usr
BuildRoot: /var/tmp/%{name}-%{version}-root
ExcludeArch: sparc
AutoReqProv: No 
Requires: ld-linux.so.2 libc.so.6 libdl.so.2 libm.so.6 libnsl.so.1 libpthread.so.0 libstdc++-libc6.1-1.so.2 libX11.so.6 libXext.so.6 libgdk-1.2.so.0 libglib-1.2.so.0 libgmodule-1.2.so.0 libgthread-1.2.so.0 libgtk-1.2.so.0 libttf.so.2 libc.so.6(GLIBC_2.0) libm.so.6(GLIBC_2.1) libpthread.so.0(GLIBC_2.1) libpthread.so.0(GLIBC_2.0) libc.so.6(GLIBC_2.1) libdl.so.2(GLIBC_2.1) libdl.so.2(GLIBC_2.0)

%description
FreeAmp is an extensible, cross-platform audio player. It features an
optimized version of the GPLed Xing MPEG decoder which makes it one of the
fastest and best sounding players available. FreeAmp provides a number of the
most common features users have come to expect in a clean, easy to use
interface.

%prep
%setup -q -n freeamp

%build
./configure --prefix=%{prefix}
make

%install
PATH=$PATH:/sbin:/usr/sbin ; export PATH
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT%{prefix}{/bin,/lib,/share}
mkdir -p $RPM_BUILD_ROOT%{prefix}/lib/freeamp/plugins
mkdir -p $RPM_BUILD_ROOT%{prefix}/share/freeamp/{help,themes}

install -s {MakeTheme,freeamp} $RPM_BUILD_ROOT%{prefix}/bin
install themes/* $RPM_BUILD_ROOT%{prefix}/share/freeamp/themes
#install -s plugins/* $RPM_BUILD_ROOT%{prefix}/lib/freeamp/plugins
for file in plugins/*.*; do
    enable=f
    case "$file" in
      */alsa*.pmo)
         if ldconfig -p | grep 'libasound\.so' > /dev/null; then
            enable=t
         else
            enable=f
         fi
      ;;
      */esound*.pmo)
         if ldconfig -p | grep 'libesd\.so' > /dev/null; then
            enable=t
         else
            enable=f
         fi
      ;;
      */ncurses*.ui)
         if ldconfig -p | grep 'libncurses\.so' > /dev/null; then
            enable=t
         else
            enable=f
         fi
      ;;
      *)
      enable=t
      ;;
    esac
    if [ "$enable" = t ]; then
        install -s $file $RPM_BUILD_ROOT%{prefix}/lib/freeamp/plugins
    fi
done  

(cd $RPM_BUILD_ROOT%{prefix}/share/freeamp/ ;
tar xfz $RPM_BUILD_DIR/%{name}/help/unix/freeamphelp.tar.gz)

%clean
rm -rf $RPM_BUILD_ROOT

%files 
%defattr(-,root,root)
%doc AUTHORS CHANGES COPYING INSTALL NEWS README README.linux
/usr/bin/*
/usr/lib/freeamp 
/usr/share/freeamp

%changelog
* Thu Feb 17 2000 Robert Kaye <rob@freeamp.com>
- conversion from emusic to normal freeamp rpm
* Mon Jan 31 2000 Robert Kaye <rob@freeamp.com>
- conversion from freeamp to emusic
* Wed Jan 12 2000 Tim Powers <timp@redhat.com>
- initial build of freeamp for Powertools
- not origional spec file, violates Red Hat policy as to what can be done in
	the install section of the spec file (not bad, just violates policy)



Summary: MP3 audio player with theme user interface and streaming support
Name: emusic
Version: 2.0.2
Release: b1
Copyright: GPL
Group: Applications/Multimedia
Source: ftp://www.freeamp.org/pub/emusic/src/emusic-%{version}-%{release}.tar.gz
Prefix: /usr
BuildRoot: /var/tmp/%{name}-%{version}-root
ExcludeArch: sparc
Requires: audiofile, gtk+ >= 1.2, esound, netscape-communicator

%description
EMusic is an extensible, cross-platform audio player. It features an
optimized version of the GPLed Xing MPEG decoder which makes it one of the
fastest and best sounding players available. EMusic provides a number of the
most common features users have come to expect in a clean, easy to use
interface.

%prep
%setup -q -n emusic

%build
./configure --prefix=%{prefix}
make

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT%{prefix}{/bin,/lib,/share}
mkdir -p $RPM_BUILD_ROOT%{prefix}/lib/emusic/plugins
mkdir -p $RPM_BUILD_ROOT%{prefix}/share/emusic/{help,themes}

install -s {MakeTheme,emusic} $RPM_BUILD_ROOT%{prefix}/bin
install -s plugins/* $RPM_BUILD_ROOT%{prefix}/lib/emusic/plugins
install themes/* $RPM_BUILD_ROOT%{prefix}/share/emusic/themes

(cd $RPM_BUILD_ROOT%{prefix}/share/emusic/ ;
tar xfz $RPM_BUILD_DIR/%{name}/help/unix/emusichelp.tar.gz)

%clean
rm -rf $RPM_BUILD_ROOT

%files 
%defattr(-,root,root)
%doc AUTHORS CHANGES COPYING INSTALL NEWS README README.linux
/usr/bin/*
/usr/lib/emusic 
/usr/share/emusic

%changelog
* Mon Jan 31 2000 Robert Kaye <rob@emusic.com>
- conversion from freeamp to emusic
* Wed Jan 12 2000 Tim Powers <timp@redhat.com>
- initial build of freeamp for Powertools
- not origional spec file, violates Red Hat policy as to what can be done in
	the install section of the spec file (not bad, just violates policy)

# -------------------------------------------------------------------------
# Global defines
# -------------------------------------------------------------------------
%define name    freeamp
%define cname   FreeAmp
%define version 2.1.rc5
%define release 1
%define prefix  /usr  

# -------------------------------------------------------------------------
# Main Package defines
# -------------------------------------------------------------------------
Summary:     File and streaming MP3/OggVorbis/Audio CD player/jukebox.
Name:        %{name}
Version:     %{version}
Release:     %{release}
Copyright:   GPL
Group:       Applications/Multimedia
Source:      ftp://www.freeamp.org/pub/freeamp/src/%{name}-%{version}.tar.gz
Prefix:      %{prefix}
BuildRoot:   /var/tmp/%{name}-%{version}-root
ExcludeArch: sparc
AutoReqProv: No 
Requires: id3lib >= 3.7.13 musicbrainz >= 1.0.0pre6 ld-linux.so.2 libc.so.6 libdl.so.2 libm.so.6 libnsl.so.1 libpthread.so.0 libstdc++-libc6.1-1.so.2 libX11.so.6 libXext.so.6 libgdk-1.2.so.0 libglib-1.2.so.0 libgmodule-1.2.so.0 libgthread-1.2.so.0 libgtk-1.2.so.0 libttf.so.2 libc.so.6(GLIBC_2.0) libm.so.6(GLIBC_2.1) libpthread.so.0(GLIBC_2.1) libpthread.so.0(GLIBC_2.0) libc.so.6(GLIBC_2.1) libdl.so.2(GLIBC_2.1) libdl.so.2(GLIBC_2.0)

%description
%{cname} is an MP3/OggVorbis/AudioCD player/jukebox that can play local files,
SHOUTcast/icecast streams and RTP MPEG streams. It features a Themed interface,
which also supports WinAmp and KJofol skins, and it also supports automated
download from RMP enabled music sites like emusic.com.

# -------------------------------------------------------------------------
# ESD Package defines
# -------------------------------------------------------------------------
%package     esd
Summary:     %{cname} - %{cname} plugin for ESD output
Group:       Applications/Multimedia
Requires:    %{name} = %{version}
Requires:    esound >= 0.2.12

%description    esd
%{cname} plugin for ESD output

# -------------------------------------------------------------------------
# Alsa Package defines
# -------------------------------------------------------------------------
%package     alsa
Summary:     %{cname} - %{cname} plugin for ALSA output
Group:       Applications/Multimedia
Requires:    %{name} = %{version}
Requires:    alsa >= 0.5.0

%description alsa
%{cname} plugin for ALSA output

# -------------------------------------------------------------------------
# Extras Package defines
# -------------------------------------------------------------------------
%package     extras
Summary:     Freeamp - All the remaining plugins (mpg123 ui, cmdline ui, ncurses ui, irman, lcd)
Group:       Applications/Multimedia
Requires:    %{name} = %{version}

%description extras
All the remaining plugins (mpg123 ui, cmdline ui, ncurses ui, irman, lcd)

# -------------------------------------------------------------------------
# Corba Package defines
# -------------------------------------------------------------------------
%package     corba
Summary:     Freeamp - Corba remote control interface
Group:       Applications/Multimedia
Requires:    %{name} = %{version}
Requires:    ORBit >= 0.5.0

%description extras
Corba remote control interface plugin

# -------------------------------------------------------------------------
# Prep and build stuff 
# -------------------------------------------------------------------------
%prep
%setup -q -n %{name} 

%build
./configure --prefix=%{prefix}
make

# -------------------------------------------------------------------------
# Install section
# -------------------------------------------------------------------------
%install
PATH=$PATH:/sbin:/usr/sbin ; export PATH
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT%{prefix}{/bin,/lib,/share}
mkdir -p $RPM_BUILD_ROOT%{prefix}/lib/%{name}/plugins
mkdir -p $RPM_BUILD_ROOT%{prefix}/share/%{name}/{help,themes}

install -s {MakeTheme,%{name}} $RPM_BUILD_ROOT%{prefix}/bin
install themes/* $RPM_BUILD_ROOT%{prefix}/share/%{name}/themes
install -s plugins/* $RPM_BUILD_ROOT%{prefix}/lib/%{name}/plugins
(cd $RPM_BUILD_ROOT%{prefix}/share/%{name}/ ;
tar xfz $RPM_BUILD_DIR/%{name}/help/unix/freeamphelp.tar.gz)

%clean
rm -rf $RPM_BUILD_ROOT

%files 
%defattr(-,root,root)
%doc AUTHORS CHANGES COPYING INSTALL NEWS README README.linux
%{prefix}/bin/*
%{prefix}/share/%{name}
%{prefix}/lib/%{name}/plugins/cd.lmc
%{prefix}/lib/%{name}/plugins/cd.pmo
%{prefix}/lib/%{name}/plugins/cddb.mdf
%{prefix}/lib/%{name}/plugins/download.ui
%{prefix}/lib/%{name}/plugins/freeamp.ui
%{prefix}/lib/%{name}/plugins/httpinput.pmi
%{prefix}/lib/%{name}/plugins/id3v1.mdf
%{prefix}/lib/%{name}/plugins/id3v2.mdf
%{prefix}/lib/%{name}/plugins/kjofol.ftf
%{prefix}/lib/%{name}/plugins/localfileinput.pmi
%{prefix}/lib/%{name}/plugins/m3u.plf
%{prefix}/lib/%{name}/plugins/misc.mdf
%{prefix}/lib/%{name}/plugins/musicbrowser.ui
%{prefix}/lib/%{name}/plugins/obsinput.pmi
%{prefix}/lib/%{name}/plugins/pls.plf
%{prefix}/lib/%{name}/plugins/rmp.dlf
%{prefix}/lib/%{name}/plugins/signature.pmo
%{prefix}/lib/%{name}/plugins/soundcard.pmo
%{prefix}/lib/%{name}/plugins/vorbis.lmc
%{prefix}/lib/%{name}/plugins/vorbis.mdf
%{prefix}/lib/%{name}/plugins/wavout.pmo
%{prefix}/lib/%{name}/plugins/winamp.ftf
%{prefix}/lib/%{name}/plugins/xingmp3.lmc  

%files esd 
%defattr(-,root,root)
%{prefix}/lib/%{name}/plugins/esound.pmo

%files alsa 
%defattr(-,root,root)
%{prefix}/lib/%{name}/plugins/alsa.pmo

%files extras 
%defattr(-,root,root)
%{prefix}/lib/%{name}/plugins/irman.ui
%{prefix}/lib/%{name}/plugins/lcd.ui
%{prefix}/lib/%{name}/plugins/freeampcmd.ui
%{prefix}/lib/%{name}/plugins/mpg123.ui
%{prefix}/lib/%{name}/plugins/ncurses.ui

%files corba 
%defattr(-,root,root)
%{prefix}/lib/%{name}/plugins/corba.ui

%changelog
* Fri Sep 22 2000 Robert Kaye <rob@freeamp.org>
- Create several smaller rpms, rather than one big one..
* Thu Feb 17 2000 Robert Kaye <rob@freeamp.org>
- conversion from emusic to normal freeamp rpm
* Mon Jan 31 2000 Robert Kaye <rob@freeamp.org>
- conversion from freeamp to emusic
* Wed Jan 12 2000 Tim Powers <timp@redhat.com>
- initial build of freeamp for Powertools
- not origional spec file, violates Red Hat policy as to what can be done in
	the install section of the spec file (not bad, just violates policy)



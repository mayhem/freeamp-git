# -------------------------------------------------------------------------
# Global defines
# -------------------------------------------------------------------------
%define name    freeamp
%define version 2.1.beta6
%define release 1
%define prefix  /usr  

# -------------------------------------------------------------------------
# Main FreeAmp Package defines
# -------------------------------------------------------------------------
Summary:     File and streaming MP3/OggVorbis/Audio CD player/jukebox.
Name:        %{name}
Version:     %{version}
Release:     %{release}
Copyright:   GPL
Group:       Applications/Multimedia
Source:      ftp://www.freeamp.org/pub/freeamp/src/freeamp-%{version}.tar.gz
Prefix:      %{prefix}
BuildRoot:   /var/tmp/%{name}-%{version}-root
ExcludeArch: sparc
AutoReqProv: No 
Requires: id3lib >= 3.7.8 ld-linux.so.2 libc.so.6 libdl.so.2 libm.so.6 libnsl.so.1 libpthread.so.0 libstdc++-libc6.1-1.so.2 libX11.so.6 libXext.so.6 libgdk-1.2.so.0 libglib-1.2.so.0 libgmodule-1.2.so.0 libgthread-1.2.so.0 libgtk-1.2.so.0 libttf.so.2 libc.so.6(GLIBC_2.0) libm.so.6(GLIBC_2.1) libpthread.so.0(GLIBC_2.1) libpthread.so.0(GLIBC_2.0) libc.so.6(GLIBC_2.1) libdl.so.2(GLIBC_2.1) libdl.so.2(GLIBC_2.0)

%description
FreeAmp is an MP3/OggVorbis/AudioCD player/jukebox that can play local files,
SHOUTcast/icecast streams and RTP MPEG streams. It features a Themed interface,
which also supports WinAmp and KJofol skins, and it also supports automated
download from RMP enabled music sites like emusic.com.

# -------------------------------------------------------------------------
# ESD Package defines
# -------------------------------------------------------------------------
%package     esd
Summary:     Freeamp - FreeAmp plugin for ESD output
Group:       Applications/Multimedia
Requires:    %{name} = %{version}
Requires:    esound >= 0.2.12

%description    esd
FreeAmp plugin for ESD output

# -------------------------------------------------------------------------
# Alsa Package defines
# -------------------------------------------------------------------------
%package     alsa
Summary:     Freeamp - FreeAmp plugin for ALSA output
Group:       Applications/Multimedia
Requires:    %{name} = %{version}
Requires:    alsa >= 0.5.0

%description alsa
FreeAmp plugin for ALSA output

# -------------------------------------------------------------------------
# Extras Package defines
# -------------------------------------------------------------------------
%package     extras
Summary:     Freeamp - All the remaining plugins (mpg123 ui, cmdline ui, ncurses ui, irman, lcd)
Group:       Applications/Multimedia
Requires:    %{name} = %{version}
Requires:    alsa >= 0.5.0

%description extras
All the remaining plugins (mpg123 ui, cmdline ui, ncurses ui, irman, lcd)


# -------------------------------------------------------------------------
# Prep and build stuff 
# -------------------------------------------------------------------------
%prep
%setup -q -n freeamp

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
mkdir -p $RPM_BUILD_ROOT%{prefix}/lib/freeamp/plugins
mkdir -p $RPM_BUILD_ROOT%{prefix}/share/freeamp/{help,themes}

install -s {MakeTheme,freeamp} $RPM_BUILD_ROOT%{prefix}/bin
install themes/* $RPM_BUILD_ROOT%{prefix}/share/freeamp/themes
install -s plugins/* $RPM_BUILD_ROOT%{prefix}/lib/freeamp/plugins
(cd $RPM_BUILD_ROOT%{prefix}/share/freeamp/ ;
tar xfz $RPM_BUILD_DIR/%{name}/help/unix/freeamphelp.tar.gz)

%clean
rm -rf $RPM_BUILD_ROOT

%files 
%defattr(-,root,root)
%doc AUTHORS CHANGES COPYING INSTALL NEWS README README.linux
%{prefix}/bin/*
%{prefix}/share/freeamp
%{prefix}/lib/freeamp/plugins/cd.lmc
%{prefix}/lib/freeamp/plugins/cd.pmo
%{prefix}/lib/freeamp/plugins/cddb.mdf
%{prefix}/lib/freeamp/plugins/download.ui
%{prefix}/lib/freeamp/plugins/freeamp.ui
%{prefix}/lib/freeamp/plugins/httpinput.pmi
%{prefix}/lib/freeamp/plugins/id3v1.mdf
%{prefix}/lib/freeamp/plugins/id3v2.mdf
%{prefix}/lib/freeamp/plugins/kjofol.ftf
%{prefix}/lib/freeamp/plugins/localfileinput.pmi
%{prefix}/lib/freeamp/plugins/m3u.plf
%{prefix}/lib/freeamp/plugins/misc.mdf
%{prefix}/lib/freeamp/plugins/musicbrowser.ui
%{prefix}/lib/freeamp/plugins/obsinput.pmi
%{prefix}/lib/freeamp/plugins/pls.plf
%{prefix}/lib/freeamp/plugins/rmp.dlf
%{prefix}/lib/freeamp/plugins/signature.pmo
%{prefix}/lib/freeamp/plugins/soundcard.pmo
%{prefix}/lib/freeamp/plugins/vorbis.lmc
%{prefix}/lib/freeamp/plugins/vorbis.mdf
%{prefix}/lib/freeamp/plugins/wavout.pmo
%{prefix}/lib/freeamp/plugins/winamp.ftf
%{prefix}/lib/freeamp/plugins/xingmp3.lmc  

%files esd 
%defattr(-,root,root)
%{prefix}/lib/freeamp/plugins/esound.pmo

%files alsa 
%defattr(-,root,root)
%{prefix}/lib/freeamp/plugins/alsa.pmo

%files extras 
%defattr(-,root,root)
%{prefix}/lib/freeamp/plugins/irman.ui
%{prefix}/lib/freeamp/plugins/lcd.ui
%{prefix}/lib/freeamp/plugins/freeampcmd.ui
%{prefix}/lib/freeamp/plugins/mpg123.ui
%{prefix}/lib/freeamp/plugins/ncurses.ui

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



#!/bin/env perl
# $Id$
# A lame perl script to compile mwbres .r src and add necessary resources
# into a binary. Written by someone who can't write perl.

require 'getopt.pl';

sub getdefine
{
    my $symbol = shift;
    my $value;

    open CONFIG, "config/config.h";
    while ( <CONFIG> )
    {
        if ( /#define $symbol/ )
        {
            chomp;
            ($value = $_) =~ s/#define $symbol (.*)$/$1/;
            $value =~ s/\"//g;
        }
    }

    return $value;
}

sub getversion
{
    # retrieve the version info.
    my $versionstring = getdefine( "FREEAMP_VERSION" );
    $versionstring =~ s/.*\"(.+)\"$/$1/;

    my ($major, $middle, $minor) = split( '\.', $versionstring, 3 );
    if ( $middle eq "" ) { $middle = 0 }
    if ( $minor eq "" ) { $minor = 0 }

    return ($major, $middle, $minor);
}

sub echo_and_run
{
    $cmd = shift;
    print "$cmd\n";
    system( $cmd );
}

Getopt( 'otsr' );

$binary = $opt_o;
$appsig = $opt_s;
$filetype = $opt_t;

# strip the excess fat.

echo_and_run( "strip $binary" );

# add the resources.

foreach $file ( @ARGV )
{
    if ( $file =~ /.+\.r$/ )
    {
        # compile .r into .rsrc
        # mwbres is pretty dumb, e.g. doesn't understand include path option, etc.
        # so we need to actually cd into the .r dir.
        print "$file -> mwbres -> xres\n";
        $mwbressrc = $resdir = $file;
        $resdir =~ s/\/[^\/]+$//;
        $mwbressrc =~ s/$resdir\///;
        $rsrcfile = $mwbressrc;
        $rsrcfile =~ s/\..*$/\.rsrc/;
        echo_and_run( "cd $resdir; mwbres -o $rsrcfile $mwbressrc" );
        push @rsrcs, "$resdir/$rsrcfile";
    }
    if ( $file =~ /.+\.rsrc$/ )
    {
        print "$file -> xres\n";
        push @rsrcs, $file;
    }
}
echo_and_run( "xres -o $binary @rsrcs" );

# Set the version

$branding = getdefine( "the_BRANDING" );
($major, $middle, $minor) = getversion();
echo_and_run( "setversion $binary -app $major $middle $minor -short \"$branding\" -long \"$branding $major.$middle.$minor\"" );

# Set the application signature

if ( $appsig )
{
    echo_and_run( "settype -s \"$appsig\" $binary" );
}

# Update the file attribute.

echo_and_run( "mimeset -F $binary" );


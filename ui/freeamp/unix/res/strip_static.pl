#!/usr/bin/perl -w

foreach $k (@ARGV) {
    open(INFILE,"$k");
    open(OUTFILE,">$k.tmp");

    while (<INFILE>) {
	s/static //g;
	print OUTFILE;
    }
    
    close INFILE;
    close OUTFILE;

    unlink $k;
    rename "$k.tmp",$k;

}

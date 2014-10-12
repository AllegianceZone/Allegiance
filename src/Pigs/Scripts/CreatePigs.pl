#Imagotrigger@gmail.com

use strict;
use Win32::OLE;

my $behavior = $ARGV[0] || "imago";
my $seconds = $ARGV[1] || 3;
my $total = $ARGV[2] || 10;
my $cmd = "perl CreatePig.pl $behavior";
for (1 .. $total ) {
	system($cmd);
 	sleep $seconds;
}
exit 0;
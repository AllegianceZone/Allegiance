#Imagotrigger@gmail.com

use strict;
use Win32::OLE;

print "Creating Pigs session\n";
my $sess  = Win32::OLE->new("Pigs.Session");
if ($sess) {
	my $behavior = ($ARGV[0]) ? $ARGV[0] : "Default";
	print "Creating a Pig with '$behavior' behavior\n";
	my $pig = $sess->CreatePig($behavior);
	if ($pig) {
		my $me = $pig->Name;
		if (!$me) {
			exit 1;
		} else {
			print "Piglet named $me was born!\n";
			exit 0;
		}
	} else {
		my $err = Win32::OLE::LastError();
		print "Piglet was aborted: $err!\n";
		exit 1;
	}
} else {
	my $err = Win32::OLE::LastError();
	print "Pigs.Session error: $err!\n";
	exit 1;
}


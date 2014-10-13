#Imagotrigger@gmail.com

use strict;
use Win32::OLE;

my $pigname = $ARGV[0] || 'botpilot1';

print "Creating Pigs session\n";
my $sess  = Win32::OLE->new("Pigs.Session");
if ($sess) {
	my $pig = $sess->Pigs($pigname);
	if ($pig) {
		print "$pigname is currently ".$pig->PigStateName."\n";
		#############################
		# Put your command(s) here! #
		#############################
		
		#$pig->JoinTeam("Iron Coalition");
		$pig->Ship->Goto("Garrison");
		
		#############################		
		my $err = Win32::OLE::LastError();
		if ($err == 0) {
			print "Ok!\n";
			exit 0;
		} else {
			print "Pig $pigname: $err!\n";
			exit 1;
		}
	} else {
		my $err = Win32::OLE::LastError();
		($err == 0) ? print "Pig $pigname does not exist!\n" : print "Pig $pigname: $err!\n";
		exit 1;
	}
} else {
	my $err = Win32::OLE::LastError();
	print "Pigs.Session error: $err!\n";
	exit 1;
}

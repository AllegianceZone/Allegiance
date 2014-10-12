#Imagotrigger@gmail.com

use strict;
use Win32::OLE;

my $pigname = $ARGV[0] || 'botpilot1';

print "Creating Pigs session\n";
my $sess  = Win32::OLE->new("Pigs.Session");
if ($sess) {
	my $pig = $session->Pigs();
	if ($pig) {
		print "$pigname is currently ".$pig->PigStateName."\n";
		
		###########################
		#Put your command(s) here!#
		###########################
		
		#$pig->JoinTeam("Iron Coalition");
		#$pig->Ship->Attack("BBT");
		
	} else {
		my $err = Win32::OLE::LastError();
		print "Pig $pigname: $err!\n";
		exit 1;
	}
} else {
	my $err = Win32::OLE::LastError();
	print "Pigs.Session error: $err!\n";
	exit 1;
}

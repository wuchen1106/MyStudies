#!/usr/bin/perl -w

########################################################################
# Script to merge g4beamline format B fieldmap files, and manipulate   #
# them before outputting as a single merged file.                      #
########################################################################

#Example input file format (apart from the leading '#'):
##This is the BLFieldMap input file for SEG1 in cartesian co-ords
#grid X0=-600 Y0=0 Z0=-1000 nX=121 nY=61 nZ=401 dX=10 dY=10 dZ=10
#extendY flip=Bx,By
#data
#-600 0 3000 -1.25983 4.38823e-17 -1.74329
#-600 10 3000 -1.25997 0.0211124 -1.74342
#-600 20 3000 -1.2606 0.0422431 -1.74301
#-600 30 3000 -1.26183 0.0633884 -1.74188
#-600 40 3000 -1.26353 0.0846047 -1.74022
#
use strict;
use warnings;
use POSIX;

use Getopt::Long qw(:config );

open DATA, ">datapoints.dat";

my $help = 0;
my $dipolefield1 = 0.030; # Tesla
my $dipolefield2 = 0.050; # Tesla
my $dipolefield3 = 0.170; # Tesla
my $dipolefield12minz = 4350; # mm
my $dipolefield1maxx = 3000; # mm
my $dipolefield2minx = 3800; # mm
my $dipolefield3maxz = -701; # mm
my $dipolefield3minx = 3800; # mm (not critical)
my $dX = 10; # mm
my $dY = 10; # mm
my $dZ = 10; # mm
my $noflipXZ = 0;

my $result = GetOptions (
		"help" => \$help,
		"dipolefield1=f" => \$dipolefield1,
		"dipolefield2=f" => \$dipolefield2,
		"dipolefield3=f" => \$dipolefield3,
		"dX=i" => \$dX,
		"dY=i" => \$dY,
		"dZ=i" => \$dZ,
		"noflipXZ" => \$noflipXZ
		);

if($help){
	print <<"EOF";

Usage:    CreateDefaultFieldMap.pl filenames

			  where "filenames" are the possibly gzipped files which contain
			  the input g4beamline format files containing the field
			  information.

			  The full fieldmap is printed to "datapoints.dat", and the
			  header to "header.dat".
			  cat header.dat datapoints.dat > defaultFieldMap.dat
			  will create the full field map file (not performed automatically
					  because of file size issues).

			  ASSUMES THAT A MIRROR  FIELD FOR y < 0, WITH THE y-COMPONENT
			  FLIPPED, IS APPROPRIATE.

			  Dipole fields are superimposed on the full field, after
			  the y-flipping.

			  Options:  --dipolefield1 field # vertical field in Teslas to superimpose
# on the first 90 degrees of the muon
# transport solenoid
# (currently $dipolefield1 Tesla)

			  --dipolefield2 field # vertical field in Teslas to superimpose
# on the second 90 degrees of the muon
# transport solenoid
# (currently $dipolefield2 Tesla)

			  --dipolefield3 field # vertical field in Teslas to superimpose
# on the electron spectrometer
# (currently $dipolefield3 Tesla)

			  --dX gridsep         # separation in mm of grid points along X
# (currently $dX mm)

			  --dY gridsep         # separation in mm of grid points along Y
# (currently $dY mm)

			  --dZ gridsep         # separation in mm of grid points along Z
# (currently $dZ mm)

			  --noflipXZ           # does not multiply the X and Z components of the field
# by -1, to make the solenoidal field go the same
# way as in the CDR
			  EOF
}

my $minx = 999999;
my $maxx = -999999;
my $miny = 999999;
my $maxy = -999999;
my $minz = 999999;
my $maxz = -999999;

# To open gzipped files
# (from http://alumnus.caltech.edu/~svhwan/prodScript/avoidSystemBackticks.html)
my $magicNumber_GZIP = pack("C*", 0x1f, 0x8b, 0x08, 0x08);

print STDERR "Reading in files. Note that grid, extendY and data lines are not used\n";

foreach my $inputfile (@ARGV){
	if(!-r $inputfile){
		die "Cannot read file $inputfile\n";
	}
# Some code to open files whether they are gzipped or not from HERE
#
# Just peek at the first 4 characters in the file and
# close it again.
	open(INPUTFILE, $inputfile) or die "Couldn't open file $inputfile: $!";
	binmode(INPUTFILE);
	my $magicNumber;
	read(INPUTFILE, $magicNumber, 4);
	close(INPUTFILE);

	if ($magicNumber eq $magicNumber_GZIP) {    # file starts w/gzip magic number
		my $pid;
		if (not defined($pid = open(INPUTFILE, "-|"))) {
			die "can't fork: $!";
		}
		if ($pid) {
# parent process - do nothing
		} else {
# child process
			exec( "gzip", "-f", "-c", "-d", $inputfile);
		}
	} else {
		open(INPUTFILE, $inputfile)
			or die "couldn't open $inputfile: $!";
	}

# ...to HERE (code to open files, gzipped or not)

	my $x;
	my $y;
	my $z;
	my $Bx;
	my $By;
	my $Bz;



	while (<INPUTFILE>) {
		if(m#^([-\d]+)\s+([-\d]+)\s+([-\d]+)\s+([-\de.]+)\s+([-\de.]+)\s+([-\de.]+)\s*$#){
			$x = $1;
			$y = $2;
			$z = $3;
			$Bx = $4;
			$By = $5;
			$Bz = $6;

			if($x < $minx){ $minx = $x; }
			if($x > $maxx){ $maxx = $x; }
			if($y < $miny){ $miny = $y; }
			if($y > $maxy){ $maxy = $y; }
			if($z < $minz){ $minz = $z; }
			if($z > $maxz){ $maxz = $z; }

			if($z > $dipolefield12minz && $x < $dipolefield1maxx){ $By += $dipolefield1; }
			if($z > $dipolefield12minz && $x > $dipolefield2minx){ $By += $dipolefield2; }
			if($z < $dipolefield3maxz && $x > $dipolefield3minx){ $By += $dipolefield3; }
			if(!$noflipXZ){
				$Bx = -$Bx;
				$Bz = -$Bz;
			}
			print DATA "$x $y $z $Bx $By $Bz\n";

			if($y > 0){
				$By = -1.0*$5;
				if($z > $dipolefield12minz && $x < $dipolefield1maxx){ $By += $dipolefield1; }
				if($z > $dipolefield12minz && $x > $dipolefield2minx){ $By += $dipolefield2; }
				if($z < $dipolefield3maxz && $x > $dipolefield3minx){ $By += $dipolefield3; }
				print DATA "$x -$y $z $Bx $By $Bz\n";
			}

		} else {
			print STDERR "Not using line: $_";
		}

	}
	close(INPUTFILE);
}

close (DATA);

my $nx = POSIX::floor(($maxx - $minx + 1.5*$dX)/$dX);
my $ny = POSIX::floor(($maxy - $miny + 1.5*$dY)/$dY);
my $nz = POSIX::floor(($maxz - $minz + 1.5*$dZ)/$dZ);

$miny = -$maxy;
$ny = 2* $ny - 1;

print STDERR "minx: $minx, maxx: $maxx, nx: $nx\n";
print STDERR "miny: $miny, maxy: $maxy, ny: $ny\n";
print STDERR "minz: $minz, maxz: $maxz, nz: $nz\n";

my $date = localtime();

open HEADER, ">header.dat";

# Print header
print HEADER<<"EOF";
# This is the BLFieldMap for COMET, calculated with these values:
# Dipolefield1: $dipolefield1 Tesla
# Dipolefield2: $dipolefield2 Tesla
# Dipolefield3: $dipolefield3 Tesla
# Dipolefield12minz: $dipolefield12minz mm
# Dipolefield1maxx:  $dipolefield1maxx mm
# Dipolefield2minx:  $dipolefield2minx mm
# Dipolefield3maxz:  $dipolefield3maxz mm
# Dipolefield3minx:  $dipolefield3minx mm: (not a critical value)
# using the script CreateDefaultFieldMap.pl on $date
grid X0=$minx Y0=$miny Z0=$minz nX=$nx nY=$ny nZ=$nz dX=$dX dY=$dY dZ=$dZ
data
EOF

close(HEADER);

print STDERR "after checking the output files, run\n";
print STDERR "  cat header.dat datapoints.dat > defaultFieldMap.dat\n";
print STDERR "to produce the standard field  map for comet_g4\n";

import commands
import os
import sys

f = open('listana', 'r')
for line in f:
	options = line.split()
	if (options[0]=='G1' or options[0]=='#'):
		continue
	else:
		runname = ""
		name = ""
		for word in options:
			runname += "."+word
			name += word+" "
#		runname += ".Ethane"
		output = commands.getoutput('./RawAna/getNoise noise'+runname+' dir 3934331 CyDet.beam.all'+runname+' CyDet.dio.o50'+runname+' CyDet.pmc'+runname+' 4 1 1 4.63491694480 28.34 2815 1 2 3')
		print name+output
		sys.stdout.flush()

import commands
import os
import sys

f = open('listsig', 'r')
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
		output = commands.getoutput('./RawAna/getSignal signal'+runname+' dir CyDet.signal'+runname+' 1 100000')
		print name+output
		sys.stdout.flush()

import commands
import os
import sys

f = open('list', 'r')
os.chdir("comet")
for line in f:
	options = line.split()
	if (options[0]=='G1' or options[0]=='#'):
		continue
	else:
		commands.getoutput('cp configure/sub_Trigger/subgeo_Trigger_OPTtemp configure/sub_Trigger/subgeo_Trigger_OPT')
		commands.getoutput('sed -i "s/opG1/  '+options[0]+'/" configure/sub_Trigger/subgeo_Trigger_OPT')
		commands.getoutput('sed -i "s/opG2/  '+options[1]+'/" configure/sub_Trigger/subgeo_Trigger_OPT')
		commands.getoutput('sed -i "s/opWSCI/  '+options[2]+'/" configure/sub_Trigger/subgeo_Trigger_OPT')
		commands.getoutput('sed -i "s/opWCHE/  '+options[3]+'/" configure/sub_Trigger/subgeo_Trigger_OPT')
		commands.getoutput('sed -i "s/opASCI/  '+options[4]+'/" configure/sub_Trigger/subgeo_Trigger_OPT')
		commands.getoutput('sed -i "s/opACHE/  '+options[5]+'/" configure/sub_Trigger/subgeo_Trigger_OPT')
		commands.getoutput('sed -i "s/opTH6/  '+options[6]+'/" configure/sub_Trigger/subgeo_Trigger_OPT')
		commands.getoutput('sed -i "s/opTHS/  '+options[7]+'/" configure/sub_Trigger/subgeo_Trigger_OPT')
		commands.getoutput('sed -i "s/opTHT/  '+options[8]+'/" configure/sub_Trigger/subgeo_Trigger_OPT')
		commands.getoutput('sed -i "s/opTH7/  '+options[9]+'/" configure/sub_Trigger/subgeo_Trigger_OPT')
		commands.getoutput('sed -i "s/opTH8/  '+options[10]+'/" configure/sub_Trigger/subgeo_Trigger_OPT')
		commands.getoutput('sed -i "s/opTH0/  '+options[11]+'/" configure/sub_Trigger/subgeo_Trigger_OPT')
		commands.getoutput('sed -i "s/opTH1/  '+options[12]+'/" configure/sub_Trigger/subgeo_Trigger_OPT')
		commands.getoutput('sed -i "s/opDD/  '+options[13]+'/" configure/sub_Trigger/subgeo_Trigger_OPT')
		commands.getoutput('sed -i "s/opMTS/  '+options[14]+'/" configure/sub_Trigger/subgeo_Trigger_OPT')
		commands.getoutput('sed -i "s/opMSS/  '+options[15]+'/" configure/sub_Trigger/subgeo_Trigger_OPT')
		commands.getoutput('sed -i "s/opMS/  '+options[16]+'/" configure/sub_Trigger/subgeo_Trigger_OPT')
		runname = ""
		for word in options:
			runname += "."+word
		runname+=".Ethane"
		output = commands.getoutput('SRA_gen_sim -f -t sim -b 0 -e 0 -q besq opt.sig CyDet.signal'+runname)
		print output                                  
		output = commands.getoutput('SRA_gen_sim -f -t sim -a fl.CyDet.ALL -b 0 -e 3 -q besq opt.beam.all CyDet.beam.all'+runname)
		print output                                  
#		output = commands.getoutput('SRA_gen_sim -f -t sim -b 0 -e 0 -q besq opt.beam.o10 CyDet.beam.o10'+runname)
#		print output                                  
		output = commands.getoutput('SRA_gen_sim -f -t sim -b 0 -e 0 -q besq opt.dio.o50 CyDet.dio.o50'+runname)
		print output                                  
		output = commands.getoutput('SRA_gen_sim -f -t sim -b 0 -e 0 -q besq opt.pmc CyDet.pmc'+runname)
		print output
		sys.stdout.flush()



import os
import sys
import re

from TargetList import *

__all__ = [
'BaseParser',
'PythonCMTParser',
]

##################################################################
# class BaseParser
##################################################################
class BaseParser():
	def __init__(self,verbose=0):
		self.m_status=0
		self.m_TargetList=TargetList()
		self.m_verbose=verbose

	@property
	def status(self):
		return self.m_status

	def GetTargetList(self,target,target_type,target_dir):
		aTarget = Target("Example","")
		self.m_TargetList.append("anExample","")
		return self.m_TargetList

##################################################################
# class PythonCMTParser
##################################################################
class PythonCMTParser(BaseParser):
	def __init__(self,verbose=0):
		BaseParser.__init__(self,verbose)

	def GetTargetList(self,target,target_type,target_dir):
		requirements = os.path.join(target_dir, "cmt", "requirements")
		if not os.path.isfile(requirements):
			self.m_status=1 # cannot open the requirements file
			return ""
		pattern = re.compile(r'use[ \t]+(.*)[ \t]+(.*)')
		statement = ""
		if self.m_verbose >= 10:
			print "## in PythonCMTParser GetTargetList"
			print "##   reading %s" % (requirements)
		try:
			for l in open(requirements):
				if '#' in l:
					l = l[:l.find('#')]
				l = l.strip()
				# if we have something in the line, extend the statement
				if l:
					statement = l
					if statement.endswith('\\'):
						# if the statement requires another line, get the next
						statement = statement[:-1] + ' '
						continue
				# either we got something more in the statement or not, but
				# an empty line after a '\' means ending the statement
				if statement:
					match = pattern.match(statement)
					if match:
						name = match.group(1)
						name = name.strip()
						version = match.group(2)
						version = version.strip()
						if self.m_verbose >= 10:
							print "##   (%s,%s) from \"%s\"" % (name,version,statement)
						self.m_TargetList.append(name,version)
		except:
			sys.exit("Cannot open requirements file "+requirements+"\nExiting...")
		return self.m_TargetList

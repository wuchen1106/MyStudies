
import os
import sys
import re

from TargetList import *

__all__ = [
'BaseParser',
'PythonCMTParser',
'PythonCMakeParser',
'CMTParser',
'CMakeParser',
]

##################################################################
# class BaseParser
##################################################################
class BaseParser(object):
	def __init__(self,verbose=0):
		self.m_status=0
		self.m_TargetList=TargetList()
		self.m_verbose=verbose
		self.m_values = {}

	@property
	def status(self):
		return self.m_status

	def GetTargetList(self,requirements):
		aTarget = Target("Example","")
		self.m_TargetList.append("anExample","")
		return self.m_TargetList

##################################################################
# class PythonCMTParser
##################################################################
class PythonCMTParser(BaseParser):
	def __init__(self,verbose=0):
		BaseParser.__init__(self,verbose)

	def GetTargetList(self,requirements):
		self.m_status=0 # this statusflag is only supposed to indicate the status during one parse so we have to reset it at each beginning
		self.m_values = {} # I currently suppose that a macro value defined somewhere else cannot be read by this requirements file so I just clean the dictionary at each beginning
		self.m_TargetList=TargetList()
		if not os.path.isfile(requirements):
			self.m_status=1 # cannot find the requirements file
			return TargetList()
		pattern_general = re.compile(r'(\w+)[ \t]+(.*)')
		pattern_macro = re.compile(r'(\w+)[ \t]+\"(.*)\".*')
		# FIXME: cannot deal with recursive macros yet
		pattern_macro_append = re.compile(r'(\w+)[ \t]+\"(.*)\".*')
		pattern_macro_append_more = re.compile(r'(\w+)[ \t]+\"(.*)\"(\w+[ \t]+\".*\")')
		pattern_use_macro = re.compile(r'\$\((\w+)\)(.*)')
		pattern_use = re.compile(r'(\w+)[ \t]*([a-zA-Z0-9_*.]*)')
		statement = ""
		if self.m_verbose >= 15:
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
					match = pattern_general.match(statement)
					if match:
						prefex = match.group(1)
						content = match.group(2)
						content = content.strip()
						if prefex == "macro":
							match_macro = pattern_macro.match(content)
							if match_macro:
								name = match_macro.group(1)
								value = match_macro.group(2)
								self.m_values[name] = value
								if self.m_verbose >= 15:
									print "##   (%s)=\"%s\" from \"%s\"" % (name,value,statement)
						elif prefex == "macro_append":
							match_macro_append_more = pattern_macro_append_more.match(content)
							while match_macro_append_more:
								name = match_macro_append_more.group(1)
								pre_value = self.m_values.get(name,'NONE')
								if pre_value == 'NONE':
									if self.m_verbose >= 15:
										print "WARNING: variable (%s) in \"%s\" was not declared before! in \"%s\"" % (name,statement,requirements)
								else:
									value = match_macro_append_more.group(2)
									content = match_macro_append_more.group(3)
									self.m_values[name] += value
									match_macro_append_more = pattern_macro_append_more.match(content)
									if self.m_verbose >= 15:
										print "##   (%s)+=\"%s\"=\"%s\" from \"%s\"" % (name,value,self.m_values[name],statement)
							match_macro_append = pattern_macro_append.match(content)
							if match_macro_append:
								name = match_macro_append.group(1)
								pre_value = self.m_values.get(name,'NONE')
								if pre_value == 'NONE':
									if self.m_verbose >= 15:
										print "WARNING: variable (%s) in \"%s\" was not declared before! in \"%s\"" % (name,statement,requirements)
								else:
									value = match_macro_append.group(2)
									self.m_values[name] += value
									if self.m_verbose >= 15:
										print "##   (%s)+=\"%s\"=\"%s\" from \"%s\"" % (name,value,self.m_values[name],statement)
						elif prefex == "use":
							match_use_macro = pattern_use_macro.match(content)
							if match_use_macro:
								name = match_use_macro.group(1)
								value = self.m_values.get(name,'NONE')
								if value == 'NONE':
									if self.m_verbose >= 5:
										print "WARNING: variable (%s) in \"%s\" was not declared before! in \"%s\"" % (name,statement,requirements)
								else:
									content = value+match_use_macro.group(2)
									content = content.strip()
									match_use = pattern_use.match(content)
									if match_use:
										target = match_use.group(1)
										target = target.strip()
										version = match_use.group(2)
										version = version.strip()
										if self.m_verbose >= 15:
											print "##   %-25s @%-15s from \"%s\"" % ('"'+target+'"','['+version+']',statement)
										self.m_TargetList.append(target,version)
							else:
								match_use = pattern_use.match(content)
								if match_use:
									target = match_use.group(1)
									target = target.strip()
									version = match_use.group(2)
									version = version.strip()
									if self.m_verbose >= 15:
										print "##   %-25s @%-15s from \"%s\"" % ('"'+target+'"','['+version+']',statement)
									self.m_TargetList.append(target,version)
		except:
			self.m_status=2 # cannot open the requirements file
			return TargetList()
		return self.m_TargetList

##################################################################
# class PythonCMakeParser
##################################################################
class PythonCMakeParser(BaseParser):
	def __init__(self,verbose=0):
		BaseParser.__init__(self,verbose)

	def GetTargetList(self,requirements):
		return self.m_TargetList

##################################################################
# class CMakeParser
##################################################################
class CMakeParser(BaseParser):
	def __init__(self,verbose=0):
		BaseParser.__init__(self,verbose)

	def GetTargetList(self,requirements):
		return self.m_TargetList

##################################################################
# class CMTParser
##################################################################
class CMTParser(BaseParser):
	def __init__(self,verbose=0):
		BaseParser.__init__(self,verbose)

	def GetTargetList(self,requirements):
		return self.m_TargetList

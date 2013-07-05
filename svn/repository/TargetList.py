
from Target import *

__all__ = [
'TargetList',
'ProjectList',
'PackageList',
]

##################################################################
# class TargetList 
##################################################################
class TargetList(object):
	def __init__(self):
		self.m_TargetList = []
		self.m_type = "TargetList"
		self.m_names = []

	@property
	def names(self):
		return self.m_names

	@property
	def targets(self):
		return self.m_TargetList

	def get(self,name):
		for aTarget in self.m_TargetList:
			if aTarget.name == name:
				return aTarget
		return 0
	def append(self,name,version=""):
		aTarget = self.get(name)
		if not aTarget:
			new_Target=Target(name,version)
			self.m_TargetList.append(new_Target)
			self.m_names.append(name)

	def check(self,name,version):
		aTarget=self.get(name)
		if not aTarget:
			return -1 # cannot get this Target
		elif aTarget.checked():
			return 1 # already checked
		else:
			aTarget.check(version)
		return 0

	def Dump(self):
		for aTargetName in self.m_names:
			aTarget = self.get(aTargetName)
			text_output = "  " + aTarget.name
			if aTarget.checked():
				text_output += " checkedout already! version = "
				text_output += aTarget.version
			print text_output
#			print "  %-25s checked?[%s] for \"%s\"" % (aTarget.name,aTarget.checked(),aTarget.version)
##################################################################
# class PackageList 
##################################################################
class PackageList(TargetList):
	def __init__(self):
		TargetList.__init__(self)
		self.m_type = "PackageList"

	def append(self,name,version=""):
		aPackage = self.get(name)
		if not aPackage:
			new_Package=Package(name,version)
			self.m_TargetList.append(new_Package)
			self.m_names.append(name)
##################################################################
# class ProjectList 
##################################################################
class ProjectList(TargetList):
	def __init__(self):
		TargetList.__init__(self)
		self.m_type = "ProjectList"

	def append(self,name,version=""):
		aProject = self.get(name)
		if not aProject:
			new_Project=Package(name,version)
			self.m_TargetList.append(new_Project)
			self.m_names.append(name)

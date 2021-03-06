
__all__ = [
'Target',
'Project',
'Package',
]

##################################################################
# class Target
##################################################################
class Target(object):
	def __init__(self,name,version=""):
		self.m_name = name
		self.m_checked = False
		self.m_version = version
		self.m_checkedVersion = "None"

	def checked(self):
		if self.m_checked:
			return 1 # checked already
		return 0

	def check(self,version):
		if self.checked():
			return 1 # already checked
		else:
			self.m_checked = True
			self.m_checkedVersion = version
		return 0

	@property
	def version(self):
		return self.m_version
	@property
	def checkedVersion(self):
		return self.m_checkedVersion
	@property
	def name(self):
		return self.m_name
##################################################################
# class Package
##################################################################
class Package(Target):
	def __init__(self,name,version):
		Target.__init__(self,name,version)
##################################################################
# class Project
##################################################################
class Project(Target):
	def __init__(self,name,version):
		Target.__init__(self,name,version)

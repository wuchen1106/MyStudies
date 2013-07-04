
"""
Storing target lists consisting of target objects
	where target objects can store information like whether it has been checked out or not
"""

import subprocess

from TargetList import *

__all__ = [
'RepoStructure',
'RepoStructureSVN',
]

##################################################################
# class RepoStructure
##################################################################
class RepoStructure(object):
	def __init__(self,urls):
		self.m_packages = []
		self.m_projects = []
		self.m_unrecogonized = TargetList()
		self.ini_target_list(urls)
		self.m_status = 0

	def ini_target_list(self,urls):
		return []

	@property
	def status(self):
		return self.m_status

	@property
	def packages(self):
		return self.m_packages

	@property
	def projects(self):
		return self.m_projects

	@property
	def unrecogonized(self):
		return self.m_unrecogonized

	def Dump(self):
		print "packages:"
		self.m_packages.Dump()
		print "projects:"
		self.m_projects.Dump()
##################################################################
# class RepoStructureSVN
##################################################################
class RepoStructureSVN(RepoStructure):
	def __init__(self,urls):
		RepoStructure.__init__(self,urls)

	def ini_target_list(self,urls):
		self.m_packages = PackageList()
		self.m_projects = ProjectList()
		p = subprocess.Popen(["svn","list",urls.package_url],stdout = subprocess.PIPE,stderr = subprocess.PIPE)
		p.wait()
		l = p.stdout.readlines()
		if p.returncode:
			self.m_status = 1
			print "ERROR: Cannot recogonize URL: %s" % (urls.package_url)
		else:
			for i in l:
				self.m_packages.append(i[:-2])
		p = subprocess.Popen(["svn","list",urls.project_url],stdout = subprocess.PIPE,stderr = subprocess.PIPE)
		p.wait()
		l = p.stdout.readlines()
		if p.returncode:
			self.m_status = 1
			print "ERROR: Cannot recogonize URL: %s" % (urls.project_url)
		else:
			for i in l:
				self.m_projects.append(i[:-2])

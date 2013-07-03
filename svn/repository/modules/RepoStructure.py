
import subprocess

from TargetList import *

__all__ = [
'RepoStructure',
'RepoStructureSVN',
]

##################################################################
# class RepoStructure
##################################################################
class RepoStructure():
	def __init__(self,urls):
		self.m_packages = self.ini_package_list(urls)
		self.m_projects = self.ini_project_list(urls)
		self.m_status = 0

	def ini_package_list(self,urls):
		return []

	def ini_project_list(self,urls):
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
##################################################################
# class RepoStructureSVN
##################################################################
class RepoStructureSVN(RepoStructure):
	def __init__(self,urls):
		RepoStructure.__init__(self,urls)

	def ini_package_list(self,urls):
		p = subprocess.Popen(["svn","list",urls.package_url],stdout = subprocess.PIPE,stderr = subprocess.PIPE)
		p.wait()
		l = p.stdout.readlines()
		alist = PackageList()
		if p.returncode:
			self.m_status = 1
			print "ERROR: Cannot recogonize URL: %s" % (urls.package_url)
		else:
			for i in l:
				alist.append(i[:-2])
		return alist

	def ini_project_list(self,urls):
		p = subprocess.Popen(["svn","list",urls.project_url],stdout = subprocess.PIPE,stderr = subprocess.PIPE)
		p.wait()
		l = p.stdout.readlines()
		alist = ProjectList()
		if p.returncode:
			self.m_status = 1
			print "ERROR: Cannot recogonize URL: %s" % (urls.project_url)
		else:
			for i in l:
				alist.append(i[:-2])
		return alist

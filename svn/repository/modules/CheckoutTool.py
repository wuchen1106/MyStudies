
import re
import os
import subprocess

from URLs import *
from Parser import *
from RepoStructure import *

__all__ = [
'CheckoutTool',
'CheckoutToolSVN',
]

##################################################################
# class CheckoutTool
##################################################################

class CheckoutTool():
	def __init__(self,depParser,verbose=0):
		self.m_status=0
		self.m_Recursive=False
		self.m_BaseDirectory = ""
		self.m_RepoStructure=""
		self.m_verbose=verbose
		result=self.GetRepoStructure()
		if result:
			self.m_status=1 # cannot recogonize URL
		if depParser == "PythonCMT":
			self.m_DependencyParser = PythonCMTParser(verbose)
#		elif depParser == "CMT":
#			self.m_DependencyParser = CMTParser()
#		elif depParser == "CMake":
#			self.m_DependencyParser = CMakeParser()
#		elif depParser == "PythonCMake":
#			self.m_DependencyParser = PythonCMakeParser()
		else:
			print "ERROR!!! Cannot recogonize dependency parser type \"%s\"" % (depParser)
			self.m_status=2 # cannot recogonize depParser parser type

	@property
	def status(self):
		return self.m_status
	@property
	def repoStructure(self):
		return self.m_RepoStructure

	# check the type of target
	def checkout(self,target,version,recursive=False):
		self.m_Recursive = recursive

		# Analyse target type
		target_type = self.GetTargetType(target)
		if target_type == "":
			print "ERROR!!! \"%s\" is neither a project nor a package!" % (target)
			self.m_status = 1
			return 1 # Cannot find target
		elif target_type == "CHECKED":
			return 0 # checked already

		# check version type
		version_type = self.GetVersionType(version)

		# if this is the first time, get direcoties
		if self.m_BaseDirectory == "":
			pwd=os.getcwd()
			if target in pwd:
				self.m_BaseDirectory=pwd[:pwd.find(target)-1]
			else:
				self.m_BaseDirectory=pwd
			if target_type == "project":
				self.m_ProjectDirectory=self.m_BaseDirectory+"/"+target
			else:
				self.m_ProjectDirectory=self.m_BaseDirectory

		# get the directory
		target_dir = self.GetDirectory(target,target_type)

		# check it out
		if self.m_verbose >= 1:
			print "==> checking out %s[%s] @%s[%s] recursively[%s] to %s" % (target_type,target,version_type,version,recursive,target_dir)
		status=self.CheckoutTarget(target,version,target_type,version_type,target_dir)
		if status:
			self.m_status = -1
			print "ERROR!!! in CheckoutTarget(%s,%s,%s,%s,%s)" % (target,version_type,target_type,version_type,target_dir)
			return -1 # error occurred when CheckoutTarget
		if target_type == "project":
			self.m_RepoStructure.projects.check(target,version)
		if target_type == "package":
			self.m_RepoStructure.packages.check(target,version)

		# recursive?
		if self.m_Recursive:
			target_list = self.m_DependencyParser.GetTargetList(target,target_type,target_dir)
			if self.m_verbose >= 10:
				print "%s has " % (target)
				target_list.Dump()
			if self.m_DependencyParser.status:
				self.m_status = -3
				print "ERROR!!! cannnot find dependency of \"%s\" @ \"%s\"!" % (target,target_dir)
				return -3 # cannot get the paser
			for aTarget in target_list.targets:
				result = self.checkout(aTarget.name,aTarget.version,True)
				if result:
					return -1 # error occurred when checkout
		return 0

	def GetTargetType(self,target):
		project = self.m_RepoStructure.projects.get(target)
		package = self.m_RepoStructure.packages.get(target)
		if project:
			if project.checked():
				return "CHECKED"
			else:
				return "project"
		elif package:
			if package.checked():
				return "CHECKED"
			else:
				return "package"
		else:
			return "" # cannnot recogonize this target!

	def GetVersionType(self,version):
		if version == "trunk":
			return "trunk"
		elif version == "*":
			#FIXME infact this should refer to the newes tag. Do we need to support regular expression?
			return "trunk"
		else:
			pattern = re.compile(r'.*_t\d+')
			match = pattern.match(version)
			if match:
				return "branch"
			else:
				return "tag"

	def GetDirectory(self,target,target_type):
		if target_type == "project":
			return self.m_BaseDirectory + "/" + target
		elif target_type == "package":
			return self.m_ProjectDirectory + "/packages/" + target

	def GetRepoStructure(self):
		# prepare urls, repoStructure, and Base Directory
		self.m_urls=URLs()
		print "Checking projects and packages repoStructure from \"%s\"" % (self.m_urls.base_url)
		self.m_RepoStructure=RepoStructure(self.m_urls)
		if self.m_RepoStructure.status:
			return 1 # cannot recogonize structure from the given url
		return 0

	def CheckoutTarget(self,target,version,target_type,version_type,target_dir):
		print "  ##Check out using CheckoutTool (?could not be...)"

##################################################################
# class CheckoutToolSVN
##################################################################
class CheckoutToolSVN(CheckoutTool):
	def __init__(self,depParser,verbose=0):
		CheckoutTool.__init__(self,depParser,verbose)

	def GetRepoStructure(self):
		# prepare urls, repoStructure, and Base Directory
		self.m_urls=URLs()
		print "Checking projects and packages repoStructure from \"%s\"" % (self.m_urls.base_url)
		self.m_RepoStructure=RepoStructureSVN(self.m_urls)
		if self.m_RepoStructure.status:
			return 1 # cannot recogonize structure from the given url
		return 0

	def CheckoutTarget(self,target,version,target_type,version_type,target_dir):
		url = ""
		if target_type == "project":
			url = self.m_urls.project_url + "/" + target
		elif target_type == "package":
			url = self.m_urls.package_url + "/" + target
		if version_type == "trunk":
			url = url + "/trunk"
		elif version_type == "tag":
			url = url + "/" + version
		elif version_type == "branch":
			url = url + "/branches/" + version
		p = subprocess.Popen(["svn","checkout",url,target_dir],stdout=subprocess.PIPE,stderr = subprocess.PIPE)
		p.wait()
		if p.returncode:
			return 1
		return 0


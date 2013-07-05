
"""
A series of classes which deal with the checkout work for ICEDUST
with different version control tools and dependency managing tools respectively:

USAGE:
	Initialize:
		myCheckoutTool = CheckoutToolX(dependency_manage,verbose)
	checkout:
		myCheckoutTool.checkout(destination,target,version,recursive)

About destination and target, conventions should follow:
	target:
		a. set explicitly:
			use the given target name.
			Check it with target list got from repository.
			if not found, stop checking and output error message.
		b. not set:
			analyse current directory.
			a. Found a directory name in current directory which matches a target name from given target list got from repository
				suppose current directory is /.../A_Target_Name/.../B_Target_Name/...
				use 'B_Target_Name' (i.e. the closest one) as the target.
			b. Not found any target name from given target list got from repository which match any directory name in current directory.
				Cannot go on anymore. Stop and show error message
	destination:
		1. first get the referrence directory
			a. set explicitly:
				ref_dir = os.path.realpath(destination)
			b. not set:
				ref_dir = current directory
		2. then get the m_BaseDirectory and m_package_reldir
			a. if target is project:
				a. if target name is found in ref_dir
					suppose ref_dir = /.../target_name/...
					m_BaseDirectory = /.../target_name
					m_package_reldir = "packages"
				b. if not found
					m_BaseDirectory = ref_dir/target_name
					m_package_reldir = "packages"
			b. if target is package:
				a. if target name is found in ref_dir
					suppose ref_dir = /.../rel_dir/target_name/...
					if rel_dir is not "packages" then it means that we are not in a standard working copy.
					show warnings, m_BaseDirectory = /..., go on
				b. if not found
					m_BaseDirectory = rel_dir
					m_package_reldir = "packages"
		3. subsequent checking will copy targets to:
			a. target is a project
				put copy to m_BaseDirectory/
			b. target is a package
				put copy to m_BaseDirectory/m_package_reldir
		NOTICE: Never try to checkout more than one project at one time. It will be confusing for package location.
				This also means that we should not let a project/package depend on a project

To choose different version control tools you have to use different classes:
	CheckoutToolSVN: for SVN
	CheckoutToolGIT: for GIT (not supported yet)
	CheckoutToolGITSVN: for GIT-SVN (not supported yet)
Each class support all available dependency managing tools,
so you can just change the first argument "dependency_manage" in initializer to:
	pcmt		(use Python to read from CMT files)
	pcmake		(use Python to read from CMake files)
	cmt			(use CMT directly)
	cmake		(use CMake directly)

Use URLs class to get all relative urls
Use Parser classes to deal with dependency managing
Use RepoStructure to get a list of target objects from repository
	where target objects can store information like whether it has been checked out or not

About verbose control:
	<=0:  nothing but error messages and basic prompt
	>=1:  + checking progress
	>=5:  + important warnings during checking
	>=10: + everything else
"""

import re
import os
import subprocess

from URLs import *
from Parser import *
from RepoStructure import *

__all__ = [
'CheckoutTool',
'CheckoutToolSVN',
'CheckoutToolGIT',
'CheckoutToolGITSVN',
]

##################################################################
# class CheckoutTool
##################################################################

class CheckoutTool(object):
	def __init__(self,depParser,Logfile,verbose=0):
		self.m_status=0
		self.m_Recursive=False
		self.m_BaseDirectory = ""
		self.m_RepoStructure=""
		self.m_verbose=verbose
		self.m_package_reldir = "packages"
		self.m_target = ""
		self.m_target_type = ""
		self.m_version = ""
		self.m_version_type = ""
		self.m_checkoutlog = ""
		self.m_Logfile = Logfile
		result=self.GetRepoStructure()
		if result:
			self.m_status=1 # cannot recogonize URL
		if depParser == "pcmt":
			self.m_DependencyParser = PythonCMTParser(verbose)
		elif depParser == "cmt":
			self.m_DependencyParser = CMTParser(verbose)
		elif depParser == "cmake":
			self.m_DependencyParser = CMakeParser(verbose)
		elif depParser == "pcmake":
			self.m_DependencyParser = PythonCMakeParser(verbose)
		else:
			print "ERROR!!! Cannot recogonize dependency parser type \"%s\"" % (depParser)
			self.m_status=2 # cannot recogonize depParser parser type

	@property
	def status(self):
		return self.m_status
	@property
	def repoStructure(self):
		return self.m_RepoStructure
	@property
	def checkoutlog(self):
		return self.m_checkoutlog

	# check the type of target
	def checkout(self,dest_dir,target,version,recursive=False):

		# analyse target type
		self.UpdateTarget(target,version)
		# if this is the first time, get direcoties and output basic informations
		if self.m_BaseDirectory == "":
			if self.m_target == "":
				print "ERROR!!! Cannot recogonize this primary target \"%s\"!!" % (target)
				return -1
			if dest_dir == "":
				pwd=os.getcwd()
			else:
				pwd = os.path.realpath(dest_dir)
			if self.m_target_type == "project":
				if self.m_target in pwd:
					self.m_BaseDirectory=pwd[:pwd.find(self.m_target)] + self.m_target
				else:
					self.m_BaseDirectory=pwd+"/"+self.m_target
			elif self.m_target_type == "package":
				if self.m_target in pwd:
					self.m_BaseDirectory=pwd[:pwd.find(self.m_target)-1]
					package_reldir = os.path.basename(self.m_BaseDirectory)
					if not package_reldir == self.m_package_reldir:
						self.m_BaseDirectory = self.m_BaseDirectory[:self.m_BaseDirectory.find(package_reldir)-1]
						self.m_package_reldir = package_reldir
						print " WARNING: you are not under a standard directory!! packages will be put to \"%s\"" % (self.m_BaseDirectory+"/"+self.m_package_reldir)
				else:
					self.m_BaseDirectory=pwd
			else:
				print "ERROR!!! primary target \"%s\" is neither a project nor a package!" % (self.m_target)
				# This is the primary target so please stop since it's not recognized and send the error message
				return -1
			print " Destination:         \"%s\"" % (self.m_BaseDirectory)
			print " Target:              \"%s\"(%s)" % (self.m_target,self.m_target_type)

		if self.m_target_type == "":
			if self.m_verbose >= 5:
				print "WARNING!!! \"%s\" is neither a project nor a package!" % (self.m_target)
				print "  will omit this target and continue!"
			return 0
		elif self.m_target_type == "CHECKED":
			return 0 # checked already
		elif self.m_target_type == "OCCURRED":
			return 0 # checked already

		# get options
		self.m_Recursive = recursive
		# get the directory
		target_dir = self.GetDirectory(self.m_target,self.m_target_type)

		# check it out
		if self.m_verbose >= 1:
			print "==> checking out %s[%s] @%s[%s] recursively[%s] to %s" % (self.m_target_type,self.m_target,self.m_version_type,self.m_version,recursive,target_dir)
		status=self.CheckoutTarget(self.m_target,self.m_version,self.m_target_type,self.m_version_type,target_dir)
		if status:
			self.m_status = -1
			print "ERROR!!! in CheckoutTarget(%s,%s,%s,%s,%s)" % (self.m_target,self.m_version_type,self.m_target_type,self.m_version_type,target_dir)
			return -1 # error occurred when CheckoutTarget
		if self.m_target_type == "project":
			self.m_RepoStructure.projects.check(self.m_target,self.m_version)
		elif self.m_target_type == "package":
			self.m_RepoStructure.packages.check(self.m_target,self.m_version)

		# recursive?
		if self.m_Recursive:
			target_list = self.m_DependencyParser.GetTargetList(self.m_target,self.m_target_type,target_dir)
			# this is a list from parser, which we only store information like name and version but no checked status.
			# checking record is only preserved in m_RepoStructure.targets
			if self.m_verbose >= 10:
				print "%s has " % (self.m_target)
				for p in target_list.targets:
					project = self.m_RepoStructure.projects.get(p.name)
					if project:
						text_output = ""
						if project.checked():
							text_output += " checkedout already! version = "
							text_output += project.checkedVersion
						print "  \"%-25s\" @%-15s %s" % (p.name,p.version,text_output)
					else:
						package = self.m_RepoStructure.packages.get(p.name)
						if package:
							text_output = ""
							if package.checked():
								text_output += " checkedout already! version = "
								text_output += package.checkedVersion
							print "  \"%-25s\" @%-15s %s" % (p.name,p.version,text_output)
						else:
							print "  \"%-25s\" @%-15s  !!!is neither a project nor a package" % (p.name,p.version)
			if self.m_DependencyParser.status:
				self.m_status = -3
				print "ERROR!!! cannnot find dependency of \"%s\" @ \"%s\"!" % (self.m_target,target_dir)
				return -3 # cannot get the paser
			for aTarget in target_list.targets:
				result = self.checkout(self.m_BaseDirectory,aTarget.name,aTarget.version,True)
				if result:
					return -1 # error occurred when checkout
		return 0

	def UpdateTarget(self,target,version):
		# update version
		if version == "trunk":
			self.m_version = "trunk"
			self.m_version_type = "trunk"
		elif version == "*":
			#FIXME infact this should refer to the newest tag. Do we need to support regular expression?
			self.m_version = "trunk"
			self.m_version_type = "trunk"
		elif version == "":
			#FIXME infact this should refer to the newest tag. Do we need to support regular expression?
			self.m_version = "trunk"
			self.m_version_type = "trunk"
		else:
			pattern = re.compile(r'.*_t\d+')
			match = pattern.match(version)
			if match:
				self.m_version = version
				self.m_version_type = "branch"
			else:
				self.m_version = version
				self.m_version_type = "tag"
		# update target name
		if target == "":
			top_dir = os.getcwd()
			self.m_target = self.dir2name(top_dir)
		else:
			self.m_target = target
		project = self.m_RepoStructure.projects.get(self.m_target)
		package = self.m_RepoStructure.packages.get(self.m_target)
		unrecogonized = self.m_RepoStructure.unrecogonized.get(self.m_target)
		if project:
			if project.checked():
				self.m_target_type = "CHECKED"
				if not self.m_version == project.checkedVersion:
					if self.m_verbose >= 5:
						print "WARNING: \"%s\": current required version(%s) does not match previous checked version(%s)!!!" % (package.name,self.m_version,project.checkedVersion)
			else:
				self.m_target_type = "project"
		elif package:
			if package.checked():
				self.m_target_type = "CHECKED"
				if not self.m_version == package.checkedVersion:
					if self.m_verbose >= 5:
						print "WARNING: \"%s\": current required version(%s) does not match previous checked version(%s)!!!" % (package.name,self.m_version,package.checkedVersion)
			else:
				self.m_target_type = "package"
		elif unrecogonized:
			self.m_target_type = "OCCURRED"
		else:
			self.m_RepoStructure.unrecogonized.append(self.m_target,self.m_version)
			self.m_target_type = "" # cannnot recogonize this target!

	def GetDirectory(self,target,target_type):
		if target_type == "project":
			return self.m_BaseDirectory
		elif target_type == "package":
			return self.m_BaseDirectory + "/" + self.m_package_reldir + "/" + target

	def GetRepoStructure(self):
		# prepare urls, repoStructure, and Base Directory
		self.m_urls=URLs()
		print "Checking projects and packages repoStructure from \"%s\"" % (self.m_urls.base_url)
		self.m_RepoStructure=RepoStructure(self.m_urls)
		if self.m_RepoStructure.status:
			return 1 # cannot recogonize structure from the given url
		return 0

	def dir2name(self,directory):
		for p in self.m_RepoStructure.projects.names:
			if p in directory:
				return p
		for p in self.m_RepoStructure.packages.names:
			if p in directory:
				return p
		return ""

	def writeToFile(self):
		f = open(self.m_Logfile, "w")
		f.write(self.m_checkoutlog)
		f.close()

	def CheckoutTarget(self,target,version,target_type,version_type,target_dir):
		print "  ##Check out using CheckoutTool (?could not be...)"

##################################################################
# class CheckoutToolSVN
##################################################################
class CheckoutToolSVN(CheckoutTool):
	def __init__(self,depParser,Logfile,verbose=0):
		CheckoutTool.__init__(self,depParser,Logfile,verbose)

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
		self.m_checkoutlog = "svn checkout " + url + " " + target_dir + ":\n"
		self.m_checkoutlog += p.stdout.read()
		self.writeToFile()
		if p.returncode:
			return 1
		return 0

##################################################################
# class CheckoutToolGIT
##################################################################
class CheckoutToolGIT(CheckoutTool):
	def __init__(self,depParser,Logfile,verbose=0):
		CheckoutTool.__init__(self,depParser,Logfile,verbose)

	def GetRepoStructure(self):
		return 0

	def CheckoutTarget(self,target,version,target_type,version_type,target_dir):
		return 0

##################################################################
# class CheckoutToolGITSVN
##################################################################
class CheckoutToolGITSVN(CheckoutTool):
	def __init__(self,depParser,Logfile,verbose=0):
		CheckoutTool.__init__(self,depParser,Logfile,verbose)

	def GetRepoStructure(self):
		return 0

	def CheckoutTarget(self,target,version,target_type,version_type,target_dir):
		return 0

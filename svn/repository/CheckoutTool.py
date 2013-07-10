
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
import sys
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
		self.m_recursive=False
		self.m_BaseDirectory = ""
		self.m_RepoStructure=""
		self.m_verbose=verbose
		self.m_package_reldir = "packages"
		self.m_target = ""
		self.m_target_type = ""
		self.m_version = ""
		self.m_version_type = ""
		self.m_checkoutlog = ""
		self.m_LogfileName = Logfile
		self.m_Logfile = ''
		self.m_target_num = 0
		try:
			self.m_Logfile = open(Logfile,'w')
		except:
			print "ERROR!!! Cannot open \"%s\"" % (self.m_LogfileName)
			sys.exit(-1)
		print "Retrieving packages and projects list ..."
		sys.stdout.flush()
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

##################################################################
	@property
	def status(self):
		return self.m_status
	@property
	def repoStructure(self):
		return self.m_RepoStructure
	@property
	def checkoutlog(self):
		return self.m_checkoutlog

##################################################################
	def checkout(self,dest_dir,target,version,recursive=False,test=False):

		# get options
		self.m_recursive = recursive
		self.m_test = test

		# update target and version information
		self.AnalyseVersion(version)
		self.AnalyseTarget(target)

		# if this is the first time, get direcoties and output basic informations
		if self.m_BaseDirectory == "":
			result = self.GetBaseDirectory(dest_dir)
			if result:
				# This is the primary target so please stop since it's not recognized and send the error message
				return -1
			print " Destination:         \"%s\"" % (self.m_BaseDirectory)
			print " Target:              \"%s\"(%s)" % (self.m_target,self.m_target_type)
			print "**************************************************************"

		# get direcoty for this checkout
		self.GetDirectory()

		# check target type
		if self.m_target_type == "":
			if self.m_verbose >= 5:
				print "WARNING!!! \"%s\" is neither a project nor a package!" % (self.m_target)
				print "  will omit this target and continue!"
			return 0 # ignore the unrecognized
		elif self.m_target_type == "CHECKED":
			return 0 # checked already
		elif self.m_target_type == "OCCURRED":
			return 0 # checked already

		# check it out
		if self.m_verbose >= 5:
			self.m_target_num += 1;
			print "==> [%s] checking out \"%s\" @[%s]" % (self.m_target_num,self.m_target,self.m_version)
			sys.stdout.flush() # see what happended everytime we checked something down
		status=self.CheckoutTarget(self.m_test)
		if status:
			self.m_status = -1 # single package download error
			print "ERROR!!! in CheckoutTarget for \"%s\"[%s] @%s[%s] recursively[%s] to \"%s\"" % (self.m_target,self.m_target_type,self.m_version,self.m_version_type,self.m_recursive,self.m_target_dir)
			# This will return back to upper loop and let the upper loop deal with this error status
			# Since this package is possibly containing requirements file, we had better try to read it anyway
		else:
			if self.m_verbose >= 5:
				print "		Successful!"

		# record check information
		if self.m_target_type == "project":
			self.m_RepoStructure.projects.check(self.m_target,self.m_version)
		elif self.m_target_type == "package":
			self.m_RepoStructure.packages.check(self.m_target,self.m_version)

		# recursive?
		if self.m_recursive:
			requirements = os.path.join(self.m_target_dir, "cmt", "requirements")
			target_list = self.m_DependencyParser.GetTargetList(requirements)
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
								text_output += " checked already @["
								text_output += package.checkedVersion + ']'
							print "  %-25s @%-15s %s" % ('"'+p.name+'"','['+p.version+']',text_output)
						else:
							print "  %-25s @%-15s  !!!is neither a project nor a package" % ('"'+p.name+'"','['+p.version+']')
			if self.m_DependencyParser.status:
				if self.m_verbose >= 5:
					print "WARNING!!! status(%d) cannnot find dependency of \"%s\" @ \"%s\"!" % (self.m_DependencyParser.status,self.m_target,self.m_target_dir)
					return 0 # cannot get the paser
					# cannot continue with a further loop, back to upper loop
					# since this is a minor error, so we have to return successful status so that the upper loop can keep going
			for aTarget in target_list.targets:
				result = self.checkout(self.m_BaseDirectory,aTarget.name,aTarget.version,True,self.m_test)
				if result: # must be serious error that should force us to stop the whole process
					if self.m_verbose >= 0:
						print "ERROR!!! serious error occurred when checkout \"%s\" @ \"%s\"!" % (self.m_target,self.m_version)
					return 1 # terminate the loop and stop checking process recursively
		return 0 # successful

##################################################################
	def AnalyseVersion(self,version):
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

##################################################################
	def AnalyseTarget(self,target):
		# update target name
		if target == "":
			top_dir = os.getcwd()
			self.m_target = self.dir2name(top_dir)
		else:
			self.m_target = target
		project = self.m_RepoStructure.projects.get(self.m_target)
		package = self.m_RepoStructure.packages.get(self.m_target)
		unrecognized = self.m_RepoStructure.unrecognized.get(self.m_target)
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
		elif unrecognized:
			self.m_target_type = "OCCURRED"
		else:
			self.m_RepoStructure.unrecognized.append(self.m_target,self.m_version)
			self.m_target_type = "" # cannnot recogonize this target!

##################################################################
	def GetDirectory(self):
		if self.m_target_type == "project":
			self.m_target_dir = self.m_BaseDirectory
		elif self.m_target_type == "package":
			self.m_target_dir = self.m_BaseDirectory + "/" + self.m_package_reldir + "/" + self.m_target + "/" + self.m_version

##################################################################
	def GetBaseDirectory(self,dest_dir):
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

##################################################################
	def GetRepoStructure(self):
		# prepare urls, repoStructure, and Base Directory
		self.m_urls=URLs()
		print "Checking projects and packages repoStructure from \"%s\"" % (self.m_urls.base_url)
		self.m_RepoStructure=RepoStructure(self.m_urls)
		if self.m_RepoStructure.status:
			return 1 # cannot recogonize structure from the given url
		return 0

##################################################################
	def dir2name(self,directory):
		for p in self.m_RepoStructure.projects.names:
			if p in directory:
				return p
		for p in self.m_RepoStructure.packages.names:
			if p in directory:
				return p
		return ""

##################################################################
	def CheckoutTarget(self,test=False):
		print "  ##Check out using CheckoutTool (?could not be...)"

##################################################################
# class CheckoutToolSVN
##################################################################
class CheckoutToolSVN(CheckoutTool):
	def __init__(self,depParser,Logfile,verbose=0):
		CheckoutTool.__init__(self,depParser,Logfile,verbose)

##################################################################
	def GetRepoStructure(self):
		# prepare urls, repoStructure, and Base Directory
		self.m_urls=URLs()
		print "Checking projects and packages repoStructure from \"%s\"" % (self.m_urls.base_url)
		self.m_RepoStructure=RepoStructureSVN(self.m_urls)
		if self.m_RepoStructure.status:
			return 1 # cannot recogonize structure from the given url
		return 0

##################################################################
	def CheckoutTarget(self,test=False):
		url = ""
		if self.m_target_type == "project":
			url = self.m_urls.project_url + "/" + self.m_target
		elif self.m_target_type == "package":
			url = self.m_urls.package_url + "/" + self.m_target
		if self.m_version_type == "trunk":
			url = url + "/trunk"
		elif self.m_version_type == "tag":
			url = url + "/" + self.m_version
		elif self.m_version_type == "branch":
			url = url + "/branches/" + self.m_version

		destination = self.m_target_dir

		if test:
			url += "/cmt"
			destination += "/cmt"

		if self.m_verbose >= 10:
			print "    #$ svn checkout %s %s" % (url,destination)
			sys.stdout.flush() # see what happended everytime we checked something down

		self.m_checkoutlog = "#$ svn checkout " + url + " " + destination + "\n"
		self.m_Logfile.write(self.m_checkoutlog)
		self.m_Logfile.flush()

		p = subprocess.Popen(["svn","checkout",url,destination],stdout=subprocess.PIPE,stderr = subprocess.PIPE)
		p.wait()

		self.m_checkoutlog = p.stdout.read()
		self.m_checkoutlog += p.stderr.read()
		self.m_Logfile.write(self.m_checkoutlog)
		self.m_Logfile.flush()
		if p.returncode:
			return 1
		return 0

##################################################################
# class CheckoutToolGIT
##################################################################
class CheckoutToolGIT(CheckoutTool):
	def __init__(self,depParser,Logfile,verbose=0):
		CheckoutTool.__init__(self,depParser,Logfile,verbose)

##################################################################
	def GetRepoStructure(self):
		return 0

##################################################################
	def CheckoutTarget(self,test=False):
		return 0

##################################################################
# class CheckoutToolGITSVN
##################################################################
class CheckoutToolGITSVN(CheckoutTool):
	def __init__(self,depParser,Logfile,verbose=0):
		CheckoutTool.__init__(self,depParser,Logfile,verbose)

##################################################################
	def GetRepoStructure(self):
		return 0

##################################################################
	def CheckoutTarget(self,test=False):
		return 0

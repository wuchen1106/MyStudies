#!/usr/bin/env python2

# FIXME names here are totally a mess!
# FIXME status control is totally a mess!
# FIXME and output!!!

"""
Script to checkout from ICEDUST repository
"""
import os
import sys
import re
import subprocess

##################################################################
# class URLs
##################################################################
class URLs():
	def __init__(self, BASE_URL = "https://comet-framework-chen.googlecode.com/svn/trunk"):
		self.m_base_url = BASE_URL
		self.m_package_url = BASE_URL + "/packages"
		self.m_project_url = BASE_URL + "/projects"

	@property
	def base_url(self):
		return self.m_base_url

	@property
	def package_url(self):
		return self.m_package_url

	@property
	def project_url(self):
		return self.m_project_url

##################################################################
# class Target
##################################################################
class Target():
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
	def checkedVersion(self):
		return self.m_checkedVersion
	@property
	def name(self):
		return self.m_name
##################################################################
# class Package
##################################################################
class Package(Target):
	def __init__(self,name):
		Target.__init__(self,name)
##################################################################
# class Project
##################################################################
class Project(Target):
	def __init__(self,name):
		Target.__init__(self,name)

##################################################################
# class TargetList 
##################################################################
class TargetList():
	def __init__(self):
		self.m_TargetList = []
		self.m_type = "TargetList"

	def get(self,name):
		for aTarget in self.m_TargetList:
			if aTarget.name == name:
				return aTarget
		return 0

	def append(self,name):
		aTarget = self.get(name)
		if not aTarget:
			new_Target=Target(name)
			self.m_TargetList.append(new_Target)

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
		print "##%s has:" % (self.m_type)
		for aTarget in self.m_TargetList:
			print "  %-25s checked?[%s] for \"%s\"" % (aTarget.name,aTarget.checked(),aTarget.version)
##################################################################
# class PackageList 
##################################################################
class PackageList(TargetList):
	def __init__(self):
		TargetList.__init__(self)
		self.m_type = "PackageList"

	def append(self,name):
		aPackage = self.get(name)
		if not aPackage:
			new_Package=Package(name)
			self.m_TargetList.append(new_Package)
##################################################################
# class ProjectList 
##################################################################
class ProjectList(TargetList):
	def __init__(self):
		TargetList.__init__(self)
		self.m_type = "ProjectList"

	def append(self,name):
		aProject = self.get(name)
		if not aProject:
			new_Project=Project(name)
			self.m_TargetList.append(new_Project)

##################################################################
# class RepoStructure
##################################################################
class RepoStructure():
	def __init__(self,urls):
		self.m_packages = self.get_package_list(urls)
		self.m_projects = self.get_project_list(urls)
		self.m_status = 0

	def get_package_list(self,urls):
		return []

	def get_project_list(self,urls):
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

	def get_package_list(self,urls):
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

	def get_project_list(self,urls):
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

##################################################################
# class BaseParser
##################################################################
class BaseParser():
	def __init__(self):
		self.m_status=0
		self.m_TargetList=TargetList()

	@property
	def status(self):
		return self.m_status

	def GetList(self,target,target_type,target_dir):
		aTarget = Target("Example","")
		self.m_TargetList.append(aTarget)
		return self.m_TargetList

##################################################################
# class PythonCMTParser
##################################################################
class PythonCMTParser(BaseParser):
	def __init__(self):
		BaseParser.__init__(self)

	def GetList(self,target,target_type,target_dir):
		requirements = os.path.join(target_dir, "cmt", "requirements")
		if not os.path.isfile(requirements):
			self.m_status=1 # cannot open the requirements file
			return ""
		pattern = re.compile(r'\Ause\Z[ \t]+\A(.*)\Z[ \t]+\A(.*)\Z')
		statement = ""
		for l in open(requirements):
			if '#' in l:
				l = l[:l.find('#')]
			l = l.strip()
			# if we have something in the line, extend the statement
			if l:
				statement += l
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
					version = match.group(2)
					aTarget = Target(name,version)
					self.m_TargetList.append(aTarget)
		return self.m_TargetList

##################################################################
# class CheckoutTool
##################################################################
class CheckoutTool():
	def __init__(self,depParser):
		self.m_status=0
		self.m_Recursive=False
		self.m_RepoStructure=""
		self.m_BaseDirectory = GetBaseDir(os.getcwd())
		result=self.GetRepoStructure()
		if result:
			self.m_status=1 # cannot recogonize URL
		else:
			self.m_RepoStructure.projects.Dump()
			self.m_RepoStructure.packages.Dump()
		if depParser == "CMT":
			self.m_DependencyParser = CMTParser()
		elif depParser == "CMake":
			self.m_DependencyParser = CMakeParser()
		elif depParser == "PythonCMake":
			self.m_DependencyParser = PythonCMakeParser()
		elif depParser == "PythonCMT":
			self.m_DependencyParser = PythonCMTParser()
		else:
			print "ERROR!!! Cannot recogonize dependency parser type \"%s\"" % (depParser)
			self.m_status=2 # cannot recogonize depParser parser type

	@property
	def status(self):
		return self.m_status

	# check the type of target
	def checkout(self,target,version,recursive=False):
		self.m_Recursive = recursive

		# Analyse target type
		target_type = self.GetTargetType(target)
		if target_type == "":
			print "ERROR!!! \"%s\" is neither a project nor a package!" % (target)
			self.m_status = 1
			return 1 # Cannot find target

		# check version type
		version_type = self.GetVersionType(version)

		# get the directory
		target_dir = self.GetDirectory(target,target_type)

		# check it out
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
			print "Will do it recursively!"
			target_list = self.m_DependencyParser.GetList(target,target_type,target_dir)
			if self.m_DependencyParser.status:
				self.m_status = -3
				print "ERROR!!! cannnot find dependency of \"%s\" @ \"%s\"!" % (target,target_dir)
				return -3 # cannot get the paser
			for aTarget in target_list:
				result = self.checkout(aTarget.name,aTarget.version,True)
				if result:
					return -1 # error occurred when checkout
		return 0

	def GetTargetType(self,target):
		project = self.m_RepoStructure.projects.get(target)
		package = self.m_RepoStructure.packages.get(target)
		if project:
			return "project"
		elif package:
			return "package"
		else:
			return "" # cannnot recogonize this target!

	def GetVersionType(self,version):
		if version == "trunk":
			return "trunk"
		elif version == "*":
			#FIXME need a regular expression
			return "branch"
		else:
			return "tag"

	def GetDirectory(self,target,target_type):
		if target_type == "project":
			return self.m_BaseDirectory
		elif target_dir == "package":
			#FIXME Problem!!!
			return self.m_BaseDirectory

	def GetRepoStructure(self):
		# prepare urls, repoStructure, and Base Directory
		urls=URLs("https://comet-framework-chen.googlecode.com/svn/trunk")
		print "Checking projects and packages repoStructure from %s" % (urls.base_url)
		self.m_RepoStructure=RepoStructure(urls)
		if self.m_RepoStructure.status:
			return 1 # cannot recogonize structure from the given url
		return 0

	def CheckoutTarget(self,target,version,target_type,version_type,target_dir):
		print "  ##Check out using CheckoutTool"
##################################################################
# class CheckoutToolSVN
##################################################################
class CheckoutToolSVN(CheckoutTool):
	def __init__(self,depParser):
		CheckoutTool.__init__(self,depParser)

	def GetRepoStructure(self):
		# prepare urls, repoStructure, and Base Directory
		urls=URLs("https://comet-framework-chen.googlecode.com/svn/trunk")
		print "Checking projects and packages repoStructure from %s" % (urls.base_url)
		self.m_RepoStructure=RepoStructureSVN(urls)
		if self.m_RepoStructure.status:
			return 1 # cannot recogonize structure from the given url
		return 0

	def CheckoutTarget(self,target,version,target_type,version_type,target_dir):
		print "  ##Check out using CheckoutToolSVN"

##################################################################
# function dir2name
##################################################################
def dir2name(directory):
#	FIXME: need implementation
	return directory

##################################################################
# function Dirs
##################################################################
def GetBaseDir(aDir):
#	FIXME: need implementation
	return aDir

##################################################################
# main
##################################################################
def main(args=None):
# read options and arguments
	from optparse import OptionParser
	parser = OptionParser(usage="%prog [options] [project or package]",
						  description="Checkout a working copy from remote repository")
	parser.add_option("-R", "--recursive", action="store_true",
					  help="Checkout all packages/projects required by the given one recursively")
	parser.add_option("-N", "--norecursive", action="store_true",
					  help="Disable recursive mode. Checkout the specified project or package only")
	parser.add_option('-v' ,'--version', dest='version',
					  help='Specify a tag/branch for the project or package')

	opts, args = parser.parse_args(args=args)

# default setting
	# by default the package/project containing current directory will be considered as the original target
	top_dir = os.getcwd()
	target = dir2name(top_dir) # Let's leave the safty check to CheckoutTool, which acturally defines the format of target.
	recursive = True
	version = "trunk"

# update
	if args:
		target = args[0]
	if opts.recursive:
		recursive = True
	if opts.norecursive:
		recursive = False
	if opts.version:
		version = opts.version

# display options and arguments
	print "#########################################################"
	print "Target:              %s" % (target)
	print "Recusive?            %s" % (recursive)
	print "Version:             %s" % (version)
	print "#########################################################"

# a dedicated function is supposed to checkout the target recursively or not
	# choose repository type and dependency managing tool here
	# currently supported:
	#	repository managing tool:
	#       SVN:		CheckoutToolSVN
	#       GIT:     	(Developing...)
	#       GIT-SVN: 	(Developing...)
	#	dependency managing tool:\
	#		PythonCMT   "PythonCMT"			PythonCMTParser
	#		PythonCMake (Developing...)
	#		CMT			(Developing...)
	#		CMake		(Developing...)
	myCheckoutTool = CheckoutToolSVN("PythonCMT")
	if not myCheckoutTool.status:
		myCheckoutTool.checkout(target,version,recursive)
	if myCheckoutTool.status:
		print "FAILED!!!"
		sys.exit(1)
	else:
		print "SUCCESSFUL!"
		sys.exit(0)

if __name__ == '__main__':
	main()
	sys.exit(0)

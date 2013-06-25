#!/usr/bin/env python2
"""
Script to checkout from ICEDUST repository
"""
import os
import sys
import re
import logging
import shelve
import json
import operator
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
# class Item
##################################################################
class Item():
	def __init__(self,name):
		self.m_name = name
		self.m_logged = False
		self.m_version = ""

	def logged(self):
		if self.m_logged:
			return 1 # logged already
		return 0

	def log(self,version):
		if self.logged():
			return 1 # already logged
		else:
			self.m_logged = True
			self.m_version = version

	@property
	def version(self):
		return self.m_version
	@property
	def name(self):
		return self.m_name
##################################################################
# class Package
##################################################################
class Package(Item):
	def __init__(self,name):
		Item.__init__(self,name)
##################################################################
# class Project
##################################################################
class Project(Item):
	def __init__(self,name):
		Item.__init__(self,name)

##################################################################
# class ItemList 
##################################################################
class ItemList():
	def __init__(self):
		self.m_ItemList = []
		self.m_type = "ItemList"

	def get(self,name):
		for aItem in self.m_ItemList:
			if aItem.name == name:
				return aItem
		return 0

	def append(self,name):
		aItem = self.get(name)
		if not aItem:
			new_Item=Item(name)
			self.m_ItemList.append(new_Item)

	def log(self,name,version):
		aItem=self.get(name)
		if not aItem:
			return -1 # cannot get this Item
		elif aItem.logged():
			return 1 # already logged
		else:
			aItem.log(version)

	def Dump(self):
		print "##%s has:" % (self.m_type)
		for aItem in self.m_ItemList:
			print "  %-25s checked?[%s] for \"%s\"" % (aItem.name,aItem.logged(),aItem.version)
##################################################################
# class PackageList 
##################################################################
class PackageList(ItemList):
	def __init__(self):
		ItemList.__init__(self)
		self.m_type = "PackageList"

	def append(self,name):
		aPackage = self.get(name)
		if not aPackage:
			new_Package=Package(name)
			self.m_ItemList.append(new_Package)
##################################################################
# class ProjectList 
##################################################################
class ProjectList(ItemList):
	def __init__(self):
		ItemList.__init__(self)
		self.m_type = "ProjectList"

	def append(self,name):
		aProject = self.get(name)
		if not aProject:
			new_Project=Project(name)
			self.m_ItemList.append(new_Project)

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
			print "Cannot recogonize URL: %s" % (urls.package_url)
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
			print "Cannot recogonize URL: %s" % (urls.project_url)
		else:
			for i in l:
				alist.append(i[:-2])
		return alist

##################################################################
# function dir2name
##################################################################
def dir2name(directory):
#	FIXME: need implementation
	return directory


##################################################################
# function checkout
##################################################################
def checkout(target,version,recursive,urls,repoStructure):
	# check the type of target
	target_type = ""
	project = repoStructure.projects.get(target)
	package = repoStructure.packages.get(target)
	if project:
		target_type = "project"
		print "\"%s\" is a project!" %(target)
	elif package:
		target_type = "package"
		print "\"%s\" is a package!" %(target)
	else:
		print "\"%s\" is neither a project nor a package!" % (target)
		return -1

	# check the version
	version_type = ""
	if version == "trunk":
		version_type = "trunk"
	elif version == "*":
		version_type = "branch"
	else:
		version_type = "tag"

	# output the arguments
	print "==> checking out %s[%s] @%s[%s] recursively[%s]" % (target_type,target,version_type,version,recursive)

	# check out the targe

	print "==> checkout finished"

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
#    parser.add_option("-x", "--xml",
#                      action="callback", callback=addOperation,
#                      type="str", nargs=1, callback_args=('loadXML',),
#                      help="XML file describing the changes to the environment")

	opts, args = parser.parse_args(args=args)

# default setting
	# by default the package/project containing current directory will be considered as the original target
	top_dir = os.getcwd()
	ori_target = dir2name(top_dir) # Let's leave the safty check to checkout function, which acturally defines the format of target.
	recursive = True
	version = "trunk"

# update
	if args:
		ori_target = args[0]
	if opts.recursive:
		recursive = True
	if opts.norecursive:
		recursive = False
	if opts.version:
		version = opts.version

# display options and arguments
	print "#########################################################"
	print "Target:              %s" % (ori_target)
	print "Recusive?            %s" % (recursive)
	print "Version:             %s" % (version)
	print "#########################################################"

# prepare urls and repoStructure
	urls=URLs("https://comet-framework-chen.googlecode.com/svn/trunk")
	print "Checking projects and packages repoStructure from %s" % (urls.base_url)
	repoStructure=RepoStructureSVN(urls) # FIXME need to offer more choices instead of SVN only
	if repoStructure.status:
		exit(1)
	repoStructure.projects.Dump()
	repoStructure.packages.Dump()

# a dedicated function is supposed to checkout the target recursively or not
	status = checkout(ori_target,version,recursive,urls,repoStructure)
	if status:
		print "FAILED!!!"
		sys.exit(1)
	else:
		print "SUCCESSFUL!"
		sys.exit(0)

if __name__ == '__main__':
	main()
	sys.exit(0)

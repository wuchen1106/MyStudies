#!/usr/bin/env python2

# FIXME names here are totally a mess!
# FIXME status control is totally a mess!
# FIXME and output!!!
# FIXME what if co a target costs too much time?
# FIXME version consistency check

"""
Script to checkout from ICEDUST repository

It is supposed to support different version control tools (like SVN) and dependency managing tools (like CMT)
although some functionalities are still missing...
You have to modify this code to change this external tool set.
The line where 'myCheckoutTool' is declared is the place you choose tool sets:
	myCheckoutTool = CHECKOUTTOOL("XXX",verbose)
	where CHECKOUTTOOL can be:
		 CheckoutToolSVN	(for SVN)
    	*CheckoutToolGIT	(for GIT)
    	*CheckoutToolGITSVN	(for GIT-SVN)
	and "XXX" can be:
		 "PythonCMT"	(use Python to read from CMT files)
		*"PythonCMake"	(use Python to read from CMake files)
		*"CMT"			(use CMT directly)
		*"CMake"		(use CMake directly)
	options with '*' means under developing and currently not supported yet.

if no target specified, package/project containing current directory will be considered as the original target
if no destination directory specified, current directory will be set as the base directory holding everything you check down
"""

import os
import sys
import signal

import CheckoutToolSVN

#################################################################
def signal_handler(signal,frame):
	print '\nYou pressed Ctrl+C!\n'
	sys.exit(0);
signal.signal(signal.SIGINT, signal_handler)

##################################################################
def dir2name(directory,aRepoStructure):
	name=""
	prodir=""
	for p in aRepoStructure.projects.names:
		if p in directory:
			prodir = directory[:directory.find(p)] + p
			name = p
			break
	if not name == "":
		pacbasedir = prodir + "/packages"
		if pacbasedir in directory:
			reldir = directory[len(reldir)+1:]
			pacname = reldir[:reldir.find('/')]
			for p in aRepoStructure.packages.names:
				if p == pacname:
					name = p
					break
	return name

##################################################################
def main(args=None):
# read options and arguments
	from optparse import OptionParser
	parser = OptionParser(usage="%prog [options] [project or package] [destination]",
						  description="Checkout a working copy from remote repository")
	parser.add_option("-R", "--recursive", action="store_true",
					  help="Checkout all packages/projects required by the given one recursively")
	parser.add_option("-N", "--norecursive", action="store_true",
					  help="Disable recursive mode. Checkout the specified project or package only")
	parser.add_option('-V' ,'--version', dest='version',
					  help='Specify a tag/branch for the project or package')
	parser.add_option('-v' ,'--verbose', dest='verbose',
					  help= 'Specify a verbose level\n \
							<=0:  nothing but error messages and basic prompt\n \
							>=1:  + checking progress	(default level)\n \
							>=10: + everything else\n'
					  )

	opts, args = parser.parse_args(args=args)

# default setting
	target = ""
	recursive = True
	version = "trunk"
	verbose = 1

# update
	if opts.recursive:
		recursive = True
	if opts.norecursive:
		recursive = False
	if opts.version:
		version = opts.version
	if opts.verbose:
		verbose = opts.verbose

# prepare toolkit
	# choose repository type and dependency managing tool here
	myCheckoutTool = CheckoutToolSVN("PythonCMT",verbose)
	if myCheckoutTool.status:
		print "FAILED!!!"
		sys.exit(1)

	if args:
		top_dir = ""
		target = args[0]
	else: # if no target specified, package/project containing current directory will be considered as the original target
		top_dir = os.getcwd()
		target = dir2name(top_dir,myCheckoutTool.repoStructure)
	if len(args) >= 2:
		dest_dir = args[1]
	else: # if no destination directory specified, current directory will be set as the base directory holding everything you check down
		dest_dir = os.getcwd()

	if target == "":
		print "ERROR: cannot recogonize current directory! Please specify a target and the current directory will be considered as base directory"
		sys.exit(-1)

# display options and arguments
	print "#########################################################"
	print "Target:              %s" % (target)
	print "Recusive?            %s" % (recursive)
	print "Version:             %s" % (version)
	print "verbose:             %s" % (verbose)
	print "#########################################################"

# a dedicated function is supposed to checkout the target recursively or not
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

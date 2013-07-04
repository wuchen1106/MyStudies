#!/usr/bin/env python2

# FIXME names here are totally a mess!
# FIXME status control is totally a mess!
# FIXME and output!!!
# FIXME what if co a target costs too much time?
# FIXME version consistency check

"""
Script to checkout from ICEDUST repository

It is supposed to support different version control tools (default as SVN)
and dependency managing tools (default as CMT)
although some functionalities are still missing...
To choose other tools use '-c' and '-d' options respectively:
	-c:
		 svn		(for SVN)
    	*git		(for GIT)
    	*gitsvn		(for GIT-SVN)
    -d:
		 pcmt		(use Python to read from CMT files)
		*pcmake		(use Python to read from CMake files)
		*cmt		(use CMT directly)
		*cmake		(use CMake directly)
	options with '*' means under developing and not supported yet.

Option version is a string following a given syntax which can be understood by CheckoutTool.
By default it is set to "trunk" if not set explicitly
Default destination and target are blank and CheckoutTool will deal with them properly.
Conventions should follow:
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

About verbose control:
	<=0:  nothing but error messages and basic prompt
	>=1:  + checking progress
	>=5:  + important warnings during checking	(default level)
	>=10: + everything else
"""

import os
import sys
import signal

from CheckoutTool import *

#################################################################
def signal_handler(signal,frame):
	print '\nYou pressed Ctrl+C!\n'
	sys.exit(0);
signal.signal(signal.SIGINT, signal_handler)

#################################################################
def writeToFile(filename, data):
	f = open(filename, "w")
	f.write(data)
	f.close()

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
							>=1:  + checking progress\n \
							>=5:  + important warnings during checking	(default level)\n \
							>=10: + everything else\n'
					  )
	parser.add_option('-c' ,'--version-control', dest='version_control',
					  help= 'Specify a version control tool\n \
							svn		(for SVN)(default)\n \
							git		(for GIT)(not supported yet)\n \
							gitsvn	(for GIT-SVN)(not supported yet)\n'
					  )
	parser.add_option('-d' ,'--dependency-manage', dest='dependency_manage',
					  help= 'Specify a dependency manage tool\n \
							pcmt	(use Python to read from CMT files)(default)\n \
							pcmake	(use Python to read from CMake files)(not supported yet)\n \
							cmt		(use CMT directly)(not supported yet)\n \
							cmake	(use CMake directly)(not supported yet)\n'
					  )
	parser.add_option('-l' ,'--Logfile', dest='Logfile',
					  help= 'Specify a Logfile where checkout logs should be put in\n'
					  )

	opts, args = parser.parse_args(args=args)

# default setting
	dest_dir = ""
	target = ""
	recursive = True
	version = "trunk"
	verbose = 5
	version_control = "svn"
	dependency_manage = "pcmt"
	Logfile = "ICEDUST_checkoutlog.txt"

# update
	if opts.recursive:
		recursive = True
	if opts.norecursive:
		recursive = False
	if opts.version:
		version = opts.version
	if opts.verbose:
		verbose = opts.verbose
	if opts.version_control:
		version_control=opts.version_control
	if opts.dependency_manage:
		dependency_manage = opts.dependency_manage
	if opts.Logfile:
		Logfile = opts.Logfile
	if args:
		target = args[0]
	if len(args) >= 2:
		dest_dir = args[1]

# display options and arguments
	print "#########################################################"
	print "Recusive?            %s"		% (recursive)
	print "Version:             \"%s\"" % (version)
	print "Verbose Level:       %s"		% (verbose)
	print "Version Control:     \"%s\""	% (version_control)
	print "Dependency Manage:   \"%s\""	% (dependency_manage)
	print "Logfile:				\"%s\""	% (Logfile)
	print "#########################################################"

# prepare toolkit
	# choose repository type and dependency managing tool here
	myCheckoutTool = ''
	if version_control == "svn":
		myCheckoutTool = CheckoutToolSVN(dependency_manage,verbose)
	elif version_control == "git":
		myCheckoutTool = CheckoutToolGIT(dependency_manage,verbose)
	elif version_control == "gitsvn":
		myCheckoutTool = CheckoutToolGITSVN(dependency_manage,verbose)
	else:
		print "ERROR: \"%s\" is not supported yet for version control!!!" % (version_control)
		sys.exit(1)

	if myCheckoutTool.status:
		print "FAILED in initializing CheckoutTool!!!"
		sys.exit(1)

# a dedicated function is supposed to checkout the target recursively or not
	myCheckoutTool.checkout(dest_dir,target,version,recursive)
	writeToFile(Logfile,myCheckoutTool.checkoutlog)

	if myCheckoutTool.status:
		print "FAILED!!!"
		sys.exit(1)
	else:
		print "SUCCESSFUL!"
		sys.exit(0)

if __name__ == '__main__':
	main()
	sys.exit(0)

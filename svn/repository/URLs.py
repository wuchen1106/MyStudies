
import os

__all__ = [
'URLs',
]

##################################################################
# class URLs
##################################################################
class URLs(object):
	def __init__(self, BASE_URL = "https://www.muec-uk.org/muecuk/COMET/ICEDUST"):
		BASE_URL_env = os.getenv('ICEDUST_REPOSITORY','NONE')
		if BASE_URL_env == 'NONE':
			print "WARNING: environment viriable ICEDUST_REPOSITORY is not set!"
			print "  will use \"%s\" as default" % (BASE_URL)
		else:
			BASE_URL = BASE_URL_env
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


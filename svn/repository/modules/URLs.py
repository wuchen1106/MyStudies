
__all__ = [
'URLs',
]

##################################################################
# class URLs
##################################################################
class URLs():
	def __init__(self, BASE_URL = "https://www.muec-uk.org/muecuk/COMET/ICEDUST"):
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


include $(top_srcdir)/Common.mk
include $(srcdir)/../CodacDev.mk
include $(srcdir)/../CodacDan.mk


## ////////////////////////////////////////////////////////////////////////// ##
## /// UNITS //////////////////////////////////////////////////////////////// ##
## ////////////////////////////////////////////////////////////////////////// ##

NAME     ?= $(lastword $(IC_UNITS))

IC_UNITS  = \
			m-DAN-sample \
			m-DAN-thrput

DEV_UNITS = \
			m-maven-iter-settings \
			m-codac-unit-api \
			m-maven-iter-plugin \
			m-dan-common \
			m-dan-daq \
			m-collectd-ccs \
			m-collector-lib \
			m-dan-archiver-reader \
			m-dan-archiver-writer



m_maven_iter_settings_SVN_DEP =

m_codac_unit_api_SVN_DEP = m-maven-iter-settings


m_maven_iter_plugin_SVN_BRA = trunk
m_maven_iter_plugin_SVN_DEP = m-maven-iter-settings \
							  m-codac-unit-api

# // DO NOT USE TRUNK FOR 6.0 //

#m_collector_lib_SVN_DEP = m-collectd-ccs
#m_dan_archiver_writer_SVN_DEP = m-collector-lib

#m_dan_daq_SVN_DEP = m-collector-lib
#m_dan_daq_SVN_BRA = trunk

#m_dan_daq_SVN_BRA = trunk
m_dan_daq_SVN_DEP = m-dan-common

#m_dan_common_SVN_BRA = trunk
m_dan_common_SVN_DEP = m-codac-unit-api \
					   m-maven-iter-plugin

# m_DAN_sample_SVN_BRA = trunk
m_DAN_sample_SVN_DEP = m-maven-iter-settings \
					   m-maven-iter-plugin







VPATH = .

$(IC_UNITS):
	@ $(MAKE) svn_co NAME=$@ SVN_DIR=$(ICDEV_DIR)

$(DEV_UNITS):
	@ $(MAKE) svn_co NAME=$@ SVN_DIR=$(DEV_DIR)

.PHONY: download-all
download-all: ##@svnpub perform svn checkout on all deps
download-all: $(NAME) ##@svnpub perform svn checkout on all deps
	@ $(foreach x,$(SVN_DEP),$(MAKE) $x;)

all: $(NAME) mvn-compile


list: ##@svnpub list defined ic units
list : _item = $(info | $1: ) $(foreach x,$($1),$(info |  - $x))
list:
	@ \
	$(info ,-----------------------------------------------------------------) \
	$(info | IC units defined ) \
	$(info |) \
	$(call _item,IC_UNITS) \
	$(info |) \
	$(call _item,DEV_UNITS) \
	$(info |) \
	$(info | CURRENT: $(NAME)) \
	$(info |) \
	$(info `-----------------------------------------------------------------) :



## ////////////////////////////////////////////////////////////////////////// ##
## /// MAVEN //////////////////////////////////////////////////////////////// ##
## ////////////////////////////////////////////////////////////////////////// ##


export MVN_ARGS ?=

export MAVEN_HOME   ?= $(abs_top_builddir)/codac/mavenrepo
export MVN_SETTINGS ?= $(abs_top_srcdir)/codac/maven_settings.xml
export M2_HOME      ?= $(MAVEN_HOME)
export M3_HOME      ?= $(MAVEN_HOME)

mvn-compile: ##@mvn perform maven action on unit: $(NAME)
mvn-run: ##@mvn perform maven action on unit: $(NAME)
mvn-stop: ##@mvn perform maven action on unit: $(NAME)


mvn-run:  dan-aw-start dan-api-init
mvn-stop: dan-api-close

$(MAVEN_HOME):
	$(info )
	$(info CREATING NEW MAVENREPO FORM /opt/codac/common/mavenrepo  )
	$(info -------------------------------------------------------  )
	$(info )
	cp -a /opt/codac/common/mavenrepo $@


mvn-%-dep: ##@mvn perform install on all deps only
	@ $(foreach x,$(SVN_DEP),$(MAKE) $(subst -dep,-all,$@) NAME=$x;)

mvn-install-all: USER=root
mvn-%-all: ##@mvn perform install on all deps
	@ $(foreach x,$(SVN_DEP),$(MAKE) $@ NAME=$x &&) \
	  $(MAKE) $(subst -all,,$@)

mvn-%: $(NAME) $(MAVEN_HOME)
	@ $(info )
	  $(info Performing mvn $(subst mvn-,,$@) on unit: $(NAME) )
	  $(info ----------------------------------------------------  )
	  $(info )
	  cd $(NAME); \
	  mvn $(subst mvn-,,$@) -s $(MVN_SETTINGS) $(MVN_ARGS)


mvn-install: USER=root
mvn-install: $(NAME) $(MAVEN_HOME)
	@ $(info )
	  $(info Performing mvn $(subst mvn-,,$@) on unit: $(NAME) as root )
	  $(info --------------------------------------------------------  )
	  $(info )
	@ cd $(NAME); \
	  mvn $(subst mvn-,,$@) -s $(MVN_SETTINGS) $(MVN_ARGS) && \
	  rpm -Uvh --force target/*.rpm ||:


shell:
	@ bash -l



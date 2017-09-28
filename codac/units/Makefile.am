include $(top_srcdir)/Common.mk
include $(srcdir)/../CodacDev.mk

SVN_URL     ?= https://svnpub.iter.org/codac/iter
SVN_BRA     ?= trunk
DEV_DIR     ?= codac/dev/units
ICDEV_DIR   ?= codac/icdev/units


IC_UNITS  = m-DAN-sample
DEV_UNITS =
NAME     ?= $(lastword $(IC_UNITS))

NODOCKERBUILD += all $(IC_UNITS) $(DEV_UNITS) ic-dialog dev-dialog

$(IC_UNITS):
	@ svn co $(SVN_URL)/$(ICDEV_DIR)/$@/$(SVN_BRA) $@; \
	  ln -sr $@ $(srcdir)/$@


ic-dialog: ##@svnpub manually download icdev units
ic-dialog: dialog_svnpub_download.sh
	@ . $< $(SVN_URL) $(DEV_DIR) $(SVN_BRA)



dev-dialog: ##@svnpub manually download devel units
dev-dialog: dialog_svnpub_download.sh
	@ . $< $(SVN_URL) $(DEV_DIR) $(SVN_BRA)


all: $(IC_UNITS) mvn-compile mvn-run


## ////////////////////////////////////////////////////////////////////////// ##
## /// MAVEN //////////////////////////////////////////////////////////////// ##
## ////////////////////////////////////////////////////////////////////////// ##

MVN_TARGETS = compile \
			  run \
			  stop \
			  migrate \
			  clean


export MVN_ARGS ?=


mvn-compile: ##@mvn perform maven action on unit: $(NAME)
mvn-run: ##@mvn perform maven action on unit: $(NAME)
mvn-stop: ##@mvn perform maven action on unit: $(NAME)

mvn-run:  dan-aw-start dan-api-init
# mvn-stop: dan-api-close

mvn-%: $(NAME)
	@ $(info )
	  $(info Performing target: $(subst mvn-,,$@) on unit: $(NAME) )
	  $(info ----------------------------------------------------  )
	  $(info )
	  cd $(NAME); \
	  mvn $(subst mvn-,,$@) ${MVN_ARGS}


## ////////////////////////////////////////////////////////////////////////// ##
## /// DAN API ////////////////////////////////////////////////////////////// ##
## ////////////////////////////////////////////////////////////////////////// ##

CU_SETTINGS_DIR ?= $(NAME)/src/main/controlunitsettings
DAN_APITOOL_BIN ?= /opt/codac/bin/danApiTool

# this Fixes the dan-plot tool for the Qt-X11 window
export QT_X11_NO_MITSHM = 1

CU_PCF_DANCONF   = $(shell ls $(CU_SETTINGS_DIR)/*-PCF-*/dan_config.xml)

dan-api-init:   ##@dan DAN-API initialize unit PCF config
dan-api-reload: ##@dan DAN-API reload unit PCF config
dan-api-init dan-api-reload:
	- $(call _env_set) \
	  \
	  $(foreach x,$(CU_PCF_DANCONF),nohup $(DAN_APITOOL_BIN) api $(subst dan-api-,,$@) $x;)

dan-api-close:  ##@dan DAN-API close all active API
dan-api-status: ##@dan DAN-API return status
dan-api-close dan-api-status:
	@ $(call _env_set) \
	  $(DAN_APITOOL_BIN) api $(subst dan-api-,,$@)



SUBSCRIBERS ?= ALL
dan-st-start: ##@dan STREAMER start ${SUBSCRIBERS}
dan-st-stop: ##@dan STREAMER stop ${SUBSCRIBERS}
dan-st-restart: ##@dan STREAMER restart ${SUBSCRIBERS}
dan-st-%:
	@ $(call _env_set) \
	  $(DAN_APITOOL_BIN) streamer $(subst dan-st-,,$@) $(SUBSCRIBERS)


dan-aw-start:   ##@dan ARCHIVER start
dan-aw-restart: ##@dan ARCHIVER restart
dan-aw-stop:    ##@dan ARCHIVER stop
dan-aw-%: USER=root
dan-aw-%:
	-@ $(call _env_set) \
	  dan_archiver_writer $(subst dan-aw-,,$@)












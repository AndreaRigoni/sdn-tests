## ////////////////////////////////////////////////////////////////////////// ##
## /// DAN API ////////////////////////////////////////////////////////////// ##
## ////////////////////////////////////////////////////////////////////////// ##

CU_SETTINGS_DIR ?= $(srcdir)
DAN_APITOOL_BIN ?= /opt/codac/bin/danApiTool

# this Fixes the dan-plot tool for the Qt-X11 window
export QT_X11_NO_MITSHM = 1

dan-api-init:   ##@dan DAN-API initialize unit PCF config
dan-api-reload: ##@dan DAN-API reload unit PCF config
dan-api-init dan-api-reload:
	- $(call _env_set) \
	  $(foreach x,$(CU_PCF_DANCONF) $($(call _flt,$(NAME))_DANCFG),$(DAN_APITOOL_BIN) api $(subst dan-api-,,$@) $x;)

dan-api-close:  ##@dan DAN-API close all active API
dan-api-status: ##@dan DAN-API return status
dan-api-close dan-api-status:
	@ $(call _env_set) \
	  $(info $(call _flt,$(NAME))_ICPROG = $($(call _flt,$(NAME))_ICPROG))
	  $(DAN_APITOOL_BIN) api $(subst dan-api-,,$@) $($(call _flt,$(NAME))_ICPROG)


SUBSCRIBERS ?= all
dan-st-start: ##@dan STREAMER start ${SUBSCRIBERS}
dan-st-stop: ##@dan STREAMER stop ${SUBSCRIBERS}
dan-st-restart: ##@dan STREAMER restart ${SUBSCRIBERS}
dan-st-%:
	@ $(call _env_set) \
	  $(DAN_APITOOL_BIN) streamer $(subst dan-st-,,$@) $($(call _flt,$(NAME))_ICPROG) $(SUBSCRIBERS)

dan-aw-start:   ##@dan ARCHIVER start
dan-aw-restart: ##@dan ARCHIVER restart
dan-aw-stop:    ##@dan ARCHIVER stop
dan-aw-%: USER=root
dan-aw-%:
	-@ $(call _env_set) \
	  dan_archiver_writer $(subst dan-aw-,,$@)


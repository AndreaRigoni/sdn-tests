## ////////////////////////////////////////////////////////////////////////// ##
## /// CODAC DEV //////////////////////////////////////////////////////////// ##
## ////////////////////////////////////////////////////////////////////////// ##

define _env_set =
 . /etc/profile;
endef

run_%:
	@ $(call _env_set) \
	  ./$(subst run_,,$@) $${ARGS}

SDD_WS ?= $(abs_top_builddir)/codac/codac_ws

$(SDD_WS):
	@ mkdir -p $@

sdd-editor: ##@codac start sdd-editor in current workspace
eclipse:    ##@codac start eclipse in current workspace
css:        ##@codac start CS-Studio in current workspace

css sdd-editor eclipse: $(SDD_WS)
	@ $(call _env_set) \
	  $@ -data $(SDD_WS) -dir .


CODAC_VERSION   ?= 5
CODAC_NAME      ?= codac_v$(CODAC_VERSION)

export DOCKER_ENTRYPOINT = $(abs_top_srcdir)/codac/$(CODAC_NAME)_entry.sh

## ////////////////////////////////////////////////////////////////////////// ##
## /// UNITS DEV //////////////////////////////////////////////////////////// ##
## ////////////////////////////////////////////////////////////////////////// ##


SVN_URL     ?= https://svnpub.iter.org/codac/iter
SVN_BRA     ?= trunk
DEV_DIR     ?= codac/dev/units
ICDEV_DIR   ?= codac/icdev/units


NODOCKERBUILD += $(IC_UNITS) $(DEV_UNITS) ic-dialog dev-dialog


unit_VARIABLES = SVN_BRA SVN_DEP MVN_ARGS

 _flt = $(subst ' ',_,$(subst .,_,$(subst -,_,$1)))
 _var = $(or $($(call _flt,$(NAME))_$1),$($1))

$(foreach x,$(unit_VARIABLES),$(eval override $x=$(call _var,$x)))


svn_%: $(SVN_DEP)
	  $(info ,----------------------------------------------------- )
	  $(info | performing svn on unit: $(NAME))
	  $(info |   unit:   $(NAME))
	  $(info |   branch: $(SVN_BRA))
	  $(info |   deps:   $(SVN_DEP))
	@ svn $(subst svn_,,$@) $(SVN_URL)/$(SVN_DIR)/$(NAME)/$(SVN_BRA) $(NAME); \
	  ln -s $(subst svn_,,$@) $(srcdir)/$(subst svn_,,$@)


ic-dialog: ##@svnpub manually download icdev units
ic-dialog: dialog_svnpub_download.sh
	@ . $< $(SVN_URL) $(DEV_DIR) $(SVN_BRA)


dev-dialog: ##@svnpub manually download devel units
dev-dialog: dialog_svnpub_download.sh
	@ . $< $(SVN_URL) $(DEV_DIR) $(SVN_BRA)

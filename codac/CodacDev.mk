## ////////////////////////////////////////////////////////////////////////// ##
## /// CODAC DEV //////////////////////////////////////////////////////////// ##
## ////////////////////////////////////////////////////////////////////////// ##

define _env_set =
 . /etc/profile;
endef

SDD_WS ?= $(abs_top_builddir)/codac/codac_ws

$(SDD_WS):
	@ mkdir -p $@

sdd-editor: ##@codac start sdd-editor in current workspace
eclipse: ##@codac start eclipse in current workspace
sdd-editor eclipse: $(SDD_WS)
	@ $(call _env_set) \
	  $@ -data $(SDD_WS) -dir .





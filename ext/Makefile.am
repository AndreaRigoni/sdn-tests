include $(top_srcdir)/Common.mk
# include $(top_srcdir)/conf/kscripts/ext_ccStickers.mk
# include $(top_srcdir)/conf/kscripts/ext_libjit.mk
# include $(top_srcdir)/conf/kscripts/ext_llvm.mk
# include $(top_srcdir)/conf/kscripts/ext_nodejs_node.mk

# for qwt
include $(top_srcdir)/conf/kscripts/qtdesktop.mk



define _ext_bootstrap
$(eval
$1/configure: $1
	@ cd $1; ./bootstrap
)
endef

define _ext_autogen_sh
$(eval
$1/configure: $1
	@ cd $1; ./autogen.sh
)
endef

define _ext_configure
$(eval
$1/config.log: $1 $1/configure
	@ cd $1; ./configure $($(subst -,_,$(subst ' ',_,$(subst .,_,$1)))_CONFIGURE_ARGS)
$1/Makefile: $1/config.log
	@ $(MAKE) $(AM_MAKEFLAGS) -C $1 Makefile
)
endef


ext-: ##@ext enter ext module with $TARGET
ext-%: %/Makefile
	@ $(MAKE) $(AM_MAKEFLAGS) -C $(subst ext-,,$@) $(TARGET)

all: TARGET = all
all: $(EXT_MODULES)

clean: TARGET = clean
clean: ext-libjit


ext-list: ##@ext list defined ext modules
ext-list : _item = $(info | $1: ) $(foreach x,$($1),$(info |  - $x))
ext-list:
	@ \
	$(info ,-----------------------------------------------------------------) \
	$(info | ext modules defined ) \
	$(info |) \
	$(call _item,EXT_MODULES) \
	$(info |) \
	$(info |) \
	$(info `-----------------------------------------------------------------) :




# /////////////////////////////////////////////////////////////////////////// //
# //  EXT MODULES  ////////////////////////////////////////////////////////// //
# /////////////////////////////////////////////////////////////////////////// //

NODOCKERBUILD = download qwt

DOWNLOADS  = $(EXT_MODULES)

ccStickers: ##@ext download ccStickers sources
ccStickers_URL = $(CCSTICKERS_URL)
$(call _ext_bootstrap,ccStickers)
$(call _ext_configure,ccStickers)

libjit: ##@ext download libjit sources
libjit_URL = $(LIBJIT_URL)
$(call _ext_bootstrap,libjit)
$(call _ext_configure,libjit)

llvm:   ##@ext download llvm sources
llvm_URL   = $(LLVM_URL)

fio:    ##@ext download fio sources
fio_URL = $(FIO_URL)
$(call _ext_configure,fio)


ext-nodejs-node: AM_MAKEFLAGS += -j$(MAKE_PROCESS)
nodejs-node:
nodejs_node_URL = $(NODEJS_NODE_URL)
nodejs_node_CONFIGURE_ARGS = --shared
$(call _ext_configure,nodejs-node)

qwt: ##@ext download qwt
DOWNLOADS += qwt
qwt_URL = git://github.com/AndreaRigoni/qwt.git


hdf5: ##@ext download hdf5
DOWNLOADS += hdf5
hdf5_URL = git://github.com/live-clones/hdf5.git
$(call _ext_autogen,hdf5)
$(call _ext_configure,hdf5)




#PRO = qwt/qwt/qwt.pro
#export QT_X11_NO_MITSHM = 1
#ext-qwt: $(TARGET)
#ext-qwt: qwt/qwt/Makefile
#	@ $(info help)




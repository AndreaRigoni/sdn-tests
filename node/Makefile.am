include $(top_srcdir)/Common.mk

NODOCKERBUILD += %

RDIR      ?= $(srcdir)
RDIR_NAME ?= $(RDIR)/$(NAME)


st-%: NAME =
st-%: RDIR = $(srcdir)

st_testserver_DEPS = ffi

st-testserver: ##@standalone testserver.js
st-testserver: testserver.js
	node $<

st-app: ##@standalone app.js
st-app: app.js
	node $<



## REACT PROJECTS ##

NAME ?= r1
r1_DEPS = rc-menu react-grid-layout create-react-class webpack node-json-rpc
# r1_DEPS = rc-menu react-grid-layout create-react-class webpack \
#           react-addons react-box-layout



$(RDIR):
	@ $(MKDIR_P) $@

$(RDIR)/react/$(NAME): $(RDIR)
	@ create-react-app $@


rc-%: RDIR = $(srcdir)/react

rc-create: ##@react create react app
rc-create:
	@ $(MAKE) $(RDIR_NAME)

rc-start: ##@react start app $NAME
rc-start: deps npm-start

rc-stop: ##@react stop app $NAME
rc-stop: npm-start



## NPM COMMANDS ##

npm-start: ##@npm start app
npm-stop:  ##@npm stop app
npm-%:
	@ cd $(RDIR_NAME); npm $(subst npm-,,$@) $(NPM_ARGS)


DEPS = $($(subst -,_,$(NAME))_DEPS)
$(addprefix $(RDIR_NAME)/node_modules/,$(DEPS)):
	@ $(info install: $@) \
	  cd $(RDIR_NAME); npm install $(notdir $@)

deps:
	@ $(MAKE) $(AM_MAKEFLAGS) $(addprefix $(RDIR_NAME)/node_modules/,$(DEPS)) \
	  RDIR_NAME=$(RDIR_NAME)

clean:
	@ $(info not implemented yet)

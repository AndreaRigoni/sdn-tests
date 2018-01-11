include $(top_srcdir)/Common.mk

.PHONY: testserver
testserver: testserver.js
	node $<

.PHONY: app
app: app.js
	node $<


NAME ?= t1
RDIR ?= $(srcdir)/react
RDIR_NAME := $(RDIR)/$(NAME)


$(RDIR):
	@ $(MKDIR_P) $@

$(RDIR_NAME): $(RDIR)
	@ create-react-app $@

react-create: ##@react create react app
react-create:
	@ $(MAKE) $(RDIR_NAME)

react-start: ##@react start app
react-stop: ##@react stop app
react-%: $(RDIR_NAME)
	@ cd $(RDIR_NAME); npm $(subst react-,,$@)

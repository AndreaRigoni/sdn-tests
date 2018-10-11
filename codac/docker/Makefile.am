include $(top_srcdir)/Common.mk
include $(top_srcdir)/conf/kscripts/iter_codac.mk

.ONESHELL:
## ////////////////////////////////////////////////////////////////////////// ##
## /// DOCKER /////////////////////////////////////////////////////////////// ##
## ////////////////////////////////////////////////////////////////////////// ##


# Disable any target be executed inside docker image here
NODOCKERBUILD += %

if ENABLE_LIBGUEST_DKIMG
$(CODAC_NAME).tar: $(CODAC_NAME).img
	@
	echo "Extracting tar from qcow2 image: this can take a while ...."; \
	docker pull $(CODAC_LIBGUEST_DKIMG); \
	docker run --rm -v $(shell pwd $(builddir)):/mnt $(CODAC_LIBGUEST_DKIMG) \
	 sh -c "cd mnt; virt-tar-out -a $(CODAC_NAME).img / $@";
else
$(CODAC_NAME).tar:
	$(error please enable libguest via docker)
endif

#.PHONY: $(CODAC_NAME)_entry.sh
$(CODAC_NAME)_entry.sh:
	@ [ -f $@ ] || cp $(srcdir)/$@ .

#.PHONY: .dockerignore
.dockerignore:
	@ cat > $@ << EOF
	*
	!$(CODAC_NAME).tar
	!$(DOCKERFILE)
	!$(CODAC_NAME)_entry.sh
	EOF

DOCKERFILE = Dockerfile_v$(CODAC_VERSION)

# .PHONY: Dockerfile
$(DOCKERFILE): | $(CODAC_NAME)_entry.sh .dockerignore
	@ cat > $@ << EOF
	FROM scratch
	ADD $(CODAC_NAME).tar /
	COPY $(CODAC_NAME)_entry.sh /root
# ENTRYPOINT /root/$(CODAC_NAME)_entry.sh
	EOF

docker-image: ##@codac build docker from valid tar file
docker-image: $(DOCKERFILE) $(CODAC_NAME).tar
	docker build -t $(CODAC_NAME):$(VERSION) -f $< .


## ////////////////////////////////////////////////////////////////////////// ##
## /// CLEAN //////////////////////////////////////////////////////////////// ##
## ////////////////////////////////////////////////////////////////////////// ##

MOSTLYCLEANFILES = Dockerfile $(DOWNLOADS)
clean-all: ##@codac remove everything .. tar img and the docker install instance
clean-all:
	docker rmi $(CODAC_LIBGUEST_DKIMG);
	rm -rfi $(CODAC_NAME).tar $(CODAC_NAME).img

print:
	echo $(CODAC_NAME)


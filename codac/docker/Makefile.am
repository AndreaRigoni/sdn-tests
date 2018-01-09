include $(top_srcdir)/Common.mk
include $(top_srcdir)/conf/kscripts/iter_codac.mk

.ONESHELL:
## ////////////////////////////////////////////////////////////////////////// ##
## /// DOCKER /////////////////////////////////////////////////////////////// ##
## ////////////////////////////////////////////////////////////////////////// ##


# Disable any target be executed inside docker image here
NODOCKERBUILD += %

$(NAME).tar: $(NAME).img
	@
	echo "Extracting tar from qcow2 image: this can take a while ...."; \
	docker pull $(LIBGUEST_DKIMG); \
	docker run --rm -v $(shell pwd $(builddir)):/mnt $(LIBGUEST_DKIMG) \
	 sh -c "cd mnt; virt-tar-out -a $(NAME).img / $@";

.PHONY: $(NAME)_entry.sh
$(NAME)_entry.sh:
	@ [ -f $@ ] || cp $(srcdir)/$@ .

.PHONY: .dockerignore
.dockerignore:
	@ cat > $@ << EOF
	*
	!$(NAME).tar
	!Dockerfile
	!$(NAME)_entry.sh
	EOF

.PHONY: Dockerfile
Dockerfile: $(NAME)_entry.sh .dockerignore
	@ cat > $@ << EOF
	FROM scratch
	ADD $(NAME).tar /
	COPY $(NAME)_entry.sh /root
	ENTRYPOINT /root/$(NAME)_entry.sh
	EOF

docker-image: ##@codac build docker from valid tar file
docker-image: Dockerfile $(NAME).tar
	docker build -t $(NAME):$(VERSION) -f $< .


## ////////////////////////////////////////////////////////////////////////// ##
## /// CLEAN //////////////////////////////////////////////////////////////// ##
## ////////////////////////////////////////////////////////////////////////// ##

MOSTLYCLEANFILES = Dockerfile $(DOWNLOADS)
clean-all:
	docker rmi $(LIBGUEST_DKIMG);
	rm -rfi $(NAME).tar $(NAME).img




include $(top_srcdir)/Common.mk
include $(srcdir)/CodacDev.mk

SUBDIRS = units

.ONESHELL:



## ////////////////////////////////////////////////////////////////////////// ##
## /// CODAC VMs //////////////////////////////////////////////////////////// ##
## ////////////////////////////////////////////////////////////////////////// ##


NODOCKERBUILD += boot install-iso

QEMU            ?= qemu-system-x86_64
QEMU_IMG_SIZE   ?= 40G
QEMU_IMG_FORMAT ?= qcow2

NAME ?= codac_v5

DOWNLOADS = \
			codac_v4.iso \
			codac_v4.md5 \
			codac_v5.iso \
			codac_v5.md5

codac_v4_iso_URL ?= http://static.iter.org/codac/cs-iso/Iter-CCS-v4.iso
codac_v4_md5_URL ?= http://static.iter.org/codac/cs-iso/Iter-CCS-v4.md5
codac_v5_iso_URL ?= http://static.iter.org/codac/cs-iso/Iter-CCS-v5.iso
codac_v5_md5_URL ?= http://static.iter.org/codac/cs-iso/Iter-CCS-v5.md5


$(NAME).img:
	@ qemu-img create -f $(QEMU_IMG_FORMAT) $@ $(QEMU_IMG_SIZE)


install-iso: ##@codac boot iso to install codac
install-iso: $(NAME).img $(NAME).iso
	@ $(QEMU) -m 4G -enable-kvm -cpu host -smp 4 -hda $(NAME).img -cdrom $(NAME).iso -boot d


boot: ##@codac boot into codac image
boot: $(NAME).img
	@ $(QEMU) -m 4G -enable-kvm -cpu host -smp 4 -hda $(NAME).img





## ////////////////////////////////////////////////////////////////////////// ##
## /// DOCKER /////////////////////////////////////////////////////////////// ##
## ////////////////////////////////////////////////////////////////////////// ##

PRIV_USER = root
LIBGUEST_DKIMG = flemzord/libguestfs-glance

NODOCKERBUILD += Dockerfile docker-image clean-all $(NAME).tar .dockerimage $(NAME)_entry.sh

export DOCKER_ENTRYPOINT = /root/$(NAME)_entry.sh

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




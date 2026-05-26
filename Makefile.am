## Makefile.am
##
## Top level Makefile.am for gpi_send
##
## Use automake to process this into a Makefile.in
##
##   (C) Copyright 2011-2024 Fred Gleason <fredg@paravelsystems.com>
##       All Rights Reserved.
##

SUBDIRS = helpers\
          debian\
          docs\
          src\
          systemd

ACLOCAL_AMFLAGS = -I m4

maintainer-clean-local:	
	rm -rf m4

deb:	dist
	./build_debs.sh

rpm:	dist
	cp gpi_send-$(VERSION).tar.gz $(HOME)/rpmbuild/SOURCES/
	$(RPMBUILD) -ba --target $(ARCH)-$(VENDOR)-linux gpi_send.spec
	mv $(HOME)/rpmbuild/RPMS/$(ARCH)/gpi_send*.rpm $(top_srcdir)/
	mv $(HOME)/rpmbuild/SRPMS/gpi_send*.src.rpm $(top_srcdir)/
	rm $(HOME)/rpmbuild/SOURCES/gpi_send-$(VERSION).tar.gz
	rm -rf $(HOME)/rpmbuild/BUILD/gpi_send-$(VERSION)

EXTRA_DIST = autogen.sh\
             CODINGSTYLE\
             get_target.sh\
             get_distro.pl\
             PACKAGE_VERSION\
             gpi_send.spec.in

CLEANFILES = *~\
             *.buildinfo\
             *.changes\
             *.deb\
             *.ddeb\
             *.dsc\
             *.mo\
             *.rpm\
             *.tar.gz\
             *.tar.xz\
             *.tgz\
             *.zip

DISTCLEANFILES = *.spec\
                 build_debs.sh\
                 config.guess\
                 config.sub\
                 gpi_send.spec

MAINTAINERCLEANFILES = *~\
                       *.spec\
                       *.tar.gz\
                       aclocal.m4\
                       compile\
                       configure\
                       config.guess\
                       config.status\
                       config.sub\
                       depcomp\
                       install-sh\
                       libtool\
                       ltmain.sh\
                       Makefile.in\
                       missing\
                       mkinstalldirs

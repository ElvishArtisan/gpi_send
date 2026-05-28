## Makefile.am
##
## (C) Copyright 2002-2026 Fred Gleason <fredg@paravelsystems.com>
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License version 2 as
##   published by the Free Software Foundation.
##
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
##
##   You should have received a copy of the GNU General Public
##   License along with this program; if not, write to the Free Software
##   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##
##
## Use automake to process this into a Makefile.in

SUBDIRS = helpers\
          conf\
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
	cp gpisend-$(VERSION).tar.gz $(HOME)/rpmbuild/SOURCES/
	$(RPMBUILD) -ba --target $(ARCH)-$(VENDOR)-linux gpisend.spec
	mv $(HOME)/rpmbuild/RPMS/$(ARCH)/gpisend*.rpm $(top_srcdir)/
	mv $(HOME)/rpmbuild/SRPMS/gpisend*.src.rpm $(top_srcdir)/
	rm $(HOME)/rpmbuild/SOURCES/gpisend-$(VERSION).tar.gz
	rm -rf $(HOME)/rpmbuild/BUILD/gpisend-$(VERSION)

EXTRA_DIST = autogen.sh\
             get_target.sh\
             get_distro.pl\
             PACKAGE_VERSION

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
                 gpisend.spec

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

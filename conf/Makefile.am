## Makefile.am
##
## (C) Copyright 2026 Fred Gleason <fredg@paravelsystems.com>
##
##   All Rights Reserved.
##
##
## Use automake to process this into a Makefile.in

install-exec-local:	
	mkdir -p $(DESTDIR)@prefix@/share/gpisend
	cp gpisend-sample.conf $(DESTDIR)@prefix@/share/gpisend/

uninstall-local:	
	rm -f $(DESTDIR)@prefix@/share/gpisend/gpisend-sample.conf

EXTRA_DIST = gpisend-sample.conf

CLEANFILES = *~
MAINTAINERCLEANFILES = *~\
                       aclocal.m4\
                       configure\
                       Makefile.in

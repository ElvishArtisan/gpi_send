dnl configure.ac
dnl
dnl Autoconf configuration for gpi_send.
dnl
dnl Use autoconf to process this into a configure script
dnl
dnl   (C) Copyright 2011-206 Fred Gleason <fredg@paravelsystems.com>
dnl       All Rights Reserved.
dnl

AC_INIT([gpi_send],m4_esyscmd(cat PACKAGE_VERSION))
AM_INIT_AUTOMAKE
AC_SUBST(RPM_RELEASE,1)
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])
AC_CONFIG_MACRO_DIR([m4])

#
# Basic Compiler Checks
#
AC_PROG_CXX
AC_LANG(C++)

#
# Determine the target architecture
#
AR_GCC_TARGET()
AC_SUBST(VENDOR,$ar_gcc_distro)
AC_SUBST(ARCH,$ar_gcc_arch)

#
# Determine number of available CPU cores for building packages
#
AX_COUNT_CPUS([],[])
if test $CPU_COUNT -lt 2 ; then
   AC_SUBST(CPUS_AVAIL,1)
else
   if test $CPU_COUNT -gt 16 ; then
      AC_SUBST(CPUS_AVAIL,16)
   else
      AC_SUBST(CPUS_AVAIL,$(($CPU_COUNT-1)))
   fi
fi

#
# Determine Distro
#
AR_GET_DISTRO()
AC_SUBST(DISTRO,$ar_gcc_distro)
AC_MSG_NOTICE($ar_distro_id)
DISTRO_NAME="|                         Distribution Name ... Unknown   |"
case "$ar_distro_id" in
     debian)
       AC_MSG_NOTICE([Distro is Debian-ish])
       DISTRO_FAMILY="|                       Distribution Family ... Debian    |"
       DISTRO_IS_DEBIANISH=yes
       DISTRO_NAME="|                         Distribution Name ... Debian    |"
       AC_SUBST(APACHE_PKG,"apache2")
       AC_SUBST(APACHE_CONFIG_DIR,"/etc/apache2/conf.d")
       AC_SUBST(USERMODE_PKG,"")
       AC_SUBST(DOC_PATH,"/usr/share/doc/gpi_send")
       AC_SUBST(HAVE_USERMODE,"no")
       AC_SUBST(GPI_SEND_DESKTOP_FILE,"rivendell-rdalsaconfig-sudo.desktop")
     ;;

     raspbian)
       AC_MSG_NOTICE([Distro is Debian-ish])
       DISTRO_FAMILY="|                       Distribution Family ... Debian    |"
       DISTRO_IS_DEBIANISH=yes
       DISTRO_NAME="|                  Distribution Name ... Raspberry Pi OS  |"
       AC_SUBST(APACHE_PKG,"apache2")
       AC_SUBST(APACHE_CONFIG_DIR,"/etc/apache2/conf.d")
       AC_SUBST(USERMODE_PKG,"")
       AC_SUBST(DOC_PATH,"/usr/share/doc/gpi_send")
       AC_SUBST(HAVE_USERMODE,"no")
       AC_SUBST(GPI_SEND_DESKTOP_FILE,"rivendell-rdalsaconfig-sudo.desktop")
     ;;

     linuxmint)
       AC_MSG_NOTICE([Distro is Debian-ish])
       DISTRO_FAMILY="|                       Distribution Family ... Debian    |"
       DISTRO_IS_DEBIANISH=yes
       DISTRO_NAME="|                         Distribution Name ... Linux Mint|"
       AC_SUBST(APACHE_PKG,"apache2")
       AC_SUBST(APACHE_CONFIG_DIR,"/etc/apache2/conf.d")
       AC_SUBST(USERMODE_PKG,"")
       AC_SUBST(DOC_PATH,"/usr/share/doc/gpi_send")
       AC_SUBST(HAVE_USERMODE,"no")
       AC_SUBST(GPI_SEND_DESKTOP_FILE,"rivendell-rdalsaconfig-sudo.desktop")
     ;;

     ubuntu)
       AC_MSG_NOTICE([Distro is Debian-ish])
       DISTRO_FAMILY="|                       Distribution Family ... Debian    |"
       DISTRO_IS_DEBIANISH=yes
       DISTRO_NAME="|                         Distribution Name ... Ubuntu    |"
       AC_SUBST(APACHE_PKG,"apache2")
       AC_SUBST(APACHE_CONFIG_DIR,"/etc/apache2/conf.d")
       AC_SUBST(USERMODE_PKG,"")
       AC_SUBST(DOC_PATH,"/usr/share/doc/gpi_send")
       AC_SUBST(HAVE_USERMODE,"no")
       AC_SUBST(GPI_SEND_DESKTOP_FILE,"rivendell-rdalsaconfig-sudo.desktop")
     ;;

     centos|fedora|rhel|rocky)
       AC_MSG_NOTICE([Distro is RedHat-ish])
       DISTRO_FAMILY="|                       Distribution Family ... RedHat    |"
       if test $ar_distro_id = "centos" ; then
         DISTRO_NAME="|                         Distribution Name ... CentOS    |"
       fi
       if test $ar_distro_id = "fedora" ; then
         DISTRO_NAME="|                         Distribution Name ... Fedora    |"
       fi
       if test $ar_distro_id = "rhel" ; then
         DISTRO_NAME="|                         Distribution Name ... RHEL      |"
       fi
       if test $ar_distro_id = "rocky" ; then
         DISTRO_NAME="|                         Distribution Name ... Rocky     |"
       fi
       AC_SUBST(APACHE_PKG,"httpd")
       AC_SUBST(APACHE_CONFIG_DIR,"/etc/httpd/conf.d")
       AC_SUBST(USERMODE_PKG,"usermode usermode-gtk")
       AC_SUBST(DOC_PATH,"/usr/share/doc/gpi_send-$PACKAGE_VERSION")
       AC_SUBST(HAVE_USERMODE,"yes")
       AC_SUBST(GPI_SEND_DESKTOP_FILE,"rivendell-rdalsaconfig-root.desktop")
       ;;

     *)
       AC_MSG_NOTICE([Distro is unknown])
       DISTRO_FAMILY="|                       Distribution Family ... Unknown   |"
       AC_SUBST(APACHE_PKG,"apache2")
       AC_SUBST(APACHE_CONFIG_DIR,"/etc/apache2/conf.d")
       AC_SUBST(USERMODE_PKG,"")
       AC_SUBST(DOC_PATH,"/usr/share/doc/gpi_send")
       AC_SUBST(HAVE_USERMODE,"no")
       AC_SUBST(GPI_SEND_DESKTOP_FILE,"rivendell-rdalsaconfig.desktop")
     ;;
esac

#
# Check for Qt5 (Mandatory)
#
PKG_CHECK_MODULES(QT5_CLI,Qt5Core Qt5Network,[],[AC_MSG_ERROR([*** Qt5 not found ***])])
AC_CHECK_PROG(MOC_NAME,moc-qt5,[moc-qt5],[moc])
AC_SUBST(QT_MOC,$MOC_NAME)

#
# Check for libgpiod (Mandatory)
#
PKG_CHECK_MODULES(LIBGPIOD,libgpiod,[],[AC_MSG_ERROR([*** LIBGPIOD not found ***])])

#
# Check for systemd
#
AC_CHECK_PROG(SYSTEMD,[systemctl],[yes],[])
if test $SYSTEMD ; then
    USING_SYSTEMD=yes
    AC_DEFINE(HAVE_SYSTEMD)
fi
AM_CONDITIONAL([SYSTEMD_AM], [test "$USING_SYSTEMD" = yes])

#
# Generated File Warnings
#
AC_SUBST_FILE(GENERATED_C_FILE_WARNING)
GENERATED_C_FILE_WARNING=$srcdir/helpers/c_file_warning

AC_SUBST_FILE(GENERATED_SCRIPT_FILE_WARNING)
GENERATED_SCRIPT_FILE_WARNING=$srcdir/helpers/script_file_warning

AC_SUBST_FILE(GENERATED_XML_FILE_WARNING)
GENERATED_XML_FILE_WARNING=$srcdir/helpers/xml_file_warning

#
# Configure RPM Build
#
AC_CHECK_PROG(RPMBUILD_FOUND,rpmbuild,[yes],[])
if test -z $RPMBUILD_FOUND ; then
  AC_SUBST(RPMBUILD,rpm)
else
  AC_SUBST(RPMBUILD,rpmbuild)
fi
if test -d /usr/src/redhat ; then
  AC_SUBST(RPM_ROOT,/usr/src/redhat)
  AC_SUBST(VENDOR,redhat)
else
  AC_SUBST(RPM_ROOT,/usr/src/packages)
  AC_SUBST(VENDOR,suse)
fi 
AC_SUBST(RPM_DATESTAMP,`date +%a\ %b\ %d\ %Y`)

AC_CONFIG_FILES([ \
    conf/Makefile \
    debian/Makefile \
    debian/source/Makefile \
    helpers/Makefile \
    docs/Makefile \
    src/gpi_send/Makefile \
    src/gpioscan/Makefile \
    src/Makefile \
    systemd/Makefile \
    systemd/gpi_send.service \
    build_debs.sh \
    Makefile ])
AC_OUTPUT()
chmod 755 build_debs.sh

#
# Configuration Results
#
AC_MSG_NOTICE()
AC_MSG_NOTICE(Now enter 'make' to build the software.)
AC_MSG_NOTICE()

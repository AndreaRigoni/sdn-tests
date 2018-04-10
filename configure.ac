## ////////////////////////////////////////////////////////////////////////// //
##
## This file is part of the dan-tests project.
## Copyright 2018 Andrea Rigoni Garola <andrea.rigoni@igi.cnr.it>.
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.
##
## ////////////////////////////////////////////////////////////////////////// //



dnl Process this file with autoconf to produce a configure script.

dnl AUTOCONF ///////////////////////////////////////////////////////////////////
AC_PREREQ([2.69])
AC_INIT([autoconf-bootstrap], [0.1], [andrea.rgn@gmail.com])
AC_CONFIG_MACRO_DIRS([conf/m4])
AC_CONFIG_AUX_DIR([conf])
dnl AC_CONFIG_SRCDIR([...])
AC_CANONICAL_TARGET

dnl AUTOMAKE ///////////////////////////////////////////////////////////////////
AM_INIT_AUTOMAKE([foreign subdir-objects])


dnl update submodules at first ... (must be cleaned out)
AS_IF([${srcdir}/conf/update_submodules.sh],
      [AS_ECHO(["Submodules succesfully updated"])],
      [AS_ECHO(["Error updating submodules"])])


dnl AX_KCONFIG /////////////////////////////////////////////////////////////////
dnl see: m4/ax_enable_kconfig.m4
AS_VAR_SET([srcdir],[${srcdir}])
AS_VAR_SET([builddir],[$(pwd)])
AX_KCONFIG([conf/kconfig])

dnl AX_TARGET_SELFHELP /////////////////////////////////////////////////////////
AX_TARGET_SELFHELP

# //////////////////////////////////////////////////////////////////////////// #
# //  DOCKER BUILD /////////////////////////////////////////////////////////// #
# //////////////////////////////////////////////////////////////////////////// #

AS_VAR_IF([CONFIG_ENABLE_DOCKER_BUILD],[y],[
           AS_VAR_SET_IF([CONFIG_DOCKER_CONTAINER], 
              [AS_VAR_SET([DOCKER_CONTAINER],[${CONFIG_DOCKER_CONTAINER}])] )
           AS_VAR_SET_IF([CONFIG_DOCKER_IMAGE], 
              [AS_VAR_SET([DOCKER_IMAGE],[${CONFIG_DOCKER_IMAGE}])] )
           AS_VAR_SET_IF([CONFIG_DOCKER_URL], 
              [AS_VAR_SET([DOCKER_URL],[${CONFIG_DOCKER_URL}])])
          ])

AX_KCONFIG_VAR_WITH([DOCKER_SHARES],["set with DOCKER_SHARES"])
AC_SUBST([DOCKER_SHARES])
AX_KCONFIG_VAR_WITH([DOCKER_NETWORKS],["set with DOCKER_NETWORKS"])
AC_SUBST([DOCKER_NETWORKS])
AX_KCONFIG_VAR_WITH([DOCKER_ENTRYPOINT],["set with DOCKER_ENTRYPOINT"])
AC_SUBST([DOCKER_ENTRYPOINT])
AX_KCONFIG_VAR_WITH([DOCKER_PS1],["set with DOCKER_PS1"])
AC_SUBST([DOCKER_PS1])

AX_DOCKER_BUILD([bootstrap])


AC_CONFIG_HEADERS([config.h])

dnl RELEASE
AX_IS_RELEASE([always])
AX_CHECK_ENABLE_DEBUG([no],,,,[-g -O2])

# Checks for programs.
AC_PROG_AWK
AC_PROG_MKDIR_P
AC_PROG_LN_S
AC_PROG_SED
AC_PROG_CC
AC_PROG_CXX

AC_CHECK_PROGS([CURL],[curl])
AC_CHECK_PROGS([TAR],[gtar tar])
AC_CHECK_PROGS([UNIQ],[uniq])
AC_CHECK_PROGS([WC],[wc])

AC_PROG_LEX
AC_PROG_YACC


# needed for sh_envsubst
AC_CHECK_PROGS([PERL],[perl])

## LIBTOOL
LT_INIT()

## /////////////////////////////////////////////////////////////////////////////
## //  KSCRIPTS  ///////////////////////////////////////////////////////////////
## /////////////////////////////////////////////////////////////////////////////

## BUILD_COMMON ##
m4_sinclude(conf/kscripts/build_common.ac)

## TOOLCHAIN ##
m4_sinclude(conf/kscripts/toolchain.ac)

## CODAC ##
m4_sinclude(conf/kscripts/iter_codac.ac)

## QT ##
m4_sinclude(conf/kscripts/qtdesktop.ac)

## NODEJS ##
m4_sinclude(conf/kscripts/nodejs.ac)

## DOXYGEN ##
m4_sinclude(conf/autoconf-doxygen/doxygen.ac)


## EXT_CCSTICKERS ##
m4_sinclude(conf/kscripts/ext_ccStickers.ac)

## EXT_FIO ##
m4_sinclude(conf/kscripts/ext_jsonrpcc.ac)

## EXT_FIO ##
m4_sinclude(conf/kscripts/ext_fio.ac)


## EXT ##
AX_KCONFIG_MODULES([EXT],
		  [ccStickers],[ccStickers external sources],
		  [jsonrpcc],[jsonrpcc external sources],
		  [fio],[fio external sources]
		  )
AC_SUBST([EXT_MODULES])



## // GIT IGNORE // ##
AX_GITIGNORE_ADD_PATH([${builddir}])
AX_GITIGNORE_SUBST

## SUBDIRS configure.ac ##
AX_CONFIG_SUBDIRS(
		codac/reports
		)

AC_CONFIG_FILES( Makefile
				 codac/Makefile
				 codac/units/Makefile
				 codac/tmp/Makefile
				 codac/tmp/Qt/Makefile
				 codac/tmp/Qwt_cpuplot/Makefile
				 codac/tmp/Netlink/Makefile
				 codac/docker/Makefile
				 ext/Makefile
				 node/Makefile
				 .gitignore )
AC_OUTPUT


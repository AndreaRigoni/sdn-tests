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

m4_sinclude(conf/kscripts/docker.ac)
AX_DOCKER_BUILD

# //////////////////////////////////////////////////////////////////////////// #

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

## EXT_LIBJIT ##
m4_sinclude(conf/kscripts/ext_libjit.ac)

## EXT_LLVM ##
m4_sinclude(conf/kscripts/ext_llvm.ac)

## EXT_CCSTICKERS ##
m4_sinclude(conf/kscripts/ext_ccStickers.ac)

## EXT_JSONRPCC ##
m4_sinclude(conf/kscripts/ext_jsonrpcc.ac)

## EXT_FIO ##
m4_sinclude(conf/kscripts/ext_fio.ac)


## EXT_MDSPLUS ##
m4_sinclude(conf/kscripts/ext_mdsplus.ac)

## EXT ##
AX_KCONFIG_MODULES([EXT],
		  [libjit],[libjit external sources],
		  [llvm],[llvm external sources],
		  [ccStickers],[ccStickers external sources],
		  [jsonrpcc],[jsonrpcc external sources],
		  [fio],[fio external sources],
                  [mdsplus],[mdsplus external sources]
		  )
AC_SUBST([EXT_MODULES])


AS_VAR_SET([LDFLAGS],[-lrt])
AC_LANG(C)
AC_RUN_IFELSE(
 [AC_LANG_PROGRAM([
 #include <sys/types.h>
 #include <sys/mman.h>
 #include <sys/stat.h>
 #include <fcntl.h>
 #include <unistd.h>
  ], [dnl
	int32_t fd=-1;
	fd=shm_open("test", O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
	if (fd < 0) { return 1; }
	shm_unlink("test");
	return 0;
   ])],
  [AC_MSG_NOTICE(["SHM open succeded"])
   AS_VAR_SET([ENABLE_SHM],[yes])],
  [AC_MSG_FAILURE(["SHM open failed"])
   AS_VAR_SET([ENABLE_SHM],[no])])




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


### autoconf configure.ac && autoreconf -if

AC_INIT(DESCRIPTION)

R_SCMD="${R_HOME}/bin/Rscript -e"


# SEXPtools
#R_SEXPTOOLS=`${R_SCMD} "source('./R/get_lib.r');get.lib('R_SEXPTOOLS','"${R_ARCH}"')"`
#SEXPTOOLS_LDFLAGS=${R_SEXPTOOLS}
R_SEXPTOOLS=`${R_SCMD} "SEXPtools:::ldflags()"`
SEXPTOOLS_LDFLAGS=${R_SEXPTOOLS}
SEXPTOOLS_CPPFLAGS=`${R_SCMD} "SEXPtools:::cppflags()"`


# pbdMPI
SPMD_CPPFLAGS=`${R_SCMD} "source('./R/get_conf.r');get.conf('PKG_CPPFLAGS','"${R_ARCH}"')"`
SPMD_LDFLAGS=`${R_SCMD} "source('./R/get_conf.r');get.conf('PKG_LIBS','"${R_ARCH}"')"`


# PETSc
PETSC_DIR=/usr/lib/petsc
PETSC_ARCH=""


# Report
echo " "
echo "***************** Results of pbdSBASE package configure *****************"
echo " "
echo ">> SEXPTOOLS_CPPFLAGS = ${SEXPTOOLS_CPPFLAGS}"
echo ">> SEXPTOOLS_LDFLAGS = ${SEXPTOOLS_LDFLAGS}"
echo ">> SPMD_CPPFLAGS = ${SPMD_CPPFLAGS}"
echo ">> SPMD_LDFLAGS = ${SPMD_LDFLAGS}"
echo ">> PETSC_DIR = ${PETSC_DIR}"
echo ">> PETSC_ARCH = ${PETSC_ARCH}"
echo " "
echo "*************************************************************************"
echo " "


# Substitute templates
AC_SUBST(SEXPTOOLS_CPPFLAGS)
AC_SUBST(SEXPTOOLS_LDFLAGS)
AC_SUBST(SPMD_CPPFLAGS)
AC_SUBST(SPMD_LDFLAGS)
AC_SUBST(PETSC_DIR)
AC_SUBST(PETSC_ARCH)

AC_OUTPUT(src/Makevars)

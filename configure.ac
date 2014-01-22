### autoconf configure.ac && autoreconf -if

AC_INIT(DESCRIPTION)

R_SCMD="${R_HOME}/bin/Rscript -e"


# SEXPtools
R_SEXPTOOLS=`${R_SCMD} "source('./R/get_lib.r');get.lib('R_SEXPTOOLS','"${R_ARCH}"')"`
SEXPTOOLS_LDFLAGS=${R_SEXPTOOLS}

# pbdMPI
SPMD_CPPFLAGS=`${R_SCMD} "source('./R/get_conf.r');get.conf('PKG_CPPFLAGS','"${R_ARCH}"')"`
SPMD_LDFLAGS=`${R_SCMD} "source('./R/get_conf.r');get.conf('PKG_LIBS','"${R_ARCH}"')"`


# Report
echo " "
echo "***************** Results of pbdSBASE package configure *****************"
echo " "
echo ">> R_SEXPTOOLS = ${R_SEXPTOOLS}"
echo ">> SEXPTOOLS_LDFLAGS = ${SEXPTOOLS_LDFLAGS}"
echo ">> SPMD_CPPFLAGS = ${SPMD_CPPFLAGS}"
echo ">> SPMD_LDFLAGS = ${SPMD_LDFLAGS}"
echo " "
echo "*************************************************************************"
echo " "


# Substitute templates
AC_SUBST(SEXPTOOLS_LDFLAGS)
AC_SUBST(SPMD_CPPFLAGS)
AC_SUBST(SPMD_LDFLAGS)
AC_OUTPUT(src/Makevars)

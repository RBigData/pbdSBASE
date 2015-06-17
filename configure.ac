### autoconf configure.ac && autoreconf -if

################################################################################
# This file is free and unencumbered software released into the public domain.
#
# Anyone is free to copy, modify, publish, use, compile, sell, or distribute 
# this file, for any purpose, and by any means.
#
# In jurisdictions that recognize copyright laws, the author or authors of this 
# file dedicate any and all copyright interest in the file to the public
# domain. We make this dedication for the benefit of the public at large and to 
# the detriment of our heirs and successors. We intend this dedication to be an 
# overt act of relinquishment in perpetuity of all present and future rights to 
# this file under copyright law.
#
# THE FILE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
# AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN 
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
# WITH THIS FILE OR THE USE OR OTHER DEALINGS IN THIS FILE.
#
# For more information, please refer to http://unlicense.org/
################################################################################


AC_INIT(DESCRIPTION)
AC_LANG([C])


### PETSc and SLEPc
#PETSC_DIR=${HOME}/libs/petsc-3.5.2
#PETSC_ARCH="."


AC_ARG_WITH([petsc-home],
  [AS_HELP_STRING(
    [--with-petsc-home=PATH],
    [PETSC_HOME])],
  [PETSC_DIR=$withval])
AC_ARG_WITH([petsc-arch],
  [AS_HELP_STRING(
    [--with-petsc-arch=PATH],
    [PETSC_ARCH])],
  [PETSC_ARCH=$withval])


SLEPC_DIR=${HOME}/libs/slepc-3.4.3
SLEPC_ARCH="linux"



### RNACI and pbdMPI
R_SCMD="${R_HOME}/bin/Rscript -e"

SPMD_CPPFLAGS=`${R_SCMD} "source('./R/get_conf.r');get.conf('PKG_CPPFLAGS','"${R_ARCH}"')"`
SPMD_LDFLAGS=`${R_SCMD} "source('./R/get_conf.r');get.conf('PKG_LIBS','"${R_ARCH}"')"`


### Report
echo " "
echo "***************** Results of pbdSBASE package configure *****************"
echo " "
echo ">> SPMD_CPPFLAGS = ${SPMD_CPPFLAGS}"
echo ">> SPMD_LDFLAGS = ${SPMD_LDFLAGS}"
echo ">> PETSC_DIR = ${PETSC_DIR}"
echo ">> PETSC_ARCH = ${PETSC_ARCH}"
echo ">> SLEPC_DIR = ${SLEPC_DIR}"
echo ">> SLEPC_ARCH = ${SLEPC_ARCH}"
echo " "
echo "*************************************************************************"
echo " "


### Substitute templates
AC_SUBST(SPMD_CPPFLAGS)
AC_SUBST(SPMD_LDFLAGS)
AC_SUBST(PETSC_DIR)
AC_SUBST(PETSC_ARCH)
AC_SUBST(SLEPC_DIR)
AC_SUBST(SLEPC_ARCH)

AC_OUTPUT(src/Makevars)


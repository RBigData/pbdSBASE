#ifndef __SBASE_GLOBAL__
#define __SBASE_GLOBAL__


#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#include <SEXPtools.h>

#include <petscmat.h>


// Dummy return
#define R_ret_0 SEXP ret;PROTECT(ret=allocVector(INTSXP,1));INTEGER(ret)[0]=0;UNPROTECT(1);return ret


// petsc_converters.c
Mat sbase_convert_r_to_petsc(SEXP dim, SEXP data, SEXP row_ptr, SEXP col_ind);
SEXP sbase_convert_petsc_to_r(Mat mat);


#endif

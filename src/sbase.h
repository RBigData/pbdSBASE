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
SEXP sbase_convert_petsc_to_r(Mat mat);
Mat sbase_convert_r_to_petsc(SEXP dim, SEXP ldim, SEXP data, SEXP row_ptr, SEXP col_ind);


// petsc_print.c
SEXP sbase_petsc_matprinter_fmt(SEXP fmt);
SEXP sbase_petsc_matprinter(SEXP dim, SEXP ldim, SEXP data, SEXP row_ptr, SEXP col_ind);

#endif

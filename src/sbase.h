#ifndef __SBASE_GLOBAL__
#define __SBASE_GLOBAL__


#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

// Dummy return
#define R_ret_0 SEXP ret;PROTECT(ret=allocVector(INTSXP,1));INTEGER(ret)[0]=0;UNPROTECT(1);return ret

// Obtain int/double pointers
#define INT(x,i) (INTEGER(x)[i])
#define RL(x,i) (REAL(x)[i])

// Obtain character pointers
#define CHARPT(x,i)	((char*)CHAR(STRING_ELT(x,i)))


#endif

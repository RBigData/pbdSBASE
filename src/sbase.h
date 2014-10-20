#ifndef __SBASE_GLOBAL__
#define __SBASE_GLOBAL__


#include <RNACI.h>
#include <petscmat.h>


#define SBASEERR -1

#define R_ret_0 SEXP ret;PROTECT(ret=allocVector(INTSXP,1));INTEGER(ret)[0]=0;UNPROTECT(1);return ret
#define RETSBASEERR   SEXP SBASEERRVAL;PROTECT(SBASEERRVAL=allocVector(INTSXP,1));INTEGER(SBASEERRVAL)[0]=SBASEERR;UNPROTECT(1);return SBASEERRVAL

// TODO add comm_error() call after RETSBASEERR call
#define RCHKERRQ(n)   do { \
  if (PetscUnlikely(n)) { \
    PetscError(PETSC_COMM_SELF,__LINE__,PETSC_FUNCTION_NAME,__FILE__,n,PETSC_ERROR_REPEAT," "); \
    RETSBASEERR; \
  } \
} while (0)


// petsc_blas.c
SEXP sbase_petsc_matmatmult(SEXP A_dim, SEXP A_ldim, SEXP A_data, SEXP A_row_ptr, SEXP A_col_ind,
                            SEXP B_dim, SEXP B_ldim, SEXP B_data, SEXP B_row_ptr, SEXP B_col_ind);
SEXP sbase_petsc_mattranspose(SEXP dim, SEXP ldim, SEXP data, SEXP row_ptr, SEXP col_ind);


// petsc_comm.c
SEXP sbase_petsc_init();
SEXP sbase_petsc_finalize();
SEXP sbase_petsc_end();


// petsc_converters.c
SEXP sbase_convert_petscsparse_to_rsparse(Mat mat);
Mat sbase_convert_rsparse_to_petscsparse(SEXP dim, SEXP ldim, SEXP data, SEXP row_ptr, SEXP col_ind);


// petsc_invert.c
SEXP sbase_petsc_matinvert(SEXP dim, SEXP ldim, SEXP data, SEXP row_ptr, SEXP col_ind);


// petsc_printing.c
SEXP sbase_petsc_matprinter_fmt(SEXP fmt);
SEXP sbase_petsc_matprinter(SEXP dim, SEXP ldim, SEXP data, SEXP row_ptr, SEXP col_ind);


// petsc_special.c
PetscErrorCode sbase_petsc_identity_dense(Mat *identity, int m, int n, int M, int N);


// slepc_comm.c
SEXP sbase_slepc_init();
SEXP sbase_slepc_finalize();

#endif



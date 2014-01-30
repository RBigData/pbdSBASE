/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2013, Schmidt


#include "sbase.h"

// 
SEXP sbase_petsc_matinvert(SEXP dim, SEXP ldim, SEXP data, SEXP row_ptr, SEXP col_ind)
{
  SEXP R_list;
  Mat A, ;
  PetscErrorCode ierr;
  
  
  // Convert to PETSc storage
  A = sbase_convert_rsparse_to_petscsparse(dim, ldim, data, row_ptr, col_ind);
  
  
  
  
  
  // destroy petsc matrix
  if (A) {ierr = MatDestroy(&mat);CHKERRQ(ierr);}
  
  
  return R_list;
}



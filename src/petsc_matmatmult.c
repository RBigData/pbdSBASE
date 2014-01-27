/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2013, Schmidt


#include "sbase.h"

SEXP sbase_petsc_test_rmat(SEXP A_dim, SEXP A_ldim, SEXP A_data, SEXP A_row_ptr, SEXP A_col_ind)
{
  SEXP C_dim, C_ldim, C_data, C_row_ptr, C_col_ind;
  Mat A_mat, B_mat, C_mat;
  PetscErrorCode ierr;
  
  
  // build petsc matrix
  mat = sbase_convert_r_to_petsc(dim, ldim, data, row_ptr, col_ind);
  
  
  
  // Recreate R matrix
  rmat = sbase_convert_petsc_to_r(mat);
  
  // destroy petsc matrix
  if (mat)  {ierr = MatDestroy(&mat);CHKERRQ(ierr);}
  
  return rmat;
}

/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2014, Schmidt


#include "sbase.h"

SEXP sbase_petsc_matmatmult(SEXP A_dim, SEXP A_ldim, SEXP A_data, SEXP A_row_ptr, SEXP A_col_ind,
                            SEXP B_dim, SEXP B_ldim, SEXP B_data, SEXP B_row_ptr, SEXP B_col_ind)
{
  SEXP R_list;
  Mat A, B, C;
  PetscErrorCode ierr;
  
  
  // Convert to PETSc storage
  A = sbase_convert_rsparse_to_petscsparse(A_dim, A_ldim, A_data, A_row_ptr, A_col_ind);
  B = sbase_convert_rsparse_to_petscsparse(B_dim, B_ldim, B_data, B_row_ptr, B_col_ind);
  
/*  MatView(A,PETSC_VIEWER_STDOUT_WORLD);*/
  
  ierr = MatMatMult(A, B, MAT_INITIAL_MATRIX, PETSC_DEFAULT, &C);
  
  
  // Convert to R storage
  R_list = sbase_convert_petscsparse_to_rsparse(C);
  
  
  // destroy petsc matrix
  if (A) {ierr = MatDestroy(&A);CHKERRQ(ierr);}
  if (B) {ierr = MatDestroy(&B);CHKERRQ(ierr);}
  if (C) {ierr = MatDestroy(&C);CHKERRQ(ierr);}
  
  
  return R_list;
}





SEXP sbase_petsc_mattranspose(SEXP dim, SEXP ldim, SEXP data, SEXP row_ptr, SEXP col_ind)
{
  SEXP R_list;
  Mat mat;
  PetscErrorCode ierr;
  
  
  // Convert to PETSc storage
  mat = sbase_convert_rsparse_to_petscsparse(dim, ldim, data, row_ptr, col_ind);
  
/*  ierr = MatTranspose(mat, MAT_REUSE_MATRIX, &mat);*/
  ierr = MatTranspose(mat, MAT_INITIAL_MATRIX, &mat);
  
  
  // Convert to R storage
  R_list = sbase_convert_petscsparse_to_rsparse(mat);
  
  
  // destroy petsc matrix
  if (mat) {ierr = MatDestroy(&mat);CHKERRQ(ierr);}
  
  
  return R_list;
}



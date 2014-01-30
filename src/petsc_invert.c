/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2013, Schmidt


#include "sbase.h"

// From the PETSc FAQ:
/* 
   The inverse of a matrix (dense or sparse) is essentially always 
   dense, so begin by creating a dense matrix B and fill it with the 
   identity matrix (ones along the diagonal), also create a dense 
   matrix X of the same size that will hold the solution. Then factor 
   the matrix you wish to invert with MatLUFactor() or 
   MatCholeskyFactor(), call the result A. Then call 
   MatMatSolve(A,B,X) to compute the inverse into X.
*/ 
SEXP sbase_petsc_matinvert(SEXP dim, SEXP ldim, SEXP data, SEXP row_ptr, SEXP col_ind)
{
  SEXP R_mat;
  Mat A, B, X;
  PetscErrorCode ierr;
  MatFactorInfo info;
  PetscInt rstart, rend;
  IS row, col;
  int i, j;
  double v;
  const int M = INT(dim, 0), N = INT(dim, 1);
/*  const int m = PETSC_DECIDE, n = PETSC_DECIDE;*/
  const int m = INT(ldim, 0), n = INT(ldim, 1);
  
  
  // Convert input to PETSc storage
  A = sbase_convert_rsparse_to_petscsparse(dim, ldim, data, row_ptr, col_ind);
  
  
  // Preallocate identity matrix B and solution matrix X
  ierr = sbase_petsc_identity_dense(&B, m, n, M, N);CHKERRQ(ierr);
  
  
  ierr = MatCreate(PETSC_COMM_WORLD, &X);CHKERRQ(ierr);
  ierr = MatSetType(X, MATMPIDENSE);CHKERRQ(ierr);
  
  ierr = MatSetSizes(X, m, n, M, N);CHKERRQ(ierr);
  ierr = MatSetFromOptions(X);CHKERRQ(ierr);
  
  
  // Invert
  MatLUFactor(A, row, col, &info);
  MatMatSolve(A, B, X);
  
  
  // Convert back
  R_mat = sbase_convert_petscdense_to_rdense(X);
  
  
  // destroy petsc matrix
  if (A) {ierr = MatDestroy(&A);CHKERRQ(ierr);}
  if (B) {ierr = MatDestroy(&B);CHKERRQ(ierr);}
  if (X) {ierr = MatDestroy(&X);CHKERRQ(ierr);}
  
  
  return R_mat;
}



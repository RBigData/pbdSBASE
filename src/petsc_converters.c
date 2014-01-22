/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2013, Schmidt


#include "sbase.h"
#include <petscmat.h>

#include <stdlib.h>


// Convert petsc AIJ storage to R storage
SEXP sbase_convert_r_to_petsc(SEXP x)
{
  Mat mat;
  
  return mat;
}


// Convert petsc AIJ storage to R storage
PetscErrorCode sbase_convert_petsc_to_r_data(Mat mat, SEXP *R_data, SEXP *R_data_rows, SEXP *R_data_cols)
{
  Mat                 mat_local;
  PetscBool           get_row_check = PETSC_TRUE,
                      shift = PETSC_FALSE,
                      symmetric = PETSC_FALSE;
  PetscInt            rstart, rend;
  PetscErrorCode      ierr;
  PetscInt            row, ncols, nrows, n;
  const PetscInt      *cols, *ia, *ja;
  const PetscScalar   *vals;
  int                 i, j;
  int                 ct = 0, len = 0;
  
  
  ierr = MatGetOwnershipRange(mat, &rstart, &rend);CHKERRQ(ierr);
  
  // Determine local size and allocate R object
  for (row=rstart; row<rend; row++)
  {
    ierr  = MatGetRow(mat, row, &ncols, &cols, NULL);CHKERRQ(ierr);
    len += ncols;
    ierr = MatRestoreRow(mat, row, &ncols, &cols, NULL);CHKERRQ(ierr);
  }
  
  PROTECT(*R_data = allocVector(REALSXP, len));
  for (j=0; j<len; j++)
    REAL(*R_data)[j] = 0.;
  
  // Fill the R object
  for (row=rstart; row<rend; row++) 
  {
    ierr = MatGetRow(mat, row, &ncols, &cols, &vals);CHKERRQ(ierr);
    
    for (j=0; j<ncols; j++) 
    {
      REAL(*R_data)[ct] = vals[j];
      ct++;
    }
    
    ierr = MatRestoreRow(mat, row, &ncols, &cols, &vals);CHKERRQ(ierr);
  }
  
  
  
  ierr = MatMPIAIJGetLocalMat(mat, MAT_INITIAL_MATRIX, &mat_local);
/*  ierr = MatMPIAIJGetLocalMatCondensed(mat, MAT_INITIAL_MATRIX, NULL, NULL, &mat_local);*/
  
/*  ierr = MatGetOwnershipRange(mat_local, &rstart, &rend);CHKERRQ(ierr);*/
  
  ierr = MatGetRowIJ(mat_local, 1, shift, symmetric, &n, &ia, &ja, &get_row_check);CHKERRQ(ierr);
  
  PROTECT(*R_data_rows = allocVector(INTSXP, n));
  
  for (i=0; i<n; i++)
    INTEGER(*R_data_rows)[i] = ia[i];
  
  PROTECT(*R_data_cols = allocVector(INTSXP, len));
  
  for (i=0; i<len; i++)
    INTEGER(*R_data_cols)[i] = ja[i];
  
  ierr = MatRestoreRowIJ(mat_local, 1, shift, symmetric, &n, &ia, &ja, &get_row_check);CHKERRQ(ierr);
  
  MatDestroy(&mat_local);
  
  
  
  
  UNPROTECT(3);
  return ierr;
}



SEXP sbase_convert_petsc_to_r(Mat mat)
{
  SEXP R_data, R_data_rows, R_data_cols;
  SEXP R_list;
  
/*  MatView(mat,PETSC_VIEWER_STDOUT_WORLD);*/
  
  sbase_convert_petsc_to_r_data(mat, &R_data, &R_data_rows, &R_data_cols);
  
  return R_data_rows;
/*  return R_data;*/
}



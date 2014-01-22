/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2013, Schmidt


#include "sbase.h"
#include <petscmat.h>

#include <stdlib.h>

#include <SEXPtools.h>


// Convert R storage to PETSc MPIAIJ storage
#if 0
SEXP sbase_convert_r_to_petsc(SEXP x, SEXP dim)
{
  Mat                 mat;
  PetscErrorCode      ierr;
  const int           m = nrow(x), n = ncol(x);
  const int           M = INTEGER(dim)[0], N = INTEGER(dim)[1];
  
  
  ierr = MatCreate(PETSC_COMM_WORLD, &mat);CHKERRQ(ierr);
  ierr = MatSetType(mat, MATMPIAIJ);CHKERRQ(ierr);
  
  ierr = MatSetSizes(mat, m, n, N, M);CHKERRQ(ierr);
  ierr = MatSetFromOptions(mat);CHKERRQ(ierr);
  
/*  ierr = MatMPIAIJSetPreallocation(mat, PetscInt d_nz,const PetscInt d_nnz[],PetscInt o_nz,const PetscInt o_nnz[]);*/
  
  ierr = MatSetUp(mat);CHKERRQ(ierr);
  ierr = MatGetOwnershipRange(mat,&rstart,&rend);CHKERRQ(ierr);
  for (i=rstart; i<rend; i+=2) {
    for (j=0; j<n; j+=2) {
      v = 10.0*i+j;
      ierr = MatSetValues(mat,1,&i,1,&j,&v,INSERT_VALUES);CHKERRQ(ierr);
    }
  }
  ierr = MatAssemblyBegin(mat,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  ierr = MatAssemblyEnd(mat,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  
  
  
  
  
  return mat;
}
#endif


// Convert PETSc MPIAIJ storage to R storage
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
  SEXP R_list, R_list_names;
  
/*  MatView(mat,PETSC_VIEWER_STDOUT_WORLD);*/
  
  sbase_convert_petsc_to_r_data(mat, &R_data, &R_data_rows, &R_data_cols);
  
  R_list_names = make_list_names(3, "Data", "rows", "cols");
  R_list = make_list(R_list_names, R_data, R_data_rows, R_data_cols);
  
  return R_list;
/*  return R_data;*/
}



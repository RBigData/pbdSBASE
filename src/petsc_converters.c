/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2013, Schmidt


#include "sbase.h"
#include <stdlib.h>


// ----------------------------------------------------
// Convert R storage to PETSc MPIAIJ storage
// ----------------------------------------------------

Mat sbase_convert_r_to_petsc(SEXP dim, SEXP ldim, SEXP data, SEXP row_ptr, SEXP col_ind)
{
  Mat                 mat;
  PetscErrorCode      ierr;
  int rstart, rend;
  int i, j;
  double v;
  int           m = INT(ldim, 0), n = INT(ldim, 1);
  int           M = INT(dim, 0), N = INT(dim, 1);
  int c = 0, r = 0, diff;
  
  n = PETSC_DECIDE; // FIXME why does this work...?
  
  PROTECT(row_ptr);
  PROTECT(col_ind);
  PROTECT(data);
  for (i=0; i<LENGTH(row_ptr); i++)
    INT(row_ptr, i) = INT(row_ptr, i)-1;
  for (i=0; i<LENGTH(col_ind); i++)
    INT(col_ind, i) = INT(col_ind, i)-1;
  
  ierr = MatCreate(PETSC_COMM_WORLD, &mat);CHKERRQ(ierr);
  ierr = MatSetType(mat, MATMPIAIJ);CHKERRQ(ierr);
  
  ierr = MatSetSizes(mat, m, n, M, N);CHKERRQ(ierr);
  ierr = MatSetFromOptions(mat);CHKERRQ(ierr);
  
  MatMPIAIJSetPreallocationCSR(mat, INTP(row_ptr), INTP(col_ind), DBLP(data));
  
  ierr = MatCreateMPIAIJWithArrays(PETSC_COMM_WORLD, m, n, M, N, INTP(row_ptr), INTP(col_ind), DBLP(data), &mat);CHKERRQ(ierr);
  
  UNPROTECT(3);
  
  
  
/*  ierr = MatCreate(PETSC_COMM_WORLD, &mat);CHKERRQ(ierr);*/
/*  ierr = MatSetType(mat, MATMPIAIJ);CHKERRQ(ierr);*/
/*  */
/*  ierr = MatSetSizes(mat, m, n, M, N);CHKERRQ(ierr);*/
/*  ierr = MatSetFromOptions(mat);CHKERRQ(ierr);*/
/*  */
/*  //ierr = MatMPIAIJSetPreallocation(mat, PetscInt d_nz,const PetscInt d_nnz[],PetscInt o_nz,const PetscInt o_nnz[]);*/
/*  */
/*  ierr = MatSetUp(mat);CHKERRQ(ierr);*/
/*  ierr = MatGetOwnershipRange(mat,&rstart,&rend);CHKERRQ(ierr);*/
/*  i = 0;*/
/*  while (r < m && INT(row_ptr, r) < INT(row_ptr, m))*/
/*  {*/
/*    diff = INT(row_ptr, r+1) - INT(row_ptr, r);*/
/*    */
/*    if (diff == 0)*/
/*      goto increment;*/
/*    else*/
/*    {*/
/*      while (diff)*/
/*      {*/
/*        j = INT(col_ind, c)-1;*/
/*        v = DBL(data, c);*/
/*        */
/*        printf("%d %d %f\n", i, j, v);*/
/*        ierr = MatSetValues(mat,1,&i,1,&j,&v,INSERT_VALUES);CHKERRQ(ierr);*/
/*        */
/*        c++; // hehehe*/
/*        diff--;*/
/*      }*/
/*    }*/
/*    */
/*    increment:*/
/*      r++;*/
/*      i++;*/
/*  }*/
/*  */
/*  ierr = MatAssemblyBegin(mat,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);*/
/*  ierr = MatAssemblyEnd(mat,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);*/
  
  
  return mat;
}



// ----------------------------------------------------
// Convert PETSc MPIAIJ storage to R storage
// ----------------------------------------------------

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
  
  PROTECT(*R_data_rows = allocVector(INTSXP, n+1));
  
  for (i=0; i<=n; i++)
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
  SEXP data, row_ptr, col_ind;
  SEXP R_list, R_list_names;
  
  
  sbase_convert_petsc_to_r_data(mat, &data, &row_ptr, &col_ind);
  
  R_list_names = make_list_names(3, "Data", "row_ptr", "col_ind");
  R_list = make_list(R_list_names, data, row_ptr, col_ind);
  
  return R_list;
/*  return R_data;*/
}



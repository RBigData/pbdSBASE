/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

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
SEXP sbase_convert_petsc_to_r(Mat mat)
{
  PetscInt            rstart, rend;
  PetscErrorCode      ierr;
  PetscInt            row, ncols, nrows, n;
  PetscBool           get_row_check = PETSC_TRUE;
  const PetscInt      *cols, *ia, *ja;
  const PetscScalar   *vals;
  int                 i, j;
  int                 len = 0, ct = 0;
  SEXP                R_data, R_data_rows, R_data_cols;
  
  
  ierr = MatGetOwnershipRange(mat, &rstart, &rend);CHKERRQ(ierr);
  
  // Determine local size and allocate R object
  for (row=rstart; row<rend; row++)
  {
    ierr  = MatGetRow(mat, row, &ncols, &cols, NULL);CHKERRQ(ierr);
    len += ncols;
    ierr = MatRestoreRow(mat, row, &ncols, &cols, NULL);CHKERRQ(ierr);
  }
  
  PROTECT(R_data = allocVector(REALSXP, len));
  for (j=0; j<len; j++)
    REAL(R_data)[j] = 0.;
  
  // Fill the R object
  for (row=rstart; row<rend; row++) 
  {
    ierr = MatGetRow(mat, row, &ncols, &cols, &vals);CHKERRQ(ierr);
    ierr = MatGetRowIJ(mat, 1, PETSC_FALSE, PETSC_FALSE, &n, &ia, &ja, &get_row_check);CHKERRQ(ierr);
    printf("check=%d, PETSC_FALSE=%d\n", get_row_check, PETSC_FALSE);
    
    if (get_row_check == PETSC_FALSE) exit(1);
    
    printf("ncols=%d, n=%d\n", ncols, n);
    for (j=0; j<ncols; j++) 
    {
      REAL(R_data)[ct] = vals[j];
      ct++;
/*      printf("%d\n", ia[j]);*/
    }
    
    ierr = MatRestoreRow(mat, row, &ncols, &cols, &vals);CHKERRQ(ierr);
    ierr = MatRestoreRowIJ(mat, 1, PETSC_FALSE, PETSC_TRUE, &n, ia, ja, &get_row_check);
  }
  
  
  return R_data;
}



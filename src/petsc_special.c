/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2014, Schmidt


#include "sbase.h"


// does what it says
PetscErrorCode sbase_petsc_identity_dense(Mat *identity, int m, int n, int M, int N)
{
  PetscErrorCode ierr;
  MatFactorInfo info;
  PetscInt rstart, rend;
  int i, j;
  double v;
  
  
  ierr = MatCreate(PETSC_COMM_WORLD, identity);RCHKERRQ(ierr);
  ierr = MatSetType(*identity, MATMPIDENSE);RCHKERRQ(ierr);
  
  ierr = MatSetSizes(*identity, m, n, M, N);RCHKERRQ(ierr);
  ierr = MatSetFromOptions(*identity);RCHKERRQ(ierr);
  
/*  ierr = MatGetOwnershipRange(*identity, &rstart, &rend);RCHKERRQ(ierr);*/
  ierr = MatSetUp(*identity);RCHKERRQ(ierr);
  
  for (j=0; j<N; j++)
  {
    for (i=0; i<N; i++)
    {
      if (i == j)
        v = 1.0;
      else
        v = 0.0;
      
      ierr = MatSetValues(*identity, M, &i, n, &j, &v, INSERT_VALUES);
    }
  }
  
  ierr = MatAssemblyBegin(*identity, MAT_FINAL_ASSEMBLY);RCHKERRQ(ierr);
  ierr = MatAssemblyEnd(*identity, MAT_FINAL_ASSEMBLY);RCHKERRQ(ierr);
  
  ierr  = MatView(*identity, PETSC_VIEWER_STDOUT_WORLD);RCHKERRQ(ierr);
  
  PetscFunctionReturn(0);
}






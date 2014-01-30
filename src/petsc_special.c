/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2013, Schmidt


#include "sbase.h"


// does what it says
Mat sbase_petsc_identity_dense(Mat *identity, int m, int n, int M, int N)
{
  PetscErrorCode ierr;
  MatFactorInfo info;
  PetscInt rstart, rend;
  int i, j;
  double v;
  
  
  ierr = MatCreate(PETSC_COMM_WORLD, identity);CHKERRQ(ierr);
  ierr = MatSetType(identity, MATMPIDENSE);CHKERRQ(ierr);
  
  ierr = MatSetSizes(*identity, m, n, M, N);CHKERRQ(ierr);
  ierr = MatSetFromOptions(*identity);CHKERRQ(ierr);
  
/*  ierr = MatGetOwnershipRange(*identity, &rstart, &rend);CHKERRQ(ierr);*/
  ierr = MatSetUp(*identity);CHKERRQ(ierr);
  
  for (j=0; j<1; j++)
  {
    for (i=0; i<1; i++)
    {
      if (i == j)
        v = 1.0;
      else
        v = 0.0;
      
      ierr = MatSetValues(*identity, M, &i, n, &j, &v, INSERT_VALUES);
    }
  }
  
  ierr = MatAssemblyBegin(*identity, MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  ierr = MatAssemblyEnd(*identity, MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  
  ierr  = MatView(*identity, PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
  
  return identity;
}






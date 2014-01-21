/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2013, Schmidt


#include "sbase.h"
#include <petscmat.h>


// Start petsc, calls MPI_Init()
SEXP sbase_petsc_init()
{
  PetscErrorCode ierr;
  
  ierr = PetscInitialize(0, NULL, NULL, NULL);CHKERRQ(ierr);
  
  R_ret_0;
}

// Calls MPI_Finalize() if MPI_Init() had not been called before PetscInitialize()
SEXP sbase_petsc_finalize()
{
  PetscErrorCode ierr;
  
  ierr = PetscFinalize();CHKERRQ(ierr);
  
  R_ret_0;
}

// Calls PetscFinalize() and ends the program
SEXP sbase_petsc_end()
{
  PetscErrorCode ierr;
  
  // Start petsc, calls MPI_Init()
  ierr = PetscEnd();CHKERRQ(ierr);
  
  R_ret_0;
}




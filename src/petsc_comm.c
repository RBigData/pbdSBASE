/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2014, Schmidt


#include <RNACI.h>
#include <petscmat.h>


// Start petsc, calls MPI_Init()
SEXP sbase_petsc_init()
{
  PetscErrorCode ierr;
  
  ierr = PetscInitialize(0, NULL, NULL, NULL);CHKERRQ(ierr);
  
  return RNULL;
}

// Calls MPI_Finalize() if MPI_Init() had not been called before PetscInitialize()
SEXP sbase_petsc_finalize()
{
  PetscErrorCode ierr;
  
  ierr = PetscFinalize();CHKERRQ(ierr);
  
  return RNULL;
}

// Calls PetscFinalize() and ends the program
SEXP sbase_petsc_end()
{
  PetscErrorCode ierr;
  
  // Start petsc, calls MPI_Init()
  ierr = PetscEnd();CHKERRQ(ierr);
  
  return RNULL;
}




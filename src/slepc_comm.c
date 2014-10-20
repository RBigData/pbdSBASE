/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2014, Schmidt


#include "sbase.h"


// Start petsc, calls MPI_Init()
SEXP sbase_slepc_init()
{
  PetscErrorCode ierr;
  
  ierr = SlepcInitialize(0, NULL, (char*)0, NULL);RCHKERRQ(ierr);
  
  return RNULL;
}

// Calls MPI_Finalize() if MPI_Init() had not been called before PetscInitialize()
SEXP sbase_slepc_finalize()
{
  PetscErrorCode ierr;
  
  ierr = SlepcFinalize();RCHKERRQ(ierr);
  
  return RNULL;
}



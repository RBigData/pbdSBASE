/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// Copyright 2013, Schmidt


#include "sbase.h"
#include <petscmat.h>


// Print format for matrices
// Apologies for the ugliness
#define SBASE_MATPRINT_DEFAULT 0
#define SBASE_MATPRINT_MATLAB 1
#define SBASE_MATPRINT_DENSE 2
#define SBASE_MATPRINT_IMPL 3
#define SBASE_MATPRINT_INFO 4
#define SBASE_MATPRINT_INFO_DETAIL 5
#define SBASE_MATPRINT_COMMON 6
#define SBASE_MATPRINT_INDEX 7
#define SBASE_MATPRINT_SYMMODU 8
#define SBASE_MATPRINT_VTK 9
#define SBASE_MATPRINT_NATIVE 10
#define SBASE_MATPRINT_BASIC 11
#define SBASE_MATPRINT_LG 12
#define SBASE_MATPRINT_CONTOUR 13

SEXP sbase_petsc_matprinter(SEXP fmt)
{
  PetscErrorCode ierr;
  
  if (INT(fmt, 0) == SBASE_MATPRINT_DEFAULT)
    ierr = PetscViewerSetFormat(PETSC_VIEWER_STDOUT_WORLD, PETSC_VIEWER_DEFAULT);
  else if (INT(fmt, 0) == SBASE_MATPRINT_MATLAB)
    ierr = PetscViewerSetFormat(PETSC_VIEWER_STDOUT_WORLD, PETSC_VIEWER_ASCII_MATLAB);
  else if (INT(fmt, 0) == SBASE_MATPRINT_DENSE)
    ierr = PetscViewerSetFormat(PETSC_VIEWER_STDOUT_WORLD, PETSC_VIEWER_ASCII_DENSE);
  else if (INT(fmt, 0) == SBASE_MATPRINT_IMPL)
    ierr = PetscViewerSetFormat(PETSC_VIEWER_STDOUT_WORLD, PETSC_VIEWER_ASCII_IMPL);
  else if (INT(fmt, 0) == SBASE_MATPRINT_INFO)
    ierr = PetscViewerSetFormat(PETSC_VIEWER_STDOUT_WORLD, PETSC_VIEWER_ASCII_INFO);
  else if (INT(fmt, 0) == SBASE_MATPRINT_INFO_DETAIL)
    ierr = PetscViewerSetFormat(PETSC_VIEWER_STDOUT_WORLD, PETSC_VIEWER_ASCII_INFO_DETAIL);
  else if (INT(fmt, 0) == SBASE_MATPRINT_COMMON)
    ierr = PetscViewerSetFormat(PETSC_VIEWER_STDOUT_WORLD, PETSC_VIEWER_ASCII_COMMON);
  else if (INT(fmt, 0) == SBASE_MATPRINT_INDEX)
    ierr = PetscViewerSetFormat(PETSC_VIEWER_STDOUT_WORLD, PETSC_VIEWER_ASCII_INDEX);
  else if (INT(fmt, 0) == SBASE_MATPRINT_SYMMODU)
    ierr = PetscViewerSetFormat(PETSC_VIEWER_STDOUT_WORLD, PETSC_VIEWER_ASCII_SYMMODU);
  else if (INT(fmt, 0) == SBASE_MATPRINT_VTK)
    ierr = PetscViewerSetFormat(PETSC_VIEWER_STDOUT_WORLD, PETSC_VIEWER_ASCII_VTK);
  else if (INT(fmt, 0) == SBASE_MATPRINT_NATIVE)
    ierr = PetscViewerSetFormat(PETSC_VIEWER_STDOUT_WORLD, PETSC_VIEWER_NATIVE);
  else if (INT(fmt, 0) == SBASE_MATPRINT_BASIC)
    ierr = PetscViewerSetFormat(PETSC_VIEWER_STDOUT_WORLD, PETSC_VIEWER_DRAW_BASIC);
  else if (INT(fmt, 0) == SBASE_MATPRINT_LG)
    ierr = PetscViewerSetFormat(PETSC_VIEWER_STDOUT_WORLD, PETSC_VIEWER_DRAW_LG);
  else if (INT(fmt, 0) == SBASE_MATPRINT_CONTOUR)
    ierr = PetscViewerSetFormat(PETSC_VIEWER_STDOUT_WORLD, PETSC_VIEWER_DRAW_CONTOUR);
  
  CHKERRQ(ierr);
  
  R_ret_0;
}

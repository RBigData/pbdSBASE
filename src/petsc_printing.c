/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2014, Schmidt


#include "sbase.h"
/*#define PM(id,rn) {id, #id, rn}*/
#define PM(id,rn) {id, rn}

struct printmap_s{
  int id;
/*  const char *petsc_name;*/
  const char *Rname;
};

const static struct printmap_s printmap[] = 
{
  PM(PETSC_VIEWER_DEFAULT, "default"),
  PM(PETSC_VIEWER_ASCII_MATLAB, "matlab"),
  PM(PETSC_VIEWER_ASCII_DENSE, "dense"), 
  PM(PETSC_VIEWER_ASCII_IMPL, "impl"), 
  PM(PETSC_VIEWER_ASCII_INFO, "info"), 
  PM(PETSC_VIEWER_ASCII_INFO_DETAIL, "info_detail"),
  PM(PETSC_VIEWER_ASCII_COMMON, "common"), 
  PM(PETSC_VIEWER_ASCII_INDEX, "index"), 
  PM(PETSC_VIEWER_ASCII_SYMMODU, "symmodu"),
  PM(PETSC_VIEWER_ASCII_VTK, "vtk"),
  PM(PETSC_VIEWER_NATIVE, "native"),
  PM(PETSC_VIEWER_DRAW_BASIC, "basic"),
  PM(PETSC_VIEWER_DRAW_LG, "lg"),
  PM(PETSC_VIEWER_DRAW_CONTOUR, "contour"),
  {-1, NULL}
};



SEXP sbase_petsc_printer_lookup_code(SEXP fmtnm)
{
  R_INIT;
  SEXP fmt;
  int i;
  
  newRvec(fmt, 1, "int");
  
  for (i=0; printmap[i].Rname != NULL; i++)
  {
    if (strcmp(STR(fmtnm), printmap[i].Rname) == 0)
    {
      INT(fmt) = printmap[i].id;
      return fmt;
    }
  }
  
  R_END;
  return RNULL;
}



SEXP sbase_petsc_matprinter_fmt(SEXP fmt)
{
  PetscErrorCode ierr = 0;
  
  ierr = PetscViewerSetFormat(PETSC_VIEWER_STDOUT_WORLD, INT(fmt));
  RCHKERRQ(ierr);
  
  return RNULL;
}



SEXP sbase_petsc_matprinter(SEXP dim, SEXP ldim, SEXP data, SEXP row_ptr, SEXP col_ind)
{
  Mat mat;
  PetscErrorCode ierr;
  
  // Convert to PETSc storage
  mat = sbase_convert_rsparse_to_petscsparse(dim, ldim, data, row_ptr, col_ind);
  
  // Print
  MatView(mat, PETSC_VIEWER_STDOUT_WORLD);
  
  // destroy petsc matrix
  if (mat) {ierr = MatDestroy(&mat);RCHKERRQ(ierr);}
  
  return RNULL;
}


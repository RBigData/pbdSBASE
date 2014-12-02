/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2014, Schmidt


//FIXME

#include "sbase.h"

SEXP sbase_petsc_test()
{
  Mat               A,Asp;
  PetscViewer       fd;                        /* viewer */
  char              file[PETSC_MAX_PATH_LEN];  /* input file name */
  PetscErrorCode    ierr;
  PetscInt          m,n,rstart,rend;
  PetscBool         flg;
  PetscInt          row,ncols,j,nrows,nnzA=0,nnzAsp=0;
  const PetscInt    *cols;
  const PetscScalar *vals;
  PetscReal         norm,percent,val,dtol=1.e-16;
  PetscMPIInt       rank;
  MatInfo           matinfo;
  PetscInt          Dnnz,Onnz;


  ierr = MPI_Comm_rank(PETSC_COMM_WORLD,&rank);RCHKERRQ(ierr);

  /* Determine files from which we read the linear systems. */
  ierr = PetscOptionsGetString(NULL,"-f",file,PETSC_MAX_PATH_LEN,&flg);RCHKERRQ(ierr);
  if (!flg) SETERRQ(PETSC_COMM_WORLD,1,"Must indicate binary file with the -f option");

  /* Open binary file.  Note that we use FILE_MODE_READ to indicate
     reading from this file. */
  ierr = PetscViewerBinaryOpen(PETSC_COMM_WORLD,file,FILE_MODE_READ,&fd);RCHKERRQ(ierr);

  /* Load the matrix; then destroy the viewer. */
  ierr = MatCreate(PETSC_COMM_WORLD,&A);RCHKERRQ(ierr);
  ierr = MatSetOptionsPrefix(A,"a_");RCHKERRQ(ierr);
  ierr = MatSetFromOptions(A);RCHKERRQ(ierr);
  ierr = MatLoad(A,fd);RCHKERRQ(ierr);
  ierr = PetscViewerDestroy(&fd);RCHKERRQ(ierr);
  ierr = MatGetSize(A,&m,&n);RCHKERRQ(ierr);
  ierr = MatGetInfo(A,MAT_LOCAL,&matinfo);RCHKERRQ(ierr);
  /*printf("matinfo.nz_used %g\n",matinfo.nz_used);*/

  /* Get a sparse matrix Asp by dumping zero entries of A */
  ierr = MatCreate(PETSC_COMM_WORLD,&Asp);RCHKERRQ(ierr);
  ierr = MatSetSizes(Asp,m,n,PETSC_DECIDE,PETSC_DECIDE);RCHKERRQ(ierr);
  ierr = MatSetOptionsPrefix(Asp,"asp_");RCHKERRQ(ierr);
  ierr = MatSetFromOptions(Asp);RCHKERRQ(ierr);
  Dnnz = (PetscInt)matinfo.nz_used/m + 1;
  Onnz = Dnnz/2;
  printf("Dnnz %d %d\n",Dnnz,Onnz);
  ierr = MatSeqAIJSetPreallocation(Asp,Dnnz,NULL);RCHKERRQ(ierr);
  ierr = MatMPIAIJSetPreallocation(Asp,Dnnz,NULL,Onnz,NULL);RCHKERRQ(ierr);
  /* The allocation above is approximate so we must set this option to be permissive.
   * Real code should preallocate exactly. */
  ierr = MatSetOption(Asp,MAT_NEW_NONZERO_LOCATION_ERR,PETSC_FALSE);RCHKERRQ(ierr);

  /* Check zero rows */
  ierr  = MatGetOwnershipRange(A,&rstart,&rend);RCHKERRQ(ierr);
  nrows = 0;
  for (row=rstart; row<rend; row++) {
    ierr  = MatGetRow(A,row,&ncols,&cols,&vals);RCHKERRQ(ierr);
    nnzA += ncols;
    norm  = 0.0;
    for (j=0; j<ncols; j++) {
      val = PetscAbsScalar(vals[j]);
      if (norm < val) norm = norm;
      if (val > dtol) {
        ierr = MatSetValues(Asp,1,&row,1,&cols[j],&vals[j],INSERT_VALUES);RCHKERRQ(ierr);
        nnzAsp++;
      }
    }
    if (!norm) nrows++;
    ierr = MatRestoreRow(A,row,&ncols,&cols,&vals);RCHKERRQ(ierr);
  }
  ierr = MatAssemblyBegin(Asp,MAT_FINAL_ASSEMBLY);RCHKERRQ(ierr);
  ierr = MatAssemblyEnd(Asp,MAT_FINAL_ASSEMBLY);RCHKERRQ(ierr);

  percent=(PetscReal)nnzA*100/(m*n);
  ierr   = PetscPrintf(PETSC_COMM_SELF," [%d] Matrix A local size %d,%d; nnzA %d, %g percent; No. of zero rows: %d\n",rank,m,n,nnzA,percent,nrows);
  percent=(PetscReal)nnzAsp*100/(m*n);
  ierr   = PetscPrintf(PETSC_COMM_SELF," [%d] Matrix Asp nnzAsp %d, %g percent\n",rank,nnzAsp,percent);

  /* investigate matcoloring for Asp */
  PetscBool Asp_coloring = PETSC_FALSE;
  ierr = PetscOptionsHasName(NULL,"-Asp_color",&Asp_coloring);RCHKERRQ(ierr);
  if (Asp_coloring) {
    ISColoring    iscoloring;
    MatFDColoring matfdcoloring;
    ierr = PetscPrintf(PETSC_COMM_WORLD," Create coloring of Asp...\n");
    ierr = MatGetColoring(Asp,MATCOLORINGSL,&iscoloring);RCHKERRQ(ierr);
    ierr = MatFDColoringCreate(Asp,iscoloring,&matfdcoloring);RCHKERRQ(ierr);
    ierr = MatFDColoringSetFromOptions(matfdcoloring);RCHKERRQ(ierr);
    /*ierr = MatFDColoringView(matfdcoloring,PETSC_VIEWER_STDOUT_WORLD);RCHKERRQ(ierr);*/
    ierr = ISColoringDestroy(&iscoloring);RCHKERRQ(ierr);
    ierr = MatFDColoringDestroy(&matfdcoloring);RCHKERRQ(ierr);
  }

  /* Write Asp in binary for study - see ~petsc/src/mat/examples/tests/ex124.c */
  PetscBool Asp_write = PETSC_FALSE;
  ierr = PetscOptionsHasName(NULL,"-Asp_write",&Asp_write);RCHKERRQ(ierr);
  if (Asp_write) {
    PetscViewer viewer;
    ierr = PetscPrintf(PETSC_COMM_SELF,"Write Asp into file Asp.dat ...\n");
    ierr = PetscViewerBinaryOpen(PETSC_COMM_WORLD,"Asp.dat",FILE_MODE_WRITE,&viewer);RCHKERRQ(ierr);
    ierr = MatView(Asp,viewer);RCHKERRQ(ierr);
    ierr = PetscViewerDestroy(&viewer);RCHKERRQ(ierr);
  }

  ierr = MatDestroy(&A);RCHKERRQ(ierr);
  ierr = MatDestroy(&Asp);RCHKERRQ(ierr);
/*  ierr = PetscFinalize();*/
  
  
  R_ret_0;
}















#if 0
// test
SEXP sbase_petsc_test()
{
  SEXP           ret;
  
  
  PetscInt ma, na;
  MatInfo info;
  Mat            mat;
  PetscInt       m = 7,n,i,j,rstart,rend,rect = 0;
  PetscErrorCode ierr;
  PetscMPIInt    size, rank;
  PetscBool      flg;
  PetscScalar    v, alpha;
  PetscReal      normf,normi,norm1;
  
  
  const PetscScalar *vals;
  PetscInt          row,ncols,nrows,nnzA=0,nnzAsp=0;
  const PetscInt    *cols;
  
  
  
  
  
  
  
  PROTECT(ret = allocVector(REALSXP, 1));
  
  
  
  ierr = PetscOptionsGetInt(NULL,"-m",&m,NULL);RCHKERRQ(ierr);
  
  ierr = MPI_Comm_rank(PETSC_COMM_WORLD,&rank);RCHKERRQ(ierr);
  ierr = MPI_Comm_size(PETSC_COMM_WORLD,&size);RCHKERRQ(ierr);
  
  printf("%d\n", size);
  
  n = m;
  
  ierr = PetscOptionsHasName(NULL,"-rectA",&flg);RCHKERRQ(ierr);
  if (flg) {n += 2; rect = 1;}
  ierr = PetscOptionsHasName(NULL,"-rectB",&flg);RCHKERRQ(ierr);
  if (flg) {n -= 2; rect = 1;}
  
  
  /* ------- Assemble matrix, test MatValid() --------- */
  ierr = MatCreate(PETSC_COMM_WORLD,&mat);RCHKERRQ(ierr);
  ierr = MatSetType(mat, MATMPIAIJ);RCHKERRQ(ierr);
  
  ierr = MatSetSizes(mat,PETSC_DECIDE,PETSC_DECIDE,m,n);RCHKERRQ(ierr);
  ierr = MatSetFromOptions(mat);RCHKERRQ(ierr);
  ierr = MatSetUp(mat);RCHKERRQ(ierr);
  ierr = MatGetOwnershipRange(mat,&rstart,&rend);RCHKERRQ(ierr);
  for (i=rstart; i<rend; i+=2) {
    for (j=0; j<n; j+=2) {
      v    = 10.0*i+j;
      ierr = MatSetValues(mat,1,&i,1,&j,&v,INSERT_VALUES);RCHKERRQ(ierr);
    }
  }
  ierr = MatAssemblyBegin(mat,MAT_FINAL_ASSEMBLY);RCHKERRQ(ierr);
  ierr = MatAssemblyEnd(mat,MAT_FINAL_ASSEMBLY);RCHKERRQ(ierr);
  
  
  
  
  
  ierr  = MatView(mat,PETSC_VIEWER_STDOUT_WORLD);RCHKERRQ(ierr);
  
  
  
  ierr = MatGetInfo(mat, MAT_LOCAL, &info);RCHKERRQ(ierr);
  
  
  
  
  
  MatGetSize(mat, &ma, &na);
  printf("%d  %d\n", ma, na);
  MatGetLocalSize(mat, &m, &n);
  printf("%d  %d\n", ma, na); 
  
  
  
  
  
  
  
  
  
  /* Free data structures */
  if (mat)  {ierr = MatDestroy(&mat);RCHKERRQ(ierr);}

/*  ierr = PetscFinalize();*/
  
  
  UNPROTECT(1);
  return ret;
}




/*MatGetSize(mat, &ma, &na);*/
/*printf("%d  %d\n", ma, na);*/
/*MatGetLocalSize(mat, &m, &n);*/
/*printf("%d  %d\n", ma, na); */


#endif


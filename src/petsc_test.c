#include "sbase.h"
#include <petscmat.h>

#include <stdlib.h>


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







// test
SEXP sbase_petsc_test()
{
  SEXP dense_mat;
  
  
  PetscInt ma, na;
  MatInfo info;
  Mat            mat;
  MatScalar      *a;
  PetscInt       m = 7,n,i,j,rstart,rend,rect = 0;
  PetscErrorCode ierr;
  PetscMPIInt    size, rank;
  PetscBool      flg;
  
  PetscScalar    v, alpha;
  
  PetscViewer       fd;                        /* viewer */
  char              file[PETSC_MAX_PATH_LEN];  /* input file name */
  PetscInt          row,ncols,nrows,nnzA=0,nnzAsp=0;
  const PetscInt    *cols;
  const PetscScalar *vals;
  PetscReal         norm,percent,val,dtol=1.e-16;
  MatInfo           matinfo;
  PetscInt          Dnnz,Onnz;
  
  
  
  
  
  
  ierr = PetscOptionsGetInt(NULL,"-m",&m,NULL);CHKERRQ(ierr);
  
  ierr = MPI_Comm_rank(PETSC_COMM_WORLD,&rank);CHKERRQ(ierr);
  ierr = MPI_Comm_size(PETSC_COMM_WORLD,&size);CHKERRQ(ierr);
  
  printf("%d\n", size);
  
  n = m;
  
  ierr = PetscOptionsHasName(NULL,"-rectA",&flg);CHKERRQ(ierr);
  if (flg) {n += 2; rect = 1;}
  ierr = PetscOptionsHasName(NULL,"-rectB",&flg);CHKERRQ(ierr);
  if (flg) {n -= 2; rect = 1;}
  
  
  /* ------- Assemble matrix, test MatValid() --------- */
  ierr = MatCreate(PETSC_COMM_WORLD,&mat);CHKERRQ(ierr);
  ierr = MatSetType(mat, MATMPIAIJ);CHKERRQ(ierr);
  
  ierr = MatSetSizes(mat,PETSC_DECIDE,PETSC_DECIDE,m,n);CHKERRQ(ierr);
  ierr = MatSetFromOptions(mat);CHKERRQ(ierr);
  ierr = MatSetUp(mat);CHKERRQ(ierr);
  ierr = MatGetOwnershipRange(mat,&rstart,&rend);CHKERRQ(ierr);
  for (i=rstart; i<rend; i+=2) {
    for (j=0; j<n; j+=2) {
      v    = 10.0*i+j;
      ierr = MatSetValues(mat,1,&i,1,&j,&v,INSERT_VALUES);CHKERRQ(ierr);
    }
  }
  ierr = MatAssemblyBegin(mat,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  ierr = MatAssemblyEnd(mat,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  
  
  
  
  // Print matrix with petsc printer
/*  ierr  = MatView(mat,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);*/
  
  
  
  ierr = MatGetInfo(mat, MAT_LOCAL, &info);CHKERRQ(ierr);
  
  
  
  
  
/*  MatGetSize(mat, &ma, &na);*/
/*  printf("%d  %d\n", ma, na);*/
/*  MatGetLocalSize(mat, &ma, &na);*/
/*  printf("%d  %d\n", ma, na); */
  
  
  
/*  ierr  = MatGetOwnershipRange(mat,&rstart,&rend);CHKERRQ(ierr);*/
  
/*  printf("%d  %d\n", rstart, rend);*/
  
  
  dense_mat = sbase_convert_petsc_to_r(mat);
  
  
  
  /* Free data structures */
  if (mat)  {ierr = MatDestroy(&mat);CHKERRQ(ierr);}

/*  ierr = PetscFinalize();*/
  
  
  UNPROTECT(1);
  return dense_mat;
}




/*MatGetSize(mat, &ma, &na);*/
/*printf("%d  %d\n", ma, na);*/
/*MatGetLocalSize(mat, &m, &n);*/
/*printf("%d  %d\n", ma, na); */








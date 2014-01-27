#include "sbase.h"
#include <stdlib.h>


SEXP sbase_petsc_test_rmat(SEXP dim, SEXP ldim, SEXP data, SEXP row_ptr, SEXP col_ind)
{
  SEXP rmat;
  Mat mat;
  PetscErrorCode ierr;
  
  
  // build petsc matrix
  sbase_convert_r_to_petsc(dim, ldim, data, row_ptr, col_ind);
  
  // Print matrix with petsc printer
  ierr  = MatView(mat,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
  
  // Recreate R matrix
  rmat = sbase_convert_petsc_to_r(mat);
  
  // destroy petsc matrix
  if (mat)  {ierr = MatDestroy(&mat);CHKERRQ(ierr);}
  
  return rmat;
}















// test
#if 0
SEXP sbase_petsc_test()
{
  SEXP dense_mat;
  
  
  PetscInt ma, na;
  MatInfo info;
  Mat            mat;
  MatScalar      *a;
  PetscInt       m,n,i,j,rstart,rend,rect = 0;
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
  
  m = 8;
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
  for (i=rstart; i<rend; i+=2) 
  {
    for (j=0; j<n; j+=2) 
    {
      v    = 10.0*i+j;
      ierr = MatSetValues(mat,1,&i,1,&j,&v,INSERT_VALUES);CHKERRQ(ierr);
    }
  }
  ierr = MatAssemblyBegin(mat,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  ierr = MatAssemblyEnd(mat,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  
  
  
  
  // Print matrix with petsc printer
  ierr  = MatView(mat,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
  
  
  
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
  
  return dense_mat;
}
#endif



/*MatGetSize(mat, &ma, &na);*/
/*printf("%d  %d\n", ma, na);*/
/*MatGetLocalSize(mat, &m, &n);*/
/*printf("%d  %d\n", ma, na); */








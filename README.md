# pbdSBASE

pbdSBASE is a set of low-level PETSc bindings and utility functions for 
doing distributed sparse matrix computations in R.  For higher-level
access, see the pbdDMAT package.

Sparse matrices should be distributed by row, and locally CSR
storage.  The pbdDMAT package has numerous utilities for managing 
this and other matrix distributions.



## Installation

At this time, installation is non-trivial.  The basic steps are:

1. Build PETSc with `mpicc -fPIC`.  This requires the use of PETSc's
`PETSC_HOME` and `PETSC_ARCH` environment variables.  See official
PETSc documentation for details.
2. Assuming `PETSC_HOME` and `PETSC_ARCH` are set, build pbdSBASE via:

```
R CMD INSTALL sbase --configure-args="--with-petsc-home=${PETSC_HOME} --with-petsc-arch=${PETSC_ARCH}"
```

3. To run a program using pbdSBASE, make sure that you have 
`${PETSC_HOME}/${PETSC_ARCH}/lib` in your `LD_LIBRARY_PATH`.

4. Realistically, you should also have pbdDMAT built as well.

We are developing an experimental analogue of pbdSLAP for PETSc called
pbdSSLAP, but it is not ready at this time.



## Example

This simple example computes a crossproduct of a sparse (represented)
matrix.  


```r
library(pbdSBASE, quietly=TRUE)
library(pbdDMAT, quietly=TRUE)
init.grid()
petsc_init()

n <- 10
p <- 3


x <- matrix(1:30, 10)

dx <- as.dmat(x)
dsx <- as.dsmatrix(dx)
dsy <- t(dsx) %*% dsx
print(dsy)
y <- as.matrix(dsy)
comm.print(y)

petsc_finalize()
finalize()
```

Save this example as `sbase_example.r` and run it from the terminal
via:

```
mpirun -np 2 Rscript sbase_example.r
```



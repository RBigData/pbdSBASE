library(pbdSBASE, quietly=TRUE)
library(pbdDMAT, quietly=TRUE)
init()
petsc_init()


m <- 9
n <- 8
x <- matrix(c(
              rep(0, n),
              1, 0, 2, 0, 4, 0, 6, 0, 
              rep(0, n), 
              20, 0, 22, 0, 24, 0, 26, 0, 
              rep(0, n), 
              40, 0, 42, 0, 44, 0, 46, 0, 
              rep(0, n), 
              60, 0, 62, 0, 64, 0, 66, 0, 
              rep(0, n)), 
            m, n, byrow=T)


comm.print(x)

petsc_matprinter("matlab")

l <- pbdDMAT:::convert_dense_to_csr(x)
petsc_test_rmat(dim(x), dim(x), l$Data, l$row_ptr, l$col_ind)


petsc_finalize()


finalize()

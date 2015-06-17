library(pbdSBASE, quietly=TRUE)
library(pbdDMAT, quietly=TRUE)
petsc_init()

x <- matrix(1:30, 10)

dx <- as.dmat(x)
dsx <- as.dsmatrix(dx)
dsy <- t(dsx) %*% dsx

print(dsy)
print(dsy, all=TRUE, fmt="matlab")

petsc_finalize()
finalize()


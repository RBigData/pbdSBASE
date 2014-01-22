library(pbdSBASE, quietly=TRUE)
init()


petsc_init()
petsc_matprinter("matlab")
petsc_test()
petsc_finalize()


finalize()

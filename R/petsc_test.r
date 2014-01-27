petsc_test <- function()
{
  out <- .Call("sbase_petsc_test", PACKAGE="pbdSBASE")
  
  return( out )
}


petsc_test_rmat <- function(dim, ldim, data, row_ptr, col_ind)
{
  storage.mode(dim) <- "integer"
  storage.mode(ldim) <- "integer"
  storage.mode(data) <- "double"
  storage.mode(row_ptr) <- "integer"
  storage.mode(col_ind) <- "integer"
  
  out <- .Call("sbase_petsc_test_rmat", dim, ldim, data, row_ptr, col_ind, PACKAGE="pbdSBASE")
  
  return( out )
}


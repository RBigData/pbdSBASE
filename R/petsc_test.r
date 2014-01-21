petsc_test <- function()
{
  out <- .Call("sbase_petsc_test", PACKAGE="pbdSBASE")
  
  return( out )
}


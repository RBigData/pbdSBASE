petsc_init <- function()
{
  invisible( .Call("sbase_petsc_init", PACKAGE="pbdSBASE") )
}

petsc_finalize <- function()
{
  invisible( .Call("sbase_petsc_finalize", PACKAGE="pbdSBASE") )
}

petsc_end <- function()
{
  invisible( .Call("sbase_petsc_end", PACKAGE="pbdSBASE") )
}


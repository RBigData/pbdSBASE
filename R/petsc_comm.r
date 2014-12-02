petsc_init <- function()
{
  invisible(.Call(sbase_petsc_init))
}



petsc_finalize <- function()
{
  invisible(.Call(sbase_petsc_finalize))
}



petsc_end <- function()
{
  invisible(.Call(sbase_petsc_end))
}


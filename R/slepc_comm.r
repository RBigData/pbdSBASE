slepc_init <- function()
{
  invisible( .Call("sbase_slepc_init", PACKAGE="pbdSBASE") )
}



slepc_finalize <- function()
{
  invisible( .Call("sbase_slepc_finalize", PACKAGE="pbdSBASE") )
}

